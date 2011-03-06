#include "common.h"

tnode_t TnodeMan::invalid_tnode()
{
	return tnode_t();
}

bool TnodeMan::invalid(const tnode_t& tnode)
{
	return tnode.idx == 0;
}

bool TnodeMan::invalid(const idx_t& idx)
{
	return idx == 0;
}

TnodeMan::TnodeMan(Database* db)
	:db_(db), err_(0)
{
}

idx_t TnodeMan::create(tag_t* master, const unistr& comment)
{
	db_->begin_transaction();
	/*
	 * parameter 1: table index (used in database self)
	 * 	- on the contray to the name, Database reprents the CONFIGURE and Databases 
	 * 		instances of a set of tags, files and mount points
	 * parameter 2: column number
	 * 	- considering about AUTOINCREMENT column
	 */
	/*
	 * stmt provides auto_pointer to real database related stmt instance
	 */
	sql_stmt stmt = db_->create_insert_stmt(Database::TnodeTable, TNODE_COLUMN_NUMBER);
	stmt.bind(1, 1);//refc = 1
	if ( master )
		stmt.bind(2, master->name);
	else
		stmt.bind(2, unistr());
	stmt.bind(3, comment);

	int err = stmt.execute();
	idx_t ret = 0;
	if ( judge_and_replace(err, err_) )
	{
		ret = db_->last_serial();
		if ( master )
			master->tnode = ret;
	}
	db_->final_transaction();

	return ret;
}

tnode_t TnodeMan::locate(idx_t idx)
{
	db_->begin_transaction();
	/* 
	 * parameter 1: table index
	 * parameter 2: index name(string)
	 */
	sql_stmt stmt = db_->create_simsel_stmt(Database::TnodeTable, "idx", "*");
	stmt.bind(1, idx);
	if ( !stmt.step() )
		return invalid_tnode();
	tnode_t tnode;
	stmt.col(1, tnode.idx);
	stmt.col(2, tnode.refc);
	stmt.col(3, tnode.mastername);
	stmt.col(4, tnode.comment);
	db_->final_transaction();

	return tnode;
}

static const char* updatecontent[2] =
{
	"mastername",
	"comment"
};

bool TnodeMan::update(const tnode_t& tnode)
{
	db_->begin_transaction();
	/* 
	 * parameter 1: table index
	 * parameter 2: primary key
	 * parameter 3: contents number
	 * parameter 4: update content name
	 */
	sql_stmt stmt = db_->create_update_stmt(Database::TnodeTable, "idx", 2, updatecontent);
	stmt.bind(1, tnode.idx);
	stmt.bind(2, tnode.mastername);
	stmt.bind(3, tnode.comment);

	int err = stmt.execute();
	judge_and_replace(err, err_);

	db_->final_transaction();
	return judge(err);
}

bool TnodeMan::refcinc(idx_t idx)
{
	sql_stmt stmt = db_->create_stmt_ex("UPDATE "+db_->table(Database::TnodeTable)+" SET refc=refc+1 WHERE idx=$1;");
	stmt.bind(1, idx);
	stmt.execute();
	
	return true; // no error detection
}

bool TnodeMan::refcdec(idx_t idx)
{
	db_->begin_transaction();
	sql_stmt stmt = db_->create_stmt_ex("UPDATE "+db_->table(Database::TnodeTable)+" SET refc=refc-1 WHERE idx=$1;");
	stmt.bind(1, idx);
	stmt.execute();
	stmt = db_->create_stmt_ex("DELETE FROM "+db_->table(Database::TnodeTable)+" WHERE refc=0;");
	stmt.execute();
	db_->final_transaction();
	return true;
}

taglist_t TnodeMan::names(idx_t idx)
{
	taglist_t ret;
	db_->begin_transaction();
	sql_stmt stmt = db_->create_stmt_ex("SELECT name,tnode FROM "+
		db_->table(Database::TagTable)+
		", "+
		db_->table(Database::TnodeTable)+
		" WHERE idx=$1 AND idx=tnode");
	stmt.bind(1, idx);
	stmt.execute();
	load_tags(stmt, ret);
	db_->final_transaction();
	return ret;
}

tag_t TnodeMan::anyone(idx_t idx)
{
	tag_t ret;
	db_->begin_transaction();
	sql_stmt stmt = db_->create_stmt_ex("SELECT name,tnode FROM "+
		db_->table(Database::TagTable)+
		", "+
		db_->table(Database::TnodeTable)+
		" WHERE idx=$1 AND idx=tnode");
	stmt.bind(1, idx);
	stmt.execute();
	load_tag(stmt, ret);
	db_->final_transaction();
	return ret;
}

int TnodeMan::eno() const
{
	return err_;
}
