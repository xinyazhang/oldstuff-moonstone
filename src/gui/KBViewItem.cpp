#include <QtCore/QString>
#include <QtCore/QStringList>
#include "KBViewItem.h"
#include "KBView-tag.h"
#include <kernel/common.h>

KBViewItem* KBViewItem::RootFactory(Database* db, KBViewItemType t, QStringList locators)
{
	unistr_list ul;
	for(QStringList::const_iterator iter = locators.begin();
			iter != locators.end();
			iter++)
	{
		ul.push_back(unistr(*iter));
	}

	if ( t == KB_tag_item )
	{
		return KBViewTag::RootFactory(db, ul);
	}
	return NULL;
}

KBViewItem::KBViewItem(KBViewItem* parent)
	:parent_(parent)
{
}

KBViewItem::~KBViewItem()
{
}

QString common_header[] = { QObject::tr("Item"), QObject::tr("Description") };

QVariant KBViewItem::header(int idx) const
{
	if (idx < 0 || idx >= 2)
		return QVariant();

	return common_header[idx];
}

int KBViewItem::header_count() const
{
	return 2;
}

int KBViewItem::row() const
{
	if ( parent_ )
		return parent_->get_child_index(this);
	return 0;
}

KBViewItem* KBViewItem::parent()
{
	return parent_;
}

KBViewItem* KBViewItem::child(Database* db, int index)
{
	if (!children_[index])
		create_child(db, index);

	return children_[index].get();
}

int KBViewItem::get_child_index(const KBViewItem* ptr) const
{
	for(std::vector<boost::shared_ptr<KBViewItem> >::const_iterator iter = children_.begin();
			iter != children_.end();
			iter++)
	{
		if (iter->get() == ptr)
			return iter - children_.begin();
	}
	return 0;
}
