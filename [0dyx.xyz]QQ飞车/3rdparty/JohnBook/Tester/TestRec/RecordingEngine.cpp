/*----------------------------------------------------------------------
Debugging Applications for Microsoft .NET and Microsoft Windows
Copyright © 1997-2003 John Robbins -- All rights reserved.
----------------------------------------------------------------------*/

#include "stdafx.h"
#include "RecordingEngine.h"
#include "KeyStrings.h"
#define COMPILE_MULTIMON_STUBS
#include "MultiMon.h"

/*//////////////////////////////////////////////////////////////////////
// Special Conditional Compilation
//////////////////////////////////////////////////////////////////////*/
//#define _DEBUG_REC_STATE_TRANSITIONS
//#define _DEBUG_MOUSE_REC_STATE_TRANSITIONS

// To debug state transitions I have special tracing.
#ifdef _DEBUG_REC_STATE_TRANSITIONS
#define STATETRACE TRACE
#else
#define STATETRACE   __noop
#endif

#ifdef _DEBUG_MOUSE_REC_STATE_TRANSITIONS
#define MSTATETRACE TRACE
#else
#define MSTATETRACE   __noop
#endif

/*//////////////////////////////////////////////////////////////////////
// File Scope Defines, Structures, and Constants
//////////////////////////////////////////////////////////////////////*/

const LPCTSTR k_MOUSEBUTTONTEXT[] =
{
    _T ( "LEFT"   ) ,
    _T ( "RIGHT"  ) ,
    _T ( "MIDDLE" ) ,
    _T ( "X1"     ) ,
    _T ( "X2"     )
} ;

/*//////////////////////////////////////////////////////////////////////
// Implementation Starts
//////////////////////////////////////////////////////////////////////*/

void CRecordingEngine :: ProcessJournalEvent ( int       iCode ,
                                               PEVENTMSG pMsg   )
{
#ifdef _DEBUG
    int iStrokeCount = m_iKeyBuffKeys ;
#endif

    // The first thing to do is to check the iCode value.
    if ( HC_SYSMODALON == iCode )
    {
        // The system box went away so I can continue to record.  I will
        // set the state to what it was previously.
        eCurrKeyState = ePrevKeyState ;

        // The mouse state always goes back to the start.
        eCurrMouseState = eMouseStart ;
    }
    else if ( HC_SYSMODALOFF == iCode )
    {
        // A system model box popped up, the docs say I am supposed to
        // stop recording so save the current state and shift to the
        // paused state.
        ePrevKeyState = eCurrKeyState ;
        eCurrKeyState = ePaused ;

        // TODO TODO - Gotta clean up partial mouse states!!

        eCurrMouseState = eMouseStart ;

    }
    else
    {
        ASSERT ( iCode == HC_ACTION ) ;

        // Try and figure out the focus.
        if ( eCurrKeyState == eDetermineFocus )
        {
            ProcessKeyboardSetFocus ( ) ;
        }

        // Handle any keystrokes.
        if ( ( WM_KEYDOWN    == pMsg->message ) ||
             ( WM_KEYUP      == pMsg->message ) ||
             ( WM_SYSKEYDOWN == pMsg->message ) ||
             ( WM_SYSKEYUP   == pMsg->message )    )
        {
            // This is a normal HC_ACTION so I can proceed with the
            // processing.  Based on the current state, do the
            // transition.
            switch ( eCurrKeyState )
            {
                case eStart             :
                    StartState ( pMsg ) ;
                    break ;
                case eNormalKey         :
                    NormalKeyState ( pMsg ) ;
                    break ;
                case eCheckBreak        :
                    CheckBreakState ( pMsg ) ;
                    break ;
                case eIsTabKey          :
                    CheckAltTabs ( pMsg ) ;
                    break ;
                case eAltTabbingAway    :
                    ProcessAltTabbing ( pMsg ) ;
                    break ;
                case ePaused            :
                    // Just spin until the HC_SYSMODALON state comes
                    // through.
                    break ;

                default     :
                    ASSERT ( !"Unknown recording engine state!" ) ;
                    break ;
            }
        }
        else
        {
            eButtonEnum eButtonAction = m_eLastButtonWithAction ;

            // It's a mouse action.
            switch ( pMsg->message )
            {
                case WM_LBUTTONUP   :
                    eButtonAction = eLeft ;
                    break ;
                case WM_LBUTTONDOWN :
                    eButtonAction = eLeft ;
                    break ;
                case WM_RBUTTONDOWN :
                    eButtonAction = eRight ;
                    break ;
                case WM_RBUTTONUP :
                    eButtonAction = eRight ;
                    break ;
                case WM_MBUTTONUP   :
                    eButtonAction = eMiddle ;
                    break ;
                case WM_MBUTTONDOWN :
                    eButtonAction = eMiddle ;
                    break ;
                case WM_XBUTTONDOWN :
                case WM_XBUTTONUP   :
                    // Do the X button processing here to figure out
                    // which button it is.
                    // I don't have an Explorer Mouse to figure out
                    // how to do this.
                    break ;
                case WM_MOUSEWHEEL :
                    MouseWheel ( pMsg ) ;
                    break ;
                default             :
                    break ;
            }
            if ( eMaxButton == eButtonAction )
            {
                // The only thing I'll handle in this case is a mouse
                // move.
                if ( WM_MOUSEMOVE == pMsg->message )
                {
                    MouseMove ( pMsg ) ;
                }
            }
            else
            {
                //TRACE ( "Message time : %d\n" , pMsg->time ) ;

                // Get the button state information for this button.
                LPMOUSEBUTTONSTATE pButtonState =
                                     &m_aButtonStates[ eButtonAction ] ;
                ASSERT ( NULL != pButtonState ) ;

                // Save this action button as the last one done.
                m_eLastButtonWithAction = eButtonAction ;

                switch ( pButtonState->eCurrButtonState )
                {
                    case eMouseStart                 :
                        MouseStartState ( eButtonAction ,
                                          pButtonState  ,
                                          pMsg           ) ;
                        break ;
                    case eMouseLookForUp             :
                        MouseLookForUpState ( eButtonAction ,
                                              pButtonState  ,
                                              pMsg           ) ;
                        break ;
                    case eMouseDragging              :
                        MouseDrag ( eButtonAction , pButtonState ,pMsg);
                        break ;
                    case eMouseLookForDblClickDown   :
                        MouseLookForDblClickDown ( eButtonAction ,
                                                   pButtonState  ,
                                                   pMsg           ) ;
                        break ;
                    default                          :
                        ASSERT ( !"Invalid mouse state!!" ) ;
                        break ;
                }
            }
        }
    }
#ifdef _DEBUG
    if ( iStrokeCount != m_iKeyBuffKeys )
    {
        if ( m_cKeyBuff.Length ( ) > 0 )
        {
            TRACE ( " Key buff : %S\n" , (LPCTSTR)m_cKeyBuff ) ;
        }
    }
#endif
}

