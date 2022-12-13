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
#define GUI_VSCROLLBAR_WIDTH				16		//! ������������
#define GUI_HSCROLLBAR_HEIGHT				16		//! ����������߶�
#define GUI_VSCROLLBAR_PAGEBTN_HEIGHT		16		//! �����������ҳ��ť�߶�
#define GUI_HSCROLLBAR_PAGEBTN_WIDTH		16		//! �����������ҳ��ť���
#define	GUI_VSCROLLBAR_THUMB_HEIGHT			14		//! ���������������ť�߶�
#define	GUI_HSCROLLBAR_THUMB_WIDTH			14		//! ���������������ť���
#define GUI_VSCROLLBAR_SLIDER_HEIGHT		30		//! ��������������߶�
#define GUI_HSCROLLBAR_SLIDER_WIDTH			30		//! ����������������
#define GUI_MENU_BORDER_WIDTH				4		//! �˵�����߿�
#define GUI_MENU_BORDER_HEIGHT				4		//! �˵�����߿�
#define GUI_LIST_SORTBOUND_WIDTH			18
#define GUI_LIST_SORT_WIDTH					16
#define GUI_LIST_SORT_HEIGHT				16
#define GUI_COMBOBOX_BUTTON_WIDTH			16		//! ��Ͽ�ť���
#define GUI_TAB_INTERVAL_WIDTH				4		//! ��ҳ��ť���

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

// ������
#define	GUI_ERR_SUCCESS				0			// ���óɹ�
#define GUI_ERR_OUTOFCODEPAGE		-1
#define GUI_ERR_EVENTNOTFOUND		-1
#define GUI_ERR_OUTOFVERTEX			-1 

// ��갴��
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

// �����������
#define GUI_DT_SINGLELINE			0x00000001			// �������
#define GUI_DT_LEFT					0x00000002			// �����
#define GUI_DT_RIGHT				0x00000004			// �Ҷ���
#define GUI_DT_TOP					0x00000008			// �϶���
#define	GUI_DT_BOTTOM				0x00000010			// �¶���
#define GUI_DT_VCENTER				0x00000020			// �������
#define GUI_DT_HCENTER				0x00000040			// �������
#define GUI_DT_WRAPTEXT				0x00000080			// �����ı����������ָ����ȣ�����...����
// ����״̬
#define GUI_WF_VISIBLE				0x00000001			// �����Ƿ�ɼ���������ڲ��ɼ������ᱻ��Ⱦ��Ҳ������Ӧϵͳ��Ϣ
#define GUI_WF_ENABLE				0x00000002			// �����Ƿ���ã�������ڲ�������Ȼ�ᱻ��Ⱦ����������Ӧϵͳ��Ϣ
#define GUI_WF_ACTIVE				0x00000004			// �����Ƿ񼤻������ڱ�������в�һ������Ⱦ���߼����ϲ�ͬ

// ��ť״̬
#define GUI_BF_HOVER				0x00010000			// ����Ƶ���ť����
#define GUI_BF_CLICKED				0x00020000			// �������ť

// ���ڷ��
#define GUI_WS_NORMAL				0x00000000			// ���ɸı�ؼ���С,û�йرհ�ť
#define GUI_WS_TABSTOP				0x00000001			// ����ͨ��Tab��ý���
#define	GUI_WS_HSCROLL				0x00000002			// �Ƿ���ʾ���������
#define	GUI_WS_VSCROLL				0x00000004			// �Ƿ���ʾ���������
#define GUI_WS_CANMOVE				0x00000008			// �����ƶ�
#define GUI_WS_BORDER				0x00000010			// �Ƿ��б�
#define GUI_WS_CAPTION				0x00000020			// ���ڱ���
#define GUI_WS_SYSMENU				0x00000040			// �йرհ�ť
#define GUI_WS_THICKFRAME			0x00000080			// �ǲ��ǿ��Ըı��С
#define GUI_WS_POPUP				0x00000100			// ���ڲ��͸��������ü�
#define GUI_WS_CHILD				0x00000200			// ���û��ָ��GUI_WS_POPUP, Ĭ�Ͻ����и÷��, ���ڽ������и��������ü�
#define GUI_WS_TABGROUP				0x00000400			// �����Ƿ�ΪTAB��

// ���������
#define GUI_SBS_VERT				0x00010000			// ���������
#define GUI_SBS_HORZ				0x00020000			// ���������

// �������
#define GUI_ES_AUTOHSCROLL			0x00010000			// �����Զ����
#define	GUI_ES_AUTOVSCROLL			0x00020000			// �����Զ��ݹ�, �����ESF_MUTILINE���ʹ��
#define	GUI_ES_MUTILINE				0x00040000			// ���б༭
#define	GUI_ES_SINGLELINE			0x00080000			// ���б༭, �ñ�ǲ��ܺ�ESF_MUTILINE,ESF_AUTOVSCROLLͬʱʹ��
#define GUI_ES_READONLY				0x00100000			// ֻ������д
#define GUI_ES_PASSWORD				0x00200000			// ���������

// ����ê
#define GUI_LEFTARCHOR_LEFT			0x00000001			// ���
#define GUI_LEFTARCHOR_RIGHT		0x00000002			// �ұ�
#define GUI_TOPARCHOR_TOP			0x00000004			// ����
#define GUI_TOPARCHOR_BOTTOM		0x00000008			// �ױ�
#define GUI_RIGHTARCHOR_LEFT		0x00000010			// ���
#define GUI_RIGHTARCHOR_RIGHT		0x00000020			// �ұ�
#define GUI_BOTTOMARCHOR_TOP		0x00000040			// ����
#define GUI_BOTTOMARCHOR_BOTTOM		0x00000080			// �ױ�

// �����б����
#define GUI_LCS_LEFT				0x00010000
#define GUI_LCS_RIGHT				0x00020000
#define GUI_LCS_CENTER				0x00040000

// �б����
#define GUI_LBS_MOUSEOVER			0x00010000		

// �˵����
#define GUI_MS_POPUP				0x00000001			// ���Ӳ˵�
#define GUI_MS_SEPARATOR			0x00000002			// �ָ���

// ���������
#define GUI_PS_SLIDER				0x00010000			// 

// ���������
#define GUI_CBS_DOWNEXPAND			0x00010000			// 
#define GUI_CBS_UPEXPAND			0x00020000			// 

// ��Ϣ����
#define GUI_MBS_OK					0x00010000
#define GUI_MBS_CANCEL				0x00020000

// ��������
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

// ϵͳ���� CGUISinkCommand
#define GUI_CMD_FILEDIALOGCLOSED	0x80000001
#define GUI_CMD_COLOURDIALOGCLOSED	0x80000002
#define GUI_CMD_MESSAGEYES			0x80000003
#define GUI_CMD_MESSAGENO			0x80000004