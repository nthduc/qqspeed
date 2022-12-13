// stdafx.cpp : source file that includes just the standard includes
//	QQTPPP2ND.pch will be the pre-compiled header
//	stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"

void QQTPPPTrace(const char *format, ...)
{
	va_list args;
	char szMsg[1024];

	int nLen = _snprintf(szMsg, 1024, "[QQTPPP] ");

	va_start(args,format);
	_vsnprintf(szMsg+nLen, sizeof(szMsg)-nLen, format, args);
    va_end(args);

	OutputDebugString(szMsg);
}

//#pragma comment(lib, "QQTHelper.lib")
#pragma comment(lib, "QQCrypt.lib")
