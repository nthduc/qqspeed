#ifndef __UIOBJECT_STRUCT_H__
#define __UIOBJECT_STRUCT_H__

#include "list"
#include "tstring.h"
using namespace std;

enum
{
	UIOBJECT_IMAGE,
	UIOBJECT_TEXT,
	UIOBJECT_LINE,
	UIOBJECT_RECT,
	UIOBJECT_FRAMERECT,
	UIOBJECT_LAYER
};

typedef struct tagUIOBJECT
{
	BYTE byFormat;
	int x;
	int y;
	RECT rcClip;
	BYTE byAlpha;
	BOOL  bVisible;
} UIOBJECT,FAR *LPUIOBJECT;

typedef struct tagIMAGEUIOBJECT
{
	UIOBJECT   uiobj;
	LPVOID     pImage;	// 图象对象， 各种引擎对应不一样， 自行转换
	int		   nFrame;
} IMAGEUIOBJECT, FAR *LPIMAGEUIOBJECT;

typedef struct tagTEXTUIOBJECT
{
	UIOBJECT   uiobj;

    tstring	lpszText;
	COLORREF clrText;

	int nCharWidth;
} TEXTUIOBJECT, FAR *LPTEXTUIOBJECT;

typedef struct tagLINEUIOBJECT
{
	UIOBJECT   uiobj;

	COLORREF clr;

	int	nWidth;
	int nHeight;
	int nLineWidth;
} LINEUIOBJECT, FAR *LPLINEUIOBJECT;

typedef struct tagRECTUIOBJECT
{
	UIOBJECT   uiobj;

	COLORREF clr;

	int	nWidth;
	int nHeight;
} RECTUIOBJECT, FAR *LPRECTUIOBJECT;

typedef struct tagFRAMEUIOBJECT
{
	UIOBJECT   uiobj;

	COLORREF clr;

	int	nWidth;
	int nHeight;
	int nLineWidth;
} FRAMEUIOBJECT, FAR *LPFRAMEUIOBJECT;

typedef struct tagLAYERUIOBJECT
{
	UIOBJECT	uiobj;
	void*		pLayer;
} LAYERUIOBJECT, FAR *LPLAYERUIOBJECT;

typedef list<LPUIOBJECT>	LISTUIOBJECT;


#endif
