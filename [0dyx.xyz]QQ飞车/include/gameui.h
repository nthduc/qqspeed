
#ifndef __GAMEUI_H__
#define __GAMEUI_H__

#include <windows.h>

#include "kernel.h"
#include "variant.h"

extern "C"
{
#include "lua/lua.h"
#include "lua/lualib.h"
#include "lua/lauxlib.h"
}

//ÿ����Ļ��ִ�нű���Ŀ
#define UI_SCRN_FILE_MAX 512

//���涨��(�ű���������)
enum
{		

	SCRIPT_FUNC_ON_INIT			  =0,
	SCRIPT_FUNC_ON_DENIT			,
	SCRIPT_FUNC_ON_LOAD				,

	SCRIPT_FUNC_ON_CLICK			,
	SCRIPT_FUNC_ON_DBCLICK          ,
	SCRIPT_FUNC_ON_RCLICK			,

	SCRIPT_FUNC_ON_ESCAPE			,
	SCRIPT_FUNC_ON_ENTER			,
	SCRIPT_FUNC_ON_TAB   			,
	
	SCRIPT_FUNC_ONCHAR				,
	SCRIPT_FUNC_ONKEYDOWN			,			

	SCRIPT_FUNC_ON_OK				,
	SCRIPT_FUNC_ON_CANCEL			,

	SCRIPT_FUNC_ON_UPDATE           ,
	SCRIPT_FUNC_ON_VALUE_CHANGE		,
	
	SCRIPT_FUNC_ON_SELFSHOW   		,
	SCRIPT_FUNC_ON_SELFHIDE   		,

	SCRIPT_FUNC_ON_CHECKED   		,
	SCRIPT_FUNC_ON_GAINFOCUS		,
	SCRIPT_FUNC_ON_LOSTFOCUS		,

	SCRIPT_FUNC_ON_MSGBOX_OK		,
	SCRIPT_FUNC_ON_MSGBOX_CANCEL	,
	
	SCRIPT_FUNC_ON_MOUSEIN			,
	SCRIPT_FUNC_ON_MOUSEOUT			,

	SCRIPT_FUNC_ON_DRAGENTER		,
	SCRIPT_FUNC_ON_DRAGLEAVE		,

	SCRIPT_FUNC_ON_PATHCOMMAND		,
	SCRIPT_FUNC_ON_TIMER			,

	SCRIPT_FUNC_ON_CAPTURE_RELAY	,
	SCRIPT_FUNC_ON_FOCUS_RELAY		,
	SCRIPT_FUNC_ON_HOT_RELAY		,

	SCRIPT_FUNC_ON_PARSE_TOOLTIP	,
	SCRIPT_FUNC_ON_PARSE_INFOTIP	,

	
	SCRIPT_FUNC_ON_OUT_SCREEN		,
	SCRIPT_FUNC_ON_AREAWAKE			,
	
	SCRIPT_FUNC_ON_EDGEAJUST		,

	SCRIPT_FUNC_ON_QUERYDIRTY		,

	SCRIPT_FUNC_ON_QUERYLOCK		,

	SCRIPT_FUNC_ON_ERROR			,

	SCRIPT_FUNC_ON_DEBUG			,


	SYSTEM_SCIRPT_NUM				//���ϵͳ�ű���Ŀ

};


//���涨��(��������)
static const char* UI_Type_Names[]=
{
	
	"WIDGET",
	"BUTTON",
	"CHECK",	
	"SPIN",
	"TABWIN",
	"TABLIST",
	"LABEL",
	"EDIT",
	"KEYEDIT",
	"MENU",
	"CONSOLE",
	"SCREEN",
	"DIALOG",
	"SYSDIALOG",
	"PICLABEL",
	"PICKER",
	"LISTBOX",
	"LISTBASE",
	"LISTFILE",
	"WIDGETLIST",
	"CONTAINER",
	"SWITCHER",
	"SHAPER",
	"PROGRESSBAR",
	"CHANGEBAR",
	"SCROLLBAR",
	"COMBOBOX",
	"MESSAGEBOX",
	"RICHEDIT",
	"MULTIEDIT",
	"TEXTLIST",
	"TREECTRL",
	"ROOT"
		
};

//���涨��(�ű���������)
static const char* g_SystemScriptFuncs[SYSTEM_SCIRPT_NUM]=
{

	"OnInit",		
	"OnDenit",		
	"OnLoad",	

	"OnClick",			
	"OnDBClick",	    
	"OnRClick",		

	"OnEscape",
	"OnEnter",			
	"OnTab",

	"OnChar",			
	"OnKeyDown",		

	"OnOk",			
	"OnCancel",		

	"OnUpdate",	    
	"OnValueChange",	

	"OnSelfShow",		
	"OnSelfHide",

	"OnCheck",
	"OnGainFocus",
	"OnLostFocus",

	"OnMsgBoxOk",		
	"OnMsgBoxCancel",	

	"OnMouseIn",
	"OnMouseOut",
	
	"OnDragEnter",
	"OnDragLeave",

	"OnPathCommand",
	"OnTimer",

	"OnCaptureRelay",
	"OnFocusRelay",
	"OnHotRelay",

	"OnParseTooltip",
	"OnParseInfotip",

	"OnOutScreen",
	"OnAreaWake",

	"OnEdgeAjust",

	"OnQueryDirty",

	"OnQueryLock",

	"OnError",

	"OnDebug",


};


//UIģ��ű�������������ֵ�����ַ���
#define UI_SCRIPT_BUFSIZE 4096  

//UIģ��ű������г��ֵ��ַ���Ĭ�ϳ���(����textlist���ı���󳤶ȵ�)
#define UI_DEFAULT_BUFSIZE 1024

//UIģ����ʱ�ַ�������
#define UI_TEMPORARY_BUFSIZE 128  

//TEXTLISTÿ�и�����Ϣ���ַ�����󳤶�
#define  UI_LINEDATA_LEN 512

#define UI_QUERY_TYPE_NONE	-1

#define UI_DRAG_TYPE_NONE	-1

#define UI_RICHFLAG_NONE 0

#define UI_MSGID_DEFAULT	0
#define UI_ITEM_NONE		-1	


#define UI_EDIT_HISTORY 8

#define UI_ADORNPIC_NUMBER	 8

#define UI_ACCEL_SCRIPT 128

#define UI_TABLE_PARAM_START 3

//Ĭ�ϵ����϶���ʽ
//0:�����϶�
//1:�����϶�
#define  UI_DEFAULT_DRAG_STYLE 1



#define DEFAULT_CURSOR_NAME		"Normal"
#define DEFAULT_HORIZON_SIZER	"Horizon"
#define DEFAULT_VERTICAL_SIZER	"Vertical"
#define DEFAULT_DRAG_CURSOR		"DragCursor"
#define DEFAULT_EDIT_CURSOR		"EditCursor"
#define DEFAULT_LINK_CURSOR		"LinkCursor"
#define BUTTON_CURSOR_NAME		"Click"


#define DEFAULT_TEMPLATE_KEY	"_UIID_"

//���ñ༭��Ĭ�ϵ��м��ļ����Ŀ¼
#define DEFAULT_EDITOR_FOLDER	"UIEditor"

//Ĭ�ϵ���ɫ,����,���,���ӿ��Ʒ��ŵ���ʼ����
//�������ַ�����ֵΪ��ʼ����ʱ,��ʾ�ָ�����Ĭ�ϵ���ɫ,����,���
#define DEFAULT_ICON_BASE	0
#define DEFAULT_COLOR_BASE	0
#define DEFAULT_FONT_BASE	0
#define DEFAULT_STYLE_BASE	0
#define DEFAULT_HLINK_BASE	0
#define DEFAULT_FIELD_BASE	0

