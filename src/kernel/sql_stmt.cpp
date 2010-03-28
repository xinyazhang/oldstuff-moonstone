#include "sql_stmt.h"
#include "../dal/sql_stmt_interface.h"

sql_stmt::sql_stmt(sql_stmt_interface* i)
	:internal_(i)
{
}

void sql_stmt::bind(int c)
{
	internal_->bind(c);
}

void sql_stmt::bind(int c, idx_t value)
{
	internal_->bind(c, value);
}

void sql_stmt::bind(int c, const unistr& value)
{
	internal_->bind(c, value);
}

int sql_stmt::execute()
{
	return internal_->execute();
}

bool sql_stmt::step()
{
	return internal_->step();
}

void sql_stmt::col(int c, unistr& value)
{
	internal_->col(c, value);
}

void sql_stmt::col(int c, idx_t& value)
{
	internal_->col(c, value);
}
