#ifndef __GIFPLAY_EVENTIMPL_T_H__
#define __GIFPLAY_EVENTIMPL_T_H__

#include "event_impl_t.h"

namespace PartialTemplateSpecializationForEvent
{
	template<>
		class IDefaultEventImpl<IGifPlayerEvent> : public IGifPlayerEvent
	{
	protected:
		virtual void OnGifLButtonDown(UINT nID, UINT nFlags, POINT point){};
		virtual void OnGifRClick(UINT nID, POINT point){};
	};
};

#endif

