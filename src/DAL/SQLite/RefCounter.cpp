/*
 * WARNING!
 * This a .cpp file but included in a .h
 * Seperating declare and define to get a clearer interface .h
 */
#include "RefCounter.h" // this line is used for test compile
#ifdef _REFCOUNTER_IMPLEMENT_
#define _REFCOUNTER_IMPLEMENT_

template<typename T>
typename SQLiteRefCounter<T>::Ref SQLiteRefCounter<T>::create(index_t idx)
{
	MapIter iter = map_.find(idx);
	Ref ref;

	if ( iter == map_.end() )
	{
		// not found, create new memory object;
		ref = Ref(new T(this), typename T::Deleter());

		sqlite3_stmt *stmt;
		sqlite3_prepare_u(db_, UT("select * from remus_tag where idx='"+idx+"';"), &stmt, NULL);
		result = sqlite3_step(stmt);

		sqlite3_finalize(stmt);

		map_[idx] = ref;
	} else
		ref = Ref(weak->second);

	return ref;
}

template<class T>
void SQLiteRefCounter<T>::destroy(index_t idx)
{
	MapIter iter = map_.find(idx);

	if ( iter == map_.end() )
	{
		// not found...
	} else
	{
		delete iter->second;
		map_.erase(iter);
	}
}

template<class T>
void SQLiteRefCounter<T>::replace(typename SQLiteRefCounter<T>::Ref ref)
{
	index_t idx = ref->idx();
	map[idx]->set_idx(0);
	map[idx] = Weak(ref);
	map[idx]->set_idx(idx);
}
#endif