#ifndef __SKINEDIT_EVENTIMPL_T_H__
#define __SKINEDIT_EVENTIMPL_T_H__

#include "eventimpl_t.h"

namespace PartialTemplateSpecializationForEvent
{
	template<>
		class IDefaultEventImpl<Tenio::ISkinEditEvent> : public Tenio::ISkinEditEvent
	{
	protected:
		virtual void OnEditTextChange(UINT /*nCtrlID*/) {}
		virtual void OnEditKillFocus(UINT /*nCtrlID*/, HWND /*hWndNew*/)	{};	
		virtual void OnEditKeyDown(
			UINT /*nCtrlID*/, 
			UINT /*nChar*/, 
			UINT /*nRepCnt*/, 
			UINT /*nFlags*/) {};		
	};
};

#endif
