#ifndef KERNEL_TNODE_H
#define KERNEL_TNODE_H

struct tnode_t
{
	idx_t idx; 
		static const char* idxcol = "idx";
	unistr mastername; 
		static const char* masternamecol = "mastername";
	unistr comment;
		static const char* commentcol = "comment";
};

#define TNODE_COLUMN_NUMBER 3

#endif
