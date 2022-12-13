// -------------------------------------------------------------------------
//	文件名		：	TraceStack.cpp
//	创建者		：	王清明
//	创建时间	：	2003-1-27 20:02:46
//	功能描述	：	
//
// -------------------------------------------------------------------------

#include "stdafx.h"
#include "TraceStack.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static BOOL g_bSymIsInit = FALSE;

static DWORD __stdcall GetModBase ( HANDLE hProcess , DWORD dwAddr )
{
	// Check in the symbol engine first.
	IMAGEHLP_MODULE stIHM ;

	// This is what the MFC stack trace routines forgot to do so their
	//  code will not get the info out of the symbol engine.
	stIHM.SizeOfStruct = sizeof ( IMAGEHLP_MODULE ) ;

	if (SymGetModuleInfo (hProcess, dwAddr , &stIHM ) )
	{
		return ( stIHM.BaseOfImage ) ;
	}
	else
	{
		// Let's go fishing.
		MEMORY_BASIC_INFORMATION stMBI ;

		if ( 0 != VirtualQueryEx(hProcess, (LPCVOID)dwAddr, &stMBI, sizeof(stMBI)))
		{
			// Try and load it.
			DWORD dwNameLen = 0 ;
			TCHAR szFile[ MAX_PATH ] ;
			memset(szFile, 0, sizeof(szFile));
			HANDLE hFile = NULL ;

			dwNameLen = GetModuleFileName((HINSTANCE)stMBI.AllocationBase, szFile, MAX_PATH);
			if ( 0 != dwNameLen )
			{
				hFile = CreateFile(szFile, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, 0) ;
				SymLoadModule(hProcess, hFile,(dwNameLen ? szFile : NULL),NULL,(DWORD)stMBI.AllocationBase,0);
				CloseHandle(hFile);
			}
			return ( (DWORD)stMBI.AllocationBase ) ;
		}
	}
	return ( 0 ) ;
}

#define STACK_LEN 4000
static void __stdcall ConvertAddress (HANDLE hProcess, CString& strAppend, DWORD dwAddr, DWORD dwParm1, DWORD dwParm2,DWORD dwParm3,DWORD dwParm4)
{
	char szTemp [ STACK_LEN + sizeof ( IMAGEHLP_SYMBOL ) ] ;

	PIMAGEHLP_SYMBOL pIHS = (PIMAGEHLP_SYMBOL)&szTemp ;

	IMAGEHLP_MODULE stIHM ;

	ZeroMemory ( pIHS , STACK_LEN + sizeof ( IMAGEHLP_SYMBOL ) ) ;
	ZeroMemory ( &stIHM , sizeof ( IMAGEHLP_MODULE ) ) ;

	pIHS->SizeOfStruct = sizeof ( IMAGEHLP_SYMBOL ) ;
	pIHS->Address = dwAddr ;
	pIHS->MaxNameLength = STACK_LEN ;

	stIHM.SizeOfStruct = sizeof ( IMAGEHLP_MODULE ) ;

	// Always stick the address in first.
	CString strTemp;

	// Get the module name.
	if (SymGetModuleInfo (hProcess, dwAddr , &stIHM ) )
	{
		// Strip off the path.
		LPTSTR szName = _tcsrchr ( stIHM.ImageName , _T ( '\\' ) ) ;
		if ( NULL != szName )
		{
			szName++ ;
		}
		else
		{
			szName = stIHM.ImageName ;
		}
		strTemp.Format(_T("0x%08X[%X] %s: ") , stIHM.BaseOfImage, dwAddr - stIHM.BaseOfImage, szName);
		strAppend += strTemp;
	}
	else
	{
		strTemp.Format(_T ( "0x%08X " ) , dwAddr);
		strAppend += strTemp;
		
		strAppend += _T ( "<unknown module>: " );
	}

	// Get the function.
	DWORD dwDisp ;
	if (SymGetSymFromAddr (hProcess, dwAddr , &dwDisp , pIHS ) )
	{
		strAppend += pIHS->Name;
		strTemp.Format("(%d,%d,%d,%d)", dwParm1, dwParm2, dwParm3, dwParm4);
		strAppend += strTemp;

#ifdef _DEBUG
	#if (_WIN32_WINNT >= 0x0500)
		{
			// If I got a symbol, give the source and line a whirl.
			IMAGEHLP_LINE stIHL ;
			ZeroMemory ( &stIHL , sizeof ( IMAGEHLP_LINE)) ;
			stIHL.SizeOfStruct = sizeof ( IMAGEHLP_LINE ) ;

			if (SymGetLineFromAddr(hProcess, dwAddr,&dwDisp,&stIHL))
			{
				// Put this on the next line and indented a bit.
				strTemp.Format(_T("\t\t%s, Line %d"), stIHL.FileName, stIHL.LineNumber);
				strAppend += strTemp;
			}
		}
	#endif //_WIN32_WINNT >= 0x0500
#endif //_DEBUG
	}
	else
	{
		strTemp.Format("(%d,%d,%d,%d)", dwParm1, dwParm2, dwParm3, dwParm4);
		strAppend += strTemp;
	}
	
	strAppend += _T("\r\n");
}

