#ifndef __TSCROLLBAR_I_H__
#define __TSCROLLBAR_I_H__

#include "../teniointerface.h"
#include "control_i.h"

namespace Tenio
{
	class ITScrollBar;

	TENIO_MAKE_TYPEID_MAP(ITScrollBar, COMPONENTID_TSCROLLBAR);

	class ITScrollBar : public IControl
	{
	public:
		virtual void InitScroll(int nType, 
			int nScrollWidth, 
			int nBtnLength, 
			int nMinThumbLength) = 0;	
		virtual void SetScrollPos(int nPos, BOOL bRedraw) = 0;
		virtual void SetScrollInfo(LPSCROLLINFO lpsi, BOOL bRedraw) = 0;
		virtual void SetScrollInfo(
			int nScrollLength, //滚动总长度
			int nPageLength,   //单页长度
			int nScrollStep,   //单步长度
			int nCurPos,	   //当前位置（相对于滚动总长度）
			BOOL bRedraw) = 0;
		virtual void GetScrollInfo(LPSCROLLINFO lpsi) = 0;
		virtual int  GetScrollPos() = 0;
		virtual int  GetScrollWidth() = 0;
	};
};

#endif

// InterfaceName:TScrollBar
// SubFolderName:UILib
