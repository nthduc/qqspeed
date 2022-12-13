/*----------------------------------------------------------------------
Debugging Applications for Microsoft .NET and Microsoft Windows
Copyright © 1997-2003 John Robbins -- All rights reserved.
------------------------------------------------------------------------
The core code that processes a string passed to PlayInput

I tried to make this ready for internationalization, but I have not
tested this with different keyboard layouts.  If you find that it is not
ready, I would appreciate if you would let me know!
----------------------------------------------------------------------*/

#include "stdafx.h"
#include "PlayInput.h"
#include "ParsePlayInputString.h"

/*//////////////////////////////////////////////////////////////////////
// File Scope Constants, Defines, and Enums
//////////////////////////////////////////////////////////////////////*/
// The following flags indicate the current special key state.  These
// are used in ProcessString and AddOEMChar.  These are not used in the
// case where {SHIFT/CTRL/ATL DOWN/UP} are used as those keys are
// generated when doing mouse processing.
#define k_STATE_CTRLDOWN        0x01
#define k_STATE_SHIFTDOWN       0x02
#define k_STATE_ALTDOWN         0x04

// Some helpful macros.
#define ISSTATECTRLDOWN(x) (k_STATE_CTRLDOWN == (k_STATE_CTRLDOWN & x))
#define ISSTATESHIFTDOWN(x)(k_STATE_SHIFTDOWN == (k_STATE_SHIFTDOWN&x))
#define ISSTATEALTDOWN(x)  (k_STATE_ALTDOWN == (k_STATE_ALTDOWN & x))

#define SETCTRLDOWNSTATE(x) (x |= k_STATE_CTRLDOWN)
#define SETSHIFTDOWNSTATE(x) (x |= k_STATE_SHIFTDOWN)
#define SETALTDOWNSTATE(x) (x |= k_STATE_ALTDOWN)

// A wrapper around MapVirtualKey.  The problem is that the input
// structure defines scan codes as WORDs and MapVirtualKey returns them
// as UINTS.
#define MVK_TO_SC(ch) ((WORD)MapVirtualKey(ch,0))

// The SEH exception that I will raise if there is a problem parsing.
// I don't really use any objects in this file and since I don't want
// to force whoever uses this file to mess with the synchronous
// exception settings, I just use trusty SEH.
#define STATUS_PARSE_ERROR  0xE0000001

/*//////////////////////////////////////////////////////////////////////
// File Scope Structures
//////////////////////////////////////////////////////////////////////*/
// The structure that defines the lookup table used to find the special
// codes key codes inside braces.  If the scan code is not zero, then
// the character is converted with AddOEMChar.
typedef struct tag_BRACECODE
{
    LPCTSTR szString ;
    WORD    wVK      ;
    WORD    wScan    ;
} BRACECODE , * PBRACECODE ;

// Certain modifier keys have to be expressed with a down or up state
// to allow them to be used with mouse actions.  This structure defines
// a key codes and what key action it makes.  wFlag is 0 for a keydown
// and KEYEVENTF_KEYUP for a release.
typedef struct tag_MODIFIERBRACECODE
{
    LPCTSTR szString ;
    WORD    wVK      ;
    WORD    wFlag    ;
} MODIFIERBRACECODE , * PMODIFIERBRACECODE ;

// The enum that describes the mouse operation.
enum eMouseOperation
{
    eBtnDown  = 0 ,
    eBtnUp    = 1 ,
    eClick    = 2 ,
    eDblClick = 3 ,
    eMoveTo   = 4 ,
} ;

// The mouse brace codes.
typedef struct tag_MOUSEBRACECODE
{
    LPCTSTR         szString ;
    DWORD           dwParamCount ;
    eMouseOperation eOp ;
} MOUSEBRACECODE , * PMOUSEBRACECODE ;

// The enum that describes the button.
enum eMouseButton
{
    eLeft   = 0 ,
    eMiddle = 1 ,
    eRight  = 2 ,
    eX1     = 3 ,   // Currently not used.
    eX2     = 4 ,   // Currently not used.
    eMaxBtn = 5 ,
} ;

// The mouse button codes.
typedef struct tag_MOUSEBUTTONCODE
{
    LPCTSTR      szString ;
    eMouseButton eButton  ;
} MOUSEBUTTONCODE , * PMOUSEBUTTONCODE ;

/*//////////////////////////////////////////////////////////////////////
// File Scope Globals
//////////////////////////////////////////////////////////////////////*/
// The total size of the current buffer.
static UINT g_uiBuffSize = 20 ;
static UINT g_uiNextBuffSize = 20 * 2 ;

// The current buffer of converted keystrokes.
static PINPUT g_pRetKeys = NULL ;
// The current position in the g_pRetKeys array.
static UINT g_uiCount = 0 ;

// The keyboard layout at the time this function was called.
static HKL g_hklKeyboard = NULL ;

