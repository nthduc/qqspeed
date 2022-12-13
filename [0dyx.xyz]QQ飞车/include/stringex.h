/*

	Ultra-lightweight std::string additions. (Should be) UNICODE compatible.

	(C) Jerry Evans 2002

	jerry@chordia.co.uk


	Canonical usage


	Chordia::CStringEx str;

	// gee this is a contrived example !
	str.Format("   %  ",1);
	str.TrimRight();
	str.TrimLeft();

	// there is no implicit conversion operator
	CString cs = str.c_str();

*/

#ifndef STRINGEX_H
#define STRINGEX_H

#include <string>
#include <algorithm>
#include <vector>
#include <stdio.h>

#ifndef UNICODE
#ifndef _TEXT
#define _TEXT(T) T
#endif
#endif

//改了基类兼容unicode
#ifdef _UNICODE
typedef std::wstring CStringEx_Base;
#else
typedef std::string CStringEx_Base;
#endif

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

class CStringEx : public CStringEx_Base
{
	// size of Format() buffer.
	enum { _MAX_CHARS = 8096 };

	public:

	CStringEx()	{}
	~CStringEx()	{}

	// various constructor flavours
	CStringEx(const CStringEx& arg);
	CStringEx(const std::string& arg);
	CStringEx(const TCHAR* pArg);

	// various assignment operator flavours
	CStringEx& operator=(const TCHAR* pArg);
	CStringEx& operator=(const std::string& arg);
	CStringEx& operator=(const CStringEx& arg);

	// the essential MFC member functions
	int Format(const TCHAR* szFormat,...);
	// this returns a reference so you can do things like this:
	// CStringEx query = "abc";
	// query += CStringEx().Format("%d %s",1,"abc");
	CStringEx& FormatEx(const TCHAR* szFormat,...);
	//format and return const TCHAR
	const TCHAR* FormatPt(const TCHAR* szFormat,...);
	// trim the right hand of the string of whitespace characters
	CStringEx& TrimRight(const TCHAR* pszDelimiters = _TEXT(" \t\r\n"));
    // trim the right hand of the string of whitespace characters
    CStringEx& TrimLeft(const TCHAR* pszDelimiters = _TEXT(" \t\r\n"));
    // trim leading and trailing whitespace
    CStringEx& TrimAll(const TCHAR* pszDelimiters = _TEXT(" \t\r\n"));
	// convert to upper and lower case
	CStringEx& MakeUpper();
	CStringEx& MakeLower();
};

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
inline CStringEx::CStringEx(const CStringEx& arg)
{ 
	assign(arg.c_str());
}

inline CStringEx::CStringEx(const std::string& arg)
{
	assign(arg);
}

inline CStringEx::CStringEx(const TCHAR* pArg)
{
	assign(pArg);
}

inline CStringEx& CStringEx::operator=(const TCHAR* pArg)
{
	assign(pArg);
	return (*this);
}

inline CStringEx& CStringEx::operator=(const std::string& arg)
{
	assign(arg);
	return (*this);
}

inline CStringEx& CStringEx::operator=(const CStringEx& arg)
{
	assign(arg.c_str());
	return (*this);
}

// the essential MFC member functions
inline int CStringEx::Format(const TCHAR* szFormat,...)
{
	std::vector<TCHAR> _buffer(_MAX_CHARS);
	va_list argList;
	va_start(argList,szFormat);
#ifdef _UNICODE
	int ret = _vsnwprintf(&_buffer[0],_MAX_CHARS,szFormat,argList);
#else
	int ret = _vsnprintf(&_buffer[0],_MAX_CHARS,szFormat,argList);
#endif
	va_end(argList);
	assign(&_buffer[0],ret);
	return ret;
}

// this returns a reference so you can do things like this:
// CStringEx query = "abc";
// query += CStringEx().Format("%d %s",1,"abc");
inline CStringEx& CStringEx::FormatEx(const TCHAR* szFormat,...)
{
	std::vector<TCHAR> _buffer(_MAX_CHARS);
	va_list argList;
	va_start(argList,szFormat);
#ifdef _UNICODE
	int ret = _vsnwprintf(&_buffer[0],_MAX_CHARS,szFormat,argList);
#else
	int ret = _vsnprintf(&_buffer[0],_MAX_CHARS,szFormat,argList);
#endif
	va_end(argList);
	assign(&_buffer[0],ret);
	return (*this);
}

//format and return const TCHAR
inline const TCHAR* CStringEx::FormatPt(const TCHAR* szFormat,...)
{
	std::vector<TCHAR> _buffer(_MAX_CHARS);
	va_list argList;
	va_start(argList,szFormat);
#ifdef _UNICODE
	int ret = _vsnwprintf(&_buffer[0],_MAX_CHARS,szFormat,argList);
#else
	int ret = _vsnprintf(&_buffer[0],_MAX_CHARS,szFormat,argList);
#endif
	va_end(argList);
	assign(&_buffer[0],ret);
	return c_str();
}

// trim the right hand of the string of whitespace characters
inline CStringEx& CStringEx::TrimRight(const TCHAR* pszDelimiters/* = _TEXT(" \t\r\n")*/)
{
	size_type idx = find_last_not_of(pszDelimiters);
	if (idx != std::string::npos)
	{
		erase(++idx);
	}
	return *this;
}

// trim the right hand of the string of whitespace characters
inline CStringEx& CStringEx::TrimLeft(const TCHAR* pszDelimiters/* = _TEXT(" \t\r\n")*/)
{
	size_type idx = find_first_not_of(pszDelimiters);
	if (idx != std::string::npos)
	{
		erase(0,idx);
	}
	else
	{
		erase();
	}
	return *this;
}

// trim leading and trailing whitespace
inline CStringEx& CStringEx::TrimAll(const TCHAR* pszDelimiters/* = _TEXT(" \t\r\n")*/)
{
	return TrimLeft(pszDelimiters).TrimRight(pszDelimiters);
}

inline CStringEx& CStringEx::MakeUpper()
{
	std::transform(begin(),end(),begin(),toupper);
	return (*this);
}

inline CStringEx& CStringEx::MakeLower()
{
	std::transform(begin(),end(),begin(),tolower);
	return (*this);
}

#endif
