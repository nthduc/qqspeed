#pragma once

#define GUI_DYNCREATE( Name )									\
	class C##Name##Factory : public CGUIWindowFactory			\
	{															\
	public:														\
	C##Name##Factory( ) : CGUIWindowFactory( #Name )	{	}	\
	public:														\
		virtual ~C##Name##Factory( )	{	}					\
	public:														\
		virtual CGUIWindow* Create( const char* pName, unsigned int vStyle, const CGUIRect& rRect, CGUIWindow* pParent, void* pUserData )	\
			{ return new C##Name( mType.c_str( ), pName, vStyle, rRect, pParent, pUserData ); }	\
		virtual void Destroy( CGUIWindow* pWindow )				\
		{														\
			if ( strcmp( pWindow->GetType( ).c_str( ), mType.c_str( ) ) == 0 )	\
				delete pWindow;									\
		}														\
		std::string GetType( ) const { return mType; }			\
	};															\

#define GUI_LODWORD( value )		( ( value ) & 0x0000FFFF )
#define GUI_HIDWORD( value )		( ( ( value ) & 0xFFFF0000 ) >> 16 )
#define GUI_MAKEDWORD( high, low )	( ( ( high ) << 16 ) | ( low )  )
#define GUI_MAKECOLOUR( tAlpha, tRed, tGreen, tBlue ) ( tAlpha ) << 24 | ( tRed ) << 16 | ( tGreen ) << 8 | ( tBlue )
#define GUI_MIN( a, b )	( ( a ) > ( b ) ? ( b ) : ( a ) )
#define GUI_MAX( a, b )	( ( a ) > ( b ) ? ( a ) : ( b ) )
#define GUI_DELETEPTR( p )       { if ( p ) { delete ( p ); ( p ) = NULL; } }
#define GUI_DELETE_ARRAY( p ) { if ( p ) { delete [ ] ( p ); ( p ) = NULL; } }
#define GUI_RELEASE( p )      { if ( p ) { ( p )->Release( ); ( p ) = NULL; } }
#define NULL		0
#define WHEEL_DELTA 120

#define GUI_LOADTEXTURE( pTexture, pTexName )	\
	if ( pTexture == NULL )						\
	{											\
		CGUIRenderDriver* tpRenderDriver = CGUISystem::GetSingletonPtr( )->GetRenderDriver( );	\
		pTexture = tpRenderDriver->LoadTexture( pTexName );		\
	}	\

#define GUI_DESTROYTEXTURE( pTexture )	GUI_DELETEPTR( pTexture )

#define GUI_WINDOW_BORDER_WIDTH				4
#define GUI_WINDOW_BORDER_HEIGHT			4
#define GUI_WINDOW_CLOSEBUTTON_WIDTH		16
#define GUI_WINDOW_CLOSEBUTTON_HEIGHT		16
#define GUI_VSCROLLBAR_WIDTH				16		//! 纵向滚动条宽度
#define GUI_HSCROLLBAR_HEIGHT				16		//! 横向滚动条高度
#define GUI_VSCROLLBAR_PAGEBTN_HEIGHT		16		//! 纵向滚动条翻页按钮高度
#define GUI_HSCROLLBAR_PAGEBTN_WIDTH		16		//! 横向滚动条翻页按钮宽度
#define	GUI_VSCROLLBAR_THUMB_HEIGHT			14		//! 纵向滚动条滚动按钮高度
#define	GUI_HSCROLLBAR_THUMB_WIDTH			14		//! 横向滚动条滚动按钮宽度
#define GUI_VSCROLLBAR_SLIDER_HEIGHT		30		//! 纵向滚动条滑道高度
#define GUI_HSCROLLBAR_SLIDER_WIDTH			30		//! 横向滚动条滑道宽度
#define GUI_MENU_BORDER_WIDTH				4		//! 菜单横向边宽
#define GUI_MENU_BORDER_HEIGHT				4		//! 菜单纵向边宽
#define GUI_LIST_SORTBOUND_WIDTH			18
#define GUI_LIST_SORT_WIDTH					16
#define GUI_LIST_SORT_HEIGHT				16
#define GUI_COMBOBOX_BUTTON_WIDTH			16		//! 组合框按钮宽度
#define GUI_TAB_INTERVAL_WIDTH				4		//! 分页框按钮间隔

#define ON_BN_CLICKED( Window, MemberFx )	{	\
			Window->AddEvent( new CGUIEvent( CGUISinkClickButton::sEventType, this,	\
			(CGUIEvent::MemberFunc) (CGUISinkClickButton::ClickButtonProc) ( MemberFx ) ) );	\
		}	\

