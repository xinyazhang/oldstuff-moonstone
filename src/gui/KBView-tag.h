#ifndef LAIN_GUI_KNOWLEDGE_BASE_TAG_VIEW_H
#define LAIN_GUI_KNOWLEDGE_BASE_TAG_VIEW_H

#include <pal/stdtype.h>
#include "KBViewItem.h"

class KBViewTag
	:public KBViewItem
{
public:
	KBViewTag(Database*, idx_t, KBViewItem* parent);

	virtual QVariant col_data(Database*, int ) const;
	virtual int children_count(Database* ) const;
	virtual KBViewItem* child(Database*, int index);
	virtual void reload(Database* );

protected:
	void create_child(Database*, int index);

private:

	idx_t idx_;
	tnodelist_t taggees_;
};

#endif
