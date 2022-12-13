/*----------------------------------------------------------------------
Debugging Applications for Microsoft .NET and Microsoft Windows
Copyright © 1997-2003 John Robbins -- All rights reserved.
----------------------------------------------------------------------*/
#ifndef _BREAKPOINT_H
#define _BREAKPOINT_H

/*//////////////////////////////////////////////////////////////////////
                              Enumerations
//////////////////////////////////////////////////////////////////////*/
// The enumeration that indicates the breakpoint types.
enum eBreakpointType
{
    // An uninitialized BP type.
    eUninitialized  = 1 ,
    // A one-shot.  Primarily used to stop a process.  When this BP
    // fires, it is removed from the breakpoint list.
    eOneShot  ,
    // A simple location breakpoint.  No matter what thread hits this
    // breakpoint, it stops in the debugger.
    eLocation ,
    // A breakpoint with a pass count.  This breakpoint will not trigger
    // until the requisite number of executions have taken place.  NOTE:
    // I did not implement these.  If you do implement these, you should
    // derive from CLocationBP
    eCounted  ,
    // A breakpoint with an expression associated with it.  If the
    // expression evaluates to TRUE, the breakpoint will stop in the
    // debugger.  NOTE:  I did not implement these.
    eExpression
} ;

// The enumeration that indicates the breakpoint state.
enum eBreakpointState
{
    // The state that indicates a breakpoint has not been initialized.
    eNoState  = 1 ,
    // The breakpoint has been disabled by the user.
    eDisabled = 2 ,
    // If the debuggee is running, the BP is armed and ready.
    eEnabled  = 3 ,
    // The breakpoint is awaiting a trace exception so it can reset
    // and rearm.
    eWaitingForTrace = 4 ,
    // The breakpoint has not been resolved.  The debugger will need to
    // look up the name and resolve it when the debuggee starts
    // running.  Hopefully, these will turn into enabled BPs when
    // the information can be resolved.
    eUnresolved = 5
} ;

/*//////////////////////////////////////////////////////////////////////
                  The base class for all breakpoints.
//////////////////////////////////////////////////////////////////////*/
class CBreakpoint : public CObject
{
// Public constructors and destructors.
public      :
    CBreakpoint ( )
    {
        m_eState   = eUnresolved ;
        m_dwBPAddr = 0 ;
        m_opCode   = 0 ;
        m_hProc    = INVALID_HANDLE_VALUE ;
    } ;

// The basic functionality common to all breakpoints.
public      :
    // The type indicator.
    virtual eBreakpointType GetType ( void ) = 0 ;

    // The function that each breakpoint type will override to handle
    // the breakpoints.  If the return value is 1, the debugger will
    // stop.  Returning DBG_CONTINUE will tell the debugger to continue
    // executing.  A return of DBG_EXCEPTION_NOTHANDLED will route the
    // breakpoint through the normal exception trapping.
    virtual LPCVOID HandleBreakpoint ( HANDLE    hThread ,
                                       CONTEXT & stCtx    ) = 0 ;

    // This function will get called on single steps.  Each type of BP
    // must override this to do the appropriate action.  If the function
    // returns TRUE, the debugger will assume that the BP set the single
    // step and has properly reset itself.  If the function returns
    // FALSE, the debugger will continue to look for a breakpoint that
    // set the single step.
    // For those breakpoints that do handle the single step, the
    // appropriate value must be placed in pdwRet.  1 tells the debugger
    // to halt execution so that the user can perform actions on the
    // debuggee.  If it is DBG_CONTINUE or DBG_EXCEPTION_NOTHANDLED, the
    // debugger will pass them straight back onto the OS.
    virtual BOOL HandleSingleStep ( HANDLE    hThread ,
                                    CONTEXT & stCtx   ,
                                    LPDWORD   lpdwRet  ) = 0 ;

    // The function to set the location of the breakpoint.  To arm the
    // breakpoint, call ArmBreakpoint when the debuggee is running.
    virtual BOOL SetBreakpointLocation ( LPCVOID dwAddr ) ;

    // Sets the "name" (symbol or address) of the BP.  Only needed if
    // the BP needs to be displayed to the user at some point.
    virtual void SetBreakpointName ( LPCTSTR szName ) ;

    // Sets the actual breakpoint.  This assumes the debuggee is
    // loaded into memory.  SetBreakpointLocation must be called before
    // this function.
    // Returns:
    // TRUE  - Breakpoint was armed.  State becomes Enabled.
    // FALSE - There was a problem.  State remains unresolved.
    // -1    - There is a breakpoint already at the specified address.
    //         The internal state remains unresolved.
    virtual int ArmBreakpoint ( HANDLE hProc ) ;

    // Returns the address where this breakpoint is located.
    virtual LPCVOID GetBreakpointLocation ( void ) ;

    // Returns the breakpoint name assigned in SetBreakpointLocation.
    virtual LPCTSTR GetBreakpointName ( void ) ;

    // Clears the breakpoint and sets the breakpoint state to disabled.
    // This function does nothing with the registers.  It simply removes
    // the breakpoint from memory.
    virtual BOOL ClearBreakpoint ( void ) ;

