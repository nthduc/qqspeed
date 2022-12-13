#ifndef __SKINTAB_EVENTIMPL_T_H__
#define __SKINTAB_EVENTIMPL_T_H__

#include "event_impl_t.h"

namespace PartialTemplateSpecializationForEvent
{
	template<>
		class IDefaultEventImpl<ISkinTabEvent> : public ISkinTabEvent
	{
	protected:
		virtual void OnTabClickItem   (int /*nTabID*/, int /*nItemIndex*/) {}
		virtual void OnTabRClickItem  (int /*nTabID*/, int /*nItemIndex*/) {}
		virtual void OnTabDbClkItem   (int /*nTabID*/, int /*nItemIndex*/) {}
		virtual void OnTabActivateItem(int /*nTabID*/, int /*nItemIndex*/) {}
	};
};

#endif
