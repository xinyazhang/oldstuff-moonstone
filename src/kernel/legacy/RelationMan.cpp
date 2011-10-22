#include "RelationMan.h"
#include "common.h"

#define TAG_TAG_RELATION_COLUMN_NUMBER 2

RelationMan::RelationMan(Database* db)
	:db_(db)
{
}

bool RelationMan::tag(tag_t& tagger, tag_t& taggee)
{
	return tag_in_tnode(db_->tagman()->access_tnode(tagger).idx, db_->tagman()->access_tnode(taggee).idx);
}

bool RelationMan::tag_in_tnode(idx_t tagger_tnode, idx_t taggee_tnode)
{
	db_->begin_transaction();
	sql_stmt stmt = db_->create_insert_stmt(Database::TagTagRelationTable, 
			TAG_TAG_RELATION_COLUMN_NUMBER);
	stmt.bind(1, tagger_tnode);
	stmt.bind(2, taggee_tnode);

	int err = stmt.execute();
	bool success = judge_and_replace(err, err_);

	db_->final_transaction();
	return success;
}

bool RelationMan::hastag(const tag_t& tagger, const tag_t& taggee)
{
	if ( TagMan::invalid(tagger) || TagMan::invalid(taggee) )
		return false;
	db_->begin_transaction();

	unistr sql("SELECT DISTINCT * FROM ");
	sql += db_->table(Database::TagTagRelationTable);
	sql += " WHERE tagger = $1 AND taggee = $2 ";
	sql_stmt stmt = db_->create_stmt_ex(sql);
	stmt.bind(1, tagger.tnode);
	stmt.bind(2, taggee.tnode);

	bool ret;
	if ( stmt.step() )
		ret = true;
	else 
		ret = false;
	db_->final_transaction();
	return ret;
}

static const char* deletinglocators[] =
{
	"tagger",
	"taggee"
};

bool RelationMan::detag(const tag_t& tagger, const tag_t& taggee)
{
	bool ret;
	db_->begin_transaction();
	if ( !hastag(tagger, taggee) )
	{
		ret = false;
	} else
	{
		sql_stmt stmt = db_->create_del_stmt(Database::TagTagRelationTable,
				TAG_TAG_RELATION_COLUMN_NUMBER,
				deletinglocators);
		stmt.bind(1, tagger.tnode);
		stmt.bind(2, taggee.tnode);

		int err = stmt.execute();
		bool success = judge_and_replace(err, err_);

		ret = success;
	}
	
	db_->final_transaction();
	return ret;
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

tnodelist_t RelationMan::tagger(const idx_t& taggee)
{
	if ( TnodeMan::invalid(taggee) )
		return tnodelist_t();
	tnodelist_t tnodel;
	unistr sql("SELECT DISTINCT tagger FORM ");
	sql += db_->table(Database::TagTagRelationTable);
	sql += " WHRER taggee = $1 ";
	sql_stmt stmt = db_->create_stmt_ex(sql);
	stmt.bind(1, taggee);

	load_tnodelist(db_, stmt, tnodel);

	return tnodel;
}

tnodelist_t RelationMan::taggee(const idx_t& tagger)
{
	if ( TnodeMan::invalid(tagger) )
		return tnodelist_t();
	tnodelist_t tnodel;
	unistr sql("SELECT DISTINCT taggee FORM ");
	sql += db_->table(Database::TagTagRelationTable);
	sql += " WHRER tagger = $1 ";
	sql_stmt stmt = db_->create_stmt_ex(sql);
	stmt.bind(1, tagger);

	load_tnodelist(db_, stmt, tnodel);

	return tnodel;
}
