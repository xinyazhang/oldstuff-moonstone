#ifndef KERNEL_TAGLIST_H
#define KERNEL_TAGLIST_H

#include <vector>

template<typename T>
class distinctlist_t
	:public std::vector<T>
{
public:
	void add_distinct(const T& t)
	{
		for(const_iterator iter = begin();
				iter != end();
				iter++)
		{
			if (t == *iter)
				return;
		}
		//this->insert(end(), 1, t); // workaround for cl compiler bug
		this->push_back(t);
	}
};

typedef distinctlist_t<tag_t> taglist_t;
typedef distinctlist_t<tnode_t> tnodelist_t;

#endif
