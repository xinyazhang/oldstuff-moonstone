#include "UniStr.h"
#include <sstream>

UniStr::UniStr()
{
}

UniStr::UniStr(const uchar* uni)
	:basic_string<uchar>(uni)
{
}

UniStr::UniStr(const std::basic_string<uchar>& us)
	:basic_string<uchar>(us)
{
}


#ifdef REMUS_UNICODE_UTF16LE
UniStr::UniStr(const char* ascii)
{
	size_t len = strlen(ascii);
	size_t ptr = 0;
	while(ptr < len)
	{
		wchar_t wc = ascii[ptr];
		push_back(wc);
	}
}
#endif

UniStr::UniStr(int64_t value)
{
	std::basic_stringstream<uchar> ss;
	ss << value;
	this->append(ss.str());
}
