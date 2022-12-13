/*----------------------------------------------------------------------
Debugging Applications for Microsoft .NET and Microsoft Windows
Copyright © 1997-2003 John Robbins -- All rights reserved.
----------------------------------------------------------------------*/
#ifndef _RECORDINGENGINE_H
#define _RECORDINGENGINE_H

#include "JString.h"

class CRecordingUIOutput
{
/*//////////////////////////////////////////////////////////////////////
// Public Constructors and Destructors
//////////////////////////////////////////////////////////////////////*/
public      :
    CRecordingUIOutput ( void )
    {
    }
    virtual ~CRecordingUIOutput ( void )
    {
    }
/*//////////////////////////////////////////////////////////////////////
// Public Methods Derived Classes Must Implement
//////////////////////////////////////////////////////////////////////*/
public      :
    // Returns TRUE if the active application with focus is the
    // recording application.
    virtual BOOL RecorderHasFocus ( void ) = 0 ;

    // The recording engine calls this method when the recording as been
    // aborted.
    virtual void RecordingAborted ( void ) = 0 ;

    // Called when recording starting and stopping.
    virtual void StartingRecording ( void ) = 0 ;
    virtual void StoppingRecording ( void ) = 0 ;

    // Called when a new document is created so any common template
    // code can be dropped in.
    virtual void NewDocumentCreated ( void ) = 0 ;

    ////////////////////////////////////////////////////////////////////
    // The various actions being recorded.
    ////////////////////////////////////////////////////////////////////

    // Keystrokes or mouse movements have been recorded.  Note that all
    // values in the szStr parameter are in Tester.PlayInput format.
    virtual void Input ( LPCTSTR szStr ) = 0 ;

    // Tells how to interpret coordinates.
    //virtual void UseAbsoluteScreenCoordinates ( BOOL bUseAbsolute ) = 0;

    // Use SM_CXVIRTUALSCREEN or SM_CXSCREEN.
    virtual void UseMultipleMonitors ( BOOL bUseMM ) = 0 ;

    // Finds a particular top level window and sets focus to it.
    virtual void FindTopWindow ( LPCTSTR szClass , LPCTSTR szTitle )= 0;

    // Sets the size of a particular window.
    virtual void SetWindowSize ( int iLeft   ,
                                 int iTop    ,
                                 int iWidth  ,
                                 int iHeight  ) = 0 ;

    // Finds the particular child window and sets focus to it.
    virtual void FindChildWindowWithID ( LPCTSTR szClass ,
                                         int     iID     ,
                                         LPCTSTR szTitle  ) = 0 ;

    // Set focus to a particular application.
    virtual void SetFocus ( LPCTSTR szForeTitle   ,
                            LPCTSTR szParentTitle ,
                            LPCTSTR szChildTitle  ,
                            UINT    idFocus        ) = 0 ;

    // Takes care of adding sleep statements.
    virtual void Sleep ( DWORD dwMilliseconds ) = 0 ;

} ;

////////////////////////////////////////////////////////////////////////

// The main recording engine.
class CRecordingEngine
{
/*//////////////////////////////////////////////////////////////////////
// Public Constructors and Destructors
//////////////////////////////////////////////////////////////////////*/
public      :
    CRecordingEngine ( CRecordingUIOutput * pOutput )
    {
        ASSERT ( FALSE == IsBadReadPtr ( pOutput ,
                                         sizeof(CRecordingUIOutput*))) ;
        m_pOutput = pOutput ;

        // Set all the option defaults.
        m_bAllMouseMovements = FALSE ;
        m_bTrackFocus = TRUE ;
        SetMinDragLength ( 50 ) ;
        m_bDoChildFocus = FALSE ;
        m_bMultiMon = TRUE ;

        ResetTrackingVariables ( ) ;
    }

    virtual ~CRecordingEngine ( void )
    {
    }

/*//////////////////////////////////////////////////////////////////////
// Public Member Functions
//////////////////////////////////////////////////////////////////////*/
public      :
    void ProcessJournalEvent ( int iCode , PEVENTMSG pMsg ) ;

    void AbortRecording ( BOOL bProcessKeys = TRUE ) ;

