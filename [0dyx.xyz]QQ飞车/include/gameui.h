
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

//每个屏幕可执行脚本数目
#define UI_SCRN_FILE_MAX 512

//界面定义(脚本函数类型)
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


	SYSTEM_SCIRPT_NUM				//最大系统脚本数目

};


//界面定义(窗口名称)
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

//界面定义(脚本函数名称)
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


//UI模块脚本函数中允许出现的最大字符串
#define UI_SCRIPT_BUFSIZE 4096  

//UI模块脚本函数中出现的字符串默认长度(例如textlist中文本最大长度等)
#define UI_DEFAULT_BUFSIZE 1024

//UI模块临时字符串长度
#define UI_TEMPORARY_BUFSIZE 128  

//TEXTLIST每行附带信息中字符串最大长度
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

//默认道具拖动方式
//0:自由拖动
//1:附着拖动
#define  UI_DEFAULT_DRAG_STYLE 1



#define DEFAULT_CURSOR_NAME		"Normal"
#define DEFAULT_HORIZON_SIZER	"Horizon"
#define DEFAULT_VERTICAL_SIZER	"Vertical"
#define DEFAULT_DRAG_CURSOR		"DragCursor"
#define DEFAULT_EDIT_CURSOR		"EditCursor"
#define DEFAULT_LINK_CURSOR		"LinkCursor"
#define BUTTON_CURSOR_NAME		"Click"


#define DEFAULT_TEMPLATE_KEY	"_UIID_"

//内置编辑器默认的中间文件存放目录
#define DEFAULT_EDITOR_FOLDER	"UIEditor"

//默认的颜色,字体,风格,链接控制符号的起始索引
//当控制字符索引值为起始索引时,表示恢复窗口默认的颜色,字体,风格
#define DEFAULT_ICON_BASE	0
#define DEFAULT_COLOR_BASE	0
#define DEFAULT_FONT_BASE	0
#define DEFAULT_STYLE_BASE	0
#define DEFAULT_HLINK_BASE	0
#define DEFAULT_FIELD_BASE	0

//////////////////////////////////////////////////////////////////////////
//提供给gameui库的回调函数//begin
//////////////////////////////////////////////////////////////////////////
//界面定义(回调函数类型)
enum UICallbackEvent
{
	UI_Callback_GetGameCursor	  =0,	
	UI_Callback_DrawCursor			,	
	UI_Callback_EnableIME			,		

	UI_Callback_Encrypt				,//密码控件改进
	UI_Callback_Decrypt				,//密码控件改进

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
	
	//事件最大数目
	UI_Callback_MAXNUM,

};

/*
//edit methods
enum
{
	EDIT_NORMAL=0,//英文，数字，符号,中文
	EDIT_NUMBER,
	EDIT_CODER,
};
*/

//edit methods
enum
{

	UI_EDITMETHOD_ENGLISH	=1,//英文
	UI_EDITMETHOD_NUMBER	=2,//数字
	UI_EDITMETHOD_SYMBOL	=4,//符号
	UI_EDITMETHOD_CHINESE	=8,//中文

	UI_EDITMETHOD_VALUE		=16,//数值
	UI_EDITMETHOD_CODER		=32 //密码

};


//当为数值输入框,并且值在某个范围时候隐藏
enum
{
	EDIT_STYLE_NONE		=0,
	EDIT_STYLE_KEEPSEL		=1,//即使失去焦点也保持选择
	EDIT_STYLE_KEEPCURSOR	=2,//即使失去焦点也绘制光标
	EDIT_STYLE_FLASHCURSOR	=4,//绘制光标的时候,总是闪烁光标
	EDIT_STYLE_SMARTCURSOR	=8,//失去焦点的状态,如果没有其他窗口绘制光标,则绘制光标
};

/*
enum
{
	REDIT_STYLE_NONE		=0,
	REDIT_STYLE_KEEPSEL		=1,//即使失去焦点也保持选择
	REDIT_STYLE_KEEPCURSOR	=2,//即使失去焦点也绘制光标
	REDIT_STYLE_FLASHCURSOR	=4,//绘制光标的时候,总是闪烁光标
	REDIT_STYLE_SMARTCURSOR	=8,//失去焦点的状态,如果没有其他窗口绘制光标,则绘制光标
};


enum
{
	MEDIT_STYLE_NONE		=0,
	MEDIT_STYLE_KEEPSEL		=1,//即使失去焦点也保持选择
	MEDIT_STYLE_KEEPCURSOR	=2,//即使失去焦点也绘制光标
	MEDIT_STYLE_FLASHCURSOR	=4,//绘制光标的时候,总是闪烁光标
	MEDIT_STYLE_SMARTCURSOR	=8,//失去焦点的状态,如果没有其他窗口绘制光标,则绘制光标
};
*/

