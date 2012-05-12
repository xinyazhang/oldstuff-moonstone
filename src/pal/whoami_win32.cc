#include "whoami.h"
#include <windows.h>

unistr whoami()
{
	wchar_t buf[UNLEN+1];
	buf[0] = L"\0\0";
	::GetUserName(buf, UNLEN+1);
	unistr ret(buf);
	return ret;
}