    void ChangeUIOutputClass ( CRecordingUIOutput * pOutput )
    {
        m_pOutput = pOutput ;
    }

/*//////////////////////////////////////////////////////////////////////
// Public Member Option Functions
//////////////////////////////////////////////////////////////////////*/
public      :

    // Tells how to handle multiple monitor situations.
    void MultipleMonitors ( BOOL bUseMM ) ;

    BOOL GetMultipleMonitors ( void )
    {
        return ( m_bMultiMon ) ;
    }

    // Set bAllMouse to TRUE to record every single mouse movement.  The
    // default is clicks only record clicks, and double clicks.
    void RecordAllMouseMovements ( BOOL bAllMouseMovements )
    {
        m_bAllMouseMovements = bAllMouseMovements ;
    }

    BOOL GetRecordAllMouseMovements ( void )
    {
        return ( m_bAllMouseMovements ) ;
    }

    // By default, I try to set the focus on all mouse downs, ups and
    // double clicks.  To turn that off, set bTrackFocus to TRUE.
    void RecordFocusForMouseActions ( BOOL bTrackFocus )
    {
        m_bTrackFocus = bTrackFocus ;
    }

    BOOL GetRecordFocusForMouseActions ( void )
    {
        return ( m_bTrackFocus ) ;
    }

    // To avoid tons of mouse movement events, you can set the
    // threashold to hit before generating a mouse move when dragging.
    // By default, this is 50 (pixels).  Set this value to 0 to always
    // record every movement.  For example, to test a painting program,
    // you will want to set this to 0 to get the same line flow.  Also,
    // make sure to pass TRUE to RecordAllMouseMovements.
    void SetMinDragLength ( int iMinDragLengthToStartRecording )
    {
        m_iMinDragLengthToStartRecording =
                                        iMinDragLengthToStartRecording ;
        m_iConvertedMinDragLengthX =
               //ConvertXCoordinate ( m_iMinDragLengthToStartRecording ) ;
               m_iMinDragLengthToStartRecording ;
        m_iConvertedMinDragLengthY =
               //ConvertYCoordinate ( m_iMinDragLengthToStartRecording ) ;
               m_iMinDragLengthToStartRecording ;

    }

    int GetMinDragLength ( void )
    {
        return ( m_iMinDragLengthToStartRecording ) ;
    }

    void RecordChildFocus ( BOOL bDoChildFocus )
    {
        m_bDoChildFocus = bDoChildFocus ;
    }

    BOOL GetRecordChildFocus ( void )
    {
        return ( m_bDoChildFocus ) ;
    }

/*//////////////////////////////////////////////////////////////////////
// Protected Enumerations
//////////////////////////////////////////////////////////////////////*/
protected   :
    // The states the recording engine can be in.
    enum eKeyStates
    {
        // The start state.
        eStart      ,
        // The paused state.  This is when HC_SYSMODALON comes in from
        // the hook function.  Since a system model box is up, I am
        // supposed to stop recording.
        // The only transition from this state is to eStart.
        ePaused     ,
        // The normal key state.  This is for most of the keys.
        eNormalKey  ,
        // The CTRL key was pressed, make sure the next key is not a
        // BREAK.  If it is, abort processing.  If it's not, treat it
        // as a normal key.
        eCheckBreak ,
        // The ALT key was pressed, so I need to watch the TABS.
        eIsTabKey   ,
        // ALT+TABbing is going on.  This state allows me to generate
        // the SetFocus events which is safer than the random
        // %({TAB}{TAB}) in scripts.
        eAltTabbingAway ,
        // The next keystroke after the ALT TAB window goes away will
        // generate the SetFocus method.
        eDetermineFocus ,
    } ;

    // The enumeration for each mouse button.
    enum eButtonEnum
    {
        eLeft       = 0 ,
        eRight      = 1 ,
        eMiddle     = 2 ,
        eX1         = 3 ,
        eX2         = 4 ,
        eMaxButton  = 5 ,
    } ;

