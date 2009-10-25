#include "Category.h"
#include "Tag.h"
#include "TagIterator.h"

Category::Category(Database* db)
	:db_(db)
{
	tagrc_ = RefCounterRef<Tag>(new RefCounter<TagRef>(this, db));
	tagrc_->self_ = tagrc_;
}

TagRef Category::create_tag(
		const UniStr& name = UniStr(), 
		const UniStr& family = UniStr(), 
		OpenFlag flag)
{
	index_t idx = 0;
	bool exactily = false;
	bool hit = false;

	SqlQueryRef query = db_->create_query();
	query->set_operate(SqlQuery::search);
	query->app_table("remus_tag");
	query->app_target("name", name);
	query->app_value("*");

	if ( !family.empty() )
		query->app_target("family", family); // if family is not null, search for it!

	db_->exec(query);
	int rows = query->rows();

	hit = (rows == 1);
	if ( hit )
		query->col(0, &idx);

	query.reset();

	TagRef ref;
	if ( hit || flag != open_existing )
	{
		ref = tagrc_->create(idx);
		if ( idx < 0 )
		{
			ref->set_name(name);
			ref->set_family(create_family(family, flag));
		}
	} 
	return ref;
}

void Category::create_tag(index_t idx)
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
		query->set_table_name("remus_tag");
		query->set_target("idx", tag->idx());
		db_->exec(query);
	}
}

FamilyRef Category::create_family(const UniStr& name, OpenFlag flag)
{
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
	}
}

FamilyRef Category::create_family(index_t idx)
{
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
}
