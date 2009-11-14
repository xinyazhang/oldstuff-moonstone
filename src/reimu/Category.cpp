#include "Category.h"
#include "Tag.h"
#include "TagIterator.h"
#include "../DAL/Database.h"
#include "../DAL/SQLite/SQLite.h"

Category::Category(Database* db)
	:db_(db)
{
	memdb_ = new SQLite(":memory:");
	tagrc_ = RefCounter<Tag>::RCRef(new RefCounter<Tag>(this, db));
	tagrc_->self_ = tagrc_;
}

Category::~Category()
{
	delete memdb_;
}

TagRef Category::create_tag(
		const UniStr& name, 
		const UniStr& family, 
		OpenFlag flag)
{
	index_t idx = find_tag(db_, name, family);
	index_t mem_idx = 0; 
	if ( idx == 0 )
		mem_idx = find_tag(memdb_, name, family);
	bool hit = (idx != 0 ) || (mem_idx != 0);

	TagRef ref;
	/*
	 * Condition 1: tag founded in solid db
	 * Condition 2: tag founded in memory db
	 * Condition 3: tag not found
	 */
	if ( hit || flag != open_existing )
	{
		ref = tagrc_->create(idx); // create a tag in memory
		if ( idx < 0 )
		{
			ref->set_name(name);
			ref->set_family(create_family(family, flag));
		} else if ( idx != 0 )
			ref->load();
		else if ( mem_idx != 0 )
			ref->load(memdb_);
	} 
	return ref;
}

TagRef Category::create_tag(index_t idx)
{
	TagRef ref;

	SqlQueryRef query = db_->create_query();
	query->set_operate(SqlQuery::search);
	query->app_table("remus_tag");
	query->app_target("idx", idx);
	query->app_value("idx");

	db_->exec(query);
	if ( query->rows() > 0 )
	{
		ref = tagrc_->create(idx);
	}
	return ref;
}

void Category::rm_tag(const UniStr& name, const UniStr& family)
{
	TagRef tag = create_tag(name, family, open_existing);
	if ( tag )
	{
		SqlQueryRef query = db_->create_query();
		query->set_operate(SqlQuery::remove);
		query->app_table("remus_tag");
		query->app_target("idx", tag->idx());
		db_->exec(query);
	}
}

FamilyRef Category::create_family(const UniStr& name, OpenFlag flag)
{
	/*
	FamilyRef ref;
	SqlQueryRef query = db_->create_query();

	query->set_operate(SqlQuery::search);
	query->app_table("remus_family");
	query->app_target("name", name);
	query->app_value("idx");
	db_->exec(query);

	if ( query->rows() > 0 )
	{
		index_t idx;
		query->col(0, &idx);
		ref = familyrc_->create(idx);
	}*/
	return FamilyRef();
}

FamilyRef Category::create_family(index_t idx)
{
	/*
	FamilyRef ref;
	SqlQueryRef query = db_->create_query();

	query->set_operate(SqlQuery::search);
	query->app_table("remus_family");
	query->app_target("idx", idx);
	query->app_value("idx");
	db_->exec(query);

	if ( query->rows() > 0 )
	{
		ref = familyrc_->create(idx);
	}
	*/
	return FamilyRef();
}

index_t Category::find_tag(Database* db, const UniStr& name, const UniStr& family)
{
	bool hit = false;
	index_t ret = 0;
	SqlQueryRef query = db->create_query();
	query->set_operate(SqlQuery::search);
	query->app_table("remus_tag");
	query->app_target("name", name);
	query->app_value("idx");

	if ( !family.empty() )
		query->app_target("family", family); // if family is not null, search for it!

	db_->exec(query);
	int rows = query->rows();

	hit = (rows == 1);
	if ( hit )
		query->col(0, &ret);
}