void CRecordingEngine :: AbortRecording ( BOOL bProcessKeys )
{
    if ( TRUE == bProcessKeys )
    {
        if ( FALSE == m_cKeyBuff.IsEmpty ( ) )
        {
            DoKeyStrokes ( TRUE ) ;
        }
    }
    // Tell the output class that recording is ending.
    m_pOutput->RecordingAborted ( ) ;

    // Rest the recording state.
    ResetTrackingVariables ( ) ;
}

void CRecordingEngine :: StartState ( PEVENTMSG pMsg )
{
    // Here I can transition to one of three states:
    // 1.  If it's a CTRL key, eCheckBreak
    // 2.  If it's an ALT key, eIsTabKey
    // 3.  Anything else goes right to eNormalKey.
    STATETRACE ( _T("RECSTATE: Start state\n")) ;

    // The key checking obviously only happens when keys are pressed!
    if ( ( WM_KEYDOWN    == pMsg->message ) ||
         ( WM_SYSKEYDOWN == pMsg->message )   )
    {
        switch ( LOBYTE ( pMsg->paramL ) )
        {
        case VK_MENU    :
            STATETRACE (_T("RECSTATE: Looking for TAB key\n")) ;
            // Look for ALT+TABbing.
            eCurrKeyState = eIsTabKey ;
            break ;
        case VK_CONTROL :
            STATETRACE (_T("RECSTATE: Looking for BREAK key\n")) ;
            // Look for CTRL+BREAK.
            eCurrKeyState = eCheckBreak ;
            break ;
        default         :
            // It's a normal key.
            eCurrKeyState = eNormalKey ;
            NormalKeyState ( pMsg ) ;
            break ;
        }
    }
    else
    {
        // Ignore any up keys.  It's perfectly reasonable for recording
        // to start with some keys down.  I only want to start my
        // processing when keys go down.
    }
}

void CRecordingEngine :: CheckBreakState ( PEVENTMSG pMsg )
{

    // I only stay in the eCheckBreak state if it's a keypress.
    if ( ( WM_KEYDOWN    == pMsg->message ) ||
         ( WM_SYSKEYDOWN == pMsg->message )   )
    {
        // If it's a BREAK, abort.
        if ( VK_CANCEL == LOBYTE ( pMsg->paramL ) )
        {
            STATETRACE (_T("RECSTATE: Aborting out\n")) ;
            // Leave now.
            AbortRecording ( FALSE ) ;
        }
        else if ( VK_CONTROL == LOBYTE ( pMsg->paramL ) )
        {
            // The journal recorder gets multiple keydowns over time.
            // If the user sits on the control key, I could shift to the
            // normal state if I don't eat the extra VK_CONTROL keys
            // that come through.
        }
        else
        {
            // It's another key.  Add the {CTRL DOWN} to the string if
            // the mouse code didn't already do it.
            if ( FALSE == m_bCtrlDownInStream )
            {
                m_cKeyBuff += _T ( "{CTRL DOWN}" ) ;
                m_iKeyBuffKeys++ ;
            }

            // Flop to the normal state.
            eCurrKeyState = eNormalKey ;
            NormalKeyState ( pMsg ) ;
        }
    }
    else
    {
        // The mouse code put in the {CTRL DOWN}
        if ( TRUE == m_bCtrlDownInStream )
        {
            m_cKeyBuff += _T ( "{CTRL UP}" ) ;
            m_bCtrlDownInStream = FALSE ;
        }
        else
        {
            // I just got a {CTRL DOWN}{CTRL UP} so plop it in.
            m_cKeyBuff += _T ( "{CTRL DOWN}{CTRL UP}" ) ;
        }
        m_iKeyBuffKeys += 1 ;

        eCurrKeyState = eStart ;
    }
}

void CRecordingEngine :: CheckAltTabs ( PEVENTMSG pMsg )
{
    // I only stay in the eIsTabKey state if it's a keypress.
    if ( ( WM_KEYDOWN    == pMsg->message ) ||
         ( WM_SYSKEYDOWN == pMsg->message )   )
    {
        // If it's a TAB, do it.
        if ( VK_TAB == LOBYTE ( pMsg->paramL ) )
        {
            // If the mouse code had to put an ALT DOWN in the stream,
            // I need to do an ALT UP here so any mouse down will appear
            // bracketed by a "{ALT DOWN}<mouse stuff>{ALT UP}"
            if ( TRUE == m_bAltDownInStream )
            {
                m_cKeyBuff += _T ( "{ALT UP}" ) ;
                m_iKeyBuffKeys += 1 ;
                m_bAltDownInStream = FALSE ;
            }

            STATETRACE (_T("RECSTATE: ALT TABbing away\n")) ;
            eCurrKeyState = eAltTabbingAway ;
            ProcessAltTabbing ( pMsg ) ;
        }
        else if ( VK_MENU == LOBYTE ( pMsg->paramL ) )
        {
            // The journal recorder gets multiple keydowns over time.
            // If the user sits on the control key, I could shift to the
            // normal state if I don't eat the extra VK_MENU keys
            // that come through.
        }
        else
        {
            // Slide to the normal state.  If the ALT key is going up,
            // it'll generate a {ALT DOWN}{ALT UP} in the stream.

            // Put in the fact the ALT key is down!
            m_cKeyBuff += _T ( "{ALT DOWN}" ) ;
            m_iKeyBuffKeys++ ;

            // Flop to the normal state.
            eCurrKeyState = eNormalKey ;
            NormalKeyState ( pMsg ) ;
        }
    }
    else
    {
        // This might be a just {ALT DOWN}{ALT UP} but the mouse code
        // could have already put in the {ALT DOWN}
        if ( TRUE == m_bAltDownInStream )
        {
            m_cKeyBuff += _T ( "{ALT UP}" ) ;
            m_bAltDownInStream = FALSE ;
        }
        else
        {
            // I just got a {ALT DOWN){ALT UP} so plop it in.
            m_cKeyBuff += _T ( "{ALT DOWN){ALT UP}" ) ;
        }
        m_iKeyBuffKeys++ ;

        eCurrKeyState = eStart ;
    }
}

