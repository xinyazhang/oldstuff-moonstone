#include "tracing.h"
#include <boost/thread.hpp>

/* Data */
std::vector<path_internal> tracing_paths;
static boost::thread* tracing_thread;

/* Additional functions implemented in <platform>/ */
void tracing_impl();
void tracing_impl_tell_exit();
void tracing_impl_tell_change();

void tracing_blank()
{
	/* Nothing to initialize ... */
}

void start_tracing()
{
#if MILESTONE >=3 && MILESTONE < 6
	/* NO Interface, tracing all partitions */
	if (tracing_paths.empty())
	{
		for(partition_list::iterator iter = online_partitions;
				iter != online_partitions.end();
				iter++)
		{
			path_internal* cur = new path_internal;
			cur->partition = *iter;
			cur->journal.journal_status = JOURNAL_HAVENT_DETECTED;
			tracing_paths.push_back(boost::shared_ptr<path_internal>(cur));
		}
	}
#endif
	tracing_thread = new boost::thread(&tracing);
}

void tracing_stop()
{
	tracing_impl_tell_exit();
	tracing_thread->join();
	delete tracing_thread;
}

void tracing_commit_changes()
{
	tracing_impl_tell_change();
}
