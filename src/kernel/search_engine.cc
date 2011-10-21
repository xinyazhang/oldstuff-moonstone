#include "search_engine.h"
#include <boost/thread.hpp>
#include <boost/mutex.hpp>
#include <queue>
#include <algorithm>
#include "Database.h"
#include "notifier.h"

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
		thread = new boost::thread(boost::bind(&search, serv));
		front = new std::vector<line_data>;
		read = new std::vector<line_data>;
		reading = new std::vector<line_data>;

		register_callback(func, cookie);
	}
	~search_service()
	{
		thread_exit = 1;
		task_queue.push_back(unistr());
		trigger.unlock();
		thread->join();
		delete thread;
		delete front;
		delete reading;
	}

	search_engine_t::cb_func_t cb_func;

	boost::thread* thread;
	boost::condition_variable cond;
	boost::mutex lock; // protect thread_exit and task_queue
	int thread_exit;
	std::deque<unistr> task_queue;

	Database* db;
	std::vector<line_data> *front; // UI thread
	std::vector<line_data> *read; // UI, working shared
	std::vector<line_data> *reading; // working thread
	boost::mutex flip_lock; // protect read

	void flip()
	{
		boost::scoped_lock sl(flip_lock);
		std::swap(read, front);
	}
};

static sql_stmt parse_search(search_service* serv, const unistr& pat)
{
	Database* db = serv->db;
	unistr sql();
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
		where_sub += " name LIKE $";
		where_sub += unistr::number(i);
		if (i < pats.size())
			where_sub += " AND ";
	}

	sql_stmt stmt = db->create_stmt_ex(
			"SELECT (inode,volid) FROM known_files WHERE "
		   	+ where_sub);
	for(int i = 1; i <= pats.size(); i++)
		stmt.bind(i, "%"+pats[i-1]+"%");

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
			while (task_queue.empty())
				serv->cond.wait(lock);
			thread_exit = serv->thread_exit;
			search_req = task_queue.pop_back();
			task_queue.clear();
		}

		if (thread_exit)
			return ;

		/* Searching... */
		serv->reading.clear();
		sql_stmt stmt = prepare_search(serv, search_req);
		while(stmt.step())
		{
			line_data ld;
			stmt.col(1, ld.inode)
			stmt.col(2, ld.volid)
			serv->reading.push_back(ld);
		}
		serv->flip_lock.lock();
		std::swap(serv->reading, serv->read);
		serv->flip_lock.unlock();

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
}

int 
search_engine_t::line_count(class search_service* serv)
{
	return serv->front.size();
}

unistr 
search_engine_t::data(class search_service* serv, int row, int column)
{
	ld = (*serv->front)[row - 1];
	if (!ld.looked)
	{
		generate_fullpath(serv->db, ld);
		(*serv->front)[row - 1] = ld;
	}
	return ld.data[column];
}

void
generate_fullpath(Database* db, line_data& ld)
{
	sql_stmt stmt = db->create_stmt_ex(
			"SELECT name FROM known_files WHERE inode = $1");

	stmt.col(1, ld.data[0]);
	ld.data[1] = ld.data[2] = ld.data[3] = "TODO";
	ld.looked = true;
}
