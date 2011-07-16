#include "partition.h"
#include <algorithm>

int64_t kpi_last;
partition_list known_partitions;
partition_list online_partitions;

partition_sptr locate_partition(partition_list& list, const uuid_t& uuid)
{
	partition_list::iterator iter = list.begin();
	for(; iter != list.end(); iter++)
		if ((*iter)->uuid == uuid)
			return *iter;

	partition_sptr part = partition_sptr(new partition);
	part->kpi = kpi_last++;
	part->uuid = uuid;

	list.push_back(part);
	return part;
}

void partition_blank()
{
	kpi_last = 1;
}
