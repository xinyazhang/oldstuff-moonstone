#ifndef PLATFORM_ABSTRACTLAYER_STDTYPE_H
#define PLATFORM_ABSTRACTLAYER_STDTYPE_H

#include "int_type.h"
#include "unistr.h"
#include <boost/shared_ptr.hpp>
using boost::shared_ptr;

typedef int64_t idx_t;
#define USE_INT64_INDEX

#ifndef UNICODE_SELECTED
#error "Please choose UTF-16LE or UTF-8
#else

#ifdef USE_UTF16LE
#include <fstream>
typedef std::wfstream filestream;
#endif

#ifdef USE_UTF8
#include <fstream>
typedef std::fstream filestream;
#endif
#endif

#ifdef _WIN32
#define native_fd void*
#else
#define native_fd int
#endif

#endif
