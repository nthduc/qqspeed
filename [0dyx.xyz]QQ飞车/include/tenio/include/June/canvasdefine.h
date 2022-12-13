#ifndef __CANVASDEFINE_H__
#define __CANVASDEFINE_H__

namespace Tenio
{

//! ��������
typedef enum enmPenStyle
{
	EPS_SOLID = 0,
	EPS_DOT = 2
} ENMPENSTYLE;

//! ������Ϣ
typedef struct tagPenInfo
{
	COLORREF color;
	UINT nWidth;
	ENMPENSTYLE	eStyle;
} PENINFO;


}

#endif