void CRecordingEngine :: ProcessAltTabbing ( PEVENTMSG pMsg )
{
    // The ALT key is down and processed.

    UINT vkCode = LOBYTE ( pMsg->paramL ) ;

    // There's only three keys that you can use when ALT+TABing, ALT,
    // TAB, and SHIFT.  Any others drop you out of the ALT+TABing.
    if ( WM_SYSKEYDOWN == pMsg->message )
    {
        switch ( vkCode )
        {
            case VK_TAB     :
            case VK_SHIFT   :
                break ;
            default         :
                m_bIgnoreAltTab = TRUE ;
                break ;
        }
    }
    else
    {
        // It's a key up.
        if ( VK_MENU == vkCode )
        {
            if ( TRUE == m_bIgnoreAltTab )
            {
                m_bIgnoreAltTab = FALSE ;
                // Back to the start state.
                eCurrKeyState = eStart ;
            }
            else
            {
                STATETRACE (_T("RECSTATE: Determining focus\n")) ;
                eCurrKeyState = eDetermineFocus ;
            }
        }
    }
}

void CRecordingEngine :: ProcessKeyboardSetFocus ( )
{

    // Do the real work!
    BOOL bRet = DoSetFocus ( ) ;
    if ( FALSE == bRet )
    {
        // Major problems...
        AbortRecording ( ) ;
    }
    // Set up for the next round.
    //m_cAltTabBuff.Empty ( ) ;
    // Back to the start state.
    eCurrKeyState = eStart ;
}

BOOL CRecordingEngine :: DoSetFocus ( void )
{
    // Before anything else, check to see if the recorder has
    // focus.  If so, stop recording.
    if ( TRUE == m_pOutput->RecorderHasFocus ( ) )
    {
        return ( FALSE ) ;
    }

    // Get the window with focus.  Assume this is the child window.
    GUITHREADINFO stTI ;
    memset ( &stTI , NULL , sizeof ( GUITHREADINFO ) ) ;
    stTI.cbSize = sizeof ( GUITHREADINFO ) ;

    if ( FALSE == GetGUIThreadInfo ( NULL , &stTI ) )
    {
        ASSERT ( !"GetGUIThreadInfo failed!" ) ;
        // Major failure.
        return ( FALSE ) ;
    }

    TCHAR szChildTitle[ 100 ] ;
    TCHAR szChildClass[ 100 ] ;

    // Get the class name.
    if ( 0 == GetClassName ( stTI.hwndFocus  , szChildClass , 100 ) )
    {
        // If I can't get the class name, I probably just got the
        // ALT+TAB window or a window that went away before this method
        // got called.  In that case, I'll call GetForegroundWindow to
        // try and get everything in sync.
        stTI.hwndFocus = ::GetForegroundWindow ( ) ;
        if ( 0 == GetClassName ( stTI.hwndFocus ,
                                 szChildClass   ,
                                 100             ) )
        {
            // Major failure.
            return ( FALSE ) ;
        }
    }

    // Get the text.
    if ( 0 == GetWindowText ( stTI.hwndFocus  , szChildTitle , 100 ) )
    {
        szChildTitle[ 0 ] = _T ( '\0' ) ;
    }

    // Get the ID.
    int iID = GetWindowLongPtr ( stTI.hwndFocus  , GWLP_ID ) ;

    TCHAR szTopTitle[ 100 ] = _T ( "\0" ) ;
    TCHAR szTopClass[ 100 ] = _T ( "\0" ) ;

    // Now get the root window for the child.
    HWND hRootParent = ::GetAncestor ( stTI.hwndFocus  , GA_ROOT ) ;
    if ( NULL != hRootParent )
    {

        if ( 0 == ::GetWindowText ( hRootParent , szTopTitle , 100 ))
        {
            szTopTitle[ 0 ] = _T ( '\0' ) ;
        }
        if ( 0 == GetClassName ( hRootParent , szTopClass , 100 ) )
        {
            ASSERT ( !"Unable to get root class name!!" ) ;
            return ( FALSE ) ;
        }
    }
    else
    {
        // This window is the root window..
        _tcscpy ( szTopTitle , szChildTitle ) ;
        _tcscpy ( szTopClass , szChildClass ) ;

        szChildTitle[ 0 ] = _T ( '\0' ) ;
        szChildClass[ 0 ] = _T ( '\0' ) ;
    }

    // If the child and top level window class and title are the same
    // I'm looking at the top level window only.
    if ( ( 0 == _tcscmp ( szTopTitle , szChildTitle ) ) &&
         ( 0 == _tcscmp ( szTopClass , szChildClass ) )    )
    {
        // Zero out the child stuff so I won't output it.
        szChildTitle[ 0 ] = _T ( '\0' ) ;
        szChildClass[ 0 ] = _T ( '\0' ) ;
    }

    // Output any keys lingering in the buffer.  Even if the windows
    // all match up, I need to get these keystrokes done because the
    // next thing that happens could be a click.
    DoKeyStrokes ( TRUE ) ;

    // If this is the same window I previously set focus to, there's
    // no need to generate all the code again.

    if ( ( m_hWndTop   == hRootParent    ) &&
         ( m_hWndChild == stTI.hwndFocus  )   )
    {
        return ( TRUE ) ;
    }

    m_hWndTop   = hRootParent ;
    m_hWndChild = stTI.hwndFocus  ;

    RECT stRect ;

    // Get the window to the same size it was during the recording.
    GetWindowRect ( hRootParent , &stRect ) ;

    BOOL bIsProgramManager = (0==_tcscmp(szTopClass , _T ( "Progman")));

    if ( FALSE == bIsProgramManager )
    {
        if ( ( FALSE ==
                CheckIfPointOnPrimaryMonitor ( stRect.left ,
                                               stRect.top   ) ) ||
             ( FALSE ==
                CheckIfPointOnPrimaryMonitor ( stRect.right  ,
                                               stRect.bottom  ) ) )
        {
           return ( FALSE ) ;
        }
    }

    // Output the command to set focus to the top level window.
    m_pOutput->FindTopWindow ( szTopClass , szTopTitle ) ;

    if ( FALSE == bIsProgramManager )
    {
        m_pOutput->SetWindowSize ( stRect.left                ,
                                   stRect.top                 ,
                                   stRect.right - stRect.left ,
                                   stRect.bottom - stRect.top  ) ;

        // Output the child window stuff if it's there and I'm supposed
        // to.
        if ( TRUE == m_bDoChildFocus )
        {
            if ( ( _T ( '\0' ) != szChildClass[ 0 ] ) ||
                 ( _T ( '\0' ) != szChildTitle[ 0 ] )   )
            {
                m_pOutput->FindChildWindowWithID ( szChildClass ,
                                                   iID          ,
                                                   szChildTitle  ) ;
            }
        }
    }
    return ( TRUE ) ;
}