//////////////////////////////////////////////////////////////////////////
//�ṩ��gameui��Ļص�����//begin
//////////////////////////////////////////////////////////////////////////
//���涨��(�ص���������)
enum UICallbackEvent
{
	UI_Callback_GetGameCursor	  =0,	
	UI_Callback_DrawCursor			,	
	UI_Callback_EnableIME			,		

	UI_Callback_Encrypt				,//����ؼ��Ľ�
	UI_Callback_Decrypt				,//����ؼ��Ľ�

	UI_Callback_IsKeyChar			,
	UI_Callback_GetKeyChar			,
	UI_Callback_DrawKeyChar			,
	UI_Callback_HitKeyChar			,
	UI_Callback_ValidKeyChar		,

	UI_Callback_IsValidItem			,	

	UI_Callback_QueryItemInfo		,
	UI_Callback_UpdateItemInfo		,

	UI_Callback_ClickItem			,
	UI_Callback_DClickItem			,
	UI_Callback_RClickItem			,
	UI_Callback_MoveItem			,
	UI_Callback_DrawItem			,

	UI_Callback_BeginDragItem		,
	UI_Callback_EndDragItem			,

	UI_Callback_GetItemPixel		,

	UI_Callback_QueryDirty			,

	UI_Callback_DrawCustomBuffer	,
	UI_Callback_SetCustomBuffer		,
	UI_Callback_GetCustomBuffer		,
	UI_Callback_ParseCustomBuffer	,
	UI_Callback_CustomBufferSelfShow,
	UI_Callback_CustomBufferDeinit  ,

	UI_Callback_QueryNode			,
	UI_Callback_QueryNodePath		,

	UI_Callback_PlaySound			,
	
	//�¼������Ŀ
	UI_Callback_MAXNUM,

};

/*
//edit methods
enum
{
	EDIT_NORMAL=0,//Ӣ�ģ����֣�����,����
	EDIT_NUMBER,
	EDIT_CODER,
};
*/

//edit methods
enum
{

	UI_EDITMETHOD_ENGLISH	=1,//Ӣ��
	UI_EDITMETHOD_NUMBER	=2,//����
	UI_EDITMETHOD_SYMBOL	=4,//����
	UI_EDITMETHOD_CHINESE	=8,//����

	UI_EDITMETHOD_VALUE		=16,//��ֵ
	UI_EDITMETHOD_CODER		=32 //����

};


//��Ϊ��ֵ�����,����ֵ��ĳ����Χʱ������
enum
{
	EDIT_STYLE_NONE		=0,
	EDIT_STYLE_KEEPSEL		=1,//��ʹʧȥ����Ҳ����ѡ��
	EDIT_STYLE_KEEPCURSOR	=2,//��ʹʧȥ����Ҳ���ƹ��
	EDIT_STYLE_FLASHCURSOR	=4,//���ƹ���ʱ��,������˸���
	EDIT_STYLE_SMARTCURSOR	=8,//ʧȥ�����״̬,���û���������ڻ��ƹ��,����ƹ��
};

/*
enum
{
	REDIT_STYLE_NONE		=0,
	REDIT_STYLE_KEEPSEL		=1,//��ʹʧȥ����Ҳ����ѡ��
	REDIT_STYLE_KEEPCURSOR	=2,//��ʹʧȥ����Ҳ���ƹ��
	REDIT_STYLE_FLASHCURSOR	=4,//���ƹ���ʱ��,������˸���
	REDIT_STYLE_SMARTCURSOR	=8,//ʧȥ�����״̬,���û���������ڻ��ƹ��,����ƹ��
};


enum
{
	MEDIT_STYLE_NONE		=0,
	MEDIT_STYLE_KEEPSEL		=1,//��ʹʧȥ����Ҳ����ѡ��
	MEDIT_STYLE_KEEPCURSOR	=2,//��ʹʧȥ����Ҳ���ƹ��
	MEDIT_STYLE_FLASHCURSOR	=4,//���ƹ���ʱ��,������˸���
	MEDIT_STYLE_SMARTCURSOR	=8,//ʧȥ�����״̬,���û���������ڻ��ƹ��,����ƹ��
};
*/

enum
{
	MEDIT_ENTER_STYLE_NONE=0,//����enter��
	MEDIT_ENTER_STYLE_QQ=1,//enter�༭��ctrl+enter����OnEnter
	MEDIT_ENTER_STYLE_MSN=2,//enter����OnEnter��ctrl+enter�༭
	MEDIT_ENTER_STYLE_EDIT=3,//enter�༭,ctrl+enter�༭
	MEDIT_ENTER_STYLE_RETURN=4,//enter����OnEnter��ctrl+enter����OnEnter	
};

enum
{

	UI_SHAPEMODE_NONE=0,
	UI_SHAPEMODE_NORMAL=1,

};

enum UI_BTN_STYLE
{
	
	UI_BTN_STYLE_NONE		  =0,
	UI_BTN_STYLE_POPUPMSG	  =1,
	UI_BTN_STYLE_RADIO		  =2,
	UI_BTN_STYLE_AUTO_TOGGLE  =4,//״̬(value)�Զ��л�	
	UI_BTN_STYLE_AUTO_ROUND   =8,//״̬(value)�Զ���ת	
	UI_BTN_STYLE_SPIN		  =16,//���º�ÿ��threshholdʱ�䣬��clickһ��

};


enum 
{

	SCROLLBAR_STYLE_NONE			=	 0, 
	SCROLLBAR_BLOCKBTN_AUTO_SIZE	=	 1,
	SCROLLBAR_BLOCKBTN_DOCK_FLIP	=	 2,
	SCROLLBAR_DIRECTION_FLIP		=	 4,
	SCROLLBAR_RENDER_SLOT			=	 8,
	SCROLLBAR_AUTO_HIDE				=	 16,
	SCROLLBAR_AUTO_DISABLE			=	 32,
	

	SCROLLBAR_BTN_MIN_SIZE			=	 8, 
	SCROLLBAR_BTN_MAX_SIZE			=	 800, 

};

//������ǲ��ܵ�����
enum LISTBOX_STYLE
{
	
	LISTBOX_STYLE_MULTI_SEL=			1<<0,//��ѡ
	LISTBOX_STYLE_ITEMHEIGHT_FIX=		1<<1,//�̶��߶�
	LISTBOX_STYLE_ENUM_ITEM=			1<<3,//����Ŀ����,Ĭ�ϰ����ع���
	LISTBOX_STYLE_WHEEL=				1<<4,//֧��mouse wheel����
	LISTBOX_STYLE_DIRECTORY=			1<<5,//����Ŀ¼��,ֻ��ʾĿ¼
	LISTBOX_STYLE_FILE=					1<<6,//�����ļ���,��ʾĿ¼���ļ�
	LISTBOX_STYLE_CUSTOM_ARRANGE=		1<<7,//�ű�ָ���б���Ŀ��λ��
	LISTBOX_STYLE_DBCLICK_SEL=			1<<8,//˫��ѡ��,���򵥻�ѡ��
	LISTBOX_STYLE_AUTO_COMFORT=			1<<9,//�Զ�comfort
	LISTBOX_STYLE_RICHTEXT=				1<<10,//���ƴ������ַ����ı�
	LISTBOX_STYLE_COLORLIST=			1<<11,//�Ƿ���ɫ�б�
};


enum LISTBOX_ITEM_FLAG
{
	LISTBOX_ITEM_FLAG_NONE			=0,
};	

enum LISTBOX_ITEM_TYPE
{
	LISTBOX_ITEM_TYPE_NONE			=-1,	//��Ч
	LISTBOX_ITEM_TYPE_DIRECTORY		=0,		//Ŀ¼
	LISTBOX_ITEM_TYPE_FILE			=1,		//�ļ�		
		
};	

//������ǲ��ܵ�����
enum WGTLIST_STYLE
{
	
