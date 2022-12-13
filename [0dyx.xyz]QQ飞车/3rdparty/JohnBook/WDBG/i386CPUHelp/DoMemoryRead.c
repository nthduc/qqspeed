/*----------------------------------------------------------------------
Debugging Applications for Microsoft .NET and Microsoft Windows
Copyright © 1997-2003 John Robbins -- All rights reserved.
----------------------------------------------------------------------*/
#include "pch.h"
#include "CPUHelp.h"
#include "DoMemoryRead.h"

BOOL
DoMemoryRead(
    PDEBUGPACKET  dp,
    LPVOID        addr,
    LPVOID        buf,
    DWORD         size,
    LPDWORD       lpcb
    )
{
    return ReadProcessMemory( dp->hProcess, addr, buf, size, lpcb );
}
