#ifndef LOG_USN_H
#define LOG_USN_H

#include <stdint.h>
#include <boost/uuid/uuid_io.hpp>
#include "../partition.h"
#include <Windows.h>
#include "boost_arctype.h"

#define JOURNAL_BUFFER_SIZE 0x20000

class logusn_context_t
{
public:
	virtual ~logusn_context_t(){};

	enum SQLUSN_EVENT_TYPE
	{
		INVALID_SQLUSN_EVENT = 0,
		SWITCH_FILE = 1, 	/* Current file is too big and should start a new one */
		SWITCH_VOLUME = 2, 	/* Following events are related to this journal */
		JOURNALRUPT = 3, 	/* USN journal was once deleted and re-created */
		USN_AND_RECORD = 4
	};

	virtual SQLUSN_EVENT_TYPE cur_ev() {return INVALID_SQLUSN_EVENT;}; /* SHOULD NOT BE USED in writing */

	virtual void ev_uuid(uuid_t& ) = 0;
	virtual void ev_jid(uint64_t&) = 0;
	virtual void ev_usn_records(USN& usn, char* , size_t& bytes) = 0;
};

logusn_context_t* logusn_create_for_read(const unistr& varpath, int fid, int64_t initseek);
logusn_context_t* logusn_create_for_write(const unistr& varpath);
extern int lastfid;

void logusn_init();
void logusn_load(barc_i& ar);
void logusn_save(barc_o& ar);

#endif
