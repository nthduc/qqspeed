#ifndef __TMULTIMIFIMAGEBOX_I_H__
#define __TMULTIMIFIMAGEBOX_I_H__

#include "control_i.h"

namespace Tenio
{
	class ITMultiMifImageBox;

	TENIO_MAKE_TYPEID_MAP(ITMultiMifImageBox, COMPONENTID_TMULTIMIFIMAGEBOX);

	class ITMultiMifImageBox : public IControl
	{
	public:
		virtual void StartPlay(BOOL bReplay = FALSE) = 0;
		
		virtual void AddMifImageLayer(DWORD dwRefLayer, LPCTSTR lpszFileName) = 0;
		virtual void RemoveMifImageLayer(DWORD dwRefLayer) = 0;
		virtual void RemoveAllLayer() = 0;
	};
};

#endif

// InterfaceName:TMultiMifImageBox
// SubFolderName:uilib
