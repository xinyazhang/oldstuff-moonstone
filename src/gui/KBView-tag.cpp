#include "KBView-tag.h"
#include "KBView-tag-file-inter.h"
#include <kernel/common.h>

KBViewTag::KBViewTag(Database* db, idx_t idx, KBViewItem* parent)
	:KBViewItem(parent)
{
	KBViewTag::reload(db);
}


QVariant KBViewTag::col_data(Database* db, int col) const
{
	QString ret;
	if ( col == 0 )
	{
		ret = db->tnodeman()->locate(idx_)->mastername;
	}else
	{
		taglist_t tl = db->tnodeman()->names(idx_);
		for(taglist_t::const_iterator iter = tl.begin();
				iter != tl.end();
				iter++)
		{
			ret += iter->name;
			ret += ' ';
		}
	}
	return QVariant(ret);
}

KBViewItem* KBViewTag::child(Database* 0, int index)
{
#if 0 // has been done by KBModel
	if (index < 0 || index >= children_count())
	{
		return NULL;
	}
#endif

	if (!children_[index])
		touch(index);

	return children_[index].get();
}

void KBViewTag::touch(int index) //must guarntee children_[index].get() == NULL
{
	if (index == 0)
	{
		/*
		 * The first child: files tagged by this tag
		 */
		children_[index] = shared_ptr<KBViewItem>(new KBViewTFI(db, idx_, this));
	} else
	{
		/*
		 * Tags tageed by this
		 */
		children_[index] = shared_ptr<KBViewItem>(new KBViewTag(db, taggees_[index], this));
	}
}

int KBViewTag::children_count(Database*) const
{
	return taggees_.size();
}

void KBViewTag::reload(Database* db) const
{
	taggees_ = db->relman()->taggee(idx_);
	children_.clear();
	children_.resize(KBViewTag::children_count(db));
}
