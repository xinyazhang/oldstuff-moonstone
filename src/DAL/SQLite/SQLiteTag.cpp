#include "DomainImpl.h"
#include "RefCounter.h"

void SQLiteTagDeleter::operator()(SQLiteTag* tag)
{
	SQLiteRefCounter<SQLiteTag>* rc = tag->rc_;
	index_t idx = tag->idx();
	rc->destroy(idx);
}