	WGTLIST_STYLE_NONE=			        0,
	WGTLIST_STYLE_HOVER_FOCUS=			1<<0,//���ͣ��
	WGTLIST_STYLE_ENUM_ITEM=			1<<1,//����Ŀ����,Ĭ�ϰ����ع���
	WGTLIST_STYLE_DELETE_ITEM=			1<<2,//ɾ�����ຢ�ӣ�Ĭ��Ϊ���غ��ӣ�
	WGTLIST_STYLE_WHEEL=				1<<3,//֧��mouse wheel����
};

//������ǲ��ܵ�����
enum TEXTLIST_STYLE
{
	TEXTLIST_STYLE_NONE			=0,
	TEXTLIST_STYLE_ALPHAFONT	=1,//���ư�͸������
	TEXTLIST_STYLE_REVERSE		=2,//���ϵ��»����ı�
	TEXTLIST_STYLE_AUTOSCROLL	=4,//�Զ���������������ı�
	TEXTLIST_STYLE_SMARTSCROLL	=8,//�����ı��ײ�
	TEXTLIST_STYLE_DELAYSCROLL	=16,//�Զ������Ĺ�����������ı�
	TEXTLIST_STYLE_BLOCKINMIDDLE	= 32,//���鲻�ڵײ�ʱ������

};

enum TreeCtrl_STYLE
{
	TREECTRL_STYLE_NONE=			0,
	TREECTRL_STYLE_AUTO_COMFORT=	1,//�Զ�comfort
	TREECTRL_STYLE_DRAWGRID=		1<<1,//�Ƿ��������
};

enum TabWin_STYLE
{
	TABWIN_STYLE_NONE			=   0,
	TABWIN_STYLE_MANUAL_CHECK	=	1,//�ֶ�check
};


enum
{

	TABWIN_TYPE_DEFAULT=0,//HotCover��λ��ֱ��ָ��,��ͨ������õ�

	TABWIN_TYPE_DOWN,//HotCover�ڴ��ڵ��·�
	TABWIN_TYPE_UP,//HotCover�ڴ��ڵ��Ϸ�
	TABWIN_TYPE_LEFT,//HotCover�ڴ��ڵ����
	TABWIN_TYPE_RIGHT,//HotCover�ڴ��ڵ��ұ�

};

enum
{

	UI_TIPFLAG_NONE			=0,
	UI_TIPFLAG_INREFWIN		=1,//tooltip�ڲο����Σ����細���е�item����

};

enum
{
	//��Ч����
	UI_SOUND_NONE		=-1,

	UI_SOUND_CLICK		=0,
	UI_SOUND_MOUSEIN	=1,

	UI_SOUND_DRAG		=2,
	UI_SOUND_DROP		=3,

	UI_SOUND_SHOW		=4,
	UI_SOUND_HIDE		=5,

	UI_SOUND_ARRIVE		=6,

	UI_SOUND_NODE_EXTEND	=7,
	UI_SOUND_NODE_COLLAPSE	=8,

	//�������������Ŀ
	UI_SOUND_MAXNUM,

};


//���涨��(��������)
enum WidgetType
{
	WGTTYPE_WIDGET = 0,
	WGTTYPE_BUTTON,
	WGTTYPE_CHECK,	
	WGTTYPE_SPIN,
	WGTTYPE_TABWIN,
	WGTTYPE_TABLIST,
	WGTTYPE_LABEL,
	WGTTYPE_EDIT,
	WGTTYPE_KEYEDIT,
	WGTTYPE_MENU,
	WGTTYPE_CONSOLE,
	WGTTYPE_SCREEN,
	WGTTYPE_DIALOG,
	WGTTYPE_SYSDIALOG,
	WGTTYPE_PICLABEL,
	WGTTYPE_PICKER,
	WGTTYPE_LISTBOX,
	WGTTYPE_LISTBASE,
	WGTTYPE_LISTFILE,
	WGTTYPE_WIDGETLIST,
	WGTTYPE_CONTAINER,
	WGTTYPE_SWITCHER,
	WGTTYPE_SHAPER,
	WGTTYPE_PROGRESSBAR,
	WGTTYPE_CHANGEBAR,
	WGTTYPE_SCROLLBAR,
	WGTTYPE_COMBOBOX,
	WGTTYPE_MESSAGEBOX,
	WGTTYPE_RICHEDIT,
	WGTTYPE_MULTIEDIT,
	WGTTYPE_TEXTLIST,
	WGTTYPE_TREECTRL,
	WGTTYPE_ROOT
};

//���涨��(��鴰������)
enum UIBuddyType
{
	
	BUDDY_WIDGET_NONE=0,
	BUDDY_WIDGET_TIPWGT=1,
	BUDDY_WIDGET_MSGBOX=2,
	BUDDY_WIDGET_CONSOLE=3,
	BUDDY_WIDGET_HOST=4,
	BUDDY_WIDGET_INFOWGT=5,
	BUDDY_WIDGET_BUDDY=6,
	BUDDY_WIDGET_SCROLL_PRIMARY=7,
	BUDDY_WIDGET_SCROLL_SECONDARY=8,
	BUDDY_WIDGET_BLOCK_BUTTON=9,
	BUDDY_WIDGET_BUDDY_SECOND=10,
};

//���ڵĻ��������ʽ
enum UIBuddyStyle
{
	
	BUDDY_STYLE_SELF	=0,//ֻ���Լ������һ�鴰��
	BUDDY_STYLE_FATHER	=1,//�����Լ�������,�Ҳ����͵���Ļ����
	BUDDY_STYLE_SCREEN	=2,//�����Լ�������,�Ҳ����͵���Ļ����
	BUDDY_STYLE_ROOT	=4,//�����Լ�������,�Ҳ����͵�����������

		
};


//���Ʊ�־
enum UIDrawFlag
{	
	DRAWFLAG_NONE				=0,
	DRAWFLAG_WIN_EDGE			=1,
	DRAWFLAG_WIN_FILL			=1<<1,
	DRAWFLAG_CLIENT_EDGE		=1<<2,
	DRAWFLAG_CLIENT_FILL		=1<<3,
	DRAWFLAG_CAPTION_EDGE		=1<<4,
	DRAWFLAG_CAPTION_FILL		=1<<5,
	DRAWFLAG_DRAG_EDGE			=1<<6,
	DRAWFLAG_DRAG_FILL			=1<<7,
	DRAWFLAG_ENABLE_EFFECT		=1<<8,//ֻ����enable��ʱ�����effectͼƬ
	DRAWFLAG_FOCUS_EFFECT		=1<<9,//ֻ����focus��ʱ�����effectͼƬ
	DRAWFLAG_CHECK_EFFECT		=1<<10,//ֻ����check��ʱ�����effectͼƬ
	DRAWFLAG_DRAW_EFFECT		=1<<11,//���ǻ���effectͼƬ
	DRAWFLAG_CLIENTPRIOR		=1<<12,//�ͻ��˻���(custombuffer)������UI����
	DRAWFLAG_FOCUSCAPTION		=1<<13,//����Focus��ʱ��ʹ��m_FocusColor����m_DrawColor���ƴ���caption
	DRAWFLAG_HOTCAPTION			=1<<14,//����mouseover��ʱ��ʹ��m_FocusColor����m_DrawColor���ƴ���caption
	DRAWFLAG_BACKPRIOR			=1<<15,//�ͻ��˻���(custombuffer)���ڱ�������
};

