#ifndef _REMUS_CORE_INT_TYPE_
#define _REMUS_CORE_INT_TYPE_

#include <stdint.h> // as we're using VC10, it's unnecessary to define our own stdint.h

typedef uint32_t u32;
typedef int32_t i32;
typedef uint64_t u64;
typedef int64_t i64;

#ifdef _WIN32
#define EXPORT_TO_DLL __declspec(dllexport)
#endif

#endif
