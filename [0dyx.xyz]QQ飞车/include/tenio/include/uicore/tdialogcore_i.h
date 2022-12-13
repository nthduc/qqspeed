#ifndef __TDIALOGCORE_I_H__
#define __TDIALOGCORE_I_H__

#include "../teniointerface.h"
#include "controlcore_i.h"
#include "../uilib/tdialog_i.h"
namespace Tenio
{
	class ITDialogCore;
	TENIO_MAKE_TYPEID_MAP(ITDialogCore, COMPONENTID_TDIALOGCORE);
	class ITDialogCore : 
		public IControlCore,
		public ITDialog
	{
	public:
	};
}

#endif