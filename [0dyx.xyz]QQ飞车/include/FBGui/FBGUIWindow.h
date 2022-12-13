#pragma once

namespace FBGUI
{

namespace Sink
{

// ����Update�¼�
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

// ������Ⱦ�¼�
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

// ���ڽ����¼�
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

// ���������¼�
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

// ���ڼ��̰����¼�
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

// ���ڼ���̧���¼�
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

// ��������м������¼�
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

// ��������м�̧���¼�
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

// ��������Ҽ������¼�
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

// ��������Ҽ�̧���¼�
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

// ���������������¼�
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

// �������˫���¼�
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

// ����������̧���¼�
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

// ��������ƶ��¼�
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

// �����������¼�
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

// ��������뿪�¼�
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

// �����������¼�
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

// ���ڳ��ֻ�����ʧ�¼�
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

// ���ڿ��û��ǲ������¼�
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

// ���ڼ�����ǲ������¼�
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

// ���ڻ�ý������ʧȥ�����¼�
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

// ���ڵĸ������ƶ��¼�
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

// ���ڵĸ����ڴ�С�ı��¼�
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

// �����Ϸ��¼�
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

// �����Ϸ��¼�
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

// ���ڴ�С�ı�ǰ�¼�
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

// ���ڴ�С�ı���¼�
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

// �����ƶ�ǰ�¼�
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

// �����ƶ����¼�
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

// �������������¼�
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

// ��ť������¼�
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

// ��������¼�
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

// �б��ѡ��ı��¼�
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

// �����б��ѡ��ı��¼�
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

// �����¼�
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

// ��������ݸı��¼�
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

// �����б��˫���¼�
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

// Sliderλ�øı��¼�
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

// Tabѡ��ı��¼�
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
	std::string					mType;			// Window ����
	std::string					mTitle;			// Window ����
	std::string					mName;			// Window ����, GUIϵͳΨһ
	unsigned int				mWindowStyle;	// Window ���
	unsigned int				mWindowFlag;	// Window ״̬
	unsigned int				mArchor;		// ê
	unsigned int				mTextColour;	// �ı���ɫ
	CGUIScrollBar*				mpHScrollBar;	// ���������
	CGUIScrollBar*				mpVScrollBar;	// ���������
	CGUIImageButton*			mpExitBtn;		// �رհ�ť
	CGUIWindow*					mpParent;		// ������
	CGUITexture*				mpTexture;		// ����
	std::vector< CGUIWindow* >	mChildren;		// �Ӵ���
	CGUIRect					mRect;			// ����ڸ����ڵ����꣬ͬê��ͬ����
	CGUIPoint					mLastLButton;	// ���һ����������������(�߼���Ļ����)
	bool						mDragWindow;	// ��ǰ�ǲ����϶�����
	bool						mSizeWindow;	// ��ǰ�ǲ��Ǹı䴰�ڴ�С
	bool						mResourceLoaded;// ������Դ�Ƿ����
	bool						mIsModal;
	CGUIPoint					mLossOffset;
	CGUIPoint					mCamPoint;
	CGUIFont*					mpFont;			//! ��ǰ����

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
	
	CGUIRect ScreenToWindow( const CGUIRect& rRect ) const;			//! ��Ļ����ת������������
	CGUIPoint ScreenToWindow( const CGUIPoint& rPoint ) const;		//! ��Ļ����ת������������
	CGUIRect WindowToScreen( const CGUIRect& rRect ) const;			//! ��������ת������Ļ����
	CGUIPoint WindowToScreen( const CGUIPoint& rPoint ) const;		//! ��������ת������Ļ����
	CGUIRect ScreenToClient( const CGUIRect& rRect ) const;			//! ��Ļ����ת������������
	CGUIPoint ScreenToClient( const CGUIPoint& rPoint ) const;		//! ��Ļ����ת������������
	CGUIRect ClientToScreen( const CGUIRect& rRect ) const;			//! �ͻ�����ת������Ļ����
	CGUIPoint ClientToScreen( const CGUIPoint& rPoint ) const;		//! �ͻ�����ת������Ļ����

