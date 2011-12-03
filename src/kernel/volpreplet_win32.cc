#include "volpreplet_win32.h"

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
	stat::watching_vol_lock.lock();
	watching_container::iterator iter = 
		std::find(stat::watching_volumes.begin(), stat::watching_volumes.end(), vol_);

	if (evid() == VOLFDPREP_ADD) {
		if (iter != stat::watching_volumes.end())
			return -1; // already watching

		watching_t* watch = factory::watching(vol_, db_);
		if (!watch->check()) {
			return -1;
		}
		watch->init();
		watch->dispach_read();

		stat::watching_volumes.push_back(watch);
		pool_->async_attach(watch);
		stat::append(stat::watching_vol_content, vol_, stat::watching_vol_lock);
	} else (evid() == VOLFDPREP_RMV) {
		if (iter == stat::watching_volumes.end())
			return -1;
		watching_t* watch = *iter;
		stat::watching_volumes.erase(iter);
		delete watch;
		stat::remove(stat::watching_vol_content, vol_, stat::watching_vol_lock);
	}
	stat::watching_vol_lock.unlock();
	return 0;
}

