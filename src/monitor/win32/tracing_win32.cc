#include <vector>
#include <boost/thread.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/lexical_cast.hpp>
#include "../tracing.h"
#include <stdio.h>
#include <Windows.h>
#include "logusn.h"
#include "../special_path.h"
#include "../packet_handler.h"

static std::vector<HANDLE> waiting_handles;
static tracing_paths_t waiting_traces;
static HANDLE ev_exit, ev_change;
static const READ_USN_JOURNAL_DATA default_read_data = {0, 0xFFFFFFFF, FALSE, 0, 0};
static logusn_context_t* logusn_ctx;

static enum SPECIAL_OBJECT_IDS
{
	TOLD_TO_EXIT = 0,
	TOLD_TRACING_CHANGE,
	SPECIAL_END
};

static HANDLE general_event_create()
{
	return CreateEvent(NULL, TRUE, FALSE, NULL);
}

#if 0
static void concern_internal_path(path_internal& path)
{
	for(std::vector<opened_t>::iterator iter = concerning.begin();
			iter != concerning.end();
			iter++)
	{
		if (iter->partition == path.partition)
		{
			op.interested_paths.push_back(path.path);
			return ;
		}
	}
	opened_t op;
	op.partition = path.partition;
	op.volume_handle = INVALID_HANDLE_NAME;
	op.interested_paths.push_back(path.path);
	concerning.push_back(op);
}
#endif
/* static void unconcern_internal_path(path_internal&); */ // Future API

static void init_opened(path_internal_sptr vol)
{
	unistr volname(UT("\\\\?\\Volume{"));
	volname += boost::lexical_cast<std::wstring>(vol->partition->uuid);
	volname += UT("}");

	if (vol->journal_status == JOURNAL_DISABLED)
		return ;

	opened_t& tracing_staff = vol->tracing_staff;
	tracing_staff.volume_handle = CreateFileW(volname.native(),
			GENERIC_READ | GENERIC_WRITE, 
			FILE_SHARE_READ | FILE_SHARE_WRITE,
			NULL,
			OPEN_EXISTING,
			FILE_FLAG_OVERLAPPED,
			NULL);
	if (tracing_staff.volume_handle == INVALID_HANDLE_VALUE)
	{
		DWORD err = GetLastError();
		vol->journal_status = JOURNAL_DISABLED;
		return;
	}
	DWORD ret_bytes;
	BOOL bret = DeviceIoControl(tracing_staff.volume_handle,
			FSCTL_QUERY_USN_JOURNAL,
			NULL,
			0,
			&tracing_staff.cur_usn_meta,
			sizeof(tracing_staff.cur_usn_meta),
			&ret_bytes,
			NULL);
	if (!bret)
	{
		DWORD err = GetLastError();
		vol->journal_status = JOURNAL_DISABLED;
		return;
	}
	vol->journal_status = JOURNAL_ENABLED;

	tracing_staff.usn_selecting = default_read_data;
	tracing_staff.usn_selecting.UsnJournalID = tracing_staff.cur_usn_meta.UsnJournalID;	
	if (vol->journal.journal_id_last_seen != tracing_staff.cur_usn_meta.UsnJournalID)
	{
		logusn_ctx->ev_uuid(vol->partition->uuid);
		logusn_ctx->ev_jid(tracing_staff.cur_usn_meta.UsnJournalID);
		vol->journal.last_recorded_usn = 0;
	}
	vol->journal.journal_id_last_seen = tracing_staff.cur_usn_meta.UsnJournalID;

	tracing_staff.overlap.hEvent = general_event_create();
	tracing_staff.usn_buffer.swap(boost::scoped_ptr<char>(new char[JOURNAL_BUFFER_SIZE]));
}

/*
 * Open handle for:
 * 	1. Volume access
 * 	2. Corresponding event in OVERLAPPED
 * 	3. event handle
 */
static void open_handles()
{
	logusn_ctx = logusn_create_for_write(locate_conf_dir());
	ev_exit = general_event_create();
	ev_change = general_event_create();

	for(tracing_paths_t::iterator iter2sp = tracing_paths.begin();
			iter2sp != tracing_paths.end();
			iter2sp++)
	{
		init_opened(*iter2sp);
	}
}

