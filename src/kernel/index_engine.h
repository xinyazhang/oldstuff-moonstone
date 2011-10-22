#ifndef INDEX_ENGINE_H
#define INDEX_ENGINE_H

#include "notifier.h"
#include <pal/stdtype.h>

/*
 * Currently: win32+NTFS only
 */

class EXPORT_TO_DLL index_engine_t
	:public notifier
{
public:
	index_engine_t(class Database* db_mgr);
	~index_engine_t();

	bool queue_volume(const struct volume&);
	bool remove_volume(const struct volume&);
	std::vector<volume> volume_list() const;
private:
	class indexer_t* indexer_;
};

#endif
