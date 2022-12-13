#pragma once

namespace FBGUI
{

namespace Sink
{

// 窗口Update事件
class CGUISinkWindowUpdate : public CGUISink
{
public:
	static const int sEventType;
	typedef void ( CGUIWindow::*WindowUpdateProc )( );

protected:
	virtual void Fire( CGUIEvent* pEvent )
		{ ( pEvent->mpWindow->*( (WindowUpdateProc) pEvent->mpEventHandler ) )( ); }

	virtual int GetType( ) { return sEventType; }
};

// 窗口渲染事件
class CGUISinkWindowRender : public CGUISink
{
public:
	static const int sEventType;
	typedef void ( CGUIWindow::*WindowRenderProc )( );

protected:
	virtual void Fire( CGUIEvent* pEvent )
		{ ( pEvent->mpWindow->*( (WindowRenderProc) pEvent->mpEventHandler ) )( ); }

	virtual int GetType( ) { return sEventType; }
};

// 窗口建立事件
class CGUISinkWindowCreate : public CGUISink
{
public:
	static const int sEventType;
	typedef void ( CGUIWindow::*WindowCreateProc )( );

protected:
	virtual void Fire( CGUIEvent* pEvent )
		{ ( pEvent->mpWindow->*( (WindowCreateProc) pEvent->mpEventHandler ) )( ); }

	virtual int GetType( ) { return sEventType; }
};

// 窗口销毁事件
class CGUISinkWindowDestory : public CGUISink
{
public:
	static const int sEventType;
	typedef void ( CGUIWindow::*WindowDestoryProc )( );

protected:
	virtual void Fire( CGUIEvent* pEvent )
		{ ( pEvent->mpWindow->*( (WindowDestoryProc) pEvent->mpEventHandler ) )( ); }

	virtual int GetType( ) { return sEventType; }
};

// 窗口键盘按下事件
class CGUISinkWindowKeyDown : public CGUISink
{
public:
	static const int sEventType;
	typedef void (CGUIWindow::*WindowKeyDown)( unsigned int vVirtualKey, unsigned int vFlag );

public:
	virtual void Fire( CGUIEvent* pEvent )
		{ ( pEvent->mpWindow->*( (WindowKeyDown) pEvent->mpEventHandler ) )( (unsigned int) mData[ 0 ].mDataValue, (unsigned int) mData[ 1 ].mDataValue ); }

	virtual int GetType( ) { return sEventType; }
};

// 窗口键盘抬起事件
class CGUISinkWindowKeyUp : public CGUISink
{
public:
	static const int sEventType;
	typedef void (CGUIWindow::*WindowKeyUp)( unsigned int vVirtualKey, unsigned int vFlag );

public:
	virtual void Fire( CGUIEvent* pEvent )
		{ ( pEvent->mpWindow->*( (WindowKeyUp) pEvent->mpEventHandler ) )( (unsigned int) mData[ 0 ].mDataValue, (unsigned int) mData[ 1 ].mDataValue ); }

	virtual int GetType( ) { return sEventType; }
};

// 窗口鼠标中键按下事件
class CGUISinkMButtonDown : public CGUISink
{
public:
	static const int sEventType;
	typedef void ( CGUIWindow::*MButtonDownProc )( const CGUIPoint*, unsigned int );

protected:
	virtual void Fire( CGUIEvent* pEvent )
		{ ( pEvent->mpWindow->*( (MButtonDownProc) pEvent->mpEventHandler ) )( (CGUIPoint*) mData[ 0 ].mDataValue, (unsigned int) mData[ 1 ].mDataValue ); }

	virtual int GetType( ) { return sEventType; }
};

// 窗口鼠标中键抬起事件
class CGUISinkMButtonUp : public CGUISink
{
public:
	static const int sEventType;
	typedef void ( CGUIWindow::*MButtonUpProc )( const CGUIPoint*, unsigned int );

protected:
	virtual void Fire( CGUIEvent* pEvent )
		{ ( pEvent->mpWindow->*( (MButtonUpProc) pEvent->mpEventHandler ) )( (CGUIPoint*) mData[ 0 ].mDataValue, (unsigned int) mData[ 1 ].mDataValue ); }