enum
{
	MEDIT_ENTER_STYLE_NONE=0,//屏蔽enter键
	MEDIT_ENTER_STYLE_QQ=1,//enter编辑，ctrl+enter调用OnEnter
	MEDIT_ENTER_STYLE_MSN=2,//enter调用OnEnter，ctrl+enter编辑
	MEDIT_ENTER_STYLE_EDIT=3,//enter编辑,ctrl+enter编辑
	MEDIT_ENTER_STYLE_RETURN=4,//enter调用OnEnter，ctrl+enter调用OnEnter	
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
	UI_BTN_STYLE_AUTO_TOGGLE  =4,//状态(value)自动切换	
	UI_BTN_STYLE_AUTO_ROUND   =8,//状态(value)自动回转	
	UI_BTN_STYLE_SPIN		  =16,//按下后，每个threshhold时间，算click一次

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

//宽度总是不能调整的
enum LISTBOX_STYLE
{
	
	LISTBOX_STYLE_MULTI_SEL=			1<<0,//多选
	LISTBOX_STYLE_ITEMHEIGHT_FIX=		1<<1,//固定高度
	LISTBOX_STYLE_ENUM_ITEM=			1<<3,//按项目滚动,默认按象素滚动
	LISTBOX_STYLE_WHEEL=				1<<4,//支持mouse wheel滚动
	LISTBOX_STYLE_DIRECTORY=			1<<5,//返回目录名,只显示目录
	LISTBOX_STYLE_FILE=					1<<6,//返回文件名,显示目录和文件
	LISTBOX_STYLE_CUSTOM_ARRANGE=		1<<7,//脚本指定列表项目的位置
	LISTBOX_STYLE_DBCLICK_SEL=			1<<8,//双击选择,否则单击选择
	LISTBOX_STYLE_AUTO_COMFORT=			1<<9,//自动comfort
	LISTBOX_STYLE_RICHTEXT=				1<<10,//绘制带控制字符的文本
	LISTBOX_STYLE_COLORLIST=			1<<11,//是否颜色列表
};


enum LISTBOX_ITEM_FLAG
{
	LISTBOX_ITEM_FLAG_NONE			=0,
};	

enum LISTBOX_ITEM_TYPE
{
	LISTBOX_ITEM_TYPE_NONE			=-1,	//无效
	LISTBOX_ITEM_TYPE_DIRECTORY		=0,		//目录
	LISTBOX_ITEM_TYPE_FILE			=1,		//文件		
		
};	

//宽度总是不能调整的
enum WGTLIST_STYLE
{
	
	WGTLIST_STYLE_NONE=			        0,
	WGTLIST_STYLE_HOVER_FOCUS=			1<<0,//鼠标停留
	WGTLIST_STYLE_ENUM_ITEM=			1<<1,//按项目滚动,默认按象素滚动
	WGTLIST_STYLE_DELETE_ITEM=			1<<2,//删除多余孩子（默认为隐藏孩子）
	WGTLIST_STYLE_WHEEL=				1<<3,//支持mouse wheel滚动
};

//宽度总是不能调整的
enum TEXTLIST_STYLE
{
	TEXTLIST_STYLE_NONE			=0,
	TEXTLIST_STYLE_ALPHAFONT	=1,//绘制半透明字体
	TEXTLIST_STYLE_REVERSE		=2,//从上到下绘制文本
	TEXTLIST_STYLE_AUTOSCROLL	=4,//自动滚动到最新添加文本
	TEXTLIST_STYLE_SMARTSCROLL	=8,//对齐文本首部
	TEXTLIST_STYLE_DELAYSCROLL	=16,//自动缓慢的滚向最新添加文本
	TEXTLIST_STYLE_BLOCKINMIDDLE	= 32,//滑块不在底部时不滚动

};

enum TreeCtrl_STYLE
{
	TREECTRL_STYLE_NONE=			0,
	TREECTRL_STYLE_AUTO_COMFORT=	1,//自动comfort
	TREECTRL_STYLE_DRAWGRID=		1<<1,//是否绘制线条
};

enum TabWin_STYLE
{
	TABWIN_STYLE_NONE			=   0,
	TABWIN_STYLE_MANUAL_CHECK	=	1,//手动check
};


enum
{

