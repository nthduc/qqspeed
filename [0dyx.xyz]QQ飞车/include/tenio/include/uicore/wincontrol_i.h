#ifndef __WINCONTROL_I_H__
#define __WINCONTROL_I_H__

#include "../teniointerface.h"
#include "../uilib/control_i.h"
#include "../layer/scene_i.h"
#include "../layer/sceneupdate_i.h"

namespace Tenio
{
	class IWinControl;
	TENIO_MAKE_TYPEID_MAP(IWinControl, COMPONENTID_WINCONTROL);
	class IWinControl : public IComponent
	{
	public:
		virtual void SetProperty(IVariableParam* pParam) = 0;
		virtual BOOL CreateWin(DWORD dwStyle, DWORD dwStyleEx, IControl* pParent) = 0;
		virtual void SetParent(IControl* pParent) = 0;
		virtual void MoveWin(RECT& rc) = 0;
		virtual void ShowWin(int nShowCode) = 0;
		virtual void EnableWin(BOOL bEnable) = 0;
		virtual void Redraw(RECT* lpRect, BOOL bEraseBg) = 0;
		virtual HWND GetHWnd() = 0;
		virtual IJuneScene* GetSelfScene() = 0;
		virtual IJuneSceneUpdate* GetSelfSceneUpdate() = 0;
		virtual void SetControlID(DWORD dwID) = 0;

		virtual void SetFocus() = 0;
		virtual void KillFocus() = 0;
		virtual void SetCapture() = 0;
		virtual void ReleaseCapture() = 0;
	};
}
#endif