	virtual int GetType( ) { return sEventType; }
};

// 窗口鼠标右键按下事件
class CGUISinkRButtonDown : public CGUISink
{
public:
	static const int sEventType;
	typedef void ( CGUIWindow::*RButtonDownProc )( const CGUIPoint*, unsigned int );

protected:
	virtual void Fire( CGUIEvent* pEvent )
		{ ( pEvent->mpWindow->*( (RButtonDownProc) pEvent->mpEventHandler ) )( (CGUIPoint*) mData[ 0 ].mDataValue, (unsigned int) mData[ 1 ].mDataValue ); }

	virtual int GetType( ) { return sEventType; }
};

// 窗口鼠标右键抬起事件
class CGUISinkRButtonUp : public CGUISink
{
public:
	static const int sEventType;
	typedef void ( CGUIWindow::*RButtonUpProc )( const CGUIPoint*, unsigned int );

protected:
	virtual void Fire( CGUIEvent* pEvent )
		{ ( pEvent->mpWindow->*( (RButtonUpProc) pEvent->mpEventHandler ) )( (CGUIPoint*) mData[ 0 ].mDataValue, (unsigned int) mData[ 1 ].mDataValue ); }

	virtual int GetType( ) { return sEventType; }
};

// 窗口鼠标左键按下事件
class CGUISinkLButtonDown : public CGUISink
{
public:
	static const int sEventType;
	typedef void ( CGUIWindow::*LButtonDownProc )( const CGUIPoint*, unsigned int );

protected:
	virtual void Fire( CGUIEvent* pEvent )
		{ ( pEvent->mpWindow->*( (LButtonDownProc) pEvent->mpEventHandler ) )( (CGUIPoint*) mData[ 0 ].mDataValue, (unsigned int) mData[ 1 ].mDataValue ); }

	virtual int GetType( ) { return sEventType; }
};

// 窗口鼠标双击事件
class CGUISinkLButtonDblClk : public CGUISink
{
public:
	static const int sEventType;
	typedef void ( CGUIWindow::*LButtonDblClkProc )( const CGUIPoint*, unsigned int );

protected:
	virtual void Fire( CGUIEvent* pEvent )
		{ ( pEvent->mpWindow->*( (LButtonDblClkProc) pEvent->mpEventHandler ) )( (CGUIPoint*) mData[ 0 ].mDataValue, (unsigned int) mData[ 1 ].mDataValue ); }

	virtual int GetType( ) { return sEventType; }
};

// 窗口鼠标左键抬起事件
class CGUISinkLButtonUp : public CGUISink
{
public:
	static const int sEventType;
	typedef void ( CGUIWindow::*LButtonUpProc )( const CGUIPoint*, unsigned int );

protected:
	virtual void Fire( CGUIEvent* pEvent )
		{ ( pEvent->mpWindow->*( (LButtonUpProc) pEvent->mpEventHandler ) )( (CGUIPoint*) mData[ 0 ].mDataValue, (unsigned int) mData[ 1 ].mDataValue ); }
		
	virtual int GetType( ) { return sEventType; }
};

// 窗口鼠标移动事件
class CGUISinkMouseMove : public CGUISink
{
public:
	static const int sEventType;
	typedef void ( CGUIWindow::*MouseMoveProc )( const CGUIPoint*, unsigned int );

protected:
	virtual void Fire( CGUIEvent* pEvent )
		{ ( pEvent->mpWindow->*( (MouseMoveProc) pEvent->mpEventHandler ) )( (CGUIPoint*) mData[ 0 ].mDataValue, (unsigned int) mData[ 1 ].mDataValue ); }

	virtual int GetType( ) { return sEventType; }
};

// 窗口鼠标进入事件
class CGUISinkMouseEnter : public CGUISink
{
public:
	static const int sEventType;
	typedef void ( CGUIWindow::*MouseEnterProc )( const CGUIPoint*, unsigned int );

protected:
	virtual void Fire( CGUIEvent* pEvent )
		{ ( pEvent->mpWindow->*( (MouseEnterProc) pEvent->mpEventHandler ) )( (CGUIPoint*) mData[ 0 ].mDataValue, (unsigned int) mData[ 1 ].mDataValue ); }

	virtual int GetType( ) { return sEventType; }
};

// 窗口鼠标离开事件
class CGUISinkMouseLeave : public CGUISink
{
public:
	static const int sEventType;
	typedef void ( CGUIWindow::*MouseLeaveProc )( const CGUIPoint*, unsigned int );

protected:
	virtual void Fire( CGUIEvent* pEvent )
		{ ( pEvent->mpWindow->*( (MouseLeaveProc) pEvent->mpEventHandler ) )( (CGUIPoint*) mData[ 0 ].mDataValue, (unsigned int) mData[ 1 ].mDataValue ); }

