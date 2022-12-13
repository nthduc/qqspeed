#ifndef __TLISTCELLCONTROL_I_H__
#define __TLISTCELLCONTROL_I_H__

#include "uilibdefine.h"

namespace Tenio
{
	class ITListCellControl;
//////////////////////////////////////////////////////////////////////////
// ���ӿ�����Ҫ���뵽list��Ԫ���е�UIObject�̳�

class ITListCellControl
{
public:
	// ���Ƹõ�Ԫ��
	// ������޴��ڶ��������;
	// �д��ڶ�����ˢ���Լ�, Ȼ�󽫴����Ƶ���Ԫ������λ��
	virtual void DrawCellControl(LISTITEMINFO* pItemInfo, HWND hWndList) = 0;
	// ����
	virtual void HideCellControl() = 0;
	
	// ����true,����Ϣ���ӿؼ�����list���ٴ������Ϣ��Ҳ�������ⷢ�¼���
	// ��֮���Լ�����ȱʡ�������Ϣ
	// ptΪ��ǰ���λ�������cell���Ͻǵ�����ֵ
	virtual BOOL NotifyListCellMouseMove(int nItem, int nSubItem, POINT pt) = 0;
	virtual BOOL NotifyListCellMouseLeave(int nItem, int nSubItem, POINT pt) = 0;
	virtual BOOL NotifyListCellLButtonDown(int nItem, int nSubItem, POINT pt) = 0;
	virtual BOOL NotifyListCellLButtonUp(int nItem, int nSubItem, POINT pt) = 0;
};
}

#endif