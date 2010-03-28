#ifndef _REMUS_UNICODE_ABSTRACTION_
#define _REMUS_UNICODE_ABSTRACTION_

#define AUTOMATIC_SELECT_UNICODE

#ifdef AUTOMATIC_SELECT_UNICODE
#	ifdef _WIN32
#		define USE_UTF16LE
#		define UNICODE_SELECTED
#	else
#		define USE_UTF8
#		define UNICODE_SELECTED
#	endif
#endif

#ifdef USE_UTF16LE
#	define UT(x) L##x
#	define unichar wchar_t
#else
#	define UT(x) x
typedef char unichar;
#endif

#ifndef UNICODE_SELECTED
#error "Platform Error: This platform don't support UNICODE"
#endif

#endif