	virtual int GetType( ) { return sEventType; }
};

// 窗口鼠标滚轮事件
class CGUISinkMouseWheel : public CGUISink
{
public:
	static const int sEventType;
	typedef void ( CGUIWindow::*MouseWheelProc )( const CGUIPoint*, int );

protected:
	virtual void Fire( CGUIEvent* pEvent )
		{ ( pEvent->mpWindow->*( (MouseWheelProc) pEvent->mpEventHandler ) )( (CGUIPoint*) mData[ 0 ].mDataValue, (int) mData[ 1 ].mDataValue ); }

	virtual int GetType( ) { return sEventType; }
};

// 窗口出现或是消失事件
class CGUISinkWindowVisible : public CGUISink
{
public:
	static const int sEventType;
	typedef void ( CGUIWindow::*WindowVisibleProc )( bool );

protected:
	virtual void Fire( CGUIEvent* pEvent )
		{ ( pEvent->mpWindow->*( (WindowVisibleProc) pEvent->mpEventHandler ) )( (bool) mData[ 0 ].mDataValue ); }

	virtual int GetType( ) { return sEventType; }
};

// 窗口可用或是不可用事件
class CGUISinkWindowEnable : public CGUISink
{
public:
	static const int sEventType;
	typedef void ( CGUIWindow::*WindowEnableProc )( bool );

protected:
	virtual void Fire( CGUIEvent* pEvent )
		{ ( pEvent->mpWindow->*( (WindowEnableProc) pEvent->mpEventHandler ) )( (bool) mData[ 0 ].mDataValue ); }

	virtual int GetType( ) { return sEventType; }
};

// 窗口激活或是不激活事件
class CGUISinkWindowActive : public CGUISink
{
public:
	static const int sEventType;
	typedef void ( CGUIWindow::*WindowActiveProc )( bool );

protected:
	virtual void Fire( CGUIEvent* pEvent )
		{ ( pEvent->mpWindow->*( (WindowActiveProc) pEvent->mpEventHandler ) )( (bool) mData[ 0 ].mDataValue ); }

	virtual int GetType( ) { return sEventType; }
};

// 窗口获得焦点或是失去焦点事件
class CGUISinkWindowFocus : public CGUISink
{
public:
	static const int sEventType;
	typedef void ( CGUIWindow::*WindowFocusProc )( CGUIWindow*, bool );

protected:
	virtual void Fire( CGUIEvent* pEvent )
		{ ( pEvent->mpWindow->*( (WindowFocusProc) pEvent->mpEventHandler ) )( (CGUIWindow*) mData[ 0 ].mDataValue, (bool) mData[ 1 ].mDataValue ); }

	virtual int GetType( ) { return sEventType; }
};

// 窗口的父窗口移动事件
class CGUISinkParentMoved : public CGUISink
{
public:
	static const int sEventType;
	typedef void ( CGUIWindow::*ParentMovedProc )( const CGUIPoint* );

protected:
	virtual void Fire( CGUIEvent* pEvent )
		{ ( pEvent->mpWindow->*( (ParentMovedProc) pEvent->mpEventHandler ) )( (const CGUIPoint*) mData[ 0 ].mDataValue ); }

	virtual int GetType( ) { return sEventType; }
};

// 窗口的父窗口大小改变事件
class CGUISinkParentSized : public CGUISink
{
public:
	static const int sEventType;
	typedef void ( CGUIWindow::*ParentSizedProc )( const CGUIPoint* );

protected:
	virtual void Fire( CGUIEvent* pEvent )
		{ ( pEvent->mpWindow->*( (ParentSizedProc) pEvent->mpEventHandler ) )( (const CGUIPoint*) mData[ 0 ].mDataValue ); }

	virtual int GetType( ) { return sEventType; }
};

// 窗口拖放事件
class CGUISinkWindowDragFrom : public CGUISink
{
public:
	static const int sEventType;
	typedef void ( CGUIWindow::*DragFromProc )( CGUIWindow*, const CGUIPoint* pPoint );

protected:
	virtual void Fire( CGUIEvent* pEvent )
		{ ( pEvent->mpWindow->*( (DragFromProc) pEvent->mpEventHandler ) )( (CGUIWindow*) mData[ 0 ].mDataValue, (const CGUIPoint*) mData[ 1 ].mDataValue ); }

