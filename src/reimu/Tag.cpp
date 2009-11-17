#include "Tag.h"
#include "../DAL/Database.h"
#include "Category.h"
#include "Family.h"
#include "CommonT.h"

const Tag::RCRef Category::* rc = Category::get_tagrc_ptr();

TagParameters Tag::Para()
{
	return TagParameters();
}

Tag::Tag(index_t idx, Category* cat, Database* db, RefCounter<Tag>::RCRef rc)
	:IndexedObject(idx, cat, db), rc_(rc)
{
}

UniStr Tag::name() const
{
	return name_;
}

FamilyRef Tag::family() const
{
	if ( !family_ref_ && family_ )
		family_ref_ = cat_->create_family(family_);
	return family_ref_;
}


void Tag::load()
{
	if ( idx() <= 0 )
		return;
	load(db_);
}

void Tag::load(Database* db)
{
	SqlQueryRef query = db->create_query();
	query->set_operate(SqlQuery::search);
	query->app_table("remus_tag");
	query->app_target("idx", idx_);
	query->app_value("*");

	db->exec(query);
	query->col(1, &name_);
	query->col(2, &family_);
	db->close_query(query);
}

void Tag::store()
{
	if ( idx() > 0 )
	{
		SqlQueryRef query = db_->create_query();

		query->set_operate(SqlQuery::update);
		query->app_table("remus_tag");
		// for update operation, target means how we can identify the target
		query->app_target("idx", idx_);
		// value gives out the pairs
		query->app_value("name", name());

		db_->exec(query);
		db_->close_query(query);
	} else
	{
		//db_->begin_transaction();
		
		SqlQueryRef query = db_->create_query();

		// set opearte
		query->set_operate(SqlQuery::insert);
		// set table
		query->app_table("remus_tag");
		// for insert
		query->app_value("NULL");
		query->app_value(name());
		query->app_value(family()->idx());

		db_->exec(query);

		query->reset_query();

		// Re-index
		query->app_table("remus_tag");
		reidx(rc_, query, idx_);
		db_->close_query(query);
	}
}

void Tag::set_family(FamilyRef family_ref)
{
	/*
	family_ref_ = family_ref;
	family_ = family_ref->idx();
	*/
}
