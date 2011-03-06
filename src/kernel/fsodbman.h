#ifndef KERNEL_FSO_POOL_MAN_H
#define KERNEL_FSO_POOL_MAN_H

#include "common_declare.h"

class EXPORT_TO_DLL FsodbMan
{
public:
	FsodbMan(Database* );
public:
	/*
	 * High Level Interfaces
	 */
	//bool add(const unistr& path, int flag = 0);
	bool add_fso(const unistr& name, idx_t parentid);
	bool add_fso(fso_t& fso, idx_t parentid);
	//bool add(const fso_t&);
	idx_t ensure(const unistr& name, idx_t parent);
	idx_t ensure(fso_t& fso, idx_t parentid);
	unistr fullpath(idx_t fsoid);
	idx_t locate(const unistr& path);
	idx_t locate(const unistr& name, idx_t parentid);
	fso_t locate(idx_t);
	bool rm(idx_t fsoid);
	bool chroot(idx_t fsoid, idx_t parentnew);
	bool update(const fso_t&);// update extra attribute other than path and idx_t

	/*
	 * Low Level Interfaces
	 */
	bool fsocd(fso_t&, const unistr& name); 
	bool fsocdup(fso_t& ); 
	fso_t fsocontent(const fso_t&); // pickup the first child
	bool fsonext(fso_t& ); // next fso, used to iterate elements, use LIMIT clause for 1 result
	bool reloadfso(fso_t&);
	bool updatefso(fso_t&);
	bool haschild(idx_t );
	int childcound(idx_t);

	void maintain_mtimer(idx_t); // mtimer = max(max(mtime), max(mtimer)) in all direct children

	int eno() const;
private:
	Database* db_;

	int err_;
};

#endif
