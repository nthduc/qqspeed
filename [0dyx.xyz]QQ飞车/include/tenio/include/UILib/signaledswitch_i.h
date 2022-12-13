#ifndef __SIGNALEDSWITCH_I_H__
#define __SIGNALEDSWITCH_I_H__

#include "../teniointerface.h"
#include "uilib/control_i.h"

namespace Tenio
{
	class ISignaledSwitch;

	TENIO_MAKE_TYPEID_MAP(ISignaledSwitch, COMPONENTID_SIGNALEDSWITCH);

	class ISignaledSwitch : public IControl
	{
	public:
	};
};

#endif

// InterfaceName:SignaledSwitch
// SubFolderName:UILib
