#include "watching.h"
#include <pal/ipc.h>
#include <pal/ipc_packet_type.h>
#include "privilege.h"
#include "Database.h"
#include "dentry.h"

#define JOURNAL_BUFFER_SIZE 16384

static HANDLE general_event_create()
{
	return CreateEvent(NULL, TRUE, FALSE, NULL);
}

watching_t watching_t::create(Database* dbmgr, const struct volume& vol)
{
	watching_t watch;
	watching.vol = vol;
#if 0
	watch.fd = INVALID_HANDLE_VALUE;
	native_fd server = ipc_center::lock_server_connect();

	ipc_direct_write_packet(server, PT_PRIV_WIN32_VOLUME_HANDLE_REQ, uuid2b(vol.uuid), 16);
	ipc_packet* packet = ipc_read_packet(native_fd);

	if (packet->header.type != PT_PRIV_WIN32_VOLUME_HANDLE_REP)
		return watch;

	watch.fd = ipc_recv_fd(server);

	ipc_center::release_server();
#endif
	watch.fd = privilege::open_volume(vol.uuid);

	/*
	 * Sync from DB
	 */
	dbmgr->volmgr()->witness(vol.uuid, &watch.vol.kpi);
	if (detect_fstype(watch.fd) == FILESYSTEM_NTFS) {
		dbmgr->volmgr()->load_ntfs(watch.vol.kpi, &watch.lastjid, &watch.lastusn);
	} else {
		CloseHandle(watch.fd);
		watch.fd = INVALID_HANDLE_VALUE;
	}
}

/*
 * Check whether we could watch the volume
 *
 */
bool watching_t::check(const watching_t& watch)
{
	if (watch.fd == INVALID_HANDLE_VALUE)
		return false;

	DWORD ret_bytes;
	BOOL bret = DeviceIoControl(watch.fd,
			FSCTL_QUERY_USN_JOURNAL,
			NULL,
			0,
			&watch.usn_meta,
			sizeof(watch.usn_meta),
			&ret_bytes,
			NULL);
	if (!bret)
	    return false;

	return true;
}

void watching_t::init(Database* dbmgr)
{
static const READ_USN_JOURNAL_DATA default_read_data = {0, 0xFFFFFFFF, FALSE, 0, 0};
	usn_param = default_read_data;
	if (usn_meta.UsnJournalID != lastjid) {
		dbmgr->begin_transaction();
		dbmgr->filemgr()->checkbegin(vol.kpi);
		dbmgr->volmgr()->update_lastjid(vol.kpi, usn_meta.UsnJournalID);
		recheck = true;
	} else {
		recheck = false;
		usn_param.StartUsn = lastusn;
	}

	lastjid = usn_param.UsnJournalID;
	usn_buffer = shared_ptr(new char[JOURNAL_BUFFER_SIZE]);
	overlap = general_event_create();
}

void watching_t::dispach_read()
{
	BOOL ret = DeviceIoControl(fd,
			FSCTL_READ_USN_JOURNAL, 
			&usn_param,
			sizeof(usn_param),
			usn_buffer.get(),
			JOURNAL_BUFFER_SIZE,
			&usn_read_bytes,
			&overlap);
}

bool watching_t::close(watching_t& watch)
{
	CancelIo(watch.fd);
	CloseHandle(watch.fd);
	CloseHandle(watch.overlap.hEvent);
	watch.fd = INVALID_HANDLE_VALUE;
}

void watching_t::dump(class Database* dbmgr)
{
static const DWORD USN_BLOB_CHANGE = (USN_REASON_DATA_EXTEND|
		USN_REASON_DATA_OVERWRITE|
		USN_REASON_DATA_TRUNCATION);
	DWORD ret_bytes;
	GetOverlappedResult(staff.volume_handle,
			&staff.overlap,
			&ret_bytes,
			FALSE);
	size_t left = ret_bytes - sizeof(USN);
	PUSN_RECORD record_ptr = (PUSN_RECORD)(usn_buffer.get() + sizeof(USN));  
	while (left > 0) {
		printf("USN: %I64x\n", record_ptr->Usn );
		printf("File name: %.*S\n", 
				record_ptr->FileNameLength/2, 
				record_ptr->FileName );
		printf("Reason: %x\n", record_ptr->Reason );
		printf("\n");

		unistr filename(record_ptr->FileName, record_ptr->FileNameLength/2);
		/* Process the REASON! */
		DWORD r = record_ptr->Reason;

		dentry_t dentry(vol.kpi,
				record_ptr->FileReferenceNumber, /* inode */
				record_ptr->ParentFileReferenceNumber, /* parent's inode */
				FileName, /* filename */
				record_ptr->FileNameLength/2 /* filename length, -1 for \0 term */
				);

		dbmgr->filemgr()->witness(dentry);

		if (r & USN_BLOB_CHANGE) {
			dbmgr->filemgr()->blobchange(dentry);
		} else if (r & USN_REASON_FILE_CREATE) {
			dbmgr->filemgr()->ack(dentry);
		} else if (r & USN_REASON_FILE_DELETE) {
			dbmgr->filemgr()->nak(dentry);
		} else if (r & USN_REASON_HARD_LINK_CHANGE) {
			dbmgr->filemgr()->existance_flip(dentry);
		} else if (r & USN_REASON_RENAME_NEW_NAME) {
			dbmgr->filemgr()->rename(dentry);
		} else if (r & USN_REASON_REPARSE_POINT_CHANGE) {
			dbmgr->filemgr()->symlinkchange(dentry);
		}

		left -= record_ptr->RecordLength;

		// Find the next record
		record_ptr = (PUSN_RECORD)(((char*)record_ptr) + 
				record_ptr->RecordLength); 
	}
	usn_param.StartUsn = *(USN *)staff.usn_buffer.get();
	lastusn = usn_param.StartUsn;

	if (check && lastusn >= usn_meta.NextUsn - 1) {
		dbmgr->filemgr()->checkdone(vol.kpi);
		dbmgr->volmgr()->update_ntfsext(vol.kpi, lastjid, lastusn);
		dbmgr->final_transaction();
	}
	dispach_read();
}

native_fd watching_t::waitobj()
{
	return overlap.hEvent;
}
