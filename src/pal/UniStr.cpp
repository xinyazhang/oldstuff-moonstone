#include "unistr.h"
//#include <sstream>
#include <QtCore/QString>
using std::wstring;

unistr::unistr()
{
}

unistr::unistr(const wchar_t* w)
	:wstring(w)
{
	//*this = QString::fromWCharArray(w);
}

unistr::unistr(const unistr& another)
	:wstring(another.native())
{
}

unistr::unistr(const char* ascii)
	:wstring(QString(ascii).toStdWString())
{
}

unistr unistr::number(int n)
{
	return unistr(QString::number(n).utf16());
}

#if 0
unistr::unistr(const QString& s)
	:QString(s)
{
}
#endif