#ifndef _REMUS_CORE_UNICODE_STRING_
#define _REMUS_CORE_UNICODE_STRING_

#include "unicode.h"
#include "int_type.h"
#include <string>

//typedef basic_string<uchar> unistr;

class unistr
	:public std::basic_string<uchar>
{
public:
	unistr();
	unistr(const uchar* uni);
	unistr(const std::basic_string<uchar>& us);
#ifdef REMUS_UNICODE_UTF16LE
	unistr(const char* ascii);
#endif
	// This works a little different
	// use int to construct sames to itoa
	unistr(int64_t value);
};

#endif
