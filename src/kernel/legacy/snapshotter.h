#ifndef KERNEL_SNAPSHOTTER_H
#define KERNEL_SNAPSHOTTER_H

#include "common_declare.h"

class EXPORT_TO_DLL snapshotter
{
public:
	snapshotter(Database* );
	~snapshotter();

	void redirect(FsodbMan* fsodbman);

	bool add(const unistr& path, int flag = 0);
	idx_t ensure_dir(const unistr&);
	bool withdraw(const unistr& path, int flag = 0);

	enum FsoFlagShift
	{
		recursive_shift = 1
	};
	static const int recursive_flag = 1 << recursive_shift;
	
private:
	Database* db_;
	FsodbMan* fsodb_;

	bool add_recursive(const unistr& path, idx_t rootfso);
};

#endif
