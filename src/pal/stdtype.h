#ifndef PLATFORM_ABSTRACTLAYER_STDTYPE_H
#define PLATFORM_ABSTRACTLAYER_STDTYPE_H

#include "int_type.h"
#include "ioinfo.h"
#include "unistr.h"
#include <boost/shared_ptr.hpp>
#include <boost/shared_array.hpp>

using boost::shared_ptr;
using boost::shared_array;

typedef int64_t idx_t;
#define USE_INT64_INDEX

#ifndef UNICODE_SELECTED

#	error "Please choose UTF-16LE or UTF-8

#else

#	ifdef USE_UTF16LE
#		include <fstream>
#		include <sstream>
typedef std::wfstream filestream;
typedef std::wstringstream native_sstream;
#		define nativelize_QString(str) ((unichar*)((str).utf16()))
#	endif
#	ifdef USE_UTF8
#		include <fstream>
#		include <sstream>
typedef std::fstream filestream;
typedef std::stringstream native_sstream;
#		define nativelize_QString(str) ((str).toUtf8().data())
#	endif

#endif

#ifdef _WIN32
typedef void* native_fd;
#else /* Must be a POSIX system */
typedef int native_fd;
#endif

typedef int (*cb_func_t)(void* cookie, int type, void* data);

#endif