	virtual int GetType( ) { return sEventType; }
};

// 窗口拖放事件
class CGUISinkWindowDragTo : public CGUISink
{
public:
	static const int sEventType;
	typedef void ( CGUIWindow::*DragToProc )( CGUIWindow*, const CGUIPoint* pPoint );

protected:
	virtual void Fire( CGUIEvent* pEvent )
		{ ( pEvent->mpWindow->*( (DragToProc) pEvent->mpEventHandler ) )( (CGUIWindow*) mData[ 0 ].mDataValue, (const CGUIPoint*) mData[ 1 ].mDataValue  ); }

	virtual int GetType( ) { return sEventType; }
};

// 窗口大小改变前事件
class CGUISinkWindowSizing : public CGUISink
{
public:
	static const int sEventType;
	typedef void ( CGUIWindow::*WindowSizingProc )( const CGUIPoint* );

protected:
	virtual void Fire( CGUIEvent* pEvent )
		{ ( pEvent->mpWindow->*( (WindowSizingProc) pEvent->mpEventHandler ) )( (const CGUIPoint*) mData[ 0 ].mDataValue ); }

	virtual int GetType( ) { return sEventType; }
};

// 窗口大小改变后事件
class CGUISinkWindowSized : public CGUISink
{
public:
	static const int sEventType;
	typedef void ( CGUIWindow::*WindowSizedProc )( const CGUIPoint* );

protected:
	virtual void Fire( CGUIEvent* pEvent )
		{ ( pEvent->mpWindow->*( (WindowSizedProc) pEvent->mpEventHandler ) )( (const CGUIPoint*) mData[ 0 ].mDataValue ); }

	virtual int GetType( ) { return sEventType; }
};

// 窗口移动前事件
class CGUISinkWindowMoving : public CGUISink
{
public:
	static const int sEventType;
	typedef void ( CGUIWindow::*WindowMovingProc )( const CGUIPoint* );

protected:
	virtual void Fire( CGUIEvent* pEvent )
		{ ( pEvent->mpWindow->*( (WindowMovingProc) pEvent->mpEventHandler ) )( (const CGUIPoint*) mData[ 0 ].mDataValue ); }

	virtual int GetType( ) { return sEventType; }
};

// 窗口移动后事件
class CGUISinkWindowMoved : public CGUISink
{
public:
	static const int sEventType;
	typedef void ( CGUIWindow::*WindowMovedProc )( const CGUIPoint* );

protected:
	virtual void Fire( CGUIEvent* pEvent )
		{ ( pEvent->mpWindow->*( (WindowMovedProc) pEvent->mpEventHandler ) )( (const CGUIPoint*) mData[ 0 ].mDataValue ); }

	virtual int GetType( ) { return sEventType; }
};

// 窗口设置字体事件
class CGUISinkWindowSetFont : public CGUISink
{
public:
	static const int sEventType;
	typedef void ( CGUIWindow::*WindowSetFont )( CGUIWindow* );

protected:
	virtual void Fire( CGUIEvent* pEvent )
		{ ( pEvent->mpWindow->*( (WindowSetFont) pEvent->mpEventHandler ) )( (CGUIWindow*) mData[ 0 ].mDataValue ); }

	virtual int GetType( ) { return sEventType; }
};

// 按钮鼠标点击事件
class CGUISinkClickButton : public CGUISink
{
public:
	static const int sEventType;
	typedef void (CGUIWindow::*ClickButtonProc)( CGUIWindow* pButton );

protected:
	virtual void Fire( CGUIEvent* pEvent )
		{ ( pEvent->mpWindow->*( (ClickButtonProc) pEvent->mpEventHandler ) )( (CGUIWindow*) mData[ 0 ].mDataValue ); }

	virtual int GetType( ) { return sEventType; }
};

// 滑块滚动事件
class CGUISinkThumbChanged : public CGUISink
{
public:
	static const int sEventType;
	typedef void (CGUIWindow::*ThumbChangedProc)( CGUIWindow* pScrollBar, int vNewPos );

public:
	virtual void Fire( CGUIEvent* pEvent )
		{ ( pEvent->mpWindow->*( (ThumbChangedProc) pEvent->mpEventHandler ) )( (CGUIWindow*) mData[ 0 ].mDataValue, (int) mData[ 1 ].mDataValue ); }

