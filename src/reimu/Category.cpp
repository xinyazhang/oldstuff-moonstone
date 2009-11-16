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

index_t Category::find_in_db(Database* db, const UniStr& table, const Parameters& args)
{
	index_t ret = 0;

	SqlQueryRef query = db->create_query();
	query->set_operate(SqlQuery::search);
	query->app_table(table);
	query->assembly(ref);
	query->app_value("idx");

	db_->exec(query);
	int rows = query->rows();

	if ( rows == 1 )
		query->col(0, &ret);

	return ret;
}
