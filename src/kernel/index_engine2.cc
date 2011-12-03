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
#include "factory.h"
#include "tasklet_queue.h"
#include "fdpool.h"

index_engine_t::index_engine_t(Database* dbmgr)
	:dbmgr_(dbmgr)
{
	fdpool_ = factory::fdpool();
	prep_queue_ = factory::tasklet_queue();
}

index_engine_t::~index_engine_t()
{
	delete prep_queue_;
	delete fdpool_;
}

bool index_engine_t::queue_volume(const struct volume& vol)
{
	return prep_queue_->enqueue(
			factory::volpreplet(VOLFDPREP_ADD, vol, fdpool_, dbmgr_));
}

bool index_engine_t::remove_volume(const struct volume& vol)
{
	return prep_queue_->enqueue(
			factory::volpreplet(VOLFDPREP_RMV, vol, fdpool_, dbmgr_));
}

std::vector<volume> index_engine_t::volume_list() const
{
	stat::watching_vol_lock.lock();
	std::vector<volume> ret = stat::watching_volumes;
	stat::watching_vol_lock.unlock();
	return ret;
}