	virtual int GetType( ) { return sEventType; }
};	

// 列表框选择改变事件
class CGUISinkListBoxSelChanged : public CGUISink
{
public:
	static const int sEventType;
	typedef void (CGUIWindow::*ListBoxSelChangedProc)( CGUIWindow* pListBox, int vCurSel );

public:
	virtual void Fire( CGUIEvent* pEvent )
		{ ( pEvent->mpWindow->*( (ListBoxSelChangedProc) pEvent->mpEventHandler ) )( (CGUIWindow*) mData[ 0 ].mDataValue, (int) mData[ 1 ].mDataValue ); }

	virtual int GetType( ) { return sEventType; }
};

// 多行列表框选择改变事件
class CGUISinkListSelChanged : public CGUISink
{
public:
	static const int sEventType;
	typedef void (CGUIWindow::*ListSelChangedProc)( CGUIWindow* pListBox, int vCurSel );

public:
	virtual void Fire( CGUIEvent* pEvent )
		{ ( pEvent->mpWindow->*( (ListSelChangedProc) pEvent->mpEventHandler ) )( (CGUIWindow*) mData[ 0 ].mDataValue, (int) mData[ 1 ].mDataValue ); }

	virtual int GetType( ) { return sEventType; }
};

// 命令事件
class CGUISinkCommand : public CGUISink
{
public:
	static const int sEventType;
	typedef void (CGUIWindow::*CommandProc)( CGUIWindow* pWindow, int vCommandID );

public:
	virtual void Fire( CGUIEvent* pEvent )
		{ ( pEvent->mpWindow->*( (CommandProc) pEvent->mpEventHandler ) )( (CGUIWindow*) mData[ 0 ].mDataValue, (int) mData[ 1 ].mDataValue ); }

	virtual int GetType( ) { return sEventType; }
};

// 输入框内容改变事件
class CGUISinkEditChanged : public CGUISink
{
public:
	static const int sEventType;
	typedef void (CGUIWindow::*EditChangedProc)( CGUIWindow* pWindow );

public:
	virtual void Fire( CGUIEvent* pEvent )
		{ ( pEvent->mpWindow->*( (EditChangedProc) pEvent->mpEventHandler ) )( (CGUIWindow*) mData[ 0 ].mDataValue ); }

	virtual int GetType( ) { return sEventType; }
};

// 多行列表框双击事件
class CGUISinkListDblClk : public CGUISink
{
public:
	static const int sEventType;
	typedef void (CGUIWindow::*ListDblClkProc)( CGUIWindow* pWindow, int vItemIndex );

public:
	virtual void Fire( CGUIEvent* pEvent )
		{ ( pEvent->mpWindow->*( (ListDblClkProc) pEvent->mpEventHandler ) )( (CGUIWindow*) mData[ 0 ].mDataValue, (int) mData[ 1 ].mDataValue ); }

	virtual int GetType( ) { return sEventType; }
};

// Slider位置改变事件
class CGUISinkSliderPosChanged : public CGUISink
{
public:
	static const int sEventType;
	typedef void (CGUIWindow::*SliderPosChanged)( CGUIWindow* pWindow, int vNewPos );

public:
	virtual void Fire( CGUIEvent* pEvent )
		{ ( pEvent->mpWindow->*( (SliderPosChanged) pEvent->mpEventHandler ) )( (CGUIWindow*) mData[ 0 ].mDataValue, (int) mData[ 1 ].mDataValue ); }

	virtual int GetType( ) { return sEventType; }
};

// Tab选择改变事件
class CGUISinkTabSelChanged : public CGUISink
{
public:
	static const int sEventType;
	typedef void (CGUIWindow::*TabSelChanged)( CGUIWindow* pWindow, int vCurSel );

public:
	virtual void Fire( CGUIEvent* pEvent )
		{ ( pEvent->mpWindow->*( (TabSelChanged) pEvent->mpEventHandler ) )( (CGUIWindow*) mData[ 0 ].mDataValue, (int) mData[ 1 ].mDataValue ); }

