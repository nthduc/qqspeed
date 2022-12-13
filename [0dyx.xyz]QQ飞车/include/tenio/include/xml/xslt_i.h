#ifndef __XSLT_I_H__
#define __XSLT_I_H__

#include "../teniointerface.h"

namespace Tenio
{
	class IXSLT;

	TENIO_MAKE_TYPEID_MAP(IXSLT, COMPONENTID_XSLT);

	class IXSLT : public IComponent
	{
	public:
		virtual BOOL Transform(LPCTSTR lpszSrcXMLFile, LPCTSTR lpszXSLTFile, 
			LPCTSTR lpszDestFile) = 0;
	};
};

#endif