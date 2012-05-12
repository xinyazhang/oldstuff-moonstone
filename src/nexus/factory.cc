#include "factory.h"
#include "fdpool_win32.h"
#include "tasklet_queue.h"
#include "volpreplet_win32.h"
#include "watching_win32.h"

namespace factory {
	fdpool_t* fdpool() {
		return new fdpool_win32;
	}

	tasklet_queue_t* tasklet_queue() {
		return new tasklet_queue_t(1);
	}

	tasklet_t* volpreplet(int evid, 
			const volume& vol, 
			fdpool_t* pool,
			Database* db) {
		return new volpreplet_win32(evid, vol, pool, db);
	}

	watching_t* watching(const volume& vol, class Database* db) {
		return new watching_win32(vol, db);
	}
};
