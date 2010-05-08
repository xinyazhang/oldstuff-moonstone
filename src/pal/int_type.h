#ifndef _REMUS_CORE_INT_TYPE_
#define _REMUS_CORE_INT_TYPE_

#ifdef _WIN32 //add missing stdint for cl
typedef unsigned __int64 uint64_t;
typedef unsigned __int32 uint32_t;
typedef unsigned __int16 uint16_t;

typedef __int64 int64_t;
typedef __int32 int32_t;
typedef __int16 int16_t;
#define EXPORT_TO_DLL __declspec(dllexport)
#endif

#endif
