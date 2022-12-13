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

// ����һ��item���ַ�������󳤶�
#define MAX_ITEM_NAME_LENGTH	128
	
//////////////////////////////////////////////////////////////////////////
//enum
//////////////////////////////////////////////////////////////////////////
//! �ؼ��ڲ����е�ͣ��ģʽ
typedef enum enmLayoutMode
{
	ELM_NONE				= 0,	/*!< û��λ��ģʽ���÷��ķ��� */
	ELM_ALIGN_LEFT			,		/*!< ����� */ 
	ELM_ALIGN_RIGHT			,		/*!< ���ұ� */ 
	ELM_ALIGN_TOP			,		/*!< ������ */ 
	ELM_ALIGN_BOTTOM		,		/*!< ���ײ� */
	ELM_FILL_UNUSED			,		/*!< ������µ����пռ� */

	ELM_ANCHOR_TOPLEFT		,		/*!< �ϱ߾����߾಻�� */
	ELM_ANCHOR_TOPRIGHT		,		/*!< �ϱ߾���ұ߾಻�� */
	ELM_ANCHOR_BOTTOMLEFT	,		/*!< �±߾����߾಻�� */
	ELM_ANCHOR_BOTTOMRIGHT			/*!< �±߾���ұ߾಻�� */
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

// dialog �ĳ�ʼλ��
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
// ���嵥Ԫ���ڵ�С�ؼ�����
#define LIST_CONTROL_TYPE_NONE		0
#define LIST_CONTROL_TYPE_SPIN		1
#define LIST_CONTROL_TYPE_BUTTON	2
	
// ��Ԫ��С�ؼ��¼�ID
#define LIST_CONTROL_EVENT_ID_CLICK			0
#define LIST_CONTROL_EVENT_ID_LBUTTONDOWN	1
#define LIST_CONTROL_EVENT_ID_LBUTTONUP		2
	
// ��Ԫ��С�ؼ����¼���Ӧ����ID
#define LIST_CONTROL_EVENTAREA_ID_BUTTON		4
#define LIST_CONTROL_EVENTAREA_ID_SPIN_LEFT		5
#define LIST_CONTROL_EVENTAREA_ID_SPIN_RIGHT	6
//////////////////////////////////////////////////////////////////////////

//��item���뵽�б�β
#define LIST_INSERT_LAST	(-1)
#define ITEMPOSITION		DWORD


//�������ߴ�
#define SCROLL_BTN_LENGTH		15
#define SCROLL_WIDTH			15
#define SCROLL_MIN_THUMB_LENGTH	12

//�����������ĸ���״̬
#define SCROLL_STATE_NORMAL		0
#define SCROLL_STATE_HOVER		1
#define SCROLL_STATE_SELECT		2
#define SCROLL_STATE_DISABLE	3

//����������
#define SCROLL_VERT				0
#define SCROLL_HORZ				1

//�������и���������
#define SCROLL_ITEM_PREBTN		0
#define SCROLL_ITEM_NEXTBTN		1
#define SCROLL_ITEM_PASSED		2
#define SCROLL_ITEM_UNPASS		3
#define SCROLL_ITEM_THUMB		4

//Slider����
#define SLIDER_VERT				0
#define SLIDER_HORZ				1

//////////////////////////////////////////////////////////////////////////
	
// ��ť��״̬��splitter,tab,button���ô�״̬
#define BUTTON_STATE_NORMAL		0x0001		//����
#define BUTTON_STATE_HOVER		0x0002		//��ͣ
#define BUTTON_STATE_PRESS		0x0004		//����
#define BUTTON_STATE_CHECKED	0x0008		//ѡ��
#define BUTTON_STATE_DISABLE	0x0010		//����Ӧ���
#define BUTTON_STATE_DEFAULT	0x0020		//ȱʡ��ť
#define BUTTON_STATE_DISABLE_DEFAULT	0x0040	//����ʱΪȱʡ��ť}

}
#endif