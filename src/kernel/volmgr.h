#ifndef VOLMGR_H
#define VOLMGR_H

#include <pal/volume.h>

class volmgr_t
{
public:
	volmgr_t(class Database* );

	/* witness a volume
	 */
	void witness(const uuids&, int64_t* kpi); 
	void load_ntfs(int64_t kpi, uint64_t* jid, uint64_t* read_usn);
	void update_lastjid(int64_t kpi, uint64_t jid);
	void update_ntfsext(int64_t kpi, uint64_t jid, uint64_t usn);
private:
	class Database* dbmgr_;
};

#endif