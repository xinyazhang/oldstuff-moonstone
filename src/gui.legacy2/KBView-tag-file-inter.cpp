#include <QtCore/QString>
#include <QtCore/QVariant>
#include "KBView-tag-file-inter.h"
#include "KBView-file.h"
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
		static QString value_template(QObject::tr("Click here to show files tagged with: "));
		ret = value_template;
		ret += parent_->col_data(db, 0).toString();
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
	return (int)toplevel_.size();
}

void KBViewTFI::create_child(Database* db, int index)
{
	children_[index] = 
		shared_ptr<KBViewItem>(new KBViewFile(db, toplevel_[index], this));
}

void KBViewTFI::reload(Database* db)
{
	toplevel_ = db->ftman()->tagged_file_toplevel(db->tnodeman()->anyone(idx_));
	children_.clear();
	children_.resize(children_count(db));
}

