#ifndef PAL_PATH_C_H
#define PAL_PATH_C_H

#include "unicode.h"

extern int remove_unc(const unichar*);
int isdir(const unichar*);
unichar* abspath(const unichar*);
void abspath_dispose(unichar*);

typedef void* dir_handle;
typedef void* dir_entry_data;

dir_handle open_dir(const unichar*); // XP version of POSIX's opendir
int valid_dir_entry(dir_entry_data);
dir_entry_data dir_buf(dir_handle);
dir_entry_data dir_next(dir_handle);
void close_dir(dir_handle);

#endif