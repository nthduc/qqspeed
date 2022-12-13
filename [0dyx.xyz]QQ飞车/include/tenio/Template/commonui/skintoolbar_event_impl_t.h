#ifndef __SKINTOOLBAR_EVENTIMPL_T_H__
#define __SKINTOOLBAR_EVENTIMPL_T_H__

#include "eventimpl_t.h"

namespace PartialTemplateSpecializationForEvent
{
	template<>
		class IDefaultEventImpl<Tenio::ISkinToolBarEvent> : public Tenio::ISkinToolBarEvent
	{
	protected:
		virtual void OnClickToolBarItem      (UINT /*nCtrlID*/, UINT /*nItemID*/) {}
		virtual void OnClickToolBarItemExpand(UINT /*nCtrlID*/, UINT /*nItemID*/) {}
	};
};

#endif
