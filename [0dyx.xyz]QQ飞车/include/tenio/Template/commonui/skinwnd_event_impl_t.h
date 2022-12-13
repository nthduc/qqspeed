#ifndef __SKINWND_EVENTIMPL_T_H__
#define __SKINWND_EVENTIMPL_T_H__

#include "eventimpl_t.h"

namespace PartialTemplateSpecializationForEvent
{
	template<>
		class IDefaultEventImpl<Tenio::ISkinWndEvent> : public Tenio::ISkinWndEvent
	{
	protected:
		virtual void OnWndClose     (ISkinWnd* /*pSkinWnd*/, BOOL* /*pbClose*/) {}
		virtual void OnWndSize      (ISkinWnd* /*pSkinWnd*/) {}
		virtual void OnWndSetFocus  (ISkinWnd* /*pSkinWnd*/) {}
		virtual void OnWndKillFocus (ISkinWnd* /*pSkinWnd*/) {}
		virtual void OnWndMouseWheel(ISkinWnd* /*pSkinWnd*/, UINT /*nFlags*/, short /*zDelta*/, POINT /*pt*/) {}
		virtual void OnLButtonDown	(ISkinWnd* /*pSkinWnd*/, UINT /*nFlags*/, POINT /*point*/) {}
		virtual void OnLButtonUp	(ISkinWnd* /*pSkinWnd*/, UINT /*nFlags*/, POINT /*point*/) {}
		virtual void OnLButtonDblClk(ISkinWnd* /*pSkinWnd*/, UINT /*nFlags*/, POINT /*point*/) {}
		virtual void OnRButtonDown	(ISkinWnd* /*pSkinWnd*/, UINT /*nFlags*/, POINT /*point*/) {}
		virtual void OnRButtonUp	(ISkinWnd* /*pSkinWnd*/, UINT /*nFlags*/, POINT /*point*/) {}
		virtual void OnMouseMove	(ISkinWnd* /*pSkinWnd*/, UINT /*nFlags*/, POINT /*point*/) {}
		virtual void OnMouseLeave	(ISkinWnd* /*pSkinWnd*/) {}
		virtual void OnSetCursor	(ISkinWnd* /*pSkinWnd*/, HWND /*hWnd*/, UINT /*nHitTest*/, UINT /*nMessage*/) {}
	};
};

#endif