	TABWIN_TYPE_DEFAULT=0,//HotCover的位置直接指定,不通过计算得到

	TABWIN_TYPE_DOWN,//HotCover在窗口的下方
	TABWIN_TYPE_UP,//HotCover在窗口的上方
	TABWIN_TYPE_LEFT,//HotCover在窗口的左边
	TABWIN_TYPE_RIGHT,//HotCover在窗口的右边

};

enum
{

	UI_TIPFLAG_NONE			=0,
	UI_TIPFLAG_INREFWIN		=1,//tooltip在参考矩形（例如窗口中的item）中

};

enum
{
	//无效声音
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

	//最大声音索引数目
	UI_SOUND_MAXNUM,

};


//界面定义(窗口类型)
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

//界面定义(伙伴窗口类型)
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

//窗口的伙伴搜索方式
enum UIBuddyStyle
{
	
	BUDDY_STYLE_SELF	=0,//只在自己里面找伙伴窗口
	BUDDY_STYLE_FATHER	=1,//先在自己里面找,找不到就到屏幕中找
	BUDDY_STYLE_SCREEN	=2,//先在自己里面找,找不到就到屏幕中找
	BUDDY_STYLE_ROOT	=4,//先在自己里面找,找不到就到根窗口中找

		
};


//绘制标志
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
	DRAWFLAG_ENABLE_EFFECT		=1<<8,//只是在enable的时候绘制effect图片
	DRAWFLAG_FOCUS_EFFECT		=1<<9,//只是在focus的时候绘制effect图片
	DRAWFLAG_CHECK_EFFECT		=1<<10,//只是在check的时候绘制effect图片
	DRAWFLAG_DRAW_EFFECT		=1<<11,//总是绘制effect图片
	DRAWFLAG_CLIENTPRIOR		=1<<12,//客户端绘制(custombuffer)优先于UI绘制
	DRAWFLAG_FOCUSCAPTION		=1<<13,//窗口Focus的时候，使用m_FocusColor代替m_DrawColor绘制窗口caption
	DRAWFLAG_HOTCAPTION			=1<<14,//窗口mouseover的时候，使用m_FocusColor代替m_DrawColor绘制窗口caption
	DRAWFLAG_BACKPRIOR			=1<<15,//客户端绘制(custombuffer)优于背景绘制
};

//界面定义(窗口状态)
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
	WGTSTATE_VISIBLE		= 1<<7,   //自身是否能见
	WGTSTATE_UPDATE_SORTED	= 1<<8,   //已经排序完毕	
	WGTSTATE_UPDATE_ALIGNED	= 1<<9,   //子控件已经布局完毕
	WGTSTATE_UPDATE_AJUSTED	= 1<<10,  //窗口大小调整完毕了
	WGTSTATE_LOADED			= 1<<11,  //窗口载入完毕了
	WGTSTATE_ACTIVED		= 1<<12,  //窗口激活了
	WGTSTATE_UPDATE_DIRTY	= 1<<13,  //需要执行窗口的自定义更新了	
	WGTSTATE_ACCEPT_IDLE	= 1<<14,  //接受idle消息
	WGTSTATE_PAUSE_PATHMOVE	= 1<<15,  //停止沿着路径点的移动
	WGTSTATE_FLIP_PATHMOVE	= 1<<16,  //停止沿着路径点的移动
	WGTSTATE_ALIGN_DIRTY	= 1<<17,  //窗口大小调整后，需要重新布局孩子
	WGTSTATE_IME_ENABLE		= 1<<18,  //窗口中输入法是否打开

};

