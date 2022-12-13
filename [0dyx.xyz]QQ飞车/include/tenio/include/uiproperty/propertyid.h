#ifndef __PROPERTYID_H__
#define __PROPERTYID_H__

namespace Tenio
{
#define TEXT_NULL			_T("null")
#define TEXT_LOCALRESFOLD	_T("LocalRes")


//////////////////////////////////////////////////////////////////////////
// property值字符串所对应的实际值的类型
//////////////////////////////////////////////////////////////////////////

//! property值字符串所对应的实际值的类型枚举
typedef enum enmValueType
{
	EVT_INT		=0,  /*!< 整型 */ 
	EVT_STRING,      /*!< 字符串类型 */ 
	EVT_DWORD,       /*!< DWORD类型 */ 
	EVT_BYTE,        /*!< BYTE类型 */ 
	EVT_FLOAT,       /*!< 浮点型 */
	EVT_BOOL,        /*!< 布尔型		在 IVariableParam 中以int类型存放 */ 
	EVT_HEX,		 /*!< HEX类型		例如"0xffffff", 在 IVariableParam 中以DWORD类型存放 */  
	EVT_IMAGESET,	 /*!< imageset类型, 例如"ONEIMAGE:XXX.TGA;", 详见编辑器使用文档, 需要分解后存放 */
	EVT_FONTSET, 	 /*!< fontset类型,　例如"subset1;subset2;...", 详见编辑器使用文档，需要分解后存放*/
	EVT_ALIGNMODE,   /*!< alignmode类型,详见编辑器使用文档,　在 IVariableParam 中以int类型存放 */
	EVT_LAYOUTMODE,	 /*!< layoutmode类型, 详见 enmLayoutMode 定义，在 IVariableParam 中以int类型存放*/
	EVT_ENUM,        /*!< 枚举类型，在 IVariableParam 中以DWORD类型存放*/
	EVT_BEHAVIOR     /*!< Behavior类型, 例如"Beeline:xStart,yStart,xEnd, yEnd", 详见编辑器使用文档, 需要分解后存放 */
} ENMVALUETYPE;

// 0x00000000 - 0x00000FFF
// 这个区间的Id用于最常用的Id, 为了加快速度, VariableParam 组件对于这个区间
// 内的 Id 是特殊处理的

#define PARAMID_MESSAGE			0x00000000			//int
#define PARAMID_WPARAM			0x00000001			//DWORD
#define PARAMID_LPARAM			0x00000002			//DWORD
#define PARAMID_RESULT			0x00000003			//DWORD
#define EPARAMID_EVENTID		0x00000004			//int	
#define EPARAMID_CONTROLNAME	0x00000005			//string		
#define EPARAMID_UILIBPTR		0x00000006			//DWORD
#define EPARAMID_USERDOIT		0x00000007			//int 返回值为1,则不继续传消息,否则为0


//////////////////////////////////////////////////////////////////////////
// 以下property对应xml文件中的property name，被uilayoutparser和controls使用。
//////////////////////////////////////////////////////////////////////////
#define PROPERTYID_TYPEID		0x00001000			//DWORD
#define PROPERTYID_XOFFSET		0x00001001			//int
#define PROPERTYID_YOFFSET		0x00001002			//int	
#define PROPERTYID_WIDTH		0x00001003			//int
#define PROPERTYID_HEIGHT		0x00001004			//int
#define PROPERTYID_VISIBLE		0x00001005			//BOOL
#define PROPERTYID_DISABLE		0x00001006			//BOOL
#define PROPERTYID_ALPHA		0x00001007			//byte
#define PROPERTYID_TRANSPARENT	0x00001008			//BOOL	对消息透明
#define PROPERTYID_TIP			0x00001009			//string
#define PROPERTYID_TABSTOP		0x0000100a			//BOOL
#define PROPERTYID_LAYOUTMODE	0x0000100b			//int		//布局自动停靠模式 见 ENMLAYOUTMODE 定义
#define PROPERTYID_OUTERMARGINS	0x0000100c			//string	格式：left,top,right,bottom
#define PROPERTYID_INNERMARGINS	0x0000100d			//string	格式：left,top,right,bottom
#define PROPERTYID_FADEINTIME	0x0000100e			//int		//淡入时延(单位：ms)
#define PROPERTYID_FADEOUTTIME	0x0000100f			//int		//淡出时延(单位：ms)

#define PROPERTYID_BACKGROUND	0x00001010			//imageset
#define PROPERTYID_BG_HOVER		0x00001011			//imageset
#define PROPERTYID_BG_PRESSED	0x00001012			//imageset
#define PROPERTYID_BG_DISABLE	0x00001013			//imageset   
#define PROPERTYID_BG_CHECKED	0x00001014			//imageset
#define PROPERTYID_TRANSCOLOR	0x00001015			//DWORD透明色
#define PROPERTYID_IMAGEOVERTURNMODE 0x00001016     //DWORD     图像翻转类型  见IMAGEOVERTURNMODE定义
#define PROPERTYID_IMAGEPAINTMODE 0x00001017        //enum     图像绘制方式  见ONEIMAGEBGLAYERPAINTMODE定义

#define PROPERTYID_USERDATA		0x00001018			//DWORD  用户应用的customData
#define PROPERTYID_CLASSNAME	0x00001019			//string 用户自定义的classname(主要用于标识组合控件的container)

#define PROPERTYID_CREATETYPE	0x0000101a			//string	//ignore,win,winless
	
#define PROPERTYID_SIZEBOX		0x00001020			//bool
#define PROPERTYID_ALWAYSONTOP	0x00001021			//bool
#define PROPERTYID_TOOLWINDOW	0x00001022			//bool
#define PROPERTYID_CAPTIONHEIGHT	0x00001023		//int
#define PROPERTYID_MINWIDTH		0x00001024			//int
#define PROPERTYID_MINHEIGHT	0x00001025			//int
#define PROPERTYID_MAXWIDTH		0x00001026			//int
#define PROPERTYID_MAXHEIGHT	0x00001027			//int

// topwinodw region
#define PROPERTYID_REGIONSET	0x00001028			//string, 1)NONE:; 2)FROMBGIMAGES:maskcolor; 3)FROMREGIONIMAGES:region imageset

#define PROPERTYID_ICON			0x00001029			//string

// dialog 初始位置
#define PROPERTYID_INITPOSITION  0x0000102a			//string 1)ignore,center,fullscreen
#define PROPERTYID_LAYEREDWINDOW 0x0000102b			//bool TRUE则使窗口具有WM_EX_LAYERED属性，即可以与屏幕做alpha混合

// Add by fiskerlin for static support
#define PROPERTYID_TEXTCOLOR      0x00001030          // DWORD
#define PROPERTYID_ALIGNMODE      0x00001031          // alignmode
#define PROPERTYID_CAPTION        0x00001032          // string
#define PROPERTYID_LOGFONT        0x00001033          // fontset
#define PROPERTYID_AUTOCHANGELINE 0x00001034          // bool

// button
#define PROPERTYID_BUTTONTYPE			0x00001040		//enum:NormalButton,UnreversedButton
#define PROPERTYID_EXCLUDEITEMLIST		0x00001041		//string, 各互斥控件的名字，中间逗号分隔
#define PROPERTYID_SIGNALED				0x00001042		//int, 非零表示设置为Signaled状态
#define PROPERTYID_SIGNALEDNAME			0x00001043		//string, 在singnaled事件中指定被signaled的item name

#define PROPERTYID_TEXTFONT_NORMAL		0x00001044		//string
#define PROPERTYID_TEXTFONT_HOVER		0x00001045		//string
#define PROPERTYID_TEXTFONT_PRESSED		0x00001046		//string
#define PROPERTYID_TEXTCOLOR_HOVER		0x00001047		//DWORD
#define PROPERTYID_TEXTCOLOR_PRESSED	0x00001048		//DWORD

#define PROPERTYID_AUTOSIZE				0x00001049		//bool
#define PROPERTYID_AUTOSIZE_IMAGE		0x0000104a		//bool

#define PROPERTYID_DEFAULTBUTTON		0x0000104b		//bool

#define PROPERTYID_URL					0x0000104c		//string

// 以下是 OverlapContainer 的属性 30-3f
#define PROPERTYID_OVERLAPSELECTITEM	0x00001050		//string, 指定overlapcontainer当前选择的子控件名字
 
// 以下是 SignaledSwitch 的属性 40-4f
#define PROPERTYID_SWITCHADAPTER		0x00001060		//设定 SignaledSwitch 的对应关系
#define PROPERTYID_SWITCHSOURCEITEM		0x00001061		//设定 SignaledSwitch 监听的来源 ControlName

// 以下是TitleBar的属性 50-5f
#define PROPERTYID_HOSTNAME				0x00001070		//string
#define PROPERTYID_CLOSEBUTTONNAME		0x00001071		//string
#define PROPERTYID_MINBUTTONNAME		0x00001072		//string
#define PROPERTYID_MAXBUTTONNAME		0x00001073		//string
#define PROPERTYID_RESTOREBUTTONNAME	0x00001074		//string

// list的属性
#define PROPERTYID_CAPTIONNAMES			0x00001080		//string
#define PROPERTYID_VIRTUALMODE			0x00001082		//bool
#define PROPERTYID_MULTISELECT			0x00001083		//bool
#define PROPERTYID_SELECTWHOLEROW		0x00001084		//bool
#define PROPERTYID_SHOWSELECTEDALWAYS	0x00001085		//bool
#define PROPERTYID_GRIDMODE				0X00001086		//string
#define PROPERTYID_SCROLLBAR			0x00001087		//string
#define PROPERTYID_HEADIMAGE			0x00001088		//string
#define PROPERTYID_HEADSPANIMAGE		0x00001089		//string
#define PROPERTYID_SCROLLIMAGE			0x0000108a		//string
#define PROPERTYID_CAPTIONTEXTCOLOR		0x0000108b		//DWORD
#define PROPERTYID_CAPTIONLOGFONT		0x0000108c		//string
#define PROPERTYID_SELECTTEXTCOLOR		0x0000108d		//DOWRD
#define PROPERTYID_SELECTBGCOLOR		0x0000108e		//DWORD
#define PROPERTYID_GRIDCOLOR			0x0000108f		//DWORD
#define PROPERTYID_ALTERNATEBG			0x00001090		//bool
#define PROPERTYID_ALTERNATEBGCOLOR1	0x00001091		//DWORD
#define PROPERTYID_ALTERNATEBGCOLOR2	0x00001092		//DWORD
#define PROPERTYID_ITEMHEIGHT			0x00001093		//int
#define PROPERTYID_ITEMMARGIN			0x00001094		//int

// Edit
#define PROPERTYID_NUMBERONLY			0x000010a0		//bool
#define PROPERTYID_PASSWORD             0x000010a1      //bool
#define PROPERTYID_MULTILINE            0x000010a2      //bool
#define PROPERTYID_MAXCHARACTERS        0x000010a3      //int
#define PROPERTYID_CARETCOLOR           0x000010a4      //color
#define PROPERTYID_EDITSELECTBGCOLOR    0x000010a5      //color
#define PROPERTYID_EDITSELECTTEXTCOLOR  0x000010a6     //color

// CombobBox
#define PROPERTYID_DROPLIST				0x000010b0		//bool
#define PROPERTYID_BUTTONBG				0x000010b1		//imageset
#define PROPERTYID_BUTTONBGHOVER		0x000010b2		//imageset
#define PROPERTYID_BUTTONBGPRESSED		0x000010b3		//imageset
#define PROPERTYID_BUTTONBGDISABLE		0x000010b4		//imageset
#define PROPERTYID_LISTBOXBACKGROUND	0x000010b5		//imageset
#define PROPERTYID_VISIBLEITEMNUM		0x000010b6		//int
#define PROPERTYID_POPUPLISTBOX			0x000010b7		//bool
#define PROPERTYID_BUTTONWIDTH			0x000010b8		//int
#define PROPERTYID_BUTTONHEIGHT			0x000010b9		//int
#define PROPERTYID_LISTBOXBORDER		0x000010ba		//bool

// Border
#define PROPERTYID_HASBORDER			0x000010c0		//bool
#define PROPERTYID_BORDERCOLOR			0x000010c1		//color

//Level
#define PROPERTYID_LEVELTOPIMAGE		0x000010d0		//string
#define PROPERTYID_LEVELBOTTOMIMAGE		0x000010d1		//string
#define PROPERTYID_LEVELHEIGHT       	0x000010d2		//int
#define PROPERTYID_LEVELWIDTH       	0x000010d3		//int
#define PROPERTYID_TEXTVISIBLE       	0x000010d4		//BOOL

//image property
#define PROPERTYID_IMAGE	            0x000010e0      //string
#define PROPERTYID_IMG_HOVER            0x000010e1		//string
#define PROPERTYID_IMG_DISABLE			0x000010e2		//string  
#define PROPERTYID_IMG_PRESSED	        0x000010e3		//string

//CheckBox
#define PROPERTYID_IMG_CHECKED			0x000010f0		//string
#define PROPERTYID_IMG_DISABLECHECKED	0x000010f1		//string
#define PROPERTYID_IMG_HOVERCHECKED	    0x000010f2		//string
#define PROPERTYID_IMG_PRESSEDCHECKED	0x000010f3		//string

//Slider
#define PROPERTYID_SLIDERTOPIMAGE		0x00001100		//string
#define PROPERTYID_SLIDERBOTTOMIMAGE	0x00001101		//string
#define PROPERTYID_SLIDERCHANNELHEIGHT	0x00001102		//int
#define PROPERTYID_SLIDERISHORIZON  	0x00001103		//bool

//////////////////////////////////////////////////////////////////////////
// sound property
//////////////////////////////////////////////////////////////////////////
#define PROPERTYID_SOUND_CLICK			0x00001110		//string
#define PROPERTYID_SOUND_OPEN			0x00001111		//string

//////////////////////////////////////////////////////////////////////////
// mif property
//////////////////////////////////////////////////////////////////////////
#define PROPERTYID_MIF_FILENAME			0x00001120		//string
#define PROPERTYID_MIF_AUTOPLAY			0x00001121		//bool
#define PROPERTYID_MIF_REPLAY			0x00001122		//bool

//ScrollBar
#define PROPERTYID_SCROLLBARISHORIZON  	0x00001130		//bool
#define PROPERTYID_SB_IMAGEPATH         0x00001131      //string
#define PROPERTYID_SB_MIN_THUMB_LENGTH  0x00001132      //int

//Header
#define PROPERTYID_HDR_DIVIDER			0x00001144		//string
#define PROPERTYID_HDR_DIVIDERHOVER		0x00001145		//string
#define PROPERTYID_HDR_DIVIDERPRESSED	0x00001146		//string
#define PROPERTYID_HDR_DIVIDERDISABLE	0x00001147		//string

//Label
#define PROPERTYID_LBL_TYPE             0x00001150      //string
#define PROPERTYID_LBL_ICONADDRESS      0x00001151      //string
#define PROPERTYID_LBL_INTERVAL         0x00001152      //int

// ToolTip
#define PROPERTYID_TIP_ISGLOBAL			0x00001160		//bool

//ScrollView
#define PROPERTYID_SV_HSCROLL_IMAGEPATH 0x00001170      //string
#define PROPERTYID_SV_VSCROLL_IMAGEPATH 0x00001171      //string
#define PROPERTYID_SV_HAS_HSCROLL       0x00001172      //BOOL
#define PROPERTYID_SV_HAS_VSCROLL       0x00001173      //BOOL
#define PROPERTYID_SV_HSCROLL_HEIGHT    0x00001174      //int
#define PROPERTYID_SV_VSCROLL_WIDTH     0x00001175      //int
#define PROPERTYID_SV_DOCUMENT_WIDTH    0x00001176      //int
#define PROPERTYID_SV_DOCUMENT_HEIGHT   0x00001177      //int
#define PROPERTYID_SV_VIEW_ORIGINX		0x00001178      //int
#define PROPERTYID_SV_VIEW_ORIGINY		0x00001179      //int
#define PROPERTYID_SV_HSCROLL_STEPLEN	0x0000117a      //int
#define PROPERTYID_SV_VSCROLL_STEPLEN	0x0000117b      //int

//Table
#define PROPERTYID_TBL_DOTLINECOLOR     0x00001180      //DWORD
#define PROPERTYID_TBL_DOTLINEWIDTH     0x00001181      //DWORD
#define PROPERTYID_TBL_DOTLINE_VISIBLE  0x00001182      //BOOL
#define PROPERTYID_TBL_GRIDCOLOR		0x00001183      //DWORD
#define PROPERTYID_TBL_GRIDLINEWIDTH    0x00001184      //DWORD
#define PROPERTYID_TBL_ISVIRTUALMODE    0x00001185      //BOOL
#define PROPERTYID_TBL_ITEMINNERMARGINS 0x00001186      //string

//RichEdit
#define PROPERTYID_RE_MAXLINECOUNT      0x00001187      //int
#define PROPERTYID_RE_LINESPACING       0x00001188      //int

// timekeeper
#define PROPERTYID_TIMEKEEOER_MAX		0x00000189		//dword
#define PROPERTYID_TIMEKEEOER_MIN		0x0000018a		//dword
#define PROPERTYID_TIMEKEEOER_MODE		0x0000018b		//dword
#define PROPERTYID_TIMEKEEOER_DIR		0x0000018c		//dword
#define PROPERTYID_TIMEKEEOER_INCREASE	0x0000018d		//bool
#define PROPERTYID_TIMEKEEOER_FLASHTIME	0x0000018e		//dword

#define PROPERTYID_TIMEKEEOER_SPACE		0x0000018f		//dword
#define PROPERTYID_TIMEKEEOER_X			0x00000190		//dword
#define PROPERTYID_TIMEKEEOER_Y			0x00000191		//dword
#define PROPERTYID_TIMEKEEOER_ZEROSHOW	0x00000192		//bool
#define PROPERTYID_TIMEKEEOER_COVERIMG	0x00000193		//string
#define PROPERTYID_TIMEKEEOER_ALPHAIMG	0x00000194		//string
#define PROPERTYID_TIMEKEEOER_POSITION	0x00000195		//string
#define PROPERTYID_TIMEKEEOER_DIGITIMG	0x00000196		//string

//////////////////////////////////////////////////////////////////////////
// BehaviorDesigner
//////////////////////////////////////////////////////////////////////////
#define PROPERTYID_BHVDSGN_OBJECT		0x000001a0      //string
#define PROPERTYID_BHVDSGN_TIME			0x000001a1      //DWORD
#define PROPERTYID_BHVDSGN_PARAM		0x000001a2      //string
#define PROPERTYID_BHVDSGN_ACTION		0x000001a3      //DWORD

//MultiImageVermin
#define PROPERTYID_MIV_DEPOSITOR        0x000001b0      //string

// 注意：Last ID = 0x000001b0
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// 以下property为由xml文件中的复杂property分解出的子property,仅被controls使用
//////////////////////////////////////////////////////////////////////////
// 以下由PROPERTYID_BACKGROUND分解
#define PROPERTYID_BGMODE				0x00010000			//int
#define PROPERTYID_BGIMAGE1				0x00010001			//string
#define PROPERTYID_BGIMAGE2				0x00010002			
#define PROPERTYID_BGIMAGE3				0x00010003
#define PROPERTYID_BGIMAGE4				0x00010004
#define PROPERTYID_BGIMAGE5				0x00010005
#define PROPERTYID_BGIMAGE6				0x00010006
#define PROPERTYID_BGIMAGE7				0x00010007
#define PROPERTYID_BGIMAGE8				0x00010008
#define PROPERTYID_BGIMAGE9				0x00010009
#define PROPERTYID_BGCOLOR				0x0001000a			//DWORD
#define PROPERTYID_BG_IMAGEPAINTMODE    0x0001000b        //enum 
#define PROPERTYID_BGMODE_HOVER			0x00010010			//int
#define PROPERTYID_BGIMAGE1_HOVER		0x00010011			//string
#define PROPERTYID_BGIMAGE2_HOVER		0x00010012			
#define PROPERTYID_BGIMAGE3_HOVER		0x00010013
#define PROPERTYID_BGIMAGE4_HOVER		0x00010014
#define PROPERTYID_BGIMAGE5_HOVER		0x00010015
#define PROPERTYID_BGIMAGE6_HOVER		0x00010016
#define PROPERTYID_BGIMAGE7_HOVER		0x00010017
#define PROPERTYID_BGIMAGE8_HOVER		0x00010018
#define PROPERTYID_BGIMAGE9_HOVER		0x00010019
#define PROPERTYID_BGCOLOR_HOVER		0x0001001a			//DWORD
#define PROPERTYID_BGHOVER_IMAGEPAINTMODE 0x0001001b        //enum 
#define PROPERTYID_BGMODE_PRESSED		0x00010020			//int
#define PROPERTYID_BGIMAGE1_PRESSED		0x00010021			//string
#define PROPERTYID_BGIMAGE2_PRESSED		0x00010022			
#define PROPERTYID_BGIMAGE3_PRESSED		0x00010023
#define PROPERTYID_BGIMAGE4_PRESSED		0x00010024
#define PROPERTYID_BGIMAGE5_PRESSED		0x00010025
#define PROPERTYID_BGIMAGE6_PRESSED		0x00010026
#define PROPERTYID_BGIMAGE7_PRESSED		0x00010027
#define PROPERTYID_BGIMAGE8_PRESSED		0x00010028
#define PROPERTYID_BGIMAGE9_PRESSED		0x00010029
#define PROPERTYID_BGCOLOR_PRESSED		0x0001002a			//DWORD
#define PROPERTYID_BGPRESSED_IMAGEPAINTMODE  0x0001002b     //enum 
#define PROPERTYID_BGMODE_DISABLE		0x00010030			//int
#define PROPERTYID_BGIMAGE1_DISABLE		0x00010031			//string
#define PROPERTYID_BGIMAGE2_DISABLE		0x00010032			
#define PROPERTYID_BGIMAGE3_DISABLE		0x00010033
#define PROPERTYID_BGIMAGE4_DISABLE		0x00010034
#define PROPERTYID_BGIMAGE5_DISABLE 	0x00010035
#define PROPERTYID_BGIMAGE6_DISABLE		0x00010036
#define PROPERTYID_BGIMAGE7_DISABLE		0x00010037
#define PROPERTYID_BGIMAGE8_DISABLE		0x00010038
#define PROPERTYID_BGIMAGE9_DISABLE		0x00010039
#define PROPERTYID_BGCOLOR_DISABLE		0x0001003a			//DWORD
#define PROPERTYID_BGDISABLE_IMAGEPAINTMODE  0x0001003b     //enum 
#define PROPERTYID_BGMODE_CHECKED		0x00010040			//int
#define PROPERTYID_BGIMAGE1_CHECKED		0x00010041			//string
#define PROPERTYID_BGIMAGE2_CHECKED		0x00010042			
#define PROPERTYID_BGIMAGE3_CHECKED		0x00010043
#define PROPERTYID_BGIMAGE4_CHECKED		0x00010044
#define PROPERTYID_BGIMAGE5_CHECKED		0x00010045
#define PROPERTYID_BGIMAGE6_CHECKED		0x00010046
#define PROPERTYID_BGIMAGE7_CHECKED		0x00010047
#define PROPERTYID_BGIMAGE8_CHECKED		0x00010048
#define PROPERTYID_BGIMAGE9_CHECKED		0x00010049
#define PROPERTYID_BGCOLOR_CHECKED		0x0001004a			//DWORD
#define PROPERTYID_BGCHECKED_IMAGEPAINTMODE  0x0001004b     //enum 

// 以下由PROPERTYID_LOGFONT分解
#define PROPERTYID_LOGFONT_HEIGHT         0x0001004b		//int
#define PROPERTYID_LOGFONT_WIDTH          0x0001004c		//int
#define PROPERTYID_LOGFONT_ESCAPEMENT     0x0001004d		//int
#define PROPERTYID_LOGFONT_ORIENTATION    0x0001004e		//int
#define PROPERTYID_LOGFONT_WEIGHT         0x0001004f		//byte
#define PROPERTYID_LOGFONT_ITALIC         0x00010050		//byte
#define PROPERTYID_LOGFONT_UNDERLINE      0x00010051		//byte
#define PROPERTYID_LOGFONT_STRIKEOUT      0x00010052		//byte
#define PROPERTYID_LOGFONT_CHARSET        0x00010053		//byte
#define PROPERTYID_LOGFONT_OUTPRECISION   0x00010054		//byte
#define PROPERTYID_LOGFONT_CLIPPRECISION  0x00010055		//byte
#define PROPERTYID_LOGFONT_QUALITY        0x00010056		//byte
#define PROPERTYID_LOGFONT_PITCHANDFAMILY 0x00010057		//byte
#define PROPERTYID_LOGFONT_FACENAME       0x00010058		//string

// 由　RegionSet分解
#define PROPERTYID_MASKCOLOR			  0x00010060		//dword

// 以下由PROPERTYID_BHVDSGN_PARAM分解
#define PROPERTYID_BHVDSGN_MODE           0x00010070        //DWORD   BEHAVIORMODE枚举
#define PROPERTYID_BD_START_LEFT          0x00010071        //int
#define PROPERTYID_BD_START_TOP           0x00010072        //int
#define PROPERTYID_BD_END_LEFT            0x00010073        //int
#define PROPERTYID_BD_END_TOP             0x00010074        //int
#define PROPERTYID_BD_START_RIGHT         0x00010075        //int
#define PROPERTYID_BD_START_BOTTOM        0x00010076        //int
#define PROPERTYID_BD_END_RIGHT           0x00010077        //int
#define PROPERTYID_BD_END_BOTTOM          0x00010078        //int

// 注意：Last ID = 0x00010079
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// window message
// 
// 当message为WM_NOTIFY时，表示此消息是由子控件发出并通知父控件的，
// wParam为子控件ID，lParam为子控件消息参数集（包括下面四个参数）
//
// 当message为WM_CREATE时，表示控件创建成功，wParam为控件创建时的属性集
//////////////////////////////////////////////////////////////////////////
//#define PARAMID_MESSAGE			0x00020000			//int
//#define PARAMID_WPARAM			0x00020001			//DWORD
//#define PARAMID_LPARAM			0x00020002			//DWORD
//#define PARAMID_RESULT			0x00020003			//DWORD

//////////////////////////////////////////////////////////////////////////
#define PARAMID_CONTROLSMGRPTR		0x00020004		//DWORD
#define PARAMID_BOUNDARYMGRPTR		0x00020005		//DWORD
#define PARAMID_UILIBPTR			0x00020006		//DWORD
#define PARAMID_LAYOUTMGRPTR		0x00020007		//DWORD

#define PARAMID_TOPWINDOWPARENTHANDLE	0x00020007		//DWORD
#define PARAMID_TOPWINDOWHANDLE			0x00020008		//DWORD
//////////////////////////////////////////////////////////////////////////
// control event
//////////////////////////////////////////////////////////////////////////
//#define EPARAMID_EVENTID		0x00030000			//int	
//#define EPARAMID_CONTROLNAME	0x00030001			//string		
//#define EPARAMID_UILIBPTR		0x00030002			//DWORD
//////////////////////////////////////////////////////////////////////////
// common Event
//////////////////////////////////////////////////////////////////////////
#define EI_POSITONCHANGED			0x0003ff00 //控件矩形框发生改变
#define EI_BASEMESSAGE				0x0003ff30 //基本window消息，参数为window message的四个PARAMID

enum
{
	ACCEL_KEY_DOWN		=0,
	ACCEL_KEY_UP		=1,
	ACCEL_KEY_PRESS	=2,
};

#define EI_ACCELERATOR				0x0003ff10 //快捷键消息，
#define EPARAMID_ACCEL_TYPE			0x0003ff11 //消息类型down:0;up:1　		int 
#define EPARAMID_ACCEL_KEY			0x0003ff12 //主键值						BYTE
#define EPARAMID_ACCEL_SHIFT		0x0003ff13 //是否附加shift键			bool
#define EPARAMID_ACCEL_CTRL			0x0003ff14 //是否附加ctrl键				bool
#define EPARAMID_ACCEL_ALT			0x0003ff15 //是否附加alt键				bool
//////////////////////////////////////////////////////////////////////////
// Button Event
//////////////////////////////////////////////////////////////////////////
#define EI_BUTTON_CLICK				0x00030100
#define EPARAMID_BUTTON_CLICK_X		0x00030101		//int
#define EPARAMID_BUTTON_CLICK_Y		0x00030102		//int

#define EI_SIGNALED					0X00030110		//Button/Radio Button 有可能发出这个消息
#define EI_UNSIGNALED				0x00030111		//取消 SIGNALED 状态

//////////////////////////////////////////////////////////////////////////
// Dialog Event
//////////////////////////////////////////////////////////////////////////
#define EI_DIALOG_CLOSE				0x00030200
#define EPARAMID_DIALOG_CLOSE_APPDO		0x00030201		//int	是否由应用处理关闭操作
//////////////////////////////////////////////////////////////////////////
// ScrollBar Event
//////////////////////////////////////////////////////////////////////////
#define EI_SCROLLBAR_THUMBPOSCHANGED	0x00030300
#define EPARAMID_SB_SCROLLTYPE	0x00030301	//int
#define EPARAMID_SB_SCROLLINFO	0x00030302	//DWORD (LPSCROLLINFO)
#define EI_SCROLLBAR_MOUSELEAVE	0x00030303

//////////////////////////////////////////////////////////////////////////
// Header Event
//////////////////////////////////////////////////////////////////////////
#define EI_HEADER_BUTTONDBCLICK	0x00030310
#define EPARAMID_HDR_OBJECTTYPE 0x00030311	//DWORD (HEADEROBJECTTYPE)
#define EPARAMID_HDR_INDEX  	0x00030312	//DWORD 
#define EI_HEADER_BUTTONCLICK	0x00030313
#define EI_HEADER_BUTTONDOWN	0x00030314
#define EI_HEADER_BUTTONUP		0x00030315
#define EI_HEADER_MOUSEMOVE		0x00030316
#define EI_HEADER_CONTENTRESIZE 0x00030317
#define EPARAMID_HDR_CONTENT_WIDTH  0x00030318	//int 

//////////////////////////////////////////////////////////////////////////
// Table Event
//////////////////////////////////////////////////////////////////////////
#define EI_TABLE_SETLINE	         0x00030320
#define EPARAMID_TBL_ROWINDEX        0x00030321	//int
#define EI_TABLE_SETCOLUMN           0x00030322
#define EPARAMID_TBL_COLUMNINDEX     0x00030323 //int
#define EI_TABLE_ITEMCLICK           0x00030324
#define EI_TABLE_ITEMDBCLICK         0x00030325
#define EI_TABLE_ITEMRCLICK          0x00030326
#define EI_DOCUMENT_CHANGE           0x00030327
#define EPARAMID_DOCUMENT_WIDTH      0x00030328 //int
#define EPARAMID_DOCUMENT_HEIGHT     0x00030329 //int
#define EI_TABLE_SET_ITEM_MAX_WIDTH_IN_COL 0x0003032a
#define EPARAMID_TBL_COLUMN_INDEX    0x0003032b //int
#define EPARAMID_TBL_ITEM_MAX_WIDTH  0x0003032c //DWORD(int *)

//////////////////////////////////////////////////////////////////////////
// List Event
//////////////////////////////////////////////////////////////////////////
#define EI_LIST_CLICKITEM	0x00030400
#define EI_LIST_DBCLICKITEM	0x00030410
#define EI_LIST_RCLICKITEM	0x00030420
#define EI_LIST_CLICKCOLUMN	0x00030430

#define EPARAMID_ITEM_INDEX		0x00030401	//int
#define EPARAMID_SUBITEM_INDEX	0x00030402	//int
#define EPARAMID_COLUMN_INDEX	0x00030403	//int

#define EI_LIST_GETDISPLAYINFO	0x00030440
#define EPARAMID_LIST_ITEMINFO	0x00030441	//DWORD (LISTITEMINFO)

#define EI_LIST_CELLCONTROLEVENT	0x00030450
#define EPARAMID_CC_EVENTID			0x00030451	//int
#define EPARAMID_CC_EVENTAREAID		0x00030452	//int

//////////////////////////////////////////////////////////////////////////
// ListBox 
//////////////////////////////////////////////////////////////////////////
#define EI_LISTBOX_SELECTITEM		0x00030500

//////////////////////////////////////////////////////////////////////////
// Combobox Event
//////////////////////////////////////////////////////////////////////////
#define EI_COMBOBOX_SELCHANGED	0x00030600
#define EI_COMBOBOX_TEXTCHANGED	0x00030610

//////////////////////////////////////////////////////////////////////////
// Slider Event
//////////////////////////////////////////////////////////////////////////
#define EI_SLIDER_THUMBPOSCHANGED	0x00030700
#define EPARAMID_SLD_TYPE	    0x00030701	//int

//////////////////////////////////////////////////////////////////////////
// MifImageBox Event
//////////////////////////////////////////////////////////////////////////
#define EI_MIF_FRAME_CHANGE				0x00030800
#define EI_MIF_FRAME_TRIGER				0x00030801
#define EPARAMID_MIF_FRAME_INDEX		0x00030802		//int

//////////////////////////////////////////////////////////////////////////
// UIBinder UserInfo 0x00030810 ~ 0x0003082f
//////////////////////////////////////////////////////////////////////////
#define PROPERTYID_UIBIND_UIN			0x00030810  //string
#define PROPERTYID_UIBIND_NICK			0x00030811  //string
#define PROPERTYID_UIBIND_ORG			0x00030812  //string
#define PROPERTYID_UIBIND_POSITION		0x00030813  //string
#define PROPERTYID_UIBIND_ESCRATE		0x00030814  //string
#define PROPERTYID_UIBIND_WINRATE		0x00030815  //string
#define PROPERTYID_UIBIND_OFFRATE		0x00030816  //string
#define PROPERTYID_UIBIND_POINTS		0x00030817  //string
#define PROPERTYID_UIBIND_MONEY			0x00030818  //string
#define PROPERTYID_UIBIND_WINGAMES		0x00030819  //string
#define PROPERTYID_UIBIND_LOSSGAMES		0x0003081a  //string
#define PROPERTYID_UIBIND_EQUALGAMES	0x0003081b  //string
#define PROPERTYID_UIBIND_GAMESHOW		0x0003081c  //string
#define PROPERTYID_UIBIND_USERLIST		0x0003081d  //string

//////////////////////////////////////////////////////////////////////////
// ScrollView Event
//////////////////////////////////////////////////////////////////////////
#define EI_SV_CLIENTCHANGE				0x00030820
#define EPARAMID_SV_CLIENT_HEIGHT		0x00030821		//int
#define EPARAMID_SV_CLIENT_WIDTH		0x00030822		//int

//////////////////////////////////////////////////////////////////////////
// UI事件属性，用于支持脚本 0x000F0000 ~ 0x000FFFFF 
// 每个事件属性值由PROPERTYID_EVENTFUNC_BEGIN加上对应脚本函数序号
// 见
//////////////////////////////////////////////////////////////////////////
#define PROPERTYID_EVENTFUNC_BEGIN  0x000F0000
#define PROPERTYID_EVENTFUNC_END    0x000FFFFF
}

#endif