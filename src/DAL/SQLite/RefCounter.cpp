/*
 * WARNING!
 * This a .cpp file but included in a .h
 * Seperating declare and define to get a clearer interface .h
 */

template<class T>
SQLiteRefCounter<T>::Ref SQLiteRefCounter<T>::create(index_t idx)
{
	MapIter iter = map_.find(idx);
	Ref ref;

	if ( weak == map_.end() )
	{
		// not found, create new memory object;
		ref = Ref(new T(this), typename T::Deleter());
		map_[idx] = ref;
	} else
		ref = Ref(weak->second);

	return ref;
}

template<class T>
SQLiteRefCounter<T>::Ref SQLiteRefCounter<T>::destroy(index_t idx)
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
SQLiteRefCounter<T>::Ref SQLiteRefCounter<T>::replace(SQLiteRefCounter<T>::Ref ref)
{
	index_t idx = ref->idx();
	map[idx]->set_idx(0);
	map[idx] = Weak(ref);
	map[idx]->set_idx(idx);
}
