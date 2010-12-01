#include "snapshotter.h"
#include "common.h"
#include <pal/path.hpp>
#include <vector>

using std::vector;

snapshotter::snapshotter(Database* db)
	:db_(db), fsodb_(db->fsodbman())
{
}

snapshotter::~snapshotter()
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

	if ( ::is_dir(path) )
	{
		rootfso = ensure_dir(path);
		if ( rootfso < 0 )
			return false;
	} else
	{
		ensure_dir(::abs_fullpath(path));
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
		//err_ = NOTEXIST;
		return false;
	}

	if ( fsodb_->haschild(pathid) && !(flag & recursive_flag) )
	{
		// have content
		//err_ = NOTEMPTY;
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
	if ( path.empty() )
		return false;

	vector<unistr> dir_content = ::ls(path);
	for(vector<unistr>::iterator iter = dir_content.begin();
			iter != dir_content.end();
			iter++)
	{
		idx_t next = fsodb_->ensure(*iter, rootfso);
		if ( ::is_dir(*iter) )
		{
			add_recursive(::abs_fullpath(*iter), next);
		}
	}
	return true;
}

idx_t snapshotter::ensure_dir(const unistr& path)
{
	if ( path.empty() )
		return -1;
	db_->begin_transaction();
	vector<unistr> path_list = split_path(path);

	idx_t parentid = 0;
	for(int i = path_list.size() - 1; i >= 0; i--)
	{
		parentid = fsodb_->ensure(path_list[i], parentid);
	}
	db_->final_transaction();
	return parentid;
}

