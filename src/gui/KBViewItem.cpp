#include <QtCore/QString>
#include <QtCore/QStringList>
#include "KBViewItem.h"
#include "KBView-tag.h"
#include "KBSearchResult.hpp"
#include <kernel/common.h>

struct tnode_strks_meta
{
	typedef unistr_list ks_type; // Knowlegde base Searching parameter TYPE
	typedef tnode_t e_type; // Element_TYPE
	typedef KBViewTag cv_type; // Child View TYPE
	static const char* header[2];

	static bool empty_ks(const ks_type& ul)
	{
		return ul.empty();
	}

	static distinctlist_t<e_type> all(Database* db)
	{
		return db->tnodeman()->all();
	}

	static distinctlist_t<e_type> select(Database* db, const ks_type& ul)
	{
		taglist_t tl = db->tagman()->locate(ul);
		distinctlist_t<e_type> ret;

		for(taglist_t::iterator iter = tl.begin();
				iter != tl.end();
				iter++)
		{
			ret.push_back(db->tagman()->access_tnode(*iter));
		}
		return ret;
	}
};
const char* tnode_strks_meta::header[2] = {"Searching results", "Attributes"};

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
		//return KBViewTag::RootFactory(db, ul);
		return KBSearchResult<tnode_strks_meta/* list */>::Factory(db, ul);
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

void KBViewItem::append_binary_layerout(QDataStream&) const
{
}
