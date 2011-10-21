#include "Preferences.h"
#include <kernel/common.h>
#include <kernel/search_engine.h>
#include <dal/supported_db.h>

Preferences* pref()
{
    static Preference* pref = NULL;
    if (!pref)
    {
        /* init */
		pref = Preferences::build();
    }
    return pref;
}

Preferences* Preferences::build()
{
	/*
	 * Fixed function
	 */
	Preferences* pref = new Preferences;
	pref->db_engine = new db_sqlite_impl("NOWHERELAND");

	pref->db_mgr = new Database(pref->db_engine);
	if (!db_mgr->initialized())
		pref->db_mgr->initialize();

	pref->search_engine = new search_engine(pref->db_mgr);

	pref->indexer = new index_engine_t(pref->db_mgr);

	return pref;
}