void CRecordingEngine :: NormalKeyState ( PEVENTMSG pMsg )
{
    // The state to shift to after processing the key passed in.
    eKeyStates eShiftToState = eNormalKey ;

    UINT vkCode = LOBYTE ( pMsg->paramL ) ;

#ifdef _DEBUG
    {
        STATETRACE (_T("RECSTATE: Normal : ")) ;
        if ( ( WM_KEYDOWN == pMsg->message    ) ||
             ( WM_SYSKEYDOWN == pMsg->message )   )
        {
            STATETRACE ( _T( "KEYDOWN " ) ) ;
        }
        else
        {
            STATETRACE ( _T ( "KEYUP " ) ) ;
        }
        TCHAR szName [ 100 ] ;
        GetKeyNameText ( pMsg->paramH << 16 , szName , 100 ) ;
        CharUpper ( szName ) ;
        STATETRACE ( _T ( "%c %d %04X (%s)\n" ) ,
                          vkCode                ,
                          vkCode                ,
                          vkCode                ,
                          szName                 ) ;

    }
#endif

    // Check that this is not key that will cause a transition out.
    switch ( vkCode )
    {
        case VK_CONTROL :
            // An CTRL down can come through after a ALT key is already
            // down.
            if ( ( WM_KEYDOWN    == pMsg->message ) ||
                 ( WM_SYSKEYDOWN == pMsg->message )   )
            {
                eShiftToState = eCheckBreak ;
                STATETRACE ( _T ( "RECSTATE: Looking for BREAK key\n"));
            }
            else
            {
                m_cKeyBuff += _T( "{CTRL UP}" ) ;
                m_iKeyBuffKeys++ ;
            }
            m_iKeyBuffKeys++ ;
            break ;
        case VK_MENU    :
            if ( ( WM_KEYDOWN    == pMsg->message ) ||
                 ( WM_SYSKEYDOWN == pMsg->message )   )
            {
                eShiftToState = eIsTabKey ;
                STATETRACE (_T("RECSTATE: Looking for TAB key\n")) ;
            }
            else
            {
                m_cKeyBuff += _T( "{ALT UP}" ) ;
                m_iKeyBuffKeys++ ;
            }
            m_iKeyBuffKeys++ ;
            break ;

        case VK_SHIFT   :
            if ( ( WM_KEYDOWN    == pMsg->message ) ||
                 ( WM_SYSKEYDOWN == pMsg->message )   )
            {
                // Make sure I only do this once!
                if ( FALSE == m_bShiftDown )
                {
                    // The shift key is down so set my flags.
                    m_bShiftDown = TRUE ;
                    m_bShiftDownInString = FALSE ;
                }
            }
            else
            {
                // If I put a {SHIFT DOWN} earlier, I need to match up
                // with a {SHIFT UP}
                if ( TRUE == m_bShiftDownInString )
                {
                    m_cKeyBuff += _T ( "{SHIFT UP}" ) ;
                    m_iKeyBuffKeys++ ;

                    m_bShiftDownInString = FALSE ;
                }
                // The shift key has popped up.
                m_bShiftDown = FALSE ;
            }
            break ;
        default :
            // It's a normal key.

            // If it's not a key down, I don't care.
            if ( ( WM_KEYDOWN    == pMsg->message ) ||
                 ( WM_SYSKEYDOWN == pMsg->message )   )
            {
                //TRACE ( "vkCode = %04X\n" , vkCode ) ;

                // Is there a key string for the virtual code?
                if ( NULL != g_KeyStrings[ vkCode ].szString  )
                {
                    // Is the shift key down?  If so, I need to ensure
                    // that I add the {SHIFT DOWN} before adding this
                    // key.
                    if ( ( TRUE  == m_bShiftDown         ) &&
                         ( FALSE == m_bShiftDownInString )   )
                    {
                        m_cKeyBuff += _T ( "{SHIFT DOWN}" ) ;
                        m_iKeyBuffKeys++ ;
                        m_bShiftDownInString = TRUE ;
                    }
                    // Add the key to the key list.
                    m_cKeyBuff += g_KeyStrings[ vkCode ].szString ;
                }
                else
                {
                    // I need to translate the key into it's character
                    // equivalent.  Getting the keyboard state and using
                    // ToAscii will properly convert things like
                    // "{SHIFT DOWN}1{SHIFT UP}" into "!"

                    // First step is to get the current keyboard state.
                    BYTE byState [ 256 ] ;

                    GetKeyboardState ( byState ) ;

                    // Now do the conversion
                    TCHAR cConv[3] = { _T ( '\0' ) } ;
                    TCHAR cChar ;
#ifdef _UNICODE
                    int iRet = ToUnicode ( vkCode               ,
                                           pMsg->paramH         ,
                                           byState              ,
                                           (LPWORD)&cConv       ,
                                           sizeof ( cConv ) /
                                            sizeof ( TCHAR )    ,
                                           0                     ) ;

#else
                    int iRet = ToAscii ( vkCode         ,
                                         pMsg->paramH   ,
                                         byState        ,
                                         (LPWORD)&cConv ,
                                         0               ) ;
#endif
                    if ( 2 == iRet )
                    {
                        // This is an international keystroke.
                        ASSERT ( !"I gotta handle this!" ) ;
                    }

                    // If it didn't convert, don't use cChar!
                    if ( 0 == iRet )
                    {
                        cChar = (TCHAR)vkCode ;
                    }
                    else
                    {
                        // Before I can use the character, I need to
                        // check if the CTRL key is down.  If so,
                        // ToAscii/ToUnicode return the ASCII control
                        // code value.  Since I want the key, I'll need
                        // to do some work to get it.
                        SHORT sCtrlDown =
                                       GetAsyncKeyState ( VK_CONTROL ) ;
                        if ( 0xFFFF8000 == ( 0xFFFF8000 & sCtrlDown ))
                        {
                            // Control is down so I need to look if
                            // the CAPSLOCK and SHIFT keys are down.
                            BOOL bCapsLock =
                                   ( 0xFFFF8000 == ( 0xFFFF8000 &
                                       GetAsyncKeyState ( VK_CAPITAL)));
                            if ( TRUE == bCapsLock )
                            {
                                // CAPSLOCK is down so if SHIFT is down,
                                // use the lowercase version of the key.
                                if ( TRUE == m_bShiftDown )
                                {
// 'variable' : conversion from 'type' to 'type' of greater size
#pragma warning ( disable : 4312 )
                                    cChar = (TCHAR)
                                        CharLower ( (LPTSTR)vkCode );
#pragma warning ( default : 4312 )
                                }
                                else
                                {
                                    // Us the upper case version.
                                    cChar = (TCHAR)vkCode ;
                                }
                            }
                            else
                            {
                                // CAPSLOCK is not down so just check
                                // the shift state.
                                if ( TRUE == m_bShiftDown )
                                {
                                    cChar = (TCHAR)vkCode ;
                                }
                                else
                                {
// 'variable' : conversion from 'type' to 'type' of greater size
#pragma warning ( disable : 4312 )
                                    cChar = (TCHAR)
                                        CharLower ( (LPTSTR)vkCode );
#pragma warning ( default : 4312 )
                                }
                            }
                        }
                        else
                        {
                            // The CTRL key is not down so I can use the
                            // converted key directly.
                            cChar = cConv[ 0 ] ;
                        }
                    }

                    switch ( cChar )
                    {
                        // Brackets, braces, and tildes have to be
                        // specially handled.  All other keys are
                        // just slapped on the output list.
                        case _T ( '[' ) :
                            m_cKeyBuff += _T ( "{[}" ) ;
                            break ;
                        case _T ( ']' ) :
                            m_cKeyBuff += _T ( "{]}" ) ;
                            break ;
                        case _T ( '~' ) :
                            m_cKeyBuff += _T ( "{~}" ) ;
                            break ;
                        case _T ( '{' ) :
                            m_cKeyBuff += _T ( "{{}" ) ;
                            break ;
                        case _T ( '}' ) :
                            m_cKeyBuff += _T ( "{}}" ) ;
                            break ;
                        case _T ( '+' ) :
                            m_cKeyBuff += _T ( "{+}" ) ;
                            break ;
                        case _T ( '^' ) :
                            m_cKeyBuff += _T ( "{^}" ) ;
                            break ;
                        case _T ( '%' ) :
                            m_cKeyBuff += _T ( "{%}" ) ;
                            break ;
                        case _T ( '(' ) :
                            m_cKeyBuff += _T ( "{(}" ) ;
                            break ;
                        case _T ( ')' ) :
                            m_cKeyBuff += _T ( "{)}" ) ;
                            break ;
                        default :
                            m_cKeyBuff += cChar ;
                    }
                }
                // Bump up the number of keys processed.
                m_iKeyBuffKeys++ ;

                if ( ( m_iKeyBuffKeys > 20         ) ||
                     (  m_cKeyBuff.Length ( ) > 50 )   )
                {
                    DoKeyStrokes ( TRUE ) ;
                }
            }

            break ;
    }

    // Set the state to move to after this key is processed.
    eCurrKeyState = eShiftToState ;
}

