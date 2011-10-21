#ifndef DENTRY_H
#define DENTRY_H

struct dentry_t
{
	dentry_t(int64_t _kpi, uint64_t _inode, uint64_t _pinode, unichar* _pchar, int _len)
		:kpi(_kpi),
		inode(_inode),
		pinode(_pinode),
		fname(_pchar, _len)
	{
	}

	int64_t kpi;
	uint64_t inode, pinode;
	unistr fname;
};

#endif
