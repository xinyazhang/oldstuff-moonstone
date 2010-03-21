#include "common.h"

static bool TagMan::invalid(tag_t tag)
{
	return tag.tnode == 0 && tag.name.empty();
}

TagMan::TagMan(Database* db)
	:db_(db), err_(0)
{
}

tag_t TagMan::create(const unistr& name)
{
	return create(name, 0);
}

tag_t TagMan::create(const unistr& name, idx_t idx)
{
	db_->begin_transaction();
	sql_stmt stmt = db_->create_insert_stmt(Database::TagTable, TAG_COLUMN_NUMBER);
	stmt.bind(1, name);
	stmt.bind(2, idx); // lazy initialization
	
	int err = stmt.execute();
	bool suceess = judge_and_replace(err, err_);

	db_->final_transaction();
	return suceess ? tag_t(name, idx) : tag_t();
}

tag_t TagMan::hardlink(tag_t& org, const unistr& name)
{
	/*
	 * 注意：UNIQUE(name, idx);
	 */
	if ( !touch_tnode(&org) )
		return tag_t();
	return create(name, org.tnode);
}

bool load_tags(sql_stmt& stmt, taglist_t& tagl)
{
	while ( stmt.step() )
	{
		tag_t t;
		stmt.col(1, t.name);
		stmt.col(2, t.tnode);
		tagl.add_distinct(t);
	}
}

taglist_t TagMan::locate(const unistr_list& l)
{
	// the troublest function of tagman
	taglist_t tagl;
	if ( l.empty() )
		return tagl;
	
	/*
	1. SELECT * FROM tag WHERE name = str[0] OR name = str[n-1]; <1>
		- this is called directly hit (first OR last)
		- 记得合并tnode相同的结果
	2. 当参数多于两个时……
		SELECT tnode FROM tag WHERE name = str[1] OR name = str[2] OR ... OR name = str[n-1]; <2>
		SELECT tnode FROM tagtag WHERE taggee in <1> and tagger in <2>; <3>
		SELECT * FROM tag WHERE tnode IN <3>;
		- 剩余处理同1
	*/
	unistr sql("SELECT DISTINCT name,tnode FROM ");
	sql += db_->table(Database::TagTable);
	sql += " WHERE name = $1 OR name = $2 ";
	sql_stmt stmt = db_->create_stmt_ex(sql);
	stmt.bind(1, l.front());
	stmt.bind(2, l.back());

	load_tags(stmt, tagl);

	return tagl;

	/*
	 * no taggee relations
	 * process 2 reserved
	 */
#if 0
	sql = "SELECT DISTINCT name,tnode FROM ";
	sql += db_->table(Database::TagTable);
	sql += 
	for(taglist_t::iteraotr iter = l.begin();
			iter != l.end();
			iter++)
	{
	}
#endif
}

tnode_t TagMan::access_tnode(tag_t& tag) // the same as hardlink
{
	if ( touch_tnode(&tag) )
		return db_->tnodeman()->locate(tag.tnode);
}

bool TagMan::update(const tag_t& old_tag, const tag_t& new_tag)
{
	TransactionFinaler(db_);
	
	sql_stmt stmt = db_->create_stmt_ex("UPDATE "+db_->table(Database::TagTable)+" set name = $1, tnode = $2 where name = $3 and tnode = $4");
	stmt.bind(1, new_tag.name);
	stmt.bind(2, new_tag.tnode);
	stmt.bind(3, old_tag.name);
	stmt.bind(4, old_tag.tnode);

	int err = stmt.execute();
	return judge_and_replace(err, err_);
}

static const char* deletinglocator[] =
{
	tag_t::namecol,
	tag_t::tnodecol
};

bool TagMan::del(const tag_t& t)
{
	db_->begin_transaction();
	sql_stmt stmt = db_->create_del_stmt(Database::TagTable, 
			TAG_COLUMN_NUMBER, 
			deletinglocators);
	stmt.bind(1, t.name);
	stmt.bind(2, t.tnode); // lazy initialization
	
	int err = stmt.execute();
	bool suceess = judge_and_replace(err, err_);
	if ( success )
	{
		db_->tnodeman()->refcdec(t.tnode);
	}

	db_->final_transaction();
	return suceess;
}

bool TagMan::setas_mastername(tag_t& tag)
{
	tnode_t tnode = access_tnode(tag);
	tnode.mastername = tag.name;
	db_->tnodeman()->update(tnode);
}

int TagMan::errno() const
{
	return err_;
}

bool TagMan::touch_tnode(tag_t* tag)
{
	if ( tag.tnode == 0 )
	{
		TnodeMan* tnodeman = db_->tnodeman();
		if ( 0 == tnodeman->create(tag, unistr()) )
			return false;
	}
	return true;
}
