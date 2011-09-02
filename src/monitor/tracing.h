#ifndef TRACING_H
#define TRACING_H

#include <vector>
#include "version.h"
#include "../pal/unistr.h"
#include "partition.h"

struct path_internal
{
	partition_sptr 	partition;
	unistr			path;
};

#if MILESTONE >= 3
extern std::vector<path_internal> tracing_paths;

void tracing_blank();
void tracing_start(); /* Implemented in <platform>/ */
void tracing_stop();
#endif

#endif
