#ifndef LAIN_GUI_KNOWLEDGE_BASE_ITEM
#define LAIN_GUI_KNOWLEDGE_BASE_ITEM

#include <kernel/common_declare.h>
#include <boost/shared_ptr.hpp>

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
	KBViewItem* parent();

	/* Type depedent */
	virtual QVariant col_data(Database*, int ) const = 0;
	virtual int children_count(Database* ) const = 0;
	// create it when first access
	// don't check range! -- it has been made by KBModel
	virtual KBViewItem* child(Database*, int index) = 0;
	// Reload from database
	virtual void reload(Database* ) = 0;

	/*
	 * create and return the root
	 * NOTE: the pointer MUST CAN be deleted by using delete.
	 */
	static KBViewItem* RootFactory(Database*, KBViewItemType); 

protected:
	KBViewItem* parent_;
	std::vector<boost::shared_ptr<KBViewItem*> > children_;
private:
	KBViewItem();
};

#endif
