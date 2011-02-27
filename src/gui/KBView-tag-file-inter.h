#ifndef LAIN_GUI_KNOWLEDGE_BASE_TAG_FILE_INTER_H
#define LAIN_GUI_KNOWLEDGE_BASE_TAG_FILE_INTER_H

#include "KBViewItem.h"

/* TFI: Tag File Inter */
class KBViewTFI
	:public KBViewItem
{
public:
	KBViewTFI(Database*, idx_t, KBViewItem* parent);

	virtual QVariant col_data(Database*, int ) const;
	virtual int children_count(Database* ) const;
	virtual KBViewItem* child(Database*, int index);
	virtual void reload(Database* );
private:
	idx_t idx_;
	idxlist_t toplevel_;
}

#endif
