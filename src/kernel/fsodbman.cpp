#include "common.h"
#include "fso.h"
#include <assert.h>
#include <vector>
using std::vector;
#include <pal/path.hpp>

#define FSO_COLUMN_NUMBER 7

/*
 * Aggresive path from the list, e.g.
 * 	picture.png collection1 pic document home /
 * return:
 * 	/home/document/pic/collection1/picture.png	
 * 
 * Note: although win32's root is null, but we don't make use of it
 */

FsodbMan::FsodbMan(Database* db)
	:db_(db)
{
}

bool FsodbMan::add_fso(const unistr& name, idx_t parentid)
{
	sql_stmt stmt = db_->create_insert_stmt(Database::FsoTable, FSO_COLUMN_NUMBER);
	fso_t fso(name);
	fso.addbind(stmt, parentid);

	int err = stmt.execute();
	bool success = judge_and_replace(err, err_);
	return success;
}

const char* ensure_fso_locators[] = 
{
	"name",
	"parentid"
};

idx_t FsodbMan::ensure(const unistr& name, idx_t parentid)
{
	sql_stmt stmt = db_->create_selall_stmt(Database::FsoTable, 2, ensure_fso_locators);
	stmt.bind(1, name);
	stmt.bind(2, parentid);
	if ( !stmt.step() )
	{
		bool must_true = add_fso(name, parentid);
		assert(must_true == true);
		return ensure(name, parentid);
	} else
	{
		idx_t ret;
		stmt.col(1, ret);
		return ret;
	}
}

int FsodbMan::eno() const
{
	return err_;
}

unistr FsodbMan::fullpath(idx_t fsoid)
{
	fso_t fso = locate(fsoid);

	vector<unistr> path_list;
	while(!fso.is_root())
	{
		path_list.push_back(fso.name());
		fsocdup(fso);
		reloadfso(fso);
	}
	return agg_path(path_list);
}

idx_t FsodbMan::locate(const unistr& path)
{
	vector<unistr> path_list = split_path(path);
	fso_t fso;
	int i;
	for(i = path_list.size() - 1; i >= 0; i--)
	{
		if( !fsocd(fso, path_list[i]) )
			break;
	}
	if ( i < 0 ) // exists in db
	{
		return fso.fsoid_;
	} else
		return 0;
}

const char* locate2selector[] =
{
	"name",
	"parentid"
};

idx_t FsodbMan::locate(const unistr& name, idx_t parentid)
{
	sql_stmt stmt = db_->create_selall_stmt(Database::FsoTable, 2, locate2selector);
	stmt.bind(1, name);
	stmt.bind(2, parentid);

	if (stmt.step())
	{
		fso_t fso;
		fso.load(stmt);
		return fso.fsoid();
	} else
		return 0;
}

fso_t FsodbMan::locate(idx_t fsoid)
{
	fso_t ret(fsoid);

	if ( reloadfso(ret) )
		return ret;
	else
		return fso_t();
}

const char* fsocd_locators[] =
{
	"parentid",
	"name"
};

bool FsodbMan::fsocd(fso_t& fso, const unistr& name)
{
	if ( name == "." )
		return true;
	if ( name == ".." )
		return fsocdup(fso);

	sql_stmt stmt = db_->create_selall_stmt(Database::FsoTable,
			2,
			fsocd_locators);
	stmt.bind(1, fso.fsoid());
	stmt.bind(2, name);

	if ( stmt.step() )
	{
		fso.load(stmt);
		return true;
	} else
		return false;
}

bool FsodbMan::fsocdup(fso_t& fso)
{
	sql_stmt stmt = db_->create_simsel_stmt(Database::FsoTable, "fsoid", "*");
	stmt.bind(1, fso.parentid());
	
	if ( stmt.step() )
		fso.load(stmt);
	else
		return false;
	return true;
}

bool FsodbMan::reloadfso(fso_t& fso)
{
	sql_stmt stmt = db_->create_simsel_stmt(Database::FsoTable, "fsoid", "*");
	stmt.bind(1, fso.fsoid());
	
	if ( stmt.step() )
		fso.load(stmt);
	else
		return false;
	return true;
}

const char* fsocollist[] = 
{
	"parentid",
	"name",
	"size",
	"fs_date",
	"recusive_date",
	"hash_algo",
	"hash"
};

bool FsodbMan::updatefso(fso_t& fso)
{
	sql_stmt stmt = db_->create_update_stmt(Database::FsoTable, "fsoid", FSO_COLUMN_NUMBER, fsocollist);
	fso.updatebind(stmt);
	return stmt.step();
}

bool FsodbMan::chroot(idx_t fsoid, idx_t parentnew)
{
#if 0
	/* I think it should be commented, FsodbMan should know nothing about os file system */
	QFileInfo info(pathnew);
	if ( !info.exists() || !info.isDir() )
		return false;
#endif
	// idx_t rootnew = ensure_dir(pathnew);
	fso_t fso = locate(fsoid);
	if ( !fso.valid() )
		return false;
	fso.chparent(parentnew);
	return updatefso(fso);
}

bool FsodbMan::haschild(idx_t dirid)
{
	sql_stmt stmt = db_->create_simsel_stmt(Database::FsoTable, "parentid", "*");
	stmt.bind(1, dirid);
	return stmt.step();
}

const char* del_locators[] =
{
	"fsoid"
};

bool FsodbMan::rm(idx_t fsoid)
{
	sql_stmt stmt = db_->create_del_stmt(Database::FsoTable, 1, del_locators);
	stmt.bind(1, fsoid);

	int err = stmt.execute();
	bool success = judge_and_replace(err, err_);
	return success;
}

const char* content_restricts[] =
{
	"parentid = "
};

const char* content_orders[] =
{
	"fsoid"
};

const bool content_order_is_asc[] =
{
	true
};

fso_t FsodbMan::fsocontent(const fso_t& fso) // pickup the first child
{
	if ( !haschild( fso.fsoid() ) )
		return fso_t();
	sql_stmt stmt = db_->create_list_stmt(Database::FsoTable, 1, content_restricts,
			1, content_orders, content_order_is_asc);
	stmt.bind(1, fso.fsoid());
	if( !stmt.step() )
		return fso_t();
	fso_t ret;
	ret.load(stmt);

	return ret;
}

const char* next_restrictions[] =
{
	"parentid = ",
	"fsoid > "
};

bool FsodbMan::fsonext(fso_t& fso) 
	// next fso, used to iterate elements, use LIMIT clause for 1 result
{
	sql_stmt stmt = db_->create_list_stmt(Database::FsoTable, 2, next_restrictions,
			1, content_orders, content_order_is_asc);
	stmt.bind(1, fso.parentid());
	stmt.bind(2, fso.fsoid());
	if ( !stmt.step() )
		return false;
	else
		fso.load(stmt);
	return true;
}
