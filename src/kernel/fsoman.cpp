#include "common.h"
#include <QtCore/QDir>
#include <QtCore/QFileInfo>
#include <QtCore/QFileInfoList>
#include "fso.h"
#include <assert.h>
#include <vector>
using std::vector;

#define FSO_COLUMN_NUMBER 8

FsoMan::FsoMan(Database* db)
	:db_(db)
{
}

bool FsoMan::add(const unistr& path, int flag)
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

vector<unistr> split_path(const unistr& path)
{
	QDir dir(path);
	std::vector<unistr> ret;
	for(; !dir.isRoot(); dir.cdUp())
		ret.push_back(dir.dirName());
#ifdef _WIN32
	ret.push_back(dir.dirName()); // we need driver name
#endif

	return ret;
}

unistr agg_path(const vector<unistr> path_list)
{
}

idx_t FsoMan::ensure_dir(const unistr& path)
{
	db_->begin_transaction();
	vector<unistr> path_list = split_path(dir);

	idx_t parentid = 0;
	for(int i = path_list.size() - 1; i >= 0; i++)
	{
		parentid = ensure_fso(path_list[i], parentid);
	}
	db_->final_transaction();
	return parentid;
}

bool FsoMan::add_fso(const unistr& path, idx_t parentid)
{
	sql_stmt stmt = db_->create_insert_stmt(Database::FsoTable, FSO_COLUMN_NUMBER);
	fso_t fso(path);
	fso.bind(stmt, parentid);

	int err = stmt.execute();
	bool success = judge_and_replace(err, err_);
	return success;
}

const char* ensure_fso_locators = 
{
	"name",
	"parentid"
};

idx_t FsoMan::ensure_fso(const unistr& path, idx_t parentid)
{
	sql_stmt stmt = db_->create_selall_stmt(Database::FsoTable, 2, ensure_fso_locators);
	stmt.bind(1, QFileInfo(path).fileName());
	stmt.bind(2, parentid);
	if ( !stmt.step() )
	{
		bool must_true = add_fso(path, parentid);
		assert(must_true == true);
		return ensure_fso(path, parentid);
	} else
	{
		idx_t ret;
		stmt.col(1, ret);
		return ret;
	}
}

bool FsoMan::add_recursive(const unistr& path, idx_t rootfso)
{
	QDir dir(path);
	QFileInfoList ls = dir.entryInfoList(QDir::AllEntries|QDir::NoDotAndDotDot);
	for(QFileInfoList::iterator iter = ls.begin();
			iter != ls.end();
			iter++)
	{
		idx_t next = ensure_fso(iter->fileName(), rootfso);
		if ( iter->isDir() )
		{
			add_recursive(iter->absoluteFilePath(), next);
		}
	}
}

int FsoMan::eno() const
{
	return err_;
}

unistr FsoMan::fullpath(idx_t fsoid)
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

idx_t FsoMan::locate(const unistr& path)
{
	vector<unistr> path_list = split_path(dir);
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

fso_t FsoMan::locate(idx_t fsoid)
{
	fso_t ret(fsoid);

	if ( reloadfso(ret) )
		return ret;
	else
		return fso_t();
}

const char* fsocd_locators =
{
	"parentid",
	"name"
};

bool FsoMan::fsocd(fso_t& fso, const unistr& name)
{
	sql_stmt stmt = db_->create_selall_stmt(Database::FsoTable,
			2,
			fsocd_locators);
	stmt.bind(1, fso.fsoid);
	stmt.bind(2, name);

	if ( stmt.step() )
	{
		fso.load(stmt);
		return true;
	} else
		return false;
}

bool FsoMan::fsocdup(fso_t& fso)
{
	sql_stmt stmt = db_->create_simsel_stmt(Database::FsoMan, "fsoid", "*");
	stmt.bind(1, fso.parentid());
	
	if ( stmt.step() )
		fso.load(stmt);
	else
		return false;
	return true;
}

bool FsoMan::reloadfso(fso_t& fso)
{
	sql_stmt stmt = db_->create_simsel_stmt(Database::FsoMan, "fsoid", "*");
	stmt.bind(1, fso.fsoid());
	
	if ( stmt.step() )
		fso.load(stmt);
	else
		return false;
	return true;
}

const char* fsocollist = 
{
	"parentid",
	"name",
	"size",
	"fs_date",
	"recusive_date",
	"hash_algo",
	"hash"
};

bool FsoMan::updatefso(fso_t& fso)
{
	sql_stmt stmt = db_->create_update_stmt(Database::FsoMan, "fsoid", FSO_COLUMN_NUMBER - 1, fsocollist);
	fso.updatebind(stmt);
	return stmt.step();
}

bool FsoMan::chroot(idx_t fsoid, const unistr& pathnew)
{
	QFileInfo info(pathnew);
	if ( !info.exists() || !info.isDir() )
		return false;
	idx_t rootnew = ensure_dir(info.absoluteFilePath());
	fso_t fso = locate(fsoid);
	fso.chparent(rootnew);
	updatefso(fso);
}
