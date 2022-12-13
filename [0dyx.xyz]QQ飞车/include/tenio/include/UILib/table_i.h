#ifndef __TABLE_I_H__
#define __TABLE_I_H__

#include "../teniointerface.h"
#include "control_i.h"
#include "layer/textdefine.h"
#include "layer/icontextlayer_i.h"

namespace Tenio
{
	class ITable;

	TENIO_MAKE_TYPEID_MAP(ITable, COMPONENTID_TABLE);

	typedef enum _tagTABLECOLUMNMODE
	{
		CM_ICONTEXT,
		CM_CUSTOM
	}TABLECOLUMNMODE;

	typedef struct _tagTABLECOLUMNINFO
	{
		TABLECOLUMNMODE eColumnMode;
		int nInterval;
		ENMALIGNMODE eAlign;
		ENUMICONTEXTTYPE eIconTextType;
		int nItemHeight;
		int nColumnWidth;
		TCHAR szLayoutAddress[MAX_PATH];
	}TABLECOLUMNINFO;

	class ITable : public IControl
	{
	public:
		virtual void InsertRow(int nOrder, BOOL bRedraw) = 0;
		virtual void DeleteRow(int nOrder, BOOL bRedraw) = 0;
		virtual void AppendRow(BOOL bRedraw) = 0;
		virtual void InsertColumn(int nOrder, TABLECOLUMNINFO& ColumnInfo, BOOL bRedraw) = 0;
		virtual void DeleteColumn(int nOrder, BOOL bRedraw) = 0;
		virtual void AppendColumn(TABLECOLUMNINFO& ColumnInfo, BOOL bRedraw) = 0;
		virtual void SetItem(LPCTSTR lpszText, LPCTSTR lpszIconAddress, int nRow, int nColumn) = 0;
		virtual IControl* GetItemTopControlPtr(int nRow, int nColumn) = 0;
		virtual int GetRowCount() = 0;
		virtual int GetColumnCount() = 0;
		virtual void HoriMoveWindowTo(int x) = 0;
		virtual void VertMoveWindowTo(int y) = 0;
		virtual POINT GetFirstSelectedItemPosition() = 0;
		virtual POINT GetNextSelectedItem() = 0;
		virtual int GetItemMaxWidthInColumn(int nColumn) = 0;
		virtual void SetColumnSelected(int nColumn) = 0;
		virtual void SetColumnUnselected(int nColumn) = 0;
		virtual void MoveDotLineTo(int x) = 0;
		virtual void GetColumnInfo(TABLECOLUMNINFO* pColumnInfo, int nOrder) = 0;
		virtual void ResetColumnWidth(int nColumnWidth, int nOrder, BOOL bRedraw) = 0;
		virtual void GetDocument(int* pHeight, int* pWidth) = 0;
		
	};
};

#endif

// InterfaceName:Table
// SubFolderName:uilib
