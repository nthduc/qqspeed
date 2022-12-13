/*----------------------------------------------------------------------
Debugging Applications for Microsoft .NET and Microsoft Windows
Copyright © 1997-2003 John Robbins -- All rights reserved.
----------------------------------------------------------------------*/
#ifndef _SWSDLL_H
#define _SWSDLL_H

#ifdef __cplusplus
extern "C" {
#endif

/*//////////////////////////////////////////////////////////////////////
                           Defines & Typedefs
//////////////////////////////////////////////////////////////////////*/
// The defines that set up how the functions or classes are exported or
// imported.
#ifndef SWSDLL_DLLINTERFACE
#ifdef BUILDING_SWSDLL_DLL
#define SWSDLL_DLLINTERFACE __declspec ( dllexport )
#else
#define SWSDLL_DLLINTERFACE __declspec ( dllimport )
#endif  // BUILDING_SWSDLL_DLL
#endif  // SWSDLL_DLLINTERFACE

// The printf-style function for verbose output.
typedef int (__cdecl *PFNOUTPUT)( LPCTSTR szFmt , ... ) ;

// The string for the event that, if signaled, will stop the SWS runtime
// processing.
#define SWS_STOPSTART_EVENT _T ( "SWS_Start_Stop_Event" )

/*----------------------------------------------------------------------
FUNCTION        :   GenerateInitialFiles
DISCUSSION      :
    Generates the initial .SWS and .SDW files for a module.  This will
    overwrite any existing files.
PARAMETERS      :
    szModule         - The module to process.
    pfnVerboseOutput - The verbose output function.  If this is NULL, no
                       verbose output is written.
RETURNS         :
    TRUE  - The function wrote both files properly.
    FALSE - There was a problem!
----------------------------------------------------------------------*/
BOOL SWSDLL_DLLINTERFACE __stdcall
                     GenerateInitialFiles ( LPCTSTR   szModule  ,
                                            PFNOUTPUT pfnVerboseOutput);

/*----------------------------------------------------------------------
FUNCTION        :   DumpFiles
DISCUSSION      :
    Dumps the specified module's SWS or TWS file information.  If a
XXX.#.SWS file is passed as szModule (where # is a number), that
particular SWS file will be used.  If a module only is passed in, the
initial SWS file will be used.
PARAMETERS      :
    szModule         - The module to process.
    pfnOutput        - The output function.
    pfnVerboseOutput - The verbose output function.  If this is NULL, no
                       verbose output is written.
RETURNS         :
    TRUE  - The function was able to dump the information.
    FALSE - There was a problem!
----------------------------------------------------------------------*/
BOOL SWSDLL_DLLINTERFACE __stdcall
                              DumpFiles ( LPCTSTR   szModule        ,
                                          PFNOUTPUT pfnOutput       ,
                                          PFNOUTPUT pfnVerboseOutput ) ;
                                          
/*----------------------------------------------------------------------
FUNCTION        :   TuneModule
DISCUSSION      :
    Writes the .TWS file for the module.
PARAMETERS      :
    szModule         - The module to process.
    pfnVerboseOutput - The verbose output function.  If this is NULL, no
                       verbose output is written.
RETURNS         :
    TRUE  - The function wrote the .TWS and .PRF files properly.
    FALSE - There was a problem!
----------------------------------------------------------------------*/
BOOL SWSDLL_DLLINTERFACE __stdcall
                     TuneModule ( LPCTSTR   szModule         ,
                                  PFNOUTPUT pfnVerboseOutput  ) ;

#ifdef __cplusplus
}
#endif
                                            
#endif  // _SWSDLL_H