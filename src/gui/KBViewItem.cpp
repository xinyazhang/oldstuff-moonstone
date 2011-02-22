#include "KBViewItem.h"
#include "KBView-Tag.h"

KBViewItem* KBViewItem::RootFactory(Database* db, KBViewItemType)
{
#if 0 // TODO
	if (KB_tag_item == KBViewItemType)
	{
	}
#endif
}

KBViewItem::KBViewItem(KBViewItem* parent)
	:parent_(parent)
{
}

~KBViewItem::KBViewItem()
{
}

QString common_header[] = { tr("Item"), tr("Description") };

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

KBViewItem* KBViewItem::parent()
{
	return parent_;
}