void CRecordingEngine :: DoKeyStrokes ( BOOL bForceOutput )
{
    // If there is anything in the key buffer do it.
    if ( FALSE == m_cKeyBuff.IsEmpty ( ) )
    {
        if ( ( m_iKeyBuffKeys  > 20 ) ||
             ( TRUE == bForceOutput         )    )
        {
            m_pOutput->Input ( m_cKeyBuff ) ;
        }
    }
    m_cKeyBuff.Empty ( ) ;
    m_iKeyBuffKeys = 0 ;
}

BOOL CRecordingEngine :: CheckIfPointOnPrimaryMonitor ( int x , int y )
{
    BOOL bRet = TRUE ;
    if ( FALSE == m_bMultiMon )
    {
        POINT pt ;
        pt.x = x ;
        pt.y = y ;

        HMONITOR hCurrMon =
            MonitorFromPoint ( pt , MONITOR_DEFAULTTONEAREST ) ;
        MONITORINFO stMI ;
        stMI.cbSize = sizeof ( MONITORINFO ) ;
        if ( ( FALSE == GetMonitorInfo ( hCurrMon , &stMI ) ) ||
            ( MONITORINFOF_PRIMARY !=
            ( stMI.dwFlags & MONITORINFOF_PRIMARY )))
        {
            AbortRecording ( FALSE ) ;
            bRet = FALSE ;
        }
    }
    return ( bRet ) ;
}

