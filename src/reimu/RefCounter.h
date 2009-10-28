#ifndef _REMUS_DAL_SQLITE_REF_COUNTER_
#define _REMUS_DAL_SQLITE_REF_COUNTER_

#include <map>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include "../core/int_type.h"
#include "CommonDeleter.h"

class Category;
class Database;

template<class T>
class RefCounter
{
	friend class Category;
public:
	typedef boost::shared_ptr< RefCounter<T> > RCRef;
	typedef boost::shared_ptr<T> Ref;
	typedef boost::weak_ptr<T> Weak;

	RefCounter(Category* cat, Database* db)
		:cat_(cat), db_(db), mem_counter_(0) { }

	Ref create(index_t idx);
	void destroy(index_t idx, T*);
	void move(index_t old_idx, index_t new_idx);
private:
	typedef typename std::map<index_t, Weak>::iterator MapIter;
	std::map<index_t, Weak> map_;
	Category* cat_;
	Database* db_;
	boost::weak_ptr<RefCounter> self_;

	index_t mem_counter_;
};

//template <typename T> typedef  boost::shared_ptr< RefCounter<T> > RefCounterRef<T>;
#include "RefCounter.cpp"

#endif
