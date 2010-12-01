#include "path.h"
#include <windows.h>
#include <malloc.h>

int remove_unc(unicode* path)
{
	if (path[0] == L'\\' &&
			path[1] == L'\\' &&
			path[2] == L'?' &&
			path[3] == L'\\' )
		return 4;
	else
		return 0;
}

int is_dir(unicode* path)
{
    DWORD attr = ::GetFileAttributesW(path);
	if ( attr == INVALID_FILE_ATTRIBUTE )
		return 0; // invalid path is no meaning for our situation
	else
		return attr & FILE_ATTRIBUTE_DIRECTORY;
}

/*
 * use realpath in UNIX
 */
unicode* abspath(unicode* relpath)
{
	DWORD len = ::GetFullPathNameW(relpath, 0, NULL, 0);
	if ( len == 0 )
		return NULL;
	unicode* buf = malloc(sizeof(unicode) * (len + 1));
	DWORD val = ::GetFullPathNameW(relpath, len, buf, 0);
	if ( val == 0 )
		return NULL;
	return buf;
}

void abspath_dispose(unicode* buffer)
{
	free(buffer);
}
