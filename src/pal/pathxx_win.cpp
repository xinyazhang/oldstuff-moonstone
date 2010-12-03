#include "path.hpp"
extern "C"
{
#include "path.h"
};
#include <Windows.h>

using std::vector;

void fso_read_entry(fso_t& fso, dir_entry_data _entry)
{
	LPWIN32_FIND_DATA entry = (LPWIN32_FIND_DATA)_entry;

	fso.name_ = unistr(entry->cFileName);

	LARGE_INTEGER i64value;
	i64value.LowPart = entry->nFileSizeLow;
	i64value.HighPart = entry->nFileSizeHigh;
	fso.size_ = i64value.QuadPart;

	fso.mtime_ = fso.mtimer_ = i64value.QuadPart;
	fso.is_dir_ = (entry->dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY);
}
