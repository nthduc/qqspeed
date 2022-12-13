/*----------------------------------------------------------------------
Debugging Applications for Microsoft .NET and Microsoft Windows
Copyright © 1997-2003 John Robbins -- All rights reserved.
----------------------------------------------------------------------*/
#ifndef _FASTTRACE_H
#define _FASTTRACE_H

#pragma once

/*//////////////////////////////////////////////////////////////////////
// Constants and Defines.
//////////////////////////////////////////////////////////////////////*/
#ifndef FASTTRACE_DLLINTERFACE
#ifdef BUILDING_FASTTRACE_DLL
#define FASTTRACE_DLLINTERFACE __declspec ( dllexport )
#else
#define FASTTRACE_DLLINTERFACE __declspec ( dllimport )
#endif  // BUILDING_FASTTRACE_DLL
#endif  // FASTTRACE_DLLINTERFACE

/*//////////////////////////////////////////////////////////////////////
// Structures and Types
//////////////////////////////////////////////////////////////////////*/
// The typedef for the debugging output function.  Note that this
// matches OutputDebugString.
typedef VOID (WINAPI *PFNDEBUGOUT)(LPCTSTR lpOutputString) ;

// The structure to set or retrieve FastTrace options.
typedef struct tag_FASTTRACEOPTIONS
{
    // Set to TRUE if you want all file writing to be written through
    // and not lazily cached.  By default this is off.  If set after
    // tracing starts, any tracing files created prior will not be
    // changed.
    BOOL bWriteThrough ;
    // Set to TRUE if you want timing information for each trace.  The
    // default is off.
    BOOL bDoTimings ;
    // The debugging output function.  If you want to see output in a
    // debug build, you must explicitly set this.  For performance
    // reasons, this is off by default.
    PFNDEBUGOUT pDebugOut ;
} FASTTRACEOPTIONS , * LPFASTTRACEOPTIONS ;

/*//////////////////////////////////////////////////////////////////////
// .FLT file format information.
//////////////////////////////////////////////////////////////////////*/
// The file extension for Fast Trace files.
#define k_FTFILEEXT _T ( ".FTL" )

// The maximum CHARACTER length of an fast trace string.
#define MAX_FASTTRACE_LEN 80

// The file signature value.
#define FTFILESIG 'TSAF'
// The structure written to the beginning of all log files.
typedef struct tag_FTFILEHEADER
{
    // The signature.  See the FTFILESIG define above.
    DWORD dwSig ;
    // The total number of traces written to the file.
    LONG  lItemCount ;
    // The maximum number of characters in a FTDATA packet.
    LONG  lMaxStringCharCount ;
    // The process ID for this file.  (Used to ensure a file matches
    // it's siblings).
    LONG  lPID ;
    // The thread ID for this file.
    LONG  lTID ;
} FTFILEHEADER , * LPFTFILEHEADER ;

// The record for an individual data.  After the header somes lItemCount
// of these structures.
typedef struct tag_FTDATA
{
    // The sequence number for this trace.
    ULONG    lSeqNum ;
    // The time this trace took place.
    FILETIME stTime ;
    TCHAR    szTrace [ MAX_FASTTRACE_LEN ] ;
} FTDATA , * LPFTDATA ;

// The output functions for the dumping and processing functions.
typedef int (*PFNOUTPUT) ( LPCTSTR , ... ) ;
    
/*----------------------------------------------------------------------
FUNCTION    :   SetFastTraceOptions
DESCRIPTION :
    Sets the fast trace options.  Ideally, you'll call this before you
start tracing.
PARAMETERS  :
    pOpts - The FASTTRACEOPTIONS structure which contains the new
            settings.
RETURNS     :
    None.
----------------------------------------------------------------------*/
FASTTRACE_DLLINTERFACE void __stdcall
                SetFastTraceOptions ( LPFASTTRACEOPTIONS pOpts ) ;

/*----------------------------------------------------------------------
FUNCTION    :   GetFastTraceOptions
DESCRIPTION :
    Retrieve's the current fast trace options.
PARAMETERS  :
    pOpts - The FASTTRACEOPTIONS structure which will contain the
            settings
RETURNS     :
    None.
----------------------------------------------------------------------*/
FASTTRACE_DLLINTERFACE void __stdcall
                GetFastTraceOptions ( LPFASTTRACEOPTIONS pOpts ) ;
                
/*----------------------------------------------------------------------
FUNCTION    :   FastTrace
DESCRIPTION :
    The tracing function that handles fast logging of trace information.
PARAMETERS  :
    szFmt     - The format string for this trace.
    ...       - Values utilized by the format string.
RETURNS     :
    None.
----------------------------------------------------------------------*/
FASTTRACE_DLLINTERFACE void FastTrace ( LPCTSTR szFmt     ,
                                        ...                ) ;

/*----------------------------------------------------------------------
FUNCTION    :   FlushFastTraceFiles
DESCRIPTION :
    Flushes all FastTrace files to disk.
PARAMETERS  :
    None.
RETURNS     :
    None.
----------------------------------------------------------------------*/
FASTTRACE_DLLINTERFACE void FlushFastTraceFiles ( void ) ;

/*----------------------------------------------------------------------
FUNCTION    :   SnapFastTraceFiles
DESCRIPTION :
    Snaps out any trace files currently in use and moves them to a new
filename.  The new tracing file is opened.  This function allows you to
look at tracing in live applications.
PARAMETERS  :
    None.
RETURNS     :
    None.
----------------------------------------------------------------------*/
FASTTRACE_DLLINTERFACE void SnapFastTraceFiles ( void ) ;

#endif  // _FASTTRACE_H

