#include "volpreplet.h"
#include "volpreplet_win32.h"
#include "watching_win32.h"
#include <algorithm>
#include "factory.h"
#include "fdpool.h"
#include <boost/thread.hpp>

typedef std::vector<watching_t*> watching_container;
static watching_container watchings;
static boost::mutex wacthings_lock;
static std::vector<volume> watching_volumes;

volpreplet_win32::volpreplet_win32(int evid, 
		const volume& vol, 
		class fdpool_t* pool, 
		class Database* db)
	:tasklet_t(evid), vol_(vol), pool_(pool), db_(db)
{
}

volpreplet_win32::~volpreplet_win32()
{
}

int volpreplet_win32::doit()
{
	wacthings_lock.lock();
	watching_container::iterator iter = 
		std::find(watchings.begin(), watchings.end(), vol_);

	if (evid() == VOLFDPREP_ADD) {
		if (iter != watchings.end())
			return -1; // already watching

		watching_t* watch = factory::watching(vol_, db_);
		if (!watch->check()) {
			return -1;
		}
		watch->init();

		watchings.push_back(watch);
		watching_volumes.push_back(vol_);

		pool_->async_attach(watch);
		watch->dispach_read();
	} else if (evid() == VOLFDPREP_RMV) {
		if (iter == watchings.end())
			return -1;
		watching_t* watch = *iter;
		std::vector<volume>::iterator iter_v = iter - watchings.begin() + watching_volumes.begin();
		watchings.erase(iter);
		watching_volumes.erase(iter_v);
		delete watch;
	}
	wacthings_lock.unlock();
	return 0;
}

namespace statistic {
	void get_watching_volume(std::vector<struct volume>& ret)
	{
		wacthings_lock.lock();
		ret = watching_volumes;
		wacthings_lock.unlock();
	}
};