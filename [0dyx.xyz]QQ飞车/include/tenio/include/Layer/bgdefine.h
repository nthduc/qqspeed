#ifndef __BGDEFINE_H__
#define __BGDEFINE_H__

namespace Tenio
{
	//////////////////////////////////////////////////////////////////////////
	// background define
	//////////////////////////////////////////////////////////////////////////
	typedef enum enmBGMode
	{
		EBGM_NONE		= 0,
		EBGM_COLOR,
		EBGM_ONEIMAGE,
		EBGM_3HIMAGES,
		EBGM_3VIMAGES,
		EBGM_9IMAGES
	} ENMBGMODE;
	
	#define TEXT_IMAGESET_NONE		_T("NONE")
	#define TEXT_IMAGESET_COLOR		_T("COLOR")
	#define TEXT_IMAGESET_ONEIMAGE	_T("ONEIMAGE")
	#define TEXT_IMAGESET_3HIMAGES	_T("3HIMAGES")
	#define TEXT_IMAGESET_3VIMAGES	_T("3VIMAGES")
	#define TEXT_IMAGESET_9IMAGES	_T("9IMAGES")
	
	typedef enum enmImagePos
	{
		EIP_TOPLEFT		= 0,
			EIP_TOP,
			EIP_TOPRIGHT,
			EIP_LEFT,
			EIP_MIDDLE,
			EIP_RIGHT,
			EIP_BOTTOMLEFT,
			EIP_BOTTOM,
			EIP_BOTTOMRIGHT
	} ENMIMAGEPOS;
}

#endif