/*----------------------------------------------------------------------
Debugging Applications for Microsoft .NET and Microsoft Windows
Copyright © 1997-2003 John Robbins -- All rights reserved.
----------------------------------------------------------------------*/
#ifndef _DOMEMORYREAD_H
#define _DOMEMORYREAD_H

BOOL
DoMemoryRead(
    PDEBUGPACKET  dp,
    LPVOID        addr,
    LPVOID        buf,
    DWORD         size,
    LPDWORD       lpcb
    ) ;


#endif  // _DOMEMORYREAD_H

