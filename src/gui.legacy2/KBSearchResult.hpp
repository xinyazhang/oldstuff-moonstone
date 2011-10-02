#ifndef LAIN_GUI_KNOWLEDGE_BASE_SEARCH_RESULT_H
#define LAIN_GUI_KNOWLEDGE_BASE_SEARCH_RESULT_H

#include <kernel/common_declare.h>
#include <kernel/kernellist.h>
#include "KBViewItem.h"

template<typename T>
class KBSearchResult
	:public KBViewItem
{
public:
	KBSearchResult(KBViewItem* parent = NULL):KBViewItem(parent) {}

	void init(Database* db, const typename T::ks_type &ksp)
	{
		ksp_ = ksp;
		reload(db);
	}

	virtual QVariant col_data(Database*, int idx) const
	{
		return T::header[idx];
	}

	virtual int children_count(Database* ) const
	{
		return (int)list_.size();
	}

	virtual void reload(Database* db)
	{
		if ( T::empty_ks(ksp_) )
		{
			list_ = T::all(db);
		} else
		{
			list_ = T::select(db, ksp_);
		}
		children_.resize(children_count(db));
	}

	static KBSearchResult<T>* Factory(Database* db, const typename T::ks_type &ksp, KBViewItem* parent = NULL)
	{
		KBSearchResult<T>* ret = new KBSearchResult<T>(parent);
		ret->init(db, ksp);
		return ret;
	}

protected:
	void create_child(Database* db, int index)
	{
		children_[index] = shared_ptr<KBViewItem>(
				new typename T::cv_type(db, list_[index], this));
	}

private:
	distinctlist_t<typename T::e_type> list_;
	typename T::ks_type ksp_;
};

#endif
