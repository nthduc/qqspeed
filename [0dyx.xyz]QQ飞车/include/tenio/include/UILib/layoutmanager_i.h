#ifndef __LAYOUTMANAGER_I_H__
#define __LAYOUTMANAGER_I_H__

#include "../teniointerface.h"
#include "boundary/uiboundarymanager_i.h"
#include "../uicore/controlsmanager_i.h"
#include "uilibdefine.h"

namespace Tenio
{
	class ILayoutManager;

	TENIO_MAKE_TYPEID_MAP(ILayoutManager, COMPONENTID_LAYOUTMANAGER);

	class ILayoutManager : public IComponent
	{
	public:
		virtual void SetUILibPtr(
			IUIBoundaryManager* pBoundaryManager,
			IControlsManager* pControlsManager) = 0;
		virtual void OnControlSize(DWORD dwControl) = 0;
		virtual void OnControlShow(DWORD dwControl) = 0;
		virtual void SetLayout(DWORD dwControl, ENMLAYOUTMODE eLayoutMode) = 0;
		virtual ENMLAYOUTMODE GetLayout(DWORD dwControl) = 0;
		virtual void ReArrange(DWORD dwControl) = 0;//当控件size发生变化后重排控件内部的子孙控件
		virtual void SetInnerMargins(DWORD dwControl, int nLeft, int nTop, int nRight, int nBottom) = 0;
		virtual void SetOuterMargins(DWORD dwControl, int nLeft, int nTop, int nRight, int nBottom) = 0;
		virtual void ClearAll() = 0;
		virtual void DeleteControl(DWORD dwControl) = 0;
		
	};
};

#endif

// InterfaceName:LayoutManager
// SubFolderName:UILib
