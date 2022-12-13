#ifndef __MULTISTATEBGLAYER_I_H__
#define __MULTISTATEBGLAYER_I_H__

#include "../teniointerface.h"
#include "bglayer_i.h"
#include "multistate_i.h"

namespace Tenio
{
	class IMultiStateBGLayer;

	TENIO_MAKE_TYPEID_MAP(IMultiStateBGLayer, COMPONENTID_MULTISTATEBGLAYER);

	class IMultiStateBGLayer : public IBGLayer, public IMultiState, public IComponent
	{
	public:
	};
};

#endif

// InterfaceName:MultiStateBGLayer
// SubFolderName:layer
