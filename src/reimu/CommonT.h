#ifndef _KERNEL_COMMON_TEMPLATE_
#define _KERNEL_COMMON_TEMPLATE_

#include "RefCounter.h"

template<typename T>
void reidx(RefCounter<T>::RCRef rc_, SqlQueryRef query, index_t idx_)
{
	query->set_operate(SqlQuery::query_max);
	// for query_max(a common usage opearte)
	// target means the column we want to know the maximum
	query->app_target("idx");

	db_->exec(query);

	index_t idx_new;
	// easy interface to gain result
	query->col(0, &idx_new);

	rc_->move(idx_, idx_new);
	idx_ = idx_new;

}

#endif
