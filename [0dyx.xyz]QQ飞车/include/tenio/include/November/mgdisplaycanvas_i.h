#ifndef __MGDISPLAYCANVAS_I_H__
#define __MGDISPLAYCANVAS_I_H__

#include "../teniointerface.h"
#include "../june/canvas_i.h"
#include "../components/november/engine/include/display.h"

namespace Tenio
{
	class IMGDisplayCanvas;

	TENIO_MAKE_TYPEID_MAP(IMGDisplayCanvas, COMPONENTID_MGDISPLAYCANVAS);

	class IMGDisplayCanvas : public IJuneCanvas, public IComponent
	{
	public:
		virtual void AttachDisplay(IDisplay* pDisplay) = 0;
		virtual IDisplay* GetDisplay() = 0;
	};
};

#endif

// InterfaceName:MGDisplayCanvas
// SubFolderName:November
