#ifndef FILEMGR_H
#define FILEMGR_H

#include <pal/stdtype.h>
struct dentry_t;

class EXPORT_TO_DLL filemgr_t
{
public:
	filemgr_t(class Database* );

	void checkbegin(int64_t kpi); // Begin to reindex the volume
	void witness(const dentry_t&); // Seen a dentry in the journal
	void blobchange(const dentry_t&); // Seen the context modification 
	void ack(const dentry_t&); // The dentry exists
	void nak(const dentry_t&); // The dentry is removed
	void existance_flip(const dentry_t&); // The dentry is filped (exits->removed, nonexits->exits)
	void rename(const dentry_t&); // The name of a dentry is updated
	void symlinkchange(const dentry_t&); // The dentry is a symlink and has been updated
	void checkdone(int64_t kpi); // The reindex of the volume is done, all dentry hasn't been witnessed should be considered removed
private:
	class Database* dbmgr_;
};

#endif