void CRecordingEngine :: MouseMove ( PEVENTMSG pMsg )
{
    if ( FALSE == CheckIfPointOnPrimaryMonitor ( pMsg->paramL ,
                                                 pMsg->paramH  ) )
    {
        return ;
    }

    // Am I supposed to do each and every mouse movement or is the mouse
    // down?
    if ( ( TRUE == m_bAllMouseMovements ) ||
         ( TRUE == m_bMouseDown         )   )
    {

        if ( TRUE == m_bMouseDown )
        {
            // The mouse might be down, but I need to check if these
            // movements are far enough to generate the event.
            if ( ( abs ( (long)( m_iLastMouseX - pMsg->paramL ) ) <=
                                        m_iConvertedMinDragLengthX  ) &&
                 ( abs ( (long)( m_iLastMouseY - pMsg->paramH ) ) <=
                                        m_iConvertedMinDragLengthY  )  )
            {
                // Nothing to do....
                return ;
            }
        }

        if ( ( m_iLastMouseX != pMsg->paramL ) &&
             ( m_iLastMouseY != pMsg->paramH )   )
        {
            // Force any keystrokes already in the string out.
            DoKeyStrokes ( TRUE ) ;
            m_cKeyBuff.Format ( _T ( "{MOVETO %d , %d}" ) ,
                                pMsg->paramL              ,
                                pMsg->paramH               ) ;
            DoKeyStrokes ( TRUE ) ;
        }
    }
    m_iLastMouseX = pMsg->paramL ;
    m_iLastMouseY = pMsg->paramH ;
}

void CRecordingEngine :: ButtonDown ( eButtonEnum eButton ,
                                      PEVENTMSG   pMsg     )
{
    // The mouse is down.
    m_bMouseDown = TRUE ;

    if ( FALSE == CheckIfPointOnPrimaryMonitor ( pMsg->paramL ,
                                                 pMsg->paramH  ) )
    {
        return ;
    }

    m_iLastMouseX = pMsg->paramL ;
    m_iLastMouseY = pMsg->paramH ;

    // Generate the set focus method if I'm supposed to.
    if ( TRUE == m_bTrackFocus )
    {
        BOOL bRet = DoSetFocus ( ) ;
        if ( FALSE == bRet )
        {
            AbortRecording ( ) ;
            return ;
        }
    }

    // Force any keystrokes already in the string out.
    DoKeyStrokes ( TRUE ) ;

    m_cKeyBuff.Format ( _T( "{BTNDOWN %s , %d , %d}" ) ,
                        k_MOUSEBUTTONTEXT[ eButton ]   ,
                        m_iLastMouseX                  ,
                        m_iLastMouseY                   ) ;
    DoKeyStrokes ( TRUE ) ;
}

void CRecordingEngine :: ButtonUp ( eButtonEnum eButton ,
                                    PEVENTMSG   pMsg     )
{
    // It's possible recording started with a mouse button down.  If
    // that's the case, throw out all ups until I see the first down.
    if ( FALSE == m_bMouseDown )
    {
        return ;
    }

    if ( FALSE == CheckIfPointOnPrimaryMonitor ( pMsg->paramL ,
                                                 pMsg->paramH  ) )
    {
        return ;
    }

    m_bMouseDown = FALSE ;

    m_iLastMouseX = pMsg->paramL ;
    m_iLastMouseY = pMsg->paramH ;

    // Generate the set focus method if I'm supposed to.
    if ( TRUE == m_bTrackFocus )
    {
        BOOL bRet = DoSetFocus ( ) ;
        if ( FALSE == bRet )
        {
            AbortRecording ( ) ;
            return ;
        }
    }

    // Force any keystrokes already in the string out.
    DoKeyStrokes ( TRUE ) ;

    m_cKeyBuff.Format ( _T( "{BTNUP %s , %d , %d}" ) ,
                        k_MOUSEBUTTONTEXT[ eButton ] ,
                        m_iLastMouseX                ,
                        m_iLastMouseY                 ) ;
    DoKeyStrokes ( TRUE ) ;
}

void CRecordingEngine :: ButtonClick ( eButtonEnum eButton ,
                                       PEVENTMSG   pMsg     )
{
    if ( FALSE == CheckIfPointOnPrimaryMonitor ( pMsg->paramL ,
                                                 pMsg->paramH  ) )
    {
        return ;
    }

    m_iLastMouseX = pMsg->paramL ;
    m_iLastMouseY = pMsg->paramH ;

    // Generate the set focus method if I'm supposed to.
    if ( TRUE == m_bTrackFocus )
    {
        BOOL bRet = DoSetFocus ( ) ;
        if ( FALSE == bRet )
        {
            AbortRecording ( ) ;
            return ;
        }
    }

    // Force any keystrokes already in the string out.
    DoKeyStrokes ( TRUE ) ;

    m_cKeyBuff.Format ( _T( "{CLICK %s , %d , %d}" )  ,
                        k_MOUSEBUTTONTEXT[ eButton ]  ,
                        m_iLastMouseX                 ,
                        m_iLastMouseY                  ) ;

    DoKeyStrokes ( TRUE ) ;
}

void CRecordingEngine :: ButtonDblClick ( eButtonEnum eButton ,
                                          PEVENTMSG   pMsg     )
{
    if ( FALSE == CheckIfPointOnPrimaryMonitor ( pMsg->paramL ,
                                                 pMsg->paramH  ) )
    {
        return ;
    }

    m_iLastMouseX = pMsg->paramL ;
    m_iLastMouseY = pMsg->paramH ;

    // Generate the set focus method if I'm supposed to.
    if ( TRUE == m_bTrackFocus )
    {
        BOOL bRet = DoSetFocus ( ) ;
        if ( FALSE == bRet )
        {
            AbortRecording ( ) ;
            return ;
        }
    }

    // Force any keystrokes already in the string out.
    DoKeyStrokes ( TRUE ) ;

    m_cKeyBuff.Format ( _T( "{DBLCLICK %s , %d , %d}" )  ,
                        k_MOUSEBUTTONTEXT[ eButton ]  ,
                        m_iLastMouseX                 ,
                        m_iLastMouseY                  ) ;

    DoKeyStrokes ( TRUE ) ;
}


