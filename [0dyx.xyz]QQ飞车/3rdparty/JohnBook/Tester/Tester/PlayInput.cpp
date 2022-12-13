/*----------------------------------------------------------------------
Debugging Applications for Microsoft .NET and Microsoft Windows
Copyright © 1997-2003 John Robbins -- All rights reserved.
----------------------------------------------------------------------*/

#include "stdafx.h"
#include "PlayInput.h"
#include "ParsePlayInputString.h"

/*//////////////////////////////////////////////////////////////////////
// File Scope Constants and Defines
//////////////////////////////////////////////////////////////////////*/

/*//////////////////////////////////////////////////////////////////////
// File Scope Types
//////////////////////////////////////////////////////////////////////*/

/*//////////////////////////////////////////////////////////////////////
// File Scope Globals
//////////////////////////////////////////////////////////////////////*/
// The flag indicating which type of coordinate system to use.  The
// default is to use absolute coordinates.
static BOOL g_bUseAbsoluteCoords = TRUE ;
// The flag to indicate if multi monitor support is enabled.
static BOOL g_bUseMultiMonitor = TRUE ;

/*//////////////////////////////////////////////////////////////////////
// File Scope Prototypes
//////////////////////////////////////////////////////////////////////*/

/*//////////////////////////////////////////////////////////////////////
// Implementation starts here.
//////////////////////////////////////////////////////////////////////*/

// See PlayKeys.h
UINT __stdcall PlayInput ( LPCTSTR szString , LPUINT  puiErrPos  )
{
    // Do the usual parameter validation happiness.
    ASSERT ( FALSE == IsBadStringPtr ( szString , 2000 ) ) ;
    ASSERT ( FALSE == IsBadWritePtr ( puiErrPos , sizeof ( UINT ) ) ) ;
    if ( ( NULL == szString                                       ) ||
         ( TRUE == IsBadWritePtr ( puiErrPos , sizeof ( UINT ) ) )   )
    {
        SetLastError ( ERROR_INVALID_PARAMETER ) ;
        return ( PI_INVALIDPARAMS ) ;
    }

    // The variables filled in by ParsePlayKeys.
    PINPUT pRetKeys = NULL ;
    UINT   uiCount  = 0 ;
    UINT   uiRet    = PI_SUCCESS ;

    // Holds the mouse ballistics.
    int    iaNewAccel[ 3 ] = { 0 , 0 , 0 } ;
    int    iaOldAccel[ 3 ] = { 0 , 0 , 0 } ;
    // Holds the before and after speeds.
    INT_PTR iOldSpeed = 1 ;
    INT_PTR iNewSpeed = 1 ;


    __try
    {

        // Let ParsePlayKeysString have a crack at it.
        uiRet = ParsePlayInputString ( szString   ,
                                       *puiErrPos ,
                                       pRetKeys   ,
                                       uiCount     ) ;
        if ( uiRet == PI_SUCCESS )
        {
            ASSERT ( NULL != pRetKeys ) ;
            ASSERT ( 0 != uiCount ) ;
            ASSERT ( FALSE == IsBadReadPtr ( pRetKeys ,
                                             sizeof(INPUT)*uiCount ) ) ;

            __try
            {
                // Save off the mouse ballistics so I can set it to
                // zero.
                BOOL bRet = SystemParametersInfo ( SPI_GETMOUSE ,
                                                   0            ,
                                                   &iaOldAccel   ,
                                                   0             ) ;
                ASSERT ( bRet ) ;
                bRet = SystemParametersInfo ( SPI_GETMOUSESPEED ,
                                              0                 ,
                                              &iOldSpeed        ,
                                              0                  ) ;
                ASSERT ( bRet ) ;
                bRet = SystemParametersInfo ( SPI_SETMOUSE      ,
                                              0                 ,
                                              &iaNewAccel        ,
                                              0                  ) ;
                ASSERT ( bRet ) ;
                bRet = SystemParametersInfo ( SPI_SETMOUSESPEED ,
                                              0                 ,
                                              (PVOID)iNewSpeed  ,
                                              0                  ) ;
                ASSERT ( bRet ) ;

                // Attempt to block input.  I don't care if this fails,
                // I'm just calling it for good measure.
                BlockInput ( TRUE ) ;

                // It is time to play.

                // I learned a lesson adding the mouse stuff.  If I pass
                // the whole array of events right to SendInput
                // directly, you can lose mouse events because you'll
                // overflow the input buffer.  It's better if I pass
                // them on one at a time so I can check for mouse
                // input.  If I see mouse input, I'll pause a bit to
                // give the input queues a chance to process the
                // message.
                for ( UINT uiCurr = 0 ; uiCurr < uiCount ; uiCurr++ )
                {
                    if ( INPUT_MOUSE == pRetKeys[uiCurr].type )
                    {
                        Sleep ( 25 ) ;
                    }
                    UINT uiSIRet = SendInput ( 1                   ,
                                               &pRetKeys[ uiCurr ] ,
                                               sizeof ( INPUT )     ) ;
                    if ( INPUT_MOUSE == pRetKeys[uiCurr].type )
                    {
                        Sleep ( 25 ) ;
                    }
                    ASSERT ( uiSIRet > 0 ) ;
                    if ( 0 == uiSIRet )
                    {
                        // SendInput failed.  Bummer.
                        uiRet = PI_ERROR ;
                        break ;
                    }
                }
            }
            __finally
            {
                // Always unblock input!!
                BlockInput ( FALSE ) ;

                // Always set the mouse ballistics back.
                BOOL bRet = SystemParametersInfo ( SPI_SETMOUSE      ,
                                                   0                 ,
                                                   &iaOldAccel        ,
                                                   0                  );
                ASSERT ( bRet ) ;
                bRet = SystemParametersInfo ( SPI_SETMOUSESPEED ,
                                              0                 ,
                                              (PVOID)iOldSpeed  ,
                                              0                  ) ;
                ASSERT ( bRet ) ;


                // Get rid of the memory allocated in
                // ParsePlayKeysString.
                delete [] pRetKeys ;
            }
        }
    }
    __except ( EXCEPTION_EXECUTE_HANDLER )
    {
        // Surely not good.
        SUPERASSERT ( FALSE ) ;
        uiRet = PI_CRASH ;
    }
    return ( uiRet ) ;
}

BOOL __stdcall SetUseAbsoluteScreenCoordinates ( BOOL bUseAbsolute )
{
    BOOL bRet = g_bUseAbsoluteCoords ;
    g_bUseAbsoluteCoords = bUseAbsolute ;
    return ( bRet ) ;
}

BOOL __stdcall GetUseAbsoluteScreenCoordinates ( void )
{
    return ( g_bUseAbsoluteCoords ) ;
}

BOOL __stdcall SetMultiMonitor ( BOOL bUseMM )
{
    BOOL bRet = g_bUseMultiMonitor ;
    g_bUseMultiMonitor = bUseMM ;
    return ( bRet ) ;
}

BOOL __stdcall GetMultiMonitor ( void )
{
    return ( g_bUseMultiMonitor ) ;
}