// The special key codes that appear inside braces.  Make sure to keep
// this in alphabetical order.
static BRACECODE g_stBraceCode[] =
{
    { _T ( "%" )          , 0           , _T ( '%' ) } ,
    { _T ( "(" )          , 0           , _T ( '(' ) } ,
    { _T ( ")" )          , 0           , _T ( ')' ) } ,
    { _T ( "+" )          , 0           , _T ( '+' ) } ,
    { _T ( "APPS" )       , VK_APPS     , 0          } ,
    { _T ( "BACKSPACE" )  , VK_BACK     , 0          } ,
    { _T ( "BKSP" )       , VK_BACK     , 0          } ,
    { _T ( "BREAK" )      , VK_PAUSE    , 0          } ,
    { _T ( "BS" )         , VK_BACK     , 0          } ,
    { _T ( "CAPSLOCK" )   , VK_CAPITAL  , 0          } ,
    { _T ( "CLEAR" )      , VK_CLEAR    , 0          } ,
    { _T ( "DEL" )        , VK_DELETE   , 0          } ,
    { _T ( "DELETE" )     , VK_DELETE   , 0          } ,
    { _T ( "DOWN" )       , VK_DOWN     , 0          } ,
    { _T ( "END" )        , VK_END      , 0          } ,
    { _T ( "ENTER" )      , VK_RETURN   , 0          } ,
    { _T ( "ESC" )        , VK_ESCAPE   , 0          } ,
    { _T ( "F1" )         , VK_F1       , 0          } ,
    { _T ( "F10" )        , VK_F10      , 0          } ,
    { _T ( "F11" )        , VK_F11      , 0          } ,
    { _T ( "F12" )        , VK_F12      , 0          } ,
    { _T ( "F13" )        , VK_F13      , 0          } ,
    { _T ( "F14" )        , VK_F14      , 0          } ,
    { _T ( "F15" )        , VK_F15      , 0          } ,
    { _T ( "F16" )        , VK_F16      , 0          } ,
    { _T ( "F2" )         , VK_F2       , 0          } ,
    { _T ( "F3" )         , VK_F3       , 0          } ,
    { _T ( "F4" )         , VK_F4       , 0          } ,
    { _T ( "F5" )         , VK_F5       , 0          } ,
    { _T ( "F6" )         , VK_F6       , 0          } ,
    { _T ( "F7" )         , VK_F7       , 0          } ,
    { _T ( "F8" )         , VK_F8       , 0          } ,
    { _T ( "F9" )         , VK_F9       , 0          } ,
    { _T ( "HELP" )       , VK_HELP     , 0          } ,
    { _T ( "HOME" )       , VK_HOME     , 0          } ,
    { _T ( "INS" )        , VK_INSERT   , 0          } ,
    { _T ( "INSERT" )     , VK_INSERT   , 0          } ,
    { _T ( "LEFT" )       , VK_LEFT     , 0          } ,
    { _T ( "LWIN" )       , VK_LWIN     , 0          } ,
    { _T ( "NUMLOCK" )    , VK_NUMLOCK  , 0          } ,
    { _T ( "NUMPAD*" )    , VK_MULTIPLY , 0          } ,
    { _T ( "NUMPAD+" )    , VK_ADD      , 0          } ,
    { _T ( "NUMPAD-" )    , VK_SUBTRACT , 0          } ,
    { _T ( "NUMPAD." )    , VK_DECIMAL  , 0          } ,
    { _T ( "NUMPAD/" )    , VK_DIVIDE   , 0          } ,
    { _T ( "NUMPAD0" )    , VK_NUMPAD0  , 0          } ,
    { _T ( "NUMPAD1" )    , VK_NUMPAD1  , 0          } ,
    { _T ( "NUMPAD2" )    , VK_NUMPAD2  , 0          } ,
    { _T ( "NUMPAD3" )    , VK_NUMPAD3  , 0          } ,
    { _T ( "NUMPAD4" )    , VK_NUMPAD4  , 0          } ,
    { _T ( "NUMPAD5" )    , VK_NUMPAD5  , 0          } ,
    { _T ( "NUMPAD6" )    , VK_NUMPAD6  , 0          } ,
    { _T ( "NUMPAD7" )    , VK_NUMPAD7  , 0          } ,
    { _T ( "NUMPAD8" )    , VK_NUMPAD8  , 0          } ,
    { _T ( "NUMPAD9" )    , VK_NUMPAD9  , 0          } ,
    { _T ( "PGDN" )       , VK_NEXT     , 0          } ,
    { _T ( "PGUP" )       , VK_PRIOR    , 0          } ,
    { _T ( "PRTSC" )      , VK_SNAPSHOT , 0          } ,
    { _T ( "RIGHT" )      , VK_RIGHT    , 0          } ,
    { _T ( "RWIN" )       , VK_RWIN     , 0          } ,
    { _T ( "SCROLLLOCK" ) , VK_SCROLL   , 0          } ,
    { _T ( "TAB" )        , VK_TAB      , 0          } ,
    { _T ( "UP" )         , VK_UP       , 0          } ,
    { _T ( "[" )          , 0           , _T ( '[' ) } ,
    { _T ( "]" )          , 0           , _T ( ']' ) } ,
    { _T ( "^" )          , 0           , _T ( '^' ) } ,
    { _T ( "{" )          , 0           , _T ( '{' ) } ,
    { _T ( "}" )          , 0           , _T ( '}' ) } ,
    { _T ( "~" )          , 0           , _T ( '~' ) }
} ;

#define BRACECODE_COUNT (sizeof ( g_stBraceCode ) / sizeof ( BRACECODE))

static MODIFIERBRACECODE g_stModifierBraceCode[] =
{
    { _T ( "ALT DOWN" )     , VK_MENU       , 0               } ,
    { _T ( "ALT UP" )       , VK_MENU       , KEYEVENTF_KEYUP } ,
    { _T ( "CTRL DOWN" )    , VK_CONTROL    , 0               } ,
    { _T ( "CTRL UP" )      , VK_CONTROL    , KEYEVENTF_KEYUP } ,
    { _T ( "SHIFT DOWN" )   , VK_SHIFT      , 0               } ,
    { _T ( "SHIFT UP" )     , VK_SHIFT      , KEYEVENTF_KEYUP } ,
} ;

