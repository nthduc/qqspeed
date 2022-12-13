////////////////////////////////////////////////
//     FileName:    gprender.h
//     Author:      Fish
//     Date:        2007-1-23 21:22:47
//     Description: 封装一些跟渲染相关的函数方便使用
////////////////////////////////////////////////

#ifndef _GPRENDER_H_
#define _GPRENDER_H_

class CGPRender
{
public:
	/*
	CreateBillboard:
	创建一个简单的公告版
	*/
	static NiBillboardNodePtr CreateBillboard(int w, int h);
};
#endif