//界面定义(窗口风格)
enum WIDGET_STYLE
{	
	WGTSTYLE_NONE				= 0,
	WGTSTYLE_FLASH				= 1,//闪烁
	WGTSTYLE_VERTICAL			= 1<<1,//竖直
	WGTSTYLE_PRIMARY			= 1<<2, //成对窗口中的主窗口（例如：一对spin窗口中的up spin）
	WGTSTYLE_CLIPSELF			= 1<<3, 
	WGTSTYLE_CLIPCHILD_WINDOW   = 1<<4,
	WGTSTYLE_CLIPCHILD_CLIENT   = 1<<5,
	WGTSTYLE_SYSTEMDRAW			= 1<<6,
	WGTSTYLE_ADVANCE			= 1<<7,
	WGTSTYLE_POPUP				= 1<<8,
	WGTSTYLE_SELF_XMOVE			= 1<<9,//可否被鼠标拖动沿着X轴移动
	WGTSTYLE_SELF_YMOVE			= 1<<10,//可否被鼠标拖动沿着Y轴移动		
	WGTSTYLE_SELF_XSIZE			= 1<<11,//可否沿着X轴调整大小
	WGTSTYLE_SELF_YSIZE			= 1<<12,//可否沿着Y轴调整大小
	WGTSTYLE_STATIC				= 1<<13,//是否是静态窗口
	WGTSTYLE_TOPMOST			= 1<<14,//是否是顶层窗口
	WGTSTYLE_ALPHAWIN			= 1<<15,//是否是透明检测窗口(透明区域不算窗口区)
	WGTSTYLE_FLIPDRAG			= 1<<16,//是否是翻转拖动区窗口(拖动区实际是不可拖动区)
	WGTSTYLE_TEMPLATE			= 1<<17,//是否是模板窗口
	WGTSTYLE_ADJUSTSELF			= 1<<18,//是否裁减自己的绘制内容
	WGTSTYLE_AJUSTFOLLOW		= 1<<19,//当父亲ajustwidth或者ajustheight时，自己也ajust
	WGTSTYLE_CLEANWIN			= 1<<20,//不响应左键点击，避免干扰玩家操作的窗口,点击,仍然会让角色走动
	WGTSTYLE_EDITABLE			= 1<<21,//是否可以编辑
	WGTSTYLE_COLORTEST			= 1<<22,//是否探测绘制颜色（置位：探测callback函数颜色，不置位：探测背景图颜色）
	WGTSTYLE_CUSTOMCLOSE		= 1<<23,//是否取消onok,oncancel,endwidget时候的自动关闭
	WGTSTYLE_CUSTOMALPHA		= 1<<24,//是否取消透明度的自动传递
	WGTSTYLE_RENDERBUFFER		= 1<<25,//是否绘制CustomBuffer
	WGTSTYLE_RENDERITEM			= 1<<26,//是否绘制CustomItem
	WGTSTYLE_AREAWAKE			= 1<<27,//是否警觉鼠标区域移动	
	WGTSTYLE_FATHERENABLE		= 1<<28,//是否关注父窗口的enable状态	
	WGTSTYLE_ALIGNFOLLOW		= 1<<29,//是否在ajustdirty时，重新align孩子
	WGTSTYLE_AUTOACTIVE			= 1<<30,//是否在mouseover时，activeself	

};


//界面定义(窗口扩展风格)
enum WIDGET_PATTERN
{	
	
	WGTPATTERN_NONE				= 0,
	WGTPATTERN_RAWVISIBLE		= 1,//当窗口视觉不可见的时候,不交互
	WGTPATTERN_ALPHAVISIBLE		= 2,//当窗口视觉可见的时候,是否绘制由renderfactor决定	

};


//界面定义(对齐风格)
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

//界面定义(对齐类型)
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

//界面定义(参数通道)
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

	//用于监测窗口状态变化
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

	//自定义数值数据
	VALUE_CHANNEL_CUSTOMSTYLE		=300,
	VALUE_CHANNEL_CUSTOMSTATE		=301,
	VALUE_CHANNEL_CUSTOMTYPE		=302,
	VALUE_CHANNEL_CUSTOMINDEX		=303,
	VALUE_CHANNEL_CUSTOMID			=304,		

	
//////从此之后是文本
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


	//自定义文本数据
	VALUE_CHANNEL_CUSTOMNAME		=500,
	VALUE_CHANNEL_CUSTOMTEXT		=501,
	VALUE_CHANNEL_CUSTOMSTRING		=502,


////从此之后是表数据
	VALUE_CHANNEL_ITEMDATA			=600,


	//最大通道数目
	VALUE_CHANNEL_MAX_NUM,

};


enum WGTERROR
{
	WGTERROR_NONE			=-1,
	WGTERROR_MEDIT_FONT		= 0,//mutliedit插入字体控制符号失败
	WGTERROR_MEDIT_COLOR	= 1,//mutliedit插入颜色控制符号失败
	WGTERROR_MEDIT_INSERT	= 2,//mutliedit插入字符失败

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
//提供给gameui库的回调函数//end
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


//该基类接口暴露窗口的绘制数据给外部
class IWgtData : public CRefObject
{


public:
	