//���涨��(����״̬)
enum WIDGET_STATE
{
	WGTSTATE_NORMAL			= 0,
	WGTSTATE_MOUSEOVER		= 1,	  //highlighted
	WGTSTATE_CLICKDOWN		= 1<<1,   //pushed
	WGTSTATE_FOCUSED		= 1<<2,   //focused
	WGTSTATE_DISABLE		= 1<<3,   //self disabled
	WGTSTATE_CHECKED		= 1<<4,   //checked
	WGTSTATE_DISCARDED		= 1<<5,   //discarded
	WGTSTATE_DRAGGING		= 1<<6,   //dragging
	WGTSTATE_VISIBLE		= 1<<7,   //�����Ƿ��ܼ�
	WGTSTATE_UPDATE_SORTED	= 1<<8,   //�Ѿ��������	
	WGTSTATE_UPDATE_ALIGNED	= 1<<9,   //�ӿؼ��Ѿ��������
	WGTSTATE_UPDATE_AJUSTED	= 1<<10,  //���ڴ�С���������
	WGTSTATE_LOADED			= 1<<11,  //�������������
	WGTSTATE_ACTIVED		= 1<<12,  //���ڼ�����
	WGTSTATE_UPDATE_DIRTY	= 1<<13,  //��Ҫִ�д��ڵ��Զ��������	
	WGTSTATE_ACCEPT_IDLE	= 1<<14,  //����idle��Ϣ
	WGTSTATE_PAUSE_PATHMOVE	= 1<<15,  //ֹͣ����·������ƶ�
	WGTSTATE_FLIP_PATHMOVE	= 1<<16,  //ֹͣ����·������ƶ�
	WGTSTATE_ALIGN_DIRTY	= 1<<17,  //���ڴ�С��������Ҫ���²��ֺ���
	WGTSTATE_IME_ENABLE		= 1<<18,  //���������뷨�Ƿ��

};

//���涨��(���ڷ��)
enum WIDGET_STYLE
{	
	WGTSTYLE_NONE				= 0,
	WGTSTYLE_FLASH				= 1,//��˸
	WGTSTYLE_VERTICAL			= 1<<1,//��ֱ
	WGTSTYLE_PRIMARY			= 1<<2, //�ɶԴ����е������ڣ����磺һ��spin�����е�up spin��
	WGTSTYLE_CLIPSELF			= 1<<3, 
	WGTSTYLE_CLIPCHILD_WINDOW   = 1<<4,
	WGTSTYLE_CLIPCHILD_CLIENT   = 1<<5,
	WGTSTYLE_SYSTEMDRAW			= 1<<6,
	WGTSTYLE_ADVANCE			= 1<<7,
	WGTSTYLE_POPUP				= 1<<8,
	WGTSTYLE_SELF_XMOVE			= 1<<9,//�ɷ�����϶�����X���ƶ�
	WGTSTYLE_SELF_YMOVE			= 1<<10,//�ɷ�����϶�����Y���ƶ�		
	WGTSTYLE_SELF_XSIZE			= 1<<11,//�ɷ�����X�������С
	WGTSTYLE_SELF_YSIZE			= 1<<12,//�ɷ�����Y�������С
	WGTSTYLE_STATIC				= 1<<13,//�Ƿ��Ǿ�̬����
	WGTSTYLE_TOPMOST			= 1<<14,//�Ƿ��Ƕ��㴰��
	WGTSTYLE_ALPHAWIN			= 1<<15,//�Ƿ���͸����ⴰ��(͸�������㴰����)
	WGTSTYLE_FLIPDRAG			= 1<<16,//�Ƿ��Ƿ�ת�϶�������(�϶���ʵ���ǲ����϶���)
	WGTSTYLE_TEMPLATE			= 1<<17,//�Ƿ���ģ�崰��
	WGTSTYLE_ADJUSTSELF			= 1<<18,//�Ƿ�ü��Լ��Ļ�������
	WGTSTYLE_AJUSTFOLLOW		= 1<<19,//������ajustwidth����ajustheightʱ���Լ�Ҳajust
	WGTSTYLE_CLEANWIN			= 1<<20,//����Ӧ�����������������Ҳ����Ĵ���,���,��Ȼ���ý�ɫ�߶�
	WGTSTYLE_EDITABLE			= 1<<21,//�Ƿ���Ա༭
	WGTSTYLE_COLORTEST			= 1<<22,//�Ƿ�̽�������ɫ����λ��̽��callback������ɫ������λ��̽�ⱳ��ͼ��ɫ��
	WGTSTYLE_CUSTOMCLOSE		= 1<<23,//�Ƿ�ȡ��onok,oncancel,endwidgetʱ����Զ��ر�
	WGTSTYLE_CUSTOMALPHA		= 1<<24,//�Ƿ�ȡ��͸���ȵ��Զ�����
	WGTSTYLE_RENDERBUFFER		= 1<<25,//�Ƿ����CustomBuffer
	WGTSTYLE_RENDERITEM			= 1<<26,//�Ƿ����CustomItem
	WGTSTYLE_AREAWAKE			= 1<<27,//�Ƿ񾯾���������ƶ�	
	WGTSTYLE_FATHERENABLE		= 1<<28,//�Ƿ��ע�����ڵ�enable״̬	
	WGTSTYLE_ALIGNFOLLOW		= 1<<29,//�Ƿ���ajustdirtyʱ������align����
	WGTSTYLE_AUTOACTIVE			= 1<<30,//�Ƿ���mouseoverʱ��activeself	

};


//���涨��(������չ���)
enum WIDGET_PATTERN
{	
	
	WGTPATTERN_NONE				= 0,
	WGTPATTERN_RAWVISIBLE		= 1,//�������Ӿ����ɼ���ʱ��,������
	WGTPATTERN_ALPHAVISIBLE		= 2,//�������Ӿ��ɼ���ʱ��,�Ƿ������renderfactor����	

};


//���涨��(������)
enum ALIGN_STYLE
{

	ALIGNSTYLE_NONE   	   = 0,
	ALIGNSTYLE_TOP_IN	   = 1,
	ALIGNSTYLE_BOTTOM_IN   = 1<<1,
	ALIGNSTYLE_LEFT_IN     = 1<<2,	
	ALIGNSTYLE_RIGHT_IN    = 1<<3,

	ALIGNSTYLE_TOP_OUT	   = 1<<4,
	ALIGNSTYLE_BOTTOM_OUT  = 1<<5,
	ALIGNSTYLE_LEFT_OUT    = 1<<6,	
	ALIGNSTYLE_RIGHT_OUT   = 1<<7,

	ALIGNSTYLE_HCENTER     = 1<<8,
	ALIGNSTYLE_VCENTER     = 1<<9,
	
	ALIGNSTYLE_HMID_LEFT   = 1<<10,
	ALIGNSTYLE_HMID_RIGHT  = 1<<11,
	
	ALIGNSTYLE_VMID_TOP	   = 1<<12,
	ALIGNSTYLE_VMID_BOTTOM = 1<<13,
	
	
};

//���涨��(��������)
enum ALIGN_TYPE
{
	
	ALIGNTYPE_NONE   		= 0,
	ALIGNTYPE_BROTHER		= 1,
	ALIGNTYPE_FATHER		= 1<<1,
	ALIGNTYPE_UNCLE			= 1<<2,
	ALIGNTYPE_GRANDPA		= 1<<3,		
		
	ALIGNTYPE_WINRECT		= 1<<4,
	ALIGNTYPE_CLIENTRECT	= 1<<5,
	ALIGNTYPE_CAPTIONRECT	= 1<<6,
	ALIGNTYPE_DRAGRECT		= 1<<7	
		
};

//���涨��(����ͨ��)
enum VALUE_CHANNEL
{

	VALUE_CHANNEL_STATE				=0, 
	VALUE_CHANNEL_STYLE				=1,	
	VALUE_CHANNEL_DRAGTYPE			=2,	
	VALUE_CHANNEL_TABSTOP			=3,
	VALUE_CHANNEL_GROUPSTOP			=4,
	VALUE_CHANNEL_GROUPID			=5,
	VALUE_CHANNEL_TEXTSTYLE			=6,
	VALUE_CHANNEL_TEXTSIZE			=7,
	VALUE_CHANNEL_SNAPH				=8,
	VALUE_CHANNEL_SNAPV				=9,
	
