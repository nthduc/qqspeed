/*----------------------------------------------------------------------
Debugging Applications for Microsoft .NET and Microsoft Windows
Copyright © 1997-2003 John Robbins -- All rights reserved.
----------------------------------------------------------------------*/
#include "stdafx.h"
#include "WDBG.h"

#include "Breakpoint.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


BOOL CBreakpoint :: SetBreakpointLocation ( LPCVOID dwAddr )

{
    m_dwBPAddr = dwAddr   ;

    m_eState = eUnresolved ;

    return ( TRUE ) ;
}

void CBreakpoint :: SetBreakpointName ( LPCTSTR szName )
{
    m_sName = szName ;
}

int CBreakpoint :: ArmBreakpoint ( HANDLE hProc )
{
    ASSERT ( INVALID_HANDLE_VALUE != hProc ) ;
    ASSERT ( 0 != m_dwBPAddr ) ;
    if ( ( INVALID_HANDLE_VALUE == hProc ) || ( 0 == m_dwBPAddr ) )
    {
        return ( FALSE ) ;
    }

    // Save the process handle.
    m_hProc = hProc ;

    // The parameters are good so do the work to set the breakpoint.
    DEBUGPACKET dp ;
    dp.hProcess = m_hProc ;

    int iRet = SetBreakpoint ( &dp , m_dwBPAddr , &m_opCode ) ;
    ASSERT ( FALSE != iRet ) ;
    if ( TRUE == iRet )
    {
        m_eState = eEnabled ;
    }
    else
    {
        m_eState = eUnresolved ;
    }
    return ( iRet ) ;
}

// Returns the address where this breakpoint is located.
LPCVOID CBreakpoint :: GetBreakpointLocation ( void )
{
    return ( m_dwBPAddr ) ;
}

LPCTSTR CBreakpoint :: GetBreakpointName ( void )
{
    return ( (LPCTSTR)m_sName ) ;
}

// Clears the breakpoint and sets the breakpoint state to unresolved.
BOOL CBreakpoint :: ClearBreakpoint ( void )
{
    // If the breakpoint is already disabled, just return.
    if ( ( eDisabled == m_eState        ) ||
         ( eUnresolved == m_eState      ) ||
         ( eWaitingForTrace == m_eState )    )
    {
        return ( TRUE ) ;
    }

    // Check that SetBreakpointLocation and ArmBreakpoint were called
    // before calling this function.
    ASSERT ( INVALID_HANDLE_VALUE != m_hProc ) ;
    ASSERT ( 0 != m_dwBPAddr ) ;
    if ( ( INVALID_HANDLE_VALUE == m_hProc ) || ( 0 == m_dwBPAddr ) )
    {
        return ( FALSE ) ;
    }

    // Clear the BP.
    DEBUGPACKET dp ;
    dp.hProcess = m_hProc ;

    BOOL bRet = ::ClearBreakpoint ( &dp        ,
                                    m_dwBPAddr ,
                                    m_opCode   ,
                                    FALSE       ) ;
    ASSERT ( TRUE == bRet ) ;
    m_eState = eUnresolved ;
    return ( bRet ) ;
}

// Returns the state of this breakpoint.
eBreakpointState CBreakpoint :: GetState ( void )
{
    return ( m_eState ) ;
}
void CBreakpoint :: SetToUnresolved ( void )
{
    m_eState = eUnresolved ;
}

void CBreakpoint :: SetToDisabled ( void )
{
    // Do I need to clear this BP?
    if ( eEnabled == m_eState )
    {
        ClearBreakpoint ( ) ;
    }
    m_eState = eDisabled ;
}

/*//////////////////////////////////////////////////////////////////////
                        The one shot breakpoint.
//////////////////////////////////////////////////////////////////////*/

LPCVOID COneShotBP :: HandleBreakpoint ( HANDLE    hThread ,
                                         CONTEXT & stCtx    )
{
    // The one-shot breakpoint means I need to clear out the breakpoint
    // and reset the instruction pointer so that it re-executes the
    // instruction.
    DEBUGPACKET dp ;
    dp.hProcess = m_hProc ;
    dp.hThread = hThread ;
    dp.context = stCtx ;

    if ( FALSE == ::ClearBreakpoint ( &dp        ,
                                      m_dwBPAddr ,
                                      m_opCode   ,
                                      TRUE        ) )
    {
        ASSERT ( !"ClearBreakpoint failed!" ) ;
    }
    m_eState = eDisabled ;
    return ( m_dwRet ) ;
}

