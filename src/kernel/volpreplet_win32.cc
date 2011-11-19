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
	watching_container::iterator iter = 
		std::find(stat::watching_volumes.begin(), stat::watching_volumes.end(), vol);

	if (evid() == VOLFDPREP_ADD) {
		if (iter != stat::watching_volumes.end())
			return -1; // already watching

		watching_t* watch = factory::watching(vol, db_);
		if (!watch->check()) {
			return -1;
		}
		watch->init();
		watch->dispach_read();

		stat::watching_volumes.push_back(watch);
		fdpool_->async_attach(watch);
	} else (evid() == VOLFDPREP_RMV) {
		if (iter == stat::watching_volumes.end())
			return -1;
		watching_t* watch = *iter;
		stat::watching_volumes.erase(iter);
		delete watch;
	}
	return 0;
}