extern inline void __stdcall GetCallStack(CString& strStack, DWORD dwLevel/*=10*/)
{
	if (dwLevel < 2)
		dwLevel = 2;
	
	CONTEXT ctx;
	memset(&ctx, 0, sizeof(ctx));
	ctx.ContextFlags = CONTEXT_FULL ;
	if (GetThreadContext(GetCurrentThread(), &ctx))
		GetCallStack(ctx, strStack, dwLevel);
}
extern inline void __stdcall GetCallStack(CONTEXT& stCtx,CString& strStack, DWORD dwLevel/*=10*/)
{
	if (dwLevel < 2)
		dwLevel = 2;
	
	HANDLE hProcess = GetCurrentProcess();

	// If the symbol engine is not initialized, do it now.
	if ( FALSE == g_bSymIsInit )
	{
		SymSetOptions(SYMOPT_UNDNAME |SYMOPT_LOAD_LINES) ;

		if (SymInitialize(hProcess, NULL, FALSE))
			g_bSymIsInit = TRUE ;
	}

	CDWordArray vAddrs ;
	STACKFRAME stFrame ;
	DWORD      dwMachine ;

	ZeroMemory ( &stFrame , sizeof ( STACKFRAME ) ) ;
	dwMachine                = IMAGE_FILE_MACHINE_I386 ;
	stFrame.AddrPC.Offset    = stCtx.Eip    ;
	stFrame.AddrPC.Mode = AddrModeFlat ;
	stFrame.AddrFrame.Offset = stCtx.Ebp    ;
	stFrame.AddrFrame.Mode   = AddrModeFlat ;

	for ( DWORD i = 0 ; i <= dwLevel ; i++ )
	{
		if (StackWalk(dwMachine,hProcess,hProcess,&stFrame,&stCtx,NULL,SymFunctionTableAccess,GetModBase, NULL))
		{
			// Also check that the address is not zero.  Sometimes StackWalk returns TRUE with a frame of zero.
			if (0 != stFrame.AddrPC.Offset)
			{
				vAddrs.Add(stFrame.AddrPC.Offset);
				vAddrs.Add(stFrame.Params[0]);
				vAddrs.Add(stFrame.Params[1]);
				vAddrs.Add(stFrame.Params[2]);
				vAddrs.Add(stFrame.Params[3]);
			}
		}
	}

	int nSize = vAddrs.GetSize();
	ASSERT(nSize % 5 == 0);
	for (int n = 0; n < nSize; n += 5)
	{
		ASSERT(n + 4 < nSize);
		CString strTemp;
		ConvertAddress(hProcess, strTemp, vAddrs[n], vAddrs[n+1],vAddrs[n+2],vAddrs[n+3], vAddrs[n+4]);
		strStack += strTemp;
	}
}
