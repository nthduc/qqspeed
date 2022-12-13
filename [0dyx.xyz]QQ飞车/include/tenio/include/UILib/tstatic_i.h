#ifndef __TSTATIC_I_H__
#define __TSTATIC_I_H__

#include "control_i.h"

namespace Tenio
{
	class ITStatic;
	TENIO_MAKE_TYPEID_MAP(ITStatic, COMPONENTID_TSTATIC);

	class ITStatic : public IControl
	{
	public:
		virtual void SetText(LPCTSTR lpszText) = 0;
		virtual LPCTSTR GetText() = 0;
	};
}; // Tenio

#endif // __TSTATIC_I_H__

// InterfaceName:TStatic
// SubFolderName:UILib
