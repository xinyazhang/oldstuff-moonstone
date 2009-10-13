#include "DomainImpl.h"
#include "RefCounter.h"

void SQLiteTagDeleter::operator(SQLiteTag* tag)
{
	SQLiteRefCounter* rc = tag->rc_;
	index_t idx = tag->idx();
	rc->destroy_tag(idx);
}
