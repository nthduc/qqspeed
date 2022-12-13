#ifndef __CANVASDEFINE_H__
#define __CANVASDEFINE_H__

namespace Tenio
{

//! 画笔类型
typedef enum enmPenStyle
{
	EPS_SOLID = 0,
	EPS_DOT = 2
} ENMPENSTYLE;

//! 画笔信息
typedef struct tagPenInfo
{
	COLORREF color;
	UINT nWidth;
	ENMPENSTYLE	eStyle;
} PENINFO;


}

#endif