#ifndef __TSLIDERCORE_I_H__
#define __TSLIDERCORE_I_H__

#include "../teniointerface.h"
#include "controlcore_i.h"
#include "../uilib/tSlider_i.h"

namespace Tenio
{
	class ITSliderCore;

	TENIO_MAKE_TYPEID_MAP(ITSliderCore, COMPONENTID_TSLIDERCORE);

	/**
	 * @ingroup tenuilib
	 * @brief uilib slider¿Ø¼þºËÐÄ
	 * @sa IControlCore , ITSlider
	 */
	class ITSliderCore : public IControlCore, public ITSlider
	{
	public:
	};
};

#endif

// InterfaceName:TSliderCore
// SubFolderName:uicore
