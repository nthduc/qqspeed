#include "FBGUIBase.h"
#include "FBGUICore.h"
#include "FBGUILua.h"
#include "FBGUIEvent.h"
#include "FBGUIWindow.h"
#include "FBGUIElement.h"
#include "FBGUISystem.h"
#include "Windows.h"
#include "FBGUIWin32MouseDrv.h"

#define GET_X_LPARAM(lParam)	( (int)( short ) LOWORD( lParam ) )
#define GET_Y_LPARAM(lParam)	( (int)( short ) HIWORD( lParam ) )

WNDPROC CGUIWin32Platform::mpOldProc = NULL;
bool CGUIWin32Platform::mIsMouseWheel = false;
bool CGUIWin32Platform::mIsLButtonDown = false;
bool CGUIWin32Platform::mIsLButtonUp = false;
bool CGUIWin32Platform::mIsLButtonDblClk = false;
bool CGUIWin32Platform::mIsRButtonDown = false;
bool CGUIWin32Platform::mIsRButtonUp = false;
bool CGUIWin32Platform::mIsMButtonDown = false;
bool CGUIWin32Platform::mIsMButtonUp = false;
bool CGUIWin32Platform::mIsMouseMove = false;
bool CGUIWin32Platform::mIsKeyDown = false;
bool CGUIWin32Platform::mIsKeyUp = false;
bool CGUIWin32Platform::mIsUniChar = false;
bool CGUIWin32Platform::mIsSysKeyDown = false;
bool CGUIWin32Platform::mIsSysKeyUp = false;
bool CGUIWin32Platform::mIsDeadChar = false;
bool CGUIWin32Platform::mIsSysDeadChar = false;
int CGUIWin32Platform::mCharIndex = 0;

CGUIPoint CGUIWin32Platform::mLButtonDownPoint;
CGUIPoint CGUIWin32Platform::mLButtonUpPoint;
CGUIPoint CGUIWin32Platform::mLButtonDblClkPoint;
CGUIPoint CGUIWin32Platform::mRButtonDownPoint;
CGUIPoint CGUIWin32Platform::mRButtonUpPoint;
CGUIPoint CGUIWin32Platform::mMButtonDownPoint;
CGUIPoint CGUIWin32Platform::mMButtonUpPoint;
CGUIPoint CGUIWin32Platform::mMouseMovePoint;
CGUIPoint CGUIWin32Platform::mMouseWheelPoint;

unsigned int CGUIWin32Platform::mLButtonUpFlag = 0;
unsigned int CGUIWin32Platform::mLButtonDownFlag = 0;
unsigned int CGUIWin32Platform::mLButtonDblClkFlag = 0;
unsigned int CGUIWin32Platform::mRButtonDownFlag = 0;
unsigned int CGUIWin32Platform::mRButtonUpFlag = 0;
unsigned int CGUIWin32Platform::mMButtonDownFlag = 0;
unsigned int CGUIWin32Platform::mMButtonUpFlag = 0;
unsigned int CGUIWin32Platform::mMouseMoveFlag = 0;
unsigned int CGUIWin32Platform::mMouseWheelFlag = 0;

unsigned int CGUIWin32Platform::mWParamKeyDown = 0;
unsigned int CGUIWin32Platform::mWParamKeyUp = 0;
unsigned int CGUIWin32Platform::mWParamSysKeyDown = 0;
unsigned int CGUIWin32Platform::mWParamSysKeyUp	= 0;
unsigned int CGUIWin32Platform::mWParamUniChar	= 0;
unsigned int CGUIWin32Platform::mWParamDeadChar = 0;
unsigned int CGUIWin32Platform::mWParamSysDeadChar = 0;
std::vector< unsigned int > CGUIWin32Platform::mWParamChars;