#define MODIFIERBRACECODE_COUNT \
          (sizeof (g_stModifierBraceCode) / sizeof (MODIFIERBRACECODE) )

static MOUSEBRACECODE g_stMouseBraceCode[] =
{
    { _T ( "BTNDOWN" )  , 3 , eBtnDown  } ,
    { _T ( "BTNUP" )    , 3 , eBtnUp    } ,
    { _T ( "CLICK" )    , 3 , eClick    } ,
    { _T ( "DBLCLICK" ) , 3 , eDblClick } ,
    { _T ( "MOVETO" )   , 2 , eMoveTo   } ,
} ;

#define MOUSEBRACECODE_COUNT \
          (sizeof (g_stMouseBraceCode) / sizeof (MOUSEBRACECODE) )

// The mouse button strings.
static MOUSEBUTTONCODE g_stMouseButtons[] =
{
    { _T ( "LEFT" )   , eLeft   } ,
    { _T ( "MIDDLE" ) , eMiddle } ,
    { _T ( "RIGHT" )  , eRight  } ,
    { _T ( "X1" )     , eX1     } , // Currently not used.
    { _T ( "X2" )     , eX2     } , // Currently not used.
} ;

#define MOUSEBUTTON_COUNT \
          (sizeof (g_stMouseButtons) / sizeof (MOUSEBUTTONCODE) )


/*//////////////////////////////////////////////////////////////////////
// File Scope Prototypes
//////////////////////////////////////////////////////////////////////*/
// Allocates a INPUT array.  This function keeps doubling the memory
// for the array each time it is called.
static PINPUT AllocateBuffer ( void ) ;

// Reallocates the input buffer and makes it bigger.
static void ReallocBuffer ( void ) ;

// The main worker function that parses the string.
static DWORD ProcessString ( LPTSTR  szString   ,
                             UINT &  uiCurrPos  ,
                             BOOL    bRecursive  ) ;

// The only function that adds to the input array.
static void AddKeyToInputArray ( WORD   wVK         ,
                                 WORD   wScanCode   ,
                                 DWORD  dwFlags  = 0  ) ;

// Adds an OEM keystroke to the input array.
static void AddOEMCharToInputArray ( TCHAR chChar ) ;

// Adds a mouse event ot the input array.
static void AddMouseEventToInputArray ( DWORD dwFlag          ,
                                        int   iX              ,
                                        int   iY              ,
                                        DWORD dwMouseData = 0  ) ;

// Handles doing the "{xxx}" statements.
static void ProcessBraceStatement ( LPTSTR  szString  ,
                                    UINT &  uiCurrPos  ) ;

// Handles mouse operation statements.
static BOOL ProcessMouseBraceStatement ( LPTSTR szBuff ) ;

// Parses up the mouse parameters
static BOOL ParseMouseParameters ( LPTSTR         szBuff      ,
                                   int            iParamCount ,
                                   eMouseButton & eBtn        ,
                                   int          & iX          ,
                                   int          & iY           ) ;

// Coordinate converters.
static int VirtualXFromAbsoluteX ( int iX ) ;
static int VirtualYFromAbsoluteY ( int iY ) ;

// Adds a mouse event to the input array.
static void DoMouseEvent ( eMouseOperation eOp  ,
                           eMouseButton    eBtn ,
                           int             iX   ,
                           int             iY    ) ;

/*//////////////////////////////////////////////////////////////////////
// Implementation starts here.
//////////////////////////////////////////////////////////////////////*/

// See ParsePlayInputString.h for the description.
UINT ParsePlayInputString ( IN  LPCTSTR  szString  ,
                            OUT UINT &   uiErrChar ,
                            OUT PINPUT & pRetKeys  ,
                            OUT UINT &   uiCount    )
{
    // Do some parameter validation.
    ASSERT ( FALSE == IsBadStringPtr ( szString , 2000 ) ) ;
    // Do real parameter validation.
    if ( ( NULL == szString ) || ( _T ( '\0' ) == *szString ) )
    {
        return ( PI_INVALIDPARAMS ) ;
    }

    // Initialize a few things.
    // The current position in the string.
    UINT uiCurrStPos = 0 ;
    // The return value.
    UINT uiRet = PI_SUCCESS ;

    // Initialize the input parameters.
    uiErrChar = 0 ;

    // Allocate the starting memory sizes.
    g_uiBuffSize = 20 ;
    g_uiNextBuffSize = 20 * 2 ;

    // g_uiCount is also the current location in g_pRetKeys.
    g_uiCount = 0 ;
    g_pRetKeys = AllocateBuffer ( ) ;

    // Duplicate the string because I poke at it a bit.
    LPTSTR szDup = _tcsdup ( szString ) ;
    
    // Grab the keyboard layout so I translate the string correctly.
    TCHAR szKeyLID[ KL_NAMELENGTH ] ;
    GetKeyboardLayoutName ( szKeyLID ) ;
    g_hklKeyboard = LoadKeyboardLayout ( szKeyLID , KLF_ACTIVATE ) ;

    // The return state from ProcessString.  This way I can reset the
    // CTRL, ALT, or SHIFT keys if they are still down.  By doing this,
    // I can handle strings such at ^+{END} that users might use.
    // This is more user friendly.
    DWORD dwState ;

    __try
    {
        dwState = ProcessString ( szDup , uiCurrStPos , FALSE ) ;
        // Check the states.
        if ( TRUE == ISSTATESHIFTDOWN ( dwState ) )
        {
            AddKeyToInputArray ( VK_SHIFT               ,
                                 MVK_TO_SC ( VK_SHIFT ) ,
                                 KEYEVENTF_KEYUP         ) ;
        }
        if ( TRUE == ISSTATECTRLDOWN ( dwState ) )
        {
            AddKeyToInputArray ( VK_CONTROL               ,
                                 MVK_TO_SC ( VK_CONTROL ) ,
                                 KEYEVENTF_KEYUP           ) ;
        }
        if ( TRUE == ISSTATEALTDOWN ( dwState ) )
        {
            AddKeyToInputArray ( VK_MENU               ,
                                 MVK_TO_SC ( VK_MENU ) ,
                                 KEYEVENTF_KEYUP        ) ;
        }
    }
    __except ( STATUS_PARSE_ERROR == GetExceptionCode ( ) )
    {
        uiErrChar = uiCurrStPos + 1 ;
        uiRet = PI_PARSEERROR ;
        // Delete the memory allocated.
        delete [] g_pRetKeys ;
        g_pRetKeys = NULL ;

        g_uiCount = 0 ;

    }

    free ( szDup ) ;

    // Fill in the information that the user needs.
    uiCount = g_uiCount ;
    pRetKeys = g_pRetKeys ;
    
    // All OK, Jumpmaster!
    return ( uiRet ) ;
}


