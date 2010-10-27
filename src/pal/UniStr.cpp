#include "unistr.h"
//#include <sstream>
#include <QtCore/QString>
using std::wstring;

//std::vector<unistr> unistrvectortest;
//
//class EXPORT_TO_DLL unistr_list
//	:public std::vector<unistr>
//{
//public:
//	template<typename T>
//		unistr_list(T cs) { push_back(unistr(cs)); }
//};
//
//unistr_list test("1");

unistr::unistr()
{
}

unistr::unistr(const QString& qs)
	:wstring((wchar_t*)qs.utf16())
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
//	:wstring(QString(ascii).toStdWString())
{
	*this = (wchar_t*)QString(ascii).utf16();
}

unistr unistr::number(int n)
{
	return unistr((wchar_t*)QString::number(n).utf16());
}

#if 0
unistr::unistr(const QString& s)
	:QString(s)
{
}
#endif