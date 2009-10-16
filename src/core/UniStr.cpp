#include "UniStr.h"
#include <sstream>

UniStr::UniStr()
{
}

UniStr::UniStr(const uchar* uni)
	:basic_string<uchar>(uni)
{
}

UniStr::UniStr(int64_t value)
{
	std::basic_stringstream<uchar> ss;
	ss << value;
	this->append(ss.str());
}