	virtual int GetType( ) { return sEventType; }
};

};

// ************************************************************ //
// CGUIWindow
// ************************************************************ //

class CGUIWindow : protected CGUIEventSet
{
	friend class CGUICanvas;
	friend class CGUIWindowCanvas;
	friend class CGUIClientCanvas;
	friend class CGUISystem;
	friend class CGUIWindowManager;
	friend class CGUILuaInterface;

protected:
	std::string					mType;			// Window 类型
	std::string					mTitle;			// Window 标题
	std::string					mName;			// Window 名称, GUI系统唯一
	unsigned int				mWindowStyle;	// Window 风格
	unsigned int				mWindowFlag;	// Window 状态
	unsigned int				mArchor;		// 锚
	unsigned int				mTextColour;	// 文本颜色
	CGUIScrollBar*				mpHScrollBar;	// 横向滚动条
	CGUIScrollBar*				mpVScrollBar;	// 纵向滚动条
	CGUIImageButton*			mpExitBtn;		// 关闭按钮
	CGUIWindow*					mpParent;		// 父窗口
	CGUITexture*				mpTexture;		// 纹理
	std::vector< CGUIWindow* >	mChildren;		// 子窗口
	CGUIRect					mRect;			// 相对于父窗口的坐标，同锚共同决定
	CGUIPoint					mLastLButton;	// 最后一次鼠标左键按下坐标(逻辑屏幕坐标)
	bool						mDragWindow;	// 当前是不是拖动窗口
	bool						mSizeWindow;	// 当前是不是改变窗口大小
	bool						mResourceLoaded;// 窗口资源是否加载
	bool						mIsModal;
	CGUIPoint					mLossOffset;
	CGUIPoint					mCamPoint;
	CGUIFont*					mpFont;			//! 当前字体

public:
	CGUIWindow( const char* pType, const char* pName, unsigned int vStyle, const CGUIRect& rRect, CGUIWindow* pParent, void* pUserData );

public:
	virtual ~CGUIWindow( );

public:
	bool IsAncestor( CGUIWindow* pWindow );
	bool IsDescendant( CGUIWindow* pWindow );
	bool IsWindowVisible( ) const;
	bool IsWindowEnable( ) const;
	bool IsWindowActive( ) const;
	bool IsHit( const CGUIPoint& rPoint ) const	
	{ 
		if ( IsWindowVisible( ) == false ) return false;
		return GetWindowClipRect( ).PtInRect( rPoint ); 
	}

	void VisibleWindow( bool vVisible );
	void EnableWindow( bool vEnable );
	void ActiveWindow( bool vActive );
	std::string GetType( ) const { return mType; }
	std::string GetName( ) const { return mName; }
	CGUIWindow* GetParent( ) const { return mpParent; }
	CGUIWindow* WindowFromPoint( const CGUIPoint& rPoint );
	
	CGUIRect ScreenToWindow( const CGUIRect& rRect ) const;			//! 屏幕坐标转换到窗口坐标
	CGUIPoint ScreenToWindow( const CGUIPoint& rPoint ) const;		//! 屏幕坐标转换到窗口坐标
	CGUIRect WindowToScreen( const CGUIRect& rRect ) const;			//! 窗口坐标转换到屏幕坐标
	CGUIPoint WindowToScreen( const CGUIPoint& rPoint ) const;		//! 窗口坐标转换到屏幕坐标
	CGUIRect ScreenToClient( const CGUIRect& rRect ) const;			//! 屏幕坐标转换到窗口坐标
	CGUIPoint ScreenToClient( const CGUIPoint& rPoint ) const;		//! 屏幕坐标转换到窗口坐标
	CGUIRect ClientToScreen( const CGUIRect& rRect ) const;			//! 客户坐标转换到屏幕坐标
	CGUIPoint ClientToScreen( const CGUIPoint& rPoint ) const;		//! 客户坐标转换到屏幕坐标

	CGUIRect GetWindowRect( ) const;								//! 得到整个窗口，包括边和标题的逻辑尺寸，以窗口区域左上角为原点
	CGUIRect GetClientRect( ) const;								//! 得到窗口客户区域，不包括边和标题的逻辑尺寸，以客户区域左上角为原点
	CGUIRect GetClientRectEx( ) const;								//! 得到窗口客户区域，不包括边和标题的逻辑尺寸，以窗口区域左上角为原点
	CGUIRect GetCaptionRect( ) const;								//! 得到窗口标题尺寸，以窗口左上角为原点
	CGUIRect GetSizeRect( ) const;									//!	得到窗口SizeBar尺寸，以窗口左上角为原点
	CGUISize GetWindowSize( ) const
		{ return GetWindowRect( ).Size( ); }

	CGUISize GetClientSize( ) const
		{ return GetClientRect( ).Size( ); }

