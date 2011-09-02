#include "tracing.h"
#include <boost/thread.hpp>

/* Data */
std::vector<path_internal> tracing_paths;
static boost::thread* tracing_thread;

/* Additional functions implemented in <platform>/ */
void tracing_impl();
void tracing_impl_tell_exit();

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
			path_internal cur;
			cur.partition = *iter;
			tracing_paths.push_back(cur);
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