// Allocates a buffer of g_uiBuffSize size and doubles g_uiBuffSize so
// the next allocation will be twice as much.
static PINPUT AllocateBuffer ( void )
{
    g_uiBuffSize = g_uiNextBuffSize ;
    PINPUT pRet = new INPUT [ g_uiBuffSize ] ;
    memset ( pRet , NULL , sizeof ( INPUT ) * g_uiBuffSize ) ;
    g_uiNextBuffSize = g_uiBuffSize * 2 ;
    return ( pRet ) ;
}

// Reallocates the input buffer.
static void ReallocBuffer ( void )
{
    // Allocate a new buffer.
    PINPUT pTemp = AllocateBuffer ( ) ;
    // Copy the data over.
    memcpy ( pTemp          ,
             g_pRetKeys     ,
             sizeof ( INPUT ) * g_uiCount ) ;
    // Get rid of the old memory.
    delete [] g_pRetKeys ;
    // Now use the bigger memory.
    g_pRetKeys = pTemp ;
}

// Adds the information into the global INPUT array.  If needed, it will
// automatically reallocate the array.  It also bumps up the current
// position in the array.
static void AddKeyToInputArray ( WORD   wVK       ,
                                 WORD   wScanCode ,
                                 DWORD  dwFlags    )
{
    // Double check that this is not off the end of the array.
    if ( g_uiCount == g_uiBuffSize )
    {
        ReallocBuffer ( ) ;
    }
    // Fill in the particular INPUT.
    g_pRetKeys[ g_uiCount ].type           = INPUT_KEYBOARD ;
    g_pRetKeys[ g_uiCount ].ki.wVk         = wVK ;
    g_pRetKeys[ g_uiCount ].ki.wScan       = wScanCode ;
    g_pRetKeys[ g_uiCount ].ki.dwFlags     = dwFlags ;
    g_pRetKeys[ g_uiCount ].ki.time        = GetTickCount ( ) ;
    g_pRetKeys[ g_uiCount ].ki.dwExtraInfo = 0 ;
    // Bump up the current array position.
    g_uiCount++ ;
}

// Adds an OEM character to the input array.
static void AddOEMCharToInputArray ( TCHAR chChar )
{
    // First try and do the conversion.
    SHORT sTemp = VkKeyScanEx ( chChar , g_hklKeyboard ) ;
    // If the conversion is -1, then there is
    // a problem.
    if ( -1 == sTemp )
    {
        RaiseException ( STATUS_PARSE_ERROR , 0 , 0 , 0 ) ;
    }
    // Get the hi byte to do the flags
    // calculations.
    BYTE hiByte = HIBYTE ( sTemp ) ;

    // If the shift key is set, add that to the output stream.
    if ( 0x01 == ( 0x01 & hiByte ) )
    {
        AddKeyToInputArray ( VK_SHIFT , MVK_TO_SC ( VK_SHIFT ) ) ;
    }

    // If the ctrl key is set, add that to the output stream.
    if ( 0x02 == ( 0x02 & hiByte ) )
    {
        AddKeyToInputArray ( VK_CONTROL , MVK_TO_SC ( VK_CONTROL ) ) ;
    }

    // If the alt key is set, add that to the output stream.
    if ( 0x04 == ( 0x04 & hiByte ) )
    {
        AddKeyToInputArray ( VK_MENU , MVK_TO_SC ( VK_MENU ) ) ;
    }

    // BUG BUG
    //  What is supposed to happen with those
    //  other flags -- the keyboard driver flags
    //  and the Hankaku flags?

    // Add the key itself.
    AddKeyToInputArray ( LOBYTE ( sTemp ) ,
                         chChar            );

    AddKeyToInputArray ( LOBYTE ( sTemp ) ,
                         chChar           ,
                         KEYEVENTF_KEYUP   ) ;

    // Undo any of the down keys.

    // If the shift key is set, add that to the output stream.
    if ( 0x01 == ( 0x01 & hiByte ) )
    {
        AddKeyToInputArray ( VK_SHIFT               ,
                             MVK_TO_SC ( VK_SHIFT ) ,
                             KEYEVENTF_KEYUP         ) ;
    }

    // If the ctrl key is set, add that to the output stream.
    if ( 0x02 == ( 0x02 & hiByte ) )
    {
        AddKeyToInputArray ( VK_CONTROL               ,
                             MVK_TO_SC ( VK_CONTROL ) ,
                             KEYEVENTF_KEYUP           ) ;
    }

    // If the alt key is set, add that to the output stream.
    if ( 0x04 == ( 0x04 & hiByte ) )
    {
        AddKeyToInputArray ( VK_MENU               ,
                             MVK_TO_SC ( VK_MENU ) ,
                             KEYEVENTF_KEYUP        ) ;
    }

}