BOOL COneShotBP :: HandleSingleStep ( HANDLE    /*hThread*/ ,
                                      CONTEXT & /*stCtx  */ ,
                                      LPDWORD   /*lpdwRet*/  )
{
    // This really should not happen.
    ASSERT ( !"COneShotBP :: HandleSingleStep should not be called!" ) ;
    return ( TRUE ) ;
}

/*//////////////////////////////////////////////////////////////////////
                        The location breakpoint.
//////////////////////////////////////////////////////////////////////*/
LPCVOID CLocationBP :: HandleBreakpoint ( HANDLE    hThread ,
                                          CONTEXT & stCtx    )
{
    // Handling a regular location breakpoint is pretty easy.  I need to
    // temporarily undo the breakpoint and set the single step flag.

    DEBUGPACKET dp ;
    dp.hProcess = m_hProc ;
    dp.hThread = hThread ;
    dp.context = stCtx ;

    // This clears the BP and resets the instruction pointer to
    // reexecute the instruction.
    if ( FALSE == ::ClearBreakpoint ( &dp        ,
                                      m_dwBPAddr ,
                                      m_opCode   ,
                                      TRUE        ) )
    {
        ASSERT ( !"ClearBreakpoint failed!" ) ;
    }

    // Set the state.
    m_eState = eWaitingForTrace ;

    // Set the CPU into single step mode.
    if ( FALSE == SetSingleStep ( &dp ) )
    {
        ASSERT ( !"SetSingleStep failed!" ) ;
    }

    // Save off the thread handle so I can compare it on the single
    // step.
    m_hSingleStepThread = hThread ;

    // Tell the debugger to stop so the user can look at the debuggee.
    return ( (LPCVOID)1 ) ;
}

BOOL CLocationBP :: HandleSingleStep ( HANDLE    hThread ,
                                       CONTEXT & /*stCtx  */ ,
                                       LPDWORD   lpdwRet  )
{
    // Is this the thread that hit the breakpoint and is now single
    // stepping over?
    if ( hThread != m_hSingleStepThread )
    {
        return ( FALSE ) ;
    }
    // If this BP is not the one that set the single step, move on.
    if ( eWaitingForTrace != m_eState )
    {
        return ( FALSE ) ;
    }

    // The single step just taken was the one this breakpoint just
    // set.  I need to reset and arm the breakpoint.
    DEBUGPACKET dp ;
    dp.hProcess = m_hProc ;

    int iRet = SetBreakpoint ( &dp , m_dwBPAddr , &m_opCode ) ;
    ASSERT ( FALSE != iRet ) ;
    if ( TRUE == iRet )
    {
        m_eState = eEnabled ;
    }
    else
    {
        m_eState = eDisabled ;
    }
    // Tell the debugger to continue.  This was just a housekeeping
    // stop.
    *lpdwRet = DBG_CONTINUE ;
    return ( TRUE ) ;
}

BOOL CLocationBP :: ClearSingleStep ( void )
{
    // Double check a few things.
    ASSERT ( eWaitingForTrace == m_eState ) ;
    ASSERT ( INVALID_HANDLE_VALUE != m_hProc ) ;
    ASSERT ( 0 != m_dwBPAddr ) ;
    if ( ( eWaitingForTrace != m_eState    ) ||
         ( INVALID_HANDLE_VALUE == m_hProc ) ||
         ( 0 == m_dwBPAddr                 )   )
    {
        return ( FALSE ) ;
    }

    DEBUGPACKET dp ;
    dp.hProcess = m_hProc ;
    dp.hThread = m_hSingleStepThread ;
    dp.context.ContextFlags = CONTEXT_FULL ;

    if ( FALSE == GetThreadContext ( dp.hThread , &dp.context ) )
    {
        ASSERT ( !"GetThreadContext failed" ) ;
        return ( FALSE ) ;
    }

    return ( ::ClearSingleStep ( &dp ) ) ;
}



IMPLEMENT_SERIAL( CLocationBP, CObject, 1 )

void CLocationBP :: Serialize ( CArchive & archive )
{
    // Let the base class have at it.
    CObject::Serialize ( archive ) ;

    if( archive.IsStoring ( ) )
    {
        // Never save an enabled state.  The only states that can be
        // saved are eUnresolved and eDisabled.
        eBreakpointState eOutState = m_eState ;
        if ( ( eEnabled == eOutState         ) ||
             ( eWaitingForTrace == eOutState )    )
        {
            eOutState = eUnresolved ;
        }
        archive << m_sName ;
        archive << (int&)eOutState ;
    }
    else
    {
        archive >> m_sName ;
        archive >> (int&)m_eState ;
    }
}


