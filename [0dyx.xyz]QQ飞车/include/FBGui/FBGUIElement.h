#pragma once

namespace FBGUI
{

namespace Element
{

// ************************************************************ //
// CGUICustom
// ************************************************************ //

class CGUICustom : public CGUIWindow
{
public:
	CGUICustom( const char* pType, const char* pName, unsigned int vStyle, const CGUIRect& rRect, CGUIWindow* pParent, void* pUserData ) 
		: CGUIWindow( pType, pName, vStyle, rRect, pParent, pUserData ) {	}
};

// ************************************************************ //
// CGUISheet
// ************************************************************ //

class CGUISheet : public CGUIWindow
{
public:
	CGUISheet( const char* pType, const char* pName, unsigned int vStyle, const CGUIRect& rRect, CGUIWindow* pParent, void* pUserData );

public:
	virtual void OnRenderCursor( const CGUIPoint& rPoint, unsigned int vFlag )	{	}
	virtual void OnRenderDragTexture( CGUITexture* pTexture, const CGUIPoint& rPoint );
	virtual void OnWindowRender( );
};

// ************************************************************ //
// CGUIFrame
// ************************************************************ //

class CGUIFrame : public CGUIWindow
{
public:
	CGUIPoint	mCamPoint;

public:
	CGUIFrame( const char* pType, const char* pName, unsigned int vStyle, const CGUIRect& rRect, CGUIWindow* pParent, void* pUserData )
		: CGUIWindow( pType, pName, GUI_WS_CAPTION | GUI_WS_SYSMENU | GUI_WS_BORDER | GUI_WS_CANMOVE | GUI_WS_TABGROUP | vStyle, rRect, pParent, pUserData ) {	}

protected:
	virtual void OnWindowRender( );
	virtual CGUIPoint OnWindowSizing( const CGUIPoint& rOffset );
	virtual void OnMouseRButtonUp( const CGUIPoint& rPoint, unsigned int vFlag );
	virtual void OnLoadResource( );
	virtual void OnDestroyResource( );
};

// ************************************************************ //
// CGUIButton
// 事件：所有的窗口事件 & ON_BN_CLICKED( Window, MemberFx )
// ************************************************************ //

class CGUIButton : public CGUIWindow
{
public:
	CGUIButton( const char* pType, const char* pName, unsigned int vStyle, const CGUIRect& rRect, CGUIWindow* pParent, void* pUserData ) 
		: CGUIWindow( pType, pName, GUI_WS_NORMAL, rRect, pParent, pUserData ) {	}

protected:
	virtual void OnMouseLButtonUp( const CGUIPoint& rPoint, unsigned int vFlag );
	virtual void OnMouseLButtonDown( const CGUIPoint& rPoint, unsigned int vFlag );
	virtual void OnMouseLeave( const CGUIPoint& rPoint, unsigned int vFlag );
	virtual void OnMouseEnter( const CGUIPoint& rPoint, unsigned int vFlag );
	virtual void OnWindowRender( );
	virtual void OnLoadResource( );
	virtual void OnDestroyResource( );
};

// ************************************************************ //
// CGUICheck
// 事件：所有的窗口事件 & ON_BN_CLICKED( Window, MemberFx )
// ************************************************************ //

class CGUICheck : public CGUIButton
{
public:
	CGUISize	mImageSize;
	bool		mIsChecked;

public:
	CGUICheck( const char* pType, const char* pName, unsigned int vStyle, const CGUIRect& rRect, CGUIWindow* pParent, void* pUserData )
		: CGUIButton( pType, pName, vStyle, rRect, pParent, pUserData ), mImageSize( 16, 16 )	{	}

protected:
	void RenderCheck( CGUICanvas* pCanvas );

public:
	bool IsCheck( ) { return mIsChecked; }
	void SetCheck( bool vCheck ) { mIsChecked = vCheck; }
	
protected:
	virtual void OnMouseLButtonUp( const CGUIPoint& rPoint, unsigned int vFlag );
	virtual void OnWindowRender( );
	virtual void OnLoadResource( );
	virtual void OnDestroyResource( );
};

// ************************************************************ //
// CGUIImageButton
// 事件：所有的窗口事件 & ON_BN_CLICKED( Window, MemberFx )
// ************************************************************ //

class CGUIImageButton : public CGUIButton
{
public:
	CGUISize	mImageSize;
	std::string mResName;

public:
	CGUIImageButton( const char* pType, const char* pName, unsigned int vStyle, const CGUIRect& rRect, CGUIWindow* pParent, void* pUserData ) 
		: CGUIButton( pType, pName, vStyle, rRect, pParent, pUserData )	{	}

public:
	void SetImage( const char* pName, const CGUISize& rSize )
		{ mResName = pName; mImageSize = rSize; }

protected:
	virtual void OnWindowRender( );
	virtual void OnLoadResource( );
	virtual void OnDestroyResource( );
};

// ************************************************************ //
// CGUIImageCheck
// 事件：所有的窗口事件 & ON_BN_CLICKED( Window, MemberFx )
// ************************************************************ //

class CGUIImageCheck : public CGUICheck
{
public:
	std::string mResName;

public:
	CGUIImageCheck( const char* pType, const char* pName, unsigned int vStyle, const CGUIRect& rRect, CGUIWindow* pParent, void* pUserData )
		: CGUICheck( pType, pName, vStyle, rRect, pParent, pUserData ) {	}

public:
	void SetImage( const char* pName, const CGUISize& rSize )
		{ mResName = pName; mImageSize = rSize; }

protected:
	virtual void OnWindowRender( );
	virtual void OnLoadResource( );
	virtual void OnDestroyResource( );
};

// ************************************************************ //
// CGUIScrollBar
// 事件：所有的窗口事件 & ON_SB_THUMBCHANGED( Window, MemberFx )
// ************************************************************ //

class CGUIScrollBar : public CGUIWindow
{
	friend class CGUIWindow;

protected:	
	int					mRangeMax;			//! 滚动条所能显示的最大范围
	int					mRangeMin;			//! 滚动光标所代表的范围
	int					mLineSize;			//! 滚动时一行的相对尺寸
	int					mPageSize;			//! 滚动时一页的相对尺寸
	int					mPosition;			//! 当前光标的位置
	CGUIImageButton*	mpPageUp;
	CGUIImageButton*	mpPageDown;
	CGUIImageButton*	mpThumbBtn;

public:
	CGUIScrollBar( const char* pType, const char* pName, unsigned int vStyle, const CGUIRect& rRect, CGUIWindow* pParent, void* pUserData );

public:
	void SetRange( int vRangeMax, int vRangeMin ) 
	{
		mRangeMax = GUI_MAX( vRangeMax, vRangeMin ); 
		mRangeMin = vRangeMin; 
	}
	void SetScrollSize( int vLineSize, int vPageSize ) { mLineSize = vLineSize; mPageSize = vPageSize; }
	void SetPosition( int vPosition );
	int GetPosition( ) const;

protected:
	virtual void OnKeyDown( unsigned int vVirtualKey, unsigned int vFlag );
	virtual void OnWindowRender( );
	virtual void OnMouseLButtonDown( const CGUIPoint& rPoint, unsigned int vFlag );
	virtual void OnMouseWheel( const CGUIPoint& rPoint, unsigned int vFlag );
	virtual void OnWindowSized( const CGUIPoint& rOffset );
	virtual int OnWindowCreate( );
	virtual void OnLoadResource( );
	virtual void OnDestroyResource( );

protected:
	void OnPageDownButtonClick( CGUIWindow* pButton );
	void OnPageUpButtonClick( CGUIWindow* pButton );
	void OnThumbLButtonDown( const CGUIPoint* pPoint, unsigned int vFlag );
	void OnThumbMouseMove( const CGUIPoint* pPoint, unsigned int vFlag );
	void OnThumbLButtonUp( const CGUIPoint* pPoint, unsigned int vFlag );
};

// ************************************************************ //
// CGUIEdit
// 
// 输入框控件，自动处理键盘字符输入消息。（目前支持多字符）
// 支持多行，支持换行符号（自动换行），支持剪贴板操作，
// 支持横向自动滚动，支持纵向滚动，有滚动条功能, 支持鼠标操作(NB)
// 事件：所有的窗口事件 & ON_EN_CHANGED( Window, MemberFx )
// ************************************************************ //
class CGUIEdit : public CGUIWindow
{
protected:
	class CEditText
	{
	public:
		char		mText[ 3 ];		// 单字符或是双字符,mText[0]的高位决定
		CGUIRect	mRect;

