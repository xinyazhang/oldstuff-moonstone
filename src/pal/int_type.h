#ifndef _REMUS_CORE_INT_TYPE_
#define _REMUS_CORE_INT_TYPE_

#include <stdint.h> // as we're using VC10, it's unnecessary to define our own stdint.h

#ifdef _WIN32
#define EXPORT_TO_DLL __declspec(dllexport)
#endif

#endif
