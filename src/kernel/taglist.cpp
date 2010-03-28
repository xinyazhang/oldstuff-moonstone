#include "taglist.h"

void taglist_t::add_distinct(const tag_t& t)
{
	for(const_iterator iter = begin();
			iter != end();
			iter++)
	{
		if (t == *iter)
			return;
	}
	this->insert(end(), 1, t); // workaround for cl compiler bug
}
