#include "../partition.h"
#include <windows.h>
#include <string.h>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/uuid_generators.hpp>

partition_list scan_online_partitons()
{
	wchar_t volname[MAX_PATH+1];
	HANDLE handle = FindFirstVolumeW(volname, MAX_PATH+1);

	if (handle == INVALID_HANDLE_VALUE)
		return partition_list();

	do
	{
/* extract UUID from \\?\Volume{4c1b02c1-d990-11dc-99ae-806e6f6e6963} */
		std::wstring uuid_str(volname, 11, 36);
		boost::uuids::string_generator gen;
		uuid_t uuid = gen(uuid_str);
		partition_sptr part = locate_partition(known_partitions, uuid);
		part->device = volname;

		online_partitions.push_back(part);
	} while(0 != FindNextVolumeW(handle, volname, MAX_PATH+1) );
}