// Used by bsearch to find stuff in the BRACECODE array.
static int CodeCompare ( const void * elem1 , const void * elem2 )
{
    return ( _tcscmp ( ((PBRACECODE)elem1)->szString ,
                       ((PBRACECODE)elem2)->szString  ) ) ;
}

// Processes an item in a brace statement.  szString points to the
// character AFTER the open brace.
static void ProcessBraceStatement ( LPTSTR  szString  ,
                                    UINT &  uiCurrPos  )
{
    // The first thing to do is to check that there is a closing brace.
    UINT uiCloseBrace = 0 ;
    while ( _T( '\0' ) != szString[ uiCloseBrace ] )
    {
        if ( _T ( '}' ) == szString[ uiCloseBrace ] )
        {
            // Since this could be a "{}}" check the next character.
            if ( _T ( '}' ) == szString[ uiCloseBrace + 1 ] )
            {
                uiCloseBrace++ ;
            }
            break ;
        }
        uiCloseBrace++ ;
    }

    // If not pointing at a close brace, kick out.
    if ( _T ( '}' ) != szString[ uiCloseBrace ] )
    {
        RaiseException ( STATUS_PARSE_ERROR , 0 , 0 , 0 ) ;
    }

    // Update the current position index to the brace.
    uiCurrPos += uiCloseBrace ;

    // Zero out the brace.  It is no longer needed.
    szString[ uiCloseBrace ] = _T ( '\0' ) ;

    // Copy the string and uppercase it for the comparison.
    TCHAR szBuff[ MAX_PATH ] ;
    _tcsncpy ( szBuff   ,
               szString ,
               min ( MAX_PATH , uiCloseBrace ) ) ;
    szBuff[ min ( MAX_PATH , uiCloseBrace ) ] = _T ( '\0' ) ;
    _tcsupr ( szBuff ) ;

    // Now do the lookup in the table.
    BRACECODE stTemp ;
    stTemp.szString = szBuff ;

    PBRACECODE pFound = (PBRACECODE)bsearch ( (void*)&stTemp        ,
                                              (void*)&g_stBraceCode ,
                                              BRACECODE_COUNT       ,
                                              sizeof ( BRACECODE )  ,
                                              CodeCompare            ) ;
    if ( NULL != pFound )
    {
        if ( 0 == pFound->wScan )
        {
            AddKeyToInputArray ( pFound->wVK               ,
                                 MVK_TO_SC ( pFound->wVK )  ) ;
            AddKeyToInputArray ( pFound->wVK               ,
                                 MVK_TO_SC ( pFound->wVK ) ,
                                 KEYEVENTF_KEYUP            ) ;
        }
        else
        {
            AddOEMCharToInputArray ( (TCHAR)pFound->wScan ) ;
        }
    }
    else
    {
        // Look to see if this is a modifier string.
        MODIFIERBRACECODE stModBrace ;
        stModBrace.szString = szBuff ;

        PMODIFIERBRACECODE pModFound = (PMODIFIERBRACECODE)
                            bsearch ( (void*)&stModBrace            ,
                                      (void*)&g_stModifierBraceCode ,
                                      MODIFIERBRACECODE_COUNT       ,
                                      sizeof ( MODIFIERBRACECODE )  ,
                                      CodeCompare                    ) ;
        if ( NULL != pModFound )
        {
            // Remember, these modifiers need to retain state over
            // multiple calls so don't set any of the CTRL/ALT/SHIFT
            // flags!
            AddKeyToInputArray ( pModFound->wVK               ,
                                 MVK_TO_SC ( pModFound->wVK ) ,
                                 pModFound->wFlag              ) ;
        }
        else
        {
            // Check to see if this one is in the mouse list.
            if ( FALSE == ProcessMouseBraceStatement ( szBuff ) )
            {
                RaiseException ( STATUS_PARSE_ERROR , 0 , 0 , 0 ) ;
            }
        }
    }
}

static BOOL ProcessMouseBraceStatement ( LPTSTR szBuff )
{
    // Scootch up to the first space so I can isolate the mouse string.
    TCHAR * pCurrChar = szBuff ;
    while ( _T ( ' ' ) != *pCurrChar )
    {
        pCurrChar++ ;
        if ( _T ( '\0' ) == *pCurrChar )
        {
            return ( FALSE ) ;
        }
    }

    // NULL out the first space so I can look for the operation.
    *pCurrChar = _T ( '\0' ) ;

    // Look for the operation.
    PMOUSEBRACECODE pFoundCode = NULL ;
    for ( int i = 0 ; i < MOUSEBRACECODE_COUNT ; i++ )
    {
        if ( 0 == _tcscmp ( szBuff , g_stMouseBraceCode[i].szString ) )
        {
            pFoundCode = &g_stMouseBraceCode[ i ] ;
            break ;
        }
    }

    if ( NULL == pFoundCode )
    {
        return ( FALSE ) ;
    }

    // Move past the NULL I put in the string.
    pCurrChar++ ;

    // Scootch past any spaces in the string.
    while ( _T ( ' ' ) == *pCurrChar )
    {
        pCurrChar++ ;
        if ( _T ( '\0' ) == *pCurrChar )
        {
            return ( FALSE ) ;
        }
    }

    // What'll hold the parsed parameters.
    eMouseButton eBtn ;
    int iX ;
    int iY ;

    if ( FALSE == ParseMouseParameters ( pCurrChar                ,
                                         pFoundCode->dwParamCount ,
                                         eBtn                     ,
                                         iX                       ,
                                         iY                        ) )
    {
        return ( FALSE ) ;
    }

    // Cool.  Everything parsed! Now do the work to add this event to
    // the input buffer!!
    DoMouseEvent ( pFoundCode->eOp , eBtn , iX , iY ) ;

    return ( TRUE ) ;
}