	public:
		CEditText( )
		{
			mText[ 0 ] = 0;
			mText[ 1 ] = 0;
			mText[ 2 ] = 0;
		}

	public:
		CEditText& operator = ( const char* pText );

	public:
		int GetTextCount( ) const { return mText[ 0 ] & 0x80 ? 2 : 1; }
		int GetTextWidth( CGUIFont* pFont ) const;
		CGUIRect GetCursorRect( ) const { return CGUIRect( mRect.mBottomRight.mX - 2, mRect.mTopLeft.mY, mRect.mBottomRight.mX, mRect.mBottomRight.mY ); }
	};

protected:
	std::vector< CEditText >	mTexts;				// 格式化后的文本
	int							mSelectionBegin;	// 选择的开始位置，字节为单位
	int							mSelectionEnd;		// 选择的结束位置，字节为单位
	int							mCursorIndex;		// 光标位置，字节为单位
	bool						mInputChinese;		// 当前输入的是否汉字
	unsigned int				mLastTickCount;		// 光标时间节拍
	unsigned int				mTurn;				// 光标状态
	CEditText					mCurrentInput;		// 当前输入

public:
	// 输入框控件构造函数。
	CGUIEdit::CGUIEdit( const char* pType, const char* pName, unsigned int vStyle, const CGUIRect& rRect, CGUIWindow* pParent, void* pUserData )
		: CGUIWindow( pType, pName, vStyle | GUI_WS_BORDER, rRect, pParent, pUserData ), mCursorIndex( 0 ), mSelectionBegin( -1 ), mSelectionEnd( -1 ),
		mLastTickCount( 0 ), mTurn( 0 ), mInputChinese( false )
	{
		mWindowStyle &= ~GUI_WS_CAPTION;
		mWindowStyle &= ~GUI_WS_THICKFRAME;
		mWindowStyle &= ~GUI_WS_CANMOVE;
		mWindowStyle &= ~GUI_WS_SYSMENU;
		if ( ( mWindowStyle & GUI_ES_MUTILINE ) == false )
			mWindowStyle |= GUI_ES_SINGLELINE;
	}

protected:
	bool InsertChar( char vCharCode );
	void SelectionLeft( int vOffset, int vCursorIndex );
	void SelectionRight( int vOffset, int vCursorIndext );
	void CursorLeft( int vShiftDown );
	void CursorRight( int vShiftDown );
	void CursorUp( int vShiftDown );
	void CursorDown( int vShiftDown );
	void VisibleCursor( );
	void UnSelection( );
	void DeleteSelection( );

