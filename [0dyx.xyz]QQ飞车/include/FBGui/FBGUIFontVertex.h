#pragma once
#include <d3d9.h>

struct FONT2DVERTEX
{
	float x;
	float y;
	float z;
	float rhw;
	DWORD color;
	float tu;
	float tv;
	FONT2DVERTEX(float x_,float y_,float z_,float rhw_,DWORD color_,float tu_,float tv_)
	{
		x=x_;
		y=y_;
		z=z_;
		rhw=rhw_;
		color=color_;
		tu=tu_;
		tv=tv_;
	}
	static const unsigned long FVF_2D;
	FONT2DVERTEX(){}
};
