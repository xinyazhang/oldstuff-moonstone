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
		if (ensure_dir(::abs_fullpath(path)) < 0)
			return false;
		recursive = false;
	}

	if ( recursive )
		return add_recursive(::abs_fullpath(path), rootfso);
	return true;
}

/*
 * In fact, withdraw shouldn't be placed here as it has nothing to do with FS
 * It's just a facade
 */
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
	} else
		return fsodb_->rm(pathid);
}

bool snapshotter::add_recursive(const unistr& path, idx_t rootfso)
{
	if ( path.empty() )
		return false;

	dir_handle handle = ::open_dir(path.native());
	fso_t fso;

	for(dir_entry_data entry = dir_buf(handle);
			valid_dir_entry(entry);
			entry = dir_next(handle))
	{
		::fso_read_entry(fso, entry);
		idx_t next = fsodb_->ensure(fso, rootfso);
		if ( fso.is_dir_ )
			add_recursive(path + fso.name(), next);
	}
	fsodb_->maintain_mtimer(rootfso);

	return true;
}

/*
 * It's safe to only index a directory name. Because:
 * 	- ensure_dir is called by function add, a function used by interface
 * 	- if user hadn't ``add'' the path (or a part), we also needn't to index it
 * 	- if user had ``add'' the whole path, the ensure_dir wouldn't invoke a
 * 	 fso_t::ensure to add a directory.
 */
idx_t snapshotter::ensure_dir(const unistr& path)
{
	if ( path.empty() )
		return -1;

	db_->begin_transaction();
	vector<unistr> path_list = split_path(path);

	idx_t parentid = 0;
	for(size_t i = 0; i < path_list.size(); i++)
	{
		parentid = fsodb_->ensure(path_list[i], parentid);
	}
	db_->final_transaction();
	return parentid;
}

