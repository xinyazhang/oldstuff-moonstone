#ifndef PAL_PATH_C_H
#define PAL_PATH_C_H

#include "unicode.h"

int remove_unc(const unicode*);
int isdir(const unicode*);
unicode* abspath(const unicode*);
void abspath_dispose(unicode*);

void* open_dir(const unicode*); // XP version of POSIX's opendir
unicode* dir_buf(void* );
unicode* dir_next(void*, unicode* );
void close_dir(void* );

#endif
