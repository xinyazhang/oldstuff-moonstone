#ifndef LAIN_GUI_KNOWLEDGE_BASE_FILE_H
#define LAIN_GUI_KNOWLEDGE_BASE_FILE_H

#include "KBViewItem.h"
#include <kernel/fso.h>

class KBViewFile
	:public KBViewItem
{
public:
	KBViewFile(Database* ,idx_t, KBViewItem* parent);

	virtual QVariant col_data(Database*, int ) const;
	virtual int children_count(Database* ) const;
	virtual void reload(Database* );

protected:
	virtual void create_child(Database* db, int index);

private:
	idx_t idx_;
	fso_t cache_;
};

#endif
