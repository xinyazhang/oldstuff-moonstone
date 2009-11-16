#include "TagIterator.h"
#include "../../core/UniStr.h"
#include "../DAL/DataBase.h"

TagIterator::TagIterator(Category* cat, DataBase* db)
	:query(db->create_query()), cat_(cat), db_(db)
{
	query->set_operate(SqlQuery::search);
	query->app_table(cat_->table_prefix() + Tag::table_name);
	db->exec(query);
}

TagRef TagIterator::read()
{
	if ( !query.done() )
	{
		index_t idx;
		query.col(0, &idx);
		return cat->create<Tag>(idx);
	} else
		return TagRef();
}

bool TagIterator::done()
{
	return query.done();
}
