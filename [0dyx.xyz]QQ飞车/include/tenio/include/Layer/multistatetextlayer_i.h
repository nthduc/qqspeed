#ifndef __MULTISTATETEXTLAYER_I_H__
#define __MULTISTATETEXTLAYER_I_H__

#include "../teniointerface.h"
#include "layer_i.h"
#include "multistate_i.h"
#include "uilib/variableparam_i.h"

namespace Tenio
{
	class IMultiStateTextLayer;

	TENIO_MAKE_TYPEID_MAP(IMultiStateTextLayer, COMPONENTID_MULTISTATETEXTLAYER);

	class IMultiStateTextLayer : 
		public IJuneLayer, 
		public IComponent,
		public IMultiState
	{
	public:
		virtual BOOL SetProperty(IVariableParam* pParam) = 0;
	};
};

#endif

// InterfaceName:MultiStateTextLayer
// SubFolderName:layer
