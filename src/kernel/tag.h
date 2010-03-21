#ifndef KERNEL_TAG_H
#define KERNEL_TAG_H

#include "common_declare.h"
#include <vector>

struct tag_t
{
	unistr name; 
		static const char* namecol = "name";
	idx_t tnode; 
		static const char* tnodecol = "tnode";
	tag_t():tnode(0){}
	tag_t(const unistr& n, idx_t = 0):name(n), tnode(0){}
};
#define TAG_COLUMN_NUMBER 2

typedef std::vector<tag_t> taglist_t;

bool operator==(const tag_t& l, const tag_t& r)
{
	/*
	 * Prepare for lazy creation
	 * 0 idx means the tag hasn't a tnode, implicts it has no hardlink
	 */
	return (l.idx == r.idx && l.idx != 0 )&& l.name == r.name;
}

#endif
