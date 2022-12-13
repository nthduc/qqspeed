#ifndef __BORDERLAYER_I_H__
#define __BORDERLAYER_I_H__

#include "../teniointerface.h"
#include "layer_i.h"
#include "../uilib/variableparam_i.h"
#include "../June/text_i.h"

namespace Tenio
{
	class IBorderLayer;
	TENIO_MAKE_TYPEID_MAP(IBorderLayer, COMPONENTID_BORDERLAYER);

	class IBorderLayer : public IJuneLayer, public IComponent
	{
	public:
		virtual BOOL SetProperty(IVariableParam* pParam) = 0;
	};
};

#endif

// InterfaceName:BorderLayer
// SubFolderName:Layer
