#ifndef PARTITION_H
#define PARTITION_H

#include <pal/stdtype.h>
#include <boost/uuid/uuid.hpp>
#include <boost/shared_ptr.hpp>

typedef boost::uuids:uuid uuid_t;
typedef std::map<unistr, unistr> partition_locator;

struct partition
{
	int64_t kpi; // Known Partition Index, an number counting from 1
	uuid_t uuid; // Partition's UUID
	unistr device; // Partition's device
	partition_locator locators;
};

typedef boost::shared_ptr<partition> partition_sptr

class partition_list
	:public std::vector<partition_sptr>
{
};

partition_list scan_online_partitons();
partition_sptr locate_partition(partition_list&, uuid_t);
void partition_blank();

extern int64_t kpi_last;
extern partition_list known_partitions;
extern partition_list online_partitions;

#endif