static BOOL ParseMouseParameters ( LPTSTR         szBuff      ,
                                   int            iParamCount ,
                                   eMouseButton & eBtn        ,
                                   int          & iX          ,
                                   int          & iY           )
{

    // If the param count is 3, the first field is the button text.
    if ( 3 == iParamCount )
    {
        // Holds the button text pulled out of szBuff.
        TCHAR szBtn[ 50 ] ;

        if ( 3 != _stscanf ( szBuff                ,
                             _T ( "%s , %d , %d" ) ,
                             szBtn                 ,
                             &iX                   ,
                             &iY                    ) )
        {
            // Something didn't parse....
            return ( FALSE ) ;
        }

        // Hunt down the button type.
        eBtn = eMaxBtn ;
        for ( int i = 0 ; i < MOUSEBUTTON_COUNT ; i++ )
        {
            if ( 0 == _tcscmp ( szBtn , g_stMouseButtons[i].szString ) )
            {
                eBtn = g_stMouseButtons[i].eButton ;
                break ;
            }
        }

        // Did a real button come through?
        if ( eMaxBtn == eBtn )
        {
            return ( FALSE ) ;
        }
    }
    else
    {
        // It's a MOVETO so the button doesn't matter.
        eBtn = eMaxBtn ;

        if ( 2 != _stscanf ( szBuff           ,
                             _T ( "%d , %d" ) ,
                             &iX              ,
                             &iY               ) )
        {
            // Something didn't parse....
            return ( FALSE ) ;
        }
    }

    // Everything got parsed so check the position values.
    if ( ( ( iX < 0 ) || ( iX > 65536 ) ) ||
         ( ( iY < 0 ) || ( iY > 65536 ) )    )
    {
        return ( FALSE ) ;
    }
    
    // Do any coordinate conversions necessary.
    iX = VirtualXFromAbsoluteX ( iX ) ;
    iY = VirtualYFromAbsoluteY ( iY ) ;

    return ( TRUE ) ;
}

static int VirtualXFromAbsoluteX ( int iX )
{
    if ( TRUE == GetUseAbsoluteScreenCoordinates ( ) )
    {
        int iParam ;
        if ( TRUE == GetMultiMonitor ( ) )
        {
            iParam = SM_CXVIRTUALSCREEN ;
        }
        else
        {
            iParam = SM_CXSCREEN ;
        }
        double Width = GetSystemMetrics ( iParam ) ;
        double Val = ( ((double)iX/Width) * (double)(65536) ) ;

        double Ceil = ceil ( Val ) ;
        double Floor = floor ( Val ) ;

        if ( Ceil > Floor )
        {
            iX = (int)Ceil ;
        }
        else
        {
            iX = (int)Floor ;
        }
    }
    return ( iX ) ;
}

static int VirtualYFromAbsoluteY ( int iY )
{
    if ( TRUE == GetUseAbsoluteScreenCoordinates ( ) )
    {
        int iParam ;
        if ( TRUE == GetMultiMonitor ( ) )
        {
            iParam = SM_CYVIRTUALSCREEN ;
        }
        else
        {
            iParam = SM_CYSCREEN ;
        }

        double Height = GetSystemMetrics ( iParam ) ;
        double Val = ( ((double)iY/Height) * (double)(65536) ) ;

        double Ceil = ceil ( Val ) ;
        double Floor = floor ( Val ) ;

        if ( Ceil > Floor )
        {
            iY = (int)Ceil ;
        }
        else
        {
            iY = (int)Floor ;
        }
    }
    return ( iY ) ;
}


static void AddMouseEventToInputArray ( DWORD dwFlag      ,
                                        int   iX          ,
                                        int   iY          ,
                                        DWORD dwMouseData  )
{
    // Double check that this is not off the end of the array.
    if ( g_uiCount == g_uiBuffSize )
    {
        ReallocBuffer ( ) ;
    }

    // Fill in the particular INPUT.
    g_pRetKeys[ g_uiCount ].type = INPUT_MOUSE ;
    g_pRetKeys[ g_uiCount ].mi.dwFlags = MOUSEEVENTF_ABSOLUTE |
                                               dwFlag           ;
    if ( TRUE == GetMultiMonitor ( ) )
    {
        g_pRetKeys[ g_uiCount ].mi.dwFlags |= MOUSEEVENTF_VIRTUALDESK ;
    }

    g_pRetKeys[ g_uiCount ].mi.dx = iX ;
    g_pRetKeys[ g_uiCount ].mi.dy = iY ;
    g_pRetKeys[ g_uiCount ].mi.time = GetTickCount ( ) ;
    g_pRetKeys[ g_uiCount ].mi.mouseData = dwMouseData ;
    g_pRetKeys[ g_uiCount ].mi.dwExtraInfo = 0 ;
    // Bump up the current array position.
    g_uiCount++ ;
}

