#include "factory.h"
#include "fdpool_win32.h"
#include "tasklet_queue_t.h"
#include "volpreplet_win32.h"
#include "watching_win32.h"

namespace factory {
	class fdpool_t* fdpool() {
		return new fdpool_win32;
	}

	class tasklet_queue_t* tasklet_queue() {
		return new tasklet_queue_t(1);
	}

	class tasklet_t* volpreplet(int evid, 
			const struct volume& vol, 
			class fdpool_t* pool,
			class Database* db) {
		return new volpreplet_win32(evid, vol, pool, db);
	}

	class watching_t* watching(const struct volume& vol, class Database* db) {
		return new watching_win32(vol, db);
	}
};
