#ifndef __TEDIT_I_H__
#define __TEDIT_I_H__

#include "control_i.h"

namespace Tenio
{
	class ITEdit;

	TENIO_MAKE_TYPEID_MAP(ITEdit, COMPONENTID_TEDIT);

	class ITEdit : public IControl
	{
	public:
		virtual LPCTSTR GetText() = 0;
		virtual void SetText(LPCTSTR pText) = 0;
	};
}; // Tenio

#endif // __TEDIT_I_H__

// InterfaceName:ITEdit
// SubFolderName:UILib