static void DoMouseEvent ( eMouseOperation eOp  ,
                           eMouseButton    eBtn ,
                           int             iX   ,
                           int             iY    )
{
    switch ( eOp )
    {
        case eBtnDown   :
            // Move to the mouse position specified by the coordinate
            // values before doing the down.
            DoMouseEvent ( eMoveTo ,eBtn , iX , iY ) ;
            switch ( eBtn )
            {
                case eLeft      :
                    AddMouseEventToInputArray ( MOUSEEVENTF_LEFTDOWN ,
                                                iX                   ,
                                                iY                    );
                    break ;
                case eMiddle    :
                    AddMouseEventToInputArray ( MOUSEEVENTF_MIDDLEDOWN,
                                                iX                    ,
                                                iY                    );
                    break ;
                case eRight     :
                    AddMouseEventToInputArray ( MOUSEEVENTF_RIGHTDOWN ,
                                                iX                    ,
                                                iY                    );
                    break ;
                case eX1        :
                    AddMouseEventToInputArray ( MOUSEEVENTF_XDOWN ,
                                                iX                ,
                                                iY                ,
                                                XBUTTON1           );
                    break ;
                case eX2        :
                    AddMouseEventToInputArray ( MOUSEEVENTF_XDOWN ,
                                                iX                ,
                                                iY                ,
                                                XBUTTON2           );
                    break ;
                default         :
                    ASSERT ( !"Invalid button in DoMouseEvent!" ) ;
                    break ;
            }
            break ;
        case eBtnUp     :
            DoMouseEvent ( eMoveTo ,eBtn , iX , iY ) ;
            switch ( eBtn )
            {
                case eLeft      :
                    AddMouseEventToInputArray ( MOUSEEVENTF_LEFTUP ,
                                                iX                 ,
                                                iY                  );
                    break ;
                case eMiddle    :
                    AddMouseEventToInputArray ( MOUSEEVENTF_MIDDLEUP ,
                                                iX                   ,
                                                iY                   );
                    break ;
                case eRight     :
                    AddMouseEventToInputArray ( MOUSEEVENTF_RIGHTUP ,
                                                iX                  ,
                                                iY                   );
                    break ;
                case eX1        :
                    AddMouseEventToInputArray ( MOUSEEVENTF_XUP ,
                                                iX              ,
                                                iY              ,
                                                XBUTTON1         ) ;
                    break ;
                case eX2        :
                    AddMouseEventToInputArray ( MOUSEEVENTF_XUP ,
                                                iX              ,
                                                iY              ,
                                                XBUTTON2         );
                    break ;
                default         :
                    ASSERT ( !"Invalid button in DoMouseEvent!" ) ;
                    break ;
            }
            break ;
        case eClick     :
            // Pretty slick trick, huh!?
            DoMouseEvent ( eBtnDown , eBtn , iX , iY ) ;
            DoMouseEvent ( eBtnUp   , eBtn , iX , iY ) ;
            break ;
        case eDblClick  :
            // Still slick!!
            DoMouseEvent ( eClick ,eBtn , iX , iY ) ;
            DoMouseEvent ( eClick ,eBtn , iX , iY ) ;
            break ;
        case eMoveTo    :
            AddMouseEventToInputArray ( MOUSEEVENTF_MOVE ,
                                        iX               ,
                                        iY                );
            break ;
        default         :
            ASSERT ( !"Invalid mouse operation!!" ) ;
            break ;
    }
}


