#ifndef _KERNEL_CATEGORY_TEMPLATE_
#define _KERNEL_CATEGORY_TEMPLATE_

#include "Category.h"

template<class P>
	typename P::Ref Category::create(P args, OpenFlag flag)
{
	UniStr table = table_prefix_ + P::table_name;

	index_t idx = find_in_db(db_, table_name, args);
	index_t mem_idx = 0;
	if ( idx == 0 )
		mem_idx = find_in_db(memdb_, table_name, args);
	bool hit = (idx != 0 ) || (mem_idx != 0);

	typename P::Ref ref;

	if ( hit || flag != open_existing )
	{
		typename P::RCRef& rc = get_rc<P>();
		ref = rc->create(idx); // create a tag in memory
		if ( idx < 0 )
		{
			ref->sets(args);
		} else if ( idx != 0 )
			ref->load();
		else if ( mem_idx != 0 )
			ref->load(memdb_);
	} 
	return ref;
}

template<class T>
	typename T::Ref Category::create(index_t idx)
{
	TagRef ref;

	SqlQueryRef query = db_->create_query();
	query->set_operate(SqlQuery::search);
	query->app_table(table_prefix_ + T::table_name);
	query->app_target("idx", idx);
	query->app_value("idx");

	db_->exec(query);
	if ( query->rows() > 0 )
	{
		typename P::RCRef& rc = get_rc<T::Para>();
		ref = rc->create(idx);
	}
	return ref;
}

template<class P>
	void rm(P args)
{
	P::Ref ref = create(args, open_existing);
	if ( ref )
	{
		SqlQueryRef query = db_->create_query();
		query->set_operate(SqlQuery::remove);
		query->app_table(table_prefix_ + P::table_name);
		query->app_target("idx", ref->idx());
		db_->exec(query);
	}
}

template<class T>
	typename T::IteratorRef create_iterator()
{
	return T::IteratorRef(new typename T::Iterator(this, db_));
}

#endif
