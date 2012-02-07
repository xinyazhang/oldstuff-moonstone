#include "watching_win32.h"
#include "privilege.h"
#include "Database.h"
#include "filemgr.h"
#include "volmgr.h"
#include "dentry.h"
#include "privilege.h"
#include "feedback.h"

#define JOURNAL_BUFFER_SIZE 4096

static HANDLE general_event_create()
{
	return CreateEvent(NULL, TRUE, FALSE, NULL);
}

watching_win32::watching_win32(const struct volume& _vol, class Database* _db)
	:watching_t(_vol, _db)
{
	fd_ = privilege::open_volume(vol.uuid);

	dbmgr->volmgr()->witness(vol.uuid, &vol.kpi);
	if (detect_fstype(fd_) == FILESYSTEM_NTFS) {
		dbmgr->volmgr()->load_ntfs(vol.kpi, &lastjid, &lastusn);
	} else {
		/* FB */
		CloseHandle(fd_);
		fd_ = INVALID_HANDLE_VALUE;
	}
}

watching_win32::~watching_win32()
{
	/* FB */
	CancelIo(fd_);
	CloseHandle(fd_);
	fd_ = INVALID_HANDLE_VALUE;
}

bool watching_win32::check()
{
	if (fd_ == INVALID_HANDLE_VALUE)
		return false;

	DWORD ret_bytes;
	BOOL bret = DeviceIoControl(fd_,
			FSCTL_QUERY_USN_JOURNAL,
			NULL,
			0,
			&usn_meta,
			sizeof(usn_meta),
			&ret_bytes,
			NULL);
	if (!bret)
	    return false;

	return true;
}

int watching_win32::init()
{
static const READ_USN_JOURNAL_DATA default_read_data = {0, 0xFFFFFFFF, FALSE, 0, 8, 0};
	usn_param = default_read_data;
	if (usn_meta.UsnJournalID != lastjid) {
		dbmgr->begin_transaction();
		dbmgr->filemgr()->checkbegin(vol.kpi);
		dbmgr->volmgr()->update_lastjid(vol.kpi, usn_meta.UsnJournalID, usn_meta.NextUsn);
		if (lastjid != 0)
			recheck = true;
		else
			recheck = false;
	} else {
		recheck = false;
		usn_param.StartUsn = lastusn;
	}

	lastjid = usn_meta.UsnJournalID;
	usn_param.UsnJournalID = lastjid;
	usn_buffer = shared_array<char>(new char[JOURNAL_BUFFER_SIZE]);
	memset(&overlap, 0, sizeof(overlap));
	return 0;
}

int watching_win32::dispach_read()
{
	BOOL ret = DeviceIoControl(fd_,
			FSCTL_READ_USN_JOURNAL, 
			&usn_param,
			sizeof(usn_param),
			usn_buffer.get(),
			JOURNAL_BUFFER_SIZE,
			&usn_read_bytes,
			&overlap);
	if (!ret) {
		int err;
		err = GetLastError();
	}
	return ret;
}


native_fd watching_win32::fd() const
{
	return fd_;
}

int watching_win32::process(const ioinfo_t& io)
{
static const DWORD USN_BLOB_CHANGE = (USN_REASON_DATA_EXTEND|
		USN_REASON_DATA_OVERWRITE|
		USN_REASON_DATA_TRUNCATION);
	size_t left = io.size - sizeof(USN);
	PUSN_RECORD record_ptr = (PUSN_RECORD)(usn_buffer.get() + sizeof(USN));

	if (!recheck)
		dbmgr->begin_transaction(); // Per buffer transaction if not re-checking, for performance

	if (left > 0)
		log().printf(LOG_DEBUG, UT("Begin write journal records\n"));
	while (left > 0) {
#if 0
		printf("USN: %I64x\n", record_ptr->Usn );
		printf("File name: %.*S\n", 
				record_ptr->FileNameLength/2, s
				record_ptr->FileName );
		printf("Reason: %x\n", record_ptr->Reason );
		printf("\n");
#endif

		//unistr filename(record_ptr->FileName, record_ptr->FileNameLength/2);
		/* Process the REASON! */
		DWORD r = record_ptr->Reason;

		dentry_t dentry(vol.kpi,
				record_ptr->FileReferenceNumber, /* inode */
				record_ptr->ParentFileReferenceNumber, /* parent's inode */
				record_ptr->FileName, /* filename */
				record_ptr->FileNameLength/2 /* filename length, -1 for \0 term */
				);
#if 1
		if (dentry.fname == UT("New folder"))
			dentry.fname = UT("New folder");
#endif
#if 0
		if (dentry.fname == UT("Thunderbird.7z"))
		{
#endif
		//dbmgr->filemgr()->witness(dentry);

		if (r & USN_BLOB_CHANGE) {
			dbmgr->filemgr()->blobchange(dentry);
		}
		if (r & USN_REASON_FILE_DELETE) {
			dbmgr->filemgr()->nak(dentry);
		}
		if (r & USN_REASON_HARD_LINK_CHANGE) {
			dbmgr->filemgr()->existance_flip(dentry);
		}
		if (r & USN_REASON_RENAME_OLD_NAME) {
			dbmgr->filemgr()->rename_old(dentry);
		}
		if (r & USN_REASON_RENAME_NEW_NAME) {
			dbmgr->filemgr()->rename_new(dentry);
		}
		if (r & USN_REASON_REPARSE_POINT_CHANGE) {
			dbmgr->filemgr()->symlinkchange(dentry);
		}

		if (!(r & USN_REASON_FILE_DELETE) && !(r & USN_REASON_RENAME_OLD_NAME)) {
			/* Replacement of witness */
			dbmgr->filemgr()->ack(dentry);
		}

#if 0
		}
#endif

		left -= record_ptr->RecordLength;

		// Find the next record
		record_ptr = (PUSN_RECORD)(((char*)record_ptr) + 
				record_ptr->RecordLength); 
	}
	USN usn_next = *(USN *)usn_buffer.get();

	usn_param.StartUsn = usn_next;

	dbmgr->volmgr()->update_ntfsext(vol.kpi, lastjid, lastusn);
	log().printf(LOG_DEBUG, UT("Status: KPI: %lld, USN: %lld\n"), vol.kpi, lastusn);
	if (recheck && usn_next >= lastusn) {
		dbmgr->filemgr()->checkdone(vol.kpi);
		lastusn = usn_next;
		dbmgr->final_transaction();
	} else if (!recheck) {
		lastusn = usn_next;
		dbmgr->final_transaction();
	}
	dispach_read();
	proc().printf(INDEX_PROGRESSED, UT("KPI: %lld, USN: %lld\n"), vol.kpi, lastusn);
	return 0;
}

int watching_win32::status() const
{
	/* Never exits unless removed */
	return last;
}

