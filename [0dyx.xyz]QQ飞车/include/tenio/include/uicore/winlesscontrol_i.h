#ifndef __WINLESSCONTROL_I_H__
#define __WINLESSCONTROL_I_H__

#include "../teniointerface.h"
#include "../uilib/control_i.h"
#include "../uilib/variableparam_i.h"

namespace Tenio
{
	class IWinlessControl;
	TENIO_MAKE_TYPEID_MAP(IWinlessControl, COMPONENTID_WINLESSCONTROL);
	//////////////////////////////////////////////////////////////////////////
	class IWinlessControl : public IComponent
	{
	public:
		virtual BOOL SetParent(IControl* pParent) = 0;
		virtual void Move(RECT& rc) = 0;
		virtual void SetControlID(DWORD dwID) = 0;
		virtual void SetRealParentCapture() = 0;
		virtual void SetRealParentFocus() = 0;
		virtual void ReleaseRealParentCapture() = 0;
		virtual void SetProperty(IVariableParam* pParam) = 0;
	};
}
#endif