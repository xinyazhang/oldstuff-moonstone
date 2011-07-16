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

extern std::vector<path_internal> tracing_paths;

#endif