unsigned int CGUIWin32Platform::mLParamKeyDown = 0;
unsigned int CGUIWin32Platform::mLParamKeyUp = 0;
unsigned int CGUIWin32Platform::mLParamSysKeyDown = 0;
unsigned int CGUIWin32Platform::mLParamSysKeyUp	= 0;
unsigned int CGUIWin32Platform::mLParamUniChar	= 0;
unsigned int CGUIWin32Platform::mLParamDeadChar = 0;
unsigned int CGUIWin32Platform::mLParamSysDeadChar = 0;
std::vector< unsigned int > CGUIWin32Platform::mLParamChars;

LRESULT CGUIWin32Platform::WindowProc( HWND hWnd, UINT vMsg, WPARAM vWParam, LPARAM vLParam )
{
	switch( vMsg )
	{
		case WM_KEYDOWN:
		{
			CGUIWin32Platform::mWParamKeyDown = (unsigned int) vWParam;
			CGUIWin32Platform::mLParamKeyDown = (unsigned int) vLParam;
			CGUIWin32Platform::mIsKeyDown = true;
			break;
		}
		case WM_KEYUP:
		{
			CGUIWin32Platform::mWParamKeyUp = (unsigned int) vWParam;
			CGUIWin32Platform::mLParamKeyUp = (unsigned int) vLParam;
			CGUIWin32Platform::mIsKeyUp = true;
			break;
		}
		case WM_SYSKEYDOWN:
		{
			CGUIWin32Platform::mWParamSysKeyDown = (unsigned int) vWParam;
			CGUIWin32Platform::mLParamSysKeyDown = (unsigned int) vLParam;
			CGUIWin32Platform::mIsSysKeyDown = true;
			break;
		}
		case WM_SYSKEYUP:
		{
			CGUIWin32Platform::mWParamSysKeyUp = (unsigned int) vWParam;
			CGUIWin32Platform::mLParamSysKeyUp = (unsigned int) vLParam;
			CGUIWin32Platform::mIsSysKeyUp = true;
			break;
		}
		case WM_CHAR:
		{
			CGUIWin32Platform::mWParamChars.push_back( (unsigned int) vWParam );
			CGUIWin32Platform::mLParamChars.push_back( (unsigned int) vWParam );
			break;
		}

		case 0x0109:
		{
			CGUIWin32Platform::mWParamUniChar = (unsigned int) vWParam;
			CGUIWin32Platform::mLParamUniChar = (unsigned int) vLParam;
			CGUIWin32Platform::mIsUniChar = true;
			break;
		}
		case WM_DEADCHAR:
		{
			CGUIWin32Platform::mWParamDeadChar = (unsigned int) vWParam;
			CGUIWin32Platform::mLParamDeadChar = (unsigned int) vLParam;
			CGUIWin32Platform::mIsDeadChar = true;
			break;
		}
		case WM_SYSDEADCHAR:
		{
			CGUIWin32Platform::mWParamSysDeadChar = (unsigned int) vWParam;
			CGUIWin32Platform::mLParamSysDeadChar = (unsigned int) vLParam;
			CGUIWin32Platform::mIsSysDeadChar = true;
			break;
		}

		case WM_LBUTTONDOWN:
		{
			CGUIPoint tPoint( GET_X_LPARAM( vLParam ), GET_Y_LPARAM( vLParam ) );
			CGUIWin32Platform::mLButtonDownPoint = CGUISystem::GetSingleton( ).DPToLP( tPoint );
			CGUIWin32Platform::mLButtonDownFlag  = (unsigned int) vWParam;
			CGUIWin32Platform::mIsLButtonDown = true;
			break;
		}

		case WM_LBUTTONUP:
		{
			CGUIPoint tPoint( GET_X_LPARAM( vLParam ), GET_Y_LPARAM( vLParam ) );
			CGUIWin32Platform::mLButtonUpPoint = CGUISystem::GetSingleton( ).DPToLP( tPoint );
			CGUIWin32Platform::mLButtonUpFlag  = (unsigned int) vWParam;
			CGUIWin32Platform::mIsLButtonUp = true;
			break;
		}
		
		case WM_LBUTTONDBLCLK:
		{
			CGUIPoint tPoint( GET_X_LPARAM( vLParam ), GET_Y_LPARAM( vLParam ) );
			CGUIWin32Platform::mLButtonDblClkPoint = CGUISystem::GetSingleton( ).DPToLP( tPoint );
			CGUIWin32Platform::mLButtonDblClkFlag = (unsigned int) vWParam;
			CGUIWin32Platform::mIsLButtonDblClk = true;
			break;
		}

		case WM_RBUTTONDOWN:
		{
			CGUIPoint tPoint( GET_X_LPARAM( vLParam ), GET_Y_LPARAM( vLParam ) );
			CGUIWin32Platform::mRButtonDownPoint = CGUISystem::GetSingleton( ).DPToLP( tPoint );
			CGUIWin32Platform::mRButtonDownFlag  = (unsigned int) vWParam;
			CGUIWin32Platform::mIsRButtonDown = true;
			break;
		}
		case WM_RBUTTONUP:
		{
			CGUIPoint tPoint( GET_X_LPARAM( vLParam ), GET_Y_LPARAM( vLParam ) );
			CGUIWin32Platform::mRButtonUpPoint = CGUISystem::GetSingleton( ).DPToLP( tPoint );
			CGUIWin32Platform::mRButtonUpFlag  = (unsigned int) vWParam;
			CGUIWin32Platform::mIsRButtonUp = true;
			break;
		}
		case WM_MBUTTONDOWN:
		{
			CGUIPoint tPoint( GET_X_LPARAM( vLParam ), GET_Y_LPARAM( vLParam ) );
			CGUIWin32Platform::mMButtonDownPoint = CGUISystem::GetSingleton( ).DPToLP( tPoint );
			CGUIWin32Platform::mMButtonDownFlag  = (unsigned int) vWParam;
			CGUIWin32Platform::mIsMButtonDown = true;
			break;
		}
		case WM_MBUTTONUP:
		{
			CGUIPoint tPoint( GET_X_LPARAM( vLParam ), GET_Y_LPARAM( vLParam ) );
			CGUIWin32Platform::mMButtonUpPoint = CGUISystem::GetSingleton( ).DPToLP( tPoint );
			CGUIWin32Platform::mMButtonUpFlag  = (unsigned int) vWParam;
			CGUIWin32Platform::mIsMButtonUp = true;
			break;
		}
		case WM_MOUSEMOVE:
		{
			CGUIPoint tPoint( GET_X_LPARAM( vLParam ), GET_Y_LPARAM( vLParam ) );
			CGUIWin32Platform::mMouseMovePoint = CGUISystem::GetSingleton( ).DPToLP( tPoint );
			CGUIWin32Platform::mMouseMoveFlag  = (unsigned int) vWParam;
			CGUIWin32Platform::mIsMouseMove = true;
			break;
		}
		case 0x020A:
		{
			CGUIPoint tPoint( GET_X_LPARAM( vLParam ), GET_Y_LPARAM( vLParam ) );
			CGUIWin32Platform::mMouseWheelPoint = CGUISystem::GetSingleton( ).DPToLP( tPoint );
			CGUIWin32Platform::mMouseWheelFlag  = (unsigned int) vWParam;
			CGUIWin32Platform::mIsMouseWheel = true;
			break;
		}
	}

	return CallWindowProc( CGUIWin32Platform::mpOldProc, hWnd, vMsg, vWParam, vLParam );
}