	// 该函数计算每一个字符的矩形大小，如果发现最后一个字符操作显示区域，会自动出现滚动条
	void CalcTextRect( );
	int GetTextIndex( const CGUIPoint& rPoint ) const;
	CGUIRect GetCursorRect( ) const;

public:
	void LineScroll( int vLine );
	void SetSel( int vStartChar, int vEndChar );
	void Clear( );
	void Copy( );
	void Paste( );
	void Cut( );
	void ResetContext( );
	void SetEditText( const char* pText );
	std::string GetEditText( );

protected:
	virtual void OnMouseLButtonDown( const CGUIPoint& rPoint, unsigned int vFlag );
	virtual void OnMouseLButtonUp( const CGUIPoint& rPoint, unsigned int vFlag );
	virtual void OnMouseMove( const CGUIPoint& rPoint, unsigned int vFlag );
	virtual void OnKeyDown( unsigned int vVirtualKey, unsigned int vFlag );
	virtual void OnChar( unsigned int vCharCode, unsigned int vFlag );
	virtual void OnWindowRender( );
	virtual void OnWindowUpdate( );
	virtual void OnWindowSized( const CGUIPoint& rOffset );
	virtual CGUIPoint OnWindowSizing( const CGUIPoint& rOffset );
	virtual void OnLoadResource( );
	virtual void OnDestroyResource( );
};

// ************************************************************ //
// CGUIListBox
// 事件：所有的窗口事件 & ON_LB_SELCHANGED( Window, MemberFx )
// ************************************************************ //

class CGUIListBox : public CGUIWindow
{
protected:
	class CListItem
	{
	public:
		std::string		mText;
		CGUIPoint		mPoint;
		bool			mIsSelected;
		bool			mIsHover;
		unsigned int	mItemData;

	public:
		CListItem( const char* pText ) : mText( pText ), mIsSelected( false ), 
			mIsHover( false ), mItemData( 0 )	{	}
	};

protected:
	std::vector< CListItem* >	mItems;
	unsigned int				mSelColour;

public:
	CGUIListBox( const char* pType, const char* pName, unsigned int vStyle, const CGUIRect& rRect, CGUIWindow* pParent, void* pUserData ) 
		: CGUIWindow( pType, pName, vStyle, rRect, pParent, pUserData ), mSelColour( GUI_MAKECOLOUR( 255, 30, 45, 46 ) )
	{
		mWindowStyle &= ~GUI_WS_CAPTION;
		mWindowStyle &= ~GUI_WS_BORDER;
		mWindowStyle &= ~GUI_WS_THICKFRAME;
		mWindowStyle &= ~GUI_WS_CANMOVE;
		mWindowStyle &= ~GUI_WS_SYSMENU;
		mWindowStyle |= GUI_WS_HSCROLL;
		mWindowStyle |= GUI_WS_VSCROLL;
	}

public:
	virtual ~CGUIListBox( )	{ RemoveAllItems( ); }

public:
	int AppendItem( const char* pText );
	void RemoveAllItems( );
	void RemoveItem( int vItemIndex );
	void SetCurSel( int vItem );
	int GetCurSel( ) const;
	int GetItemCount( ) const { return int( mItems.size( ) ); }
	std::string GetItemText( int vItemIndex ) const { return mItems[vItemIndex]->mText;	}
	void SetItemData( int vItemIndex, unsigned int vItemData ) { mItems[vItemIndex]->mItemData = vItemData; }
	unsigned int GetItemData( int vItemIndex ) const { return mItems[vItemIndex]->mItemData; }
	void SetSelColour( unsigned int vColour ) {	mSelColour = vColour; }
	int	HitTest( const CGUIPoint& rPoint ) const;

protected:
	void RefreshItems( );

protected:
	virtual void OnKeyDown( unsigned int vVirtualKey, unsigned int vFlag );
	virtual void OnMouseLButtonUp( const CGUIPoint& rPoint, unsigned int vFlags );
	virtual void OnMouseLButtonDown( const CGUIPoint& rPoint, unsigned int vFlags );
	virtual void OnMouseLButtonDblClk( const CGUIPoint& rPoint, unsigned int vFlags );
	virtual void OnMouseMove( const CGUIPoint& rPoint, unsigned int vFlags );
	virtual void OnMouseLeave( const CGUIPoint& rPoint, unsigned int vFlag );
	virtual void OnWindowRender( );
	virtual void OnWindowSized( const CGUIPoint& rOffset );
	virtual void OnDrawItem( CGUICanvas* pCanvas, const CListItem* pItem, const CGUIRect& rItemRect );
	virtual void OnLoadResource( );
	virtual void OnDestroyResource( );
};

// ************************************************************ //
// CGUIMenu
// 事件：所有的窗口事件 & ON_WM_COMMAND( Window, MemberFx )
// ************************************************************ //

class CGUIMenu : public CGUIWindow
{
protected:
	class CMenuItem
	{
		friend class CGUIMenu;