    // Returns the state of this breakpoint.
    virtual eBreakpointState GetState ( void ) ;

    // Called to set the breakpoint to unresolved.  This does nothing
    // more than set the state and clear the process handle.  You will
    // need to call this when you are about to start the debuggee so
    // that you get the correct addresses set.
    virtual void SetToUnresolved ( void ) ;

    // Sets the breakpoint to disabled.  If the breakpoint is enabled,
    // it will remove it from memory.
    virtual void SetToDisabled ( void ) ;

// The data common to all breakpoints.
protected   :

    // The process for this happy BP.
    HANDLE m_hProc ;

    // The current breakpoint state.
    eBreakpointState m_eState ;

    // The breakpoint location.
    LPCVOID m_dwBPAddr ;

    // The opcode the breakpoint displaced.
    OPCODE  m_opCode ;

    // The name of the breakpoint.
    CString m_sName ;

} ;

/*//////////////////////////////////////////////////////////////////////
                        The one shot breakpoint.
//////////////////////////////////////////////////////////////////////*/
class COneShotBP : public CBreakpoint
{
public      :
    COneShotBP ( void )
    {
        m_dwRet = (LPCVOID)1 ;
    }

    COneShotBP ( const COneShotBP & cBP )
    {
        m_dwRet    = cBP.m_dwRet    ;
        m_hProc    = cBP.m_hProc    ;
        m_eState   = cBP.m_eState   ;
        m_dwBPAddr = cBP.m_dwBPAddr ;
        m_opCode   = cBP.m_opCode   ;
        m_sName    = cBP.m_sName    ;
    }
    COneShotBP & operator = ( const COneShotBP & cBP )
    {
        m_dwRet    = cBP.m_dwRet    ;
        m_hProc    = cBP.m_hProc    ;
        m_eState   = cBP.m_eState   ;
        m_dwBPAddr = cBP.m_dwBPAddr ;
        m_opCode   = cBP.m_opCode   ;
        m_sName    = cBP.m_sName    ;
        return ( *this ) ;
    }

    // The type indicator.
    virtual eBreakpointType GetType ( void )
    {
        return ( eOneShot ) ;
    }
    virtual LPCVOID HandleBreakpoint ( HANDLE    hThread ,
                                       CONTEXT & stCtx    ) ;
    virtual BOOL HandleSingleStep ( HANDLE    hThread ,
                                    CONTEXT & stCtx   ,
                                    LPDWORD   lpdwRet  ) ;

    // One-shot breakpoints are special beasts.  Therefore, in order to
    // make them a little more useable, I need to know how to continue
    // after HandleBreakpoint or HandleSingleStep is called.  By
    // default, one-shots return 1.
    void SetReturn ( LPCVOID dwRet )
    {
        m_dwRet = dwRet ;
    }

protected   :
    LPCVOID m_dwRet ;
} ;

/*//////////////////////////////////////////////////////////////////////
                        The location breakpoint.
//////////////////////////////////////////////////////////////////////*/
class CLocationBP : public CBreakpoint
{
public      :
    CLocationBP ( void )
    {
        m_hSingleStepThread = INVALID_HANDLE_VALUE ;
    }

// The parts needed to allow serialization.
public      :
    DECLARE_SERIAL ( CLocationBP )
    void Serialize ( CArchive & archive ) ;

// Functions needed to get these into MFC data structures.
    CLocationBP ( const CLocationBP & cBP )
    {
        m_hSingleStepThread = cBP.m_hSingleStepThread ;
        m_hProc             = cBP.m_hProc    ;
        m_eState            = cBP.m_eState   ;
        m_dwBPAddr          = cBP.m_dwBPAddr ;
        m_opCode            = cBP.m_opCode   ;
        m_sName             = cBP.m_sName    ;
    }
    CLocationBP & operator = ( const CLocationBP & cBP )
    {
        m_hSingleStepThread = cBP.m_hSingleStepThread ;
        m_hProc             = cBP.m_hProc    ;
        m_eState            = cBP.m_eState   ;
        m_dwBPAddr          = cBP.m_dwBPAddr ;
        m_opCode            = cBP.m_opCode   ;
        m_sName             = cBP.m_sName    ;
        return ( *this ) ;
    }

    // The type indicator.
    virtual eBreakpointType GetType ( void )
    {
        return ( eLocation ) ;
    }
    virtual LPCVOID HandleBreakpoint ( HANDLE    hThread ,
                                       CONTEXT & stCtx    ) ;
    virtual BOOL HandleSingleStep ( HANDLE    hThread ,
                                    CONTEXT & stCtx   ,
                                    LPDWORD   lpdwRet  ) ;

    // Clears the single step.  Only call this function if this BP's
    // state is eWaitingForTrace
    virtual BOOL ClearSingleStep ( void ) ;

protected   :
    // The thread I set the single step on so that I could reset the
    // breakpoint.
    HANDLE m_hSingleStepThread ;
} ;


#endif  // _BREAKPOINT_H