#define ON_WM_WINDOWFOCUS( Window, MemberFx ) {	\
			Window->AddEvent( new CGUIEvent( CGUISinkWindowFocus::sEventType, this,	\
			(CGUIEvent::MemberFunc) (CGUISinkWindowFocus::WindowFocusProc) ( MemberFx ) ) );	\
		}	\

#define ON_WM_WINDOWKEYDOWN( Window, MemberFx ) {	\
			Window->AddEvent( new CGUIEvent( CGUISinkWindowKeyDown::sEventType, this,	\
			(CGUIEvent::MemberFunc) (CGUISinkWindowKeyDown::WindowKeyDown) ( MemberFx ) ) );	\
		}	\

#define ON_WM_WINDOWKEYUP( Window, MemberFx ) {	\
			Window->AddEvent( new CGUIEvent( CGUISinkWindowKeyUp::sEventType, this,	\
			(CGUIEvent::MemberFunc) (CGUISinkWindowKeyUp::WindowKeyUp) ( MemberFx ) ) );	\
		}	\

#define ON_WM_WINDOWSETFONT( Window, MemberFx ) {	\
			Window->AddEvent( new CGUIEvent( CGUISinkWindowSetFont::sEventType, this,	\
			(CGUIEvent::MemberFunc) (CGUISinkWindowSetFont::WindowKeyUp) ( MemberFx ) ) );	\
		}	\

#define ON_WM_LBUTTONDBLCLK( Window, MemberFx ) {	\
			Window->AddEvent( new CGUIEvent( CGUISinkLButtonDblClk::sEventType, this, \
			(CGUIEvent::MemberFunc) (CGUISinkLButtonDblClk::LButtonDblClkProc)( MemberFx ) ) ); \
		}	\

#define ON_WM_LBUTTONDOWN( Window, MemberFx ) {	\
			Window->AddEvent( new CGUIEvent( CGUISinkLButtonDown::sEventType, this, \
			(CGUIEvent::MemberFunc) (CGUISinkLButtonDown::LButtonDownProc)( MemberFx ) ) ); \
		}	\

#define ON_WM_MOUSEMOVE( Window, MemberFx )	{	\
			Window->AddEvent( new CGUIEvent( CGUISinkMouseMove::sEventType, this,	\
			(CGUIEvent::MemberFunc) (CGUISinkMouseMove::MouseMoveProc)( MemberFx ) ) );	\
		}	\

#define ON_WM_LBUTTONUP( Window, MemberFx )	{	\
			Window->AddEvent( new CGUIEvent( CGUISinkLButtonUp::sEventType, this,	\
			(CGUIEvent::MemberFunc) (CGUISinkLButtonUp::LButtonUpProc)( MemberFx ) ) );	\
		}	\

#define ON_WM_DRAGFROM( Window, MemberFx ) {	\
		Window->AddEvent( new CGUIEvent( CGUISinkWindowDragFrom::sEventType, this, \
			(CGUIEvent::MemberFunc) (CGUISinkWindowDragFrom::DragFromProc)( MemberFx ) ) ); \
		}	\

#define ON_WM_DRAGTO( Window, MemberFx ) {	\
			Window->AddEvent( new CGUIEvent( CGUISinkWindowDragTo::sEventType, this,	\
			(CGUIEvent::MemberFunc) (CGUISinkWindowDragTo::DragToProc)( MemberFx ) ) );	\
		}	\

#define ON_SB_THUMBCHANGED( Window, MemberFx )	{	\
			Window->AddEvent( new CGUIEvent( CGUISinkThumbChanged::sEventType, this,\
			(CGUIEvent::MemberFunc) (CGUISinkThumbChanged::ThumbChangedProc)( MemberFx ) ) ); \
		}	\

#define ON_LB_SELCHANGED( Window, MemberFx )	{	\
			Window->AddEvent( new CGUIEvent( CGUISinkListBoxSelChanged::sEventType, this,\
			(CGUIEvent::MemberFunc) (CGUISinkListBoxSelChanged::ListBoxSelChangedProc)( MemberFx ) ) ); \
		}	\

