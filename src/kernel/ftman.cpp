#include "common.h"

#define COLUMN_NUMBER 2

ftman_t::ftman_t(Database* db)
	:db_(db)
{
}

ftman_t::~ftman_t()
{
}

bool ftman_t::add_tag(const fso_t& fso, tag_t& tag)
{
	db_->begin_transaction();
	sql_stmt stmt = db_->create_insert_stmt(Database::FtTable, COLUMN_NUMBER);
	stmt.bind(1, fso.fsoid());
	stmt.bind(2, access_tnode(tag));

	int err = stmt.execute();
	bool success = judge_and_replace(err, err_);

	db_->final_transaction();
	return success;
}

static void load_tnodelist(Database* db, sql_stmt& stmt, tnodelist_t& tnodel)
{
	while( stmt.step() )
	{
		idx_t idx;
		stmt.col(1, idx);
		tnodel.add_distinct(db->tnodeman()->locate(idx));
	}
}

tnodelist_t ftman_t::tnodes(const fso_t& fso)
{
	tnodelist_t tnodel;
	unistr sql("SELECT DISTINCT tnode FROM ");
	sql += db_->table(Database::FtTable);
	sql += " WHERE fsoid = $1;";
	sql_stmt stmt = db_->create_stmt_ex(sql);
	stmt.bind(1, fso.fsoid());

	load_tnodelist(db_, stmt, tnodel);
	return tnodel;
}

idxlist_t ftman_t::tagged_file(tag_t& tag)
{
	idxlist_t idxl;
	sql_stmt stmt = sql_get_tagged_file(tag);

	while( stmt.step() )
	{
		idx_t idx;
		stmt.col(1, idx);
		idxl.add_distinct(idx);
	}
	return idxl;
}

static const char* deletinglocators[] =
{
	"fsoid", "tnode"
};

idxlist_t ftman_t::tagged_file_toplevel(tag_t& tag)
{
	idxlist_t idxl;
	sql_stmt stmt = sql_get_tagged_file(tag);

	while( stmt.step() )
	{
		idx_t idx;
		stmt.col(1, idx);
		/* check it ancestors */
		bool found = false;
		fso_t iter = db_->fsodbman()->locate(idx);
		while ( db_->fsodbman()->fsocdup(iter) )
			if (idxl.find(iter.fsoid()))
			{
				found = true;
				break;
			}

		if (!found)
			idxl.add_distinct(idx);
		else
			idxl.replace(iter.fsoid(), idx);
	}
	return idxl;
}

bool ftman_t::withdraw_tag(const fso_t& fso, tag_t& tag)
{
	db_->begin_transaction();
	sql_stmt stmt = db_->create_del_stmt(Database::FtTable, 
			COLUMN_NUMBER, 
			deletinglocators);
	stmt.bind(1, fso.fsoid());
	stmt.bind(2, access_tnode(tag)); // lazy initialization

	int err = stmt.execute();
	bool success = judge_and_replace(err, err_);

	db_->final_transaction();
	return success;
}

idx_t ftman_t::access_tnode(tag_t& tag)
{
	return db_->tagman()->access_tnode(tag).idx;
}

sql_stmt ftman_t::sql_get_tagged_file(tag_t& tag)
{
	unistr sql("SELECT DISTINCT fsoid FROM ");
	sql += db_->table(Database::FtTable);
	sql += " WHERE tag = $1;";
	sql_stmt stmt = db_->create_stmt_ex(sql);
	stmt.bind(1, access_tnode(tag));
	return stmt;
}
