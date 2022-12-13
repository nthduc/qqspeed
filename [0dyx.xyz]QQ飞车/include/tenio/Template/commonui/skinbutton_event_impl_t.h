#ifndef __SKINBUTTON_EVENTIMPL_T_H__
#define __SKINBUTTON_EVENTIMPL_T_H__

#include "eventimpl_t.h"

namespace PartialTemplateSpecializationForEvent
{
	template<>
		class IDefaultEventImpl<Tenio::ISkinButtonEvent> : public Tenio::ISkinButtonEvent
	{
	protected:
		virtual void OnBtnClick     (UINT /*nID*/) {}
		virtual void OnBtnMouseHover(UINT /*nID*/) {}
		virtual void OnBtnMouseLeave(UINT /*nID*/) {}
		virtual void OnLButtonDown  (UINT /*nID*/, UINT /*nFlags*/, POINT /*point*/){}
		virtual void OnLButtonUp    (UINT /*nID*/, UINT /*nFlags*/, POINT /*point*/){}
	};
};

#endif
