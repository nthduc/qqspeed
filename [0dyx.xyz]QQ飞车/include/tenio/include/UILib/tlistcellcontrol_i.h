#ifndef __TLISTCELLCONTROL_I_H__
#define __TLISTCELLCONTROL_I_H__

#include "uilibdefine.h"

namespace Tenio
{
	class ITListCellControl;
//////////////////////////////////////////////////////////////////////////
// 本接口由需要插入到list单元格中的UIObject继承

class ITListCellControl
{
public:
	// 绘制该单元格，
	// 如果是无窗口对象，则绘制;
	// 有窗口对象则刷新自己, 然后将窗口移到单元格所有位置
	virtual void DrawCellControl(LISTITEMINFO* pItemInfo, HWND hWndList) = 0;
	// 隐藏
	virtual void HideCellControl() = 0;
	
	// 返回true,则消息由子控件处理，list不再处理该消息，也不再向外发事件，
	// 反之则仍继续按缺省处理该消息
	// pt为当前鼠标位置相对于cell左上角的坐标值
	virtual BOOL NotifyListCellMouseMove(int nItem, int nSubItem, POINT pt) = 0;
	virtual BOOL NotifyListCellMouseLeave(int nItem, int nSubItem, POINT pt) = 0;
	virtual BOOL NotifyListCellLButtonDown(int nItem, int nSubItem, POINT pt) = 0;
	virtual BOOL NotifyListCellLButtonUp(int nItem, int nSubItem, POINT pt) = 0;
};
}

#endif