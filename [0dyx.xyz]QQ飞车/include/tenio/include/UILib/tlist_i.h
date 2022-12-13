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
		// ��ģʽ��ʹ��
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
		
		//! ���õ�Ԫ���ӿؼ����ͺ�ID,ÿһ�еĿؼ�type��ID��ͬ��ID��Ӧ�ö��壬������ͼʱʹ�á�
		virtual void SetSubItemCtrl(int nColumnIndex, int nCtrlType, UINT nCtrlID) = 0;

		//! ���õ�Ԫ���Ƿ����ӿؼ�
		virtual void HaveSubCtrl(int nItem, int nSubItem, BOOL bHaveSubCtrl) = 0;
		
		/*  ���������ĳ���ӿؼ����¼���Ӧ����
		 *	lSubControlDisable : 0x00 ȫ��; 0x01 ��spin��; 0x10 ��spin��; 0x11 ȫ��
		 *  ����button���͵ķ�����Ϊ����
		 */ 
		virtual void SetSubCtrlDisable(
			int nItem, 
			int nSubItem, 
			LONG lSubControlDisable) = 0;

		// ��������cell control bitmap
		virtual void SetBitmapbyAreaID(
			UINT nAreaID,
			HBITMAP hBitmap,
			HBITMAP hBitmap2,
			HBITMAP hBitmap3,
			DWORD dwBitmapStates,
			COLORREF crMark, 
			BOOL bRedraw) = 0;

		/**
		 *  @brief ����ĳ�е��ֶ��뷽ʽ
		 *  @param nIndex ��������
		 *  @param nTextFormat �ֻ��Ƹ�ʽ
		 */		
		virtual void SetColumnTextFormat(int nIndex, UINT nTextFormat) = 0;
	};
};

#endif

// InterfaceName:TWinList
// SubFolderName:uilib