CGUIWin32Platform::CGUIWin32Platform( HWND vWnd ) 
{
	mpOldProc = (WNDPROC) GetWindowLong( vWnd, GWL_WNDPROC );
	SetWindowLong( vWnd, GWL_WNDPROC, (LONG) WindowProc );
}

void CGUIWin32Platform::MbToWc( const char* pText, int tTextCount, wchar_t* pUnicode, int tUniCount )
{
	::MultiByteToWideChar( CP_ACP, 0, pText, int( ::strlen( pText ) ), pUnicode, tUniCount );
}

bool CGUIWin32Platform::IsChar( unsigned int& rCode, unsigned int& rFlag )
{
	bool tIsChar = false;
	if ( mWParamChars.empty( ) == false )
	{
		tIsChar = true;
		rCode = mWParamChars[ 0 ]; mWParamChars.erase( mWParamChars.begin( ) );
		rFlag = mLParamChars[ 0 ]; mLParamChars.erase( mLParamChars.begin( ) );
	}
	return tIsChar;
}

bool CGUIWin32Platform::IsKeyDown( unsigned int& rVirtualKey, unsigned int& rFlag )
{
	rVirtualKey = mWParamKeyDown;
	rFlag = mLParamKeyDown;
	bool tIsKeyDown = mIsKeyDown;
	mIsKeyDown = false;
	return tIsKeyDown;
}

