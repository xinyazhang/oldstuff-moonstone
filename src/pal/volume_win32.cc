#include "volume.h"
#include <boost/uuid/uuid_io.hpp>
#include <boost/lexical_cast.hpp>
#include <windows.h>
#include <pal/stdtype.h>

unistr uuid2unistr(const uuids& uuid)
{
	return boost::lexical_cast<std::wstring>(uuid);
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
	std::vector<volname> ret;
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
		volume vol;
		vol.kpi = -1;
		vol.uuid = uuid;
		ret.push_back(vol);
	} while(0 != FindNextVolumeW(handle, volname, MAX_PATH+1) );

	return ret;
}
