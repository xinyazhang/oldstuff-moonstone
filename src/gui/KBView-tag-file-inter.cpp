#include "KBView-tag-file-inter.h"
#include <kernel/common.h>

using boost::shared_ptr;

KBViewTFI::KBViewTFI(Database* db,idx_t idx, KBViewItem* parent)
	:KBViewItem(parent)
{
	reload(db);
}

QVariant KBViewTFI::col_data(Database* db, int col) const
{
	QString ret;
	if ( col == 0 )
	{
		static QString value_template(tr("Click here to show files tagged with: "));
		ret = value_template;
		ret += parent->col_data(db, 0).toString();
	} else
	{
		taglist_t tl;
		for(taglist_t::const_iterator iter = tl.begin();
				iter != tl.end();
				iter++)
		{
			ret += iter->name;
			ret += TAG_SEPERATOR;
		}
	}
	return ret;
}

int KBViewTFI::children_count(Database*) const
{
	return toplevel_.size();
}

KBViewItem* KBViewTFI::child(Database* db, int index)
{
	if ( !children_[index] )
	{
		// create it
		children_[index] = 
			shared_ptr<KBViewItem>(new KBViewFile(db, toplevel_[index], this));
	}

	return children_[index].get();
}

void KBViewTFI::reload(Database* db)
{
	toplevel_ = db->ftman()->tagged_file_toplevel(db->tagman()->anyone(idx_));
	children_.clearn();
	children_.resize(children_count(db));
}

