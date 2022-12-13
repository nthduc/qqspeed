#include "FBGUIBase.h"
#include "FBGUICore.h"
#include "NiDX9Renderer.h"
namespace FBGUI
{
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
	const unsigned long FONT2DVERTEX::FVF_2D=D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1;

	// ************************************************************ //
	// CGUISize
	// ************************************************************ //

	CGUIPoint CGUISize::operator - ( const CGUIPoint& rPoint ) const
	{
		return rPoint - *this; 
	}

	CGUIRect CGUISize::operator - ( const CGUIRect& rRect ) const
	{
		return rRect - *this;	
	}

	CGUIPoint CGUISize::operator + ( const CGUIPoint& rPoint ) const
	{
		return rPoint + *this; 
	}

	CGUIRect CGUISize::operator + ( const CGUIRect& rRect ) const
	{
		return rRect + *this; 
	}




}