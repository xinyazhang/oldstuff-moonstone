#ifndef TRACING_H
#define TRACING_H

#include <vector>
#include "../pal/unistr.h"
#include "partition.h"

struct path_internal
{
	partition_sptr 	partition;
	unistr			path;
};

void start_tracing();

#if MILESTONE >= 3
extern std::vector<path_internal> tracing_paths;
extern tracing_path_list tracing_paths;
extern tracing_object tracings;
#endif

#endif
