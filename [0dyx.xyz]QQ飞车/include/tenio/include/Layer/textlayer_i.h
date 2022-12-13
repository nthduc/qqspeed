#ifndef __TEXTLAYER_I_H__
#define __TEXTLAYER_I_H__

#include "teniointerface.h"
#include "layer_i.h"
#include "../uilib/variableparam_i.h"
#include "../June/text_i.h"

namespace Tenio
{
	class ITextLayer;
	TENIO_MAKE_TYPEID_MAP(ITextLayer, COMPONENTID_TEXTLAYER);
	
	class ITextLayer : public IJuneLayer, public IComponent
	{
	public:
		virtual BOOL SetProperty(IVariableParam* pParam) = 0;
		virtual LONG GetCharHeight() = 0;
		virtual LOCATION AdjustPosition(const POINT& ptSrc, POINT& ptDst) = 0;
		virtual LOCATION GetForwardCharPosition(const LOCATION& location, POINT& ptDst) = 0;
		virtual LOCATION GetBackwardCharPosition(const LOCATION& location, POINT& ptDst) = 0;
		virtual LOCATION GetForwardLinePosition(const LOCATION& location, POINT& ptDst) = 0;
		virtual LOCATION GetBackwardLinePosition(const LOCATION& location, POINT& ptDst) = 0;
		virtual LOCATION InsertChar(const LOCATION& location, POINT& ptDst, UINT uChar) = 0;
		virtual LOCATION DelForwardChar(const LOCATION& location, POINT& ptDst) = 0;
		virtual LOCATION DelBackwardChar(const LOCATION& location, POINT& ptDst) = 0;
		virtual LPCTSTR GetText() = 0;
		virtual void SetSel(const LOCATION& lcStart, const LOCATION& lcEnd) = 0;
		virtual LPTSTR GetSelText(LPTSTR pText) = 0;
		virtual LOCATION DelSelText(POINT& ptDst) = 0;
	};
};

#endif