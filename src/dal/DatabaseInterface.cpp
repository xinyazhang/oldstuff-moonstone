#include "DatabaseInterface.h"

/*
 * Transaction's default implementations
 * Override for more powerful transaction operations
 */
DatabaseInterface::DatabaseInterface(const unistr& prefix)
	:prefix_(prefix), initialized_(false)
{
}

void DatabaseInterface::begin_transaction()
{
	db_lock_.lock();
}

void DatabaseInterface::final_transaction()
{
	db_lock_.unlock();
}
