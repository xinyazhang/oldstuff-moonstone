#include "path.h"
#include <windows.h>

int remove_unc(const unichar* path)
{
	if (path[0] == L'\\' &&
			path[1] == L'\\')
	{
		if ( path[2] == L'?' && path[3] == L'\\' )
			return 4;
		else
			return 2;
	} else
		return 0;
}

int isdir(const unichar* path)
{
    DWORD attr = GetFileAttributesW(path);
	if ( attr == INVALID_FILE_ATTRIBUTES )
		return 0; // invalid path is no meaning for our situation
	else
		return attr & FILE_ATTRIBUTE_DIRECTORY;
}

/*
 * use realpath in UNIX
 */
unichar* abspath(const unichar* relpath)
{
	DWORD len = GetFullPathNameW(relpath, 0, NULL, 0);
	unichar* buf;
	DWORD val;
	if ( len == 0 )
		return NULL;
	buf = (unichar*)malloc(sizeof(unichar) * (len + 1));
	val = GetFullPathNameW(relpath, len, buf, 0);
	if ( val == 0 )
		return NULL;
	return buf;
}

void abspath_dispose(unichar* buffer)
{
	free(buffer);
}

struct dir_handle_internal
{
	HANDLE find_file;
	WIN32_FIND_DATAW find_data;
};

dir_handle open_dir(const unichar* path)
{
	struct dir_handle_internal* handle = (struct dir_handle_internal*)
		malloc(sizeof(struct dir_handle_internal));
	handle->find_file = FindFirstFileW(path, &handle->find_data);
	return handle;
}

int valid_dir_entry(dir_entry_data _entry)
{
	return _entry != NULL;
}

dir_entry_data dir_buf(dir_handle _handle)
{
	struct dir_handle_internal* handle = (struct dir_handle_internal*)_handle;
	if ( handle->find_file != INVALID_HANDLE_VALUE )
		return &handle->find_data;
	else
		return NULL;
}

dir_entry_data dir_next(dir_handle _handle)
{
	struct dir_handle_internal* handle = (struct dir_handle_internal*)_handle;
	
	if ( FindNextFileW(handle->find_file, &handle->find_data) != 0 )
		return &handle->find_data;
	else
		return NULL;
}

void close_dir(dir_handle _handle)
{
	struct dir_handle_internal* handle = (struct dir_handle_internal*)_handle;

	FindClose(handle->find_file);
	free(handle);
}