static void build_waitings()
{
	waiting_handles.clear();
	waiting_handles.push_back(ev_exit);
	waiting_handles.push_back(ev_change);

	/* Scan the tracing list for journal-enabled partitions */
	for(tracing_paths_t::iterator iter2sp = tracing_paths.begin();
			iter2sp != tracing_paths.end();
			iter2sp++)
	{
		opened_t& staff = (*iter2sp)->tracing_staff;
		if ((*iter2sp)->journal_status != JOURNAL_ENABLED)
		{
			CloseHandle(staff.volume_handle);
			staff.volume_handle = INVALID_HANDLE_VALUE;
			staff.overlap.hEvent = INVALID_HANDLE_VALUE;
			continue;
		}
		staff.handle_index = waiting_handles.size();
		waiting_handles.push_back(staff.overlap.hEvent);
		waiting_traces.push_back(*iter2sp);
	}
}

static void async_read(opened_t& staff)
{
	BOOL ret = DeviceIoControl(staff.volume_handle, 
			FSCTL_READ_USN_JOURNAL, 
			&staff.usn_selecting,
			sizeof(staff.usn_selecting),
			staff.usn_buffer.get(),
			JOURNAL_BUFFER_SIZE,
			&staff.usn_read_bytes,
			&staff.overlap);
	if (!ret)
	{
		int err;
		err = GetLastError();
	}
}

static void async_read_all()
{
	for(tracing_paths_t::iterator iter2sp = waiting_traces.begin();
			iter2sp != waiting_traces.end();
			iter2sp++)
	{
		async_read((*iter2sp)->tracing_staff);
	}
}

static int waiting()
{
	DWORD idx = WaitForMultipleObjects(waiting_handles.size(),
			&waiting_handles[0],
			FALSE,
			INFINITE);
	return (int)(idx - WAIT_OBJECT_0);
}

static void release_all()
{
	CloseHandle(ev_exit);
	CloseHandle(ev_change);

	for(tracing_paths_t::iterator iter2sp = tracing_paths.begin();
			iter2sp != tracing_paths.end();
			iter2sp++)
	{
		CancelIo((*iter2sp)->tracing_staff.volume_handle);
		CloseHandle((*iter2sp)->tracing_staff.volume_handle);
		CloseHandle((*iter2sp)->tracing_staff.overlap.hEvent);
	}
	delete logusn_ctx;
}

static void collect_journal(path_internal_sptr path)
{
	// Log the UUID of the partition
	logusn_ctx->ev_uuid(path->partition->uuid);

	opened_t& staff(path->tracing_staff);
	DWORD ret_bytes;
	GetOverlappedResult(staff.volume_handle,
			&staff.overlap,
			&ret_bytes,
			FALSE);

	// Log the USN
	size_t left = ret_bytes - sizeof(USN);

	logusn_ctx->ev_usn_records(*(USN *)staff.usn_buffer.get(),
			staff.usn_buffer.get() + sizeof(USN),
			left);
	// Find the first record
	PUSN_RECORD record_ptr = (PUSN_RECORD)(staff.usn_buffer.get() + sizeof(USN));  

	//printf("****************************************\n");

	while( left > 0 )
	{
		printf("USN: %I64x\n", record_ptr->Usn );
		printf("File name: %.*S\n", 
				record_ptr->FileNameLength/2, 
				record_ptr->FileName );
		printf("Reason: %x\n", record_ptr->Reason );
		printf("\n");

		left -= record_ptr->RecordLength;

		// Find the next record
		record_ptr = (PUSN_RECORD)(((char*)record_ptr) + 
				record_ptr->RecordLength); 
	}
	// Update starting USN for next call
	path->journal.last_recorded_usn = 
		staff.usn_selecting.StartUsn = *(USN *)staff.usn_buffer.get();
	//SetEvent(staff.overlap.hEvent);
	async_read(staff);
}

void tracing_impl()
{
	open_handles();
	build_waitings();
	async_read_all();

	while (true)
	{
		int idx = waiting();
		switch (idx)
		{
			case TOLD_TO_EXIT:
				release_all();
				return;
			case TOLD_TRACING_CHANGE:
#if MILESTONE >= 5
				adopt_changes();
				incresemental_waitings();
#endif
				break;
			default:
				collect_journal(waiting_traces[idx - SPECIAL_END]);
		}
	}
}

void tracing_impl_tell_exit()
{
	SetEvent(ev_exit);
}

void tracing_impl_tell_change()
{
	SetEvent(ev_change);
}

static int serv_list_vol(ipc_packet* packet, ph_cookie cookie)
{
	uuid_t uuid = read_uuid(packet);
	unistr volname(UT("\\\\?\\Volume{"));
	volname += boost::lexical_cast<std::wstring>(vol->partition->uuid);
	volname += UT("}");
}

void tracing_service_init()
{
	ph_register(PT_PRIV_WIN32_NTFS_TRACING_INITIALIZE, serv_init_ntfs_tracing);
}
