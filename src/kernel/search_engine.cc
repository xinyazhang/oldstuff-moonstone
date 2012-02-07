#include "search_engine.h"
#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <queue>
#include <algorithm>
#include "Database.h"
#include "volmgr.h"
#include "notifier.h"
#include "sql_stmt.h"
using boost::mutex;
#include <stdio.h>
#include "feedback.h"

static void search(search_service* serv);

class line_data
{
public:
	uint64_t inode, volid;
	unistr data[4];
	bool looked;
	line_data():looked(false){}
};

static void generate_fullpath(Database* db, line_data&);

class search_service
	:public notifier
{
public:
	search_service(void* cookie, cb_func_t func, Database* _db)
		:thread_exit(0), db(_db)
	{
		thread = new boost::thread(boost::bind(&search, this));
		front = new std::vector<line_data>;
		read = new std::vector<line_data>;
		reading = new std::vector<line_data>;

		register_callback(func, cookie);
	}
	~search_service()
	{
		thread_exit = 1;
		task_queue.push_back(unistr()); // NULL task to stop waiting.
		cond.notify_all();
		if (thread)
			thread->join();
		delete thread;
		delete front;
		delete reading;
	}

	boost::thread* thread;
	boost::condition_variable cond;
	boost::mutex lock; // protect thread_exit and task_queue
	int thread_exit;
	std::vector<unistr> task_queue;

	Database* db;
	std::vector<line_data> * volatile front; // UI thread
	std::vector<line_data> * volatile read; // UI, working shared
	std::vector<line_data> * volatile reading; // working thread
	boost::mutex flip_lock; // protect read

	void flip()
	{
		mutex::scoped_lock sl(flip_lock);
		std::swap(read, front);
	}
};

static sql_stmt prepare_search(search_service* serv, const unistr& pat)
{
	Database* db = serv->db;
	unistr where_sub;
	native_sstream ss(pat);
	std::vector<unistr> pats;
	while (!ss.eof())
	{
		unistr pat_iter;
		ss >> pat_iter;
		pats.push_back(pat_iter);
	}
	for(int i = 1; i <= pats.size(); i++)
	{
		where_sub += " name LIKE '%'||$";
		where_sub += unistr::number(i);
		where_sub += "||'%'";
		if (i < pats.size())
			where_sub += " AND ";
	}

	sql_stmt stmt = db->create_stmt_ex(
			"SELECT * FROM known_dentry WHERE "
		   	+ where_sub +";");
	for(int i = 1; i <= pats.size(); i++)
		stmt.bind(i, pats[i-1]);

	return stmt;
}

static void search(search_service* serv)
{
	do
	{
		bool thread_exit;
		unistr search_req;
		{
			boost::mutex::scoped_lock lock(serv->lock);
			while (serv->task_queue.empty())
				serv->cond.wait(lock);
			thread_exit = serv->thread_exit;
			search_req = serv->task_queue.back();
			log().printf(LOG_DEBUG, UT("Task queue snapshot:\n"));
			for(std::vector<unistr>::iterator iter =
					serv->task_queue.begin();
				iter != serv->task_queue.end();
				iter++) {
				log().printf(LOG_DEBUG, UT("%s\n"), iter->c_str());
			}
			serv->task_queue.clear();
		}


		if (thread_exit)
			return ;

		/* Searching... */
		serv->reading->clear();
		sql_stmt stmt = prepare_search(serv, search_req);
		while(stmt.step())
		{
			line_data ld;
			stmt.col(1, ld.volid);
			stmt.col(2, ld.inode);
			serv->reading->push_back(ld);
		}
		serv->flip_lock.lock();
		std::swap(serv->reading, serv->read);
		serv->flip_lock.unlock();

		log().printf(LOG_DEBUG, UT("Search \"%s\" done, stored at %p\n"), search_req.c_str(), serv->read);
		serv->broadcast(SE_JOB_DONE);
		// (*(serv->cb_func))(serv->cb_cookie, SE_JOB_DONE);
	}while(true);
}

search_engine_t::search_engine_t(class Database* db)
	:db_(db)
{
}

search_service*
search_engine_t::setup_service(void* cookie, cb_func_t func)
{
	search_service* serv = new search_service(cookie, func, db_);
	return serv;
}

void 
search_engine_t::finalize_service(class search_service* serv)
{
	delete serv;
}

void 
search_engine_t::post_search(class search_service* serv, 
		const unichar* str)
{
	boost::mutex::scoped_lock lock(serv->lock);
	serv->task_queue.push_back(str);
	serv->cond.notify_one();
}

void 
search_engine_t::update_service_state(class search_service* serv)
{
	serv->flip();
	log().printf(LOG_DEBUG, UT("search engine: switch front to %p\n"), serv->front);
}

int 
search_engine_t::line_count(class search_service* serv)
{
	return serv->front->size();
}

unistr 
search_engine_t::data(class search_service* serv, int row, int column)
{
	line_data ld = (*serv->front)[row];
	if (!ld.looked)
	{
		generate_fullpath(serv->db, ld);
		(*serv->front)[row] = ld;
	}
	return ld.data[column];
}

void
generate_fullpath(Database* db, line_data& ld)
{
	sql_stmt stmt = db->create_stmt_ex(UT("SELECT name,parent FROM known_dentry WHERE inode=$1 AND volid=$2"));

	stmt.bind(1, ld.inode);
	stmt.bind(2, ld.volid);
	if (stmt.step()) {
		idx_t parent = 0;
		stmt.col(1, ld.data[0]);
		stmt.col(2, parent);

		db->begin_transaction();
		unistr cur;
		unistr_list inversed_list;
		do {
			stmt = db->create_stmt_ex(UT("SELECT name,parent FROM known_dentry WHERE inode=$ AND volid=$2"));
			stmt.bind(1, parent);
			stmt.bind(2, ld.volid);
			if (!stmt.step())
				break;
			stmt.col(1, cur);
			stmt.col(2, parent);
			inversed_list.push_back(cur);
		} while (true);
		db->final_transaction();

		ld.data[1].clear();
		ld.data[1] += db->volmgr()->hrid(ld.volid);
		if (!inversed_list.empty())
			for(unistr_list::reverse_iterator iter = inversed_list.rbegin();
				iter != inversed_list.rend();
				iter++) {
				ld.data[1] += *iter;
				ld.data[1] += UT("/");
			}
	}
	ld.data[2] = ld.data[3] = "TODO";
	ld.looked = true;
}
