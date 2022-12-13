#ifndef __TRICHEDIT_I_H__
#define __TRICHEDIT_I_H__

#include "control_i.h"

namespace Tenio
{
	class ITRichEdit;

	TENIO_MAKE_TYPEID_MAP(ITRichEdit, COMPONENTID_TRICHEDIT);

	class ITRichEdit : public IControl
	{
	public:
		virtual void OffSetLayers(const POINT& pt) = 0;
		virtual BOOL PushBackHtmlString(LPCTSTR pHtml) = 0;
	};
};

#endif
