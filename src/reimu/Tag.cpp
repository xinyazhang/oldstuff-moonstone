#include "Tag.h"
#include "../DAL/Database.h"
#include "Category.h"
//#include "Family.h"

Tag::Tag(index_t idx, Category* cat, Database* db, RefCounter<Tag>::RCRef rc)
	:IndexedObject(idx, cat, db), rc_(rc)
{
}

UniStr Tag::name() const
{
	return name_;
}

void Tag::set_name(const UniStr& str)
{
	name_ = str;
}

void Tag::load()
{
	if ( idx() <= 0 )
		return;

	SqlQueryRef query = db_->create_query();
	query->set_operate(SqlQuery::search);
	query->app_table("remus_tag");
	query->app_target("idx", idx_);
	query->app_value("*");

	db_->exec(query);
	query->col(1, &name_);
	query->col(2, &family_);
	db_->close_query(query);
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

		db_->exec(query);

		query->reset_query();
		query->set_operate(SqlQuery::query_max);
		// for query_max(a common usage opearte)
		query->app_table("remus_tag");
		// target means the column we want to know the maximum
		query->app_target("idx");

		db_->exec(query);

		index_t idx_new;
		// easy interface to gain result
		query->col(0, &idx_new);

		rc_->move(idx_, idx_new);
		idx_ = idx_new;

		db_->close_query(query);

		//db_->end_transaction();
	}
}

FamilyRef Tag::family() const
{
	if ( !family_ref_ && family_ )
		family_ref_ = cat_->create_family(family_);
	return family_ref_;
}

void Tag::set_family(FamilyRef family_ref)
{
	/*
	family_ref_ = family_ref;
	family_ = family_ref->idx();
	*/
}
