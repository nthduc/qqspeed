#pragma once

#include "NiGeometry.h"
#include "Avatar/ItemDefine.h"

struct SColorAttrCustom
{
	int nIndex;
	SColorAttr colorList[8];
};

///��ɫ������ʹ��ɫ����ɫģʽ�����pClrAttr!=NULL,roleIdx��colorIdx��Ч
void changeColor( NiGeometry* pGeometry, int roleIdx, int colorIdx,SColorAttr* pClrAttr=NULL);
