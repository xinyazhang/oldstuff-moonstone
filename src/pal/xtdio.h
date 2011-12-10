#ifndef XTDIO_H
#define XTDIO_H

#include "unicode.h"

#ifdef _MSC_VER
#define xvsnprintf _vsnwprintf
#define xfprintf fwprintf
#define XTDIO_WRITE_TEXT_FLAG L"ab+"
#define xfopen(x, y) _wfopen(UT(x), (y))
#else
#define xvsnprintf vsnprintf
#define xprintf fprintf
#define XTDIO_WRITE_TEXT_FLAG "a+"
#define xfopen(x,y) fopen((x), (y))
#endif

#endif