void CRecordingEngine :: MouseWheel ( PEVENTMSG /*pMsg*/ )
{
    // Unfortunately, you can't do anything with a WM_MOUSEWHEEL message
    // from the journal record hook as it does not set the wheel
    // direction. The paramL and paramH simply contain the screen
    // coordinates and nothing else.
    //TRACE ( _T ( "WHEEL : paramH %08X (%d) paramL %08X (%d)\n" ) ,
    //             pMsg->paramH                        ,
    //             pMsg->paramH                        ,
    //             pMsg->paramL                        ,
    //             pMsg->paramL                         ) ;
}

void CRecordingEngine :: MultipleMonitors ( BOOL bUseMM )
{
    DoKeyStrokes ( TRUE ) ;

    m_pOutput->UseMultipleMonitors( bUseMM ) ;

    m_bMultiMon = bUseMM ;
}

void CRecordingEngine ::
                  MouseStartState ( eButtonEnum       /*eButtonAction*/,
                                    LPMOUSEBUTTONSTATE pState          ,
                                    PEVENTMSG          pMsg            )
{
    // Is it a move?
    if ( WM_MOUSEMOVE == pMsg->message )
    {
        // Let the move happen.
        //TRACE ( "MREC STATE: MouseStartState calling MouseMove\n" ) ;
        MouseMove ( pMsg ) ;
    }
    else if ( WM_MOUSEWHEEL == pMsg->message )
    {
        // Nothing I can do as the journal hook does not tell you the
        // wheel direction.
    }
    else
    {
        BOOL bButtonIsDown = FALSE ;

        // I only care about button downs here.  A button up can come
        // through the start state if the user had a button down when
        // you started recording.
        switch ( pMsg->message )
        {
            case WM_LBUTTONDOWN :
            case WM_RBUTTONDOWN :
            case WM_MBUTTONDOWN :
                bButtonIsDown = TRUE ;
                break ;
            case WM_XBUTTONDOWN :
                // I don't have a multi-button explorer mouse so I can't
                // test this yet.  How do you determine the X mouse
                // button down in a journal hook?
                break ;
            default             :
                break ;
        }
        if ( TRUE == bButtonIsDown )
        {
            // Set the global to inticate a mouse button is down.
            m_bMouseDown = TRUE ;

            // Take a look at the keyboard state.  If CTRL, ALT, or
            // SHIFT are down, I need to get those down keys in the
            // stream.
            // While ALT+TABBING, pressing a mouse button immediately
            // cancels the ALT+TAB window.
            if ( eAltTabbingAway == eCurrKeyState )
            {
                // Shift back to the start state.
                eCurrKeyState = eStart ;
            }
            else if ( eIsTabKey == eCurrKeyState )
            {
                // The ALT key is down so I need to add that to the
                // stream and reset the state.
                m_cKeyBuff += _T ( "{ALT DOWN}" ) ;
                m_iKeyBuffKeys++ ;

                m_bAltDownInStream = TRUE ;

                // I'll keep in the eIsTabKey state as the user can
                // keep the ALT key down and start tabbing after doing
                // any sort of mouse click.
            }
            else if ( eCheckBreak == eCurrKeyState )
            {
                m_cKeyBuff += _T ( "{CTRL DOWN}" ) ;
                m_iKeyBuffKeys++ ;

                m_bCtrlDownInStream = TRUE ;
            }
            else
            {
                // Finally handle any shift keys.
                if ( ( TRUE  == m_bShiftDown         ) &&
                     ( FALSE == m_bShiftDownInString )   )
                {
                    m_cKeyBuff += _T ( "{SHIFT DOWN}" ) ;
                    m_iKeyBuffKeys++ ;
                    m_bShiftDownInString = TRUE ;
                }
            }

            // Set this button's state information and move to the
            // eMouseLookForUp state.
            pState->bIsDown = TRUE ;
            pState->eCurrButtonState = eMouseLookForUp ;
            MSTATETRACE (_T("MREC STATE: Shifting to eMouseLookForUp\n"));
            pState->aEvtMsgs[0] = *pMsg ;
        }
    }
}

void CRecordingEngine ::
              MouseLookForUpState ( eButtonEnum      eButtonAction ,
                                    LPMOUSEBUTTONSTATE pState        ,
                                    PEVENTMSG         pMsg          )
{
    BOOL bIsButtonUp = FALSE ;

    switch ( pMsg->message )
    {
        // Button ups are interesting in this state.
        case WM_LBUTTONUP   :
        case WM_RBUTTONUP   :
        case WM_MBUTTONUP   :
            bIsButtonUp = TRUE ;
            break ;
        case WM_XBUTTONUP   :
            break ;
        case WM_MOUSEWHEEL  :
            break ;
        case WM_MOUSEMOVE   :
            {
                if ( FALSE ==
                        CheckIfPointOnPrimaryMonitor ( pMsg->paramL ,
                                                       pMsg->paramH  ) )
                {
                    return ;
                }

                // On the move, look for the last button that had an
                // action.
                // Did the mouse move enough to warrent doing
                // the drag stuff?

                // The mouse might be down, but I need to check if these
                // movements are far enough to generate the event.
                if ( ( abs ( (long)( m_iLastMouseX - pMsg->paramL ) ) >
                                        m_iConvertedMinDragLengthX  ) ||
                     ( abs ( (long)( m_iLastMouseY - pMsg->paramH ) ) >
                                      m_iConvertedMinDragLengthY  )    )
                {
                    // Double check this button is down.
                    ASSERT ( TRUE == pState->bIsDown ) ;
                    // Generate the mouse down for the previous down
                    // message.
                    ButtonDown ( eButtonAction        ,
                                 &pState->aEvtMsgs[0]  ) ;
                    // Do the current move.
                    MouseMove ( pMsg ) ;
                    // Shift this button to the drag state.
                    pState->eCurrButtonState = eMouseDragging ;
                    MSTATETRACE(_T("MREC STATE: Shifting to eMouseDragging  from eMouseLookForUp\n"));
                }
                // Done with this message.
                return ;
            }
            break ;
        default :
            break ;
    }
    if ( TRUE == bIsButtonUp )
    {
        // Got us an up!
        ASSERT ( eMouseLookForUp == pState->eCurrButtonState ) ;
        ASSERT ( TRUE == pState->bIsDown ) ;

        // Set the master tracker.
        m_bMouseDown = FALSE ;
        // This button is also up.
        pState->bIsDown = FALSE ;

        DWORD dwTimeDiff = pMsg->time - pState->aEvtMsgs[0].time ;

        if ( dwTimeDiff > m_dwDoubleClickTime )
        {
            // This ain't a double click.  Generate a mouse click.
            ButtonClick ( eButtonAction , pMsg ) ;
            // Back to the start state.
            pState->eCurrButtonState = eMouseStart ;
            MSTATETRACE(_T("MREC STATE: Shifting to eMouseStart from eMouseLookForUp\n"));
        }
        else
        {
            // This could be the first click of a double click.
            // Save this message in the message list.
            pState->aEvtMsgs[1] = *pMsg ;
            // Shift to the looking for double click state.
            pState->eCurrButtonState = eMouseLookForDblClickDown ;
            MSTATETRACE(_T("MREC STATE: Shifting to eMouseLookForDblClickDown\n"));
        }
    }
}

