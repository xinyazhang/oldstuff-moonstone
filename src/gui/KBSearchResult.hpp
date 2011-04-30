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
	KBSearchResult():KBViewItem(NULL) {}

	void init(Database* db, const unistr_list& ul)
	{
		ul_ = ul;
		reload(db);
	}

	virtual QVariant col_data(Database*, int idx) const
	{
		if (idx == 0)
			return QString("Searching results");
		return QString("Attributes");
	}

	virtual int children_count(Database* ) const
	{
		return (int)list_.size();
	}

	virtual void reload(Database* db)
	{
		if ( ul_.empty() )
		{
			list_ = T::all(db);
		} else
		{
			list_ = T::select(db, ul_);
		}
		children_.resize(children_count(db));
	}

	static KBSearchResult<T>* Factory(Database* db, const unistr_list& ul)
	{
		KBSearchResult<T>* ret = new KBSearchResult<T>;
		ret->init(db, ul);
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
	unistr_list ul_;
};

#endif
