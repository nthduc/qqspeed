#ifndef __UILIBDEFINE_H__
#define __UILIBDEFINE_H__

#include "windows.h"
namespace Tenio
{
#define INVALID_CONTROLID	0
#define ROOT_CONTROLID		1

#define MAX_TIP_LENGTH			256
#define MAX_CONTROLNAME_LENGTH	64
#define URL_LENGTH				1024
//////////////////////////////////////////////////////////////////////////
//define common value
#define INVALID_ITEM_ID			0xFFFFFFFF
#define INVALID_ITEM_INDEX		(-1)

// 定义一个item上字符串的最大长度
#define MAX_ITEM_NAME_LENGTH	128
	
//////////////////////////////////////////////////////////////////////////
//enum
//////////////////////////////////////////////////////////////////////////
//! 控件在布局中的停靠模式
typedef enum enmLayoutMode
{
	ELM_NONE				= 0,	/*!< 没有位置模式，该放哪放哪 */
	ELM_ALIGN_LEFT			,		/*!< 靠左边 */ 
	ELM_ALIGN_RIGHT			,		/*!< 靠右边 */ 
	ELM_ALIGN_TOP			,		/*!< 靠顶部 */ 
	ELM_ALIGN_BOTTOM		,		/*!< 靠底部 */
	ELM_FILL_UNUSED			,		/*!< 填充余下的所有空间 */

	ELM_ANCHOR_TOPLEFT		,		/*!< 上边距和左边距不变 */
	ELM_ANCHOR_TOPRIGHT		,		/*!< 上边距和右边距不变 */
	ELM_ANCHOR_BOTTOMLEFT	,		/*!< 下边距和左边距不变 */
	ELM_ANCHOR_BOTTOMRIGHT			/*!< 下边距和右边距不变 */
} ENMLAYOUTMODE;

#define TEXT_ELM_NONE			_T("None")
#define TEXT_ELM_LEFT			_T("Left")
#define TEXT_ELM_RIGHT			_T("Right")
#define TEXT_ELM_TOP			_T("Top")
#define TEXT_ELM_BOTTOM			_T("Bottom")
#define TEXT_ELM_FILL			_T("Fill")
#define TEXT_ELM_TOPLEFT		_T("TopLeft")
#define TEXT_ELM_TOPRIGHT		_T("TopRight")
#define TEXT_ELM_BOTTOMLEFT		_T("BottomLeft")
#define TEXT_ELM_BOTTOMRIGHT	_T("BottomRight")
//////////////////////////////////////////////////////////////////////////
// TDialog

// dialog 的初始位置
typedef enum enmInitPosition
{
	EIP_IGNORE	= 0,
	EIP_CENTER,
	EIP_FULLSCREEN
} ENMINITPOSITION;
#define TEXT_EIP_IGNORE			_T("ignore")
#define TEXT_EIP_CENTER			_T("center")
#define TEXT_EIP_FULLSCREEN		_T("fullscreen")

//////////////////////////////////////////////////////////////////////////
// SkinList
#define AREA_ID_LIST_HEAD_BG			0x00000030
#define AREA_ID_LIST_HEAD_SPAN			0x00000031
//////////////////////////////////////////////////////////////////////////
// 定义单元格内的小控件类型
#define LIST_CONTROL_TYPE_NONE		0
#define LIST_CONTROL_TYPE_SPIN		1
#define LIST_CONTROL_TYPE_BUTTON	2
	
// 单元格小控件事件ID
#define LIST_CONTROL_EVENT_ID_CLICK			0
#define LIST_CONTROL_EVENT_ID_LBUTTONDOWN	1
#define LIST_CONTROL_EVENT_ID_LBUTTONUP		2
	
// 单元格小控件的事件响应区域ID
#define LIST_CONTROL_EVENTAREA_ID_BUTTON		4
#define LIST_CONTROL_EVENTAREA_ID_SPIN_LEFT		5
#define LIST_CONTROL_EVENTAREA_ID_SPIN_RIGHT	6
//////////////////////////////////////////////////////////////////////////

//将item插入到列表尾
#define LIST_INSERT_LAST	(-1)
#define ITEMPOSITION		DWORD


//滚动条尺寸
#define SCROLL_BTN_LENGTH		15
#define SCROLL_WIDTH			15
#define SCROLL_MIN_THUMB_LENGTH	12

//滚动条部件的各种状态
#define SCROLL_STATE_NORMAL		0
#define SCROLL_STATE_HOVER		1
#define SCROLL_STATE_SELECT		2
#define SCROLL_STATE_DISABLE	3

//滚动条类型
#define SCROLL_VERT				0
#define SCROLL_HORZ				1

//滚动条中各部件名称
#define SCROLL_ITEM_PREBTN		0
#define SCROLL_ITEM_NEXTBTN		1
#define SCROLL_ITEM_PASSED		2
#define SCROLL_ITEM_UNPASS		3
#define SCROLL_ITEM_THUMB		4

//Slider类型
#define SLIDER_VERT				0
#define SLIDER_HORZ				1

//////////////////////////////////////////////////////////////////////////
	
// 按钮的状态，splitter,tab,button都用此状态
#define BUTTON_STATE_NORMAL		0x0001		//正常
#define BUTTON_STATE_HOVER		0x0002		//悬停
#define BUTTON_STATE_PRESS		0x0004		//按下
#define BUTTON_STATE_CHECKED	0x0008		//选中
#define BUTTON_STATE_DISABLE	0x0010		//不响应点击
#define BUTTON_STATE_DEFAULT	0x0020		//缺省按钮
#define BUTTON_STATE_DISABLE_DEFAULT	0x0040	//禁用时为缺省按钮}

}
#endif