bool CGUIWin32Platform::IsKeyUp( unsigned int& rVirtualKey, unsigned int& rFlag )
{
	rVirtualKey = mWParamKeyUp;
	rFlag = mLParamKeyUp;
	bool tIsKeyUp = mIsKeyUp;
	mIsKeyUp = false;
	return tIsKeyUp;
}

bool CGUIWin32Platform::IsUniChar( unsigned int& rCode, unsigned int& rFlag )
{
	rCode = mWParamUniChar;
	rFlag = mLParamUniChar;
	bool tIsUniChar = mIsUniChar;
	mIsUniChar = false;
	return tIsUniChar;
}

bool CGUIWin32Platform::IsSysKeyDown( unsigned int& rVirtualKey, unsigned int& rFlag )
{
	rVirtualKey = mWParamSysKeyDown;
	rFlag = mLParamSysKeyDown;
	bool tIsSysKeyDown = mIsSysKeyDown;
	mIsSysKeyDown = false;
	return tIsSysKeyDown;
}

bool CGUIWin32Platform::IsSysKeyUp( unsigned int& rVirtualKey, unsigned int& rFlag )
{
	rVirtualKey = mWParamSysKeyUp;
	rFlag = mLParamSysKeyUp;
	bool tIsSysKeyUp = mIsSysKeyUp;
	mIsSysKeyUp = false;
	return tIsSysKeyUp;
}

bool CGUIWin32Platform::IsSysDeadChar( unsigned int& rCode, unsigned int& rFlag )
{
	rCode = mWParamSysDeadChar;
	rFlag = mLParamSysDeadChar;
	bool tIsSysDeadChar = mIsSysDeadChar;
	mIsSysDeadChar = false;
	return tIsSysDeadChar;
}

bool CGUIWin32Platform::IsDeadChar( unsigned int& rCode, unsigned int& rFlag )
{
	rCode = mWParamDeadChar;
	rFlag = mLParamDeadChar;
	bool tIsDeadChar = mIsDeadChar;
	mIsDeadChar = false;
	return tIsDeadChar;
}

bool CGUIWin32Platform::IsMouseMove( CGUIPoint& rPoint, unsigned int& rFlag )
{
	rPoint = mMouseMovePoint;
	rFlag  = mMouseMoveFlag;
	bool tMouseMove = mIsMouseMove;
	mIsMouseMove = false;
	return tMouseMove;
}

bool CGUIWin32Platform::IsMouseWheel( CGUIPoint& rPoint, unsigned int& rFlag )
{
	rPoint = mMouseWheelPoint;
	rFlag  = mMouseWheelFlag;
	bool tMouseWheel = mIsMouseWheel;
	mIsMouseWheel = false;
	return tMouseWheel;
}

bool CGUIWin32Platform::IsLButtonDown( CGUIPoint& rPoint, unsigned int& rFlag )
{
	rPoint	= mLButtonDownPoint;
	rFlag	= mLButtonDownFlag;
	bool tLButtonDown = mIsLButtonDown;
	mIsLButtonDown = false;
	return tLButtonDown;
}

