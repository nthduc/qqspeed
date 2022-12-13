#pragma once

#include "NiGeometry.h"
#include "Avatar/ItemDefine.h"

struct SColorAttrCustom
{
	int nIndex;
	SColorAttr colorList[8];
};

///换色函数，使用色度颜色模式，如果pClrAttr!=NULL,roleIdx和colorIdx无效
void changeColor( NiGeometry* pGeometry, int roleIdx, int colorIdx,SColorAttr* pClrAttr=NULL);
