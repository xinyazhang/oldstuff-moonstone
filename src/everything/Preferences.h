#ifndef PREFERENCES_H
#define PREFERENCES_H

#include <kernel/search_engine.h>
#include <moonstone/partition.h>

class Preferences
{
public:
	/*
	 * Objects in various modules
	 */
	class DatabaseInterface* db_engine;
	class Database* db_mgr;
	class search_engine_t* search_engine;
	class index_engine_t* indexer;
	
	/*
	 * Preferences
	 */
	partition_list known_partitions;
	
	/*
	 * Destructor
	 */
	~Preferences(); /* Singletion */

	static Preferences* pref();
	/*
	 * Constructor
	 */
private:
	Preferences();
};

inline Preferences* pref() { return Preferences::pref(); } 

#endif // PREFERENCES_H
