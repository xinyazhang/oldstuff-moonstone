#ifndef VOLMGR_H
#define VOLMGR_H

#include <pal/volume.h>

class EXPORT_TO_DLL volmgr_t
{
public:
	volmgr_t(class Database* );

	/* witness a volume
	 */
	void witness(const uuids&, int64_t* kpi); 
	void load_ntfs(int64_t kpi, uint64_t* jid, uint64_t* read_usn);
	void update(const volume&);
	void update_lastjid(int64_t kpi, uint64_t jid);
	void update_ntfsext(int64_t kpi, uint64_t jid, uint64_t usn);
	std::vector<volume> known_volumes() const;
private:
	class Database* dbmgr_;

	std::vector<volume> known;
};

#endif
