#include "Preferences.h"
#include <kernel/search_engine.h>
#include <kernel/index_engine.h>
#include <kernel/Database.h>
#include <kernel/filemgr.h>
#include <kernel/volmgr.h>
#include <kernel/search_engine.h>
#include <kernel/dal/supported_db.h>

Preferences* Preferences::pref()
{
    static Preferences pref;
    return &pref;
}

Preferences::Preferences()
{
	/*
	 * Fixed function
	 */
	db_engine = new db_sqlite_impl("NOWHERELAND");
	/*ASSERT*/ db_engine->connect();

	db_mgr = new Database(db_engine);
	if (!db_mgr->initialized())
		db_mgr->initialize();

	search_engine = new search_engine_t(db_mgr);

	indexer = new index_engine_t(db_mgr);
}

Preferences::~Preferences()
{
	delete indexer;
	delete search_engine;
	delete db_mgr;
	delete db_engine;
}