#define ON_WM_COMMAND( Window, MemberFx )		{	\
			Window->AddEvent( new CGUIEvent( CGUISinkCommand::sEventType, this,	\
			(CGUIEvent::MemberFunc) (CGUISinkCommand::CommandProc)( MemberFx ) ) );	\
		}	\

#define ON_LC_SELCHANGED( Window, MemberFx )	{	\
			Window->AddEvent( new CGUIEvent( CGUISinkListSelChanged::sEventType, this,\
			(CGUIEvent::MemberFunc) (CGUISinkListSelChanged::ListSelChangedProc)( MemberFx ) ) ); \
		}	\

#define ON_LC_DBLCLK( Window, MemberFx )	{	\
			Window->AddEvent( new CGUIEvent( CGUISinkListDblClk::sEventType, this,\
			(CGUIEvent::MemberFunc) (CGUISinkListDblClk::ListDblClkProc)( MemberFx ) ) ); \
		}	\

#define ON_EN_CHANGED( Window, MemberFx )	{	\
			Window->AddEvent( new CGUIEvent( CGUISinkEditChanged::sEventType, this,\
			(CGUIEvent::MemberFunc) (CGUISinkEditChanged::EditChangedProc)( MemberFx ) ) ); \
		}	\

#define ON_PS_POSCHANGED( Window, MemberFx )	{	\
			Window->AddEvent( new CGUIEvent( CGUISinkSliderPosChanged::sEventType, this,\
			(CGUIEvent::MemberFunc) (CGUISinkSliderPosChanged::SliderPosChanged)( MemberFx ) ) ); \
		}	\

#define ON_TC_SELCHANGED( Window, MemberFx )	{	\
		Window->AddEvent( new CGUIEvent( CGUISinkTabSelChanged::sEventType, this,\
			(CGUIEvent::MemberFunc) (CGUISinkTabSelChanged::TabSelChanged)( MemberFx ) ) ); \
		}	\

// 错误码
#define	GUI_ERR_SUCCESS				0			// 调用成功
#define GUI_ERR_OUTOFCODEPAGE		-1
#define GUI_ERR_EVENTNOTFOUND		-1
#define GUI_ERR_OUTOFVERTEX			-1 

// 鼠标按键
#define GUI_LBUTTON					0x00000001
#define GUI_RBUTTON					0x00000002
#define GUI_MBUTTON					0x00000010
#define GUI_BACK					0x00000008
#define	GUI_RETURN					0x0000000D
#define GUI_TAB						0x00000009
#define GUI_HOME					0x00000024
#define GUI_END						0x00000023
#define GUI_UP						0x00000026
#define GUI_DOWN					0x00000028
#define GUI_LEFT					0x00000025
#define GUI_RIGHT					0x00000027
#define GUI_ESCAPE					0x0000001B
#define GUI_SHIFT					0x00000010
#define GUI_CONTROL					0x00000011
#define GUI_DELETE					0x0000002E
#define GUI_PAGEUP					0x00000021
#define GUI_PAGEDOWN				0x00000022

// 字体输出控制
#define GUI_DT_SINGLELINE			0x00000001			// 单行输出
#define GUI_DT_LEFT					0x00000002			// 左对齐
#define GUI_DT_RIGHT				0x00000004			// 右对齐
#define GUI_DT_TOP					0x00000008			// 上对齐
#define	GUI_DT_BOTTOM				0x00000010			// 下对齐
#define GUI_DT_VCENTER				0x00000020			// 纵向居中
#define GUI_DT_HCENTER				0x00000040			// 横向居中
#define GUI_DT_WRAPTEXT				0x00000080			// 包裹文本，如果超出指定宽度，会用...代表
// 窗口状态
#define GUI_WF_VISIBLE				0x00000001			// 窗口是否可见，如果窗口不可见将不会被渲染，也不会相应系统消息
#define GUI_WF_ENABLE				0x00000002			// 窗口是否可用，如果窗口不可用仍然会被渲染，但不会相应系统消息
#define GUI_WF_ACTIVE				0x00000004			// 窗口是否激活，如果窗口被激活，会有不一样的渲染，逻辑层上不同

// 按钮状态
#define GUI_BF_HOVER				0x00010000			// 鼠标移到按钮上面
#define GUI_BF_CLICKED				0x00020000			// 鼠标点击按钮