// Does the real work of processing the input string.  Be careful, this
// functions uses recursion to handle the () processing.
static DWORD ProcessString ( LPTSTR  szString   ,
                             UINT &  uiCurrPos  ,
                             BOOL    bRecursive  )
{
    // Indicates that a ALT, CTRL, or SHIFT key is down.
    DWORD dwState = 0 ;

    WORD wScanCode ;

    while ( _T ( '\0' ) != szString [ uiCurrPos ] )
    {
        TCHAR chChar = szString[ uiCurrPos ] ;

        switch ( chChar )
        {
            // Shift key
            case _T ( '+' ) :
                // Press the shift key down.
                AddKeyToInputArray ( VK_SHIFT , MVK_TO_SC ( VK_SHIFT ));
                SETSHIFTDOWNSTATE ( dwState ) ;
                break ;
            // Control key.
            case _T ( '^' ) :
                // Press the control key down.
                AddKeyToInputArray ( VK_CONTROL              ,
                                     MVK_TO_SC ( VK_CONTROL ) ) ;
                SETCTRLDOWNSTATE ( dwState ) ;
                break ;
            // Alt key.
            case _T ( '%' ) :
                AddKeyToInputArray ( VK_MENU , MVK_TO_SC ( VK_MENU ) ) ;
                SETALTDOWNSTATE ( dwState ) ;
                break ;
            // The Enter key.
            case _T ( '~' ) :
                wScanCode = MVK_TO_SC ( VK_RETURN ) ;
                AddKeyToInputArray ( VK_RETURN , wScanCode ) ;
                AddKeyToInputArray ( VK_RETURN       ,
                                     wScanCode       ,
                                     KEYEVENTF_KEYUP  ) ;
                break ;
            // A Brace.
            case _T ( '{' ) :
                // Move past the brace.
                uiCurrPos++ ;
                ProcessBraceStatement ( szString + uiCurrPos ,
                                        uiCurrPos             ) ;
                if ( TRUE == ISSTATESHIFTDOWN ( dwState ) )
                {
                    AddKeyToInputArray ( VK_SHIFT               ,
                                         MVK_TO_SC ( VK_SHIFT ) ,
                                         KEYEVENTF_KEYUP         ) ;
                }
                if ( TRUE == ISSTATECTRLDOWN ( dwState ) )
                {
                    AddKeyToInputArray ( VK_CONTROL               ,
                                         MVK_TO_SC ( VK_CONTROL ) ,
                                         KEYEVENTF_KEYUP          );
                }
                if ( TRUE == ISSTATEALTDOWN ( dwState ) )
                {
                    AddKeyToInputArray ( VK_MENU               ,
                                         MVK_TO_SC ( VK_MENU ) ,
                                         KEYEVENTF_KEYUP        ) ;
                }
                // Clear the state!
                dwState = 0 ;
                break ;

            // The "long term Shift/Ctrl/Alt hold" code.
            case _T ( '(' ) :
                {
                    // If the Shift, Ctrl, or Alt key is not down, it
                    // is an error.
                    if ( 0 == dwState )
                    {
                        RaiseException ( STATUS_PARSE_ERROR ,
                                         0                  ,
                                         0                  ,
                                         0                   ) ;
                    }
                    // Where the matching close paren for this open
                    // paren is located.
                    UINT uiCloseParen = 0 ;
                    // The numer of open parens seen.
                    UINT uiOpenCount = 0 ;
                    LPTSTR szCurr = szString + uiCurrPos ;
                    // Find the closing paren taking into account
                    // nested paren pairs.
                    while ( _T( '\0' ) != szCurr [ uiCloseParen ] )
                    {
                        if ( _T ( '(' ) == szCurr [ uiCloseParen ] )
                        {
                            // Bump the nesting level.
                            uiOpenCount++ ;
                        }
                        if ( _T ( ')' ) == szCurr [ uiCloseParen ] )
                        {
                            // Decrement the open paren count.
                            uiOpenCount--;
                            // Is this the one?
                            if ( 0 == uiOpenCount )
                            {
                                break ;
                            }
                        }
                        uiCloseParen++ ;
                    }

                    // No closing paren, error out.
                    if ( _T ( '\0' ) == szCurr[ uiCloseParen ] )
                    {
                        RaiseException ( STATUS_PARSE_ERROR ,
                                         0                  ,
                                         0                  ,
                                         0                   ) ;
                    }
                    // Move the temp string pointer past the open paren.
                    szCurr++ ;
                    // The position in szCurr to start working on.
                    UINT uiProcessed = 0 ;
                    // Recursively handle the parethesised statement.
                    ProcessString ( szCurr ,  uiProcessed , TRUE ) ;
                    // Move the current master position past the closed
                    // paren.
                    uiCurrPos += uiProcessed ;
                    // Check the states and undo any active.
                    if ( TRUE == ISSTATESHIFTDOWN ( dwState ) )
                    {
                        AddKeyToInputArray ( VK_SHIFT               ,
                                             MVK_TO_SC ( VK_SHIFT ) ,
                                             KEYEVENTF_KEYUP         ) ;
                    }
                    if ( TRUE == ISSTATECTRLDOWN ( dwState ) )
                    {
                        AddKeyToInputArray ( VK_CONTROL               ,
                                             MVK_TO_SC ( VK_CONTROL ) ,
                                             KEYEVENTF_KEYUP          );
                    }
                    if ( TRUE == ISSTATEALTDOWN ( dwState ) )
                    {
                        AddKeyToInputArray ( VK_MENU               ,
                                             MVK_TO_SC ( VK_MENU ) ,
                                             KEYEVENTF_KEYUP        ) ;
                    }
                    // Clear the state!
                    dwState = 0 ;
                }
                break ;

            case _T ( ')' ) :
                // If the function is not recursing, and the close paren
                // is the character, it is an error.
                if ( FALSE == bRecursive )
                {
                   RaiseException ( STATUS_PARSE_ERROR , 0 , 0 , 0 ) ;
                }
                // Just move past the close paren and return.
                uiCurrPos++ ;
                return ( TRUE ) ;
                break ;

            // The following keys must be in braces or are part of the
            // control stuff.  If they are seen here, it is an error.
            case _T ( '[' ) :
            case _T ( ']' ) :
            case _T ( '}' ) :
                RaiseException ( STATUS_PARSE_ERROR , 0 , 0 , 0 ) ;
                break ;

            default :
                // Treat it as an OEM key.
                AddOEMCharToInputArray ( chChar ) ;

                if ( TRUE == ISSTATESHIFTDOWN ( dwState ) )
                {
                    AddKeyToInputArray ( VK_SHIFT               ,
                                         MVK_TO_SC ( VK_SHIFT ) ,
                                         KEYEVENTF_KEYUP         ) ;
                }
                if ( TRUE == ISSTATECTRLDOWN ( dwState ) )
                {
                    AddKeyToInputArray ( VK_CONTROL               ,
                                         MVK_TO_SC ( VK_CONTROL ) ,
                                         KEYEVENTF_KEYUP           ) ;
                }
                if ( TRUE == ISSTATEALTDOWN ( dwState ) )
                {
                    AddKeyToInputArray ( VK_MENU               ,
                                         MVK_TO_SC ( VK_MENU ) ,
                                         KEYEVENTF_KEYUP        ) ;
                }

                // Set the state back to nothing.
                dwState = 0 ;
                break ;
        }
        // Bump the current position.
        uiCurrPos++ ;
    }
    return ( dwState ) ;
}
