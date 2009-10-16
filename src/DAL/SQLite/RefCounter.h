#ifndef _REMUS_DAL_SQLITE_REF_COUNTER_
#define _REMUS_DAL_SQLITE_REF_COUNTER_

#include <map>
#include "DomainImpl.h"
#include <boost/weak_ptr.hpp>

struct sqlite3;

template<class T>
class SQLiteRefCounter
{
public:
	typedef boost::shared_ptr<T> Ref;
	typedef boost::weak_ptr<T> Weak;

	SQLiteRefCounter(sqlite3* db);

	Ref create(index_t idx);
	void destroy(index_t idx);
	void replace(Ref ref);
private:
	typedef typename std::map<index_t, Weak>::iterator MapIter;
	std::map<index_t, Weak> map_;
	sqlite3* db_;
};

#include "RefCounter.cpp"

#endif
