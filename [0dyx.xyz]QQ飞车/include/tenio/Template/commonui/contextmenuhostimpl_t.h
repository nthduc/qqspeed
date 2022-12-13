#ifndef __CONTEXTMENUHOSTIMPL_T_H__
#define __CONTEXTMENUHOSTIMPL_T_H__

#include "CommonUI/contextmenu_i.h"
#include "eventimpl_t.h"

namespace Tenio
{

template<
	class ConcreateClass,
	int   nEventVersion = 1
	>
class ContextMenuHostImpl : 
	public ConcreateClass,
	public IEventImpl<IContextMenuControllerEvent, nEventVersion>
{
public:
	ContextMenuHostImpl()
	{
		m_pContextMenuSupport = NULL;
	}

	void SetContextMenuSupprot(IContextMenuSupport* pContextMenuSupport)
	{
		m_pContextMenuSupport = pContextMenuSupport;
	}

	// IContextMenuControllerEvent
	void OnContextMenuCommand(DWORD dwCommandID)
	{
		if(m_pContextMenuSupport != NULL)
		{
			LPVOID lpParam = GetContextMenuParam();
			m_pContextMenuSupport->OnContextMenuCommand(dwCommandID, lpParam);
		}
	}

protected:

	void TriggerContextMenu(int x, int y, LPVOID lpParam)
	{
		if(m_pContextMenuSupport != NULL)
		{
			IContextMenuController* pContextMenuController = NULL;
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
			int nStandMenuMode = m_pContextMenuSupport->OnContextMenu(
				lpParam, 
				&pContextMenuController);

			if(pContextMenuController != NULL)
			{
				InsertStandMenu(nStandMenuMode, pContextMenuController);
				
				int nMenuItemCount = pContextMenuController->GetItemsCount() > 0;
				
				if(pContextMenuController != NULL && nMenuItemCount > 0)
				{
					pContextMenuController->SetEventHandler(this);
					pContextMenuController->TrackPopUp(x, y);
				}
			}
		}
	}

	virtual LPVOID GetContextMenuParam() = 0;

	virtual void InsertStandMenu(
		int nMode, 
		IContextMenuController* pContextMenuController) = 0;

protected:
	IContextMenuSupport* m_pContextMenuSupport;
};
}
#endif