	virtual bool IsClickDown()=0;
	virtual bool IsMouseOver()=0;
	virtual bool IsEnabled()=0;
	virtual bool IsFocused()=0;
	virtual bool IsChecked()=0;

	//客户端要查询或者修改的内容
	virtual const char *GetWidgetID()=0;//窗口id
	virtual const char *GetWidgetPathID()=0;//窗口完整的(.children.)id
	virtual const char *GetWidgetPath()=0;//窗口路径(不带.children.)
		
	virtual stCustomBuffer& GetCustomBuffer()=0;

	virtual int  GetQueryType()=0;
	virtual int  GetQueryIndex()=0;
	virtual int  GetDragType()=0;
	virtual int  GetDragIndex()=0;
	virtual int  GetHostType()=0;
	virtual int  GetHostIndex()=0;

	//客户端自定义的数据
	//本来custom buffer具备了自定义扩展能力
	//但是，对于非常少的窗口自定义数据，可以用这个
	virtual int  GetCustomState()=0;
	virtual int  GetCustomStyle()=0;	
	virtual int  GetCustomId()=0;
	virtual int  GetCustomType()=0;
	virtual int  GetCustomIndex()=0;
	virtual const char* GetCustomName()=0;
	virtual const char* GetCustomText()=0;
	virtual const char* GetCustomString()=0;
	

	//客户端需要的绘制参数接口
	virtual stRect GetWinRect()=0;//绝对窗口矩形
	virtual stRect GetAbsoluteClientRect()=0;//绝对客户区矩形	
	virtual stRect GetAbsoluteCaptionRect()=0;//绝对标题区矩形
	virtual stRect GetAbsoluteDragRect()=0;//绝对拖放区矩形

	virtual double GetRenderFactor()=0;//窗口当前透明因子
	virtual double GetScaleFactor()=0;//窗口当前缩放因子


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

typedef void *CallbackData;//回调数据指针类型定义  
typedef void (* CallBackFunc)(CallbackData,IWgtData*); //指向回调函数的指针


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

//树节点最大的孩子数目
#define UI_NODE_CHILD_MAX 1024

//树节点最深的节点路径层数
#define UI_NODE_PATH_MAX  64  

struct stQueryNode
{

	//一个树节点的唯一标识符
	//具体编码由客户端自己定义
	int id;
	
	//所有子节点相对于父节点或者前一个兄弟节点的绘制布局偏移参数（margin）
	int offx;
	int offy;

	//所有子节点固定尺寸参数（折叠起来以后的尺寸）
	int sizex;
	int sizey;

	stRect iconRect;//节点左侧的折叠图标相对于节点的位置和尺寸

	//子节点个数
	int  numChild;

	//子节点id列表
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



//查询一个节点的上级节点路径
struct stNodePath
{

	//一个树节点的唯一标识符
	//具体编码由客户端自己定义
	int id;	

	//路径节点个数
	int numPath;

	//路径节点id列表,格式(self,mother,grandma,...)
	int idPath[UI_NODE_PATH_MAX];

	stNodePath()
	{
		id=-1;
		numPath=0;
		memset(idPath,0,sizeof(int)*UI_NODE_PATH_MAX);
	}

	//将节点路径反序
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
	
	UI_ITEM_STATE_DEFAULT		= 0,//缺省,绘制正常状态的项目
	UI_ITEM_STATE_DRAGGING		= 1,//绘制拖放状态的项目(半透明等效果)
	UI_ITEM_STATE_HIDE			= 2,//隐藏项目
	UI_ITEM_STATE_SELECTED		= 4,//选择项目
	UI_ITEM_STATE_FOCUSED		= 8,//高亮项目
	UI_ITEM_STATE_COLLAPSED		= 16,//树节点	
		
};


//add by lincoln [2007:4:1] [13:35]
//密码控件改进
struct  stUIEncrypt
{
	unsigned char *pInBuf;
	unsigned char *pOutBuf;
	int	 nInBufLen;
	int  nOutBufLen;

};
//}}}


//对于技能，以下所有的index表示技能的id
struct stDrawItem
{
	stRect rect;
	
	//用于标示窗口(一般不变化)
	int type;
	int index;


	//用于标示窗口的附加数据(随机变化)
	int hosttype;
	int hostindex;

	int itemnum;//表示绘制拖动状态的道具的数目
	int state; //如果state<0,则表示正在拖放,不绘制
	unsigned int color;//绘制颜色
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