	VALUE_CHANNEL_VALUE				=10,
	VALUE_CHANNEL_MINVALUE			=11,
	VALUE_CHANNEL_MAXVALUE			=12,
	VALUE_CHANNEL_STEP				=13,
	VALUE_CHANNEL_PAGE_H			=14,
	VALUE_CHANNEL_PAGE_V			=15,
	VALUE_CHANNEL_PAGENUM_H			=16,
	VALUE_CHANNEL_PAGENUM_V			=17,
	VALUE_CHANNEL_SEL				=18,
	VALUE_CHANNEL_POS				=19,

	VALUE_CHANNEL_TEMPLATEID		=20,

	VALUE_CHANNEL_HOSTTYPE			=21,
	VALUE_CHANNEL_HOSTINDEX			=22,
	
	VALUE_CHANNEL_VALUE_H			=23,
	VALUE_CHANNEL_VALUE_V			=24,
	VALUE_CHANNEL_LOWER_H			=25,
	VALUE_CHANNEL_UPER_H			=26,		
	VALUE_CHANNEL_LOWER_V			=27,
	VALUE_CHANNEL_UPER_V			=28,
	VALUE_CHANNEL_METHOD			=29,

	VALUE_CHANNEL_EDGE_CHECK		=30,

	VALUE_CHANNEL_FRAME				=31,
	VALUE_CHANNEL_ITEM_NUM			=32,

	VALUE_CHANNEL_WINRECT			=34,
	VALUE_CHANNEL_CLIENTRECT		=35,
	VALUE_CHANNEL_CAPTIONRECT		=36,
	VALUE_CHANNEL_DRAGRECT			=37,
	VALUE_CHANNEL_DRAGINDEX			=38,

	VALUE_CHANNEL_AREAID			=39,

	VALUE_CHANNEL_TEXTFONT			=40,
	VALUE_CHANNEL_ALPHASPEED		=41,
	VALUE_CHANNEL_ALPHAFACTOR		=42,
	
	VALUE_CHANNEL_COLOR_MAGIC		=43,

	VALUE_CHANNEL_SOUNDID			=44,

	
	VALUE_CHANNEL_FRAMESCHEME		=45,
	VALUE_CHANNEL_TIMERSPACE		=46,
	VALUE_CHANNEL_PATHSPEED			=47,
	VALUE_CHANNEL_FRAMESPEED		=48,

	VALUE_CHANNEL_STYLEEX			=49,

			
	VALUE_CHANNEL_RANGE_BEGIN		=50,
	VALUE_CHANNEL_RANGE_END			=51,
	VALUE_CHANNEL_COLOR_BACK		=52,
	VALUE_CHANNEL_COLOR_DRAW		=53,
	VALUE_CHANNEL_COLOR_FOCUS		=54,
	VALUE_CHANNEL_COLOR_EDGE		=55,
	VALUE_CHANNEL_SWITCH			=56,
	VALUE_CHANNEL_UPDATE_DIRTY		=57,		
	VALUE_CHANNEL_SORT_STATE		=58,		
	VALUE_CHANNEL_HOT				=59,

	VALUE_CHANNEL_AJUST_EDGE		=60,		
	VALUE_CHANNEL_EDGE_THICK		=61,
	VALUE_CHANNEL_AJUST_FLAG		=62,
	VALUE_CHANNEL_AJUST_STATE		=63,
	VALUE_CHANNEL_MIN_HEIGHT		=64,
	VALUE_CHANNEL_MAX_HEIGHT		=65,
	VALUE_CHANNEL_MIN_WIDTH			=66,
	VALUE_CHANNEL_MAX_WIDTH			=67,
	
	VALUE_CHANNEL_RECTID			=68,

	VALUE_CHANNEL_MOUSEDELTA		=69,

	VALUE_CHANNEL_CAPTIONSIZE		=70,

	VALUE_CHANNEL_PATHMOVE_PAUSE	=73,
	VALUE_CHANNEL_PATHMOVE_FLIP		=74,

	VALUE_CHANNEL_MSGBOX_ID			=75,
	VALUE_CHANNEL_MSGBOX_INFO		=76,

	VALUE_CHANNEL_ALPHASTATE		=77,
	VALUE_CHANNEL_SCALEFACTOR		=78,
	VALUE_CHANNEL_ALPHAMIN			=79,
	VALUE_CHANNEL_ALPHAMAX			=80,

	VALUE_CHANNEL_ITEMCOLOR			=81,
	VALUE_CHANNEL_DRAWFLAG			=82,
	VALUE_CHANNEL_TREEID			=83,

	VALUE_CHANNEL_ALIGN_DIRTY		=84,

	VALUE_CHANNEL_COLOR_LINK		=85,

	VALUE_CHANNEL_SCROLLSPACE		=86,

	VALUE_CHANNEL_RICHMODE			=87,
	VALUE_CHANNEL_RICHFLAG			=88,
	
	VALUE_CHANNEL_DRAGFLAG			=89,
	VALUE_CHANNEL_DROPFLAG			=90,

	VALUE_CHANNEL_PATTERN			=91,

	VALUE_CHANNEL_QUERYTYPE			=92,
	VALUE_CHANNEL_QUERYINDEX		=93,

	VALUE_CHANNEL_INDEXOFFSET		=94,	

	VALUE_CHANNEL_MAXCHAR			=95,
	
	VALUE_CHANNEL_UPDATEMUTE		=96,

	//���ڼ�ⴰ��״̬�仯
	VALUE_CHANNEL_MOVEABLE			=201,
	VALUE_CHANNEL_MOUSEOVER			=202,
	VALUE_CHANNEL_CLICKDOWN			=203,
	VALUE_CHANNEL_FOCUSED			=204,
	VALUE_CHANNEL_ENABLED			=205,
	VALUE_CHANNEL_CHECKED			=206,
	VALUE_CHANNEL_DISCARDED			=207,
	VALUE_CHANNEL_DRAGGING			=208,
	VALUE_CHANNEL_VISIBLE			=209,
	VALUE_CHANNEL_ACTIVED			=210,
	VALUE_CHANNEL_EDITABLE			=211,

	VALUE_CHANNEL_CAPTURED			=212,
	VALUE_CHANNEL_CHILDFOCUSED		=213,
	VALUE_CHANNEL_DESCENDFOCUSED	=214,
	
	VALUE_CHANNEL_EFFECTFRAME		=215,

	VALUE_CHANNEL_BACKPICPOS		=216,
	VALUE_CHANNEL_COVERPICPOS		=217,
	VALUE_CHANNEL_EFFECTPICPOS		=218,
	VALUE_CHANNEL_CHECKCOVERPOS		=219,
	VALUE_CHANNEL_HOTCOVERPOS		=220,
	VALUE_CHANNEL_ADORNPICPOSX		=221,
	VALUE_CHANNEL_ADORNPICPOSY		=222,

	//�Զ�����ֵ����
	VALUE_CHANNEL_CUSTOMSTYLE		=300,
	VALUE_CHANNEL_CUSTOMSTATE		=301,
	VALUE_CHANNEL_CUSTOMTYPE		=302,
	VALUE_CHANNEL_CUSTOMINDEX		=303,
	VALUE_CHANNEL_CUSTOMID			=304,		

	
//////�Ӵ�֮�����ı�
	VALUE_CHANNEL_CAPTION			=400,
	VALUE_CHANNEL_SELTEXT			=401,
	VALUE_CHANNEL_PATHNAME			=402,
	VALUE_CHANNEL_DIRNAME			=403,
	
	VALUE_CHANNEL_EDITHISTORY		=404,
	VALUE_CHANNEL_ITEMTEXT			=405,
	VALUE_CHANNEL_TIPTEXT			=406,
	VALUE_CHANNEL_TYPENAME			=407,
	VALUE_CHANNEL_ICONLIST			=408,

	VALUE_CHANNEL_BACKPIC			=409,
	VALUE_CHANNEL_COVERPIC			=410,
	VALUE_CHANNEL_ADORNPIC			=411,
	VALUE_CHANNEL_PATCHPIC			=412,
	VALUE_CHANNEL_EFFECTPIC			=413,

