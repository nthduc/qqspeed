#ifndef __THEADER_I_H__
#define __THEADER_I_H__

#include "../teniointerface.h"
#include "control_i.h"
#include "layer/icontextlayer_i.h"
#include "layer/textdefine.h"

namespace Tenio
{
    #define II_TEXT                  0x00000001
    #define II_ICONADDRESS           0x00000002
    #define II_INTERVAL              0x00000004
    #define II_ITEMLEN               0x00000008
    #define II_ALIGN                 0x00000010
    #define II_TYPE                  0x00000020
    #define II_POS                   0x00000040
    #define II_ALL                   II_TEXT|II_ICONADDRESS|II_INTERVAL|II_ITEMLEN|II_ALIGN|II_TYPE|II_POS
	typedef struct _tagITEMINFO
	{
//	public:
//		CItemInfo()
//		{
//			m_nMask = CII_ALL;
//			m_lpszText = NULL;
//			m_lpszIconAddress = NULL;
//			m_nInterval = 0;
//			m_uItemLength = 0;
//			m_eAlign = EALIGNM_LEFT;
//			m_eIconTextType = ITT_ICONTEXT;
//			m_nPos = 0;
//
//		}
//
//		CItemInfo(int nMask, LPCTSTR lpszText, LPCTSTR lpszIconAddress, int nInterval, UINT uItemLength,
//			ENMALIGNMODE eAlign, ENUMICONTEXTTYPE eIconTextType, int nPos)
//		{
//			m_nMask = nMask;
//			m_lpszText = new TCHAR[_tcslen(lpszText)];
//			_tcscpy(m_lpszText, lpszText);
//			m_lpszIconAddress = new TCHAR[_tcslen(lpszIconAddress)];
//			_tcscpy(m_lpszIconAddress, lpszIconAddress);
//			m_nInterval = nInterval;
//			m_uItemLength = uItemLength;
//			m_eAlign = eAlign;
//			m_eIconTextType = eIconTextType;
//			m_nPos = nPos;
//		}
//
//		CItemInfo(CItemInfo& cItemInfo)
//		{
//			CItemInfo(cItemInfo.m_nMask, cItemInfo.m_lpszText, cItemInfo.m_lpszIconAddress, 
//			    cItemInfo.m_nInterval, cItemInfo.m_uItemLength,
//				cItemInfo.m_eAlign, cItemInfo.m_eIconTextType, cItemInfo.m_nPos);
//		}
//
//		virtual ~CItemInfo()
//		{
//			delete[] m_lpszText;
//			delete[] m_lpszIconAddress;
//		}
//
//	private:
		int m_nMask;
		LPTSTR m_lpszText;
		LPTSTR m_lpszIconAddress;
		int m_nInterval;
		int m_uItemLength;
		ENMALIGNMODE m_eAlign;
		ENUMICONTEXTTYPE m_eIconTextType;
		int m_nPos;

	}ITEMINFO;

	typedef enum _tagHEDAEROBJECTTYPE
	{
		ENUM_HOT_BUTTON,
		ENUM_HOT_DIVIDER
	}HEDAEROBJECTTYPE;
	
	class ITHeader;

	TENIO_MAKE_TYPEID_MAP(ITHeader, COMPONENTID_THEADER);

	class ITHeader : public IControl
	{
	public:
		virtual int GetItemCount() = 0;
		virtual BOOL SetItem(ITEMINFO& ItemInfo, int nOrder) = 0;
		virtual int InsertItem(ITEMINFO& ItemInfo, int nOrder) = 0;
		virtual BOOL DeleteItem(int nOrder) = 0;
		virtual int AppendItem(ITEMINFO& ItemInfo) = 0;
		virtual int OrderToIndex(int nOrder) = 0; 
		virtual int IndexToOrder(int nIndex) = 0;
		virtual int GetItemWidth(int nOrder) = 0;
		virtual int GetItemStartPos(int nOrder) = 0;
		virtual void HoriMoveWindowTo(int x) = 0;
	};
};

#endif

// InterfaceName:THeader
// SubFolderName:uilib
