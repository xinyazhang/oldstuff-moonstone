#ifndef KERNEL_FSO_H
#define KERNEL_FSO_H

#include "common_declare.h"
/*
 * File system object 
 *
 * can also be used as directory iterator
 */

struct EXPORT_TO_DLL fso_t
{
	fso_t(); // root dir, note for win32: \\?\ is root
	fso_t(idx_t ); // an fso only id loaded
	fso_t(const unistr& name); // only setting name
	void addbind(sql_stmt& stmt, idx_t parentid);
	void updatebind(sql_stmt& stmt);
	void load(sql_stmt& stmt);
	bool is_root() const;
	bool valid() const;
	unistr name() const;
	idx_t fsoid() const { return fsoid_; }
	idx_t parentid() const { return parent_; }
	void chparent(idx_t parentnew) { parent_ = parentnew; }

	idx_t fsoid_;
	idx_t parent_;
	unistr name_;
	unistr path_;
	int64_t size_;
	int64_t mtime_;
	int64_t mtimer_;
	int hash_algo_;
	void* hash_;
	bool is_dir_;
};

#endif
