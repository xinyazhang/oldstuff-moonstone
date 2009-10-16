#ifndef _REMUS_CORE_UNICODE_STRING_
#define _REMUS_CORE_UNICODE_STRING_

#include "unicode.h"
#include "int_type.h"
#include <string>

//typedef basic_string<uchar> UniStr;

class UniStr
	:public std::basic_string<uchar>
{
public:
	UniStr();
	UniStr(const uchar* uni);
	// This works a little different
	// use int to construct sames to itoa
	UniStr(int64_t value);
};

#endif