	CGUIRect GetWindowRect( ) const;								//! �õ��������ڣ������ߺͱ�����߼��ߴ磬�Դ����������Ͻ�Ϊԭ��
	CGUIRect GetClientRect( ) const;								//! �õ����ڿͻ����򣬲������ߺͱ�����߼��ߴ磬�Կͻ��������Ͻ�Ϊԭ��
	CGUIRect GetClientRectEx( ) const;								//! �õ����ڿͻ����򣬲������ߺͱ�����߼��ߴ磬�Դ����������Ͻ�Ϊԭ��
	CGUIRect GetCaptionRect( ) const;								//! �õ����ڱ���ߴ磬�Դ������Ͻ�Ϊԭ��
	CGUIRect GetSizeRect( ) const;									//!	�õ�����SizeBar�ߴ磬�Դ������Ͻ�Ϊԭ��
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
	//! �ƶ�����
	//! \param const CGUIPoint& rOffset �������������ƫ��
	CGUIPoint MoveWindow( const CGUIPoint& rOffset );
	//! �ı䴰�ڴ�С
	//! \param const CGUIPoint& rOffset ��������򳤶�ƫ��
	CGUIPoint SizeWindow( const CGUIPoint& rOffset );
	CGUIScrollBar* GetHScrollBar( ) { return mpHScrollBar; }
	CGUIScrollBar* GetVScrollBar( ) { return mpVScrollBar; }
	bool UpdateScrollBar( int vLogicWidth, int vLogicHeight );

protected:
	//! ���������Ҫ����һ��,һ�����ڲü����������ָ�ô������������Ȳü������߼���Ļ�ϵ�����
	CGUIRect GetWindowClipRect( ) const;
	//! ���������Ҫ����һ��,һ�����ڲü����������ָ�ô������������Ȳü������߼���Ļ�ϵ�����
	CGUIRect GetClientClipRect( ) const;
	void AdjustVScrollBar( );
	void AdjustHScrollBar( );
	void MouseClick( const CGUIPoint& rPoint );
	void AddChildWindow( CGUIWindow* pChildWindow );	//! CWindowManager���ã����һ���Ӵ���
	void RemoveChildWindow( CGUIWindow* pWindow );		//! CWindowManager���ã�ɾ��һ���Ӵ���
	void RemoveAllChildWindow( );						//! CWindowManager���ã�ɾ�������Ӵ���
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
	CGUIWindowManager�ฺ����ϵͳ�������еĴ��ڣ�Clientͨ���ýӿڽ������еĴ���
	CGUIWindowManagerά�����ڵ������ڣ�����������
*/
class CGUIWindowManager : public CSingleton< CGUIWindowManager >
{
protected:
	unsigned int				mUIDCounter;
	std::vector< CGUIWindow* >	mWindowsRegister;		// ע������д���
	std::vector< CGUIWindow* >	mWindowsDeath;			// ���ݻٵĴ���

public:
	CGUIWindowManager( ) : mUIDCounter( 0 )	{	}

public:
	~CGUIWindowManager( );

public:
	/*!	\brief
		����һ��ָ�����͵Ĵ��ڣ���ָ��һ��Ψһ���ַ�����ʾ
	\param pType
		�������ͣ�����ƥ��һ�����ڹ���������Ӧ�Ĵ���
	\param pName
		�������֣�GUIϵͳΨһ�����pName����NULL,ϵͳ�Զ�����һ��Ψһ����
	\param vStyle
		���ڷ�񣬱��磺GUI_WS_BORDER��
	\return
		����һ�������Ĵ��� */
	CGUIWindow* Create( const char* pType, const char* pName, unsigned int vStyle, const CGUIRect& rRect, CGUIWindow* pParent, void* pUserData );

	/*!	\brief
		����һ��ָ�����͵Ĵ��ڣ���ָ��һ��Ψһ���ַ�����ʾ
	\param pType
		�������ͣ�����ƥ��һ�����ڹ���������Ӧ�Ĵ���
	\param pName
		�������֣�GUIϵͳΨһ�����pName����NULL,ϵͳ�Զ�����һ��Ψһ����
	\param vStyle
		���ڷ�񣬱��磺GUI_WS_BORDER��
	\return
		����һ�������Ĵ��� */
	CGUIWindow* DoModal( const char* pType, const char* pName, unsigned int vStyle, const CGUIRect& rRect, void* pUserData );
	/*!	\brief
		����һ��ָ�����͵Ĵ��ڣ���ָ��һ��Ψһ���ַ�����ʾ
	\param pName
		���ٴ��ڣ����ݴ���Ψһ���� */
	void Destroy( const char* pName );

	/*!	\brief
		����һ��ָ�����͵Ĵ��ڣ���ָ��һ��Ψһ���ַ�����ʾ
	\param pName
		���ٴ��ڣ����ݴ���ָ�� */
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
	std::string		mType;		// ��������

public:
	CGUIWindowFactory( const char* pType ) : mType( pType )	{	}

public:
	// pName, ���ڵ�Ψһ��ʶ
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
		ע�ᴰ�ڹ������Ա��Ժ���Խ�������
	\param pFactory
		ָ���Ĵ��ڹ���	
	*/
	void RegisterFactory( CGUIWindowFactory* pFactory )
		{ mFactorysRegister.push_back( pFactory ); }

	/*!
	\brief
		ע�ᴰ�ڹ������Ա��Ժ���Խ�������
	\param pFactory
		ָ���Ĵ��ڹ���	
	*/
	CGUIWindowFactory* GetFactory( const char* pType );
};

};