#ifndef __ICONTEXTLAYER_I_H__
#define __ICONTEXTLAYER_I_H__

#include "../teniointerface.h"
#include "layer_i.h"
#include "../uilib/variableparam_i.h"

namespace Tenio
{
 	typedef enum _tagENUMICONTEXTTYPE
	{
		ITT_ICONTEXT,
		ITT_ICON,
		ITT_TEXT
	}ENUMICONTEXTTYPE;
	
	class IIconTextLayer;

	TENIO_MAKE_TYPEID_MAP(IIconTextLayer, COMPONENTID_ICONTEXTLAYER);

	class IIconTextLayer : public IJuneLayer, public IComponent
	{
	public:
		virtual void SetIconText(LPCTSTR lpszIconAddress, LPCTSTR lpszText,
			int nInterval, ENUMICONTEXTTYPE eType) = 0;
		virtual BOOL SetProperty(IVariableParam* pParam) = 0;
	};
};

#endif

// InterfaceName:IconTextLayer
// SubFolderName:layer
