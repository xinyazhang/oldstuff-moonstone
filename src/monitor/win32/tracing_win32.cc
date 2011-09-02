#include <vector>
#include <windows.h>
#include <boost/thread.hpp>
#include <boost/lexical_cast.hpp>
#include "../tracing.h"

/* Data part */
static struct opened_t
{
	partition_sptr partition;
	HANDLE volume_handle;
	std::vector<unistr> interested_paths;
	int handle_index;
};

static std::vector<opened_t> concerning;
static std::vector<HANDLE> waiting_handles;
static HANDLE ev_exit, ev_change;

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
/* static void unconcern_internal_path(path_internal&); */ // Future API

static void open_handles()
{
	ev_exit = general_event_create();
	ev_change = general_event_create();

	/* Merge path_internal to opened_t */
	for(std::vector<path_internal>::iterator iter = tracing_paths.begin();
			iter != tracing_paths.end();
			iter++)
	{
		concern_internal_path(*iter);
	}

	for(std::vector<opened_t>::iterator iter = concerning.begin();
			iter != concerning.end();
			iter++)
	{
		unistr volname(UT("\\\\?\\Volume{"));
		volname += boost::lexical_cast<std::wstring>(iter->partition->uuid);
		volname += UT("}\\");
		iter->volume_handle = CreateFile(volname.native(),
               GENERIC_READ | GENERIC_WRITE, 
               FILE_SHARE_READ | FILE_SHARE_WRITE,
               NULL,
               OPEN_EXISTING,
               0,
               NULL);
		if (iter->volume_handle == INVALID_HANDLE_NAME)
		{
			iter = concerning.erase(iter);
		}
	}
}

static void build_waitings()
{
	waiting_handles.clear();
	waiting_handles.push_back(ev_exit);
	waiting_handles.push_back(ev_change);

	for(std::vector<opened_t>::iterator iter = concerning.begin();
			iter != concerning.end();
			iter++)
	{
		iter->handle_idx = waiting_handles.size();
		waiting_handles.push_back(iter->volume_handle);
	}
}

static int waiting()
{
	DWORD idx = WaitForMultipleObjects(waiting_handles.size(),
			&waiting_handles[0],
			FALSE,
			INFINITE);
	return (int)idx;
}

static void release_all()
{
	CloseHandel(ev_exit);
	CloseHandel(ev_change);

	concerning.clear();
	for(std::vector<opened_t>::iterator iter = concerning.begin();
			iter != concerning.end();
			iter++)
	{
		CloseHandel(iter->volume_handle);
	}
}

void tracing_impl()
{
	open_handles();
	build_waitings();

	while (true)
	{
		int handle_idx = waiting();
		switch (handle_idx)
		{
			case TOLD_TO_EXIT:
				release_all();
				return;
			case TOLD_TRACING_CHANGE:
				adopt_changes();
				incresemental_waitings();
				break;
			default:
				collect_journal(waiting_handles[handle_idx]);
		}
	}
}