    // The mouse recording states.
    enum eMouseStates
    {
        // The start state.
        eMouseStart                 ,
        // A mouse button went down and could be the first click of a
        // double click.
        eMouseLookForUp             ,
        // The mouse is dragging something on the screen.
        eMouseDragging              ,
        // A full click has happened and I need to see if the next down
        // is the start of a double click.
        eMouseLookForDblClickDown   ,
    } ;

/*//////////////////////////////////////////////////////////////////////
// Protected Structure Definitions
//////////////////////////////////////////////////////////////////////*/
protected   :
    typedef struct tag_MOUSEBUTTONSTATE
    {
        // The button current state.
        eMouseStates eCurrButtonState ;
        // TRUE if last state was down.
        BOOL         bIsDown    ;
        // The event messages for tracking clicks and double clicks.
        EVENTMSG     aEvtMsgs[ 3 ] ;
    } MOUSEBUTTONSTATE , * LPMOUSEBUTTONSTATE ;

/*//////////////////////////////////////////////////////////////////////
// Protected Member Keyboard State Functions
//////////////////////////////////////////////////////////////////////*/
protected   :
    // The start state processing.
    void StartState ( PEVENTMSG pMsg );
    // A CTRL key went down.  Check that the next key is not a BREAK
    // key.  If it's a break key, that will about recording.
    void CheckBreakState ( PEVENTMSG pMsg ) ;
    // The normal key state processing.
    void NormalKeyState ( PEVENTMSG pMsg ) ;
    // An ALT key went down.  Monitor TAB situation.
    void CheckAltTabs ( PEVENTMSG pMsg ) ;
    // Process the ALT+TABS.
    void ProcessAltTabbing ( PEVENTMSG pMsg ) ;
    // Do the set focus handling after an ALT+TAB.
    void ProcessKeyboardSetFocus ( void ) ;

/*//////////////////////////////////////////////////////////////////////
// Protected Member Mouse State Functions
//////////////////////////////////////////////////////////////////////*/
protected   :
    // The start state processing for mouse events.
    void MouseStartState ( eButtonEnum        eButtonAction ,
                           LPMOUSEBUTTONSTATE pState        ,
                           PEVENTMSG          pMsg           ) ;

    // The look for up state processing.
    void MouseLookForUpState ( eButtonEnum        eButtonAction ,
                               LPMOUSEBUTTONSTATE pState        ,
                               PEVENTMSG          pMsg           ) ;

    // Potential double click on the horizon.
    void MouseLookForDblClickDown ( eButtonEnum        eButtonAction ,
                                    LPMOUSEBUTTONSTATE pState        ,
                                    PEVENTMSG          pMsg           );

    // Some mouse dragging is going on.
    void MouseDrag ( eButtonEnum        eButtonAction ,
                     LPMOUSEBUTTONSTATE pState        ,
                     PEVENTMSG          pMsg           ) ;

/*//////////////////////////////////////////////////////////////////////
// Protected Helper Methods
//////////////////////////////////////////////////////////////////////*/
protected   :
    // Does the work to record the set focus operation.
    int DoSetFocus ( void ) ;

    // Handles doing the keystroke output.
    void DoKeyStrokes ( BOOL bForceOutput = FALSE ) ;

    // Resets all the tracking variables.
    void ResetTrackingVariables ( void )
    {
        eCurrKeyState = eStart ;
        ePrevKeyState = eStart ;

        // ALT+TAB monitoring.
        m_bIgnoreAltTab = FALSE ;

        // The shift states.
        m_bShiftDown = FALSE ;
        m_bShiftDownInString = FALSE ;

        // Alt key tracking.
        m_bAltDownInStream = FALSE ;

        // The CTRL key tracking.
        m_bCtrlDownInStream = FALSE ;

        m_iLastMouseX = 0 ;
        m_iLastMouseY = 0 ;

        m_bMouseDown = FALSE ;

        m_cKeyBuff.Empty ( ) ;
        m_iKeyBuffKeys = 0 ;

        ZeroMemory ( &m_aButtonStates                         ,
                     sizeof ( MOUSEBUTTONSTATE ) * eMaxButton  ) ;
        for ( int i = 0 ; i < eMaxButton ; i++ )
        {
            m_aButtonStates[i].eCurrButtonState = eMouseStart ;
            m_aButtonStates[i].bIsDown = FALSE ;
        }
        m_eLastButtonWithAction = eMaxButton ;

        // Get the double click time.
        m_dwDoubleClickTime = GetDoubleClickTime ( ) ;
        // Get half the double click box area to make my checking
        // easier.
        m_dwHalfDblClickWidth = GetSystemMetrics ( SM_CXDOUBLECLK ) / 2;
        m_dwHalfDblClickHeight = GetSystemMetrics ( SM_CYDOUBLECLK ) /2;

        m_hWndTop   = 0 ;
        m_hWndChild = 0 ;
    }