		void Destroy( );
	void SetArchor( unsigned int vArchor )	{ mArchor = vArchor; }
	void BringWindowToTop( );
	void SetText( const char* pText )		{ mTitle = pText; }
	std::string GetText( )					{ return mTitle; }
	void SetParent( CGUIWindow* pWindow )	{ mpParent = pWindow; }
	CGUIRect GetRect( )	const				{ return mRect; }
	CGUIFont* GetFont( ) const				{ return mpFont; }
	void SetFont( CGUIFont* pFont )			{ mpFont = pFont; }
	void SetTextColour( unsigned int vColour )	{ mTextColour = vColour; }
	unsigned int GetTextColour( ) const		{ return mTextColour; }
	void CenterWindow( CGUIWindow* pWindow );
	//! 移动窗口
	//! \param const CGUIPoint& rOffset 横向和纵向坐标偏移
	CGUIPoint MoveWindow( const CGUIPoint& rOffset );
	//! 改变窗口大小
	//! \param const CGUIPoint& rOffset 横向和纵向长度偏移
	CGUIPoint SizeWindow( const CGUIPoint& rOffset );
	CGUIScrollBar* GetHScrollBar( ) { return mpHScrollBar; }
	CGUIScrollBar* GetVScrollBar( ) { return mpVScrollBar; }
	bool UpdateScrollBar( int vLogicWidth, int vLogicHeight );

protected:
	//! 这个函数需要解释一下,一个窗口裁减后的区域是指该窗口与所有祖先裁减后在逻辑屏幕上的区域
	CGUIRect GetWindowClipRect( ) const;
	//! 这个函数需要解释一下,一个窗口裁减后的区域是指该窗口与所有祖先裁减后在逻辑屏幕上的区域
	CGUIRect GetClientClipRect( ) const;
	void AdjustVScrollBar( );
	void AdjustHScrollBar( );
	void MouseClick( const CGUIPoint& rPoint );
	void AddChildWindow( CGUIWindow* pChildWindow );	//! CWindowManager调用，添加一个子窗口
	void RemoveChildWindow( CGUIWindow* pWindow );		//! CWindowManager调用，删除一个子窗口
	void RemoveAllChildWindow( );						//! CWindowManager调用，删除所有子窗口
	void Render( );
	void Update( );

protected:
	virtual void OnWindowRender( );
	virtual void OnWindowUpdate( );
	virtual void OnKeyDown( unsigned int vVirtualKey, unsigned int vFlag );
	virtual void OnKeyUp( unsigned int vVirtualKey, unsigned int vFlag );
	virtual void OnChar( unsigned int vCharCode, unsigned int vFlag );
	virtual void OnSysKeyDown( unsigned int vVirtualKey, unsigned int vFlag );
	virtual void OnSysKeyUp( unsigned int vVirtualKey, unsigned int vFlag );
	virtual void OnUniChar( unsigned int vCharCode, unsigned int vFlag );
	virtual void OnSysDeadChar( unsigned int vCharCode, unsigned int vFlag );
	virtual void OnDeadChar( unsigned int vCharCode, unsigned int vFlag );
	virtual void OnMouseMove( const CGUIPoint& rPoint, unsigned int vFlag );
	virtual void OnMouseEnter( const CGUIPoint& rPoint, unsigned int vFlag );
	virtual void OnMouseLeave( const CGUIPoint& rPoint, unsigned int vFlag );
	virtual void OnMouseLButtonDown( const CGUIPoint& rPoint, unsigned int vFlag );
	virtual void OnMouseLButtonUp( const CGUIPoint& rPoint, unsigned int vFlag );
	virtual void OnMouseRButtonDown( const CGUIPoint& rPoint, unsigned int vFlag );
	virtual void OnMouseRButtonUp( const CGUIPoint& rPoint, unsigned int vFlag );
	virtual void OnMouseMButtonDown( const CGUIPoint& rPoint, unsigned int vFlag );
	virtual void OnMouseMButtonUp( const CGUIPoint& rPoint, unsigned int vFlag );
	virtual void OnMouseLButtonDblClk( const CGUIPoint& rPoint, unsigned int vFlag ); 
	virtual void OnMouseWheel( const CGUIPoint& rPoint, unsigned int vFlag );
	virtual void OnWindowVisible( bool vVisible );
	virtual void OnWindowEnable( bool vEnable );
	virtual void OnWindowActive( bool vActive );
	virtual void OnWindowFocus( CGUIWindow* pWindow, bool vFocus );
	virtual void OnWindowDestory( );
	virtual void OnParentSized( const CGUIPoint& rOffset );
	virtual void OnParentMoved( const CGUIPoint& rOffset );
	virtual void OnWindowMoved( const CGUIPoint& rOffset );		
	virtual void OnWindowSized( const CGUIPoint& rOffset );
	virtual CGUIPoint OnWindowSizing( const CGUIPoint& rOffset );
	virtual CGUIPoint OnWindowMoving( const CGUIPoint& rOffset );
	virtual int OnWindowCreate( );
	virtual void OnLoadResource( );
	virtual void OnDestroyResource( );
	virtual void OnSetFont( );

protected:
	void OnCancel( CGUIWindow* pButton );
	void OnHScrollBar( CGUIWindow* pScrollBar, int vNewPos );
	void OnVScrollBar( CGUIWindow* pScrollBar, int vNewPos );
};

// ************************************************************ //
// CGUIWindowManager
// ************************************************************ //

/*!
\brief
	CGUIWindowManager类负责建立系统里面所有的窗口，Client通过该接口建立所有的窗口
	CGUIWindowManager维护窗口的生命期，建立和销毁
*/
class CGUIWindowManager : public CSingleton< CGUIWindowManager >
{
protected:
	unsigned int				mUIDCounter;
	std::vector< CGUIWindow* >	mWindowsRegister;		// 注册的所有窗口
	std::vector< CGUIWindow* >	mWindowsDeath;			// 被摧毁的窗口

public:
	CGUIWindowManager( ) : mUIDCounter( 0 )	{	}

public:
	~CGUIWindowManager( );

public:
	/*!	\brief
		建立一个指定类型的窗口，并指定一个唯一的字符串标示
	\param pType
		窗口类型，用来匹配一个窗口工厂建立对应的窗口
	\param pName
		窗口名字，GUI系统唯一，如果pName等于NULL,系统自动产生一个唯一名字
	\param vStyle
		窗口风格，比如：GUI_WS_BORDER等
	\return
		返回一个创建的窗口 */
	CGUIWindow* Create( const char* pType, const char* pName, unsigned int vStyle, const CGUIRect& rRect, CGUIWindow* pParent, void* pUserData );

