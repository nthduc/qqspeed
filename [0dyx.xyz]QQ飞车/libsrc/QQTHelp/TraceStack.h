// -------------------------------------------------------------------------
//	文件名		：	TraceStack.h
//	创建者		：	王清明
//	创建时间	：	2003-1-27 20:02:21
//	功能描述	：	
//
// -------------------------------------------------------------------------

#ifndef _TRACE_STACK_H__
#define _TRACE_STACK_H__

#include "imagehlp.h"

#pragma comment (lib, "imagehlp.lib")
#pragma comment (lib, "version.lib")

extern inline void __stdcall GetCallStack(CONTEXT& ctStx, CString& strStack, DWORD dwLevel = 50);
extern inline void __stdcall GetCallStack(CString& strStack, DWORD dwLevel = 50);

// The API_VERSION_NUMBER define is 5 with the NT4 IMAGEHLP.H.  It is
//  7 with the November Platform SDK version.  This seems to be the only
//  reliable way to see which header is being used.
#if ( API_VERSION_NUMBER < 7 )
typedef struct _IMAGEHLP_LINE
{
	DWORD SizeOfStruct;           // set to sizeof(IMAGEHLP_LINE)
	DWORD Key;                    // internal
	DWORD LineNumber;             // line number in file
	PCHAR FileName;               // full filename
	DWORD Address;                // first instruction of line
} IMAGEHLP_LINE, *PIMAGEHLP_LINE;
#endif  // API_VERSION_NUMBER < 7

#ifndef SYMOPT_LOAD_LINES
#define SYMOPT_LOAD_LINES	0x00000010
#endif  // SYMOPT_LOAD_LINES

#endif  // _TRACE_STACK_H__


