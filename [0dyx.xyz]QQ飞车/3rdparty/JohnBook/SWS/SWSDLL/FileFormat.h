/*----------------------------------------------------------------------
Debugging Applications for Microsoft .NET and Microsoft Windows
Copyright © 1997-2003 John Robbins -- All rights reserved.
----------------------------------------------------------------------*/
#ifndef _FILEFORMAT_H
#define _FILEFORMAT_H


/*//////////////////////////////////////////////////////////////////////
                         Defines and Structures
//////////////////////////////////////////////////////////////////////*/
// The SWS file signature (SWS2).
#define SIG_SWSFILE '2SWS'
#define EXT_SWSFILE _T ( ".SWS" )

/*//////////////////////////////////////////////////////////////////////
The SWS file header.
//////////////////////////////////////////////////////////////////////*/
typedef struct tag_SWSFILEHEADER
{
    // The file signature.  See the SIG_* defines above.
    DWORD   dwSignature ;
    // The link time for the binary associated with this file.
    DWORD   dwLinkTime ;
    // The load address for the binary.
    DWORD64 dwLoadAddr ;
    // The size of the image.
    DWORD   dwImageSize ;
    // The number of entries in this file.
    DWORD   dwEntryCount ;
    // The flags field.
    DWORD   dwFlags ;
    // The modulename for this file.
    TCHAR   szModuleName[ MAX_PATH ] ;
    DWORD   dwPadding ;
} SWSFILEHEADER , * LPSWSFILEHEADER ;

/*//////////////////////////////////////////////////////////////////////
The entry type for SWS files.
//////////////////////////////////////////////////////////////////////*/
typedef struct tag_SWSENTRY
{
    // The function address.
    DWORD64 dwFnAddr ;
    // The function size.
    DWORD   dwSize ;
    // The execution count.
    DWORD   dwExecCount ;
} SWSENTRY , * LPSWSENTRY ;

#endif  // _FILEFORMAT_H

