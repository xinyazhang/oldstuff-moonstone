#ifndef PAL_VOLUME_H
#define PAL_VOLUME_H

#include "stdtype.h"
#include <vector>
#include <boost/uuid/uuid.hpp>
#include <map>

typedef boost::uuids::uuid uuids;
typedef std::map<unistr, unistr> volume_locator;

struct volume
{
	int64_t kpi; // Known Partition Index, an number counting from 1
	uuids uuid; // Partition's UUID
	unistr device; // Partition's device
	volume_locator locators;
};

EXPORT_TO_DLL std::vector<volume> ls_volume();

bool operator==(const volume& lv, const volume& rv)
{
	return lv.uuid == rv.uuid;
}

inline const void* uuid2bb(uuids u) { return &u.data[0]; }
unistr uuid2unistr(const uuids&);

enum FS_TYPE
{
	FILESYSTEM_NTFS,
	FILESYSTEM_EXT,
	FILESYSTEM_BTRFS,
	FILESYSTEM_GENERIC,
	FILESYSTEM_LAST
};

FS_TYPE detect_fstype(native_fd);

#endif
