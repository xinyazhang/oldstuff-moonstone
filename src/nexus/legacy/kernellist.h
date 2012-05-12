#ifndef KERNEL_TAGLIST_H
#define KERNEL_TAGLIST_H

#include <vector>
#include "tag.h"
#include "tnode.h"

template<typename T>
class distinctlist_t
	:public std::vector<T>
{
public:
	bool find(const T& t) const
	{
		for(const_iterator iter = begin();
				iter != end();
				iter++)
		{
			if (t == *iter)
				return true;
		}
		return false;
	}

	void add_distinct(const T& t)
	{
		if (find(t))
			return;
		//this->insert(end(), 1, t); // workaround for cl compiler bug
		this->push_back(t);
	}

	void replace(const T& target, const T& value)
	{
		for(iterator iter = begin();
				iter != end();
				iter++)
		{
			if (target == *iter)
			{
				*iter = value;
				return ;
			}
		}
	}
};

typedef distinctlist_t<tag_t> taglist_t;
typedef distinctlist_t<tnode_t> tnodelist_t;
typedef distinctlist_t<idx_t> idxlist_t;

#endif