    // The mouse action handlers.
    void MouseMove ( PEVENTMSG pMsg ) ;
    void ButtonDown ( eButtonEnum eButton , PEVENTMSG pMsg ) ;
    void ButtonUp ( eButtonEnum eButton , PEVENTMSG pMsg ) ;
    void ButtonClick ( eButtonEnum eButton , PEVENTMSG pMsg ) ;
    void ButtonDblClick ( eButtonEnum eButton , PEVENTMSG pMsg ) ;
    void MouseWheel ( PEVENTMSG pMsg ) ;

    // Function checks if I'm only supposed to be recording on the
    // primary monitor and if the point is off the primary monitor, it
    // aborts recording.
    BOOL CheckIfPointOnPrimaryMonitor ( int x , int y ) ;

/*//////////////////////////////////////////////////////////////////////
// Private Class Data Members
//////////////////////////////////////////////////////////////////////*/
private     :

    // The output class this instance will use.
    CRecordingUIOutput * m_pOutput ;

    // The buffer used to build up the current key string.
    JString m_cKeyBuff ;
    // The number of keys that are in m_cKeyBuff.  I can't count
    // characters because things line {HOME}.
    int m_iKeyBuffKeys ;

    // The current keyboard state.
    eKeyStates eCurrKeyState ;
    // The previous keyboard state.
    eKeyStates ePrevKeyState ;

    // The current mouse state.
    eMouseStates eCurrMouseState ;

    // ALT+TAB monitoring.
    BOOL m_bIgnoreAltTab ;

    // The mouse processing had to put an {ALT DOWN} in the stream so
    // I need to put in the {ALT UP} in the start state as soon as I
    // see it come through.
    BOOL m_bAltDownInStream ;
    // Same for CTRL keys and the mouse....
    BOOL m_bCtrlDownInStream ;

    // Indicates the shift key is down.
    BOOL m_bShiftDown ;
    // The {SHIFT DOWN} is already in the keystroke string.
    BOOL m_bShiftDownInString ;

    // All the option values.
    BOOL m_bAllMouseMovements ;
    BOOL m_bTrackFocus ;
    int m_iMinDragLengthToStartRecording ;
    BOOL m_bDoChildFocus ;
    BOOL m_bMultiMon ;

    // Last mouse positions to cut down duplicate movement messages.
    //int m_iLastMouseX ;
    UINT m_iLastMouseX ;
    //int m_iLastMouseY ;
    UINT m_iLastMouseY ;

    // A mouse button is down.  If so, all mouse movements are recorded.
    BOOL m_bMouseDown ;

    // The minimum distances to move to start dragging.
    int m_iConvertedMinDragLengthX ;
    int m_iConvertedMinDragLengthY ;

    // The mouse state machines for each button.  Indexes come from the
    // eButtonEnum enumeration.
    MOUSEBUTTONSTATE m_aButtonStates[ eMaxButton ] ;

    // The last button that had an action.
    eButtonEnum m_eLastButtonWithAction ;

    // The system constants.
    // The time a double click is supposed to take.
    DWORD m_dwDoubleClickTime ;
    // Half the width of the double click box
    DWORD m_dwHalfDblClickWidth ;
    // Half the height of the double click box.
    DWORD m_dwHalfDblClickHeight ;

    // The last top level and child window I set focus to.
    HWND m_hWndTop ;
    HWND m_hWndChild ;
} ;



#endif  // _RECORDINGENGINE_H