	protected:
		CMenuItem*					mpParent;
		CMenuItem*					mpExpand;
		std::vector< CMenuItem* >	mItems;
		std::string					mText;
		unsigned int				mStyle;
		unsigned int				mMenuID;
		bool						mIsHover;
		bool						mIsExpanded;
		CGUIRect					mRect;				// 该菜单项的区域大小
		CGUIRect					mPopupRect;			// 如果该菜单项是MSF_POPUPMENU, 区域是子菜单整体大小

	public:
		CMenuItem( const char* pText, unsigned int vStyle, CMenuItem* pParent, unsigned int vMenuID ) :	mText( pText ), mStyle( vStyle ), 
			mMenuID( vMenuID ), mIsHover( false ), mIsExpanded( false ), mpParent( pParent ), mpExpand( NULL )	{	}

	public:
		~CMenuItem( );

	public:
		bool Expand( );
		void UnExpand( );
		void DrawItems( CGUICanvas* pCanvas, const CGUIMenu* pMenu ) const;
		CGUIRect GetMenuBox( ) const;
		CMenuItem* HitTest( const CGUIPoint& rPoint ) const;
	};

protected:
	CMenuItem*				mpRoot;
	CMenuItem*				mpHover;

public:
	CGUIMenu( const char* pType, const char* pName, unsigned int vStyle, const CGUIRect& rRect, CGUIWindow* pParent, void* pUserData )
		: CGUIWindow( pType, pName, GUI_WS_POPUP, CGUIRect( rRect.mTopLeft, rRect.mTopLeft ), pParent, pUserData ), mpHover( NULL ) 
	{
		mpRoot	= new CMenuItem( "", GUI_MS_POPUP, NULL, 0 );
		mpRoot->mIsExpanded = true;
		mpRoot->mPopupRect.SetRect( 0, 0, 50, GUI_MENU_BORDER_HEIGHT << 1 );
	}
		
public:
	virtual ~CGUIMenu( ) { delete mpRoot; }

public:
	HMENUITEM AppendItem( HMENUITEM vParentMenu, const char* pText, unsigned int vMenuID, unsigned int vStyle );
	void SetItemData( int vItemIndex, unsigned int vUserData );
	unsigned int GetItemData( int vItemIndex ) const;

protected:
	//! 渲染该菜单项
	virtual void OnDrawItem( CGUICanvas* pCanvas, const CMenuItem* pItem ) const;
	//! 当该菜单是POPUP时才会被系统条用，渲染该菜单的子菜单存底
	virtual void OnDrawMenu( CGUICanvas* pCanvas, const CGUIRect& rRect ) const;

protected:
	virtual void OnWindowRender( );
	virtual void OnMouseLButtonUp( const CGUIPoint& rPoint, unsigned int vFlag );
	virtual void OnMouseMove( const CGUIPoint& rPoint, unsigned int vFlag );
	virtual void OnMouseLeave( const CGUIPoint& rPoint, unsigned int vFlag );
	virtual void OnLoadResource( );
	virtual void OnDestroyResource( );
};

// ************************************************************ //
// CGUITree
// 树形控件，自动处理消息，支持横向滚动，纵向滚动
// ************************************************************ //
class CGUITree : public CGUIWindow
{
protected:
	class CTreeItem
	{
		friend class CGUITree;

	protected:
		std::vector< CTreeItem* >	mNodes;			// 子节点
		std::string					mText;			// 节点文本
		CGUIRect					mRect;			// 节点矩形区域
		bool						mIsExpanded;	// 节点是否被展开
		bool						mIsSelected;	// 节点是不是选择了

	public:
		CTreeItem( const char* pText, bool vExpanded ) : mText( pText ), mIsExpanded( vExpanded ), mIsSelected( false ) {	}

	public:
		~CTreeItem( );

