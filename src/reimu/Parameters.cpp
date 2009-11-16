#include "Parameters.h"
#include "../DAL/DataBase.h"

Parameters::Parameters()
{
}

Parameters::~Parameters()
{
}

void Parameters::push(const UniStr& col, const UniStr& val)
{
	condi_.push_back(col);
	value_.push_back(val);
}

void Parameters::assembly(SqlQueryRef ref) const
{
	for(std::vector<UniStr>::const_iterator citer = condi_.begin(), viter = value_.begin();
			citer != condi_.end();
			citer++, viter++)
	{
		if( viter->empty() )
			ref->app_target(*citer);
		else
		{
			ret->app_target(*citer, *viter);
		}
	}
}
