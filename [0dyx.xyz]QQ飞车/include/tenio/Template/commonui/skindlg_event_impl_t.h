#ifndef __SKINDLG_EVENTIMPL_T_H__
#define __SKINDLG_EVENTIMPL_T_H__

#include "eventimpl_t.h"

namespace PartialTemplateSpecializationForEvent
{
	template<>
		class IDefaultEventImpl<Tenio::ISkinDlgEvent> : public Tenio::ISkinDlgEvent
	{
	protected:
		virtual void OnInitDialog(Tenio::ISkinDlg* /*pSkinDlg*/) {}
		virtual void OnPaint	 (Tenio::ISkinDlg* /*pSkinDlg*/, HDC   /*hdc*/)	   {}
		virtual void OnClose     (Tenio::ISkinDlg* /*pSkinDlg*/, BOOL* /*pbClose*/)   {}
		virtual void OnEndDialog (Tenio::ISkinDlg* /*pSkinDlg*/, int   /*nResultID*/) {}
		virtual void OnTimer     (Tenio::ISkinDlg* /*pSkinDlg*/, UINT  /*nID*/)       {}
		virtual void OnLButtonDown(Tenio::ISkinDlg* /*pSkinDlg*/, UINT /*nFlags*/, POINT /*point*/) {};
	};
};
#endif
