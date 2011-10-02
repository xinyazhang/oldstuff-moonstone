#ifndef TRACING_H
#define TRACING_H

#include <vector>
#include "version.h"
#include "../pal/unistr.h"
#include "partition.h"
#include "journal.h"
#include "opened.h"

enum JOURNAL_STATUS
{
	JOURNAL_HAVENT_DETECTED = 0,
	JOURNAL_DISABLED = 1,
	JOURNAL_ENABLED = 2
};

struct path_internal
{
	partition_sptr			partition;
	std::vector<unistr>		paths;
	std::vector<unistr>		adding_paths;
	std::vector<unistr>		removing_paths;

	/* Serializable staff */
	JOURNAL_STATUS			journal_status;
	journal_t				journal;

	/* In-memory PD staff */
	opened_t				tracing_staff;
};

typedef boost::shared_ptr<path_internal> path_internal_sptr;

#if MILESTONE >= 3
typedef std::vector<path_internal_sptr> tracing_paths_t;
extern tracing_paths_t tracing_paths;

void tracing_blank();
void tracing_start();
void tracing_stop();
void tracing_commit_changes();
#endif

#endif