	/*!	\brief
		建立一个指定类型的窗口，并指定一个唯一的字符串标示
	\param pType
		窗口类型，用来匹配一个窗口工厂建立对应的窗口
	\param pName
		窗口名字，GUI系统唯一，如果pName等于NULL,系统自动产生一个唯一名字
	\param vStyle
		窗口风格，比如：GUI_WS_BORDER等
	\return
		返回一个创建的窗口 */
	CGUIWindow* DoModal( const char* pType, const char* pName, unsigned int vStyle, const CGUIRect& rRect, void* pUserData );
	/*!	\brief
		建立一个指定类型的窗口，并指定一个唯一的字符串标示
	\param pName
		销毁窗口，根据窗口唯一名字 */
	void Destroy( const char* pName );

	/*!	\brief
		建立一个指定类型的窗口，并指定一个唯一的字符串标示
	\param pName
		销毁窗口，根据窗口指针 */
	void Destroy( CGUIWindow* pWindow );

	CGUIWindow* GetWindow( const char* pName );
	void ClearDeathPool( );
};

// ************************************************************ //
// CWindowFactory
// ************************************************************ //

class CGUIWindowFactory
{
protected:
	std::string		mType;		// 对象类型

public:
	CGUIWindowFactory( const char* pType ) : mType( pType )	{	}

public:
	// pName, 窗口的唯一标识
	virtual CGUIWindow* Create( const char* pName, unsigned int vStyle, const CGUIRect& rRect, CGUIWindow* pParent, void* pUserData ) = 0;
	virtual void Destroy( CGUIWindow* pWindow ) = 0;

	std::string GetType( )	{ return mType; }
};

// ************************************************************ //
// CGUIWindowFactoryManager
// ************************************************************ //

class CGUIWindowFactoryManager : public CSingleton< CGUIWindowFactoryManager >
{
protected:
	std::vector< CGUIWindowFactory* > mFactorysRegister;

public:
	/*!
	\brief
		Constructs a new CGUIWindowFactoryManager object.
	*/
	CGUIWindowFactoryManager( );
public:
	/*!
	\brief
		Destructor for ~CGUIWindowFactoryManager object.
	*/
	~CGUIWindowFactoryManager( );

public:
	/*!
	\brief
		注册窗口工厂，以便以后可以建立窗口
	\param pFactory
		指定的窗口工厂	
	*/
	void RegisterFactory( CGUIWindowFactory* pFactory )
		{ mFactorysRegister.push_back( pFactory ); }

	/*!
	\brief
		注册窗口工厂，以便以后可以建立窗口
	\param pFactory
		指定的窗口工厂	
	*/
	CGUIWindowFactory* GetFactory( const char* pType );
};

};