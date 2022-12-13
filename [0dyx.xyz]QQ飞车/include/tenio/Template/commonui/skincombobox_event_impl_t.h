#ifndef __SKINCOMBOBOX_EVENTIMPL_T_H__
#define __SKINCOMBOBOX_EVENTIMPL_T_H__

#include "../eventimpl_t.h"

namespace PartialTemplateSpecializationForEvent
{
	template<>
		class IDefaultEventImpl<Tenio::ISkinComboBoxEvent> : public Tenio::ISkinComboBoxEvent
	{
	protected:
		virtual void OnComboBoxSelectItem(UINT /*nCtrlID*/, int /*nIndex*/) {}
		virtual void OnComboBoxTextChange(UINT /*nCtrlID*/)		{};	
	};
};

#endif
