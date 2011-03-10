#ifndef LAIN_GUI_KNOWLEDGE_BASE_ITEM
#define LAIN_GUI_KNOWLEDGE_BASE_ITEM

#include <boost/shared_ptr.hpp>
#include <QtCore/QVariant>
#include <kernel/common_declare.h>

using boost::shared_ptr;

enum KBViewItemType
{
	KB_tag_item,
	KB_file_item,
	KB_tag_root,
	KB_file_root,
	KB_maximun_item
};

class KBViewItem
{
public:
	KBViewItem(KBViewItem* parent); // Notice: One DB Obj may be vieweb by multiple KBViewItem 
	virtual ~KBViewItem();

	/*
	 * Shared among KBViewItem classes
	 */
	QVariant header(int ) const;
	int header_count() const;
	int row() const;
	KBViewItem* parent();
	// create it when first access
	// don't check range! -- it has been made by KBModel
	KBViewItem* child(Database*, int index);

	/* Type depedent */
	virtual QVariant col_data(Database*, int ) const = 0;
	virtual int children_count(Database* ) const = 0;
	// Reload from database
	virtual void reload(Database* ) = 0;

	/*
	 * create and return the root
	 * NOTE: the pointer MUST CAN be deleted by using delete.
	 */
	static KBViewItem* RootFactory(Database*, KBViewItemType); 

protected:
	virtual void create_child(Database* db, int index) = 0;

	KBViewItem* parent_;
	std::vector<boost::shared_ptr<KBViewItem> > children_;
	int get_child_index(const KBViewItem*) const;
private:
	KBViewItem();
};

#endif
