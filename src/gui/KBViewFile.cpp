#include "KBViewFile.h"

KBViewFile::KBViewFile(Database* db,idx_t fid, KBViewItem* parent)
	:KBViewItem(parent), idx_(fid)
{
	reload(db);
}

QVariant KBViewFile::col_data(Database* db, int col) const
{
	QString ret;
	if ( col == 0 )
		ret = cache_.name_;
	else
		ret = cache_.path_;

	return ret;
}

int KBViewFile::children_count(Database* db) const
{
	return db->fsodbman()->childcound(idx_);
}

KBViewItem* KBViewFile::child(Database* db, int index)
{
	if ( !children_[index] )
	{
		idx_t counter = 0;
		FsodbMan* man = db->fsodbman();
		fso_t fso_iter = man->fsocontent(cache_);
		while(counter < index && man->fsonext(fso_iter))
			counter++;

		if (counter != index || !fso_iter.valid())
			return NULL;

		children_[index] =
			shared_ptr<KBViewItem>(new KBViewFile(db, fso_iter.idx(), this));
	}
	return children_[index].get();
}

void KBViewFile::reload(Database* db)
{
	cache_ = db->fsodbman()->locate(idx_);
	if ( cache_.valid() )
		cache_.path = db->fsodbman()->fullpath(idx_);
}

