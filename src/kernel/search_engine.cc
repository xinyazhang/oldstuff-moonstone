#include "search_engine.h"
#include <boost/thread.hpp>
#include <boost/mutex.hpp>
#include <queue>
#include "Database.h"

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
{
public:
	search_service(void* cookie, cb_func_t func, Database* _db)
		:cb_cookie(cookie), cb_func(func), thread_exit(0), db(_db)
	{
		thread = new boost::thread(boost::bind(&search, serv));
	}
	~search_service()
	{
		thread_exit = 1;
		task_queue.push_back(unistr());
		trigger.unlock();
		thread->join();
		delete thread;
	}

	void* cb_cookie;
	search_engine_t::cb_func_t cb_func;

	boost::thread* thread;
	boost::condition_variable cond;
	boost::mutex lock;
	int thread_exit;
	std::deque<unistr> task_queue;

	Database* db;
	sql_stmt searching_stmt;
	uint64_t searching_row_count;
	sql_stmt reading_stmt;
	uint64_t reading_row_count;
	std::vector<line_data> buffer;
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
	/*
	 * Counting...
	 */
	sql_stmt stmt = db->create_stmt_ex(
			"SELECT COUNT(*) FROM known_files WHERE"+where_sub);
	for(int i = 1; i <= pats.size(); i++)
		stmt.bind(i, "%"+pats[i-1]+"%");
	stmt.step();
	stmt.col(1, serv->searching_row_count);

	stmt = db->create_stmt_ex(
			"SELECT (inode,volid) FROM known_files WHERE "
		   	+ where_sub);
	stmt.step();

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

		serv->searching_stmt = prepare_search(serv, search_req);
		(*(serv->cb_func))(serv->cb_cookie, SE_JOB_DONE);
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
	boost::mutex::scoped_lock lock(serv->lock);
	serv->reading_stmt = serv->searching_stmt;
	serv->reading_row_count = serv->searching_row_count;
	serv->buffer.clear();
}

int 
search_engine_t::line_count(class search_service* serv)
{
	return serv->reading_row_count;
}

unistr 
search_engine_t::data(class search_service* serv, int row, int column)
{
	line_data ld;
	while(serv->buffer.size() < row)
	{
		stmt.step();
		stmt.col(1, ld.inode);
		stmt.col(2, ld.volid);
		serv->buffer.push_back(ld);
	}
	ld = serv->buffer[row - 1];
	if (!ld.looked)
		generate_fullpath(serv->db, ld);
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
