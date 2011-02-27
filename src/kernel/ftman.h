#ifndef KERNEL_FILES_TAGGER_MANAGER_H
#define KERNEL_FILES_TAGGER_MANAGER_H

#include "common_declare.h"
#include "kernellist.h"

/*
 * Only basic functions are provided, get tagname from ft table is not
 * necessary.
 */

class ftman_t
{
public:
	ftman_t(Database* );
	~ftman_t();
public:
	bool add_tag(const fso_t&, tag_t&);

	tnodelist_t tnodes(const fso_t&);
	idxlist_t tagged_file(tag_t&);
	idxlist_t tagged_file_toplevel(tag_t&);

	bool withdraw_tag(const fso_t&, tag_t&);
private:
	Database* db_;
	int err_;

	idx_t access_tnode(tag_t& tag);
};

#endif
