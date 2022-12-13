#ifndef __TEXTDEFINE_H__
#define __TEXTDEFINE_H__

namespace Tenio
{
	typedef enum enmAlignMode
	{
		EALIGNM_LEFT = DT_LEFT,
		EALIGNM_RIGHT = DT_RIGHT,
		EALIGNM_CENTER = DT_CENTER
	} ENMALIGNMODE;
	
	#define TEXT_ALIGNMODE_LEFT   _T("LEFT")
	#define TEXT_ALIGNMODE_RIGHT  _T("RIGHT")
	#define TEXT_ALIGNMODE_CENTER _T("CENTER")

}; // Tenio

#endif // __TEXTDEFINE_H__