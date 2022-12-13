#ifndef __DRAWMASTER_I_H__
#define __DRAWMASTER_I_H__

#include "teniointerface.h"

#include "uiobject_struct.h"

#include "uienginecore/canvas_i.h"
#include "uienginecore/image_i.h"
#include "layer/scene_i.h"
#include "November/engineinclude/display.h"

namespace Tenio
{
	class IDrawMaster;
	class IDrawFun;

	TENIO_MAKE_TYPEID_MAP(IDrawMaster, COMPONENTID_DRAWMASTER);

	class IDrawMaster : public IComponent
	{
	public:
		virtual void SetZModified(BOOL bModified) = 0;
		virtual void Draw(IJuneCanvas* pCanvas, IJuneScene* pScene) = 0;

		virtual void AttachDisplay(IDisplay* pDisplay) = 0;
		virtual IDisplay* GetDisplay() = 0;

		virtual LPIMAGEUIOBJECT CreateImage(IImage* image, int nFrame) = 0;
	};
};

#endif

// InterfaceName:DrawMaster
// SubFolderName:DrawMaster