	int nitem;  //物品数目
	int itemtype;

	bool cangetinfo; //能右键得到信息
	bool cangettip;  //能得到tooltip信息

	bool candrag;    //能拖动
	bool canajust;   //能调整数目
	bool canattach;  //能粘连在鼠标上

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
	int id;//UI_SOUND_NONE:无声音
	int mode;//0:普通模式,1:循环模式
	int cmd;
};

enum UI_RICHMODE
{
	
	UI_RICHMODE_NONE	  =0,//图标
	UI_RICHMODE_ICON	  =1,//图标
	UI_RICHMODE_COLOR 	  =2,//颜色
	UI_RICHMODE_FONT	  =4,//字体
	UI_RICHMODE_STYLE	  =8,//风格
	UI_RICHMODE_HLINK 	  =16,//超链接
	UI_RICHMODE_FIELD     = 32, //域

	//全部特性
	UI_RICHMODE_ALL 	  =63,

};


enum
{
	
	RICH_CHAR_NONE	=-1,//无效字符

	RICH_CHAR_CHAR	=0,//英文字符
	RICH_CHAR_WORD	=1,//双字节汉字

	RICH_CHAR_HALF	=2,//半个双字节汉字

	RICH_CHAR_LF	=3,//换行控制字符

	RICH_CHAR_ICON	=4,//图标控制字符
	RICH_CHAR_COLOR	=5,//颜色控制字符
	RICH_CHAR_FONT	=6,//字体控制字符
	RICH_CHAR_STYLE	=7,//风格控制字符
	RICH_CHAR_HLINK	=8,//超链接控制字符
	RICH_CHAR_FIELD = 9,//域控制字符
		
};

enum
{
	RICHCHAR_CMD_NONE	=0,
	RICHCHAR_CMD_CLICK	=1,
	RICHCHAR_CMD_RCLICK	=2,
};

//控制字符,完全由客户端定制,与界面无关
//例如: /aiq,爱情图标 
//例如: /f0,字体0
//例如: /c0,颜色0 
//例如: /p0,图片0 
//例如: /<help=寄售>关于寄售/<>,超级链接:/<cmd=cmdparam>disptext/<>
//如果返回负数RICH_CHAR_NONE,表示,这不是一个图标或者控制字符
#define UI_KEYCHAR_MAX 128
struct stKeyChar
{
	//控制字符的类型,参考上面的enum定义
	int  type;

	//控制字符的参数(icon的id,color的颜色值,字体的索引,风格的值,超级链接的头尾标志)
	//同时,也作为是否是有效控制字符的判据之一
	//例如：index<=DEFAULT_HLINK_BASE:超级链接结束符
	//index>DEFAULT_HLINK_BASE:超级链接开始符的全局唯一id
	int  index;

	//iskeychar，validkeychar的时候，指定是否可以鼠标左右击（iskeychar的时候，可默认为左右按钮均可点击）
	//hitkeychar的时候,鼠标的代码
	int  cmdid;

	//控制字符的(图标)绘制宽度
	int  width;
	int  height;
	
	//解析出的控制字符长度
	int  keynum;

	//吃掉的长度 
	int	 keynumeat;

	//客户端自用的可扩展标志
	int flag;

	//解析出的控制字符
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

//界面的关键风格(决定了界面的行为)
//bDuplicate:是否允许两个转义字符，默认不允许
//bOriginMove:是否允许道具原地移动
//escapeChar:转义字符，默认为'/'
//bufTalkCmd:控制台切换到talk模式的命令字符串
//bufScriptCmd:控制台切换到script模式的命令字符串
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
	//当前是否只由客户端绘制鼠标
	//当客户端出于某种特殊状态，可以在UI窗口上绘制某种特殊鼠标
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
extern void RegisterUICallBack(UICallbackEvent eventName,CallBackFunc pFunc);//回调注册函数

extern void GameUIConsoleInfo( const char *fmt, ... );
extern void GameUIShowMsgBox( Msgbox_Cmd nCmdShow, const char* pText="",const char* pCaption="Message",int nMsgId=0);

//转到某个uiscreen
//param idpath 某个uiscreen的id路径
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

//设置界面的关键风格(决定界面的行为)
//需要在CreateGameUI之前调用
extern stUICoreStyle& GameUIGetCoreStyle();

//取消当前拖动的道具
extern void GameUICancelDrag();

#endif //__GAMEUI_H__
