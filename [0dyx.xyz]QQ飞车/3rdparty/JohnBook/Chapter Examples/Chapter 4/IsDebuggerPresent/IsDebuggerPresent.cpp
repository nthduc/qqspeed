/*----------------------------------------------------------------------
Debugging Applications for Microsoft .NET and Microsoft Windows
Copyright © 1997-2003 John Robbins -- All rights reserved.
----------------------------------------------------------------------*/
#include "stdafx.h"

BOOL AntiHackIsDebuggerPresent ( void )
{
    BOOL bRet = TRUE ;
    __asm
    {
        // Get the Thread Information block (TIB).
        MOV       EAX , FS:[00000018H]
        // 0x30 bytes into the TIB is a pointer field that
        // points to a structure related to debugging.
        MOV       EAX , DWORD PTR [EAX+030H]
        // The second WORD in that debugging structure indicates
        // the process is being debugged.
        MOVZX     EAX , BYTE PTR [EAX+002H]
        // Return the result.
        MOV       bRet , EAX
    }
    return ( bRet ) ;
}

void main ( void )
{
	if ( AntiHackIsDebuggerPresent ( ) )
	{
	    _tprintf ( _T ( "Being debugged!!\n" ) ) ;
	}
	else
	{
	    _tprintf ( _T ( "Not being debugged.\n" ) ) ;
	}
}

