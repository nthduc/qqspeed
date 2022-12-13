#ifndef __SKINTREE_EVENTIMPL_T_H__
#define __SKINTREE_EVENTIMPL_T_H__

#include "../eventimpl_t.h"

namespace PartialTemplateSpecializationForEvent
{
	template<>
		class IDefaultEventImpl<ISkinTreeEvent> : public ISkinTreeEvent
	{
	protected:
		virtual void OnTreeClickItem(UINT nID, DWORD dwItemID){};
		virtual void OnTreeDbClickItem(UINT nID, DWORD dwItemID){};
		virtual void OnTreeRClickItem(UINT nID, DWORD dwItemID){};
		virtual void OnTreeExpandItem(UINT nID, DWORD dwItemID) {};
		virtual void OnTreeRClickBlankArea(UINT nID, POINT point){};
		virtual void OnTreeMouseMove(UINT nID,  POINT point, DWORD dwItemID, UINT nFlag){};
	};
};

#endif
