#ifndef KERNEL_TAG_H
#define KERNEL_TAG_H

#include "common_declare.h"
#include <vector>

struct EXPORT_TO_DLL tag_t
{
	unistr name; 
		static const char* namecol;
	idx_t tnode; 
		static const char* tnodecol;
	tag_t():tnode(0){}
	tag_t(const unistr& n, idx_t = 0):name(n), tnode(0){}
};

#define TAG_COLUMN_NUMBER 2

inline bool operator==(const tag_t& l, const tag_t& r)
{
	/*
	 * Prepare for lazy creation
	 * 0 idx means the tag hasn't a tnode, implicts it has no hardlink
	 */
	return (l.tnode == r.tnode && l.tnode != 0 )|| (l.name == r.name && l.tnode==0 && r.tnode==0);
}

#endif
