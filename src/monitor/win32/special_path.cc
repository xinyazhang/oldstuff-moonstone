#include <shlobj.h>

#include "../special_path.h"
#include "../daemon_error.h"

unistr locate_conf_dir()
{
	wchar_t confpath[MAX_PATH];

	HRESULT result = SHGetFolderPathW(NULL,
		CSIDL_COMMON_APPDATA,
		-1,
		SHGFP_TYPE_CURRENT,
		confpath);

	if (result != S_OK)
	{
		daemon_report_error(DERR_CANNOT_LOCATE_CONF_FILE);
		return unistr();
	}

	return unistr(confpath) + L"/lain";
}
