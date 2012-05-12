#ifndef KERNEL_RELATIONMAN_H
#define KERNEL_RELATIONMAN_H

#include "common_declare.h"
#include "kernellist.h"

class EXPORT_TO_DLL RelationMan
{
public:
	RelationMan(Database* );
public:
	bool tag(tag_t& tagger, tag_t& taggee);
	bool tag_in_tnode(idx_t tagger_tnode, idx_t taggee_tnode);
	bool hastag(const tag_t& tagger, const tag_t& tagee);
	bool detag(const tag_t& tagger, const tag_t& taggee);
	tnodelist_t tagger(const idx_t& taggee);
	tnodelist_t taggee(const idx_t& tagger);
private:
	Database* db_;
	int err_;
};

#endif
