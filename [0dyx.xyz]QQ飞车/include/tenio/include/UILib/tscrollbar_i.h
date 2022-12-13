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
			int nScrollLength, //�����ܳ���
			int nPageLength,   //��ҳ����
			int nScrollStep,   //��������
			int nCurPos,	   //��ǰλ�ã�����ڹ����ܳ��ȣ�
			BOOL bRedraw) = 0;
		virtual void GetScrollInfo(LPSCROLLINFO lpsi) = 0;
		virtual int  GetScrollPos() = 0;
		virtual int  GetScrollWidth() = 0;
	};
};

#endif

// InterfaceName:TScrollBar
// SubFolderName:UILib