	VALUE_CHANNEL_SCRIPTTEXT		=414,
	
	VALUE_CHANNEL_TIPWGT			=415,
	VALUE_CHANNEL_MSGBOX			=416,
	VALUE_CHANNEL_CONSOLE			=417,
	VALUE_CHANNEL_HOST				=418,
	VALUE_CHANNEL_INFOWGT			=419,
	VALUE_CHANNEL_BUDDY				=420,
	VALUE_CHANNEL_PRIMESCROLL		=421,
	VALUE_CHANNEL_SECONDSCROLL		=422,
	VALUE_CHANNEL_BLOCKBUTTON 		=423,
	
	VALUE_CHANNEL_TABCOVER 			=424,


	//�Զ����ı�����
	VALUE_CHANNEL_CUSTOMNAME		=500,
	VALUE_CHANNEL_CUSTOMTEXT		=501,
	VALUE_CHANNEL_CUSTOMSTRING		=502,


////�Ӵ�֮���Ǳ�����
	VALUE_CHANNEL_ITEMDATA			=600,


	//���ͨ����Ŀ
	VALUE_CHANNEL_MAX_NUM,

};


enum WGTERROR
{
	WGTERROR_NONE			=-1,
	WGTERROR_MEDIT_FONT		= 0,//mutliedit����������Ʒ���ʧ��
	WGTERROR_MEDIT_COLOR	= 1,//mutliedit������ɫ���Ʒ���ʧ��
	WGTERROR_MEDIT_INSERT	= 2,//mutliedit�����ַ�ʧ��

};


struct stUIVar
{
	int iChannel;
	int iIndex;
	
	double fValue;
	std::string sValue;

	stUIVar()
	{
		iChannel=VALUE_CHANNEL_VALUE;
		iIndex=0;

		fValue=-1.0;
		sValue="";
	}

	stUIVar(int paraChannel,int paraIndex,double paraFValue,const char* paraSValue)
	{
		iChannel=paraChannel;
		iIndex=paraIndex;

		fValue=paraFValue;
		
		if(!paraSValue) 
			sValue="";
		else 
			sValue=paraSValue;
	}

	bool operator==(const stUIVar& src)
	{
		if(iChannel!=src.iChannel) return false;

		if(iChannel<VALUE_CHANNEL_CAPTION)
			return (fValue==src.fValue);
		else
			return (sValue==src.sValue);	
			
	}



};


//////////////////////////////////////////////////////////////////////////
//�ṩ��gameui��Ļص�����//end
//////////////////////////////////////////////////////////////////////////

class stCustomBuffer 
{

private:

	int bufType;
	int bufSize;
	void * pBuffer;

public:

	stCustomBuffer()
	{
		bufType=-1;
		bufSize=0;
		pBuffer=NULL;
	}

	stCustomBuffer(int type)
	{
		bufType=type;
		bufSize=0;
		pBuffer=NULL;
	}

	~stCustomBuffer()
	{
		FreeBuffer();		
	}


	void* GetBuffer()
	{
		return pBuffer;
	}

	void* AllocBuffer(int size)
	{
		
		if(bufSize<size)
		{
			pBuffer=realloc(pBuffer,size);
			memset(pBuffer,0,size);
		}
		
				
		bufSize=size;

		return pBuffer;

	}

	void FreeBuffer()
	{		
		
		if(pBuffer) free(pBuffer);
		
		pBuffer=NULL;
		bufType=-1;
		bufSize=0;

	}


	int GetType()
	{
		return bufType;
	}

	int GetSize()
	{
		return bufSize;
	}


};

enum
{
	UI_KEY_DOWN		=0,
	UI_KEY_UP		=1,
	UI_KEY_PRESS	=2,
};


struct stAccelItem
{
	
	char accel_key;

	bool modifier_shift;
	bool modifier_ctrl;
	bool modifier_alt;

	char accel_type;

	char accel_script[UI_ACCEL_SCRIPT]; 

	
	stAccelItem()
	{

		accel_key=0;

		modifier_shift=false;
		modifier_ctrl=false;
		modifier_alt=false;

		accel_type=UI_KEY_DOWN;
	
		memset(accel_script,0,UI_ACCEL_SCRIPT);
	}
	

	stAccelItem(char keyPrimary,bool shiftFlag=false,bool ctrlFlag=false,bool altFlag=false,char keyType=UI_KEY_DOWN,const char* pString="")
	{

		accel_key=keyPrimary;
		
		modifier_shift=shiftFlag;
		modifier_ctrl=ctrlFlag;
		modifier_alt=altFlag;
		
		accel_type=keyType;

		memcpy(accel_script,pString,sizeof(pString));
	}
	
	bool operator==(const stAccelItem& src)
	{
		return( 
			  (accel_type==src.accel_type)
			&&(accel_key==src.accel_key)
			&&(modifier_shift==src.modifier_shift)
			&&(modifier_ctrl==src.modifier_ctrl)
			&&(modifier_alt==src.modifier_alt)
			  );
			
	}
	
	
};


//�û���ӿڱ�¶���ڵĻ������ݸ��ⲿ
class IWgtData : public CRefObject
{


public:
	
	virtual bool IsClickDown()=0;
	virtual bool IsMouseOver()=0;
	virtual bool IsEnabled()=0;
	virtual bool IsFocused()=0;
	virtual bool IsChecked()=0;

	//�ͻ���Ҫ��ѯ�����޸ĵ�����
	virtual const char *GetWidgetID()=0;//����id
	virtual const char *GetWidgetPathID()=0;//����������(.children.)id
	virtual const char *GetWidgetPath()=0;//����·��(����.children.)
		
	virtual stCustomBuffer& GetCustomBuffer()=0;

	virtual int  GetQueryType()=0;
	virtual int  GetQueryIndex()=0;
	virtual int  GetDragType()=0;
	virtual int  GetDragIndex()=0;
	virtual int  GetHostType()=0;
	virtual int  GetHostIndex()=0;

	//�ͻ����Զ��������
	//����custom buffer�߱����Զ�����չ����
	//���ǣ����ڷǳ��ٵĴ����Զ������ݣ����������
	virtual int  GetCustomState()=0;
	virtual int  GetCustomStyle()=0;	
	virtual int  GetCustomId()=0;
	virtual int  GetCustomType()=0;
	virtual int  GetCustomIndex()=0;
	virtual const char* GetCustomName()=0;
	virtual const char* GetCustomText()=0;
	virtual const char* GetCustomString()=0;
	

	//�ͻ�����Ҫ�Ļ��Ʋ����ӿ�
	virtual stRect GetWinRect()=0;//���Դ��ھ���
	virtual stRect GetAbsoluteClientRect()=0;//���Կͻ�������	
	virtual stRect GetAbsoluteCaptionRect()=0;//���Ա���������
	virtual stRect GetAbsoluteDragRect()=0;//�����Ϸ�������

	virtual double GetRenderFactor()=0;//���ڵ�ǰ͸������
	virtual double GetScaleFactor()=0;//���ڵ�ǰ��������


	virtual WidgetType GetType()=0;
	virtual int  GetState()=0;
	virtual int  GetStyle()=0;
	virtual int  GetPattern()=0;
	virtual int  GetExtendStyle()=0;

	virtual void SetVar(stUIVar& uiVar)=0;
	virtual void GetVar(stUIVar& uiVar)=0;
	
	virtual int  GetNumChild()=0;
	virtual IWgtData* GetWgtChild( int i )=0;

	virtual IWgtData* GetWgtFather()=0;

	virtual std::vector<stAccelItem>& GetWgtAccels()=0;


};

typedef void *CallbackData;//�ص�����ָ�����Ͷ���  
typedef void (* CallBackFunc)(CallbackData,IWgtData*); //ָ��ص�������ָ��


