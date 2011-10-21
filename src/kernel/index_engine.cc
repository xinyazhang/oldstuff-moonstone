#include <vector>
#include <pal/common.h>
#include <pal/volume.h>
#include <pal/native_event.h>
#include "index_engine.h"
#include <boost/thread.hpp>
#include <boost/thread/locks.hpp>
#include <boost/mutex.hpp>
#include "watching.h"

class indexer_win32
{
public:
	typedef std::vector<volume> watching_container;
private:
	/* worker */
	Database* db_mgr;
	boost::thread *thread;
	volatile bool quiting, changing;
	native_fd interrupter, change_done;
	boost::mutex lock_on_change;
	vol_container watching;
	std::vector<native_fd> fd_array;

	/* Manager */
	boost::thread* manager;
	boost::mutex lock_on_queue;
	boost::condition_variable mgr_cond;
	enum CHANGE_TYPE {
		ADD,
		REMOVE,
		QUIT
	};
	struct watching_change
	{
		CHANGE_TYPE ct;
		uuids voluuid;
	};
	std::deque<watching_change> queued;
public:
	indexer_win32(Database* mgr)
		:db_mgr(mgr), quiting(false), changing(false)
	{
		interrupter = create_event(false);
		changer = create_event(false);
		fd_array.push_back(interrupter);
		create_threads();
	}

	~indexer_win32()
	{
		close_threads();
		for(watching_t::iterator iter = watching.begin(); 
				iter != watching.end();
				iter++)
		{
			watching_t::close(*iter);
		}
		close_event(interrupter);
		close_event(changer);
	}

	void create_threads()
	{
		thread = new boost::thread(thecall, this);
		manager = new boost::thread(thecall2, this);
	}

	void close_threads()
	{
		lock_on_queue.lock();
		wc = {QUIT, uuids()};
		lock_on_queue.unlock();
		mgr_cond.notify_one();
		manager->join();

		quiting = true;
		flag_event(interrupter);
	}

	bool queue_add(const struct volume& vol)
	{
		watching_change wc = {ADD, vol.uuid};
		boost::scoped_lock l(lock_on_queue);
		queued.push_back(wc);
		mgr_cond.notify_one();

		return true;
	}

	void queue_remove(const struct volume& vol)
	{
		watching_change wc = {REMOVE, vol.uuid};
		boost::scoped_lock l(lock_on_queue);
		queued.push_back(wc);
		mgr_cond.notify_one();

		return true;
	}

	void mgr()
	{
		while (true) {
			boost::unique_lock lk(lock_on_queue);
			while (queued.empty()) {
				mgr_cond.wait(lk);
			}
			watching_change work = queued.front();
			queued.pop_front();
			lk.unlock();

			if (work.ct == ADD)
				add_volume(work.voluuid);
			else if (work.ct == REMOVE)
				remove_volume(work.voluuid);
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
		boost::lock_guard lg(lock_on_change);

		watching_container::iterator iter = 
			std::find(watching.beign(), watching.end(), vol);

		if (iter != watching.end())
			return false; // already watching

		/*
		 * Add watching
		 */
		watching_t watchnew = watching_t::create(db_mgr, vol); // would load data from db and call IPC.
		if (!watching_t::check(watchnew))
			return false;
		else
			watchnew->init(db_mgr);

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
		boost::lock_guard lg(lock_on_change);

		watching_container::iterator iter = 
			std::find(watching.beign(), watching.end(), vol);

		if (iter == watching.end())
			return false; // didn't exist

		watching_t::close(*iter);
		watching.erase(iter);
		fd_array.clear();
		fd_array.push_back(interrupter);
		for(watching_container::iterator iter = watching.begin();
				iter != watching.end();
				iter++)
			fd_array.push_back(watching.waitobj());

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

	void dump_jounal(int watch_idx)
	{
		watching[watch_idx].dump(db_mgr);
	}

	void run()
	{
		do {
			wait_event(change_done); // Initially idle, No change, No work
		} while (!watching.empty());
		/*
		 * Waiting and dump into database
		 */
		while (true) {
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
				dump_journal(idx - 1);
				lock_on_change.unlock();
			}
		}
	}

private:
	static void thecall(indexer_win32* obj)
	{
		obj->run();
	}
	static void thecall2(indexer_win32* obj)
	{
		obj->mgr();
	}
};

index_engine_t::index_engine_t(Database* db_mgr)
{
	indexer_ = new indexer_win32(db_mgr);
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
