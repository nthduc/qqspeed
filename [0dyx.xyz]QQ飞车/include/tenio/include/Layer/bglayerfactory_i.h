#ifndef __BGLAYERFACTORY_I_H__
#define __BGLAYERFACTORY_I_H__

#include "../teniointerface.h"
#include "bglayer_i.h"
#include "../uilib/variableparam_i.h"

namespace Tenio
{
	class IBGLayerFactory;
	TENIO_MAKE_TYPEID_MAP(IBGLayerFactory, COMPONENTID_BGLAYERFACTORY);
	//////////////////////////////////////////////////////////////////////////
	class IBGLayerFactory : public IComponent
	{
	public:
		virtual IBGLayer* CreateBGLayer(IVariableParam* pParam) = 0;
		virtual void ReleaseBGLayer(IBGLayer* pBGLayer) = 0;
	};
}
#endif