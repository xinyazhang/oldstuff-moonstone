#ifndef _LIB_OPERATION_ABSTRACTION_
#define _LIB_OPERATION_ABSTRACTION_

#include "UniStr.h"

#ifdef _WIN32
#include <windows.h>

typedef HMODULE libmodule;
#endif

libmodule pal_loadlib(const unistr&);
bool pal_module_test(libmodule);
void* pal_getfp(libmodule, const unistr&);
bool pal_module_test_fp(void* );
libmodule pal_invalidmodule();


#endif
