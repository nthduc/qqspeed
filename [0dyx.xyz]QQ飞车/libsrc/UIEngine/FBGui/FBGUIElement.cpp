#include "FBGUIBase.h"
#include "FBGUICore.h"
#include "FBGUILua.h"
#include "FBGUIEvent.h"
#include "FBGUIWindow.h"
#include "FBGUIElement.h"
#include "FBGUISystem.h"
#include "FBGUIMemory.h"
#include <direct.h>
#include <sys/stat.h>
#include <io.h>
#include <time.h>

namespace FBGUI
{

namespace Element
{

// ************************************************************ //
// CGUISheet
// ************************************************************ //

CGUISheet::CGUISheet( const char* pType, const char* pName, unsigned int vStyle, const CGUIRect& rRect, CGUIWindow* pParent, void* pUserData )
	: CGUIWindow( pType, pName, GUI_WS_NORMAL, rRect, NULL, NULL )	
{
	mRect = CGUIRect( CGUIPoint( 0, 0 ), CGUISystem::GetSingleton( ).GetViewportExt( ) );
}

void CGUISheet::OnWindowRender( )
{
	CGUIWindowCanvas tCanvas( this );
	tCanvas.FillRect( CGUIRect( CGUIPoint( 0, 0 ), GetClientSize( ) ), 0xFF000000 );
}

void CGUISheet::OnRenderDragTexture( CGUITexture* pTexture, const CGUIPoint& rPoint )
{
	CGUIClientCanvas tCanvas( this );
	if ( pTexture != NULL )
	{
		CGUIRect tSrcRect( 0, 0, pTexture->GetTexWidth( ), pTexture->GetTexHeight( ) );
		tCanvas.BitBlt( pTexture, tSrcRect, tSrcRect + rPoint, 0xFFFFFFFF );
	}
}

// ************************************************************ //
// CGUIFrameWindow
// ************************************************************ //
void CGUIFrame::OnWindowRender( )
{
	CGUIRect tCaptionRect = GetCaptionRect( );
	CGUIWindowCanvas tWindowCanvas( this );
	tWindowCanvas.DrawCombine( mpTexture, GetWindowRect( ), CGUIPoint( 0, 0 ), CGUISize( 50, 50 ) );
	if ( IsWindowActive( ) == true )
		tWindowCanvas.FillRect( tCaptionRect, 0xFF400000 );

	if ( mWindowStyle & GUI_WS_CAPTION )
        tWindowCanvas.PrintText( mTitle.c_str( ), tCaptionRect, GUI_DT_LEFT | GUI_DT_VCENTER, mTextColour );
}

CGUIPoint CGUIFrame::OnWindowSizing( const CGUIPoint& rOffset )
{
	CGUIWindow::OnWindowSizing( rOffset );

	CGUIRect tRect = mRect;
	tRect.mBottomRight += rOffset;
	CGUIPoint tOffset = rOffset;
	if ( tRect.Width( ) < 100 ) tOffset.mX = 100 - mRect.Width( );
	if ( tRect.Height( ) < 100 ) tOffset.mY = 100 - mRect.Height( );
	return tOffset;
}

void CGUIFrame::OnMouseRButtonUp( const CGUIPoint& rPoint, unsigned int vFlag )
{
	CGUIMenu* tpMenu = CGUISystem::GetSingleton( ).TrackPopupMenu( rPoint, this );
	HMENUITEM tpSubMenu1 = tpMenu->AppendItem( NULL, "Menu1", 0, GUI_MS_POPUP );
	tpMenu->AppendItem( NULL, "Menu1", 0, GUI_MS_SEPARATOR );
	tpMenu->AppendItem( NULL, "Menu2", 0, 0 );
	tpMenu->AppendItem( tpSubMenu1, "Submenu1", 0, 0 );
	CGUISystem::GetSingleton( ).NotifyRedraw( );

	CGUIWindow::OnMouseRButtonUp( rPoint, vFlag );
}

void CGUIFrame::OnLoadResource( )
{
	GUI_LOADTEXTURE( mpTexture, "Frame.png" );
}

void CGUIFrame::OnDestroyResource( )
{
	GUI_DESTROYTEXTURE( mpTexture )
}

// ************************************************************ //
// CGUIButton
// ************************************************************ //
void CGUIButton::OnWindowRender( )
{
	CGUIRect tClientRect = GetClientRect( );
	CGUIWindowCanvas tCanvas( this );
	if ( IsWindowEnable( ) == false )
		tCanvas.DrawCombine( mpTexture, tClientRect, CGUIPoint( 0, 135 ), CGUISize( 15, 15 ) );
	else if ( mWindowFlag & GUI_BF_CLICKED )
		tCanvas.DrawCombine( mpTexture, tClientRect, CGUIPoint( 0, 90 ), CGUISize( 15, 15 ) );
	else if ( mWindowFlag & GUI_BF_HOVER )
		tCanvas.DrawCombine( mpTexture, tClientRect, CGUIPoint( 0, 45 ), CGUISize( 15, 15 ) );
	else
		tCanvas.DrawCombine( mpTexture, tClientRect, CGUIPoint( 0, 0 ), CGUISize( 15, 15 ) );

	if ( mWindowFlag & GUI_BF_CLICKED )	tClientRect += CGUIPoint( 1, 1 );
	tCanvas.PrintText( mTitle.c_str( ), tClientRect, GUI_DT_VCENTER | GUI_DT_HCENTER, mTextColour );
}

// 鼠标左键抬起消息处理函数。
void CGUIButton::OnMouseLButtonUp( const CGUIPoint& rPoint, unsigned int vFlag )
{
	CGUIWindow::OnMouseLButtonUp( rPoint, vFlag );
	mWindowFlag &= ~GUI_BF_CLICKED;

	CGUISinkClickButton tClickButton;
	tClickButton.AddArg( CGUIDataType( GUI_PWINDOW, (unsigned int) this ) );
	FireEvent( &tClickButton );

	CGUISystem::GetSingleton( ).NotifyRedraw( );
}

// 鼠标左键按下消息处理函数。
void CGUIButton::OnMouseLButtonDown( const CGUIPoint& rPoint, unsigned int vFlag )
{
	CGUIWindow::OnMouseLButtonDown( rPoint, vFlag );
	CGUISystem::GetSingleton( ).NotifyRedraw( );
	mWindowFlag |= GUI_BF_CLICKED;
}

void CGUIButton::OnMouseLeave( const CGUIPoint& rPoint, unsigned int vFlag )
{
	CGUIWindow::OnMouseLeave( rPoint, vFlag );
	CGUISystem::GetSingleton( ).NotifyRedraw( );
	mWindowFlag &= ~GUI_BF_CLICKED;
	mWindowFlag &= ~GUI_BF_HOVER;
}

void CGUIButton::OnMouseEnter( const CGUIPoint& rPoint, unsigned int vFlag )
{
	CGUIWindow::OnMouseEnter( rPoint, vFlag );
	CGUISystem::GetSingleton( ).NotifyRedraw( );
	if ( vFlag & GUI_LBUTTON )
		mWindowFlag |= GUI_BF_CLICKED;

	mWindowFlag |= GUI_BF_HOVER;
}

void CGUIButton::OnLoadResource( )
{
	GUI_LOADTEXTURE( mpTexture, "Button.png" )
}

void CGUIButton::OnDestroyResource( )
{
	GUI_DESTROYTEXTURE( mpTexture )
}

// ************************************************************ //
// CGUICheck
// ************************************************************ //
void CGUICheck::RenderCheck( CGUICanvas* pCanvas )
{
	CGUIRect tSrcRect;
	if ( IsWindowEnable( ) == false )
		tSrcRect.SetRect( 0, mImageSize.mCY * 6, mImageSize.mCX, mImageSize.mCY * 7 );
	else if ( mWindowFlag & GUI_BF_CLICKED )
	{
		if ( mIsChecked )
			tSrcRect.SetRect( 0, mImageSize.mCY * 5, mImageSize.mCX, mImageSize.mCY * 6 );
		else
			tSrcRect.SetRect( 0, mImageSize.mCY * 4, mImageSize.mCX, mImageSize.mCY * 5 );
	}
	else if ( mWindowFlag & GUI_BF_HOVER )
	{
		if ( mIsChecked )
			tSrcRect.SetRect( 0, mImageSize.mCY * 3, mImageSize.mCX, mImageSize.mCY * 4 );
		else
			tSrcRect.SetRect( 0, mImageSize.mCY, mImageSize.mCX, mImageSize.mCY * 2 );
	}
	else
	{
		if ( mIsChecked )
			tSrcRect.SetRect( 0, mImageSize.mCY * 2, mImageSize.mCX, mImageSize.mCY * 3 );
		else
			tSrcRect.SetRect( 0, 0, mImageSize.mCX, mImageSize.mCY );
	}
    
	CGUIRect tClientRect = GetClientRect( );
	CGUIRect tDestRect( tClientRect.mTopLeft, mImageSize );
	pCanvas->BitBlt( mpTexture, tSrcRect, tDestRect, 0xFFFFFFFF );
}

void CGUICheck::OnWindowRender( )
{
	CGUIWindowCanvas tCanvas( this );
	RenderCheck( &tCanvas );

	CGUIRect tClientRect = GetClientRect( );
	CGUIRect tDestRect( tClientRect.mTopLeft, mImageSize );
	CGUIRect tTextRect( tDestRect.TopRight( ), CGUIPoint( tClientRect.mBottomRight.mX, tDestRect.mBottomRight.mY ) );
	tCanvas.PrintText( mTitle.c_str( ), tTextRect, GUI_DT_LEFT | GUI_DT_VCENTER, mTextColour );
}

void CGUICheck::OnMouseLButtonUp( const CGUIPoint& rPoint, unsigned int vFlag )
{
	CGUIButton::OnMouseLButtonUp( rPoint, vFlag );
	mIsChecked == true ? mIsChecked = false : mIsChecked = true;
}

void CGUICheck::OnLoadResource( )
{
	GUI_LOADTEXTURE( mpTexture, "Check.png" )
}

void CGUICheck::OnDestroyResource( )
{
	GUI_DESTROYTEXTURE( mpTexture )
}

// ************************************************************ //
// CGUIImageButton
// ************************************************************ //
void CGUIImageButton::OnWindowRender( )
{
	if ( mpTexture == NULL )
		return;

	CGUIRect tClientRect = GetClientRect( );
	CGUIWindowCanvas tCanvas( this );
	if ( IsWindowEnable( ) == false )
		tCanvas.BitBlt( mpTexture, CGUIRect( CGUIPoint( 0, mImageSize.mCY * 3 ), mImageSize ), tClientRect, 0xFFFFFFFF );
	else if ( mWindowFlag & GUI_BF_CLICKED )
		tCanvas.BitBlt( mpTexture, CGUIRect( CGUIPoint( 0, mImageSize.mCY * 2 ), mImageSize ), tClientRect, 0xFFFFFFFF );
	else if ( mWindowFlag & GUI_BF_HOVER )
		tCanvas.BitBlt( mpTexture, CGUIRect( CGUIPoint( 0, mImageSize.mCY ), mImageSize ), tClientRect, 0xFFFFFFFF );
	else
		tCanvas.BitBlt( mpTexture, CGUIRect( CGUIPoint( 0, 0 ), mImageSize ), tClientRect, 0xFFFFFFFF );
}

void CGUIImageButton::OnLoadResource( )
{
	GUI_LOADTEXTURE( mpTexture, mResName.c_str( ) )
}

void CGUIImageButton::OnDestroyResource( )
{
	GUI_DESTROYTEXTURE( mpTexture )
}

// ************************************************************ //
// CGUIImageCheck
// ************************************************************ //
void CGUIImageCheck::OnWindowRender( )
{
	if ( mpTexture == NULL )
		return;

	CGUIWindowCanvas tCanvas( this );
	RenderCheck( &tCanvas );
}

void CGUIImageCheck::OnLoadResource( )
{
	GUI_LOADTEXTURE( mpTexture, mResName.c_str( ) )
}

void CGUIImageCheck::OnDestroyResource( )
{
	GUI_DESTROYTEXTURE( mpTexture )
}

// ************************************************************ //
// CGUIScrollBar
// ************************************************************ //
CGUIScrollBar::CGUIScrollBar( const char* pType, const char* pName, unsigned int vStyle, const CGUIRect& rRect, CGUIWindow* pParent, void* pUserData )
	: CGUIWindow( pType, pName, vStyle, rRect, pParent, pUserData ), mRangeMax( 0 ), mRangeMin( 0 ), mLineSize( 5 ), mPageSize( 20 ), mPosition( 0 )
{
	// 滚动条不带有任何窗口风格
	vStyle &= 0xFFFF0000;
}

int CGUIScrollBar::OnWindowCreate( )
{
	if ( CGUIWindow::OnWindowCreate( ) == -1 )
		return -1;

	const char* tpBtnUp		= NULL;
	const char* tpBtnDown	= NULL;
	const char* tpThumb		= NULL;
	CGUIRect tPageUpRect, tPageDownRect, tThumbRect;

	// 建立上下翻页按钮
	CGUIWindowManager* tpWmgr = CGUIWindowManager::GetSingletonPtr( );
	CGUIRect tClientRect = GetClientRect( );
	CGUISize tBtnVertSize = CGUISize( tClientRect.Width( ), GUI_VSCROLLBAR_PAGEBTN_HEIGHT );
	CGUISize tBtnHorzSize = CGUISize( GUI_HSCROLLBAR_PAGEBTN_WIDTH, tClientRect.Height( ) );
	CGUISize tSrcSize;
	int tPageUpArchor, tPageDownArchor = 0;
	if ( mWindowStyle & GUI_SBS_VERT )
	{
		tPageUpRect.SetRect( tClientRect.mTopLeft, tBtnVertSize );
		tPageDownRect.SetRect( tClientRect.mBottomRight - tBtnVertSize, tBtnVertSize );
		tpBtnUp = "VScrollUpBtn.png"; tpBtnDown = "VScrollDownBtn.png";
		tSrcSize = CGUISize( GUI_VSCROLLBAR_WIDTH, GUI_VSCROLLBAR_PAGEBTN_HEIGHT );
		tPageUpArchor	= GUI_TOPARCHOR_TOP | GUI_BOTTOMARCHOR_TOP | GUI_LEFTARCHOR_LEFT | GUI_RIGHTARCHOR_RIGHT;
		tPageDownArchor	= GUI_TOPARCHOR_BOTTOM | GUI_BOTTOMARCHOR_BOTTOM | GUI_LEFTARCHOR_LEFT | GUI_RIGHTARCHOR_RIGHT;
	}

	if ( mWindowStyle & GUI_SBS_HORZ )
	{
		tPageUpRect.SetRect( tClientRect.mTopLeft, tBtnHorzSize );
		tPageDownRect.SetRect( tClientRect.mBottomRight - tBtnHorzSize, tBtnHorzSize ); 
		tpBtnUp = "HScrollUpBtn.png"; tpBtnDown = "HScrollDownBtn.png";
		tSrcSize = CGUISize( GUI_HSCROLLBAR_PAGEBTN_WIDTH, GUI_HSCROLLBAR_HEIGHT );
		tPageUpArchor	= GUI_TOPARCHOR_TOP | GUI_BOTTOMARCHOR_BOTTOM | GUI_LEFTARCHOR_LEFT | GUI_RIGHTARCHOR_LEFT;
		tPageDownArchor	= GUI_TOPARCHOR_TOP | GUI_BOTTOMARCHOR_BOTTOM | GUI_LEFTARCHOR_RIGHT | GUI_RIGHTARCHOR_RIGHT;
	}

	mpPageUp = (CGUIImageButton*) tpWmgr->Create( "GUIImageButton", NULL, 0, tPageUpRect, this, NULL );
	mpPageDown = (CGUIImageButton*) tpWmgr->Create( "GUIImageButton", NULL, 0, tPageDownRect, this, NULL );
	mpPageUp->SetImage( tpBtnUp, tSrcSize );
	mpPageDown->SetImage( tpBtnDown, tSrcSize );
	mpPageUp->SetArchor( tPageUpArchor );
	mpPageDown->SetArchor( tPageDownArchor );

	// 建立滑块按钮
	CGUISize tThumbVertSize = CGUISize( tClientRect.Width( ), GUI_VSCROLLBAR_THUMB_HEIGHT );
	CGUISize tThumbHorzSize = CGUISize( GUI_HSCROLLBAR_THUMB_WIDTH, tClientRect.Height( ) );
	int tThumbArchor;
	if ( mWindowStyle & GUI_SBS_VERT )
	{
        tThumbRect.SetRect( tPageUpRect.BottomLeft( ), tThumbVertSize );
		tSrcSize = CGUISize( GUI_VSCROLLBAR_WIDTH, GUI_VSCROLLBAR_THUMB_HEIGHT );
		tpThumb = "VThumb.png";
		tThumbArchor = GUI_TOPARCHOR_TOP | GUI_BOTTOMARCHOR_TOP | GUI_LEFTARCHOR_LEFT | GUI_RIGHTARCHOR_RIGHT;
	}
	
	if ( mWindowStyle & GUI_SBS_HORZ )
	{
		tThumbRect.SetRect( tPageUpRect.TopRight( ), tThumbHorzSize );
		tSrcSize = CGUISize( GUI_HSCROLLBAR_THUMB_WIDTH, GUI_HSCROLLBAR_HEIGHT );
		tpThumb = "HThumb.png";
		tThumbArchor = GUI_TOPARCHOR_TOP | GUI_BOTTOMARCHOR_BOTTOM | GUI_LEFTARCHOR_LEFT | GUI_RIGHTARCHOR_LEFT;
	}

	mpThumbBtn = (CGUIImageButton*) tpWmgr->Create( "GUIImageButton", NULL, 0, tThumbRect, this, NULL );
	mpThumbBtn->SetImage( tpThumb, tSrcSize );
	mpThumbBtn->SetArchor( tThumbArchor );

	ON_BN_CLICKED( mpPageUp, OnPageUpButtonClick )
	ON_BN_CLICKED( mpPageDown, OnPageDownButtonClick )
	ON_WM_LBUTTONDOWN( mpThumbBtn, OnThumbLButtonDown )
	ON_WM_MOUSEMOVE( mpThumbBtn, OnThumbMouseMove )
	ON_WM_LBUTTONUP( mpThumbBtn, OnThumbLButtonUp )

	return 0;
}

void CGUIScrollBar::OnLoadResource( )
{
	if ( mWindowStyle & GUI_SBS_VERT ) GUI_LOADTEXTURE( mpTexture, "VSlider.png" )
	if ( mWindowStyle & GUI_SBS_HORZ ) GUI_LOADTEXTURE( mpTexture, "HSlider.png" )
}

void CGUIScrollBar::OnDestroyResource( )
{
	if ( mWindowStyle & GUI_SBS_VERT ) GUI_DESTROYTEXTURE( mpTexture )
	if ( mWindowStyle & GUI_SBS_HORZ ) GUI_DESTROYTEXTURE( mpTexture )
}

void CGUIScrollBar::OnKeyDown( unsigned int vVirtualKey, unsigned int vFlag )
{
	CGUIWindow::OnKeyDown( vVirtualKey, vFlag );

	switch( vVirtualKey )
	{
	case GUI_UP:
		SetPosition( mPosition - mLineSize );
		break;
	case GUI_DOWN:
		SetPosition( mPosition + mLineSize );
		break;
	}
}

void CGUIScrollBar::OnMouseLButtonDown( const CGUIPoint& rPoint, unsigned int vFlag )
{
	CGUIRect tClientRect = GetClientRect( );
	CGUIRect tThumbRect = mpThumbBtn->GetRect( );
	CGUIRect tBlankUpRect;
	CGUIRect tBlankDownRect;

	if ( mWindowStyle & GUI_SBS_VERT )
	{
		tBlankUpRect.SetRect( tClientRect.mTopLeft.mX, tClientRect.mTopLeft.mY + GUI_VSCROLLBAR_PAGEBTN_HEIGHT, tThumbRect.TopRight( ).mX, tThumbRect.TopRight( ).mY );
		tBlankDownRect.SetRect( tThumbRect.BottomLeft( ).mX, tThumbRect.BottomLeft( ).mY, tClientRect.BottomRight( ).mX, tClientRect.BottomRight( ).mY - GUI_VSCROLLBAR_PAGEBTN_HEIGHT );
	}

	if ( mWindowStyle & GUI_SBS_HORZ )
	{
		tBlankUpRect.SetRect( tClientRect.TopLeft( ).mX + GUI_HSCROLLBAR_PAGEBTN_WIDTH, tClientRect.TopLeft( ).mY, tThumbRect.BottomLeft( ).mX, tThumbRect.BottomLeft( ).mY );
		tBlankDownRect.SetRect( tThumbRect.TopRight( ).mX, tThumbRect.TopRight( ).mY, tClientRect.BottomRight( ).mX - GUI_HSCROLLBAR_PAGEBTN_WIDTH, tClientRect.BottomRight( ).mY );
	}

	if ( WindowToScreen( tBlankUpRect ).PtInRect( rPoint ) )
		SetPosition( GetPosition( ) - mPageSize );

	if ( WindowToScreen( tBlankDownRect ).PtInRect( rPoint ) )
		SetPosition( GetPosition( ) + mPageSize );
	
	CGUIWindow::OnMouseLButtonDown( rPoint, vFlag );
}

void CGUIScrollBar::OnMouseWheel( const CGUIPoint& rPoint, unsigned int vFlag )
{
	short tDelta = ( vFlag & 0xFFFF0000 ) >> 16;
	tDelta /= 120;
	if ( CGUISystem::GetSingleton( ).GetCapture( ) != mpThumbBtn )
		SetPosition( mPosition - tDelta * mLineSize );
	CGUIWindow::OnMouseWheel( rPoint, vFlag );
}

void CGUIScrollBar::SetPosition( int vPosition )
{
	vPosition = GUI_MAX( 0, vPosition );
	vPosition = GUI_MIN( mRangeMax - mRangeMin, vPosition );
	mPosition = vPosition;

	int tLength = 0;
	int tPosition = 0;
	int tOffset = 0; 
	float tPixelPerUnit = 0.0f;					// 单位逻辑单元有几个像数
	CGUIRect tThumbRect = mpThumbBtn->GetRect( );
	CGUIRect tClientRect = GetClientRect( );
	if ( mWindowStyle & GUI_SBS_VERT )
	{
		tLength = tClientRect.Height( ) - ( GUI_VSCROLLBAR_PAGEBTN_HEIGHT << 1 );
		tPixelPerUnit = ( tLength - GUI_VSCROLLBAR_THUMB_HEIGHT ) / (float)( GUI_MAX( 1, mRangeMax - mRangeMin ) );
		tOffset = int( tPixelPerUnit * vPosition + GUI_VSCROLLBAR_PAGEBTN_HEIGHT ) - tThumbRect.mTopLeft.mY;
		mpThumbBtn->MoveWindow( CGUIPoint( 0, tOffset ) );
	}
	
	if ( mWindowStyle & GUI_SBS_HORZ )
	{
		tLength = tClientRect.Width( ) - int( GUI_HSCROLLBAR_PAGEBTN_WIDTH << 1 );
		tPixelPerUnit = ( tLength - GUI_HSCROLLBAR_THUMB_WIDTH ) / (float)( GUI_MAX( 1, mRangeMax - mRangeMin ) );
		tOffset = int( tPixelPerUnit * vPosition + GUI_HSCROLLBAR_PAGEBTN_WIDTH ) - tThumbRect.mTopLeft.mX;
		mpThumbBtn->MoveWindow( CGUIPoint( tOffset, 0 ) );
	}
	
	CGUISinkThumbChanged tThumbChanged;
	tThumbChanged.AddArg( CGUIDataType( GUI_PWINDOW, (unsigned int) this ) );
	tThumbChanged.AddArg( CGUIDataType( GUI_INT, (unsigned int) mPosition ) );
	FireEvent( &tThumbChanged );
}

int CGUIScrollBar::GetPosition( ) const
{
	return mPosition;
}

void CGUIScrollBar::OnWindowSized( const CGUIPoint& rOffset )
{
	SetPosition( GetPosition( ) );
	CGUIWindow::OnWindowSized( rOffset );
}

void CGUIScrollBar::OnWindowRender( )
{
	CGUIWindowCanvas tCanvas( this );
	CGUIRect tClientRect = GetClientRect( );
	int tStartPos	= 0;
	int tEndPos		= 0;
	if ( mWindowStyle & GUI_SBS_VERT )
	{
		tStartPos	= tClientRect.mTopLeft.mY + GUI_VSCROLLBAR_PAGEBTN_HEIGHT;
		tEndPos		= tClientRect.mBottomRight.mY -	GUI_VSCROLLBAR_PAGEBTN_HEIGHT;
	}
	if ( mWindowStyle & GUI_SBS_HORZ )
	{
		tStartPos	= tClientRect.mTopLeft.mX + GUI_HSCROLLBAR_PAGEBTN_WIDTH;
		tEndPos		= tClientRect.mBottomRight.mX -	GUI_HSCROLLBAR_PAGEBTN_WIDTH;
	}

	CGUIRect tDestRect;
	if ( mWindowStyle & GUI_SBS_VERT )
	{
		for ( int i = tStartPos; i < tEndPos; )
		{
			tDestRect.SetRect( tClientRect.mTopLeft.mX, i, tClientRect.mBottomRight.mX, i + GUI_MIN( GUI_VSCROLLBAR_SLIDER_HEIGHT, tEndPos - i ) );
			tCanvas.BitBlt( mpTexture, CGUIRect( 0, 0, GUI_VSCROLLBAR_WIDTH, tDestRect.Height( ) ), tDestRect, 0xFFFFFFFF );
			i += GUI_MIN( GUI_VSCROLLBAR_SLIDER_HEIGHT, tEndPos - i );
		}
	}
	if ( mWindowStyle & GUI_SBS_HORZ )
	{
		for ( int i = tStartPos; i < tEndPos; )
		{
			tDestRect.SetRect( i, tClientRect.mTopLeft.mY, i + GUI_MIN( GUI_HSCROLLBAR_SLIDER_WIDTH, tEndPos - i ), tClientRect.mBottomRight.mY );
			tCanvas.BitBlt( mpTexture, CGUIRect( 0, 0, tDestRect.Width( ), GUI_HSCROLLBAR_HEIGHT ), tDestRect, 0xFFFFFFFF );
			i += GUI_MIN( GUI_HSCROLLBAR_SLIDER_WIDTH, tEndPos - i );
		}
	}
}

void CGUIScrollBar::OnPageDownButtonClick( CGUIWindow* pButton )
{
	SetPosition( mPosition + mLineSize );
	CGUISystem::GetSingleton( ).SetFocus( this );
}

void CGUIScrollBar::OnPageUpButtonClick( CGUIWindow* pButton )
{
	SetPosition( mPosition - mLineSize );
	CGUISystem::GetSingleton( ).SetFocus( this );
}

void CGUIScrollBar::OnThumbLButtonDown( const CGUIPoint* pPoint, unsigned int vFlag )
{
	CGUISystem::GetSingleton( ).SetCapture( mpThumbBtn );
	mLastLButton = *pPoint;
}

void CGUIScrollBar::OnThumbLButtonUp( const CGUIPoint* pPoint, unsigned int vFlag )
{
	CGUISystem::GetSingleton( ).ReleaseCapture( mpThumbBtn );
	CGUISystem::GetSingleton( ).SetFocus( this );
}

void CGUIScrollBar::OnThumbMouseMove( const CGUIPoint* pPoint, unsigned int vFlag )
{
	if ( vFlag & GUI_LBUTTON && CGUISystem::GetSingleton( ).GetCapture( ) == mpThumbBtn )
	{
		int tOffset = 0;
		CGUIRect tClientRect = GetClientRect( );
		CGUIRect tScrollRect = WindowToScreen( tClientRect );
		CGUIRect tThumbRect = mpThumbBtn->WindowToScreen( mpThumbBtn->GetWindowRect( ) );
		if ( mWindowStyle & GUI_SBS_VERT )
		{
			tOffset = GUI_MAX( tScrollRect.mTopLeft.mY + GUI_VSCROLLBAR_PAGEBTN_HEIGHT - tThumbRect.mTopLeft.mY, (*pPoint).mY - mLastLButton.mY );
			tOffset = GUI_MIN( tScrollRect.mBottomRight.mY - GUI_VSCROLLBAR_PAGEBTN_HEIGHT - tThumbRect.mBottomRight.mY, tOffset );
			mpThumbBtn->MoveWindow( CGUIPoint( 0, tOffset ) );
			mLastLButton += CGUIPoint( 0, tOffset );
		}

		if ( mWindowStyle & GUI_SBS_HORZ )
		{
			tOffset = GUI_MAX( tScrollRect.mTopLeft.mX + GUI_HSCROLLBAR_PAGEBTN_WIDTH - tThumbRect.mTopLeft.mX, (*pPoint).mX - mLastLButton.mX );
			tOffset = GUI_MIN( tScrollRect.mBottomRight.mX - GUI_HSCROLLBAR_PAGEBTN_WIDTH - tThumbRect.mBottomRight.mX, tOffset );
			mpThumbBtn->MoveWindow( CGUIPoint( tOffset, 0 ) );
			mLastLButton += CGUIPoint( tOffset, 0 );
		}

		int tLength = 0;
		int tPosition = 0;
		float tUnitPerPixel = 0.0f;					// 单位逻辑单元有几个像数
		if ( mWindowStyle & GUI_SBS_VERT )
		{
			tLength = tClientRect.Height( ) - ( GUI_VSCROLLBAR_PAGEBTN_HEIGHT << 1 );
			tUnitPerPixel = (float)( mRangeMax - mRangeMin ) / GUI_MAX( 1, tLength - GUI_VSCROLLBAR_THUMB_HEIGHT );
			mPosition = int( ( mpThumbBtn->GetRect( ).mTopLeft.mY - GUI_VSCROLLBAR_PAGEBTN_HEIGHT ) * tUnitPerPixel );
		}
		
		if ( mWindowStyle & GUI_SBS_HORZ )
		{
			tLength = tClientRect.Width( ) - int( GUI_HSCROLLBAR_PAGEBTN_WIDTH << 1 );
			tUnitPerPixel = (float)( mRangeMax - mRangeMin ) / GUI_MAX( 1, tLength - GUI_HSCROLLBAR_THUMB_WIDTH );
			mPosition = int( ( mpThumbBtn->GetRect( ).mTopLeft.mX - GUI_HSCROLLBAR_PAGEBTN_WIDTH ) * tUnitPerPixel );
		}

		CGUISinkThumbChanged tThumbChanged;
		tThumbChanged.AddArg( CGUIDataType( GUI_PWINDOW, (unsigned int) this ) );
		tThumbChanged.AddArg( CGUIDataType( GUI_INT, (unsigned int) mPosition ) );
		FireEvent( &tThumbChanged );
	}
}

// ************************************************************ //
// CGUIEdit
// ************************************************************ //
CGUIEdit::CEditText& CGUIEdit::CEditText::operator = ( const char* pText )
{
	if ( pText[ 0 ] & 0x80 )
	{
		mText[ 0 ] = pText[ 0 ];
		mText[ 1 ] = pText[ 1 ];
	}
	else
		mText[ 0 ] = pText[ 0 ];

	return *this;
}

int CGUIEdit::CEditText::GetTextWidth( CGUIFont* pFont ) const
{
	return pFont->GetTextWidth( mText, GetTextCount( ) );
}

int CGUIEdit::GetTextIndex( const CGUIPoint& rPoint ) const
{
	for ( int i = 0; i < int( mTexts.size( ) ); i ++ )
	{
		CGUIRect tTextRect = mTexts[i].mRect - mCamPoint;
		if ( tTextRect.PtInRect( rPoint ) )
			return i == 0 ? 0 : i + 1;
	}

	if ( mTexts.size( ) > 0 )
	{
		CGUIRect tLastRect = mTexts[ mTexts.size( ) - 1 ].mRect - mCamPoint;
		if ( tLastRect.mBottomRight.mX < rPoint.mX && tLastRect.mBottomRight.mY < rPoint.mY )
			return int( mTexts.size( ) );

		CGUIRect tFirstRect = mTexts[ 0 ].mRect - mCamPoint;
		if ( tFirstRect.mBottomRight.mX > rPoint.mX && tFirstRect.mBottomRight.mY > rPoint.mY )
			return 0;
	}

	std::vector< int > mMatchTexts;
	for ( int i = 0; i < int( mTexts.size( ) ); i ++ )
	{
		CGUIRect tTextRect = mTexts[i].mRect - mCamPoint;
		if ( rPoint.mY >= tTextRect.mTopLeft.mY && rPoint.mY <= tTextRect.mBottomRight.mY )
			mMatchTexts.push_back( i == 0 ? 0 : i + 1 );
	}

	int tMinLength = 0xFFFF;
	int tReturn = -1;
	for ( int i = 0; i < int( mMatchTexts.size( ) ); i ++ )
	{
		if ( mMatchTexts[ i ] == 0 )
		{
			if ( abs( mTexts[ mMatchTexts[ i ] ].mRect.CenterPoint( ).mX - rPoint.mX ) < tMinLength )
			{
				tMinLength = abs( mTexts[ mMatchTexts[ i ] ].mRect.CenterPoint( ).mX - rPoint.mX );
				tReturn = mMatchTexts[ i ];
			}
		}
		else
		{
			if ( abs( mTexts[ mMatchTexts[ i ] - 1 ].mRect.CenterPoint( ).mX - rPoint.mX ) < tMinLength )
			{
				tMinLength = abs( mTexts[ mMatchTexts[ i ] - 1 ].mRect.CenterPoint( ).mX - rPoint.mX );
				tReturn = mMatchTexts[ i ];
			}
		}
	}

	if ( tReturn == -1 )
	{
		mMatchTexts.clear( );
		tMinLength = 0xFFFF;

		for ( int i = 0; i < int( mTexts.size( ) ); i ++ )
		{
			CGUIRect tTextRect = mTexts[i].mRect - mCamPoint;
			if ( rPoint.mX >= tTextRect.mTopLeft.mX && rPoint.mX <= tTextRect.mBottomRight.mX )
				mMatchTexts.push_back( i == 0 ? 0 : i + 1 );
		}

		for ( int i = 0; i < int( mMatchTexts.size( ) ); i ++ )
		{
			if ( mMatchTexts[ i ] == 0 )
			{
				if ( abs( mTexts[ mMatchTexts[ i ] ].mRect.CenterPoint( ).mY - rPoint.mY ) < tMinLength )
				{
					tMinLength = abs( mTexts[ mMatchTexts[ i ] ].mRect.CenterPoint( ).mY - rPoint.mY );
					tReturn = mMatchTexts[ i ];
				}
			}
			else
			{
				if ( abs( mTexts[ mMatchTexts[ i ] - 1 ].mRect.CenterPoint( ).mY - rPoint.mY ) < tMinLength )
				{
					tMinLength = abs( mTexts[ mMatchTexts[ i ] - 1 ].mRect.CenterPoint( ).mY - rPoint.mY );
					tReturn = mMatchTexts[ i ];
				}
			}
		}
	}
	return tReturn;
}

void CGUIEdit::OnMouseLButtonDown( const CGUIPoint& rPoint, unsigned int vFlag )
{
	CGUIWindow::OnMouseLButtonDown( rPoint, vFlag );
	CGUISystem::GetSingleton( ).SetCapture( this );

	mLastTickCount		= 0;
	mTurn				= 0;

	mCursorIndex = GetTextIndex( ScreenToClient( rPoint ) );
	if ( mCursorIndex == -1 )
		mCursorIndex = int( mTexts.size( ) );

	UnSelection( );
}

void CGUIEdit::OnMouseMove( const CGUIPoint& rPoint, unsigned int vFlag )
{
	CGUIWindow::OnMouseMove( rPoint, vFlag );
	if ( mTexts.size( ) == 0 )
		return;

	if ( vFlag & GUI_LBUTTON )
	{
		int tCurIndex = GetTextIndex( ScreenToClient( rPoint ) );
		if ( tCurIndex == -1 ) return;

		if( tCurIndex < mCursorIndex )
			while( mCursorIndex != tCurIndex ) CursorLeft( true );
		else if ( tCurIndex > mCursorIndex )
			while( mCursorIndex != tCurIndex ) CursorRight( true );

		CGUISystem::GetSingleton( ).NotifyRedraw( );
	}
}

void CGUIEdit::OnMouseLButtonUp( const CGUIPoint& rPoint, unsigned int vFlag )
{
	CGUIWindow::OnMouseLButtonUp( rPoint, vFlag );
	CGUISystem::GetSingleton( ).ReleaseCapture( this );
}

void CGUIEdit::SelectionLeft( int vOffset, int vCursorIndex )
{
	// 如果没有偏移，就返回
	if ( vOffset == 0 ) return;

	if ( mSelectionBegin == -1 && mSelectionEnd == -1 )
	{
		mSelectionBegin = vCursorIndex + vOffset;
		mSelectionEnd = vCursorIndex + vOffset;
	}

	int tOldCursorIndex = vCursorIndex + vOffset;

	if ( vCursorIndex >= mSelectionBegin )
		mSelectionEnd = vCursorIndex;
	else
	{
		if ( vCursorIndex == 0 && tOldCursorIndex > mSelectionBegin )
		{
			mSelectionEnd	= mSelectionBegin;
			mSelectionBegin = 0;
		}
		else
			mSelectionBegin = vCursorIndex;
	}
}

void CGUIEdit::SelectionRight( int vOffset, int vCursorIndex )
{
	// 如果没有偏移，就返回
	if ( vOffset == 0 )
		return;

	if ( mSelectionBegin == -1 && mSelectionEnd == -1 )
	{
		mSelectionBegin = vCursorIndex - vOffset;
		mSelectionEnd = vCursorIndex - vOffset;
	}

	int tOldCursorIndex = vCursorIndex - vOffset;
	if ( vCursorIndex > mSelectionEnd )
	{
		if ( vCursorIndex == mTexts.size( ) && tOldCursorIndex < mSelectionEnd )
		{
			mSelectionBegin = mSelectionEnd;
			mSelectionEnd	= vCursorIndex;
		}
		else
            mSelectionEnd = vCursorIndex;
	}
	else
		mSelectionBegin = vCursorIndex;
}

void CGUIEdit::UnSelection( )
{
	mSelectionBegin = -1;
	mSelectionEnd	= -1;
}

void CGUIEdit::DeleteSelection( )
{
	for ( int i = mSelectionBegin; i < mSelectionEnd; i ++ )
		mTexts.erase( mTexts.begin( ) + mSelectionBegin );

	if ( mSelectionBegin != mSelectionEnd )
	{
		mCursorIndex = mSelectionBegin;
		if ( mpVScrollBar != NULL ) mpVScrollBar->SetPosition( 0 );
		if ( mpHScrollBar != NULL ) mpHScrollBar->SetPosition( 0 );
	}
}

void CGUIEdit::CursorUp( int vShiftDown )
{
	if ( mCursorIndex == 0 ) return;

	CGUIClientCanvas tCanvas( this );
	std::vector< CGUIRect > tRects;
	std::vector< int > tIndexs;
	CGUIRect tHitRect = GetCursorRect( ) - CGUIPoint( 0, tCanvas.GetFontHeight( ) );

	for ( int i = 1; i < mCursorIndex; i ++ )
	{
		if ( mTexts[i - 1].GetCursorRect( ).CenterPoint( ).mY == tHitRect.CenterPoint( ).mY )
		{
			tRects.push_back( mTexts[i - 1].GetCursorRect( ) );
			tIndexs.push_back( i );
		}
	}
	
	int tMinLength = 0x0FFFFFFF;
	int tCursorIndex = 0;
	for ( int i = 0; i < int( tRects.size( ) ); i ++ )
	{
		if ( abs( tRects[i].CenterPoint( ).mX - tHitRect.CenterPoint( ).mX ) < tMinLength )
		{
			tCursorIndex = tIndexs[i];
			tMinLength = abs( tRects[i].CenterPoint( ).mX - tHitRect.CenterPoint( ).mX );
		}
	}

	if ( tHitRect.Width( ) == 2 && tCursorIndex == 1 ) tCursorIndex = 0;

	while( mCursorIndex > tCursorIndex ) CursorLeft( vShiftDown );
}

void CGUIEdit::CursorDown( int vShiftDown )
{
	std::vector< CGUIRect > tRects;
	std::vector< int >		tIndexs;
	CGUIClientCanvas tCanvas( this );
	CGUIRect tHitRect = GetCursorRect( ) + CGUIPoint( 0, tCanvas.GetFontHeight( ) );
	
	for ( int i = int( mTexts.size( ) ); i > mCursorIndex; i -- )
	{
		if ( mTexts[i - 1].GetCursorRect( ).CenterPoint( ).mY == tHitRect.CenterPoint( ).mY )
		{
			tRects.push_back( mTexts[i - 1].GetCursorRect( ) );
			tIndexs.push_back( i );
		}
	}
	
	int tMinLength = 0x0FFFFFFF;
	int tCursorIndex = int( mTexts.size( ) );
	for ( int i = 0; i < int( tRects.size( ) ); i ++ )
	{
		if ( abs( tRects[i].CenterPoint( ).mX - tHitRect.CenterPoint( ).mX ) < tMinLength )
		{
			tCursorIndex = tIndexs[i];
			tMinLength = abs( tRects[i].CenterPoint( ).mX - tHitRect.CenterPoint( ).mX );
		}
	}

	while( mCursorIndex < tCursorIndex ) CursorRight( vShiftDown );
}

void CGUIEdit::CursorRight( int vShiftDown )
{
	if ( mCursorIndex < int( mTexts.size( ) ) )
	{
		mCursorIndex += 1;
		if ( vShiftDown ) SelectionRight( 1, mCursorIndex );					
	}

	if ( vShiftDown == false ) UnSelection( );
}

void CGUIEdit::CursorLeft( int vShiftDown )
{
	if ( mCursorIndex > 0 )
	{
		mCursorIndex -= 1;
		if ( vShiftDown ) SelectionLeft( 1, mCursorIndex );
	}

	if ( vShiftDown == false ) UnSelection( );
}

void CGUIEdit::VisibleCursor( )
{
	CGUIRect tClientRect = GetClientRect( );
	CGUIRect tCursorRect = GetCursorRect( ) - mCamPoint;
	int tOffsetY = tCursorRect.mBottomRight.mY - tClientRect.Height( );
	if ( tOffsetY > 0 && mWindowStyle & GUI_WS_VSCROLL ) mpVScrollBar->SetPosition( mpVScrollBar->GetPosition( ) + tOffsetY );
	if ( tOffsetY > 0 && !( mWindowStyle & GUI_WS_VSCROLL ) ) mCamPoint.mY += tOffsetY;

	tOffsetY = tCursorRect.mTopLeft.mY;
	if ( tOffsetY < 0 && mWindowStyle & GUI_WS_VSCROLL ) mpVScrollBar->SetPosition( mpVScrollBar->GetPosition( ) + tOffsetY );
	if ( tOffsetY < 0 && !( mWindowStyle & GUI_WS_VSCROLL ) ) mCamPoint.mY += tOffsetY;

	int tOffsetX = tCursorRect.mBottomRight.mX - tClientRect.Width( );
	if ( tOffsetX > 0 && mWindowStyle & GUI_WS_HSCROLL ) mpHScrollBar->SetPosition( mpHScrollBar->GetPosition( ) + tOffsetX );
	if ( tOffsetX > 0 && !( mWindowStyle & GUI_WS_HSCROLL ) ) mCamPoint.mX += tOffsetX;

	tOffsetX = tCursorRect.mTopLeft.mX;
	if ( tOffsetX < 0 && mWindowStyle & GUI_WS_HSCROLL ) mpHScrollBar->SetPosition( mpHScrollBar->GetPosition( ) + tOffsetX );
	if ( tOffsetX < 0 && !( mWindowStyle & GUI_WS_HSCROLL ) ) mCamPoint.mX += tOffsetX;
}

void CGUIEdit::CalcTextRect( )
{
	int	tLeft = 0, tTop = 0, tLogicWidth = 0, tLogicHeight = 0;
	int tFontHeight = mpFont->GetFontHeight( );
	CGUIRect tClientRect = GetClientRect( );
	for ( int i = 0; i < int( mTexts.size( ) ); i ++ )
	{
		bool tReturn = false;
		int tWidth = ( mWindowStyle & GUI_ES_PASSWORD ? mpFont->GetTextWidth( "*", 1 ) : mTexts[ i ].GetTextWidth( mpFont ) );
		if ( tLeft + tWidth > tClientRect.Width( ) && mWindowStyle & GUI_ES_MUTILINE && !( mWindowStyle & GUI_ES_AUTOHSCROLL ) )
		{
			tLeft	 = 0;
			tTop	+= tFontHeight;
		}
		else if ( mTexts[ i ].mText[ 0 ] == '\r' && mTexts[ i ].mText[ 1 ] == '\n' && mWindowStyle & GUI_ES_MUTILINE )
		{
			tWidth	= 2;
			tLeft	= 0;
			tTop	+= tFontHeight;
			tReturn = true;
		}

		mTexts[ i ].mRect.SetRect( tLeft, tTop, tLeft + tWidth, tTop + tFontHeight );
		tLeft		+= ( tReturn ? 0 : tWidth );
		tLogicWidth  = GUI_MAX( tLeft, tLogicWidth );
		tLogicHeight = tTop + tFontHeight;
	}

	if ( UpdateScrollBar( tLogicWidth, tLogicHeight ) )
		CalcTextRect( );
	VisibleCursor( );
}

void CGUIEdit::OnChar( unsigned int vCharCode, unsigned int vFlag )
{
	CGUIPlatform* tpPlatform = CGUISystem::GetSingletonPtr( )->GetPlatform( );
	if ( tpPlatform->GetKeyState( GUI_CONTROL ) & 0x80 ) return;
	if ( mWindowStyle & GUI_ES_READONLY || vCharCode == GUI_ESCAPE || vCharCode == GUI_TAB ) return;

	mLastTickCount	= 0;
	mTurn			= 0;
	bool tNeedFire	= false;
	if ( vCharCode == GUI_BACK )
	{
		if ( mSelectionBegin == mSelectionEnd )
		{
			// 退格键，删掉前一个字符				  
			if ( mCursorIndex > 0 )
			{
				mTexts.erase( mTexts.begin( ) + ( -- mCursorIndex ) );
				tNeedFire = true;
			}
		}
		else
		{
			tNeedFire = true;
			DeleteSelection( );
            UnSelection( );
		}
	}
	else				 
	{
		tNeedFire = true;
		DeleteSelection( );
		UnSelection( );
		if ( InsertChar( vCharCode ) && vCharCode == GUI_RETURN && mWindowStyle & GUI_WS_HSCROLL ) 
			mpHScrollBar->SetPosition( 0 );
	}

	if ( tNeedFire == true )
	{
		CGUISinkEditChanged tEditChanged;
		tEditChanged.AddArg( CGUIDataType( GUI_PWINDOW, (unsigned int) this ) );
		FireEvent( &tEditChanged );
	}

	CalcTextRect( );
}

bool CGUIEdit::InsertChar( char vCharCode )
{
	if ( vCharCode & 0x80 && mInputChinese == false )
	{
		mCurrentInput.mText[ 0 ] = vCharCode;
		mInputChinese = true;
		return true;
	}
	else if ( mInputChinese == true )
	{
		mCurrentInput.mText[ 1 ] = vCharCode;
		mCurrentInput.mText[ 2 ] = '\0';
		mInputChinese = false;
	}
	else if ( vCharCode == '\n' )
		return true;
	else if ( vCharCode == GUI_RETURN && mWindowStyle & GUI_ES_SINGLELINE ) 	// 单行输入框忽略回车
		return false;
	else if ( vCharCode == GUI_RETURN )
	{
        mCurrentInput.mText[ 0 ] = vCharCode;
		mCurrentInput.mText[ 1 ] = '\n';
	}
	else
	{
		mCurrentInput.mText[ 0 ] = vCharCode;
		mCurrentInput.mText[ 1 ] = '\0';
	}

	CGUIRect tClientRect = GetClientRect( );
	int tCount = int( mTexts.size( ) );
	int tFontHeight = mpFont->GetFontHeight( );
	if ( tCount > 0 )
	{
		CGUIRect tLastRect = mTexts[tCount - 1].mRect - mCamPoint;
		// 如果是多行输入框，而且没有设置自动纵向滚动，满屏后忽略回车
		if ( mWindowStyle & GUI_ES_MUTILINE && !( mWindowStyle & GUI_ES_AUTOVSCROLL ) )
			if ( tLastRect.mBottomRight.mY + tFontHeight >= tClientRect.Height( ) )
				if ( vCharCode == GUI_RETURN ) 
					return false;

		// 如果是单行，只要是没有设置自动横向滚动，宽度达到满屏后忽略回车
		if ( ( !( mWindowStyle & GUI_ES_AUTOHSCROLL ) ) && mWindowStyle & GUI_ES_SINGLELINE )
			if ( tLastRect.mBottomRight.mX + mCurrentInput.GetTextWidth( mpFont ) >= tClientRect.Width( ) )
				return false;

		// 如果是多行，只要是没有设置自动横向滚动，宽度和高度达到满屏后忽略回车
		if ( ( !( mWindowStyle & GUI_ES_AUTOHSCROLL ) ) && mWindowStyle & GUI_ES_MUTILINE )
			if ( tLastRect.mBottomRight.mX + mCurrentInput.GetTextWidth( mpFont ) >= tClientRect.Width( ) )
				if ( tLastRect.mBottomRight.mY + tFontHeight >= tClientRect.Height( ) )
                    return false;
	}
	
	mTexts.insert( mTexts.begin( ) + ( mCursorIndex ++ ), mCurrentInput );
	CGUISystem::GetSingleton( ).NotifyRedraw( );
	return true;
}

void CGUIEdit::ResetContext( )
{
	mTexts.clear( );
	mCursorIndex		= 0;
	mSelectionBegin		= -1;
	mSelectionEnd		= -1;
	mLastTickCount		= 0;
	mTurn				= 0;
	mInputChinese		= false;
}

// 设置输入框中的字符串。
void CGUIEdit::SetEditText( const char* pText )
{
	ResetContext( );
	for ( int i = 0; pText[ i ] != '\0'; i ++ ) 
		InsertChar( pText[ i ] );

	mCursorIndex = int( mTexts.size( ) );
	CalcTextRect( );
}

// 得到输入框中的字符串。
std::string CGUIEdit::GetEditText( )
{
	std::string tText;
	//for ( int i = 0; i < 20000; i ++ )
	//	tText.push_back( 'a' );
	for ( int i = 0; i < int( mTexts.size( ) ); i ++ )
	{
		for ( int j = 0; j < mTexts[ i ].GetTextCount( ); j ++ )
			tText.push_back( mTexts[ i ].mText[ j ] );
	}

	return tText;
}

void CGUIEdit::SetSel( int vStartChar, int vEndChar )
{
	vStartChar == -1 ? mSelectionBegin = 0 : mSelectionBegin = vStartChar;
	vEndChar == - 1 ? mSelectionEnd = int( mTexts.size( ) ) : mSelectionEnd = vEndChar;
}

void CGUIEdit::Clear( )
{
	DeleteSelection( );
}

void CGUIEdit::Copy( )
{
	//得到文字
	std::string	tSelectText;
	for ( int i = mSelectionBegin; i < mSelectionEnd; i ++ )
	{
		for ( int j = 0; j < mTexts[ i ].GetTextCount( ); j ++ )
			tSelectText.push_back( mTexts[ i ].mText[ j ] );
	}
	CGUISystem::GetSingleton( ).GetPlatform( )->ClipboardCopy( tSelectText.c_str( ), tSelectText.size( ) );
}

void CGUIEdit::Paste( )
{
	char* pBuffer = NULL;
	int tSize = 0;
	CGUISystem::GetSingleton( ).GetPlatform( )->ClipboardPaste( pBuffer, tSize );
	pBuffer = new char[ tSize ];
	CGUISystem::GetSingleton( ).GetPlatform( )->ClipboardPaste( pBuffer, tSize );
	DeleteSelection( );
	UnSelection( );

	// 加入字符
	for ( int i = 0; i < tSize - 1; i ++ )
	{
		if ( InsertChar( pBuffer[ i ] ) == false )
			break;
	}

	delete [ ] pBuffer;
	CalcTextRect( );
}

void CGUIEdit::Cut( )
{
	//得到文字
	Copy( );

	DeleteSelection( );
	UnSelection( );
	CalcTextRect( );
}

void CGUIEdit::LineScroll( int vLine )
{
	mpVScrollBar->SetPosition( vLine * CGUIClientCanvas( this ).GetFontHeight( ) );
}

CGUIRect CGUIEdit::GetCursorRect( ) const 
{
	if ( mCursorIndex == 0 )
		return CGUIRect( CGUIPoint( 0, 0 ), CGUISize( 2, mpFont->GetFontHeight( ) ) );
	else
		return mTexts[ mCursorIndex - 1 ].GetCursorRect( );
	
	return CGUIRect( );
}

void CGUIEdit::OnKeyDown( unsigned int vVirtualKey, unsigned int vFlag )
{
	if ( mWindowStyle & GUI_ES_READONLY )
		return;

	CGUIPlatform* tpPlatform = CGUISystem::GetSingleton( ).GetPlatform( );
	int tShiftDown		= tpPlatform->GetKeyState( GUI_SHIFT ) & 0x80;
	int tControlDown	= tpPlatform->GetKeyState( GUI_CONTROL ) & 0x80;
	mLastTickCount		= 0;
	mTurn				= 0;
	bool tNeedFire		= false;
	switch ( vVirtualKey )
	{
		case 'X':
		{
			if ( tControlDown ) 
			{
				Cut( );
				tNeedFire = true;
			}
			break;
		}
		case 'C':
		{
			if ( tControlDown )
			{
				Copy( );
				tNeedFire = true;
			}
			break;
		}
		case 'V':
		{
			if ( tControlDown )
			{
				Paste( );
				tNeedFire = true;
			}
			break;
		}
		case GUI_LEFT:		// 光标左移，左移需要特殊处理
		{
			CursorLeft( tShiftDown );
			VisibleCursor( );
			break;
		}
		case GUI_UP:
		{
			CursorUp( tShiftDown );
			VisibleCursor( );
			break;
		}
		case GUI_DOWN:
		{
			CursorDown( tShiftDown );
			VisibleCursor( );
			break;
		}
		case GUI_RIGHT:		// 光标右移
		{
			CursorRight( tShiftDown );
			VisibleCursor( );
			break;
		}
		case GUI_HOME:		// 光标移到开始
		{
			int tOldCursorIndex = mCursorIndex;
			mCursorIndex = 0;
			if ( tShiftDown )
				SelectionLeft( tOldCursorIndex - mCursorIndex, mCursorIndex );
			else
				UnSelection( );
			
			VisibleCursor( );
			break;
		}
		case GUI_END:		// 光标移到最后
		{
			int tOldCursorIndex = mCursorIndex;
			mCursorIndex = int( mTexts.size( ) );
			if ( tShiftDown )
				SelectionRight( mCursorIndex - tOldCursorIndex, mCursorIndex );
			else
				UnSelection( );

			VisibleCursor( );
			break;
		}
		case GUI_DELETE:		// 删除光标所指字符
		{
			if ( mSelectionBegin == mSelectionEnd && mTexts.size( ) != mCursorIndex )
			{
				tNeedFire = true;
				mTexts.erase( mTexts.begin( ) + mCursorIndex );
			}
			else if ( mSelectionBegin != mSelectionEnd )
			{
				DeleteSelection( );
				UnSelection( );
				tNeedFire = true;
			}
			CalcTextRect( );
			//fire event
			break;
		}
	}

	if ( tNeedFire == true )
	{
		CGUISinkEditChanged tEditChanged;
		tEditChanged.AddArg( CGUIDataType( GUI_PWINDOW, (unsigned int) this ) );
		FireEvent( &tEditChanged );
	}

	CGUISystem::GetSingleton( ).NotifyRedraw( );
	CGUIWindow::OnKeyDown( vVirtualKey, vFlag );
}

CGUIPoint CGUIEdit::OnWindowSizing( const CGUIPoint& rOffset ) 
{
	CGUIWindow::OnWindowSizing( rOffset );

	CGUIRect tRect = mRect;
	tRect.mBottomRight += rOffset;
	CGUIPoint tOffset = rOffset;
	if ( tRect.Width( ) < 50 ) tOffset.mX = 50 - mRect.Width( );
	if ( tRect.Height( ) < 25 ) tOffset.mY = 25 - mRect.Height( );
	return tOffset;
}

void CGUIEdit::OnWindowSized( const CGUIPoint& rOffset )
{
    CalcTextRect( );
	CGUIWindow::OnWindowSized( rOffset );
}

void CGUIEdit::OnLoadResource( )
{
	GUI_LOADTEXTURE( mpTexture, "Edit.png" )
}

void CGUIEdit::OnDestroyResource( )
{
	GUI_DESTROYTEXTURE( mpTexture )
}

void CGUIEdit::OnWindowUpdate( )
{
	CGUIPlatform* tpPlatform = CGUISystem::GetSingletonPtr( )->GetPlatform( );
	// 渲染鼠标光标
	unsigned int tThisTickCount = tpPlatform->GetTick( );
	if ( IsWindowActive( ) == true && tThisTickCount - mLastTickCount > 500 )
	{
		mTurn ++;
		mLastTickCount = tThisTickCount;
		CGUISystem::GetSingleton( ).NotifyRedraw( );
	}
}

void CGUIEdit::OnWindowRender( )
{
	CGUIWindowCanvas tCanvas( this );
	tCanvas.DrawCombine( mpTexture, GetWindowRect( ), CGUIPoint( 0, 0 ), CGUISize( 15, 15 ) );

	CGUIClientCanvas tClientCanvas( this );
	// 渲染文本
	for ( size_t i = 0; i < mTexts.size( ); i ++ )
	{
		CEditText* tpTextObj = &mTexts[i];
		CGUIRect tCharRect = tpTextObj->mRect - mCamPoint;
		// 如果是换行和回车，不做渲染
		if ( tpTextObj->mText[ 0 ] == GUI_RETURN && tpTextObj->mText[ 1 ] == '\n' )
			continue;

		if ( i >= size_t( mSelectionBegin ) && i < size_t( mSelectionEnd ) )
			tClientCanvas.FillRect( tCharRect, 0xAF808080 );

		// 如果是Tab，不渲染该字符。但要渲染被选择的时候
		if ( tpTextObj->mText[ 0 ] == GUI_TAB )
			continue;

		if ( mWindowStyle & GUI_ES_PASSWORD )
			tClientCanvas.PrintText( "*", tCharRect, GUI_DT_HCENTER, mTextColour );
		else
			tClientCanvas.PrintText( tpTextObj->mText, tCharRect, GUI_DT_BOTTOM, mTextColour );
	}

	if ( mTurn % 2 == 1 && CGUISystem::GetSingleton( ).GetFocus( ) == this )
	{
		CGUIRect tCursorRect = GetCursorRect( );
		tClientCanvas.FillRect( tCursorRect - mCamPoint, 0xFFFFFFFF );
	}
}

// ************************************************************ //
// CGUIListBox
// ************************************************************ //
void CGUIListBox::RemoveAllItems( )
{
	while ( !mItems.empty( ) )
	{
		delete mItems.at( 0 );
		mItems.erase( mItems.begin( ) );
	}
}

void CGUIListBox::RemoveItem( int vItemIndex )
{
	delete mItems.at( vItemIndex );
	mItems.erase( mItems.begin( ) + vItemIndex );
	RefreshItems( );
}

int CGUIListBox::GetCurSel( ) const
{
	for ( int i = 0; i < int( mItems.size( ) ); i ++ )
	{
		if ( mItems[i]->mIsSelected )
			return i;
	}

	return -1;
}

void CGUIListBox::SetCurSel( int vItemIndex )
{
	if ( vItemIndex < 0 || vItemIndex >= int( mItems.size( ) ) ) return;

	for ( int i = 0; i < int( mItems.size( ) ); i ++ )
		vItemIndex != -1 ? mItems[ i ]->mIsSelected = false : NULL;

	vItemIndex != -1 ? mItems[ vItemIndex ]->mIsSelected = true : NULL;

	CGUISinkListBoxSelChanged tListBoxSelChanged;
	tListBoxSelChanged.AddArg( CGUIDataType( GUI_PWINDOW, (unsigned int) this ) );
	tListBoxSelChanged.AddArg( CGUIDataType( GUI_INT, GetCurSel( ) ) );
	FireEvent( &tListBoxSelChanged );
}

int	CGUIListBox::HitTest( const CGUIPoint& rPoint ) const
{
	CGUIPoint tPoint = ScreenToClient( rPoint ) + mCamPoint;
	int tUpBorder = 0, tDownBorder = 0;
	int tTextHeight = CGUIWindowCanvas( NULL ).GetFontHeight( );
	for ( int i = 0; i < int( mItems.size( ) ); i ++ )
	{
		tUpBorder = mItems[ i ]->mPoint.mY;
		tDownBorder = mItems[ i ]->mPoint.mY + tTextHeight;

		if ( tPoint.mY > tUpBorder && tPoint.mY < tDownBorder )
			return i;
	}

	return -1;
}

void CGUIListBox::RefreshItems( )
{
	int tHeight = 0, tWidth	= 0;
	CGUIClientCanvas tCanvas( this );
	int tTextHeight = tCanvas.GetFontHeight( );
	for ( int i = 0; i < int( mItems.size( ) ); i ++ )
	{
		CListItem* tpItem = mItems[ i ];
		tpItem->mPoint.mY = i * tTextHeight;
		tWidth = GUI_MAX( tWidth, tCanvas.GetTextWidth( tpItem->mText.c_str( ), int( ::strlen( tpItem->mText.c_str( ) ) ) ) + 4 );
	}
	
	tHeight = int( mItems.size( ) ) * tTextHeight;
	UpdateScrollBar( tWidth, tHeight );
}

int CGUIListBox::AppendItem( const char* pText )
{
	mItems.push_back( new CListItem( pText ) );
	RefreshItems( );
	return int( mItems.size( ) ) - 1;
}

void CGUIListBox::OnKeyDown( unsigned int vVirtualKey, unsigned int vFlag )
{
	int tSelIndex = GetCurSel( );
	switch ( vVirtualKey )
	{
		case GUI_UP:
		{
			SetCurSel( tSelIndex - 1 );
			CGUISystem::GetSingleton( ).NotifyRedraw( );
			break;
		}
		case GUI_DOWN:
		{
			SetCurSel( tSelIndex + 1 );
			CGUISystem::GetSingleton( ).NotifyRedraw( );
			break;
		}
	}

	tSelIndex = GetCurSel( );
	if ( tSelIndex != -1 )
	{
		int tTextHeight = mpFont->GetFontHeight( );
		int tOffsetY = ( mItems[ tSelIndex ]->mPoint - mCamPoint ).mY;
		if ( tOffsetY < 0 ) 
			mpVScrollBar->SetPosition( mpVScrollBar->GetPosition( ) + tOffsetY );
		
		tOffsetY = ( mItems[ tSelIndex ]->mPoint - mCamPoint ).mY + tTextHeight - GetClientRect( ).Height( );
		if ( tOffsetY > 0 ) 
			mpVScrollBar->SetPosition( mpVScrollBar->GetPosition( ) + tOffsetY );
	}

	CGUIWindow::OnKeyDown( vVirtualKey, vFlag );
}

void CGUIListBox::OnMouseLeave( const CGUIPoint& rPoint, unsigned int vFlag )
{
	for ( int i = 0; i < int( mItems.size( ) ); i ++ )
		mItems[ i ]->mIsHover = false;

	CGUISystem::GetSingleton( ).NotifyRedraw( );
	CGUIWindow::OnMouseLeave( rPoint, vFlag );
}

void CGUIListBox::OnMouseMove( const CGUIPoint& rPoint, unsigned int vFlags )
{
	if ( GetClientRect( ).PtInRect( ScreenToWindow( rPoint ) ) )
	{
		CGUISystem::GetSingleton( ).NotifyRedraw( );
		int tItemIndex = HitTest( rPoint );
		for ( int i = 0; i < int( mItems.size( ) ); i ++ )
			tItemIndex != -1 ? mItems[ i ]->mIsHover = false : NULL;

		tItemIndex != -1 ? mItems[ tItemIndex ]->mIsHover = true : NULL;
	}

	CGUIWindow::OnMouseMove( rPoint, vFlags );
}

void CGUIListBox::OnMouseLButtonUp( const CGUIPoint& rPoint, unsigned int vFlags )
{
	CGUIWindow::OnMouseLButtonUp( rPoint, vFlags );
}

void CGUIListBox::OnMouseLButtonDown( const CGUIPoint& rPoint, unsigned int vFlags )
{
	if ( GetClientRect( ).PtInRect( ScreenToWindow( rPoint ) ) )
	{
		CGUISystem::GetSingletonPtr( )->NotifyRedraw( );
		SetCurSel( HitTest( rPoint ) );
	}

	CGUIWindow::OnMouseLButtonDown( rPoint, vFlags );
}

void CGUIListBox::OnMouseLButtonDblClk( const CGUIPoint& rPoint, unsigned int vFlags )
{
	if ( GetClientRect( ).PtInRect( ScreenToWindow( rPoint ) ) )
	{
		CGUISystem::GetSingleton( ).NotifyRedraw( );
		SetCurSel( HitTest( rPoint ) );
	}

	CGUIWindow::OnMouseLButtonDblClk( rPoint, vFlags );
}

void CGUIListBox::OnLoadResource( )
{
	GUI_LOADTEXTURE( mpTexture, "ListBox.png" )
}

void CGUIListBox::OnDestroyResource( )
{
	GUI_DESTROYTEXTURE( mpTexture )
}

void CGUIListBox::OnWindowSized( const CGUIPoint& rOffset )
{
	CGUIWindow::OnWindowSized( rOffset );
	RefreshItems( );
}

void CGUIListBox::OnWindowRender( )
{
	CGUIRect tClientRect = GetClientRect( );
	CGUIClientCanvas tCanvas( this );
	CGUIRect tItemRect;
	for ( int i = 0; i < int( mItems.size( ) ); i ++ )
	{
		tItemRect.mTopLeft.mX		= 0;
		tItemRect.mBottomRight.mX	= tClientRect.Width( );
		tItemRect.mTopLeft.mY		= mItems[ i ]->mPoint.mY;
		tItemRect.mBottomRight.mY	= mItems[ i ]->mPoint.mY + tCanvas.GetFontHeight( );

		OnDrawItem( &tCanvas, mItems[ i ], tItemRect - mCamPoint );
	}
}

void CGUIListBox::OnDrawItem( CGUICanvas* pCanvas, const CListItem* pItem, const CGUIRect& rItemRect )
{
	CGUIRect tSelRect( 0, rItemRect.mTopLeft.mY, GetClientRect( ).Width( ), rItemRect.mBottomRight.mY );
	if ( pItem->mIsSelected == true )
		pCanvas->FillRect( rItemRect, mSelColour );

	if ( mWindowStyle & GUI_LBS_MOUSEOVER && pItem->mIsHover == true )
		pCanvas->FillRect( rItemRect, mSelColour );

	pCanvas->PrintText( pItem->mText.c_str( ), rItemRect + CGUIPoint( 2, 0 ), GUI_DT_LEFT | GUI_DT_VCENTER, mTextColour );
}

// ************************************************************ //
// CGUIMenu
// ************************************************************ //
CGUIMenu::CMenuItem::~CMenuItem( )
{
	while ( !mItems.empty( ) )
	{
		delete mItems.at( 0 );
		mItems.erase( mItems.begin( ) );
	}
}

void CGUIMenu::CMenuItem::UnExpand( )
{
	mpParent->mpExpand	= NULL;
	mIsExpanded			= false;
	if ( mpExpand != NULL && mStyle == GUI_MS_POPUP ) mpExpand->UnExpand( );
}

bool CGUIMenu::CMenuItem::Expand( )
{
	bool tNeedRecalc = false;
	if ( mpParent != NULL )
	{
		if ( mpParent->mpExpand != NULL && mpParent->mpExpand != this )
		{
			mpParent->mpExpand->UnExpand( );
			tNeedRecalc	= true;
		}
			
		if ( mpParent->mpExpand != this && mStyle == GUI_MS_POPUP )
		{
			mIsExpanded			= true;
			mpParent->mpExpand	= this;
			tNeedRecalc = true;
		}
	}

	return tNeedRecalc;
}

void CGUIMenu::CMenuItem::DrawItems( CGUICanvas* pCanvas, const CGUIMenu* pMenu ) const
{
	// 如果没有父级菜单，该菜单就是根菜单，那么不能渲染该菜单项
	if ( mpParent != NULL )
		pMenu->OnDrawItem( pCanvas, this );

	if ( mStyle == GUI_MS_POPUP && mIsExpanded == true )
	{
		// 渲染子菜单的存底
		pMenu->OnDrawMenu( pCanvas, mPopupRect );
		// 渲染所有的子菜单
		for ( int i = 0; i < int( mItems.size( ) ); i ++ )
			mItems[i]->DrawItems( pCanvas, pMenu );
	}
}

CGUIMenu::CMenuItem* CGUIMenu::CMenuItem::HitTest( const CGUIPoint& rPoint ) const
{
	if ( mIsExpanded == false ) return NULL;

	CMenuItem* tpItem = NULL;
	for ( int i = 0; i < int( mItems.size( ) ); i ++ )
	{
		if ( mItems[ i ]->mRect.PtInRect( rPoint ) )	tpItem = mItems[ i ];
		else if ( mItems[ i ]->mStyle == GUI_MS_POPUP )	tpItem = mItems[ i ]->HitTest( rPoint );
		if ( tpItem != NULL )
			break;
	}

	return tpItem;
}

CGUIRect CGUIMenu::CMenuItem::GetMenuBox( ) const
{
	CGUIRect tRect;
	if ( mStyle == GUI_MS_POPUP && mIsExpanded == true )
	{
		tRect |= mPopupRect;
		for ( int i = 0; i < int( mItems.size( ) ); i ++ )
			tRect |= mItems[i]->GetMenuBox( );
	}

	return tRect;
}

void CGUIMenu::OnLoadResource( )
{
	GUI_LOADTEXTURE( mpTexture, "Menu.png" )
}

void CGUIMenu::OnDestroyResource( )
{
	GUI_DESTROYTEXTURE( mpTexture )
}

void CGUIMenu::OnDrawItem( CGUICanvas* pCanvas, const CMenuItem* pItem ) const
{
	if ( pItem->mStyle == GUI_MS_SEPARATOR )
	{
		CGUIRect tRect = pItem->mRect;
		tRect.mTopLeft.mY = pItem->mRect.mTopLeft.mY + ( pItem->mRect.Height( ) >> 1 ) - 1;
		tRect.mBottomRight.mY = pItem->mRect.mTopLeft.mY + ( pItem->mRect.Height( ) >> 1 ) + 1;

		pCanvas->DrawRect( tRect, GUI_MAKECOLOUR( 255, 30, 45, 46 ), GUI_MAKECOLOUR( 255, 130, 145, 146 ) );
		return;
	}

	if ( pItem->mIsHover == true )
		pCanvas->FillRect( pItem->mRect, GUI_MAKECOLOUR( 255, 30, 45, 46 ) );

	if ( pItem->mStyle == GUI_MS_POPUP )
	{
		CGUIRect tArrowRect = pItem->mRect;
		tArrowRect.mTopLeft.mX = pItem->mRect.mBottomRight.mX - pCanvas->GetFontHeight( ) - 2;
		tArrowRect.DeflateRect( 1, 1 );

		if ( pItem->mIsHover == true )
            pCanvas->BitBlt( mpTexture, CGUIRect( CGUIPoint( 45, 15 ), CGUISize( 16, 16 ) ), tArrowRect, 0xFFFFFFFF );
		else
			pCanvas->BitBlt( mpTexture, CGUIRect( CGUIPoint( 45, 0 ), CGUISize( 16, 16 ) ), tArrowRect, 0xFFFFFFFF );
	}

	pCanvas->PrintText( pItem->mText.c_str( ), pItem->mRect, GUI_DT_LEFT | GUI_DT_VCENTER, mTextColour );
}

void CGUIMenu::OnDrawMenu( CGUICanvas* pCanvas, const CGUIRect& rRect ) const
{
	pCanvas->DrawCombine( mpTexture, rRect, CGUIPoint( 0, 0 ), CGUISize( 15, 15 ) );
}

HMENUITEM CGUIMenu::AppendItem( HMENUITEM vParentMenu, const char* pText, unsigned int vMenuID, unsigned int vStyle )
{
	CGUIClientCanvas tCanvas( this );
	CMenuItem* tpParent = (CMenuItem*) vParentMenu;
	if ( tpParent == NULL )	tpParent = mpRoot;
	if ( tpParent->mStyle != GUI_MS_POPUP ) return NULL;

	CMenuItem* tpNewItem = new CMenuItem( pText, vStyle, tpParent, vMenuID );
	tpParent->mItems.push_back( tpNewItem );

	// 计算最大菜单宽度
	int tItemWidth	= 0;
	CGUIPoint tPoint = ( tpParent == mpRoot ? tpParent->mRect.TopRight( ) : tpParent->mRect.TopRight( ) + CGUIPoint( GUI_MENU_BORDER_WIDTH, 0 ) );
	for ( int i = 0; i < int( tpParent->mItems.size( ) ); i ++ )
	{
		int tTextCount = int( ::strlen( tpParent->mItems[ i ]->mText.c_str( ) ) );
		int tFontWidth = tCanvas.GetTextWidth( tpParent->mItems[ i ]->mText.c_str( ), tTextCount );
		tItemWidth = GUI_MAX( tItemWidth, tFontWidth + 20 );
	}
	
	// 计算菜单整体大小
	tpParent->mPopupRect.SetRect( tPoint, CGUISize( tItemWidth + GUI_MENU_BORDER_WIDTH * 2, 
		int( tpParent->mItems.size( ) ) * ( tCanvas.GetFontHeight( ) + 2 ) + GUI_MENU_BORDER_HEIGHT * 2 ) );

	// 计算每一个子菜单大小
	CMenuItem* tpMenuItem = NULL;
	for ( int i = 0; i < int( tpParent->mItems.size( ) ); i ++ )
	{
		tpMenuItem = tpParent->mItems[ i ];
		tpMenuItem->mRect.mTopLeft.mX = tPoint.mX;
		tpMenuItem->mRect.mTopLeft.mY = tPoint.mY + i * ( tCanvas.GetFontHeight( ) + 2 );
		tpMenuItem->mRect.mBottomRight.mX = tPoint.mX + tItemWidth;
		tpMenuItem->mRect.mBottomRight.mY = tPoint.mY + ( i + 1 ) * ( tCanvas.GetFontHeight( ) + 2 );
		tpMenuItem->mRect += CGUIPoint( GUI_MENU_BORDER_WIDTH, GUI_MENU_BORDER_HEIGHT );
	}

	// 根据当前菜单的最小区域调整窗口大小
    CGUISize tSizeOffset = mpRoot->GetMenuBox( ).Size( ) - mRect.Size( );
	SizeWindow( CGUIPoint( tSizeOffset.mCX, tSizeOffset.mCY ) );
	return tpNewItem;
}

void CGUIMenu::OnWindowRender( )
{
	CGUIClientCanvas tCanvas( this );
	mpRoot->DrawItems( &tCanvas, this );
}

void CGUIMenu::OnMouseLButtonUp( const CGUIPoint& rPoint, unsigned int vFlag )
{
	CGUIWindow::OnMouseLButtonUp( rPoint, vFlag );
	CMenuItem* tpItem = mpRoot->HitTest( ScreenToClient( rPoint ) );
	if ( tpItem != NULL )
	{
		CGUISinkCommand tCommand;
		tCommand.AddArg( CGUIDataType( GUI_PWINDOW, (unsigned int) this ) );
		tCommand.AddArg( CGUIDataType( GUI_INT, (unsigned int) tpItem->mMenuID ) );
		FireEvent( &tCommand );
		Destroy( );
	}
	
	CGUISystem::GetSingleton( ).NotifyRedraw( );
}

void CGUIMenu::OnMouseMove( const CGUIPoint& rPoint, unsigned int vFlag )
{
	CMenuItem* tpItem = mpRoot->HitTest( ScreenToClient( rPoint ) );

	if ( mpHover != NULL ) mpHover->mIsHover = false;
	if ( tpItem != NULL ) tpItem->mIsHover = true;

	mpHover = tpItem;
	if ( mpHover != NULL && mpHover->Expand( ) )
	{
		CGUISize tSizeOffset = mpRoot->GetMenuBox( ).Size( ) - mRect.Size( );
		SizeWindow( CGUIPoint( tSizeOffset.mCX, tSizeOffset.mCY ) );
	}

	CGUISystem::GetSingleton( ).NotifyRedraw( );
	CGUIWindow::OnMouseMove( rPoint, vFlag );
}

void CGUIMenu::OnMouseLeave( const CGUIPoint& rPoint, unsigned int vFlag )
{
	if ( mpHover != NULL ) 
	{
		mpHover->mIsHover = false;
		CGUISystem::GetSingleton( ).NotifyRedraw( );
	}

	mpHover = NULL;
	CGUIWindow::OnMouseEnter( rPoint, vFlag );
}

// ************************************************************ //
// CTreeItem
// ************************************************************ //
CGUITree::CTreeItem::~CTreeItem( )
{
	while ( !mNodes.empty( ) )
	{
		delete mNodes.at( 0 );
		mNodes.erase( mNodes.begin( ) );
	}
}

void CGUITree::CTreeItem::Expand( )
{
	if ( mNodes.size( ) == 0 ) return;
	mIsExpanded = ( mIsExpanded ? false : true );
}

CGUITree::CTreeItem* CGUITree::CTreeItem::HitTest( const CGUIPoint& rPoint )
{
	if ( mIsExpanded == false ) return NULL;

	CTreeItem* tpNode = NULL;
	for ( int i = 0; i < int( mNodes.size( ) ); i ++ )
	{
		mNodes[ i ]->mRect.PtInRect( rPoint ) ? tpNode = mNodes[ i ] : tpNode = mNodes[ i ]->HitTest( rPoint );
		if ( tpNode != NULL ) break;
	}

	return tpNode;
}

void CGUITree::CTreeItem::DrawItems( CGUICanvas* pCanvas, CGUITree* pTree )
{
	if ( ::strcmp( mText.c_str( ), "_RootNode" ) != 0 ) 
		pTree->OnDrawItem( pCanvas, this );

	if ( mIsExpanded == true )
	{
		for ( int i = 0; i < int( mNodes.size( ) ); i ++ )
			mNodes[i]->DrawItems( pCanvas, pTree );
	}
}

void CGUITree::CTreeItem::RemoveAllItems( )
{
	for ( int i = 0; i < int( mNodes.size( ) ); i ++ )
	{
		mNodes[ i ]->RemoveAllItems( );
		delete mNodes[ i ];
	}

	mNodes.clear( );
}

int CGUITree::CTreeItem::RefreshItems( CGUICanvas* pCanvas, int vLeft, int vTop, int vHeight, int& rWidth )
{
	int tLeft	= vLeft;
	int tTop	= vTop;
	int tHeight = vHeight;
	int tWidth	= 0;
	if ( mIsExpanded == false )	return tTop;
	for ( int i = 0; i < int( mNodes.size( ) ); i ++ )
	{
		tWidth = pCanvas->GetTextWidth( mNodes[ i ]->mText.c_str( ), int( ::strlen( mNodes[ i ]->mText.c_str( ) ) ) ) + 15;
		mNodes[ i ]->mRect.SetRect( tLeft, tTop, tLeft + tWidth, tTop + tHeight );
		rWidth = GUI_MAX( rWidth, tLeft + tWidth );
		tTop = mNodes[ i ]->RefreshItems( pCanvas, tLeft + 20, tTop + tHeight, vHeight, rWidth );
	}

	return tTop;
}

void CGUITree::OnDrawItem( CGUICanvas* pCanvas, CTreeItem* pItem ) const
{
	unsigned int tTextColor = 0xFFFFFFFF;
	CGUIRect tItemRect = pItem->mRect - mCamPoint;
	if ( pItem->mNodes.empty( ) == false )
	{
		if ( pItem->mIsSelected == true )
		{ 
			CGUIRect tBackRect = tItemRect;
			tBackRect.DeflateRect( 9, 0, 0, 0 );
			pCanvas->DrawCombine( mpTexture, tBackRect, CGUIPoint( 9, 0 ), CGUISize( 15, 15 ) );
		}

		if ( pItem->mIsExpanded == false )
			pCanvas->BitBlt( mpTexture, CGUIRect( 0, 0, 9, 9 ), CGUIRect( tItemRect.mTopLeft + CGUIPoint( 0, 5 ), CGUISize( 9, 9 ) ), 0xFFFFFFFF );
		else
			pCanvas->BitBlt( mpTexture, CGUIRect( 0, 18, 9, 27 ), CGUIRect( tItemRect.mTopLeft + CGUIPoint( 0, 5 ), CGUISize( 9, 9 ) ), 0xFFFFFFFF );

		pCanvas->PrintText( pItem->mText.c_str( ), tItemRect + CGUIPoint( 12, 0 ), GUI_DT_LEFT | GUI_DT_VCENTER, mTextColour );
	}
	else
	{
		if ( pItem->mIsSelected == true )
			pCanvas->DrawCombine( mpTexture, tItemRect, CGUIPoint( 9, 0 ), CGUISize( 15, 15 ) );

		pCanvas->PrintText( pItem->mText.c_str( ), tItemRect + CGUIPoint( 3, 0 ), GUI_DT_LEFT | GUI_DT_VCENTER, mTextColour );
	}
}

void CGUITree::OnWindowSized( const CGUIPoint& rOffset )
{
	CGUIWindow::OnWindowSized( rOffset );
	RefreshItems( );
}

void CGUITree::OnWindowRender( )
{
	CGUIClientCanvas tCanvas( this );
	mpRoot->DrawItems( &tCanvas, this );
}

void CGUITree::RefreshItems( )
{
	CGUIClientCanvas tCanvas( this );
	int tWidth = 0;
	int tHeight = mpRoot->RefreshItems( &tCanvas, 0, 0, tCanvas.GetFontHeight( ), tWidth );

	// 如果超过了最大的显示行数,就要显示纵向滚动条
	UpdateScrollBar( tWidth, tHeight );
}

void CGUITree::OnMouseLButtonDblClk( const CGUIPoint& rPoint, unsigned int vFlags )
{
	CTreeItem* tpNode = mpRoot->HitTest( ScreenToClient( rPoint ) + mCamPoint );
	if ( tpNode != NULL ) tpNode->Expand( );

	RefreshItems( );
	CGUIWindow::OnMouseLButtonDblClk( rPoint, vFlags );
}

void CGUITree::OnMouseLButtonDown( const CGUIPoint& rPoint, unsigned int vFlags )
{
	CTreeItem* tpNode = mpRoot->HitTest( ScreenToClient( rPoint ) + mCamPoint );
	if ( mpSelected != NULL ) mpSelected->mIsSelected = false;
	if ( tpNode != NULL ) tpNode->mIsSelected = true;

	mpSelected = tpNode;
	CGUIWindow::OnMouseLButtonDown( rPoint, vFlags );
}

void CGUITree::OnLoadResource( )
{
	GUI_LOADTEXTURE( mpTexture, "Tree.png" )
}

void CGUITree::OnDestroyResource( )
{
	GUI_DESTROYTEXTURE( mpTexture )
}

void CGUITree::RemoveAllItems( )
{
	mpRoot->RemoveAllItems( );
}

void* CGUITree::InsertItem( const char* pText, void* vParent, void* vInsertAfter )
{
	CTreeItem* tpNewNode = new CTreeItem( pText, false );
	CTreeItem* tpParent	= (CTreeItem*) vParent;
	CTreeItem* tpAfter	= (CTreeItem*) vInsertAfter;
	if ( vParent == NULL ) tpParent = mpRoot;

	if ( vInsertAfter == NULL )
	{
		if ( tpParent->mNodes.size( ) == 0 )
			tpAfter = NULL;
		else
            tpAfter = tpParent->mNodes[ tpParent->mNodes.size( ) - 1 ];
	}

	for ( int i = 0; i < int( tpParent->mNodes.size( ) ); i ++ )
	{
		if ( tpAfter == tpParent->mNodes[ i ] )
		{
			tpParent->mNodes.insert( tpParent->mNodes.begin( ) + i + 1, tpNewNode );
			break;
		}
	}

	if ( tpAfter == NULL ) tpParent->mNodes.push_back( tpNewNode );
	RefreshItems( );
	return tpNewNode;
}

// ************************************************************ //
// CGUIList
// ************************************************************ //
CGUIList::CRowItem::~CRowItem( )
{
	while ( !mItems.empty( ) )
	{
		delete mItems.at( 0 );
		mItems.erase( mItems.begin( ) );
	}
}

CGUIList::~CGUIList( )
{
	RemoveAllItems( );

	while ( !mColumns.empty( ) )
	{
		delete mColumns.at( 0 );
		mColumns.erase( mColumns.begin( ) );
	}
}

void CGUIList::RemoveItem( int vItemIndex )
{
	delete mItems.at( vItemIndex );
	mItems.erase( mItems.begin( ) + vItemIndex );
	mSlotRects.erase( mSlotRects.begin( ) + mSlotRects.size( ) - 1 );
}

void CGUIList::RemoveAllItems( )
{
	while ( !mItems.empty( ) )
	{
		delete mItems.at( 0 );
		mItems.erase( mItems.begin( ) );
	}

	mSlotRects.clear( );
}

int CGUIList::GetCount( ) const
{
	return int( mItems.size( ) );
}

std::string CGUIList::GetItemText( int vItemIndex, int vSubItem ) const
{
	return mItems[ vItemIndex ]->mItems[ vSubItem ]->mText;
}

int CGUIList::GetCurSel( ) const
{
	for ( int i = 0; i < int( mItems.size( ) ); i ++ )
	{
		if ( mItems[ i ]->mIsSelected == true)
			return i;
	}

	return -1;
}

void CGUIList::SetCurSel( int vItemIndex )
{
	// 将所有的Item标记为没有被选中
	for ( int i = 0; i < int( mItems.size( ) ); i ++ )
		mItems[ i ]->mIsSelected = false;

	// 如果超过当前的Item个数，就返回
	if ( vItemIndex < 0 || vItemIndex >= int( mItems.size( ) ) )
		return;
	
	mItems[ vItemIndex ]->mIsSelected = true;
	CGUISinkListSelChanged tListSelChanged;
	tListSelChanged.AddArg( CGUIDataType( GUI_PWINDOW, (unsigned int) this ) );
	tListSelChanged.AddArg( CGUIDataType( GUI_INT, vItemIndex ) );
	FireEvent( &tListSelChanged );
}

int CGUIList::HitTest( const CGUIPoint& rPoint )
{
	for ( int i = 0; i < int( mSlotRects.size( ) ); i ++ )
	{
		if ( mSlotRects[ i ].PtInRect( ScreenToClient( rPoint ) + mCamPoint ) )
			return i;
	}

	return -1;
}

void CGUIList::AppendColumn( const char* pText, int vWidth, int vFormat )
{
	CGUIClientCanvas tCanvas( this );
	int tColumnHeight	= tCanvas.GetFontHeight( ) + 2;
	int tItemHeight		= tCanvas.GetFontHeight( ) + 2;
	CColumnItem* tpColumn = new CColumnItem( pText );
	if ( mColumns.size( ) == 0 )
		tpColumn->mRect.SetRect( CGUIPoint( 0, 0 ), CGUISize( vWidth, tColumnHeight ) );
	else
        tpColumn->mRect.SetRect( mColumns[ mColumns.size( ) - 1 ]->mRect.TopRight( ), CGUISize( vWidth, tColumnHeight ) );

	tpColumn->mFormat = vFormat;
	for ( int i = 0; i < int( mItems.size( ) ); i ++ )
	{
		mSlotRects[ i ] = CGUIRect( mColumns[0]->mRect.mTopLeft, tpColumn->mRect.mBottomRight ) + mSlotRects[ i ].mTopLeft;
		mItems[ i ]->mItems.push_back( new CGridItem( "" ) );
	}

	mColumns.push_back( tpColumn );
	int tColumnCount = int( mColumns.size( ) );
	int tRowCount	 = int( mItems.size( ) );
	int tWidth = mColumns[ tColumnCount - 1 ]->mRect.mBottomRight.mX;
	int tHeight = tRowCount * tItemHeight + tColumnHeight;
	UpdateScrollBar( tWidth, tHeight );
}

int CGUIList::AppendItem( const char* pText )
{
	int tColumnCount = int( mColumns.size( ) );
	int tRowCount	 = int( mItems.size( ) );
	if ( tColumnCount == 0 )
		return -1;

	CGUIClientCanvas tCanvas( this );
	int tColumnHeight	= tCanvas.GetFontHeight( ) + 2;
	int tItemHeight		= tCanvas.GetFontHeight( ) + 2;
	int tWidth = mColumns[ tColumnCount - 1 ]->mRect.mBottomRight.mX;
	CGUIRect tItemRect( CGUIPoint( 0, tRowCount * tItemHeight + tColumnHeight ), CGUISize( tWidth, tColumnHeight ) );
	mSlotRects.push_back( tItemRect );

	CRowItem* tpRow = new CRowItem;
	for ( int i = 0; i < tColumnCount; i ++ )
	{
		CGridItem* tpGrid = new CGridItem( pText );
		i == 0 ? tpGrid->mText = pText : tpGrid->mText = "";
		tpRow->mItems.push_back( tpGrid );
	}

	mItems.push_back( tpRow );
	tRowCount = int( mItems.size( ) );
	int tHeight = tRowCount * tItemHeight + tColumnHeight;
	UpdateScrollBar( tWidth, tHeight );
	return tRowCount - 1;
}

void CGUIList::SetItemData( int vItem, unsigned int vUserData )
{
	int tRowCount = int( mItems.size( ) );
	if ( vItem >= tRowCount )
		return;

	mItems[ vItem ]->mItemData = vUserData;
}

unsigned int CGUIList::GetItemData( int vItem ) const
{
	int tRowCount = int( mItems.size( ) );
	if ( vItem >= tRowCount )
		return 0;

	return mItems[ vItem ]->mItemData;
}

void CGUIList::SetItem( int vItem, int vSubItem, const char* pText )
{
	int tRowCount	 = int( mItems.size( ) );
	int tColumnCount = int( mColumns.size( ) );
	if ( vSubItem >= tColumnCount || vItem >= tRowCount )
		return;
	
	mItems[ vItem ]->mItems[ vSubItem ]->mText = pText;
}

void CGUIList::OnLoadResource( )
{
	GUI_LOADTEXTURE( mpTexture, "List.png" )
}

void CGUIList::OnDestroyResource( )
{
	GUI_DESTROYTEXTURE( mpTexture )
}

void CGUIList::OnDrawColumn( CGUICanvas* pCanvas, CGUIList::CColumnItem* pItem )
{
	int tTextStyle = GUI_DT_VCENTER | GUI_DT_WRAPTEXT;
	if ( pItem->mFormat == GUI_LCS_LEFT ) 
		tTextStyle |= GUI_DT_LEFT;
	else if ( pItem->mFormat == GUI_LCS_CENTER ) 
		tTextStyle |= GUI_DT_HCENTER;
	else if ( pItem->mFormat == GUI_LCS_RIGHT ) 
		tTextStyle |= GUI_DT_RIGHT;

	CGUIRect tTextRect = pItem->mRect - mCamPoint;
	if ( pItem->mIsClicked == true )
	{
		pCanvas->DrawCombine( mpTexture, tTextRect, CGUIPoint( 0, 45 ), CGUISize( 15, 15 ) );
		tTextRect.DeflateRect( 2, 0, 2, 0 );
		pCanvas->PrintText( pItem->mText.c_str( ), tTextRect + CGUIPoint( 1, 1 ), tTextStyle, mTextColour );
	}
	else
	{
		pCanvas->DrawCombine( mpTexture, tTextRect, CGUIPoint( 0, 0 ), CGUISize( 15, 15 ) );
    	tTextRect.DeflateRect( 2, 0, 2, 0 );
	    pCanvas->PrintText( pItem->mText.c_str( ), tTextRect, tTextStyle, mTextColour );
	}

	CGUIRect tSortRect;
	if ( pItem->mFormat == GUI_LCS_LEFT )
	{
		tSortRect.mTopLeft = tTextRect.TopRight( ) - CGUIPoint( GUI_LIST_SORTBOUND_WIDTH, -1 );
		tSortRect.mBottomRight = tSortRect.mTopLeft + CGUIPoint( GUI_LIST_SORT_WIDTH, GUI_LIST_SORT_HEIGHT );
	}
	else
	{
		tSortRect.mTopLeft = tTextRect.mTopLeft + CGUIPoint( 2, 1 );
		tSortRect.mBottomRight = tSortRect.mTopLeft + CGUIPoint( GUI_LIST_SORT_WIDTH, GUI_LIST_SORT_HEIGHT );
	}
	if ( pItem->mIsClicked )
		tSortRect += CGUIPoint( 1, 1 );

	if ( pItem->mSortType == 1 )
		pCanvas->BitBlt( mpTexture, CGUIRect( CGUIPoint( 45, 0 ), CGUISize( GUI_LIST_SORT_WIDTH, GUI_LIST_SORT_WIDTH ) ), tSortRect, 0xFFFFFFFF );
	else if ( pItem->mSortType == 2 )
		pCanvas->BitBlt( mpTexture, CGUIRect( CGUIPoint( 45, 16 ), CGUISize( GUI_LIST_SORT_WIDTH, GUI_LIST_SORT_WIDTH ) ), tSortRect, 0xFFFFFFFF );
}

void CGUIList::OnDrawGrid( CGUICanvas* pCanvas, const char* pText, const CGUIRect& rRect, int vFormat )
{
	int tTextStyle = GUI_DT_VCENTER | GUI_DT_WRAPTEXT;
	if ( vFormat == GUI_LCS_LEFT ) 
		tTextStyle |= GUI_DT_LEFT;
	else if ( vFormat == GUI_LCS_CENTER ) 
		tTextStyle |= GUI_DT_HCENTER;
	else if ( vFormat == GUI_LCS_RIGHT ) 
		tTextStyle |= GUI_DT_RIGHT;

	pCanvas->PrintText( pText, rRect + CGUIPoint( 2, 0 ), tTextStyle, mTextColour );
}

void CGUIList::OnDrawItem( CGUICanvas* pCanvas, const CGUIRect& rRect, bool vIsSelected, int vItemIndex )
{
	if ( vIsSelected == true)
		pCanvas->FillRect( rRect, mSelColour );
}

void CGUIList::OnMouseLButtonUp( const CGUIPoint& rPoint, unsigned int vFlag )
{
	CGUIWindow::OnMouseLButtonUp( rPoint, vFlag );
	CGUISystem::GetSingleton( ).ReleaseCapture( this );
	for ( int i = 0; i < int( mColumns.size( ) ); i ++ )
	{
		CColumnItem* tpItem = mColumns[ i ];
		if ( tpItem->mIsClicked == true )
		{
			tpItem->mIsClicked = false;
			CGUISystem::GetSingleton( ).NotifyRedraw( );
			Sort( i, tpItem->mSortType == 1 );
			break;
		}
	}
	mSizeColumn = -1;
}

void CGUIList::OnKeyDown( unsigned int vVirtualKey, unsigned int vFlag )
{
	int tSelIndex = GetCurSel( );
	switch ( vVirtualKey )
	{
		case GUI_UP:
		{
			if ( tSelIndex > 0 ) SetCurSel( tSelIndex - 1 );
			CGUISystem::GetSingleton( ).NotifyRedraw( );
			break;
		}
		case GUI_DOWN:
		{
			if ( tSelIndex < int( mItems.size( ) - 1 ) ) SetCurSel( tSelIndex + 1 );
			CGUISystem::GetSingleton( ).NotifyRedraw( );
			break;
		}
	}

	tSelIndex = GetCurSel( );
	if ( tSelIndex != -1 )
	{
		int tOffsetY = ( mSlotRects[ tSelIndex ].mTopLeft - mCamPoint ).mY;
		if ( tOffsetY < 0 ) 
			mpVScrollBar->SetPosition( mpVScrollBar->GetPosition( ) + tOffsetY );
		
		tOffsetY = ( mSlotRects[ tSelIndex ].mBottomRight - mCamPoint ).mY - GetClientRect( ).Height( );
		if ( tOffsetY > 0 ) 
			mpVScrollBar->SetPosition( mpVScrollBar->GetPosition( ) + tOffsetY );
	}
	CGUIWindow::OnKeyDown( vVirtualKey, vFlag );
}

void CGUIList::OnMouseLButtonDblClk( const CGUIPoint& rPoint, unsigned int vFlag )
{
	int tItemIndex = GetCurSel( );
	// 如果双击时没有点中其中一项，就不点燃事件
	if ( tItemIndex != -1 )
	{
		// 点燃双击事件，告诉事件接受方发生双击的List，和当前选中的Item
		CGUISinkListDblClk tListDblClk;
		tListDblClk.AddArg( CGUIDataType( GUI_PWINDOW, (int) this ) );
		tListDblClk.AddArg( CGUIDataType( GUI_INT, tItemIndex ) );
		FireEvent( &tListDblClk );
	}
	CGUIWindow::OnMouseLButtonDblClk( rPoint, vFlag );
}

void CGUIList::OnMouseMove( const CGUIPoint& rPoint, unsigned int vFlag )
{
	if ( mSizeColumn != -1 )
	{
		int tOffset = ( rPoint - mLastLButton ).mX;
		CGUIRect tRect1 = mColumns[ mSizeColumn ]->mRect;
		CGUIRect tRect2 = mColumns[ mSizeColumn + 1 ]->mRect;
		tOffset = GUI_MAX( tOffset, 16 - tRect1.Width( ) );
		tOffset = GUI_MIN( tOffset, tRect2.Width( ) - 16 );
		mColumns[ mSizeColumn ]->mRect.InflateRect( 0, 0, tOffset, 0 );
		mColumns[ mSizeColumn + 1 ]->mRect.DeflateRect( tOffset, 0, 0, 0 );
		mLastLButton.mX += tOffset;
		CGUISystem::GetSingleton( ).NotifyRedraw( );
	}
	CGUIWindow::OnMouseMove( rPoint, vFlag );
}

void CGUIList::Sort( int vColumnIndex, bool vIsAscending )
{
	CRowItem* tpTmp = NULL;
	for ( int i = 0; i < int( mItems.size( ) ); i ++ )
	{
		for ( int j = i + 1; j < int( mItems.size( ) ); j ++ )
		{
			CGridItem* tpItem1 = mItems[ i ]->mItems[ vColumnIndex ];
			CGridItem* tpItem2 = mItems[ j ]->mItems[ vColumnIndex ];
			if ( vIsAscending )
			{
				if ( strcmp( tpItem1->mText.c_str( ), tpItem2->mText.c_str( ) ) > 0 )
				{
					tpTmp		= mItems[ j ];
					mItems[ j ]	= mItems[ i ];
					mItems[ i ]	= tpTmp;
				}
			}
			else
			{
				if ( strcmp( tpItem1->mText.c_str( ), tpItem2->mText.c_str( ) ) < 0 )
				{
					tpTmp		= mItems[ j ];
					mItems[ j ]	= mItems[ i ];
					mItems[ i ] = tpTmp;
				}
			}
		}
	}
}

void CGUIList::OnMouseLButtonDown( const CGUIPoint& rPoint, unsigned int vFlags )
{
	int tColumnCount = int( mColumns.size( ) );
	if ( tColumnCount == 0 ) return;

	CGUIRect tSizeRect;
	for ( int i = 0; i < int( mColumns.size( ) ) - 1; i ++ )
	{
		CColumnItem* tpColumn1 = mColumns[ i ];
		CColumnItem* tpColumn2 = mColumns[ i + 1 ];
		tSizeRect.SetRect( tpColumn1->mRect.TopRight( ) - CGUIPoint( 2, 0 ), tpColumn2->mRect.BottomLeft( ) + CGUIPoint( 2, 0 ) );
		if ( tSizeRect.PtInRect( ScreenToClient( rPoint ) + mCamPoint ) )
		{
			CGUISystem::GetSingleton( ).SetCapture( this );
			mSizeColumn		= i;
			mLastLButton	= rPoint; 
			return;
		}
	}

	CGUIRect tColumnRect( mColumns[ 0 ]->mRect.mTopLeft, mColumns[ tColumnCount - 1 ]->mRect.mBottomRight );
	for ( int i = 0; i < int( mColumns.size( ) ); i ++ )
	{
		CColumnItem* tpColumn = mColumns[ i ];
		if ( tpColumn->mRect.PtInRect( ScreenToClient( rPoint ) + mCamPoint ) )
		{
			CGUISystem::GetSingleton( ).SetCapture( this );
			tpColumn->mIsClicked = true;
			if ( tpColumn->mSortType == 0 )
				tpColumn->mSortType = 1;
			else if ( tpColumn->mSortType == 1 )
				tpColumn->mSortType = 2;
			else if ( tpColumn->mSortType == 2 )
				tpColumn->mSortType = 1;
		}
		else if ( tColumnRect.PtInRect( ScreenToClient( rPoint ) + mCamPoint ) )
			tpColumn->mSortType = 0;
	}

	SetCurSel( HitTest( rPoint ) );
	CGUIWindow::OnMouseLButtonDown( rPoint, vFlags );
}

void CGUIList::OnWindowSized( const CGUIPoint& rOffset )
{
	CGUIWindow::OnWindowSized( rOffset );

	CGUIClientCanvas tCanvas( this );
	int tColumnHeight	= tCanvas.GetFontHeight( ) + 2;
	int tItemHeight		= tCanvas.GetFontHeight( ) + 2;
	int tColumnCount	= int( mColumns.size( ) );
	int tRowCount		= int( mItems.size( ) );
	if ( tColumnCount == 0 )
		return;

	int tWidth	= mColumns[ tColumnCount - 1 ]->mRect.mBottomRight.mX;
	int tHeight	= tRowCount * tItemHeight + tColumnHeight;
	UpdateScrollBar( tWidth, tHeight );
}

void CGUIList::OnWindowRender( )
{
	CGUIClientCanvas tCanvas( this );
	CGUIRect tClientRect = GetClientRect( );

	for ( int i = 0; i < int( mColumns.size( ) ); i ++ )
	{
		CColumnItem* tpItem = mColumns[ i ];
		OnDrawColumn( &tCanvas, tpItem );
	}
	
	for ( int i = 0; i < int( mSlotRects.size( ) ); i ++ )
		OnDrawItem( &tCanvas, mSlotRects[ i ] - mCamPoint, mItems[ i ]->mIsSelected, i );

	for ( int i = 0; i < int( mColumns.size( ) ); i ++ )
	{
		CColumnItem* tpColoumn = mColumns[ i ];
		for ( int j = 0; j < int( mSlotRects.size( ) ); j ++ )
		{
			CRowItem* tpRow = mItems[ j ];
			CGUIRect tGridRect = tpColoumn->mRect + mSlotRects[ j ].mTopLeft - mCamPoint;
			OnDrawGrid( &tCanvas, tpRow->mItems[ i ]->mText.c_str( ), tGridRect, tpColoumn->mFormat );
		}
	}
}

// ************************************************************ //
// CGUIComboBox
// ************************************************************ //

void CGUIComboBox::ExpandDown( )
{
	if ( mExpanded == false )
	{
		// 如果当前是向上展开的
		if ( mExpandState == 1 )
		{

			SizeWindow( CGUIPoint( 0, -mListRect.Height( ) ) );
			MoveWindow( CGUIPoint( 0, mListRect.Height( ) ) );
			mpList->MoveWindow( CGUIPoint( 0, mComboRect.Height( ) ) );
			mListRect.OffsetRect( CGUIPoint( 0, mComboRect.Height( ) ) );
			mComboRect.OffsetRect( CGUIPoint( 0, -mListRect.Height( ) ) );
			mBtnRect.OffsetRect( CGUIPoint( 0, -mListRect.Height( ) ) );
			mpExpandBtn->MoveWindow( CGUIPoint( 0, -mListRect.Height( ) ) );
		}

		// 如果当前是向下展开的
		if ( mExpandState == 0 )
			SizeWindow( CGUIPoint( 0, -mListRect.Height( ) ) );
	}
	else 
	{
		// 向下展开
		SizeWindow( CGUIPoint( 0, mListRect.Height( ) ) );
		mExpandState = 0;
	}
}

void CGUIComboBox::ExpandUp( )
{
	if ( mExpanded == false )
	{
		// 如果当前是向上展开的
		if ( mExpandState == 1 )
		{
			SizeWindow( CGUIPoint( 0, -mListRect.Height( ) ) );
			MoveWindow( CGUIPoint( 0, mListRect.Height( ) ) );
			mpList->MoveWindow( CGUIPoint( 0, mComboRect.Height( ) ) );
			mListRect.OffsetRect( CGUIPoint( 0, mComboRect.Height( ) ) );
			mComboRect.OffsetRect( CGUIPoint( 0, -mListRect.Height( ) ) );
			mBtnRect.OffsetRect( CGUIPoint( 0, -mListRect.Height( ) ) );
			mpExpandBtn->MoveWindow( CGUIPoint( 0, -mListRect.Height( ) ) );
		}

		// 如果当前是向下展开的
		if ( mExpandState == 0 )
			SizeWindow( CGUIPoint( 0, -mListRect.Height( ) ) );
	}
	else 
	{
		// 向上展开
		SizeWindow( CGUIPoint( 0, mListRect.Height( ) ) );
		MoveWindow( CGUIPoint( 0, -mListRect.Height( ) ) );
		mpList->MoveWindow( CGUIPoint( 0, -mComboRect.Height( ) ) );
		mListRect.OffsetRect( CGUIPoint( 0, -mComboRect.Height( ) ) );
		mComboRect.OffsetRect( CGUIPoint( 0, mListRect.Height( ) ) );
		mBtnRect.OffsetRect( CGUIPoint( 0, mListRect.Height( ) ) );
		mpExpandBtn->MoveWindow( CGUIPoint( 0, mListRect.Height( ) ) );
		mExpandState = 1;
	}
}

void CGUIComboBox::ComboExpand( bool vExpand )
{
	if ( mExpanded == vExpand )
		return;

	mExpanded = vExpand;
	if ( mWindowStyle & GUI_CBS_DOWNEXPAND )
        ExpandDown( );
	else if ( mWindowStyle & GUI_CBS_UPEXPAND )
		ExpandUp( );
}

void CGUIComboBox::ComboExpand( )
{
	mExpanded = !mExpanded;
	if ( mWindowStyle & GUI_CBS_DOWNEXPAND )
        ExpandDown( );
	else if ( mWindowStyle & GUI_CBS_UPEXPAND )
		ExpandUp( );
}

void CGUIComboBox::OnWindowFocus( CGUIWindow* pWindow, bool vFocus )
{
	// 如果本窗口失去焦点，并且本窗口不是得到焦点的窗口的祖先
	if ( vFocus == false && pWindow != NULL && pWindow->IsAncestor( this ) == false )
		ComboExpand( false );
		
	CGUIWindow::OnWindowFocus( pWindow, vFocus );
}

void CGUIComboBox::OnExpandBtnFocus( CGUIWindow* pWindow, bool vFocus )
{
	// 如果按钮窗口失去焦点，并且本窗口不是得到焦点的窗口的祖先
	if ( vFocus == false && pWindow != NULL && pWindow->IsAncestor( this ) == false )
		ComboExpand( false );
}

void CGUIComboBox::OnListFocus( CGUIWindow* pWindow, bool vFocus )
{
	// 如果按钮窗口失去焦点，并且本窗口不是得到焦点的窗口的祖先
	if ( vFocus == false && pWindow != NULL && pWindow->IsAncestor( this ) == false )
		ComboExpand( false );
}

int CGUIComboBox::OnWindowCreate( )
{
	if ( CGUIWindow::OnWindowCreate( ) == -1 )
		return -1;

	CGUIWindowManager* tpWmgr = CGUIWindowManager::GetSingletonPtr( );
	CGUIRect tClientRect = GetClientRect( );
	mBtnRect.mTopLeft.mX		= tClientRect.mBottomRight.mX - GUI_COMBOBOX_BUTTON_WIDTH - 3;
	mBtnRect.mTopLeft.mY		= tClientRect.mTopLeft.mY + 3;
	mBtnRect.mBottomRight.mX	= tClientRect.mBottomRight.mX - 3;
	mBtnRect.mBottomRight.mY	= mBtnRect.mTopLeft.mY + mpFont->GetFontHeight( ) + 4;

	mComboRect = tClientRect;
	mComboRect.mBottomRight.mY = mBtnRect.mBottomRight.mY + 3;

	mpExpandBtn = (CGUIImageButton*) tpWmgr->Create( "GUIImageButton", NULL, 0, mBtnRect, this, NULL );
	mpExpandBtn->SetImage( "ComboExpand.png", CGUISize( 16, 16 ) );

	mListRect.mTopLeft.mX	= tClientRect.mTopLeft.mX;
	mListRect.mTopLeft.mY	= mBtnRect.mBottomRight.mY + 3;
	mListRect.mBottomRight	= tClientRect.mBottomRight;
	mpList = (CGUIListBox*) tpWmgr->Create( "GUIListBox", NULL, GUI_LBS_MOUSEOVER, mListRect - CGUIRect( 4, 4, 4, 4 ), this, NULL );

	ON_BN_CLICKED( mpExpandBtn, OnExpandClicked )
	ON_LB_SELCHANGED( mpList, OnListBoxSelChanged )
	ON_WM_WINDOWFOCUS( mpExpandBtn, OnExpandBtnFocus )
	ON_WM_WINDOWFOCUS( mpList, OnListFocus )
	ComboExpand( false );
	return 0;
}

void CGUIComboBox::OnListBoxSelChanged( CGUIWindow* pListBox, int vCurSel )
{
	mSelItem	= vCurSel;
	mTitle		= ( (CGUIListBox*) pListBox )->GetItemText( vCurSel );
	ComboExpand( false );
}

void CGUIComboBox::OnExpandClicked( CGUIWindow* pButton )
{
	ComboExpand( );
}

void CGUIComboBox::OnWindowRender( )
{
	CGUIClientCanvas tCanvas( this );
	tCanvas.DrawRect( mListRect, 0xFFFFFFFF, 0xFFFFFFFF );
	tCanvas.DrawCombine( mpTexture, mComboRect, CGUIPoint( 0, 0 ), CGUISize( 15, 15 ) );
	tCanvas.DrawRect( mComboRect - CGUIRect( 2, 2, 2, 2 ), 0xFF000000, 0xFF808080 );

	tCanvas.DrawCombine( mpTexture, mListRect, CGUIPoint( 0, 0 ), CGUISize( 15, 15 ) );
	tCanvas.DrawRect( mListRect - CGUIRect( 2, 2, 2, 2 ), 0xFF000000, 0xFF808080 );

	CGUIRect tTextRect = mComboRect - CGUIRect( 3, 3, 3 + GUI_COMBOBOX_BUTTON_WIDTH, 3 );
	tCanvas.PrintText( mTitle.c_str( ), tTextRect, GUI_DT_LEFT | GUI_DT_VCENTER | GUI_DT_WRAPTEXT, mTextColour );
}

void CGUIComboBox::OnLoadResource( )
{
	GUI_LOADTEXTURE( mpTexture, "ComboBox.png" )
}

void CGUIComboBox::OnDestroyResource( )
{
	GUI_DESTROYTEXTURE( mpTexture )
}

// ************************************************************ //
// CGUIProgress
// ************************************************************ //
void CGUIProgress::OnWindowRender( )
{
	CGUIRect tInner = GetClientRect( );
	tInner.DeflateRect( 2, 2 );
	tInner.mBottomRight.mX = tInner.mTopLeft.mX + int( ( mPos - mRangeMin ) / ( (float) mRangeMax - mRangeMin ) * tInner.Width( ) );

	CGUIWindowCanvas tCanvas( this );
	OnDrawShade( &tCanvas );
	OnDrawSlider( &tCanvas, tInner );
}

void CGUIProgress::OnLoadResource( )
{
	GUI_LOADTEXTURE( mpTexture, "Progress.png" )
}

void CGUIProgress::OnDestroyResource( )
{
	GUI_DESTROYTEXTURE( mpTexture )
}

void CGUIProgress::OnMouseLButtonDown( const CGUIPoint& rPoint, unsigned int vFlag )
{
	if ( mWindowStyle & GUI_PS_SLIDER )
	{
		CGUISystem::GetSingleton( ).SetCapture( this );
		CGUIRect tInner = GetClientRect( );
		tInner.DeflateRect( 2, 2 );
		tInner = WindowToScreen( tInner );

		int tOffset = rPoint.mX - tInner.mTopLeft.mX;
		SetPos( int( ( mRangeMax - mRangeMin ) * ( GUI_MIN( tOffset, tInner.Width( ) ) / (float) tInner.Width( ) ) ) );
		CGUISystem::GetSingleton( ).NotifyRedraw( );

		CGUISinkSliderPosChanged tSliderPosChanged;
		tSliderPosChanged.AddArg( CGUIDataType( GUI_PWINDOW, (unsigned int) this ) );
		tSliderPosChanged.AddArg( CGUIDataType( GUI_INT, GetPos( ) ) );
		FireEvent( &tSliderPosChanged );
	}

	CGUIWindow::OnMouseLButtonDown( rPoint, vFlag );
}

void CGUIProgress::OnMouseMove( const CGUIPoint& rPoint, unsigned int vFlag )
{
	if ( vFlag & GUI_LBUTTON && mWindowStyle & GUI_PS_SLIDER )
	{	
		CGUIRect tInner = GetClientRect( );
		tInner.DeflateRect( 2, 2 );
		tInner = WindowToScreen( tInner );

		int tOffset = GUI_MAX( 0, rPoint.mX - tInner.mTopLeft.mX );
		SetPos( int( ( mRangeMax - mRangeMin ) * ( GUI_MIN( tOffset, tInner.Width( ) ) / (float) tInner.Width( ) ) ) );
		CGUISystem::GetSingleton( ).NotifyRedraw( );

		CGUISinkSliderPosChanged tSliderPosChanged;
		tSliderPosChanged.AddArg( CGUIDataType( GUI_PWINDOW, (unsigned int) this ) );
		tSliderPosChanged.AddArg( CGUIDataType( GUI_INT, GetPos( ) ) );
		FireEvent( &tSliderPosChanged );
	}

	CGUIWindow::OnMouseMove( rPoint, vFlag );
}

void CGUIProgress::OnMouseLButtonUp( const CGUIPoint& rPoint, unsigned int vFlag )
{
	CGUISystem::GetSingleton( ).ReleaseCapture( this );
	CGUIWindow::OnMouseLButtonUp( rPoint, vFlag );
}

void CGUIProgress::OnDrawShade( CGUICanvas* pCanvas )
{
	pCanvas->DrawCombine( mpTexture, GetClientRect( ), CGUIPoint( 0, 0 ), CGUISize( 80, 5 ) );
}

void CGUIProgress::OnDrawSlider( CGUICanvas* pCanvas, const CGUIRect& rRect )
{
	CGUIRect tInner = GetClientRect( ); tInner.DeflateRect( 2, 2, 1, 1 );
	pCanvas->FillRect( rRect, 0xFF800000 );
	pCanvas->DrawLine( rRect.TopRight( ), rRect.BottomRight( ), 0x80000000 );
	pCanvas->DrawRect( tInner, 0xFF000000, 0xFF808080 );
}

// ************************************************************ //
// CGUIMessageBox
// ************************************************************ //
int CGUIMessageBox::OnWindowCreate( )
{
	if ( CGUIWindow::OnWindowCreate( ) == -1 )
		return -1;

	CGUIRect tClientRect = GetWindowRect( );
	CGUIPoint tBottomCenter;
	tBottomCenter.mX = tClientRect.CenterPoint( ).mX;
	tBottomCenter.mY = tClientRect.BottomRight( ).mY;
	CGUIWindowManager* tpWmgr = CGUIWindowManager::GetSingletonPtr( );
	if ( mWindowStyle & GUI_MBS_OK && mWindowStyle & GUI_MBS_CANCEL )
	{
		mpYes = (CGUIButton*) tpWmgr->Create( "GUIButton", NULL, 0, CGUIRect( tBottomCenter.mX - 80, tBottomCenter.mY - 30, tBottomCenter.mX - 20, tBottomCenter.mY - 10 ), this, NULL );
		mpNo  = (CGUIButton*) tpWmgr->Create( "GUIButton", NULL, 0, CGUIRect( tBottomCenter.mX + 20, tBottomCenter.mY - 30, tBottomCenter.mX + 80, tBottomCenter.mY - 10 ), this, NULL );
		mpYes->SetText( "确定" );
		mpNo->SetText( "取消" );
		ON_BN_CLICKED( mpYes, OnClickYes );
		ON_BN_CLICKED( mpNo, OnClickNo );
	}
	else if ( mWindowStyle & GUI_MBS_OK )
	{
		mpYes = (CGUIButton*) tpWmgr->Create( "GUIButton", NULL, 0, CGUIRect( tBottomCenter.mX - 30, tBottomCenter.mY - 30, tBottomCenter.mX + 30, tBottomCenter.mY - 10 ), this, NULL );
		mpYes->SetText( "确定" );
		ON_BN_CLICKED( mpYes, OnClickYes );
	}
	else if ( mWindowStyle & GUI_MBS_CANCEL )
	{
		mpNo = (CGUIButton*) tpWmgr->Create( "GUIButton", NULL, 0, CGUIRect( tBottomCenter.mX - 30, tBottomCenter.mY - 30, tBottomCenter.mX + 30, tBottomCenter.mY - 10 ), this, NULL );
		mpNo->SetText( "取消" );
		ON_BN_CLICKED( mpNo, OnClickNo );
	}

	if ( mpYes != NULL )
		mpYes->SetArchor( GUI_BOTTOMARCHOR_BOTTOM | GUI_TOPARCHOR_BOTTOM );

	if ( mpNo != NULL )
        mpNo->SetArchor( GUI_BOTTOMARCHOR_BOTTOM | GUI_TOPARCHOR_BOTTOM );

	return 0;
}

void CGUIMessageBox::SetMessage( const char* pText )
{
	int tTextWidth = mpFont->GetTextWidth( pText, ::strlen( pText ) );
	int tWindowHeight = GetWindowRect( ).Height( );
	int i = 0;
	int tStartPos = 0;
	int tEndPos = 0;
	CGUIMessageBox::CMessageItem* tpItem = NULL;
	std::string tTextBuffer;
	while ( tTextWidth > 280 )
	{
		int tOffsetWidth = 0;
		tTextBuffer.clear( );
		while( pText[ i ] != 0 )
		{
			tOffsetWidth += mpFont->GetTextWidth( pText + i );
			if ( tOffsetWidth > 280 )
			{
				tOffsetWidth -= mpFont->GetTextWidth( pText + i );
				for ( int j = tStartPos; j < tEndPos; j ++ )
					tTextBuffer.push_back( *( pText + j ) );
				tStartPos = tEndPos;
				break;
			}

			pText[ i ] & 0x80 ? ( i += 2, tEndPos += 2 ) : ( i ++, tEndPos ++ );
		}
		tpItem = new CGUIMessageBox::CMessageItem;
		tpItem->mText = tTextBuffer;
		mItems.push_back( tpItem );
		tTextWidth -= tOffsetWidth;
		SizeWindow( CGUIPoint( 0, mpFont->GetFontHeight( ) + 5 ) );
	}

	if ( pText[ i ] != 0 )
	{
		tTextBuffer.clear( );
		for ( int j = tStartPos; j < int( ::strlen( pText ) ); j ++ )
			tTextBuffer.push_back( *( pText + j ) );

		tpItem = new CGUIMessageBox::CMessageItem;
		tpItem->mText = tTextBuffer;
		mItems.push_back( tpItem );
	}

	CGUISize tWindowSize = GetWindowSize( );
	CGUIPoint tTopLeft = WindowToScreen( GetWindowRect( ).mTopLeft );
	CGUISize tSize = CGUISystem::GetSingleton( ).GetViewportExt( );
	int tLeft = ( tSize.mCX - tWindowSize.mCX ) >> 1;
	int tTop = ( tSize.mCY - tWindowSize.mCY ) >> 1;
	MoveWindow( CGUIPoint( tLeft, tTop ) - tTopLeft );
}

void CGUIMessageBox::OnClickYes( CGUIWindow* pButton )
{
	CGUISinkCommand tCommand;
	tCommand.AddArg( CGUIDataType( GUI_PWINDOW, (int) this ) );
	tCommand.AddArg( CGUIDataType( GUI_INT, GUI_CMD_MESSAGEYES ) );
	FireEvent( &tCommand );

	// fire event
	Destroy( );
}

void CGUIMessageBox::OnClickNo( CGUIWindow* pButton )
{
	CGUISinkCommand tCommand;
	tCommand.AddArg( CGUIDataType( GUI_PWINDOW, (int) this ) );
	tCommand.AddArg( CGUIDataType( GUI_INT, GUI_CMD_MESSAGENO ) );
	FireEvent( &tCommand );

	// fire event
	Destroy( );
}

void CGUIMessageBox::OnWindowRender( )
{
	CGUIFrame::OnWindowRender( );
	
	int tFontHeight = mpFont->GetFontHeight( );
	CGUIClientCanvas tCanvas( this );
	for ( int i = 0; i < int( mItems.size( ) ); i ++ )
	{
		CGUIRect tItemRect( CGUIPoint( 6, 10 + i * ( tFontHeight + 5 ) ), CGUISize( 280, tFontHeight ) );
        tCanvas.PrintText( mItems[ i ]->mText.c_str( ), tItemRect, GUI_DT_VCENTER | GUI_DT_HCENTER, mTextColour );
	}
}

// ************************************************************ //
// CGUITab
// ************************************************************ //

CGUITab::~CGUITab( )
{
	for ( int i = 0; i < int( mItems.size( ) ); i ++ )
		delete mItems[i];
}

void CGUITab::OnMouseLButtonDown( const CGUIPoint& rPoint, unsigned int vFlags )
{
	for ( int i = 0; i < int( mItems.size( ) ); i ++ )
	{
		if ( mItems[i]->mRect.PtInRect( ScreenToClient( rPoint ) + mCamPoint ) )
		{
			SetCurSel( i );
			break;
		}
	}

	CGUIWindow::OnMouseLButtonDown( rPoint, vFlags );
}

void CGUITab::SetCurSel( int vIndex )
{
	unsigned int tOldIndex = 0xFFFFFFFF;
	for ( int i = 0; i < int( mItems.size( ) ); i ++ )
	{
		if ( mItems[ i ]->mIsSelected == true )
		{
			mItems[ i ]->mIsSelected = false;
			tOldIndex = i;
			break;
		}
	}
	mItems[ vIndex ]->mIsSelected = true;
	mCurSel = vIndex;

	CGUISinkTabSelChanged tSelChanged;
	tSelChanged.AddArg( CGUIDataType( GUI_PWINDOW, (unsigned int) this ) );
	tSelChanged.AddArg( CGUIDataType( GUI_INT, mCurSel ) );
	FireEvent( &tSelChanged );
	//fire event
}

void CGUITab::AppendItem( const char* pText )
{
	CTabItem* tpItem = new CTabItem( pText );
	int tTabCount = int( mItems.size( ) );
	int tTextHeight = mpFont->GetFontHeight( );
	int tTabBtnWidth = 10 * 2 + mpFont->GetTextWidth( pText, int( ::strlen( pText ) ) );
	int tTabLeft = 0;
	if ( tTabCount == 0 )
		tTabLeft = 5;
	else
		tTabLeft = mItems[tTabCount - 1]->mRect.mBottomRight.mX + GUI_TAB_INTERVAL_WIDTH;

	tpItem->mRect.SetRect( tTabLeft, mRect.Height( ) - tTextHeight, tTabLeft + tTabBtnWidth, mRect.Height( ) );
	mItems.push_back( tpItem );
	if ( tTabCount == 0 ) SetCurSel( 0 );
}

void CGUITab::OnDrawTab( CGUICanvas* pCanvas, const CGUIRect& rRect )
{
	pCanvas->DrawCombine( mpTexture, rRect, CGUIPoint( 0, 0 ), CGUISize( 15, 15 ) );
}

void CGUITab::OnDrawItem( CGUICanvas* pCanvas, const char* pText, const CGUIRect& rRect, bool vIsSelected )
{
	CGUIPoint tPoint;
	CGUIRect tTextRect = rRect;
	if ( vIsSelected == true )
	{
		tPoint		= CGUIPoint( 0, 45 );
		tTextRect	-= CGUIPoint( 1, 1 );
	}
	else
		tPoint		= CGUIPoint( 0, 90 );

	pCanvas->DrawCombine( mpTexture, rRect, tPoint, CGUISize( 15, 15 ) );
	pCanvas->PrintText( pText, tTextRect, GUI_DT_BOTTOM | GUI_DT_HCENTER, mTextColour );
}

void CGUITab::OnWindowSized( const CGUIPoint& rOffset )
{
	for ( int i = 0; i < int( mItems.size( ) ); i ++ )
		mItems[ i ]->mRect += CGUIPoint( 0, rOffset.mY );

	CGUIWindow::OnWindowSized( rOffset );
}

void CGUITab::OnWindowRender( ) 
{
	CGUIWindowCanvas tCanvas( this );
	CGUIRect tTabRect = GetClientRect( );
	tTabRect.DeflateRect( 0, 0, 0, tCanvas.GetFontHeight( ) - 2 );
	CTabItem* tpSelItem = NULL;
	for ( int i = 0; i < int( mItems.size( ) ); i ++ )
	{
		if ( mItems[i]->mIsSelected == true )
			tpSelItem = mItems[i];
		else
			OnDrawItem( &tCanvas, mItems[ i ]->mText.c_str( ), mItems[i]->mRect - mCamPoint, mItems[i]->mIsSelected );
	}

	OnDrawTab( &tCanvas, tTabRect );
	if ( tpSelItem != NULL )
		OnDrawItem( &tCanvas, tpSelItem->mText.c_str( ), tpSelItem->mRect - mCamPoint, tpSelItem->mIsSelected );
}

void CGUITab::OnLoadResource( )
{
	GUI_LOADTEXTURE( mpTexture, "CommonTab.png" )
}

void CGUITab::OnDestroyResource( )
{
	GUI_DESTROYTEXTURE( mpTexture )
}

// ************************************************************ //
// CGUIFileDialog
// ************************************************************ //

int CGUIFileDialog::OnWindowCreate( )
{
	if ( CGUIFrame::OnWindowCreate( ) == -1 )
		return -1;

	SetText( "文件选择对话框" );
	CGUIRect tRect = GetWindowRect( );
	CGUIRect tCaptionRect = GetCaptionRect( );
	CGUIWindowManager* tpWmgr = CGUIWindowManager::GetSingletonPtr( );
	int tFontHeight = mpFont->GetFontHeight( );
	int tTextWidth = mpFont->GetTextWidth( "文件名:", 7 );
	mpFileName = (CGUIEdit*) tpWmgr->Create( "GUIEdit", NULL, GUI_WS_TABSTOP | GUI_ES_SINGLELINE | GUI_ES_AUTOHSCROLL, CGUIRect( tTextWidth + 10, tCaptionRect.mBottomRight.mY + 5, tTextWidth + 185, tCaptionRect.mBottomRight.mY + tFontHeight + 14 ), this, NULL );
	mpFileList = (CGUIList*) tpWmgr->Create( "GUIList", NULL, GUI_WS_TABSTOP | GUI_LCS_LEFT | GUI_WS_HSCROLL | GUI_WS_VSCROLL, CGUIRect( 5, tCaptionRect.mBottomRight.mY + tFontHeight + 20, tRect.Width( ) - 5, tRect.Height( ) - 5 ), this, NULL );
	mpUpFolder = (CGUIButton*) tpWmgr->Create( "GUIButton", NULL, 0, CGUIRect( tTextWidth + 190, tCaptionRect.mBottomRight.mY + 7, tTextWidth + 255, tCaptionRect.mBottomRight.mY + tFontHeight + 13 ), this, NULL );
	mpOk = (CGUIButton*) tpWmgr->Create( "GUIButton", NULL, 0, CGUIRect( tRect.Width( ) - 70, tCaptionRect.mBottomRight.mY + 7, tRect.Width( ) - 5, tCaptionRect.mBottomRight.mY + tFontHeight + 13 ), this, NULL );
	mpUpFolder->SetText( "<-" );
	mpOk->SetText( "确定" );

	mpFileList->AppendColumn( "名称", 300, GUI_LCS_LEFT );
	mpFileList->AppendColumn( "大小", 50, GUI_LCS_RIGHT );
	mpFileList->AppendColumn( "类型", 80, GUI_LCS_LEFT );
	mpFileList->AppendColumn( "修改时间", 160, GUI_LCS_LEFT );

	ON_BN_CLICKED( mpUpFolder, OnUpDirectory )
	ON_BN_CLICKED( mpOk, OnClickOK )
	ON_LC_SELCHANGED( mpFileList, OnFileListSelChanged )
	ON_LC_DBLCLK( mpFileList, OnFileListDblClk )

	char tBuffer[ _MAX_PATH ] = { 0 };
	if( _getcwd( tBuffer, _MAX_PATH ) != NULL )
	{
        mCurrentPath = tBuffer;
		RefreshFileList( );
	}
	return 0;
}

void CGUIFileDialog::OnClickOK( CGUIWindow* pButton )
{
	std::string tFileName = GetFilePath( );
	__stat64 tStat;	::memset( &tStat, 0, sizeof( __stat64 ) ); 
	_stat64( tFileName.c_str( ), &tStat ); 
	if ( tStat.st_mode & _S_IFDIR )
	{
		mCurrentPath = tFileName;
		RefreshFileList( );
	}
	else
	{
		CGUISinkCommand tCommand;
		tCommand.AddArg( CGUIDataType( GUI_PWINDOW, (int) this ) );
		tCommand.AddArg( CGUIDataType( GUI_INT, GUI_CMD_FILEDIALOGCLOSED ) );
		FireEvent( &tCommand );
		Destroy( );
	}
}

std::string CGUIFileDialog::GetFileName( ) const
{
	return mpFileName->GetEditText( );
}

std::string CGUIFileDialog::GetFilePath( ) const
{
	std::string tText;
	if ( mCurrentPath.empty( ) )
	{
		tText += mpFileName->GetEditText( );
		tText += "\\";
	}
	else
	{
		tText = mCurrentPath;
		tText += "\\";
		tText += mpFileName->GetEditText( );
	}

	return tText;
}

void CGUIFileDialog::OnFileListSelChanged( CGUIWindow* pList, int vItemIndex )
{
	mpFileName->SetEditText( mFileList[ ( (CGUIList*) pList )->GetItemData( vItemIndex ) ].c_str( ) );
}

void CGUIFileDialog::OnFileListDblClk( CGUIWindow* pWindow, int vItemIndex )
{
	std::string tFilePath = GetFilePath( );
	__stat64 tStat; ::memset( &tStat, 0, sizeof( __stat64 ) ); 
	_stat64( tFilePath.c_str( ), &tStat ); 
	if ( tStat.st_mode & _S_IFDIR )
	{
		mCurrentPath = tFilePath;
		RefreshFileList( );
	}
}

void CGUIFileDialog::OnUpDirectory( CGUIWindow* pButton )
{
	for ( int i = int( mCurrentPath.size( ) ) - 1; i >= 0; i -- )
	{
		if ( mCurrentPath[ i ] == '\\' )
		{
			mCurrentPath.erase( mCurrentPath.begin( ) + i );
			break;
		}
        
		mCurrentPath.erase( mCurrentPath.begin( ) + i );
	}
	RefreshFileList( );
}

void CGUIFileDialog::RefreshFileList( )
{
	char tFilePath[ _MAX_PATH ] = { 0 };
	::strcat( tFilePath, mCurrentPath.c_str( ) );
	::strcat( tFilePath, "\\*.*" );
	mpFileName->ResetContext( );
	mpFileList->RemoveAllItems( );
	mFileList.clear( );

	int tItem = 0;
	char tDriverName[3] = { 0, ':', 0 };
	if ( mCurrentPath.empty( ) )
	{
		int tMaskBit = ::_getdrives( );
		for ( int i = 0; i < 26; i ++ )
		{
			if ( tMaskBit & ( int( pow( 2, i ) ) ) )
			{
				tDriverName[0] = 'A' + i;
				mFileList.push_back( tDriverName );
				tItem = mpFileList->AppendItem( tDriverName );
				mpFileList->SetItemData( tItem, int( mFileList.size( ) ) - 1 );
				mpFileList->SetItem( tItem, 2, "本地磁盘" );
			}
		}
	}
	else
	{
		__finddata64_t tFd64;
		intptr_t tFind = ::_findfirst64( tFilePath, &tFd64 );
		if ( tFind == -1 )
			return;

		for ( int tFindNext = 0; tFindNext == 0; tFindNext = ::_findnext64( tFind, &tFd64 ) )
		{
			if ( strcmp( tFd64.name, "." ) == 0 || strcmp( tFd64.name, ".." ) == 0 )
				continue;

			if ( !( tFd64.attrib & _A_SUBDIR ) )
			{
				tItem = mpFileList->AppendItem( tFd64.name );
				__int64 tFileSize = tFd64.size;
				char tText[ 255 ] = { 0 }; ::sprintf( tText, "%dK", tFileSize / 1024 );
				mpFileList->SetItem( tItem, 1, tText );
				mpFileList->SetItem( tItem, 2, "文件" );
			}
			else
			{
				std::string tFileName = "["; tFileName += tFd64.name; tFileName += "]";
				tItem = mpFileList->AppendItem( tFileName.c_str( ) );
				mpFileList->SetItem( tItem, 2, "目录" );
			}

			mFileList.push_back( tFd64.name );
			mpFileList->SetItemData( tItem, int( mFileList.size( ) ) - 1 );
			tm* tpCreateTm = _gmtime64( &tFd64.time_create );
			char tText[ 255 ] = { 0 }; ::sprintf( tText, "%d-%d-%d %d:%d:%d", 1900 + tpCreateTm->tm_year, tpCreateTm->tm_mon, tpCreateTm->tm_mday, tpCreateTm->tm_hour,
				tpCreateTm->tm_min, tpCreateTm->tm_sec );
			mpFileList->SetItem( tItem, 3, tText );
		}

		::_findclose( tFind );
	}
	mpFileList->SetCurSel( 0 );
}

void CGUIFileDialog::OnWindowRender( )
{
	CGUIFrame::OnWindowRender( );

	CGUIClientCanvas tCanvas( this );
	tCanvas.PrintText( "文件名:", CGUIRect( 5, 10, 100, 100 ), GUI_DT_LEFT | GUI_DT_TOP, mTextColour );

	CGUIRect tListRect = mpFileList->GetRect( ); tListRect.InflateRect( 1, 1 );
	tCanvas.DrawRect( tListRect - GetClientRect( ).mTopLeft, 0xFF000000, 0xFF808080 );
}

// ************************************************************ //
// CGUIColourDialog
// ************************************************************ //
void CGUIColourDialog::OnLoadResource( )
{
	CGUIRenderDriver* tpRenderDriver = CGUISystem::GetSingletonPtr( )->GetRenderDriver( );
	if ( mp2DTexture == NULL )
	{
		mp2DTexture = tpRenderDriver->CreateTexture( "Colour2D", 256, 256 );
		unsigned int* pColour = mp2DTexture->Lock( );
		for ( int x = 0; x < 256; x ++ )
			for ( int y = 0; y < 256; y ++ )
				pColour[ y * 256 + x ] = HSLToRGB( GUI_MAKECOLOUR( 255, x, 255 - y, 120 ) );
		mp2DTexture->Unlock( );
	}

	if ( mpBrightTexture == NULL )
	{
		mpBrightTexture = tpRenderDriver->CreateTexture( "Brightness", 32, 256 );
		FillTexture( );
	}
	CGUIFrame::OnLoadResource( );
}

void CGUIColourDialog::OnDestroyResource( )
{
	GUI_DESTROYTEXTURE( mp2DTexture );
	GUI_DESTROYTEXTURE( mpBrightTexture );
	CGUIFrame::OnDestroyResource( );
}

int CGUIColourDialog::OnWindowCreate( )
{
	if ( CGUIFrame::OnWindowCreate( ) == -1 )
		return -1;
	SetText( "颜色选择对话框" );

	CGUIClientCanvas tCanvas( this );
	int tTextVertInterval = tCanvas.GetFontHeight( ) + 10;
	CGUIWindowManager* tpWmgr = CGUIWindowManager::GetSingletonPtr( );
	mpHEdit = (CGUIEdit*) tpWmgr->Create( "GUIEdit", NULL, GUI_WS_TABSTOP | GUI_ES_SINGLELINE | GUI_ES_AUTOHSCROLL, CGUIRect( 150, 290, 180, 290 + tTextVertInterval - 2 ), this, NULL );
	mpSEdit = (CGUIEdit*) tpWmgr->Create( "GUIEdit", NULL, GUI_WS_TABSTOP | GUI_ES_SINGLELINE | GUI_ES_AUTOHSCROLL, CGUIRect( 150, 290 + tTextVertInterval, 180, 290 + tTextVertInterval * 2 - 2 ), this, NULL );
	mpLEdit = (CGUIEdit*) tpWmgr->Create( "GUIEdit", NULL, GUI_WS_TABSTOP | GUI_ES_SINGLELINE | GUI_ES_AUTOHSCROLL, CGUIRect( 150, 290 + tTextVertInterval * 2, 180, 290 + tTextVertInterval * 3 - 2 ), this, NULL );

	mpREdit = (CGUIEdit*) tpWmgr->Create( "GUIEdit", NULL, GUI_WS_TABSTOP | GUI_ES_SINGLELINE | GUI_ES_AUTOHSCROLL, CGUIRect( 247, 290, 296, 290 + tTextVertInterval - 2 ), this, NULL );
	mpGEdit = (CGUIEdit*) tpWmgr->Create( "GUIEdit", NULL, GUI_WS_TABSTOP | GUI_ES_SINGLELINE | GUI_ES_AUTOHSCROLL, CGUIRect( 247, 290 + tTextVertInterval, 296, 290 + tTextVertInterval * 2 - 2 ), this, NULL );
	mpBEdit = (CGUIEdit*) tpWmgr->Create( "GUIEdit", NULL, GUI_WS_TABSTOP | GUI_ES_SINGLELINE | GUI_ES_AUTOHSCROLL, CGUIRect( 247, 290 + tTextVertInterval * 2, 296, 290 + tTextVertInterval * 3 - 2 ), this, NULL );

	mpOkBtn = (CGUIButton*) tpWmgr->Create( "GUIButton", NULL, 0, CGUIRect( 199, 380, 245, 405 ), this, NULL );
	mpCancelBtn = (CGUIButton*) tpWmgr->Create( "GUIButton", NULL, 0, CGUIRect( 250, 380, 296, 405 ), this, NULL );

	mpHEdit->SetEditText( "0" );
	mpSEdit->SetEditText( "0" );
	mpLEdit->SetEditText( "0" );
	mpREdit->SetEditText( "0" );
	mpGEdit->SetEditText( "0" );
	mpBEdit->SetEditText( "0" );
	
	mpOkBtn->SetText( "确定" );
	mpCancelBtn->SetText( "取消" );

	ON_BN_CLICKED( mpOkBtn, OnOk );
	ON_BN_CLICKED( mpCancelBtn, OnCancel );
	ON_EN_CHANGED( mpHEdit, OnHEditChanged );
	ON_EN_CHANGED( mpSEdit, OnSEditChanged );
	ON_EN_CHANGED( mpLEdit, OnLEditChanged );
	return 0;
}

void CGUIColourDialog::OnOk( CGUIWindow* pWindow )
{
	CGUISinkCommand tCommand;
	tCommand.AddArg( CGUIDataType( GUI_PWINDOW, (int) this ) );
	tCommand.AddArg( CGUIDataType( GUI_INT, GUI_CMD_COLOURDIALOGCLOSED ) );
	FireEvent( &tCommand );
	Destroy( );
}

void CGUIColourDialog::OnHEditChanged( CGUIWindow* pEdit )
{
	CGUIEdit* tpEdit = (CGUIEdit*) pEdit;
	mCurH = atoi( tpEdit->GetEditText( ).c_str( ) );
	PrintRGBValue( );
}

void CGUIColourDialog::OnSEditChanged( CGUIWindow* pEdit )
{
	CGUIEdit* tpEdit = (CGUIEdit*) pEdit;
	mCurS = atoi( tpEdit->GetEditText( ).c_str( ) );
	PrintRGBValue( );
}

void CGUIColourDialog::OnLEditChanged( CGUIWindow* pEdit )
{
	CGUIEdit* tpEdit = (CGUIEdit*) pEdit;
	mCurL = atoi( tpEdit->GetEditText( ).c_str( ) );
	PrintRGBValue( );
}

void CGUIColourDialog::OnWindowRender( )
{
	CGUIFrame::OnWindowRender( );

	CGUIClientCanvas tCanvas( this );
	int tTextVertInterval = tCanvas.GetFontHeight( ) + 10;
	tCanvas.DrawRect( CGUIRect( 5, 5, 263, 263 ), 0xFF000000, 0xFF808080 );
	tCanvas.DrawRect( CGUIRect( 270, 5, 292, 263 ), 0xFF000000, 0xFF808080 );
	tCanvas.DrawRect( CGUIRect( 5, 270, 5 + tCanvas.GetTextWidth( "颜色|纯色", 9 ) + 2, 312 ), 0xFF000000, 0xFF808080 );

	tCanvas.BitBlt( mp2DTexture, CGUIRect( 0, 0, 256, 256 ), CGUIRect( 6, 6, 262, 262 ), 0xFFFFFFFF );
	tCanvas.BitBlt( mpBrightTexture, CGUIRect( 0, 0, 20, 256 ), CGUIRect( 271, 6, 291, 262 ), 0xFFFFFFFF );
	tCanvas.FillRect( CGUIRect( 6, 271, 5 + tCanvas.GetTextWidth( "颜色|纯色", 9 ), 311 ), HSLToRGB( GUI_MAKECOLOUR( 255, mCurH, mCurS, mCurL ) ) );
	tCanvas.PrintText( "颜色|纯色", CGUIRect( 4, 313, 100, 340 ), GUI_DT_LEFT | GUI_DT_TOP, mTextColour );

	tCanvas.PrintText( "色调", CGUIRect( 90, 271, 100, 340 ), GUI_DT_LEFT | GUI_DT_TOP, mTextColour );
	tCanvas.PrintText( "饱和度", CGUIRect( 90, 271 + tTextVertInterval, 100, 340 ), GUI_DT_LEFT | GUI_DT_TOP, mTextColour );
	tCanvas.PrintText( "亮度", CGUIRect( 90, 271 + tTextVertInterval * 2, 100, 340 ), GUI_DT_LEFT | GUI_DT_TOP, mTextColour );

	tCanvas.PrintText( "红(R)", CGUIRect( 200, 271, 292, 340 ), GUI_DT_LEFT | GUI_DT_TOP, mTextColour );
	tCanvas.PrintText( "绿(G)", CGUIRect( 200, 271 + tTextVertInterval, 292, 340 ), GUI_DT_LEFT | GUI_DT_TOP, mTextColour );
	tCanvas.PrintText( "蓝(B)", CGUIRect( 200, 271 + tTextVertInterval * 2, 292, 340 ), GUI_DT_LEFT | GUI_DT_TOP, mTextColour );

	int tSaturation = 255 - mCurS;
	tCanvas.DrawLine( CGUIPoint( mCurH + 3, tSaturation ) + CGUIPoint( 5, 5 ), CGUIPoint( mCurH + 8, tSaturation ) + CGUIPoint( 5, 5 ), 0xFF000000 );
	tCanvas.DrawLine( CGUIPoint( mCurH + 3, tSaturation + 1 ) + CGUIPoint( 5, 5 ), CGUIPoint( mCurH + 8, tSaturation + 1 ) + CGUIPoint( 5, 5 ), 0xFF000000 );

	tCanvas.DrawLine( CGUIPoint( mCurH - 3, tSaturation ) + CGUIPoint( 5, 5 ), CGUIPoint( mCurH - 8, tSaturation ) + CGUIPoint( 5, 5 ), 0xFF000000 );
	tCanvas.DrawLine( CGUIPoint( mCurH - 3, tSaturation + 1 ) + CGUIPoint( 5, 5 ), CGUIPoint( mCurH - 8, tSaturation + 1 ) + CGUIPoint( 5, 5 ), 0xFF000000 );

	tCanvas.DrawLine( CGUIPoint( mCurH, tSaturation + 3 ) + CGUIPoint( 5, 5 ), CGUIPoint( mCurH, tSaturation + 8 ) + CGUIPoint( 5, 5 ), 0xFF000000 );
	tCanvas.DrawLine( CGUIPoint( mCurH + 1, tSaturation + 3 ) + CGUIPoint( 5, 5 ), CGUIPoint( mCurH + 1, tSaturation + 8 ) + CGUIPoint( 5, 5 ), 0xFF000000 );

	tCanvas.DrawLine( CGUIPoint( mCurH, tSaturation - 3 ) + CGUIPoint( 5, 5 ), CGUIPoint( mCurH, tSaturation - 8 ) + CGUIPoint( 5, 5 ), 0xFF000000 );
	tCanvas.DrawLine( CGUIPoint( mCurH + 1, tSaturation - 3 ) + CGUIPoint( 5, 5 ), CGUIPoint( mCurH + 1, tSaturation - 8 ) + CGUIPoint( 5, 5 ), 0xFF000000 );

	int tBrightness = 255 - mCurL;
	tCanvas.DrawLine( CGUIPoint( 265, tBrightness ) + CGUIPoint( 0, 5 ), CGUIPoint( 269, tBrightness ) + CGUIPoint( 0, 5 ), 0xFFFFFFFF );
	tCanvas.DrawLine( CGUIPoint( 265, tBrightness + 1 ) + CGUIPoint( 0, 5 ), CGUIPoint( 269, tBrightness + 1 ) + CGUIPoint( 0, 5 ), 0xFFFFFFFF );

	tCanvas.DrawLine( CGUIPoint( 292, tBrightness ) + CGUIPoint( 0, 5 ), CGUIPoint( 296, tBrightness ) + CGUIPoint( 0, 5 ), 0xFFFFFFFF );
	tCanvas.DrawLine( CGUIPoint( 292, tBrightness + 1 ) + CGUIPoint( 0, 5 ), CGUIPoint( 296, tBrightness + 1 ) + CGUIPoint( 0, 5 ), 0xFFFFFFFF );
}

unsigned int CGUIColourDialog::HSLToRGB( unsigned int vColourHSL ) 
{
	static float tHSLRange = 255.0f, tRGBRange = 255.0f;
	float tH = float( int( ( vColourHSL & 0x00FF0000 ) >> 16 ) );
	float tS = float( int( ( vColourHSL & 0x0000FF00 ) >> 8 ) );
	float tL = float( int( vColourHSL & 0x000000FF ) );
	float tR, tG, tB;

	int a = int( ( vColourHSL & 0x0000FF00 ) >> 8 );
	tH *= tRGBRange * 6.0 / tHSLRange;
	tS *= tRGBRange / tHSLRange;
	tL *= tRGBRange / tHSLRange;

	tR = GUI_MIN( tRGBRange, GUI_MAX( 0.0, fabs( 3.0 * tRGBRange - fmod( tH, 6.0f * tRGBRange ) ) - tRGBRange ) );
	tG = GUI_MIN( tRGBRange, GUI_MAX( 0.0, fabs( 3.0 * tRGBRange - fmod( tH + 4.0f * tRGBRange, 6.0f * tRGBRange ) ) - tRGBRange ) );
	tB = GUI_MIN( tRGBRange, GUI_MAX(0.0, fabs( 3.0 * tRGBRange - fmod( tH + 2.0f * tRGBRange, 6.0f * tRGBRange ) ) - tRGBRange ) );

	if ( tL > tRGBRange / 2 )
	{
		tR = ( tL * 2 / tRGBRange - 1 ) * ( tRGBRange - tR ) + tR;
		tG = ( tL * 2 / tRGBRange - 1 ) * ( tRGBRange - tG ) + tG;
		tB = ( tL * 2 / tRGBRange - 1 ) * ( tRGBRange - tB ) + tB;
	}
	else 
	{
		tR *= tL * 2 / tRGBRange;
		tG *= tL * 2 / tRGBRange;
		tB *= tL * 2 / tRGBRange;
	}

	double tAverage = ( tR + tG + tB ) / 3.0f;
	tR = ( 1.0 - tS / tRGBRange ) * ( tAverage - tR ) + tR;
	tG = ( 1.0 - tS / tRGBRange ) * ( tAverage - tG ) + tG;
	tB = ( 1.0 - tS / tRGBRange ) * ( tAverage - tB ) + tB;
	return GUI_MAKECOLOUR( 255, int( tR ), int( tG ), int( tB ) );
}

void CGUIColourDialog::OnMouseLButtonUp( const CGUIPoint& rPoint, unsigned int vFlag )
{
	CGUISystem::GetSingleton( ).ReleaseCapture( this );
	mClickArea = 0;
	CGUIFrame::OnMouseLButtonUp( rPoint, vFlag );
}

void CGUIColourDialog::OnMouseLButtonDown( const CGUIPoint& rPoint, unsigned int vFlag )
{
	CGUIRect t2DRect( 5, 5, 263, 263 );
	CGUIRect tBrightRect( 270, 5, 292, 263 );
	CGUIPoint tPoint = ScreenToClient( rPoint );
	if ( t2DRect.PtInRect( tPoint ) )
	{
		mClickArea = 1;
		CGUISystem::GetSingleton( ).SetCapture( this );
	}

	if ( tBrightRect.PtInRect( tPoint ) )
	{
		mClickArea = 2;
		CGUISystem::GetSingleton( ).SetCapture( this );
	}

	UpdateColour( rPoint );
	CGUIFrame::OnMouseLButtonDown( rPoint, vFlag );
}

void CGUIColourDialog::UpdateColour( const CGUIPoint& rPoint )
{
	if ( mClickArea == 1 )
	{
		CGUIRect t2DRect( 5, 5, 263, 263 );
		CGUIPoint tPoint = ScreenToClient( rPoint );
		mCurH = GUI_MAX( 0, tPoint.mX - t2DRect.mTopLeft.mX );
		mCurH = GUI_MIN( 255, mCurH );
		mCurS = GUI_MAX( 0, 255 - ( tPoint.mY - t2DRect.mTopLeft.mY ) );
		mCurS = GUI_MIN( 255, mCurS );
		char tText[ 255 ];	
		::sprintf( tText, "%d", mCurH ); mpHEdit->SetEditText( tText );
		::sprintf( tText, "%d", mCurS ); mpSEdit->SetEditText( tText );
		PrintRGBValue( );
		FillTexture( );
	}

	if ( mClickArea == 2 )
	{
		CGUIRect tBrightRect( 270, 5, 292, 263 );
		CGUIPoint tPoint = ScreenToClient( rPoint );
		mCurL = GUI_MAX( 0, 255 - ( tPoint.mY - tBrightRect.mTopLeft.mY ) );
		mCurL = GUI_MIN( 255, mCurL );
		char tText[ 255 ];	
		::sprintf( tText, "%d", mCurL ); mpLEdit->SetEditText( tText );
		PrintRGBValue( );
		FillTexture( );
	}
}

void CGUIColourDialog::OnMouseMove( const CGUIPoint& rPoint, unsigned int vFlag )
{
	UpdateColour( rPoint );
	CGUIFrame::OnMouseMove( rPoint, vFlag );
}

unsigned int CGUIColourDialog::GetRGBColour( )
{
	return HSLToRGB( GUI_MAKECOLOUR( 255, mCurH, mCurS, mCurL ) );
}

void CGUIColourDialog::PrintRGBValue( )
{
	char tText[ 255 ];
	unsigned int tColour = HSLToRGB( GUI_MAKECOLOUR( 255, mCurH, mCurS, mCurL ) );
	int tR = ( tColour & 0x00FF0000 ) >> 16;
	int tG = ( tColour & 0x0000FF00 ) >> 8;
	int tB = ( tColour & 0x000000FF );
	::sprintf( tText, "%d", tR ); mpREdit->SetEditText( tText );
	::sprintf( tText, "%d", tG ); mpGEdit->SetEditText( tText );
	::sprintf( tText, "%d", tB ); mpBEdit->SetEditText( tText );
}

void CGUIColourDialog::FillTexture( )
{
	unsigned int* pColour = mpBrightTexture->Lock( );
	for ( int x = 0; x < 20; x ++ )
		for ( int y = 0; y < 256; y ++ )
			pColour[ y * 32 + x ] = HSLToRGB( GUI_MAKECOLOUR( 255, mCurH, mCurS, 255 - y ) );

	mpBrightTexture->Unlock( );
}

// ************************************************************ //
// CGUIStatic
// ************************************************************ //

void CGUIStatic::Clear( )
{
	mTexts.clear( );
	RefreshHyper( );
}

int CGUIStatic::GetColor( const char* pText, int vIndex, unsigned int& rColor )
{
	rColor = 0;
	int tAlpha, tRed, tGreen, tBlue;
	vIndex = GetValue( pText, vIndex, tAlpha );
	vIndex = GetValue( pText, vIndex, tRed );
	vIndex = GetValue( pText, vIndex, tGreen );
	vIndex = GetValue( pText, vIndex, tBlue );
	rColor |= tAlpha << 24;
	rColor |= tRed << 16;
	rColor |= tGreen << 8;
	rColor |= tBlue;
	return vIndex;
}

int CGUIStatic::ClearPrefix( const char* pText, int vIndex )
{
	if ( pText[vIndex] == '<' && ( pText[vIndex + 1] == 'P' || pText[vIndex + 1] == 'p' ) )
		return vIndex + 3;

	return vIndex;
}

int CGUIStatic::ColorPrefix( const char* pText, int vIndex )
{
	if ( pText[vIndex] == '<' && ( pText[vIndex + 1] == 'C' || pText[vIndex + 1] == 'c' ) )
		return vIndex + 2;

	return vIndex;
}

int CGUIStatic::GetValue( const char* pText, int vIndex, int& rValue )
{
	char tValue[8] = { 0 };
	int i = 0;
	for ( i = 0; i < 8; i ++ )
	{
		if ( pText[vIndex + i] == ',' || pText[vIndex + i] == '>' || pText[vIndex +i] == ':' )
			break;

		tValue[ i ] = pText[ vIndex + i ];
	}
	rValue = ::atoi( tValue );

	return vIndex + i + 1;
}

int CGUIStatic::ColorEndmark( const char* pText, int vIndex )
{
	if ( pText[vIndex] == '<' && pText[vIndex + 1] == '/' && ( pText[vIndex + 2] == 'c' || pText[vIndex + 2] == 'C' ) && pText[vIndex + 3] == '>' )
		return vIndex + 4;

	return vIndex;
}

int CGUIStatic::LinkPrefix( const char* pText, int vIndex )
{
	if ( pText[vIndex] == 'L' && pText[vIndex + 1] == 'i' && pText[vIndex + 2] == 'n' && pText[vIndex + 3] == 'k' && pText[vIndex + 4] == ':' )
		return vIndex + 5;

	return vIndex;
}

int CGUIStatic::GetLinkInfo( const char* pText, int vIndex, char* pOpr, int& rData1, int& rData2 )
{
	int tTextIndex = 0;

	// 得到操作符
	while( pText[ vIndex ] != ' ' )	
		pOpr[ tTextIndex++ ] = pText[ vIndex++ ];

	vIndex = GetValue( pText, vIndex + 1, rData1 );
	vIndex = GetValue( pText, vIndex + 1, rData2 );
	return vIndex;
}

void CGUIStatic::Printf( const char* pText )
{
	// 预处理字符串, 在文本上作替换, 替换'\t'和一些别的东西
	std::string tTextPreprocessed;
	int tIndex = -1;
	while ( pText[ ++ tIndex ] != 0 )
	{
		if ( pText[ tIndex ] == '\t' )
		{
			for ( int i = 0; i < 4; i ++ )
			{
				tTextPreprocessed.push_back( ' ' );
				continue;
			}
		}

		tTextPreprocessed.push_back( pText[ tIndex ] );
	}

	const char* tpText = tTextPreprocessed.c_str( );
	tIndex = 0;
	// 清除标记要在最前面，后面的清除标记将忽略
	int tNewIndex = ClearPrefix( tpText, tIndex );
	if ( tNewIndex != tIndex )
	{
		mTexts.clear( );
		tIndex = tNewIndex;
	}

	CStaticItem* tpHyperText = NULL;
	// 如果没有颜色头,建立一个默认的
	if ( ColorPrefix( pText, tIndex ) == tIndex )
		tpHyperText = new CStaticItem;

	while ( tpText[ tIndex ] != 0 )
	{
		tNewIndex = ColorPrefix( tpText, tIndex );
		// 如果有颜色头,并且有一个超文本
		if ( tNewIndex != tIndex && tpHyperText != NULL )
		{
			// 保存该超文本
			mTexts.push_back( *tpHyperText );
			delete tpHyperText;
			tpHyperText = NULL;
		}
	
		// 如果有颜色头,没有一个超文本
		if ( tNewIndex != tIndex && tpHyperText == NULL )
		{
			tpHyperText = new CStaticItem;
			tIndex = GetColor( tpText, tNewIndex, tpHyperText->mColour );
			// 如果后面有link, 读取link的信息
			int tNewIndex = LinkPrefix( tpText, tIndex );
			if ( tNewIndex != tIndex )
				tIndex = GetLinkInfo( tpText, tNewIndex, tpHyperText->mOpr, tpHyperText->mData1, tpHyperText->mData2 );
		}

		tNewIndex = ColorEndmark( tpText, tIndex );
		if ( tNewIndex != tIndex )
		{
			// 找到颜色标记结束, 并且有文本开始头
			tIndex = tNewIndex;
			// 如果后面描述了Link,那么结束本次循环
			if ( LinkPrefix( tpText, tIndex ) != tIndex )
				continue;

			mTexts.push_back( *tpHyperText );
			delete tpHyperText;
			tpHyperText = NULL;
			// 如果后面紧跟着颜色开始头，那么结束本次循环
			if ( ColorPrefix( tpText, tIndex ) != tIndex )
				continue; 

			// 如果后面还有的话, 建立一个新的超文本
			if ( tpText[ tIndex ] != 0 )
                tpHyperText = new CStaticItem;
		}

		if ( tpHyperText != NULL )
            tpHyperText->mText.push_back( tpText[ tIndex ++ ] );
	}	

	if ( tpHyperText != NULL )
	{
		mTexts.push_back( *tpHyperText );
		delete tpHyperText;
	}

	RefreshHyper( );
}

void CGUIStatic::RefreshHyper( )
{
	CGUIRect tClientRect = GetClientRect( );
	int tFontHeight		= mpFont->GetFontHeight( );
	int tMaxWidth		= 0;
	int tMaxHeight		= tFontHeight;
	int tLogicWidth		= 0; 
	int tLogicHeight	= tFontHeight;
	CGUIRect tRect( 0, 0, 0, tFontHeight );
	for ( int i = 0; i < int( mTexts.size( ) ); i ++ )
	{
		CStaticItem* tpText	= &mTexts[ i ];
		tpText->mRects.clear( );
		tRect.mTopLeft.mX = tRect.mBottomRight.mX;
		for ( int tTextIndex = 0; tTextIndex < int( tpText->mText.size( ) ); )
		{
			char tText[ 3 ] = { 0 };
			int tTextCount = 0;
			if ( tpText->mText[ tTextIndex ] & 0x80 )
			{
				tText[ 0 ] = tpText->mText[ tTextIndex ];
				tText[ 1 ] = tpText->mText[ tTextIndex + 1 ];
				tTextCount = 2;
			}
			else
			{
				tText[ 0 ] = tpText->mText[ tTextIndex ];
				tTextCount = 1;
			}

			if ( tpText->mText[ tTextIndex ] == '\r' && tpText->mText[ tTextIndex + 1 ] == '\n' )
			{
				if ( tTextIndex != 0 )
                    tpText->mRects.push_back( tRect );

				tRect.SetRect( 0, tLogicHeight, 0, tLogicHeight + tFontHeight );
				tMaxWidth		= GUI_MAX( tMaxWidth, tLogicWidth );
				tMaxHeight		+= tFontHeight;
				tLogicHeight	+= tFontHeight;
				tTextIndex		+= 2;
				tLogicWidth		= 0;
				continue;
			}

			if ( tLogicWidth + mpFont->GetTextWidth( tText, tTextCount ) >= tClientRect.Width( ) )
			{
				tpText->mRects.push_back( tRect );
				tRect.SetRect( 0, tLogicHeight, 0, tLogicHeight + tFontHeight );
				tMaxWidth		= GUI_MAX( tMaxWidth, tLogicWidth );
				tMaxHeight		+= tFontHeight;
				tLogicHeight	+= tFontHeight;
				tLogicWidth		= 0;
			}
			tLogicWidth += mpFont->GetTextWidth( tText, tTextCount );
			tRect.mBottomRight.mX = tLogicWidth;
			tTextIndex	+= tTextCount;
		}
		if ( tRect.Width( ) != 0 )
            tpText->mRects.push_back( tRect );
	}

	if ( UpdateScrollBar( tMaxWidth, tMaxHeight ) )
		RefreshHyper( );
	mpVScrollBar->SetPosition( tMaxHeight );
}

void CGUIStatic::OnWindowRender( )
{
	CGUIClientCanvas tCanvas( this );
	CGUIPoint tStartPoint;
	for ( int i = 0; i < int( mTexts.size( ) ); i ++ )
		OnDrawItem( &tCanvas, tStartPoint, &mTexts[ i ] );
}

CGUIStatic::CStaticItem* CGUIStatic::HitTest( const CGUIPoint& rPoint )
{
	CGUIRect tClientRect = GetClientRect( );
	for ( int i = 0; i < int( mTexts.size( ) ); i ++ )
	{
		CStaticItem* tpText = &mTexts[i];
		for ( int j = 0; j < int ( tpText->mRects.size( ) ); j ++ )
		{
			if ( tpText->mRects[ j ].PtInRect( rPoint + mCamPoint - tClientRect.mTopLeft ) )
				return tpText;
		}
	}

	return NULL;
}

void CGUIStatic::OnMouseMove( const CGUIPoint& rPoint, unsigned int vFlags )
{
	CStaticItem* tpHitHyper = HitTest( rPoint );
	for ( int i = 0; i < int( mTexts.size( ) ); i ++ )
	{
		CStaticItem* tpText = &mTexts[i];
		tpText->mIsHover = false;
	}

	if ( tpHitHyper != NULL && tpHitHyper->mOpr[0] != 0 )
        tpHitHyper->mIsHover = true;
	
	CGUIWindow::OnMouseMove( rPoint,vFlags );
}

void CGUIStatic::OnMouseLButtonDown( const CGUIPoint& rPoint, unsigned int vFlags )
{
	CStaticItem* tpHitHyper = HitTest( rPoint );
	for ( int i = 0; i < int( mTexts.size( ) ); i ++ )
	{
		CStaticItem* tpText = &mTexts[i];
		tpText->mIsClicked = false;
	}

	if ( tpHitHyper != NULL && tpHitHyper->mOpr[0] != 0 )
        tpHitHyper->mIsClicked = true;

	CGUIWindow::OnMouseLButtonDown( rPoint,vFlags );
}

void CGUIStatic::OnMouseLButtonUp( const CGUIPoint& rPoint, unsigned int vFlags )
{
	CStaticItem* tpHitHyper = HitTest( rPoint );
	for ( int i = 0; i < int( mTexts.size( ) ); i ++ )
	{
		CStaticItem* tpText = &mTexts[i];
		tpText->mIsClicked = false;
	}

	if ( tpHitHyper != NULL )
		ExecuteLink( tpHitHyper );
	CGUIWindow::OnMouseLButtonDown( rPoint,vFlags );
}

void CGUIStatic::OnDrawItem( CGUICanvas* pCanvas, CGUIPoint& rStartPoint, const CStaticItem* tpHyper )
{
	CGUISize tClientSize = GetClientRect( ).Size( );
	int tFontHeight = pCanvas->GetFontHeight( );
	if ( tpHyper->mIsHover )
	{
		for ( int j = 0; j < int( tpHyper->mRects.size( ) ); j ++ )
		{
			const CGUIRect& rRect = tpHyper->mRects[j];
			pCanvas->DrawRect( rRect - mCamPoint, 0xFFFFFFFF, 0xFFFFFFFF );
		}
	}

	if ( tpHyper->mIsClicked )
	{
		for ( int j = 0; j < int( tpHyper->mRects.size( ) ); j ++ )
		{
			const CGUIRect& rRect = tpHyper->mRects[j];
			pCanvas->DrawRect( rRect - mCamPoint, 0xFF00FFFF, 0xFF00FFFF );
		}
	}

	for ( int tTextIndex = 0; tTextIndex < int( tpHyper->mText.size( ) ); )
	{
		char tText[ 3 ] = { 0 };
		int tTextCount = 0;
		if ( tpHyper->mText[ tTextIndex ] & 0x80 )
		{
			tText[ 0 ] = tpHyper->mText[ tTextIndex ];
			tText[ 1 ] = tpHyper->mText[ tTextIndex + 1 ];
			tTextCount = 2;
		}
		else
		{
			tText[ 0 ] = tpHyper->mText[ tTextIndex ];
			tTextCount = 1;
		}

		if ( tpHyper->mText[ tTextIndex ] == '\r' && tpHyper->mText[ tTextIndex + 1 ] == '\n' )
		{
			tTextIndex += 2;
			rStartPoint = CGUIPoint( 0, rStartPoint.mY + tFontHeight );
			continue;
		}

		if ( rStartPoint.mX + pCanvas->GetTextWidth( tText, tTextCount ) >= tClientSize.mCX )
			rStartPoint = CGUIPoint( 0, rStartPoint.mY + tFontHeight );
		pCanvas->PrintText( tText, CGUIRect( rStartPoint, CGUISize( 20, 20 ) ) - mCamPoint, GUI_DT_LEFT | GUI_DT_TOP, tpHyper->mColour );
		rStartPoint	+= CGUISize( pCanvas->GetTextWidth( tText, tTextCount ), 0 );
		tTextIndex	+= tTextCount;
	}
}

void CGUIStatic::ExecuteLink( const CStaticItem* tpHyper )
{
}

// ************************************************************ //
// CGUIDebug
// ************************************************************ //

int CGUIDebug::OnWindowCreate( )
{
	if ( CGUIFrame::OnWindowCreate( ) == -1 )
		return -1;

	CGUIRect tClientRect = GetClientRect( );
	CGUIRect tEditRect		= tClientRect;
	CGUIRect tTabRect		= tClientRect;
	tEditRect.mTopLeft.mY = tEditRect.mBottomRight.mY - 25;
	tEditRect.mBottomRight.mX = tEditRect.mBottomRight.mX;
	tTabRect.mTopLeft.mY = tTabRect.mTopLeft.mY + 3;
	tTabRect.mBottomRight.mY = tEditRect.mTopLeft.mY - 5;

	CGUIRect tStaticRect = CGUIRect( CGUIPoint( 4, 2 ), tTabRect.Size( ) - CGUISize( 8, 22 ) );
	CGUIWindowManager* tpWmgr = CGUIWindowManager::GetSingletonPtr( );
	mpDebugEdit		= (CGUIEdit*) tpWmgr->Create( "GUIEdit", NULL, GUI_ES_AUTOHSCROLL | GUI_ES_SINGLELINE, tEditRect, this, NULL );
	mpDebugTab		= (CGUITab*) tpWmgr->Create( "GUITab", NULL, 0, tTabRect, this, NULL );
	mpDebugStatic	= (CGUIStatic*) tpWmgr->Create( "GUIStatic", NULL, NULL, tStaticRect, mpDebugTab, NULL );

	mpDebugEdit->SetArchor( GUI_RIGHTARCHOR_RIGHT | GUI_BOTTOMARCHOR_BOTTOM | GUI_TOPARCHOR_BOTTOM );
	mpDebugTab->SetArchor( GUI_RIGHTARCHOR_RIGHT | GUI_BOTTOMARCHOR_BOTTOM );
	mpDebugStatic->SetArchor( GUI_RIGHTARCHOR_RIGHT | GUI_BOTTOMARCHOR_BOTTOM );
	mpDebugTab->AppendItem( "日志" );
	mpDebugTab->AppendItem( "内存" );
	mpDebugTab->AppendItem( "异常" );

	ON_TC_SELCHANGED( mpDebugTab, OnTabSelectChanged )
	mpDebugTab->SetCurSel( 0 );
	return 0;
}

void CGUIDebug::Refresh( const char* pDebugText, int vDebugType )
{
	if ( mpDebugTab->GetCurSel( ) == vDebugType )
		mpDebugStatic->Printf( pDebugText );
}

void CGUIDebug::OnTabSelectChanged( CGUIWindow* pTabWindow, int vCurSel )
{
	mpDebugStatic->Clear( );
	CGUISystem* tpSystem = CGUISystem::GetSingletonPtr( );
	if ( vCurSel == 0 )
	{
		for ( int i = 0; i < int( tpSystem->mLogs.size( ) ); i ++ )
			mpDebugStatic->Printf( tpSystem->mLogs[ i ].c_str( ) );
	}
	
	if ( vCurSel == 1 )
	{
		int tCount = CGUIMemoryManager::GetAllocCount( );
		mpDebugStatic->Printf( "<C255,0,255,0>内存分配列表:</C>\r\n" );
		mpDebugStatic->Printf( "<C255,0,255,0>-------------------------------</C>\r\n" );
		mpDebugStatic->Printf( "<C255,0,255,255>格式: 地址\t长度\t文件:行号</C>\r\n\r\n" );
		for ( int i = 0; i < tCount; i ++ )
		{
			char tBuffer[ 256 ] = { 0 };
			int tAddress	= CGUIMemoryManager::GetAllocAddress( i );
			int tSize		= CGUIMemoryManager::GetAllocSize( i );
			int tLineNumber	= CGUIMemoryManager::GetAllocLineNumber( i );
			std::string tFileName = CGUIMemoryManager::GetAllocFile( i );
			::sprintf( tBuffer, "<C255,255,192,0>0x%x\t%u\t%s : %u\r\n</C>", tAddress, tSize, tFileName.c_str( ), tLineNumber );
			mpDebugStatic->Printf( tBuffer );
		}

		char tBuffer[ 256 ] = { 0 };
		mpDebugStatic->Printf( "\r\n<C255,0,255,0>-------------------------------</C>\r\n" );
		::sprintf( tBuffer, "<C255,0,255,0>一共有%d次分配, 总共%dK</C>\r\n", tCount, CGUIMemoryManager::GetTotalSize( ) / 1024 );
		mpDebugStatic->Printf( tBuffer );
	}

	if ( vCurSel == 2 )
	{
		for ( int i = 0; i < int( tpSystem->mExceptions.size( ) ); i ++ )
			mpDebugStatic->Printf( tpSystem->mExceptions[ i ].c_str( ) );
	}
}

}

}