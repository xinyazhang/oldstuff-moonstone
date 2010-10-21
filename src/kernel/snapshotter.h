#ifndef KERNEL_SNAPSHOTTER_H
#define KERNEL_SNAPSHOTTER_H

#include "common_declare.h"

class EXPORT_TO_DLL Snapshotter
{
public:
	Snapshotter(Database* );
	~Snapshotter();

	void redirect(FsodbMan* fsodbman);

	bool add(const unistr& path, int flag = 0);
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
