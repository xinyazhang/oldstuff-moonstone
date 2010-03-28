#ifndef KERNEL_TNODE_H
#define KERNEL_TNODE_H

struct tnode_t
{
	idx_t idx; 
	idx_t refc;
//		static const char* idxcol = "idx";
	unistr mastername; 
//		static const char* masternamecol = "mastername";
	unistr comment;
//		static const char* commentcol = "comment";
	tnode_t():idx(0), refc(0){}
};

#define TNODE_COLUMN_NUMBER 3

#endif
