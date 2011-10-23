#ifndef PAL_VOLUME_H
#define PAL_VOLUME_H

#include "stdtype.h"
#include <vector>
#include <boost/uuid/uuid.hpp>
#include <map>

typedef boost::uuids::uuid uuids;
typedef std::map<unistr, unistr> volume_locator;

enum VOLUME_FLAG_SHIFT
{
	VOL_KNOWN_SHIFT = 0,
	VOL_ONLINE_SHIFT = 1,
	VOL_TRACING_SHIFT = 2,
	VOL_LAST_SHIFT
};

#define VOL_KNOWN (1 << VOL_KNOWN_SHIFT)
#define VOL_ONLINE (1 << VOL_ONLINE_SHIFT)
#define VOL_TRACING (1 << VOL_TRACING_SHIFT)

enum FS_TYPE
{
	FILESYSTEM_GENERIC = 0, /* Including FAT */
	FILESYSTEM_NTFS = 1,
	FILESYSTEM_EXT = 2,
	FILESYSTEM_BTRFS = 3,
	FILESYSTEM_LAST
};

struct volume
{
	int64_t kpi; // Known Partition Index, an number counting from 1
	uuids uuid; // Partition's UUID
	int64_t status;
	int64_t filesystem;
	unistr device; // Partition's device
	std::vector<unistr> mount_points;
	volume_locator locators;
};

EXPORT_TO_DLL std::vector<volume> ls_volume();

inline bool operator==(const volume& lv, const volume& rv)
{
	return lv.uuid == rv.uuid;
}

inline const void* uuid2bb(uuids u) { return &u.data[0]; }
EXPORT_TO_DLL unistr uuid2unistr(const uuids&);
EXPORT_TO_DLL uuids unistr2uuid(const unistr&);

EXPORT_TO_DLL FS_TYPE detect_fstype(native_fd);
EXPORT_TO_DLL void detect_mount_points(volume&);

#endif
