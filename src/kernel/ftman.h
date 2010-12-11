#ifndef KERNEL_FILES_TAGGER_MANAGER_H
#define KERNEL_FILES_TAGGER_MANAGER_H

#include "common_declare.h"
#include "kernellist.h"

class ftman_t
{
public:
	ftman_t(Database* );
	~ftman_t();
public:
	bool add_tag(fso_t, tag_t);
	taglist_t tags(fso_t):
	tnodelist_t tnodes(fso_t);
	bool withdraw_tag(fso_t, tag_t);
private:
	Database* db_;
};

#endif
