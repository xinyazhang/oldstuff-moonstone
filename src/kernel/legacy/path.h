#ifndef PAL_PATH_C_H
#define PAL_PATH_C_H

#include "unicode.h"
#include "int_type.h"

extern int EXPORT_TO_DLL remove_unc(const unichar*);
int EXPORT_TO_DLL isdir(const unichar*);
unichar* abspath(const unichar*);
void EXPORT_TO_DLL abspath_dispose(unichar*);

typedef void* dir_handle;
typedef void* dir_entry_data;

dir_handle EXPORT_TO_DLL open_dir(const unichar*); // XP version of POSIX's opendir
int EXPORT_TO_DLL valid_dir_entry(dir_entry_data);
dir_entry_data EXPORT_TO_DLL dir_buf(dir_handle);
dir_entry_data EXPORT_TO_DLL dir_next(dir_handle);
void EXPORT_TO_DLL close_dir(dir_handle);

#endif
