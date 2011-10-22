#ifndef KERNEL_TNODE_H
#define KERNEL_TNODE_H

struct EXPORT_TO_DLL tnode_t
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

inline bool operator==(const tnode_t& l, const tnode_t& r)
{
	return l.idx == r.idx;
}	

#define TNODE_COLUMN_NUMBER 3

#endif
