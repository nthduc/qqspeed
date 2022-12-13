/*----------------------------------------------------------------------
Debugging Applications for Microsoft .NET and Microsoft Windows
Copyright © 1997-2003 John Robbins -- All rights reserved.
----------------------------------------------------------------------*/
#ifndef _HELPER_H
#define _HELPER_H

#ifndef SYMOPT_PUBLICS_ONLY
#define SYMOPT_PUBLICS_ONLY             0x00004000
#endif

DWORD64 InitSymEngAndLoadModule ( CSymbolEngine  * pSym             ,
                                  TCHAR *          szModule         ,
                                  PFNOUTPUT        pfnVerboseOutput  );

DWORD Wide2Ansi ( const wchar_t * szWide   ,
                  char *          szANSI   ,
                  int             iANSILen  ) ;


#endif // _HELPER_H