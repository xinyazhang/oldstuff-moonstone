#ifndef KERNEL_TAGMAN_H
#define KERNEL_TAGMAN_H

class Database;
#include "taglist.h"

class EXPORT_TO_DLL TagMan
{
public:
	TagMan(Database* );
public:
	tag_t create(const unistr&);
	tag_t hardlink(tag_t&, const unistr&);// may modify the tag's tnode
	taglist_t locate(const unistr_list&);
	bool update(const tag_t& old_tag, const tag_t& new_tag);
	tnode_t access_tnode(tag_t& ); // the same as hardlink
	bool del(const tag_t&);
	bool setas_mastername(tag_t&);
	unistr mastername(const tag_t&);
	int eno() const;

	static bool invalid(const tag_t&);
private:
	Database* db_;
	int err_;
	tag_t create(const unistr&, idx_t);
	bool touch_tnode(tag_t* );
};

bool load_tags(sql_stmt& stmt, taglist_t& tagl);

#endif
