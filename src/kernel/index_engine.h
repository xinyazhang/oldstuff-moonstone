#ifndef INDEX_ENGINE_H
#define INDEX_ENGINE_H

#include "notifier.h"

/*
 * Currently: win32+NTFS only
 */

class index_engine_t
	:public notifier
{
public:
	index_engine_t(Database* db_mgr);
	~index_engine_t();

	bool queue_volume(const struct volume&);
	bool remove_volume(const struct volume&);
private:
	class indexer_t* indexer_;
};

#endif
