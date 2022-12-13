#ifndef __CONTROLFACTORY_I_H__
#define __CONTROLFACTORY_I_H__

#include "control_i.h"
#include "variableparam_i.h"

namespace Tenio
{
	class IControlFactory;
	TENIO_MAKE_TYPEID_MAP(IControlFactory, COMPONENTID_CONTROLFACTORY);
	class IControlFactory : public IComponent
	{
	public:
		virtual IControl* Create(IVariableParam* pParam) = 0;
		virtual void ReleaseControl(IControl* pControl) = 0;
	};
}

#endif