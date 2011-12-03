#if defined(_MSC_VER)
#pragma warning(disable : 4996) // Disable deprecated std::swap_ranges, std::equal
#endif

#include "volume.h"
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/string_generator.hpp>
#include <boost/lexical_cast.hpp>
#include <windows.h>
#include <pal/stdtype.h>

unistr uuid2unistr(const uuids& uuid)
{
	return boost::lexical_cast<std::wstring>(uuid);
}

uuids unistr2uuid(const unistr& uuid_str)
{
	boost::uuids::string_generator gen;
	return gen(uuid_str);
}

FS_TYPE detect_fstype(native_fd fd)
{
	wchar_t fs_namebuf[MAX_PATH+1];
	BOOL ret = GetVolumeInformationByHandleW(fd,
			NULL,
			0,
			NULL,
			NULL,
			NULL,
			fs_namebuf,
			MAX_PATH+1);

	if (ret && !wcscmp(fs_namebuf, L"NTFS"))
		return FILESYSTEM_NTFS;

	return FILESYSTEM_GENERIC;
}

std::vector<volume> ls_volume()
{
	std::vector<volume> ret;
	wchar_t volname[MAX_PATH+1];
	HANDLE handle = FindFirstVolumeW(volname, MAX_PATH+1);

	if (handle == INVALID_HANDLE_VALUE)
		return ret;

	do
	{
/* extract UUID from \\?\Volume{4c1b02c1-d990-11dc-99ae-806e6f6e6963} */
		std::wstring uuid_str(volname, 11, 36);
		boost::uuids::string_generator gen;
		uuids uuid = gen(uuid_str);
		/* Add to list */
		volume vol;
		vol.kpi = -1;
		vol.uuid = uuid;
		vol.filesystem = 0;
		vol.status = VOL_ONLINE;
		ret.push_back(vol);
	} while(0 != FindNextVolumeW(handle, volname, MAX_PATH+1) );

	return ret;
}

void append_online_volume(std::vector<volume>& vollist)
{
	wchar_t volname[MAX_PATH+1];
	HANDLE handle = FindFirstVolumeW(volname, MAX_PATH+1);

	if (handle == INVALID_HANDLE_VALUE)
		return ;

	do
	{
		/* extract UUID from \\?\Volume{4c1b02c1-d990-11dc-99ae-806e6f6e6963} */
		std::wstring uuid_str(volname, 11, 36);
		boost::uuids::string_generator gen;
		uuids uuid = gen(uuid_str);

		/* Add to list */
		bool exist = false;
		for(std::vector<volume>::iterator iter = vollist.begin();
			iter != vollist.end();
			iter++)	{
				if (iter->uuid == uuid) {
					exist = true;
					iter->status |= VOL_ONLINE;
					break;
				}
		}
		
		if (!exist) {
			volume vol;
			vol.kpi = -1;
			vol.uuid = uuid;
			vol.filesystem = 0;
			vol.status = VOL_ONLINE;
			vollist.push_back(vol);
		}
	} while(0 != FindNextVolumeW(handle, volname, MAX_PATH+1) );
}


void detect_mount_points(volume& vol)
{
	vol.mount_points.clear();
	unistr volname(UT("\\\\?\\Volume{"));
	volname += uuid2unistr(vol.uuid);
	volname += UT("}\\");
	unichar buf[65536];
	DWORD len;
	if (GetVolumePathNamesForVolumeName(volname.native(),
		buf, 65536, &len)) {
			DWORD ptr = 0;
			if (buf[ptr]) {
				/* Add volume label */
				wchar_t label[MAX_PATH+1];
				label[0] = L'\0';
				GetVolumeInformationW(buf,
					label,
					MAX_PATH+1,
					NULL,
					NULL,
					NULL,
					NULL,
					NULL
					);
				vol.label = label;
				do {
					vol.mount_points.push_back(unistr(buf+ptr));
					ptr += vol.mount_points.back().size()+1;
				} while (buf[ptr]);
			}
	}
}
