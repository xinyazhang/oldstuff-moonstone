#ifndef LAIN_GUI_KNOWLEDGE_BASE_TAG_FILE_INTER_H
#define LAIN_GUI_KNOWLEDGE_BASE_TAG_FILE_INTER_H

#include <kernel/kernellist.h>
#include "KBViewItem.h"

/* TFI: Tag File Inter */
class KBViewTFI
	:public KBViewItem
{
public:
	KBViewTFI(Database*, idx_t, KBViewItem* parent);

	virtual QVariant col_data(Database*, int ) const;
	virtual int children_count(Database* ) const;
	virtual void reload(Database* );

protected:
	virtual void create_child(Database* db, int index);

private:
	idx_t idx_;
	idxlist_t toplevel_;
};

#endif
