#ifndef __NAME2IDPARSER_I_H__
#define __NAME2IDPARSER_I_H__

#include "../teniointerface.h"

namespace Tenio
{
	class IName2IDParser;

	TENIO_MAKE_TYPEID_MAP(IName2IDParser, COMPONENTID_NAME2IDPARSER);

	class IName2IDParser : public IComponent
	{
	public:
		virtual BOOL LoadFile(LPCTSTR lpszFileName) = 0;
		virtual BOOL AddStart() = 0;
		virtual BOOL AddEnd() = 0;
		virtual BOOL AddOneName(LPCTSTR lpszClassName, DWORD dwComponentID) = 0;
		virtual DWORD GetID(LPCTSTR lpszClassName) = 0;
		virtual BOOL SaveToFile(LPCTSTR lpszFileName) = 0;
		virtual int  GetClassCount() = 0;		
	};
};

#endif

// InterfaceName:Name2IDParser
// SubFolderName:UILayoutParser