// 窗口风格
#define GUI_WS_NORMAL				0x00000000			// 不可改变控件大小,没有关闭按钮
#define GUI_WS_TABSTOP				0x00000001			// 可以通过Tab获得焦点
#define	GUI_WS_HSCROLL				0x00000002			// 是否显示横向滚动条
#define	GUI_WS_VSCROLL				0x00000004			// 是否显示纵向滚动条
#define GUI_WS_CANMOVE				0x00000008			// 可以移动
#define GUI_WS_BORDER				0x00000010			// 是否有边
#define GUI_WS_CAPTION				0x00000020			// 窗口标题
#define GUI_WS_SYSMENU				0x00000040			// 有关闭按钮
#define GUI_WS_THICKFRAME			0x00000080			// 是不是可以改变大小
#define GUI_WS_POPUP				0x00000100			// 窗口不和父窗口做裁减
#define GUI_WS_CHILD				0x00000200			// 如果没有指定GUI_WS_POPUP, 默认将带有该风格, 窗口将和所有父窗口做裁减
#define GUI_WS_TABGROUP				0x00000400			// 窗口是否为TAB组

// 滚动条风格
#define GUI_SBS_VERT				0x00010000			// 纵向滚动条
#define GUI_SBS_HORZ				0x00020000			// 横向滚动条

// 输入框风格
#define GUI_ES_AUTOHSCROLL			0x00010000			// 可以自动横滚
#define	GUI_ES_AUTOVSCROLL			0x00020000			// 可以自动纵滚, 必须和ESF_MUTILINE配合使用
#define	GUI_ES_MUTILINE				0x00040000			// 多行编辑
#define	GUI_ES_SINGLELINE			0x00080000			// 单行编辑, 该标记不能和ESF_MUTILINE,ESF_AUTOVSCROLL同时使用
#define GUI_ES_READONLY				0x00100000			// 只读不可写
#define GUI_ES_PASSWORD				0x00200000			// 密码输入框

// 窗口锚
#define GUI_LEFTARCHOR_LEFT			0x00000001			// 左边
#define GUI_LEFTARCHOR_RIGHT		0x00000002			// 右边
#define GUI_TOPARCHOR_TOP			0x00000004			// 顶边
#define GUI_TOPARCHOR_BOTTOM		0x00000008			// 底边
#define GUI_RIGHTARCHOR_LEFT		0x00000010			// 左边
#define GUI_RIGHTARCHOR_RIGHT		0x00000020			// 右边
#define GUI_BOTTOMARCHOR_TOP		0x00000040			// 顶边
#define GUI_BOTTOMARCHOR_BOTTOM		0x00000080			// 底边

// 多列列表框风格
#define GUI_LCS_LEFT				0x00010000
#define GUI_LCS_RIGHT				0x00020000
#define GUI_LCS_CENTER				0x00040000

// 列表框风格
#define GUI_LBS_MOUSEOVER			0x00010000		

// 菜单风格
#define GUI_MS_POPUP				0x00000001			// 有子菜单
#define GUI_MS_SEPARATOR			0x00000002			// 分割条

// 进度条风格
#define GUI_PS_SLIDER				0x00010000			// 

// 进度条风格
#define GUI_CBS_DOWNEXPAND			0x00010000			// 
#define GUI_CBS_UPEXPAND			0x00020000			// 

// 消息框风格
#define GUI_MBS_OK					0x00010000
#define GUI_MBS_CANCEL				0x00020000

// 数据类型
#define GUI_INT						0x00000000
#define GUI_UINT					0x00000001
#define GUI_BOOL					0x00000002
#define GUI_FLOAT					0x00000003
#define GUI_CHAR					0x00000005
#define GUI_PCHAR					0x00000006
#define GUI_PVOID					0x00000007
#define GUI_PPOINT					0x00000008
#define GUI_PSIZE					0x00000009
#define GUI_PRECT					0x0000000A
#define GUI_PCANVAS					0x0000000B
#define GUI_PWINDOW					0x0000000C
#define GUI_PFONT					0x0000000D

// 系统命令 CGUISinkCommand
#define GUI_CMD_FILEDIALOGCLOSED	0x80000001
#define GUI_CMD_COLOURDIALOGCLOSED	0x80000002
#define GUI_CMD_MESSAGEYES			0x80000003
#define GUI_CMD_MESSAGENO			0x80000004