	public:
        int RefreshItems( CGUICanvas* pCanvas, int vLeft, int vTop, int vHeight, int& rWidth );
		void RemoveAllItems( );
		void DrawItems( CGUICanvas* pCanvas, CGUITree* pTree );
		void Expand( );
		CTreeItem* HitTest( const CGUIPoint& rPoint );
	};

protected:
	CTreeItem*		mpSelected;
	CTreeItem*		mpRoot;		// 根节点

public:
	CGUITree( const char* pType, const char* pName, unsigned int vStyle, const CGUIRect& rRect, CGUIWindow* pParent, void* pUserData ) 
		: CGUIWindow( pType, pName, vStyle, rRect, pParent, pUserData ), mpSelected( NULL )
	{
		mWindowStyle &= ~GUI_WS_CAPTION;
		mWindowStyle &= ~GUI_WS_BORDER;
		mWindowStyle &= ~GUI_WS_THICKFRAME;
		mWindowStyle &= ~GUI_WS_CANMOVE;
		mWindowStyle &= ~GUI_WS_SYSMENU;
		mpRoot = new CTreeItem( "_RootNode", true );
	}

public:
	virtual ~CGUITree( ) { delete mpRoot; }

public:
	HTREEITEM InsertItem( const char* pText, HTREEITEM vParent = NULL, HTREEITEM vInsertAfter = NULL );
	void RemoveAllItems( );

protected:
	void RefreshItems( );

protected:
	virtual void OnDrawItem( CGUICanvas* pCanvas, CTreeItem* pItem ) const;
	virtual void OnWindowRender( );
	virtual void OnWindowSized( const CGUIPoint& rOffset );
	virtual void OnMouseLButtonDown( const CGUIPoint& rPoint, unsigned int vFlag );
	virtual void OnMouseLButtonDblClk( const CGUIPoint& rPoint, unsigned int vFlag );
	virtual void OnLoadResource( );
	virtual void OnDestroyResource( );
};

// ************************************************************ //
// CGUIList
// 事件：所有的窗口事件 & ON_LC_SELCHANGED & ON_LC_DBLCLK
// ************************************************************ //

class CGUIList : public CGUIWindow
{
protected:
	class CColumnItem
	{
		friend class CGUIList;
	protected:
		std::string		mText;
		CGUIRect		mRect;
		bool			mIsClicked;
		int				mSortType;		// 排序类型 0:无排序 1;升序 2:降序
		int				mFormat;

	public:
		CColumnItem( const char* pText ) : mText( pText ), mIsClicked( false ), mSortType( 0 )	{	}
	};

	class CGridItem
	{
		friend class CGUIList;
		friend class CRowItem;
	protected:
		std::string	mText;

	public:
		CGridItem( const char* pText ) : mText( pText )	{	}
	};

	class CRowItem
	{
		friend class CGUIList;
	protected:
		std::vector<CGridItem*>		mItems;
		bool						mIsSelected;
		unsigned int				mItemData;

