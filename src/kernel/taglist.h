#ifndef KERNEL_TAGLIST_H
#define KERNEL_TAGLIST_H

#include <vector>
#include "tag.h"

class taglist_t
	:public std::vector<tag_t>
{
public:
	void EXPORT_TO_DLL add_distinct(const tag_t&);
};

#endif
