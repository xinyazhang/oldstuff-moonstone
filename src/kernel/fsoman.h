#ifndef KERNEL_FSO_POOL_MAN_H
#define KERNEL_FSO_POOL_MAN_H

#include "common_declare.h"
class fso_t;

class EXPORT_TO_DLL FsoMan
{
public:
	FsoMan(Database* );
public:
	/*
	 * High Level Interfaces
	 */
	bool add(const unistr& path, int flag = 0);
	unistr fullpath(idx_t fsoid);
	idx_t locate(const unistr& path);
	fso_t locate(idx_t);
	bool rm(const unistr& path, int flag = 0);
	bool rm(idx_t fsoid, int flag = 0);
	bool mv(idx_t fsoid, const unistr& pathnew, int flag = recursive_flag);
	bool cp(idx_t fsoid, const unistr& pathnew, int flag = recursive_flag);
	bool chroot(idx_t fsoid, const unistr& pathnew);

	/*
	 * Low Level Interfaces
	 */
	bool fsocd(fso_t&, const unistr& name); // only update fsoid and parent
	bool fsocdup(fso_t& ); // same as above, using reloadfso to gain latest db data
	bool reloadfso(fso_t&);
	bool updatefso(fso_t&);

	int eno() const;
public:
	enum FsoFlagShift
	{
		recursive_shift = 1
	};
	static const int recursive_flag = 1 << recursive_shift;

private:
	Database* db_;

	idx_t ensure_dir(const unistr&);
	bool add_fso(const unistr& path, idx_t parentid);
	idx_t ensure_fso(const unistr& path, idx_t parentid);
	bool add_recursive(const unistr& path, idx_t rootfso);

	int err_;
};

#endif
