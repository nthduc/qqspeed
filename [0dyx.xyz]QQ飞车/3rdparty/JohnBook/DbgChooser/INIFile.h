/*---------------------------------------------------------------------
    Debugging .NET and Windows-based Applications - John Robbins
---------------------------------------------------------------------*/
#ifndef __INIFILE_H
#define __INIFILE_H

// The INI filename.
#define INI_FILE            _T ( "DbgChooser.ini" )
// The Section name.
#define INI_SECNAME         _T ( "AeDebug" )
// The VC Debugger key.
#define INI_KEY_VC          _T ( "VCDebugger" )
// The WinDBG Debugger key.
#define INI_KEY_WINDBG      _T ( "WinDBGDebugger" )
// The Dr. Watson Debugger key.
#define INI_KEY_DRWATSON    _T ( "DrWatsonDebugger" )
// The VC7 Debugger Key.
#define INI_KEY_VCSEVEN     _T ( "VCSevenDebugger" )

// The defaults for the various debugger options.  Note: these assume
// the programs are in the path.

#define VC_DEFAULT          _T ( "msdev.exe -p %ld -e %ld" )
#define WINDBG_DEFAULT      _T ( "windbg -p %ld -e %ld" )
#define DRWATSON_DEFAULT    _T ( "drwtsn32 -p %ld -e %ld -g" )
#define VCSEVEN_DEFAULT                                  \
    _T ( "\"c:\\Program Files\\Common Files\\Microsoft " ) \
    _T ( "Shared\\VS7Debug\\vs7jit.exe\" -p %ld -e %ld" )
    
// The default minimum amount of time to wait for the the spawned 
// debugger to crank up before ending (in seconds)
#define MIN_SPAWN_WAIT_TIME 8
#define INI_SPAWN_WAIT_KEY  _T ( "MinWaitTime" ) 



// The functions to get the individual debugger command lines.
BOOL GetVCDebugger ( LPTSTR szBuff , int iLen ) ;
BOOL GetWinDBGDebugger ( LPTSTR szBuff , int iLen ) ;
BOOL GetDrWatsonDebugger ( LPTSTR szBuff , int iLen ) ;
BOOL GetVCSevenDebugger ( LPTSTR szBuff , int iLen ) ;

// The functions to set the individual debugger command lines.
BOOL SetVCDebugger ( LPCTSTR szBuff ) ;
BOOL SetWinDBGDebugger ( LPCTSTR szBuff ) ;
BOOL SetDrWatsonDebugger ( LPCTSTR szBuff ) ;
BOOL SetVCSevenDebugger ( LPCTSTR szBuff ) ;

// The function to get the minimum amount of time to wait after 
// spawning the debugger.
int SpawnPauseWait ( void ) ;


#endif  // __INIFILE_H