	public:
		CRowItem( ) : mIsSelected( false ), mItemData( 0 )	{	}
	};

protected:
	std::vector<CRowItem*>		mItems;
	std::vector<CGUIRect>		mSlotRects;
	std::vector<CColumnItem*>	mColumns;
	int							mSizeColumn;
	unsigned int				mSelColour;

public:
	CGUIList( const char* pType, const char* pName, unsigned int vStyle, const CGUIRect& rRect, CGUIWindow* pParent, void* pUserData ) 
		: CGUIWindow( pType, pName, vStyle, rRect, pParent, pUserData ), mSizeColumn( -1 ), mSelColour( GUI_MAKECOLOUR( 255, 30, 45, 46 ) )
	{
		mWindowStyle &= ~GUI_WS_CAPTION;
		mWindowStyle &= ~GUI_WS_BORDER;
		mWindowStyle &= ~GUI_WS_THICKFRAME;
		mWindowStyle &= ~GUI_WS_CANMOVE;
		mWindowStyle &= ~GUI_WS_SYSMENU;
	}

public:
	virtual ~CGUIList( );

public:
	void RemoveAllItems( );
	void RemoveItem( int vItemIndex );
	void AppendColumn( const char* pText, int vWidth, int vFormat );
	void SetItem( int vItem, int vSubItem, const char* pText );
	void SetItemData( int vItem, unsigned int vUserData );
	void SetCurSel( int vItem );
	int AppendItem( const char* pText );
	int HitTest( const CGUIPoint& rPoint );
	int GetCurSel( ) const;
	int GetCount( ) const;
	void SetSelColour( unsigned int vColour ) { mSelColour = vColour; }
	unsigned int GetItemData( int vItemIndex ) const;
	std::string GetItemText( int vItemIndex, int vSubItem ) const;

protected:
	virtual void OnDrawColumn( CGUICanvas* pCanvas, CColumnItem* pItem );
	virtual void OnDrawGrid( CGUICanvas* pCanvas, const char* pText, const CGUIRect& rRect, int vFormat );
	virtual void OnDrawItem( CGUICanvas* pCanvas, const CGUIRect& rRect, bool vIsSelected, int vItemIndex );

protected:
	virtual void Sort( int vColumnIndex, bool vIsAscending );
	virtual void OnMouseLButtonDblClk( const CGUIPoint& rPoint, unsigned int vFlag );
	virtual void OnMouseLButtonDown( const CGUIPoint& rPoint, unsigned int vFlag );
	virtual void OnMouseLButtonUp( const CGUIPoint& rPoint, unsigned int vFlag );
	virtual void OnMouseMove( const CGUIPoint& rPoint, unsigned int vFlag );
	virtual void OnKeyDown( unsigned int vVirtualKey, unsigned int vFlag );
	virtual void OnLoadResource( );
	virtual void OnDestroyResource( );
	virtual void OnWindowRender( );
	virtual void OnWindowSized( const CGUIPoint& rOffset );
};

// ************************************************************ //
// CControlComboBox
// ************************************************************ //
class CGUIComboBox : public CGUIWindow
{
protected:
	CGUIImageButton*	mpExpandBtn;
	CGUIListBox*		mpList;
	CGUIRect			mListRect;
	CGUIRect			mBtnRect;
	CGUIRect			mComboRect;
	bool				mExpanded;
	int					mExpandState;
	int					mSelItem;

public:
	CGUIComboBox( const char* pType, const char* pName, unsigned int vStyle, const CGUIRect &rRect, CGUIWindow* pParent, void* pUserData )
		: CGUIWindow( pType, pName, vStyle, rRect, pParent, pUserData ), mExpanded( true ), mSelItem( -1 ), mExpandState( 0 )
	{
		mWindowStyle &= ~GUI_WS_CAPTION;
		mWindowStyle &= ~GUI_WS_BORDER;
		mWindowStyle &= ~GUI_WS_THICKFRAME;
		mWindowStyle &= ~GUI_WS_CANMOVE;
		mWindowStyle &= ~GUI_WS_SYSMENU;
	}

protected:
	CGUIRect GetListRect( ) const;
	CGUIRect GetComboRect( ) const;
	bool IsComboExpanded( ) const			{ return mExpanded; }
	void ComboExpand( );
	void ComboExpand( bool vExpand );
	void ExpandUp( );
	void ExpandDown( );

public:
	CGUIListBox* GetListBox( ) const		{ return mpList; }
	int AppendItem( const char* pText )		{ return mpList->AppendItem( pText ); }
	int GetItemData( int vItemIndex) const	{ return mpList->GetItemData( vItemIndex ); }
	int SetItemData( int vItemIndex, unsigned int vItemData ) { mpList->SetItemData( vItemIndex, vItemData ); }
	int GetCurSel( ) const	{ return mSelItem; }
	void SetCurSel( int vItemIndex )		
	{ 
		mSelItem = vItemIndex;
		mTitle = ( (CGUIListBox*) mpList )->GetItemText( mSelItem );
	}
	void SetSelColour( int vColour ) { mpList->SetSelColour( vColour ); }
	void SetListTextColour( int vColour ) { mpList->SetTextColour( vColour ); }

protected:
	virtual int OnWindowCreate( );
	virtual void OnWindowRender( );
	virtual void OnLoadResource( );
	virtual void OnDestroyResource( );
	virtual void OnWindowFocus( CGUIWindow* pWindow, bool vFocus );
	
protected:
	void OnExpandClicked( CGUIWindow* pButton );
	void OnListBoxSelChanged( CGUIWindow* pListBox, int vCurSel );
	void OnExpandBtnFocus( CGUIWindow* pWindow, bool vFocus );
	void OnListFocus( CGUIWindow* pWindow, bool vFocus );
};

// ************************************************************ //
// CControlProgress
// ************************************************************ //

class CGUIProgress : public CGUIWindow
{
public:
	int		mRangeMin;
	int		mRangeMax;
	int		mPos;

public:
	CGUIProgress( const char* pType, const char* pName, unsigned int vStyle, const CGUIRect &rRect, CGUIWindow* pParent, void* pUserData )
		: CGUIWindow( pType, pName, vStyle, rRect, pParent, pUserData ), mRangeMin( 0 ), mRangeMax( 100 ), mPos( 0 )
	{
		mWindowStyle &= ~GUI_WS_CAPTION;
		mWindowStyle &= ~GUI_WS_BORDER;
		mWindowStyle &= ~GUI_WS_THICKFRAME;
		mWindowStyle &= ~GUI_WS_CANMOVE;
		mWindowStyle &= ~GUI_WS_SYSMENU;
	}

public:
	void SetPos( int vPos )							{ mPos = vPos; }
	void SetRange( int vRangeMin, int vRangeMax )	{ mRangeMin = vRangeMin; mRangeMax = vRangeMax; }
	int GetPos( )									{ return mPos; }

protected:
	virtual void OnDrawShade( CGUICanvas* pCanvas );
	virtual void OnDrawSlider( CGUICanvas* pCanvas, const CGUIRect& rRect );

protected:
	virtual void OnWindowRender( );
	virtual void OnLoadResource( );
	virtual void OnDestroyResource( );
	virtual void OnMouseLButtonDown( const CGUIPoint& rPoint, unsigned int vFlag );
	virtual void OnMouseMove( const CGUIPoint& rPoint, unsigned int vFlag );
	virtual void OnMouseLButtonUp( const CGUIPoint& rPoint, unsigned int vFlag );
};

// ************************************************************ //
// CGUIMessageBox
// 事件：所有的窗口事件 & ON_WM_COMMAND
// ************************************************************ //

class CGUIMessageBox : public CGUIFrame
{
	class CMessageItem
	{
	public:
		std::string	mText;
	};

protected:
	std::vector< CMessageItem* >	mItems;
	CGUIButton*						mpYes;
	CGUIButton*						mpNo;

public:
	CGUIMessageBox( const char* pType, const char* pName, unsigned int vStyle, const CGUIRect& rRect, CGUIWindow* pParent, void* pUserData ) 
		: CGUIFrame( pType, pName, vStyle | GUI_WS_POPUP, rRect, pParent, pUserData ), mpYes( NULL ), mpNo( NULL )
		{	}

public:
	virtual ~CGUIMessageBox( )
	{
		for ( int i = 0; i < int( mItems.size( ) ); i ++ )
			delete mItems[ i ];
	}

public:
	void SetMessage( const char* pText );

public:
	virtual void OnWindowRender( );
	virtual int OnWindowCreate( );

protected:
	void OnClickYes( CGUIWindow* pButton );
	void OnClickNo( CGUIWindow* pButton );

};

// ************************************************************ //
// CControlTab
// ************************************************************ //

class CGUITab : public CGUIWindow
{
protected:
	class CTabItem
	{
		friend class CGUITab;

