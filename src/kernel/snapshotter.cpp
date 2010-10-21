#include "snapshotter.h"
#include "common.h"

snapshotter::snapshotter(Database* db)
	:db_(db), fsodb_(db->fsoman())
{
}

void snapshotter::redirect(FsodbMan* fsodbman)
{
	fsodb_ = fsodbman;
}

bool snapshotter::add(const unistr& path, int flag) 
	// ensure the dir's existence, and call add_recursive if flagged
{
	bool recursive = flag & recursive_flag;
	idx_t rootfso;
	QFileInfo fi(path);
	if ( fi.isDir() )
	{
		root = ensure_dir(path);
	} else
	{
		ensure_dir(fi.absolutePath());
		recursive = false;
	}
	if ( recursive )
		return add_recursive(path, rootfso);
	return true;
}

bool snapshotter::withdraw(const unistr& path, int flag)
{
	idx_t pathid = fsodb_->locate(path);
	if ( pathid < 0 )
	{
		err_ = NOTEXIST;
		return false;
	}

	if ( fsodb_->haschild(pathid) && !(flag & recursive_flag) )
	{
		// have content
		err_ = NOTEMPTY;
		return false;
	}
}

#if 0 //removed to get a cleaner design
bool snapshotter::rm(const unistr& path, int flag)
{
	idx_t pathid = fsodb_->locate(path);
	if ( pathid < 0 )
	{
		err_ = NOTEXIST;
		return false;
	}

	if ( fsodb_->haschild(pathid) && !(flag & recursive_flag) )
	{
		// have content
		err_ = NOTEMPTY;
		return false;
	}

	add_watch(path);
	db_->asyncwq()->invoke_rm(path);
}

bool snapshotter::mv(const unistr& path, const unistr& pathnew)
{
	add_watch(path);
	add_watch(pathnew);
	db_->asyncwq()->invoke_mv(path, pathnew);
	return true;
}

bool snapshotter::cp(const unistr& path, const unistr& pathnew)
{
	add_watch(path);
	add_watch(pathnew);
	AsyncWQ* asyncwq = db_->asyncwq();
}
#endif

bool snapshotter::add_recursive(const unistr& path, idx_t rootfso)
{
	QDir dir(path);
	QFileInfoList ls = dir.entryInfoList(QDir::AllEntries|QDir::NoDotAndDotDot);
	for(QFileInfoList::iterator iter = ls.begin();
			iter != ls.end();
			iter++)
	{
		idx_t next = fsodb_->ensure(iter->fileName(), rootfso);
		if ( iter->isDir() )
		{
			add_recursive(iter->absoluteFilePath(), next);
		}
	}
}

