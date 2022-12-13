#ifndef __SKINLIST_EVENTIMPL_T_H__
#define __SKINLIST_EVENTIMPL_T_H__

#include "../eventimpl_t.h"

namespace PartialTemplateSpecializationForEvent
{
	template<>
		class IDefaultEventImpl<Tenio::ISkinListEvent> : public Tenio::ISkinListEvent
	{
	protected:
		virtual void OnListClickItem  (UINT /*nID*/, int /*nRowIndex*/, int /*nSubItemIndex*/) {}
		virtual void OnListDClkItem   (UINT /*nID*/, int /*nRowIndex*/, int /*nSubItemIndex*/) {}
		virtual void OnListRClickItem (UINT /*nID*/, int /*nRowIndex*/, int /*nSubItemIndex*/) {}
		virtual void OnListClickColumn(UINT /*nID*/, int /*nColumnIndex*/) {}
		virtual void OnListItemCtrlEvent(
			UINT /*nID*/, 
			int /*nRowIndex*/, 
			int /*nSubItemIndex*/, 
			UINT /*nEventID*/,
			UINT /*nEventAreaID*/) {}
		virtual void OnListSetFocus (UINT /*nID*/) {}
		virtual void OnListKillFocus(UINT /*nID*/) {}
		virtual void OnListGetDisplayInfo(UINT /*nID*/, Tenio::LISTITEMINFO* /*pListItemInfo*/) {}
	};
};

#endif
