#ifndef _REMUS_CORE_UNICODE_STRING_
#define _REMUS_CORE_UNICODE_STRING_

#include "unicode.h"
#include "int_type.h"
//#include <string>
#include <vector>
#include <QtCore/QString>

//typedef basic_string<uchar> unistr;

class EXPORT_TO_DLL unistr
	:public std::wstring
	//:public QString
{
public:
	unistr();
	unistr(const wchar_t* uni);
	unistr(const std::wstring& stdstr):std::wstring(stdstr) {}
	unistr(const unistr& another);
	unistr(const char* ascii);
	unistr(const QString&);
	//unistr(const QString& );
	//bool empty() const {return isEmpty();}

	const unichar* native() const { return c_str(); }

	unistr& operator+=(const char* p) { this->append(unistr(p)); return *this; }
	unistr& operator+=(const unistr& another) { this->append(another.native()); return *this; }
	bool operator==(const char* p) const { return *this==unistr(p); }

	operator QString() const { return QString::fromWCharArray(native()); }

	static unistr number(int n);
	// This works a little different
	// use int to construct sames to itoa
};

class unistr_list
	:public std::vector<unistr>
{
public:
	//void push_back(const unistr& u) { push_back(u); }

	unistr_list() {}
	unistr_list(const unistr& u) { push_back(u); }
	template<typename T>
		unistr_list(T cs) { push_back(unistr(cs)); }
};

inline unistr operator+(const unistr& s1, const unistr& s2)
{
	unistr ret(s1);
	return ret+=s2;
}

template
<typename T1>
const unistr operator+(T1 s1, const unistr& s2)
{
	unistr ret(s1);
	return ret+=s2;
}

template
<typename T>
const unistr operator+(const unistr& s1, const T s2)
{
	unistr ret(s1);
	return ret+=unistr(s2);
}

#endif
