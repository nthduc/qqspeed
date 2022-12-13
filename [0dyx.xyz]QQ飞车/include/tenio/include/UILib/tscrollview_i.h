#ifndef __TSCROLLVIEW_I_H__
#define __TSCROLLVIEW_I_H__

#include "../teniointerface.h"
#include "control_i.h"

namespace Tenio
{
	class ITScrollView;

	TENIO_MAKE_TYPEID_MAP(ITScrollView, COMPONENTID_TSCROLLVIEW);

	class ITScrollView : public IControl
	{
	public:
		virtual void WindowOffset(int x, int y) = 0;
		virtual RECT GetClientRect() = 0;
	};
};

#endif

// InterfaceName:TScrollView
// SubFolderName:uilib