	protected:
		std::string		mText;
		CGUIRect		mRect;
		bool			mIsSelected;

	public:
		CTabItem( const char* pText ) : mText( pText ), mIsSelected( false )	{	}
	public:
		~CTabItem( )	{	}
	};

protected:
	std::vector< CTabItem* >	mItems;
	int							mCurSel;

public:
	CGUITab( const char* pType, const char* pName, unsigned int vStyle, const CGUIRect& rRect, CGUIWindow* pParent, void* pUserData )
		: CGUIWindow( pType, pName, GUI_WS_NORMAL, rRect, pParent, pUserData ), mCurSel( -1 )	{	}
public:
	virtual ~CGUITab( );

public:
	void AppendItem( const char* pText );
	void SetCurSel( int vIndex );
	int GetCurSel( ) const { return mCurSel; }

protected:
	void OnDrawItem( CGUICanvas* pCanvas, const char* pText, const CGUIRect& rRect, bool vIsSelected );
	void OnDrawTab( CGUICanvas* pCanvas, const CGUIRect& rRect );

protected:
	virtual void OnWindowSized( const CGUIPoint& rOffset );
	virtual void OnMouseLButtonDown( const CGUIPoint& rPoint, unsigned int vFlags );
	virtual void OnWindowRender( );
	virtual void OnLoadResource( );
	virtual void OnDestroyResource( );
};

// ************************************************************ //
// CGUIColourDialog
// 事件：所有的窗口事件 & ON_WM_COMMAND
// ************************************************************ //

class CGUIColourDialog : public CGUIFrame
{
protected:
	CGUITexture*	mp2DTexture;
	CGUITexture*	mpBrightTexture;
	int				mCurH;
	int				mCurS;
	int				mCurL;
	int				mClickArea;
	CGUIEdit*		mpHEdit;
	CGUIEdit*		mpSEdit;
	CGUIEdit*		mpLEdit;
	CGUIEdit*		mpREdit;
	CGUIEdit*		mpGEdit;
	CGUIEdit*		mpBEdit;
	CGUIButton*		mpOkBtn;
	CGUIButton*		mpCancelBtn;

public:
	CGUIColourDialog( const char* pType, const char* pName, unsigned int vStyle, const CGUIRect& rRect, CGUIWindow* pParent, void* pUserData ) 
		: CGUIFrame( pType, pName, GUI_WS_POPUP, CGUIRect( rRect.mTopLeft, CGUISize( 305, 415 ) ), pParent, pUserData )
		, mp2DTexture( NULL ), mpBrightTexture( NULL ), mCurH( 0 ), mCurS( 0 ), mCurL( 0 ), mClickArea( 0 )
	{	}
public:
	unsigned int GetRGBColour( );

protected:
	void FillTexture( );
	void PrintRGBValue( );
	void UpdateColour( const CGUIPoint& rPoint );
	unsigned int HSLToRGB( unsigned int vColourHSL );

protected:
	virtual void OnWindowRender( );
	virtual int OnWindowCreate( );
	virtual void OnLoadResource( );
	virtual void OnDestroyResource( );
	virtual void OnMouseMove( const CGUIPoint& rPoint, unsigned int vFlag );
	virtual void OnMouseLButtonDown( const CGUIPoint& rPoint, unsigned int vFlag );
	virtual void OnMouseLButtonUp( const CGUIPoint& rPoint, unsigned int vFlag );

protected:
	void OnOk( CGUIWindow* pButton );
	void OnHEditChanged( CGUIWindow* pButton );
	void OnSEditChanged( CGUIWindow* pButton );
	void OnLEditChanged( CGUIWindow* pButton );
};

// ************************************************************ //
// CGUIFileDialog
// 事件：所有的窗口事件 & ON_WM_COMMAND
// ************************************************************ //

class CGUIFileDialog : public CGUIFrame
{
protected:
	std::string					mCurrentPath;
	std::vector<std::string>	mFileList;
	CGUIList*					mpFileList;
	CGUIEdit*					mpFileName;
	CGUIButton*					mpUpFolder;
	CGUIButton*					mpOk;

public:
	CGUIFileDialog( const char* pType, const char* pName, unsigned int vStyle, const CGUIRect& rRect, CGUIWindow* pParent, void* pUserData ) 
		: CGUIFrame( pType, pName, GUI_WS_POPUP, CGUIRect( rRect.mTopLeft, CGUISize( 600, 400 ) ), pParent, pUserData )
	{	}

public:
	virtual void OnWindowRender( );
	virtual int OnWindowCreate( );

public:
	std::string GetFilePath( ) const;
	std::string GetFileName( ) const;

protected:
	void RefreshFileList( );

protected:
	void OnUpDirectory( CGUIWindow* );
	void OnFileListDblClk( CGUIWindow* pWindow, int vItemIndex );
	void OnFileListSelChanged( CGUIWindow* pList, int vItemIndex );
	void OnClickOK( CGUIWindow* pButton );
};

// ************************************************************ //
// CCGUIHyper
// ************************************************************ //

class CGUIStatic : public CGUIWindow
{
protected:
	class CStaticItem
	{
	public:
		unsigned int			mColour;
		std::string				mText;
		std::vector< CGUIRect >	mRects;
		char					mOpr[ 32 ];	// 操作符
		int						mData1;		// 操作数1
		int						mData2;		// 操作数2
		bool					mIsHover;
		bool					mIsClicked;
	public:
		CStaticItem( ) : mColour( 0xFFFFFFFF ), mData1( 0 ), mData2( 1 ), mIsHover( false ), mIsClicked( false )
			{ ::memset( mOpr, 0, sizeof( mOpr ) ); }
	};

public:
	std::vector< CStaticItem >	mTexts;

public:
	CGUIStatic( const char* pType, const char* pName, unsigned int vStyle, const CGUIRect& rRect, CGUIWindow* pParent, void* pUserData )
		: CGUIWindow( pType, pName, GUI_WS_VSCROLL, rRect, pParent, pUserData )	{	}

public:
	// <CLS>删除本屏幕
	// <Ca,r,g,b,Link:Send 1,2></C>表示颜色
	// '\n' 表示换行
	// pText 以0结束
	void Printf( const char* pText );
	void Clear( );

protected:
	int ClearPrefix( const char* pText, int vIndex );
	int PositionPrefix( const char* pText, int vIndex );
	int PositionEndmark( const char* pText, int vIndex );
	int ColorPrefix( const char* pText, int vIndex );
	int ColorEndmark( const char* pText, int vIndex );
	int GetPosition( const char* pText, int vIndex, CGUIPoint& rPoint );
	int GetValue( const char* pText, int vIndex, int& rValue );
	int GetColor( const char* pText, int vIndex, unsigned int& vColor );
	int LinkPrefix( const char* pText, int vIndex );
	int GetLinkInfo( const char* pText, int vIndex, char* pOpr, int& rData1, int& rData2 );
	void RefreshHyper( );
	CStaticItem* HitTest( const CGUIPoint& rPoint );

protected:
	virtual void OnWindowRender( );
	virtual void OnMouseLButtonDown( const CGUIPoint& rPoint, unsigned int vFlags );
	virtual void OnMouseMove( const CGUIPoint& rPoint, unsigned int vFlags );
	virtual void OnMouseLButtonUp( const CGUIPoint& rPoint, unsigned int vFlags );
	virtual void ExecuteLink( const CStaticItem* tpHyper );
	virtual void OnDrawItem( CGUICanvas* pCanvas, CGUIPoint& rStartPoint, const CStaticItem* tpHyper );
};

// ************************************************************ //
// CGUIDebug
// ************************************************************ //

class CGUIDebug : public CGUIFrame
{
protected:
	CGUIEdit*				mpDebugEdit;
	CGUITab*				mpDebugTab;
	CGUIStatic*				mpDebugStatic;

public:
	CGUIDebug( const char* pType, const char* pName, int vStyle, const CGUIRect& rRect, CGUIWindow* pParent, void* pUserData )
		: CGUIFrame( pType, pName, GUI_WS_THICKFRAME | GUI_WS_POPUP, rRect, pParent, pUserData ),
		mpDebugEdit( NULL ), mpDebugTab( NULL ), mpDebugStatic( NULL )	{	}
public:
	virtual ~CGUIDebug( )	{	}

public:
	void Refresh( const char* pDebugText, int vDebugType );

protected:
	void OnTabSelectChanged( CGUIWindow* pTabWindow, int vCurSel );

public:
	virtual int OnWindowCreate( );
};

GUI_DYNCREATE( GUIDebug )
GUI_DYNCREATE( GUICustom )
GUI_DYNCREATE( GUIStatic )
GUI_DYNCREATE( GUIFileDialog )
GUI_DYNCREATE( GUIColourDialog )
GUI_DYNCREATE( GUITab )
GUI_DYNCREATE( GUIMessageBox )
GUI_DYNCREATE( GUIProgress )
GUI_DYNCREATE( GUIComboBox )
GUI_DYNCREATE( GUIList )
GUI_DYNCREATE( GUITree )
GUI_DYNCREATE( GUIMenu )
GUI_DYNCREATE( GUIListBox )
GUI_DYNCREATE( GUIEdit )
GUI_DYNCREATE( GUIScrollBar )
GUI_DYNCREATE( GUIImageCheck )
GUI_DYNCREATE( GUIImageButton )
GUI_DYNCREATE( GUICheck )
GUI_DYNCREATE( GUIButton )
GUI_DYNCREATE( GUIFrame )
GUI_DYNCREATE( GUISheet )

};

};