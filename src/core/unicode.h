#ifndef _REMUS_UNICODE_ABSTRACTION_
#define _REMUS_UNICODE_ABSTRACTION_

#ifdef _WIN32
#define REMUS_UNICODE_UTF16LE
#else
#define REMUS_UNICODE_UTF8
#endif

#ifdef REMUS_UNICODE_UTF16LE
// this is utf-16 on windows
// we need wchar_t for all programs

#define REMUS_UNICODE_DEFINED

#define umain wmain
#define ugetenv _wgetenv
#define uprintf wprintf
#define sqlite3_open_u sqlite3_open16
#define sqlite3_prepare_u sqlite3_prepare16_v2
#define sqlite3_column_unicode sqlite3_column_text16
typedef wchar_t uchar;

#define UT(x) L##x

#endif

#ifdef REMUS_UNICODE_UTF8
// this is utf8 
// char is enough

#define REMUS_UNICODE_DEFINED

#define umain main
#define ugetenv getenv
#define uprintf printf
typedef char uchar

#define UT(x) x

#endif

#ifndef REMUS_UNICODE_DEFINED
#error "Platform Error: This platform don't support UNICODE"
#endif

#endif
