#ifndef LAIN_GUI_KNOWLEDGE_BASE_TAG_VIEW_H
#define LAIN_GUI_KNOWLEDGE_BASE_TAG_VIEW_H

#include <pal/stdtype.h>
#include <kernel/kernellist.h>
#include "KBViewItem.h"

class KBViewTag
	:public KBViewItem
{
public:
	KBViewTag(Database*, const tnode_t&, KBViewItem* parent);

	virtual QVariant col_data(Database*, int ) const;
	virtual int children_count(Database* ) const;
	virtual void reload(Database* );

	// static KBViewTag* RootFactory(Database*, const unistr_list&); 
	virtual void append_binary_layerout(QDataStream&) const;
protected:
	void create_child(Database*, int index);

private:
	tnode_t tnode_;
	taglist_t names_;
	unistr cache_col1_;
	//tnodelist_t taggees_;
};

#endif