void CRecordingEngine ::
         MouseLookForDblClickDown ( eButtonEnum        eButtonAction ,
                                    LPMOUSEBUTTONSTATE pState        ,
                                    PEVENTMSG           pMsg          )
{

    // Always start by checking the time and if it's a move.  Since
    // there are lots of WM_MOUSEMOVE messages generated, I can quickly
    // get out of this state.
    DWORD dwTimeDiff = pMsg->time - pState->aEvtMsgs[0].time ;
    if ( ( dwTimeDiff > m_dwDoubleClickTime ) &&
         ( WM_MOUSEMOVE == pMsg->message    )    )
    {
        // Take care of the click event I've got saved.
        ButtonClick ( eButtonAction , &pState->aEvtMsgs[0] ) ;

        pState->eCurrButtonState = eMouseStart ;
        MSTATETRACE(_T("MREC STATE: Time's up shifting to eMouseStart\n"));
        // Pass this event back to the start state.
        MouseStartState ( eButtonAction , pState , pMsg ) ;
        return ;
    }

    BOOL bIsButtonDown = FALSE ;

    switch ( pMsg->message )
    {
        // Button downs are interesting in this state.
        case WM_LBUTTONDOWN :
        case WM_RBUTTONDOWN :
        case WM_MBUTTONDOWN :
            bIsButtonDown = TRUE ;
            break ;
        case WM_XBUTTONDOWN :
            break ;
        case WM_MOUSEWHEEL  :
            break ;
        case WM_MOUSEMOVE   :
            // Let the move happen.
            MouseMove ( pMsg ) ;
            break ;
        default :
            break ;
    }
    if ( TRUE == bIsButtonDown )
    {
        ASSERT ( eMouseLookForDblClickDown == pState->eCurrButtonState);

        // Check to see if the second down is in the double click area.
        BOOL bHeightGood =
            ( ( (pState->aEvtMsgs[0].paramH -
                          m_dwHalfDblClickWidth) < pMsg->paramH ) &&
              ( (pState->aEvtMsgs[0].paramH +
                          m_dwHalfDblClickWidth) > pMsg->paramH )   ) ;

        BOOL bWidthGood =
            ( ( (pState->aEvtMsgs[0].paramL -
                          m_dwHalfDblClickHeight) < pMsg->paramL ) &&
              ( (pState->aEvtMsgs[0].paramL +
                          m_dwHalfDblClickHeight) > pMsg->paramL )   ) ;

        // If the down took longer than the double click time or the
        // click area was outside the double click rect, generate the
        // click and set up for the next potential double click.
        if ( ( dwTimeDiff > m_dwDoubleClickTime ) ||
             ( FALSE == bHeightGood             ) ||
             ( FALSE == bWidthGood              )   )
        {
            // Set the easy things.
            m_bMouseDown = TRUE ;
            pState->bIsDown = TRUE ;

            // I need to generate a mouse click for the mouse click I've
            // got stored in the 0 and 1 slots for this button.
            // Start the click on the down location.
            ButtonClick ( eButtonAction , &pState->aEvtMsgs[0] ) ;
            // Save this event into slot 0 so I can look for the up.
            pState->aEvtMsgs[0] = *pMsg ;
            // Move to the look for up state.
            pState->eCurrButtonState = eMouseLookForUp ;
            MSTATETRACE(_T("MREC STATE: Shifting to eMouseLookForUp from eMouseLookForDblClickDown\n"));
        }
        else
        {
            // This is a double click!
            // Let's do a double click!
            ButtonDblClick ( eButtonAction , pMsg ) ;
            // Move to the start state.
            pState->eCurrButtonState = eMouseStart ;
            MSTATETRACE(_T("MREC STATE: Got a double click\n")) ;
            MSTATETRACE(_T("MREC STATE: Shifting to eMouseStart from eMouseLookForDblClickDown\n"));
        }
    }
}

void CRecordingEngine :: MouseDrag ( eButtonEnum     /*eButtonAction*/,
                                     LPMOUSEBUTTONSTATE pState        ,
                                     PEVENTMSG          pMsg           )
{
    // Stuff is moving on the screen.
    if ( WM_MOUSEMOVE == pMsg->message )
    {
        MouseMove ( pMsg ) ;
    }
    else
    {
        // It's gotta be an up.
        eButtonEnum eCurr = eMaxButton ;
        switch ( pMsg->message )
        {
            case WM_LBUTTONUP   :
                eCurr = eLeft ;
                break ;
            case WM_RBUTTONUP   :
                eCurr = eRight ;
                break ;
            case WM_MBUTTONUP   :
                eCurr = eMiddle ;
                break ;
            case WM_XBUTTONUP   :
                break ;
            default             :
                break ;
        }
        if ( eCurr != eMaxButton )
        {
            // Set the last button with info.
            m_eLastButtonWithAction = eCurr ;

            pState = &m_aButtonStates[eCurr] ;
            pState->bIsDown = FALSE ;

            // Do this mouse up event.
            ButtonUp ( eCurr , pMsg ) ;

            // Set back to the start state.
            pState->eCurrButtonState = eMouseStart ;
            MSTATETRACE(_T("MREC STATE: Shifting to eMouseStart from dragging\n"));
        }
    }
}

