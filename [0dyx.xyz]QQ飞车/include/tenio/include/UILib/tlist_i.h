#ifndef __TWINLIST_I_H__
#define __TWINLIST_I_H__

#include "../teniointerface.h"
#include "control_i.h"
#include "uilibdefine.h"
#include "commctrl.h"

namespace Tenio
{
	class ITList;

	TENIO_MAKE_TYPEID_MAP(ITList, COMPONENTID_TLIST);

	class ITList : public IControl
	{
	public:
		// 虚模式下使用
		virtual void SetItemCount(int nItemCount) = 0;
		
		virtual int  InsertItem(int nIndex, UINT nItemID) = 0;
		virtual void DeleteItem(int nIndex) = 0;
		virtual void DeleteAllItem() = 0;
		virtual void SetSubItem(
			int nIndex, 
			int nSubIndex, 
			LPCTSTR lpszValue, 
			int nImageIndex, 
			int nImageListIndex,
			DWORD dwParam) = 0;
		
		virtual ITEMPOSITION GetFirstSelectedItem() = 0;
		virtual int GetNextSelectedItemIndex(ITEMPOSITION &pos) = 0;
		
		virtual void SelectRow(int nIndex) = 0;
		virtual UINT GetItemIDbyIndex(int nItemIndex) = 0;
		virtual int  GetItemIndexbyID(UINT nItemID) = 0;		
		virtual int  AddImageList(HIMAGELIST hImageList) = 0;		
		
		//! 设置单元格子控件类型和ID,每一列的控件type和ID相同，ID由应用定义，设置贴图时使用。
		virtual void SetSubItemCtrl(int nColumnIndex, int nCtrlType, UINT nCtrlID) = 0;

		//! 设置单元格是否有子控件
		virtual void HaveSubCtrl(int nItem, int nSubItem, BOOL bHaveSubCtrl) = 0;
		
		/*  开启或禁用某个子控件的事件响应区域
		 *	lSubControlDisable : 0x00 全开; 0x01 左spin开; 0x10 右spin开; 0x11 全禁
		 *  对于button类型的非零则为禁用
		 */ 
		virtual void SetSubCtrlDisable(
			int nItem, 
			int nSubItem, 
			LONG lSubControlDisable) = 0;

		// 用于设置cell control bitmap
		virtual void SetBitmapbyAreaID(
			UINT nAreaID,
			HBITMAP hBitmap,
			HBITMAP hBitmap2,
			HBITMAP hBitmap3,
			DWORD dwBitmapStates,
			COLORREF crMark, 
			BOOL bRedraw) = 0;

		/**
		 *  @brief 设置某列的字对齐方式
		 *  @param nIndex 列索引号
		 *  @param nTextFormat 字绘制格式
		 */		
		virtual void SetColumnTextFormat(int nIndex, UINT nTextFormat) = 0;
	};
};

#endif

// InterfaceName:TWinList
// SubFolderName:uilib
