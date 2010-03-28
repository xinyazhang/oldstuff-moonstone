#ifndef DATABASE_ABSTRACT_LAYER_SQLITE_PLATFORM_ABSTRACTION_H
#define DATABASE_ABSTRACT_LAYER_SQLITE_PLATFORM_ABSTRACTION_H

#include "../pal/unicode.h"
#include "../pal/stdtype.h"
#include "sqlite3.h"

#ifdef USE_INT64_INDEX
#	define sqlite3_bind_idx sqlite3_bind_int64
#	define sqlite3_column_idx sqlite3_column_int64
#else
#	define sqlite3_bind_idx sqlite3_bind_int
#	define sqlite3_column_idx sqlite3_column_int
#endif

#ifdef USE_UTF16LE
#define sqlite3_open_native sqlite3_open16
#define sqlite3_prepare_native sqlite3_prepare16_v2
#define sqlite3_bind_text_native sqlite3_bind_text16
#define sqlite3_column_native sqlite3_column_text16
#endif

#endif
