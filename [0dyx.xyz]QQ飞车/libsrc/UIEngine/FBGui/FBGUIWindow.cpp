#include "FBGUIBase.h"
#include "FBGUICore.h"
#include "FBGUILua.h"
#include "FBGUIEvent.h"
#include "FBGUIWindow.h"
#include "FBGUIElement.h"
#include "FBGUISystem.h"

namespace FBGUI
{

namespace Sink
{

const int CGUISinkWindowUpdate::sEventType		= 0x00000001;
const int CGUISinkWindowRender::sEventType		= 0x00000002;
const int CGUISinkWindowCreate::sEventType		= 0x00000004;
const int CGUISinkWindowDestory::sEventType		= 0x00000005;
const int CGUISinkMButtonDown::sEventType		= 0x00000006;
const int CGUISinkMButtonUp::sEventType			= 0x00000007;
const int CGUISinkRButtonDown::sEventType		= 0x00000008;
const int CGUISinkRButtonUp::sEventType			= 0x00000009;
const int CGUISinkLButtonDown::sEventType		= 0x0000000A;
const int CGUISinkLButtonUp::sEventType			= 0x0000000B;
const int CGUISinkLButtonDblClk::sEventType		= 0x0000000C;
const int CGUISinkMouseMove::sEventType			= 0x0000000D;
const int CGUISinkMouseEnter::sEventType		= 0x0000000E;
const int CGUISinkMouseLeave::sEventType		= 0x0000000F;
const int CGUISinkWindowVisible::sEventType		= 0x00000010;
const int CGUISinkWindowEnable::sEventType		= 0x00000011;
const int CGUISinkWindowActive::sEventType		= 0x00000012;
const int CGUISinkWindowFocus::sEventType		= 0x00000013;
const int CGUISinkParentMoved::sEventType		= 0x00000014;
const int CGUISinkParentSized::sEventType		= 0x00000015;
const int CGUISinkWindowSizing::sEventType		= 0x00000016;
const int CGUISinkWindowSized::sEventType		= 0x00000017;
const int CGUISinkWindowMoving::sEventType		= 0x00000018;
const int CGUISinkWindowMoved::sEventType		= 0x00000019;
const int CGUISinkWindowKeyDown::sEventType		= 0x0000001A;
const int CGUISinkWindowKeyUp::sEventType		= 0x0000001B;
const int CGUISinkWindowSetFont::sEventType		= 0x0000001C;
const int CGUISinkMouseWheel::sEventType		= 0x0000001D;
const int CGUISinkClickButton::sEventType		= 0x0000001E;
const int CGUISinkThumbChanged::sEventType		= 0x0000001F;
const int CGUISinkListBoxSelChanged::sEventType = 0x00000020;
const int CGUISinkCommand::sEventType			= 0x00000021;
const int CGUISinkEditChanged::sEventType		= 0x00000022;
const int CGUISinkListSelChanged::sEventType	= 0x00000023;
const int CGUISinkListDblClk::sEventType		= 0x00000024;
const int CGUISinkSliderPosChanged::sEventType	= 0x00000025;
const int CGUISinkWindowDragFrom::sEventType	= 0x00000026;
const int CGUISinkWindowDragTo::sEventType		= 0x00000027;
const int CGUISinkTabSelChanged::sEventType		= 0x00000028;

};

// ************************************************************ //
// CGUIWindow
// ************************************************************ //

CGUIWindow::CGUIWindow( const char* pType, const char* pName, unsigned int vStyle, const CGUIRect& rRect, CGUIWindow* pParent, void* pUserData ) : 
	mType( pType ), mName( pName ), mTitle( "" ), mWindowStyle( vStyle ), mRect( rRect ), mpParent( pParent ), mWindowFlag( GUI_WF_VISIBLE | GUI_WF_ENABLE ), 
	mDragWindow( false ), mSizeWindow( false ), mArchor( GUI_TOPARCHOR_TOP | GUI_LEFTARCHOR_LEFT | GUI_RIGHTARCHOR_LEFT | GUI_BOTTOMARCHOR_TOP ), mpTexture( NULL ),
	mpHScrollBar( NULL ), mpVScrollBar( NULL ), mpExitBtn( NULL ), mTextColour( 0xFFFFC700 ), mResourceLoaded( false ), mIsModal( false )
{
}

CGUIWindow::~CGUIWindow( )
{
}

void CGUIWindow::Destroy( )
{
	CGUIWindowManager::GetSingleton( ).Destroy( this );
}

//! 判断指定窗口是否是该窗口的祖先
bool CGUIWindow::IsAncestor( CGUIWindow* pWindow )
{
	if ( mpParent == NULL )
		return false;

	if ( pWindow == mpParent )
		return true;

	return mpParent->IsAncestor( pWindow );
}

// 判断制定窗口是否为该窗口的后裔
bool CGUIWindow::IsDescendant( CGUIWindow* pWindow )
{
	// 遍历所有的子窗口
	for ( int i = 0; i < int( mChildren.size( ) ); i ++ )
	{
		// 如果是就返回
		if ( mChildren[ i ] == pWindow )
			return true;

		// 如果是其中一个子窗口的后裔就返回
		if ( mChildren[ i ]->IsDescendant( pWindow ) == true )
			return true;
	}

	// 如果都不是所有的子窗口的后裔，返回false
	return false;
}

//! 窗口是否可以见，如果该窗口对象或是有一个祖先不可见，那么该窗口不可见
//! 如果该窗口没有父窗口,那么仅由该窗口决定
bool CGUIWindow::IsWindowVisible( ) const
{
	if ( mWindowStyle & GUI_WS_POPUP ) return ( mWindowFlag & GUI_WF_VISIBLE );
	return ( mWindowFlag & GUI_WF_VISIBLE ) && ( mpParent == NULL ? true : mpParent->IsWindowVisible( ) );
}

//! 窗口是否可以用，如果该窗口对象或是有一个祖先不可用，那么该窗口不可用
//! 如果该窗口没有父窗口,那么仅由该窗口决定
bool CGUIWindow::IsWindowEnable( ) const
{
	if ( mWindowStyle & GUI_WS_POPUP ) return ( mWindowFlag & GUI_WF_ENABLE );
	return ( mWindowFlag & GUI_WF_ENABLE ) && ( mpParent == NULL ? true : mpParent->IsWindowEnable( ) );
}

//! 窗口是否激活，如果该窗口对象或是有一个祖先没有激活，那么该窗口没有激活
bool CGUIWindow::IsWindowActive( ) const
{
	if ( mWindowStyle & GUI_WS_POPUP ) return ( mWindowFlag & GUI_WF_ACTIVE );
	return ( mWindowFlag & GUI_WF_ACTIVE ) && ( mpParent == NULL ? true : mpParent->IsWindowActive( ) );
}

void CGUIWindow::VisibleWindow( bool vVisible )
{
	CGUISystem::GetSingleton( ).NotifyRedraw( );

	vVisible ? mWindowFlag |= GUI_WF_VISIBLE : mWindowFlag &= ~GUI_WF_VISIBLE; 
	OnWindowVisible( vVisible );
}

void CGUIWindow::EnableWindow( bool vEnable )
{
	CGUISystem::GetSingleton( ).NotifyRedraw( );

	vEnable ? mWindowFlag |= GUI_WF_ENABLE : mWindowFlag &= ~GUI_WF_ENABLE;
	OnWindowEnable( vEnable );
}

//! 将窗口标记为激活,将窗口移至所有兄弟窗口的前面
void CGUIWindow::ActiveWindow( bool vActive )
{
	CGUISystem::GetSingleton( ).NotifyRedraw( );

	vActive ? mWindowFlag |= GUI_WF_ACTIVE : mWindowFlag &= ~GUI_WF_ACTIVE;
	vActive ? BringWindowToTop( ) : NULL;
	OnWindowActive( vActive );
}

void CGUIWindow::BringWindowToTop( )
{
	CGUISystem::GetSingleton( ).NotifyRedraw( );
	// 如果没有父窗口，那么就没有兄弟
	if ( mpParent == NULL )
		return;

	for ( int i = 0; i < int( mpParent->mChildren.size( ) ); i ++ )
	{
		if ( mpParent->mChildren[ i ] == this )
		{
			mpParent->mChildren.erase( mpParent->mChildren.begin( ) + i );
			mpParent->mChildren.push_back( this );
			return;
		}
	}
}

CGUIWindow* CGUIWindow::WindowFromPoint( const CGUIPoint& rPoint )
{
	for ( int i = int( mChildren.size( ) - 1 ); i >= 0 ; i -- )
	{
		if ( mChildren[ i ]->IsWindowVisible( ) && mChildren[ i ]->IsWindowEnable( ) )
		{
			CGUIWindow* tpWnd = mChildren[ i ]->WindowFromPoint( rPoint );
			if ( tpWnd != NULL )
				return tpWnd;

			if ( mChildren[ i ]->IsHit( rPoint ) )
				return mChildren[ i ];
		}
	}

	if ( IsHit( rPoint ) )
		return this;

	return NULL;
}

CGUIPoint CGUIWindow::ClientToScreen( const CGUIPoint& rPoint ) const
{
	return WindowToScreen( rPoint + GetClientRect( ).mTopLeft );
}

CGUIRect CGUIWindow::ClientToScreen( const CGUIRect& rRect ) const
{
	return WindowToScreen( rRect + GetClientRect( ).mTopLeft  );
}

CGUIPoint CGUIWindow::ScreenToClient( const CGUIPoint& rPoint ) const
{
	return rPoint - GetClientClipRect( ).mTopLeft;
}

CGUIRect CGUIWindow::ScreenToClient( const CGUIRect& rRect ) const
{
	return rRect - GetClientClipRect( ).mTopLeft;
}

CGUIPoint CGUIWindow::WindowToScreen( const CGUIPoint& rPoint ) const
{
	if ( mWindowStyle & GUI_WS_POPUP ) return rPoint + mRect.mTopLeft;

	CGUIPoint tPoint = rPoint;
	const CGUIWindow* tpParent = this;
	while ( tpParent != NULL )
	{
		if ( tpParent->mWindowStyle & GUI_WS_POPUP ) 
			return tPoint + tpParent->mRect.mTopLeft;
		tPoint += tpParent->mRect.mTopLeft;
		tpParent = tpParent->GetParent( );
	}

	return tPoint;
}

CGUIRect CGUIWindow::WindowToScreen( const CGUIRect& rRect ) const
{
	if ( mWindowStyle & GUI_WS_POPUP ) return rRect + mRect.mTopLeft;

	CGUIRect tRect = rRect;
	const CGUIWindow* tpParent = this;
	while ( tpParent != NULL )
	{
		if ( tpParent->mWindowStyle & GUI_WS_POPUP ) 
			return tRect + tpParent->mRect.mTopLeft;
		tRect += tpParent->mRect.mTopLeft;
		tpParent = tpParent->GetParent( );
	}

	return tRect;
}

CGUIPoint CGUIWindow::ScreenToWindow( const CGUIPoint& rPoint ) const
{
	return rPoint - GetWindowClipRect( ).mTopLeft;
}

CGUIRect CGUIWindow::ScreenToWindow( const CGUIRect& rRect ) const
{
	return rRect - GetWindowClipRect( ).mTopLeft;
}

// 将指定窗口移动到该窗口的中部
void CGUIWindow::CenterWindow( CGUIWindow* pWindow )
{
	// 如果指定窗口不是该窗口的后裔，就返回
	if ( IsDescendant( pWindow ) == false )
		return;

	CGUIRect tRect = pWindow->GetRect( );
	int tLeft	= ( mRect.Width( ) - tRect.Width( ) ) >> 1;
	int tTop	= ( mRect.Height( ) - tRect.Height( ) ) >> 1;
	pWindow->MoveWindow( CGUIPoint( tLeft, tTop ) - tRect.mTopLeft );
}

CGUIRect CGUIWindow::GetWindowRect( ) const
{
	return CGUIRect( CGUIPoint( 0, 0 ), mRect.Size( ) );
}

CGUIRect CGUIWindow::GetClientRect( ) const
{
	int tTextHeight = CGUIWindowCanvas( NULL ).GetFontHeight( );
	CGUIRect tClientRect = CGUIRect( CGUIPoint( 0, 0 ), mRect.Size( ) );
	if ( mWindowStyle & GUI_WS_BORDER )	tClientRect.DeflateRect( GUI_WINDOW_BORDER_WIDTH, GUI_WINDOW_BORDER_HEIGHT );
	if ( mWindowStyle & GUI_WS_CAPTION ) tClientRect.mTopLeft.mY += tTextHeight;
	if ( mWindowStyle & GUI_WS_HSCROLL && mpHScrollBar->IsWindowVisible( ) ) tClientRect.mBottomRight.mY -= GUI_HSCROLLBAR_HEIGHT;
	if ( mWindowStyle & GUI_WS_VSCROLL && mpVScrollBar->IsWindowVisible( ) ) tClientRect.mBottomRight.mX -= GUI_VSCROLLBAR_WIDTH;

	return tClientRect;
}

CGUIRect CGUIWindow::GetCaptionRect( ) const
{
	int tTextHeight = CGUIWindowCanvas( NULL ).GetFontHeight( );
	CGUIRect tCaptionRect = CGUIRect( CGUIPoint( 0, 0 ), mRect.Size( ) );
	if ( mWindowStyle & GUI_WS_BORDER )
		tCaptionRect.DeflateRect( GUI_WINDOW_BORDER_WIDTH, GUI_WINDOW_BORDER_HEIGHT );

	if ( mWindowStyle & GUI_WS_CAPTION )
		tCaptionRect.mBottomRight.mY = tCaptionRect.mTopLeft.mY + tTextHeight;
	else
		tCaptionRect.SetRect( 0, 0, 0, 0 );

	return tCaptionRect;
}

CGUIRect CGUIWindow::GetSizeRect( ) const
{
	CGUIRect tSizeRect = CGUIRect( CGUIPoint( 0, 0 ), mRect.Size( ) );
	tSizeRect.mTopLeft = tSizeRect.mBottomRight - CGUISize( GUI_WINDOW_BORDER_WIDTH, GUI_WINDOW_BORDER_HEIGHT );
	return tSizeRect;
}

CGUIRect CGUIWindow::GetWindowClipRect( ) const
{
	if ( mpParent == NULL || mWindowStyle & GUI_WS_POPUP ) return mRect;
	return mpParent->WindowToScreen( mRect ) & mpParent->GetWindowClipRect( );
}

CGUIRect CGUIWindow::GetClientClipRect( ) const
{
	if ( mpParent == NULL || mWindowStyle & GUI_WS_POPUP ) return GetClientRect( ) + mRect.mTopLeft;
	return mpParent->WindowToScreen( GetClientRect( ) + mRect.mTopLeft ) & mpParent->GetWindowClipRect( );
}

void CGUIWindow::MouseClick( const CGUIPoint& rPoint )
{
	if ( mpParent != NULL )
	{
		// 如果有父窗口,那么该对象可能会有兄弟,把所有的兄弟都变成非激活状态
		for ( int i = 0; i < int( mpParent->mChildren.size( ) ); i ++ )
		{
			CGUIWindow* tpChildren = mpParent->mChildren[ i ];
			if ( tpChildren == this )
				continue;

			tpChildren->ActiveWindow( false );
		}
	}

	// 选择性的Deactive一些子窗口
	for ( int i = 0; i < int( mChildren.size( ) ); i ++ )
	{
		CGUIWindow* tpChildren = mChildren[ i ];
		if ( tpChildren->IsHit( rPoint ) == false )
            tpChildren->ActiveWindow( false );
	}

	// 这个调用将使得该窗口对象在所有的兄弟的前面,并且该窗口变成激活
	ActiveWindow( true );
}

void CGUIWindow::Render( )
{
	if ( IsWindowVisible( ) == false )
		return;

	// 装载窗口相关资源
	if ( mResourceLoaded == false )
	{
        OnLoadResource( );
		mResourceLoaded = true;
	}

	OnWindowRender( );
	for ( int i = 0; i < int( mChildren.size( ) ); i ++ )
		mChildren[ i ]->Render( );
}

void CGUIWindow::Update( )
{
	OnWindowUpdate( );
	for ( int i = 0; i < int( mChildren.size( ) ); i ++ )
		mChildren[ i ]->Update( );
}

void CGUIWindow::OnWindowRender( )
{
	FireEvent( &CGUISinkWindowRender( ) );
}

void CGUIWindow::OnWindowUpdate( )
{
	FireEvent( &CGUISinkWindowUpdate( ) );
}

void CGUIWindow::OnKeyDown( unsigned int vVirtualKey, unsigned int vFlag )
{
	if ( vVirtualKey == GUI_TAB )
	{
		// 如果是TabStop窗口的组窗口
		if ( mWindowStyle & GUI_WS_TABGROUP )
		{
			for ( int i = 0; i < int( mChildren.size( ) ); i ++ )
			{
				CGUIWindow* tpChild = mChildren[ i ];
				if ( tpChild->mWindowStyle & GUI_WS_TABSTOP && tpChild != this )
				{
					CGUISystem::GetSingleton( ).SetFocus( tpChild );
					break;
				}
			}		
		}
	}

	CGUISinkWindowKeyDown tWindowKeyDown;
	tWindowKeyDown.AddArg( CGUIDataType( GUI_UINT, vVirtualKey ) );
	tWindowKeyDown.AddArg( CGUIDataType( GUI_UINT, vFlag ) );
	FireEvent( &tWindowKeyDown );

	if ( mpParent != NULL )
		mpParent->OnKeyDown( vVirtualKey, vFlag );
}

void CGUIWindow::OnKeyUp( unsigned int vVirtualKey, unsigned int vFlag )
{
	CGUISinkWindowKeyUp tWindowKeyUp;
	tWindowKeyUp.AddArg( CGUIDataType( GUI_UINT, vVirtualKey ) );
	tWindowKeyUp.AddArg( CGUIDataType( GUI_UINT, vFlag ) );
	FireEvent( &tWindowKeyUp );

	if ( mpParent != NULL )
		mpParent->OnKeyUp( vVirtualKey, vFlag );
}

void CGUIWindow::OnChar( unsigned int vCharCode, unsigned int vFlag )
{
	// fire event
}

void CGUIWindow::OnSysKeyDown( unsigned int vVirtualKey, unsigned int vFlag )
{
	// fire event
}

void CGUIWindow::OnSysKeyUp( unsigned int vVirtualKey, unsigned int vFlag )
{
	// fire event
}

void CGUIWindow::OnUniChar( unsigned int vCharCode, unsigned int vFlag )
{
	// fire event
}

void CGUIWindow::OnSysDeadChar( unsigned int vCharCode, unsigned int vFlag )
{
	// fire event
}

void CGUIWindow::OnDeadChar( unsigned int vCharCode, unsigned int vFlag )
{
	// fire event
}

void CGUIWindow::OnMouseMButtonDown( const CGUIPoint& rPoint, unsigned int vFlag )
{
	MouseClick( rPoint );

	CGUISinkMButtonDown tMButtonDown;
	tMButtonDown.AddArg( CGUIDataType( GUI_PPOINT, (unsigned int) &rPoint ) );
	tMButtonDown.AddArg( CGUIDataType( GUI_UINT, vFlag ) );
	FireEvent( &tMButtonDown );
}

void CGUIWindow::OnMouseMButtonUp( const CGUIPoint& rPoint, unsigned int vFlag )
{
	CGUISinkMButtonUp tMButtonUp;
	tMButtonUp.AddArg( CGUIDataType( GUI_PPOINT, (unsigned int) &rPoint ) );
	tMButtonUp.AddArg( CGUIDataType( GUI_UINT, vFlag ) );
	FireEvent( &tMButtonUp );
}

void CGUIWindow::OnMouseRButtonDown( const CGUIPoint& rPoint, unsigned int vFlag )
{
	MouseClick( rPoint );

	CGUISinkRButtonDown tRButtonDown;
	tRButtonDown.AddArg( CGUIDataType( GUI_PPOINT, (unsigned int) &rPoint ) );
	tRButtonDown.AddArg( CGUIDataType( GUI_UINT, vFlag ) );
	FireEvent( &tRButtonDown );
}

void CGUIWindow::OnMouseRButtonUp( const CGUIPoint& rPoint, unsigned int vFlag )
{
	CGUISinkRButtonUp tRButtonUp;
	tRButtonUp.AddArg( CGUIDataType( GUI_PPOINT, (unsigned int) &rPoint ) );
	tRButtonUp.AddArg( CGUIDataType( GUI_UINT, vFlag ) );
	FireEvent( &tRButtonUp );
}

void CGUIWindow::OnMouseLButtonDown( const CGUIPoint& rPoint, unsigned int vFlag )
{
	// 如果点中的Caption
	CGUIRect tCaptionRect = WindowToScreen( GetCaptionRect( ) );
	if ( tCaptionRect.PtInRect( rPoint ) && mWindowStyle & GUI_WS_CANMOVE )
	{
		// 如果有退出按钮，并且没有点中退出按钮，或者按钮不可见
		if ( mpExitBtn != NULL && ( mpExitBtn->IsHit( rPoint ) == false || mpExitBtn->IsWindowVisible( ) == false ) )
		{
			mLastLButton	= rPoint;
			mDragWindow		= true;
			CGUISystem::GetSingleton( ).SetCapture( this );
		}
	}

	// 如果点中的SizeBar
	CGUIRect tSizeRect = WindowToScreen( GetSizeRect( ) );
	if ( tSizeRect.PtInRect( rPoint ) && mWindowStyle & GUI_WS_THICKFRAME )
	{
		mLastLButton	= rPoint;
		mSizeWindow		= true;
		CGUISystem::GetSingleton( ).SetCapture( this );
	}

	CGUISinkLButtonDown tLButtonDown;
	tLButtonDown.AddArg( CGUIDataType( GUI_PPOINT, (unsigned int) &rPoint ) );
	tLButtonDown.AddArg( CGUIDataType( GUI_UINT, vFlag ) );
	FireEvent( &tLButtonDown );
}

void CGUIWindow::OnMouseLButtonDblClk( const CGUIPoint& rPoint, unsigned int vFlag )
{
	CGUISinkLButtonDblClk tLButtonDblClk;
	tLButtonDblClk.AddArg( CGUIDataType( GUI_PPOINT, (unsigned int) &rPoint ) );
	tLButtonDblClk.AddArg( CGUIDataType( GUI_UINT, vFlag ) );
	FireEvent( &tLButtonDblClk );
}

void CGUIWindow::OnMouseLButtonUp( const CGUIPoint& rPoint, unsigned int vFlag )
{
	mDragWindow	= false;
	mSizeWindow	= false;
	CGUISystem::GetSingleton( ).ReleaseCapture( this );

	CGUISinkLButtonUp tLButtonUp;
	tLButtonUp.AddArg( CGUIDataType( GUI_PPOINT, (unsigned int) &rPoint ) );
	tLButtonUp.AddArg( CGUIDataType( GUI_UINT, vFlag ) );
	FireEvent( &tLButtonUp );
}

void CGUIWindow::OnMouseMove( const CGUIPoint& rPoint, unsigned int vFlag )
{
	if ( vFlag & GUI_LBUTTON && mDragWindow )
		mLastLButton += MoveWindow( rPoint - mLastLButton );

	if ( vFlag & GUI_LBUTTON && mSizeWindow )
		mLastLButton += SizeWindow( rPoint - mLastLButton );

	CGUISinkMouseMove tMouseMove;
	tMouseMove.AddArg( CGUIDataType( GUI_PPOINT, (unsigned int) &rPoint ) );
	tMouseMove.AddArg( CGUIDataType( GUI_UINT, vFlag ) );
	FireEvent( &tMouseMove );
}

void CGUIWindow::OnMouseEnter( const CGUIPoint& rPoint, unsigned int vFlag )
{
	CGUISinkMouseEnter tMouseEnter;
	tMouseEnter.AddArg( CGUIDataType( GUI_PPOINT, (unsigned int) &rPoint ) );
	tMouseEnter.AddArg( CGUIDataType( GUI_UINT, vFlag ) );
	FireEvent( &tMouseEnter );
}

void CGUIWindow::OnMouseLeave( const CGUIPoint& rPoint, unsigned int vFlag )
{
	CGUISinkMouseLeave tMouseLeave;
	tMouseLeave.AddArg( CGUIDataType( GUI_PPOINT, (unsigned int) &rPoint ) );
	tMouseLeave.AddArg( CGUIDataType( GUI_UINT, vFlag ) );
	FireEvent( &tMouseLeave );
}
 
void CGUIWindow::OnMouseWheel( const CGUIPoint& rPoint, unsigned int vFlag )
{
	if ( mpVScrollBar != NULL )
		mpVScrollBar->OnMouseWheel( rPoint, vFlag );
	else if ( mpHScrollBar != NULL )
		mpHScrollBar->OnMouseWheel( rPoint, vFlag );

	CGUISinkMouseWheel tMouseWheel;
	tMouseWheel.AddArg( CGUIDataType( GUI_PPOINT, (unsigned int) &rPoint ) );
	tMouseWheel.AddArg( CGUIDataType( GUI_UINT, vFlag ) );
	FireEvent( &tMouseWheel );
}

void CGUIWindow::OnWindowVisible( bool vVisible )
{
	CGUISinkWindowVisible tWindowVisible;
	tWindowVisible.AddArg( CGUIDataType( GUI_BOOL, vVisible ) );
	FireEvent( &tWindowVisible );
}

void CGUIWindow::OnWindowEnable( bool vEnable )
{
	CGUISinkWindowEnable tWindowEnable;
	tWindowEnable.AddArg( CGUIDataType( GUI_BOOL, vEnable ) );
	FireEvent( &tWindowEnable );
}

void CGUIWindow::OnWindowActive( bool vActive )
{
	CGUISinkWindowActive tWindowActive;
	tWindowActive.AddArg( CGUIDataType( GUI_BOOL, vActive ) );
	FireEvent( &tWindowActive );
}

void CGUIWindow::OnWindowFocus( CGUIWindow* pWindow, bool vFocus )
{
	CGUISinkWindowFocus tWindowFocus;
	tWindowFocus.AddArg( CGUIDataType( GUI_PWINDOW, (unsigned int) pWindow ) );
	tWindowFocus.AddArg( CGUIDataType( GUI_BOOL, vFocus ) );
	FireEvent( &tWindowFocus );
}

void CGUIWindow::AdjustVScrollBar( )
{
	if ( mpVScrollBar == NULL ) return;
	int tLeft, tTop, tHeight = 0;
	int tTextHeight = CGUIWindowCanvas( NULL ).GetFontHeight( );
	CGUIRect tWindowRect = GetWindowRect( );
	if ( mWindowStyle & GUI_WS_HSCROLL && mpHScrollBar != NULL && mpHScrollBar->IsWindowVisible( ) )
	{
		if ( mWindowStyle & GUI_WS_CAPTION )
            tHeight = tWindowRect.Height( ) - GUI_HSCROLLBAR_HEIGHT - tTextHeight;
		else
			tHeight = tWindowRect.Height( ) - GUI_HSCROLLBAR_HEIGHT;
	}
	else
	{
		if ( mWindowStyle & GUI_WS_CAPTION )
			tHeight = tWindowRect.Height( ) - tTextHeight;
		else
			tHeight = tWindowRect.Height( );
	}

	if ( mWindowStyle & GUI_WS_BORDER )
	{
		if ( mWindowStyle & GUI_WS_CAPTION )
            tTop = tWindowRect.mTopLeft.mY + tTextHeight + GUI_WINDOW_BORDER_HEIGHT;
		else
			tTop = tWindowRect.mTopLeft.mY + GUI_WINDOW_BORDER_HEIGHT;
		tLeft = tWindowRect.mBottomRight.mX - GUI_VSCROLLBAR_WIDTH - GUI_WINDOW_BORDER_WIDTH;
		tHeight -= ( GUI_WINDOW_BORDER_HEIGHT << 1 );
	}
	else
	{
		if ( mWindowStyle & GUI_WS_CAPTION )
            tTop = tWindowRect.mTopLeft.mY + tTextHeight;
		else
			tTop = tWindowRect.mTopLeft.mY;
		tLeft = tWindowRect.mBottomRight.mX - GUI_VSCROLLBAR_WIDTH;
	}

	CGUIRect	tNewRect	= CGUIRect( CGUIPoint( tLeft, tTop ), CGUISize( GUI_VSCROLLBAR_WIDTH, tHeight ) );
	CGUIPoint	tMoveOffset = tNewRect.mTopLeft - mpVScrollBar->mRect.mTopLeft;
	CGUIPoint	tSizeOffset = tNewRect.mBottomRight - tNewRect.mTopLeft - mpVScrollBar->mRect.Size( );
	if ( mpVScrollBar != NULL )
	{
		mpVScrollBar->MoveWindow( tMoveOffset );
		mpVScrollBar->SizeWindow( tSizeOffset );
	}
}

void CGUIWindow::AdjustHScrollBar( )
{
	if ( mpHScrollBar == NULL ) return;
	int tLeft, tTop, tWidth = 0;
	CGUIRect tWindowRect = GetWindowRect( );
	if ( mWindowStyle & GUI_WS_VSCROLL && mpVScrollBar != NULL && mpVScrollBar->IsWindowVisible( )  )
		tWidth = tWindowRect.Width( ) - GUI_VSCROLLBAR_WIDTH;
	else
		tWidth = tWindowRect.Width( );

	if ( mWindowStyle & GUI_WS_BORDER )
	{
		tTop  = tWindowRect.mBottomRight.mY - GUI_HSCROLLBAR_HEIGHT - GUI_WINDOW_BORDER_HEIGHT;
		tLeft = tWindowRect.mTopLeft.mX + GUI_WINDOW_BORDER_WIDTH;
		tWidth -= ( GUI_WINDOW_BORDER_WIDTH << 1 );
	}
	else
	{
		tTop  = tWindowRect.mBottomRight.mY - GUI_HSCROLLBAR_HEIGHT;
		tLeft = tWindowRect.mTopLeft.mX;
	}

	CGUIRect	tNewRect	= CGUIRect( CGUIPoint( tLeft, tTop ), CGUISize( tWidth, GUI_HSCROLLBAR_HEIGHT ) );
	CGUIPoint	tMoveOffset = tNewRect.mTopLeft - mpHScrollBar->mRect.mTopLeft;
	CGUIPoint	tSizeOffset = tNewRect.mBottomRight - tNewRect.mTopLeft - mpHScrollBar->mRect.Size( );
	if ( mpHScrollBar != NULL )
	{
		mpHScrollBar->MoveWindow( tMoveOffset );
		mpHScrollBar->SizeWindow( tSizeOffset );
	}
}

bool CGUIWindow::UpdateScrollBar( int vLogicWidth, int vLogicHeight )
{
	CGUIRect tClientRect = GetClientRect( );
	bool tNeedRefresh = false;
	if ( vLogicHeight > tClientRect.Height( ) )
	{
		if ( mWindowStyle & GUI_WS_VSCROLL && mpVScrollBar->IsWindowVisible( ) == false )
		{
            mpVScrollBar->VisibleWindow( true );
			tNeedRefresh = true;
		}

		if ( mpVScrollBar != NULL )
		{
            mpVScrollBar->SetRange( vLogicHeight, tClientRect.Height( ) );
			mpVScrollBar->SetScrollSize( 1, tClientRect.Height( ) );
		}
	}
	else
	{
		if ( mWindowStyle & GUI_WS_VSCROLL && mpVScrollBar->IsWindowVisible( ) == true )
		{
            mpVScrollBar->VisibleWindow( false );
			tNeedRefresh = true;
		}

		if ( mpVScrollBar != NULL )
            mpVScrollBar->SetPosition( 0 );
	}

	// 如果超过了最大的横向显示区域,就要显示横向滚动条
	if ( vLogicWidth > tClientRect.Width( ) )
	{
		if ( mWindowStyle & GUI_WS_HSCROLL && mpHScrollBar->IsWindowVisible( ) == false )
		{
            mpHScrollBar->VisibleWindow( true );
			tNeedRefresh = true;
		}

		if ( mpHScrollBar != NULL )
		{
			mpHScrollBar->SetRange( vLogicWidth, tClientRect.Width( ) );
			mpHScrollBar->SetScrollSize( 1, tClientRect.Width( ) );
		}
	}
	else
	{
		if ( mWindowStyle & GUI_WS_HSCROLL && mpHScrollBar->IsWindowVisible( ) == true )
		{
            mpHScrollBar->VisibleWindow( false );
			tNeedRefresh = true;
		}

		if ( mpHScrollBar != NULL )
			mpHScrollBar->SetPosition( 0 );
	}

	if ( tNeedRefresh == true )
	{
		AdjustHScrollBar( );
		AdjustVScrollBar( );
		tClientRect = GetClientRect( );
		
		if ( mpVScrollBar != NULL )
            mpVScrollBar->SetRange( vLogicHeight, tClientRect.Height( ) );

		if ( mpHScrollBar != NULL )
            mpHScrollBar->SetRange( vLogicWidth, tClientRect.Width( ) );

		CGUISystem::GetSingleton( ).NotifyRedraw( );
		UpdateScrollBar( vLogicWidth, vLogicHeight );
	}
	
	return tNeedRefresh;
}

int CGUIWindow::OnWindowCreate( )
{
	// 设置窗口字体
	OnSetFont( );
	// 建立滚动条
	CGUIWindowManager* tpWmgr = CGUIWindowManager::GetSingleton( ).GetSingletonPtr( );
	int tTextHeight = CGUIWindowCanvas( NULL ).GetFontHeight( );
	CGUIRect tWindowRect = GetWindowRect( );
	if ( mWindowStyle & GUI_WS_HSCROLL )
	{
		mpHScrollBar = (CGUIScrollBar*) tpWmgr->Create( "GUIScrollBar", NULL, GUI_SBS_HORZ, CGUIRect( ), this, NULL );
		mpHScrollBar->SetArchor( GUI_TOPARCHOR_BOTTOM | GUI_BOTTOMARCHOR_BOTTOM | GUI_LEFTARCHOR_LEFT | GUI_RIGHTARCHOR_RIGHT );
		mpHScrollBar->VisibleWindow( false );
		ON_SB_THUMBCHANGED( mpHScrollBar, OnHScrollBar );
	}

	if ( mWindowStyle & GUI_WS_VSCROLL )
	{
		mpVScrollBar = (CGUIScrollBar*) tpWmgr->Create( "GUIScrollBar", NULL, GUI_SBS_VERT, CGUIRect( ), this, NULL );
		mpVScrollBar->SetArchor( GUI_TOPARCHOR_TOP | GUI_BOTTOMARCHOR_BOTTOM | GUI_LEFTARCHOR_RIGHT | GUI_RIGHTARCHOR_RIGHT );
		mpVScrollBar->VisibleWindow( false );
		ON_SB_THUMBCHANGED( mpVScrollBar, OnVScrollBar );
	}

	int tLeft, tTop = 0;
	// 建立关闭按钮
	if ( mWindowStyle & GUI_WS_SYSMENU && mWindowStyle & GUI_WS_CAPTION )
	{
		if ( mWindowStyle & GUI_WS_BORDER )
		{
			tTop  = tWindowRect.mTopLeft.mY + GUI_WINDOW_BORDER_HEIGHT + ( ( tTextHeight - GUI_WINDOW_CLOSEBUTTON_HEIGHT ) >> 1 );
			tLeft = tWindowRect.mBottomRight.mX - GUI_WINDOW_BORDER_WIDTH - GUI_WINDOW_CLOSEBUTTON_WIDTH - 1;
		}
		else
		{
			tTop  = tWindowRect.mTopLeft.mY + ( ( tTextHeight - GUI_WINDOW_CLOSEBUTTON_HEIGHT ) >> 1 );
			tLeft = tWindowRect.mBottomRight.mX - GUI_WINDOW_CLOSEBUTTON_WIDTH - 1;
		}

		CGUISize tButtonSize = CGUISize( GUI_WINDOW_CLOSEBUTTON_WIDTH, GUI_WINDOW_CLOSEBUTTON_HEIGHT );
		mpExitBtn = (CGUIImageButton*) tpWmgr->Create( "GUIImageButton", NULL, 0, CGUIRect( CGUIPoint( tLeft, tTop ), tButtonSize ), this, NULL );
		mpExitBtn->SetArchor( GUI_TOPARCHOR_TOP | GUI_BOTTOMARCHOR_TOP | GUI_LEFTARCHOR_RIGHT | GUI_RIGHTARCHOR_RIGHT );
		( (CGUIImageButton*) mpExitBtn )->SetImage( "CloseButton.png", tButtonSize );
		ON_BN_CLICKED( mpExitBtn, OnCancel )
	}

	FireEvent( &CGUISinkWindowCreate( ) );	
	return 0;
}

void CGUIWindow::OnCancel( CGUIWindow* pButton )
{
	Destroy( );
}

void CGUIWindow::OnHScrollBar( CGUIWindow* pScrollBar, int vNewPos )
{
	mCamPoint.mX = vNewPos;
}

void CGUIWindow::OnVScrollBar( CGUIWindow* pScrollBar, int vNewPos )
{
	mCamPoint.mY = vNewPos;
}

void CGUIWindow::OnWindowDestory( )
{
	// 卸载所有资源
	OnDestroyResource( );

	// 销毁所有子窗口
	if ( mpParent != NULL ) 
		mpParent->RemoveChildWindow( this );

	if ( mIsModal == true )
		CGUISystem::GetSingleton( ).RemovePopupWindow( this );

	RemoveAllChildWindow( );
	FireEvent( &CGUISinkWindowDestory( ) );	
}

void CGUIWindow::OnParentMoved( const CGUIPoint& rOffset )
{
	CGUISinkParentMoved tParentMoved;
	tParentMoved.AddArg( CGUIDataType( GUI_PPOINT, (unsigned int) &rOffset ) );
	FireEvent( &tParentMoved );
}

void CGUIWindow::OnParentSized( const CGUIPoint& rOffset )
{
	if ( mWindowStyle & GUI_WS_POPUP ) return;

	CGUIPoint tNowOffset	= rOffset - mLossOffset;
	CGUIPoint tTopLeft		= mRect.mTopLeft;
	CGUIPoint tBottomRight	= mRect.mBottomRight;
	if ( mArchor & GUI_TOPARCHOR_TOP ) tTopLeft.mY = mRect.mTopLeft.mY;
	if ( mArchor & GUI_TOPARCHOR_BOTTOM ) tTopLeft.mY = mRect.mTopLeft.mY + tNowOffset.mY;
	if ( mArchor & GUI_LEFTARCHOR_LEFT ) tTopLeft.mX = mRect.mTopLeft.mX;
	if ( mArchor & GUI_LEFTARCHOR_RIGHT ) tTopLeft.mX = mRect.mTopLeft.mX + tNowOffset.mX;
	if ( mArchor & GUI_BOTTOMARCHOR_TOP ) tBottomRight.mY = mRect.mBottomRight.mY;
	if ( mArchor & GUI_BOTTOMARCHOR_BOTTOM ) tBottomRight.mY = mRect.mBottomRight.mY + tNowOffset.mY;
	if ( mArchor & GUI_RIGHTARCHOR_LEFT ) tBottomRight.mX = mRect.mBottomRight.mX;
	if ( mArchor & GUI_RIGHTARCHOR_RIGHT ) tBottomRight.mX = mRect.mBottomRight.mX + tNowOffset.mX;

	MoveWindow( tTopLeft - mRect.mTopLeft );
	CGUIPoint tOffset = tBottomRight - tTopLeft - mRect.Size( );
	mLossOffset = SizeWindow( tOffset ) - tOffset;

	CGUISinkParentSized tParentSized;
	tParentSized.AddArg( CGUIDataType( GUI_PPOINT, (unsigned int) &rOffset ) );
	FireEvent( &tParentSized );
}

CGUIPoint CGUIWindow::OnWindowMoving( const CGUIPoint& rOffset )
{
	CGUISinkWindowMoving tWindowMoving;
	tWindowMoving.AddArg( CGUIDataType( GUI_PPOINT, (unsigned int) &rOffset ) );
	FireEvent( &tWindowMoving );

	return rOffset;
}

void CGUIWindow::OnWindowMoved( const CGUIPoint& rOffset )
{
	CGUISinkWindowMoved tWindowMoved;
	tWindowMoved.AddArg( CGUIDataType( GUI_PPOINT, (unsigned int) &rOffset ) );
	FireEvent( &tWindowMoved );
}

CGUIPoint CGUIWindow::OnWindowSizing( const CGUIPoint& rOffset )
{
	CGUISinkWindowSizing tWindowSizing;
	tWindowSizing.AddArg( CGUIDataType( GUI_PPOINT, (unsigned int) &rOffset ) );
	FireEvent( &tWindowSizing );

	return rOffset;
}

void CGUIWindow::OnWindowSized( const CGUIPoint& rOffset  )
{
	CGUISinkWindowSized tWindowSized;
	tWindowSized.AddArg( CGUIDataType( GUI_PPOINT, (unsigned int) &rOffset ) );
	FireEvent( &tWindowSized );
}

void CGUIWindow::OnLoadResource( )
{
}

void CGUIWindow::OnDestroyResource( )
{
}

void CGUIWindow::OnSetFont( )
{
	// 默认将设置系统字体，系统字体由项目设置
	mpFont = CGUIFontManager::GetSingleton( ).GetFont( "SystemFont" );

	// 点燃字体设置事件
	CGUISinkWindowSetFont tWindowSetFont;
	tWindowSetFont.AddArg( CGUIDataType( GUI_PWINDOW, (unsigned int) this ) );
	FireEvent( &tWindowSetFont );
}

CGUIPoint CGUIWindow::MoveWindow( const CGUIPoint& rOffset )
{
	CGUIPoint tOffset = OnWindowMoving( rOffset );
	mRect += tOffset;

	// 通知所有的子窗口
	for ( int i = 0; i < int( mChildren.size( ) ); i ++ )
		mChildren[ i ]->OnParentMoved( tOffset );

	OnWindowMoved( tOffset );
	mLossOffset = CGUIPoint( 0, 0 );
	CGUISystem::GetSingleton( ).NotifyRedraw( );
	return tOffset;
}

CGUIPoint CGUIWindow::SizeWindow( const CGUIPoint& rOffset )
{
	CGUIPoint tOffset = OnWindowSizing( rOffset );
	mRect.mBottomRight += tOffset;

	// 通知所有的子窗口
	for ( int i = 0; i < int( mChildren.size( ) ); i ++ )
		mChildren[ i ]->OnParentSized( tOffset );

	OnWindowSized( tOffset );
	CGUISystem::GetSingleton( ).NotifyRedraw( );
	return tOffset;
}

void CGUIWindow::RemoveChildWindow( CGUIWindow* pWindow )
{
	for ( int i = 0; i < int( mChildren.size( ) ); i ++ )
	{
		if ( mChildren[ i ] == pWindow )
		{
			mChildren.erase( mChildren.begin( ) + i );
			pWindow->SetParent( NULL );
			break;
		}
	}
}

void CGUIWindow::RemoveAllChildWindow( )
{
	for ( int i = 0; i < int( mChildren.size( ) ); )
		CGUIWindowManager::GetSingleton( ).Destroy( mChildren[ i ] );
}

void CGUIWindow::AddChildWindow( CGUIWindow* pChildWindow )
{
	mChildren.push_back( pChildWindow );
	pChildWindow->OnParentSized( CGUIPoint( 0, 0 ) );
}

// ************************************************************ //
// CGUIWindowManager
// ************************************************************ //
template<> CGUIWindowManager* CSingleton< CGUIWindowManager >::spSingleton = NULL;

CGUIWindowManager::~CGUIWindowManager( )
{
	for ( int i = 0; i < int( mWindowsRegister.size( ) ); i ++ )
		Destroy( mWindowsRegister[ i ] );

	mWindowsRegister.clear( );
	ClearDeathPool( );
}

CGUIWindow* CGUIWindowManager::DoModal( const char* pType, const char* pName, unsigned int vStyle, const CGUIRect& rRect, void* pUserData )
{
	CGUIWindow* tpWindow = Create( pType, pName, vStyle, rRect, NULL, pUserData );
	tpWindow->mIsModal = true;
	for ( int i = 0; i < int( CGUISystem::GetSingleton( ).mPopups.size( ) ); i ++ )
		CGUISystem::GetSingleton( ).mPopups[ i ]->ActiveWindow( false );
	CGUISystem::GetSingleton( ).GetSheet( )->ActiveWindow( false );
	CGUISystem::GetSingleton( ).mPopups.push_back( tpWindow );

	CGUISize tWindowSize = tpWindow->GetWindowSize( );
	CGUISize tSize = CGUISystem::GetSingleton( ).GetViewportExt( );
	int tLeft = ( tSize.mCX - tWindowSize.mCX ) >> 1;
	int tTop = ( tSize.mCY - tWindowSize.mCY ) >> 1;
	tpWindow->MoveWindow( CGUIPoint( tLeft, tTop ) );
	return tpWindow;
}

CGUIWindow* CGUIWindowManager::Create( const char* pType, const char* pName, unsigned int vStyle, const CGUIRect& rRect, CGUIWindow* pParent, void* pUserData )
{
	CGUIWindowFactory* tpFactory = CGUIWindowFactoryManager::GetSingleton( ).GetFactory( pType );
	CGUIWindow*	tpWindow = NULL;
	if ( tpFactory == NULL )
		return NULL;

	char tWindowName[ 255 ] = { 0 };
	if ( pName == NULL )
	{
		::sprintf( tWindowName, "SystemNamed_%d", mUIDCounter ++ );
		pName = tWindowName;
	}
	
	tpWindow = tpFactory->Create( pName, vStyle, rRect, pParent, pUserData );
	mWindowsRegister.push_back( tpWindow );

	if ( tpWindow->OnWindowCreate( ) == -1 )
	{
		Destroy( tpWindow );
		return NULL;
	}

	if ( pParent != NULL )
		pParent->AddChildWindow( tpWindow );

	tpWindow->ActiveWindow( true );
	return tpWindow;
}

void CGUIWindowManager::Destroy( CGUIWindow* pWindow )
{
	if ( pWindow != NULL )
        Destroy( pWindow->GetName( ).c_str( ) );
}

void CGUIWindowManager::Destroy( const char* pName )
{
	for ( int i = 0; i < int( mWindowsRegister.size( ) ); i ++ )
	{
		if ( ::strcmp( mWindowsRegister[ i ]->GetName( ).c_str( ), pName ) == 0 )
		{
			CGUIWindow* tpWindow = mWindowsRegister[ i ];
			mWindowsRegister.erase( mWindowsRegister.begin( ) + i );
			tpWindow->OnWindowDestory( );

			mWindowsDeath.push_back( tpWindow );
			return;
		}
	}
}

CGUIWindow* CGUIWindowManager::GetWindow( const char* pName )
{
	for ( int i = 0; i < int( mWindowsRegister.size( ) ); i ++ )
	{
		if ( ::strcmp( mWindowsRegister[ i ]->GetName( ).c_str( ), pName ) == 0 )
			return mWindowsRegister[ i ];
	}

	return NULL;
}

void CGUIWindowManager::ClearDeathPool( )
{
	CGUISystem* tpSystem = CGUISystem::GetSingletonPtr( );
	CGUIWindowFactoryManager* tpWfmgr = CGUIWindowFactoryManager::GetSingletonPtr( );
	for ( int i = 0; i < int( mWindowsDeath.size( ) ); i ++ )
	{
		CGUIWindow* tpWindow = mWindowsDeath[ i ];
		if ( tpSystem->GetCapture( ) == tpWindow ) tpSystem->ReleaseCapture( tpWindow );
		if ( tpSystem->GetFocus( ) == tpWindow ) tpSystem->SetFocus( NULL );
		if ( tpSystem->mpHover == tpWindow ) tpSystem->mpHover = NULL;
		if ( tpSystem->mpDragSource == tpWindow ) tpSystem->mpDragSource = NULL;
		tpWfmgr->GetFactory( tpWindow->GetType( ).c_str( ) )->Destroy( tpWindow );
	}
	mWindowsDeath.clear( );
}

// ************************************************************ //
// CGUIWindowFactoryManager
// ************************************************************ //
template<> CGUIWindowFactoryManager* CSingleton< CGUIWindowFactoryManager >::spSingleton = NULL;

CGUIWindowFactoryManager::CGUIWindowFactoryManager( )
{
}

CGUIWindowFactoryManager::~CGUIWindowFactoryManager( )
{
	for ( int i = 0; i < int( mFactorysRegister.size( ) ); i ++ )
		delete mFactorysRegister[ i ];

	mFactorysRegister.clear( );
}

CGUIWindowFactory* CGUIWindowFactoryManager::GetFactory( const char* pType )
{
	for ( int i = 0; i < int( mFactorysRegister.size( ) ); i ++ )
	{
		if ( strcmp( mFactorysRegister[ i ]->GetType( ).c_str( ), pType ) == 0 )
			return mFactorysRegister[ i ];
	}

	return NULL;
}

}