#ifndef KERNEL_TNODEMAN_H
#define KERNEL_TNODEMAN_H

#include <vector>
#include "common_declare.h"
#include "tnode.h"
#include "kernellist.h"

class EXPORT_TO_DLL TnodeMan
{
public:
	TnodeMan(Database*);
public:
	idx_t create(tag_t* master, const unistr& comment);
	tnode_t locate(idx_t );
	bool update(const tnode_t&);
	bool refcinc(idx_t );
	bool refcdec(idx_t );
	taglist_t names(idx_t );
	tag_t anyone(idx_t);
	int eno() const;

private:
	Database* db_;
	int err_;
public:
	static bool invalid(const tnode_t& );
	static bool invalid(const idx_t& );
	static tnode_t invalid_tnode();
};

#endif
