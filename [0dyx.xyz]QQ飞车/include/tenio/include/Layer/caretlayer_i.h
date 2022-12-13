#ifndef __CARETLAYER_I_H__
#define __CARETLAYER_I_H__

#include "../teniointerface.h"
#include "layer_i.h"
#include "../uilib/variableparam_i.h"

namespace Tenio
{
	class ICaretLayer;

	TENIO_MAKE_TYPEID_MAP(ICaretLayer, COMPONENTID_CARETLAYER);

	class ICaretLayer : public IJuneLayer, public IComponent
	{
	public:
		virtual void SetProperty(LONG lHeight, UINT uBlinkTime) = 0;
		virtual BOOL SetProperty(IVariableParam* pParam) = 0;
	};
}; // Tenio

#endif // __CARETLAYER_I_H__

// InterfaceName:CaretLayer
// SubFolderName:Layer