struct stQueryDirty
{
	bool bDirty;
	int  nChannel;

	int querytype;
	int queryindex;

	stQueryDirty()
	{
		bDirty=false;
		nChannel=-1;

		querytype=UI_QUERY_TYPE_NONE;
		queryindex=0;
	}
};

//���ڵ����ĺ�����Ŀ
#define UI_NODE_CHILD_MAX 1024

//���ڵ�����Ľڵ�·������
#define UI_NODE_PATH_MAX  64  

struct stQueryNode
{

	//һ�����ڵ��Ψһ��ʶ��
	//��������ɿͻ����Լ�����
	int id;
	
	//�����ӽڵ�����ڸ��ڵ����ǰһ���ֵܽڵ�Ļ��Ʋ���ƫ�Ʋ�����margin��
	int offx;
	int offy;

	//�����ӽڵ�̶��ߴ�������۵������Ժ�ĳߴ磩
	int sizex;
	int sizey;

	stRect iconRect;//�ڵ������۵�ͼ������ڽڵ��λ�úͳߴ�

	//�ӽڵ����
	int  numChild;

	//�ӽڵ�id�б�
	int idChild[UI_NODE_CHILD_MAX];

	stQueryNode()
	{
		id=-1;
		
		offx=0;
		offy=0;
		
		sizex=0;
		sizey=0;

		numChild=0;
		memset(idChild,0,sizeof(int)*UI_NODE_CHILD_MAX);
	}

};



//��ѯһ���ڵ���ϼ��ڵ�·��
struct stNodePath
{

	//һ�����ڵ��Ψһ��ʶ��
	//��������ɿͻ����Լ�����
	int id;	

	//·���ڵ����
	int numPath;

	//·���ڵ�id�б�,��ʽ(self,mother,grandma,...)
	int idPath[UI_NODE_PATH_MAX];

	stNodePath()
	{
		id=-1;
		numPath=0;
		memset(idPath,0,sizeof(int)*UI_NODE_PATH_MAX);
	}

	//���ڵ�·������
	void ReversePath()
	{		
		for(int i=0;i<numPath/2;i++)
		{
			int tmp=idPath[i];
			idPath[i]=idPath[numPath-1-i];
			idPath[numPath-1-i]=tmp;			
		}

	}

};

enum UIItemState
{	
	
	UI_ITEM_STATE_DEFAULT		= 0,//ȱʡ,��������״̬����Ŀ
	UI_ITEM_STATE_DRAGGING		= 1,//�����Ϸ�״̬����Ŀ(��͸����Ч��)
	UI_ITEM_STATE_HIDE			= 2,//������Ŀ
	UI_ITEM_STATE_SELECTED		= 4,//ѡ����Ŀ
	UI_ITEM_STATE_FOCUSED		= 8,//������Ŀ
	UI_ITEM_STATE_COLLAPSED		= 16,//���ڵ�	
		
};


//add by lincoln [2007:4:1] [13:35]
//����ؼ��Ľ�
struct  stUIEncrypt
{
	unsigned char *pInBuf;
	unsigned char *pOutBuf;
	int	 nInBufLen;
	int  nOutBufLen;

};
//}}}


//���ڼ��ܣ��������е�index��ʾ���ܵ�id
struct stDrawItem
{
	stRect rect;
	
	//���ڱ�ʾ����(һ�㲻�仯)
	int type;
	int index;


	//���ڱ�ʾ���ڵĸ�������(����仯)
	int hosttype;
	int hostindex;

	int itemnum;//��ʾ�����϶�״̬�ĵ��ߵ���Ŀ
	int state; //���state<0,���ʾ�����Ϸ�,������
	unsigned int color;//������ɫ
};

struct stMoveItem
{
	int srctype;
	int srcindex;
	int srchosttype;
	int srchostindex;
		
	int itemnum;

	int dsttype;
	int dstindex;
	int dsthosttype;
	int dsthostindex;

	stRect rect;
};

struct stValidItem
{
	int type;
	int index;
	
	int hosttype;
	int hostindex;

	int nitem;  //��Ʒ��Ŀ
	int itemtype;

	bool cangetinfo; //���Ҽ��õ���Ϣ
	bool cangettip;  //�ܵõ�tooltip��Ϣ

	bool candrag;    //���϶�
	bool canajust;   //�ܵ�����Ŀ
	bool canattach;  //��ճ���������

	stValidItem()
	{

		type=-1;
		index=-1;
		
		hosttype=-1;
		hostindex=-1;
		
		nitem=0;
		itemtype=0;
		
		cangetinfo=false;
		cangettip=false;

		candrag=false;
		canajust=true; 
		canattach=UI_DEFAULT_DRAG_STYLE;

	}

};

struct stInfoItem
{
	int type;
	int index;

	int hosttype;
	int hostindex;

};

struct stClickItem
{
	int type;
	int index;

	int hosttype;
	int hostindex;

};


struct stItemPixel
{
	int type;
	int index;

	int hosttype;
	int hostindex;

	int x, y;

	stColorQuad color;
};

struct stDrawChar
{
	stRect rect;

	int type;
	int index;

	int state;
	unsigned int color;
};

struct stDrawNumber
{
	stRect rect;
	stColorQuad color;
	char buffer[256];
};

struct stUISound
{
	int id;//UI_SOUND_NONE:������
	int mode;//0:��ͨģʽ,1:ѭ��ģʽ
	int cmd;
};

enum UI_RICHMODE
{
	
	UI_RICHMODE_NONE	  =0,//ͼ��
	UI_RICHMODE_ICON	  =1,//ͼ��
	UI_RICHMODE_COLOR 	  =2,//��ɫ
	UI_RICHMODE_FONT	  =4,//����
	UI_RICHMODE_STYLE	  =8,//���
	UI_RICHMODE_HLINK 	  =16,//������
	UI_RICHMODE_FIELD     = 32, //��

	//ȫ������
	UI_RICHMODE_ALL 	  =63,

};


enum
{
	
	RICH_CHAR_NONE	=-1,//��Ч�ַ�

	RICH_CHAR_CHAR	=0,//Ӣ���ַ�
	RICH_CHAR_WORD	=1,//˫�ֽں���

	RICH_CHAR_HALF	=2,//���˫�ֽں���

	RICH_CHAR_LF	=3,//���п����ַ�

	RICH_CHAR_ICON	=4,//ͼ������ַ�
	RICH_CHAR_COLOR	=5,//��ɫ�����ַ�
	RICH_CHAR_FONT	=6,//��������ַ�
	RICH_CHAR_STYLE	=7,//�������ַ�
	RICH_CHAR_HLINK	=8,//�����ӿ����ַ�
	RICH_CHAR_FIELD = 9,//������ַ�
		
};

enum
{
	RICHCHAR_CMD_NONE	=0,
	RICHCHAR_CMD_CLICK	=1,
	RICHCHAR_CMD_RCLICK	=2,
};

//�����ַ�,��ȫ�ɿͻ��˶���,������޹�
//����: /aiq,����ͼ�� 
//����: /f0,����0
//����: /c0,��ɫ0 
//����: /p0,ͼƬ0 
//����: /<help=����>���ڼ���/<>,��������:/<cmd=cmdparam>disptext/<>
//������ظ���RICH_CHAR_NONE,��ʾ,�ⲻ��һ��ͼ����߿����ַ�
#define UI_KEYCHAR_MAX 128
struct stKeyChar
{
	//�����ַ�������,�ο������enum����
	int  type;

	//�����ַ��Ĳ���(icon��id,color����ɫֵ,���������,����ֵ,�������ӵ�ͷβ��־)
	//ͬʱ,Ҳ��Ϊ�Ƿ�����Ч�����ַ����о�֮һ
	//���磺index<=DEFAULT_HLINK_BASE:�������ӽ�����
	//index>DEFAULT_HLINK_BASE:�������ӿ�ʼ����ȫ��Ψһid
	int  index;

