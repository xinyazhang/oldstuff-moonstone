/*
 * WARNING!
 * This a .cpp file but included in a .h
 * Seperating declare and define to get a clearer interface .h
 */
#include "RefCounter.h" // this line is used for test compile
#ifdef _REFCOUNTER_IMPLEMENT_
#define _REFCOUNTER_IMPLEMENT_

template<typename T>
typename RefCounter<T>::Ref RefCounter<T>::create(index_t idx)
{
	Ref ref;
	if ( idx == 0 )
	{
		mem_counter_--;
		ref = Ref(new T(mem_counter_, cat_, db_, self_), typename T::Deleter());
		map_[mem_counter_] = ref;
	}
	MapIter iter = map_.find(idx);

	if ( iter == map_.end() )
	{
		// not found, create new memory object;
		ref = Ref(new T(idx, cat_, db_, self_), typename T::Deleter());

		ref->load();

		map_[idx] = ref;
	} else
		ref = Ref(weak->second);

	return ref;
}

template<class T>
void RefCounter<T>::destroy(index_t idx)
{
	MapIter iter = map_.find(idx);

	if ( iter == map_.end() )
	{
		// not found...
	} else
	{
		T* ptr = iter->second;
		map_.erase(iter);
		// 弄不好这就是最后一个指向这个rc的对象
		// 当delete这个对象时，这个对象的dtor会销毁指向这个rc的shared_ptr
		// 于是指向这个rc的shared_ptr会销毁这个rc对象
		// 决不能让map_的erase操作发生在delete这个iter之前！
		delete ptr;
	}
}

template<class T>
void RefCounter<T>::replace(index_t old_idx, index_t new_idx)
{
	MapIter iter = map_.find(old_idx);

	map_[new_idx] = iter->second;
	map_.erase(iter);
}
#endif