bool CGUIWin32Platform::IsLButtonUp( CGUIPoint& rPoint, unsigned int& rFlag )
{
	rPoint	= mLButtonUpPoint;
	rFlag	= mLButtonUpFlag;
	bool tLButtonUp = mIsLButtonUp;
	mIsLButtonUp = false;
	return tLButtonUp;
}

bool CGUIWin32Platform::IsLButtonDblClk( CGUIPoint& rPoint, unsigned int& rFlag )
{
	rPoint	= mLButtonDblClkPoint;
	rFlag	= mLButtonDblClkFlag;
	bool tLButtonDblClk = mIsLButtonDblClk;
	mIsLButtonDblClk = false;
	return tLButtonDblClk;
}

bool CGUIWin32Platform::IsMButtonDown( CGUIPoint& rPoint, unsigned int& rFlag )
{
	rPoint	= mMButtonDownPoint;
	rFlag	= mMButtonDownFlag;
	bool tMButtonDown = mIsMButtonDown;
	mIsMButtonDown = false;
	return tMButtonDown;
}

bool CGUIWin32Platform::IsMButtonUp( CGUIPoint& rPoint, unsigned int& rFlag )
{
	rPoint	= mMButtonUpPoint;
	rFlag	= mMButtonUpFlag;
	bool tMButtonUp = mIsMButtonUp;
	mIsMButtonUp = false;
	return tMButtonUp;
}

bool CGUIWin32Platform::IsRButtonDown( CGUIPoint& rPoint, unsigned int& rFlag )
{
	rPoint	= mRButtonDownPoint;
	rFlag	= mRButtonDownFlag;
	bool tRButtonDown = mIsRButtonDown;
	mIsRButtonDown = false;
	return tRButtonDown;
}

bool CGUIWin32Platform::IsRButtonUp( CGUIPoint& rPoint, unsigned int& rFlag )
{
	rPoint	= mRButtonUpPoint;
	rFlag	= mRButtonUpFlag;
	bool tRButtonUp = mIsRButtonUp;
	mIsRButtonUp = false;
	return tRButtonUp;
}

int CGUIWin32Platform::GetTick( )
{
	return ::GetTickCount( );
}

int CGUIWin32Platform::GetKeyState( int vVirtualKey )
{
	return ::GetKeyState( vVirtualKey );
}

int CGUIWin32Platform::ClipboardPaste( char* pText, int& rSize )
{
	HWND tWnd = ::GetActiveWindow( );
	// µÃµ½ÄÚÈÝ
	::OpenClipboard( tWnd );
	HANDLE tHandleMemory = ::GetClipboardData( CF_TEXT );
	if ( tHandleMemory == NULL )
	{
		::CloseClipboard( );
		return -1;
	}

	int tSize = int( GlobalSize( tHandleMemory ) );
	if ( tSize > rSize )
	{
		::CloseClipboard( );
		rSize = tSize;
		return tSize;
	}

	BYTE* tpMemory = ( LPBYTE ) GlobalLock( tHandleMemory );
	::GlobalUnlock( tHandleMemory );
	::CloseClipboard( );
	::memcpy( pText, tpMemory, rSize );
	return 0;
}

int CGUIWin32Platform::ClipboardCopy( const char* pText, int tSize )
{
	HWND tWnd = ::GetActiveWindow( );
	HANDLE tHandleMemory = ::GlobalAlloc( GHND, tSize + 1 );
	BYTE* tpMemory = ( LPBYTE ) GlobalLock( tHandleMemory );

	::strcpy( (char*) tpMemory, pText );
	::GlobalUnlock( tHandleMemory );

	::OpenClipboard( tWnd );
	::EmptyClipboard( );
	::SetClipboardData( CF_TEXT, tpMemory );
	::CloseClipboard( );
	return 0;
}