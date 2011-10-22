#include <vector>
#include <deque>
#include <algorithm>
#include <pal/volume.h>
#include <pal/native_event.h>
#include "index_engine.h"
#include <boost/thread.hpp>
#include <boost/thread/locks.hpp>
#include <boost/thread/mutex.hpp>
#include "watching.h"

typedef boost::mutex::scoped_lock scoped_lock;

class indexer_t
{
public:
	typedef std::vector<watching_t> watching_container;
private:
	/* worker */
	Database* db_mgr;
	boost::thread *thread;
	volatile bool quiting, changing;
	native_fd interrupter, change_done;
	mutable boost::mutex lock_on_change;
	watching_container watching;
	std::vector<native_fd> fd_array;

	/* Manager */
	boost::thread* manager;
	mutable boost::mutex lock_on_queue;
	boost::condition_variable mgr_cond;
	enum CHANGE_TYPE {
		ADD,
		REMOVE,
		QUIT
	};
	struct watching_change
	{
		CHANGE_TYPE ct;
		volume vol;
	};
	std::deque<watching_change> queued;
public:
	indexer_t(Database* mgr)
		:db_mgr(mgr), quiting(false), changing(false)
	{
		interrupter = create_event(false);
		change_done = create_event(false);
		fd_array.push_back(interrupter);
		create_threads();
	}

	~indexer_t()
	{
		close_threads();
		for(watching_container::iterator iter = watching.begin(); 
				iter != watching.end();
				iter++)
		{
			watching_t::close(*iter);
		}
		close_event(interrupter);
		close_event(change_done);
	}

	void create_threads()
	{
		thread = new boost::thread(thecall, this);
		manager = new boost::thread(thecall2, this);
	}

	void close_threads()
	{
		lock_on_queue.lock();
		watching_change wc = {QUIT, volume()};
		lock_on_queue.unlock();
		mgr_cond.notify_one();
		manager->join();

		quiting = true;
		flag_event(interrupter);
		flag_event(change_done);
		thread->join();
	}

	bool queue_add(const struct volume& vol)
	{
		watching_change wc = {ADD, vol};
		scoped_lock l(lock_on_queue);
		queued.push_back(wc);
		mgr_cond.notify_one();

		return true;
	}

	bool queue_remove(const struct volume& vol)
	{
		watching_change wc = {REMOVE, vol};
		scoped_lock l(lock_on_queue);
		queued.push_back(wc);
		mgr_cond.notify_one();

		return true;
	}

	std::vector<volume> indexing_volumes() const
	{
		std::vector<volume> ret;
		scoped_lock l(lock_on_change);
		for(watching_container::const_iterator iter = watching.begin(); 
				iter != watching.end();
				iter++)
		{
			ret.push_back(iter->vol);
		}
		return ret;
	}

	void mgr()
	{
		while (true) {
			scoped_lock lk(lock_on_queue);
			while (queued.empty()) {
				mgr_cond.wait(lk);
			}
			watching_change work = queued.front();
			queued.pop_front();
			lk.unlock();

			if (work.ct == ADD)
				add_volume(work.vol);
			else if (work.ct == REMOVE)
				remove_volume(work.vol);
			else
				return ;
		}
	}

	/*
	 * NOTE: process the volume array immediately, we need the return value
	 */
	bool add_volume(const struct volume& vol)
	{
		changing = true;
		flag_event(interrupter);

		watching_container::iterator iter = 
			std::find(watching.begin(), watching.end(), vol);

		if (iter != watching.end())
			return false; // already watching

		/*
		 * Add watching
		 */
		watching_t watchnew = watching_t::create(db_mgr, vol); // would load data from db and call IPC.
		if (!watchnew.check())
			return false;
		else
			watchnew.init(db_mgr);

		boost::lock_guard<boost::mutex> lg(lock_on_change); // Edit watching
		watching.push_back(watchnew);
		// OK for win32 but must be changed in Linux
		fd_array.push_back(watchnew.waitobj());

		changing = false;
		flag_event(change_done);
		return true;
	}

	bool remove_volume(const struct volume& vol)
	{
		changing = true;
		flag_event(interrupter);

		watching_container::iterator iter = 
			std::find(watching.begin(), watching.end(), vol);

		if (iter == watching.end())
			return false; // didn't exist

		boost::lock_guard<boost::mutex> lg(lock_on_change);
		watching_t::close(*iter);
		watching.erase(iter);
		fd_array.clear();
		fd_array.push_back(interrupter);
		for(watching_container::iterator iter = watching.begin();
				iter != watching.end();
				iter++)
			fd_array.push_back(iter->waitobj());

		changing = false;
		flag_event(change_done);
		return true;
	}
	
	int wait_array()
	{
		DWORD idx = WaitForMultipleObjects(fd_array.size(),
				&fd_array.front(),
				FALSE,
				INFINITE);
		return (int)(idx - WAIT_OBJECT_0);
	}

	void run()
	{
		do {
			wait_event(change_done); // Initially idle, No change, No work
		} while (!watching.empty());

		/*
		 * Waiting and dump into database
		 */
		while (!quiting) {
			lock_on_change.lock();
			int idx = wait_array();
			if (idx == 0) {
				if (quiting) {
					lock_on_change.unlock();
					return ;
				} else if (changing) {
					lock_on_change.unlock();
					wait_event(change_done);
				}
			} else {
				watching[idx - 1].dump(db_mgr);
				lock_on_change.unlock();
			}
		}
	}

private:
	static void thecall(indexer_t* obj)
	{
		obj->run();
	}
	static void thecall2(indexer_t* obj)
	{
		obj->mgr();
	}
};

index_engine_t::index_engine_t(Database* db_mgr)
{
	indexer_ = new indexer_t(db_mgr);
}

index_engine_t::~index_engine_t()
{
	delete indexer_;
}

bool index_engine_t::queue_volume(const struct volume& vol)
{
	return indexer_->queue_add(vol);
}

bool index_engine_t::remove_volume(const struct volume& vol)
{
	return indexer_->queue_remove(vol);
}

std::vector<volume> index_engine_t::volume_list() const
{
	return indexer_->indexing_volumes();
}