	//iskeychar��validkeychar��ʱ��ָ���Ƿ����������һ���iskeychar��ʱ�򣬿�Ĭ��Ϊ���Ұ�ť���ɵ����
	//hitkeychar��ʱ��,���Ĵ���
	int  cmdid;

	//�����ַ���(ͼ��)���ƿ��
	int  width;
	int  height;
	
	//�������Ŀ����ַ�����
	int  keynum;

	//�Ե��ĳ��� 
	int	 keynumeat;

	//�ͻ������õĿ���չ��־
	int flag;

	//�������Ŀ����ַ�
	char key[UI_KEYCHAR_MAX];


	stKeyChar()
	{

		type=RICH_CHAR_NONE;
		index=-1;
		cmdid=RICHCHAR_CMD_NONE;

		flag=0;

		width=0;
		height=0;

		keynum=0;
		memset(key,0,UI_KEYCHAR_MAX);

	}
	

};

//����Ĺؼ����(�����˽������Ϊ)
//bDuplicate:�Ƿ���������ת���ַ���Ĭ�ϲ�����
//bOriginMove:�Ƿ��������ԭ���ƶ�
//escapeChar:ת���ַ���Ĭ��Ϊ'/'
//bufTalkCmd:����̨�л���talkģʽ�������ַ���
//bufScriptCmd:����̨�л���scriptģʽ�������ַ���
struct stUICoreStyle
{
	bool bDuplicate;
	bool bOriginMove;
	char escapeChar;
	char bufTalkCmd[16];
	char bufScriptCmd[16];
	
	std::map<int,std::string>mapVirtualKey;

	stUICoreStyle()
	{
		bDuplicate=FALSE;
		bOriginMove=TRUE;

		escapeChar='/';

		memcpy(bufTalkCmd,"/Say",5);
		memcpy(bufScriptCmd,"/Script",8);

		mapVirtualKey.clear();
		
		mapVirtualKey[VK_LBUTTON]="Mouse L";  
		mapVirtualKey[VK_RBUTTON]="Mouse R";  

		mapVirtualKey[VK_BACK]="Back";  
		mapVirtualKey[VK_TAB]="Tab"; 

		mapVirtualKey[VK_CAPITAL]="Caps Lock";  
		
			
		mapVirtualKey[VK_RETURN]="Enter";
			
		mapVirtualKey[VK_SHIFT]="Shift";
		mapVirtualKey[VK_CONTROL]="Ctrl";
		mapVirtualKey[VK_MENU]="Alt";
		mapVirtualKey[VK_PAUSE]="Pause";
			
		mapVirtualKey[VK_SPACE]="Space";
		mapVirtualKey[VK_PRIOR]="PageUp";
		mapVirtualKey[VK_NEXT]="PageDn";
		mapVirtualKey[VK_END]="End";
		mapVirtualKey[VK_HOME]="Home";
		mapVirtualKey[VK_LEFT]="Left";
		mapVirtualKey[VK_UP]="Up";
		mapVirtualKey[VK_RIGHT]="Right";
		mapVirtualKey[VK_DOWN]="Down";
		mapVirtualKey[VK_INSERT]="Insert";
		mapVirtualKey[VK_DELETE]="Delete";

		mapVirtualKey[VK_NUMPAD0]="Numpad0";
		mapVirtualKey[VK_NUMPAD1]="Numpad1";
		mapVirtualKey[VK_NUMPAD2]="Numpad2";
		mapVirtualKey[VK_NUMPAD3]="Numpad3";
		mapVirtualKey[VK_NUMPAD4]="Numpad4";
		mapVirtualKey[VK_NUMPAD5]="Numpad5";
		mapVirtualKey[VK_NUMPAD6]="Numpad6";
		mapVirtualKey[VK_NUMPAD7]="Numpad7";
		mapVirtualKey[VK_NUMPAD8]="Numpad8";
		mapVirtualKey[VK_NUMPAD9]="Numpad9";

		mapVirtualKey[VK_MULTIPLY]="Numpad*";
		mapVirtualKey[VK_ADD]="Numpad+";
		mapVirtualKey[VK_SUBTRACT]="Numpad-";
		mapVirtualKey[VK_DECIMAL]="Numpad.";
		mapVirtualKey[VK_DIVIDE]="Numpad/";

		mapVirtualKey[VK_F1]="F1";
		mapVirtualKey[VK_F2]="F2";
		mapVirtualKey[VK_F3]="F3";
		mapVirtualKey[VK_F4]="F4";
		mapVirtualKey[VK_F5]="F5";
		mapVirtualKey[VK_F6]="F6";
		mapVirtualKey[VK_F7]="F7";
		mapVirtualKey[VK_F8]="F8";
		mapVirtualKey[VK_F9]="F9";
		mapVirtualKey[VK_F10]="F10";
		mapVirtualKey[VK_F11]="F11";
		mapVirtualKey[VK_F12]="F12";

	}

};

struct stGameCursor
{
	char name[16];

	stGameCursor()
	{
		name[0]=0;
	}
	
};

struct stDrawCursor
{
	//��ǰ�Ƿ�ֻ�ɿͻ��˻������
	//���ͻ��˳���ĳ������״̬��������UI�����ϻ���ĳ���������
	bool bClientCursor;

	stDrawCursor()
	{
		bClientCursor=false;
	}
	
};


enum
{
	UI_DRAG_FLAG_NONE	=0,
	UI_DRAG_FLAG_FREE	=1,
	UI_DRAG_FLAG_ATTCH	=2,
	UI_DRAG_FLAG_ALL	=3,
};

enum
{
	UI_DRAG_STYLE_SINGLE	=0,
	UI_DRAG_STYLE_MULTI	=1,
	UI_DRAG_STYLE_CUSTOM	=2,
};


enum
{
	
	UI_MODE_DEFAULT	=0,
	UI_MODE_DEBUG   =1,

};


enum Msgbox_Cmd
{
	
	UI_MSGBOX_CMDHIDE	 =0,
	UI_MSGBOX_CMDSHOW	 =1,
	UI_MSGBOX_CMDUPDATE  =2,

};



//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

class CScriptState;
extern bool CreateGameUI( const char *uires, int w, int h, CScriptState *pscript);
extern void DestroyGameUI();
extern void UpdateGameUI( float deltatime );
extern void RenderGameUI();

extern int  GameUIMessage( UINT msg, WPARAM wparam, LPARAM lparam );
extern int  GameUICallSafeScriptEx(short funcid,const char *fmt="", ...);
extern void RegisterUICallBack(UICallbackEvent eventName,CallBackFunc pFunc);//�ص�ע�ắ��

extern void GameUIConsoleInfo( const char *fmt, ... );
extern void GameUIShowMsgBox( Msgbox_Cmd nCmdShow, const char* pText="",const char* pCaption="Message",int nMsgId=0);

//ת��ĳ��uiscreen
//param idpath ĳ��uiscreen��id·��
extern void GameUIGotoScreen( const char *idpath );
extern const char *GameUIGetCurScreen();

extern int GameUIGetNumScreen();
extern IWgtData* GameUIGetScreen(int i);

extern IWgtData* GameUIGetWidget(const char* idpath);
extern BOOL GameUIShowWidget(const char* idpath, BOOL isshow);
extern IWgtData* GameUIGetRoot();
extern IWgtData* GameUIGetFocus();

extern bool GameUIWidgetExist(IWgtData* pWgt);

extern void GameUISetCursor( const char *pname );
extern void GameUIShowCursor( bool b );

extern void GameUISetImeState( bool b );

//���ý���Ĺؼ����(�����������Ϊ)
//��Ҫ��CreateGameUI֮ǰ����
extern stUICoreStyle& GameUIGetCoreStyle();

//ȡ����ǰ�϶��ĵ���
extern void GameUICancelDrag();

#endif //__GAMEUI_H__
