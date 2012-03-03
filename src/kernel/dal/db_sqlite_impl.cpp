#include "db_sqlite_impl.h"
#include "stmt_sqlite_impl.h"
#include "sqlite3pal.h"
#include <boost/algorithm/string/replace.hpp>

#ifndef NDEBUG
#include <stdio.h>
#endif

db_sqlite_impl::db_sqlite_impl(const unistr& filename, const unistr& prefix)
	:filename_(filename), DatabaseInterface(prefix), handle_(NULL)
{
	check_version();
}

void db_sqlite_impl::begin_transaction()
{
	DatabaseInterface::begin_transaction();
	quick_sql_execute("BEGIN TRANSACTION;");
}

void db_sqlite_impl::final_transaction()
{
	quick_sql_execute("COMMIT;");
	DatabaseInterface::final_transaction();
}

void db_sqlite_impl::abort_transaction()
{
	quick_sql_execute("ROLLBACK;");
	DatabaseInterface::final_transaction();
}

db_sqlite_impl::~db_sqlite_impl()
{
	if ( handle_ )
		sqlite3_close(handle_);
}

bool db_sqlite_impl::connect()
{
	bool ret = SQLITE_OK == sqlite3_open_native(filename_.native(), &handle_);
	if ( ret )
		check_version();
	return ret;
}

sql_stmt_interface* db_sqlite_impl::create_stmt(const unistr& sqlraw)
{
	sqlite3_stmt *stmt;
	unistr sql = sqlraw;
	boost::replace_all(sql, UT("$"), UT("?"));
#ifndef NDEBUG
	fwprintf(stderr, L"SQL: %s\n", sql.native());
#endif
	size_t ret = sqlite3_prepare_native(handle_, sql.native(), sql.size() * sizeof(unichar), &stmt, NULL);
	if ( SQLITE_OK != ret )
	{
		return new stmt_sqlite_impl(NULL, NULL);
	} else
		return new stmt_sqlite_impl(stmt, handle_);
}

unistr db_sqlite_impl::initialize_sqls(int i) const
{
	return sqls_[i];
}

void db_sqlite_impl::check_version() // would modify initialized_
{
	unistr sql(UT("SELECT VERSION FROM $1META;"));
	// ugly patch
	//std::wstring tmp = sql.toStdWString();
	boost::replace_first(sql, UT("$1"), prefix_);
	//sql = QString::fromStdWString(tmp);
	boost::shared_ptr<sql_stmt_interface> stmt_ptr(create_stmt(sql));
	if ( stmt_ptr.get() )
	{
		if ( stmt_ptr->step() )
		{
			initialized_ = true;
		}
	}
	if ( !initialized_ )
	{
		build_sqls();
	}
}

static const unistr sql_template[] =
{
	UT("PRAGMA encoding = \"UTF-16\";"), 
	UT("CREATE TABLE IF NOT EXISTS machine_list(id INTEGER PRIMARY KEY ASC AUTOINCREMENT, name TEXT);"),
	UT("INSERT INTO machine_list(0, \"localhost\");"),
	UT("CREATE TABLE IF NOT EXISTS known_vols(id INTEGER PRIMARY KEY ASC AUTOINCREMENT, uuid TEXT UNIQUE, status INTEGER, filesystem INTEGER, vollabel TEXT);"),
	UT("CREATE TABLE IF NOT EXISTS known_dentry(volid INTEGER REFERENCES known_vols(id), inode INTEGER, parent INTEGER, name TEXT, checked INTEGER, PRIMARY KEY(volid, parent, name));"),
	UT("CREATE TABLE IF NOT EXISTS known_file(volid INTERGER REFERENCES known_vols(id), inode INTEGER, refc INTEGER, PRIMARY KEY(volid, inode));"),
	UT("CREATE TABLE IF NOT EXISTS known_ntfs(id INTEGER PRIMARY KEY REFERENCES known_vols(id), journal_id INTEGER, usn_pointer INTEGER, usn_next INTEGER);"),
		/* More items like hashing would be added later*/
#if 0
	UT("CREATE TABLE IF NOT EXISTS $PREFIX_META(version INTEGER);"),
	UT("INSERT INTO $PREFIX_META VALUES(1);"),
	UT("CREATE TABLE IF NOT EXISTS $PREFIX_tnode(idx INTEGER PRIMARY KEY ASC AUTOINCREMENT, refc INTEGER, mastername TEXT, comment TEXT);"),
	UT("INSERT INTO $PREFIX_tnode VALUES(0, 0, '', '');"),
	UT("CREATE TABLE IF NOT EXISTS $PREFIX_tag(name TEXT, tnode INTEGER REFERENCES $PREFIX_tnode(idx) ON DELETE CASCADE, PRIMARY KEY(name, tnode));"),
	UT("CREATE TABLE IF NOT EXISTS $PREFIX_tagtag_relation(tagger INTEGER REFERENCES $PREFIX_tnode(idx) ON DELETE CASCADE, taggee INTEGER REFERENCES $PREFIX_tnode(idx) ON DELETE CASCADE, PRIMARY KEY(tagger, taggee));"),
	UT("CREATE TABLE IF NOT EXISTS $PREFIX_fso(fsoid INTEGER PRIMARY KEY ASC AUTOINCREMENT, parentid INTEGER REFERENCES $PREFIX_fso(fsoid) ON DELETE CASCADE, name TEXT, size INTEGER, fs_date INTEGER, recusive_date INTEGER, hash_algo INTEGER, hash BLOB, UNIQUE(parentid, name));"),
	UT("INSERT INTO $PREFIX_fso values(0, 0, \"/\", 0, 0, 0, NULL, NULL)"),
	UT("CREATE TABLE IF NOT EXISTS $PREFIX_ft(fsoid INTERGER REFERENCES $PREFIX_fso(fsoid) ON DELETE CASCADE, tnode INTEGER REFERENCES $PREFIX_tnode(idx) ON DELETE CASCADE, PRIMARY KEY(fsoid, tnode));")
#endif
};

int db_sqlite_impl::initialize_sql_number() const
{
	return 5; // magical number, but it doesn't matter
}

void db_sqlite_impl::build_sqls()
{
	for(int i = 0; i < sizeof(sql_template)/sizeof(unistr); i++)
	{
		unistr str(sql_template[i]);
		boost::replace_all(str, UT("$PREFIX_"), prefix_);
		sqls_.push_back(str);
	}
}

void db_sqlite_impl::complete_initialize()
{
	sqls_.clear(); // release memory
}

idx_t db_sqlite_impl::last_insert()
{
	return sqlite3_last_insert_rowid(handle_);
}
