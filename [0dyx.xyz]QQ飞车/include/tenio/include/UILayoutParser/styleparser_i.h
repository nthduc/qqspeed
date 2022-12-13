#ifndef __STYLEPARSER_I_H__
#define __STYLEPARSER_I_H__

#include "../teniointerface.h"

namespace Tenio
{
	class IStyleParser;

	TENIO_MAKE_TYPEID_MAP(IStyleParser, COMPONENTID_STYLEPARSER);

	class IStyleParser : public IComponent
	{
	public:
		virtual BOOL LoadXMLFile(LPCTSTR lpszXMLFile) = 0;
		virtual BOOL SaveToXMLFile(LPCTSTR lpszXMLFile) = 0;
		virtual BOOL NewStyleXMLFile() = 0;
		virtual BOOL GotoXMLPath(LPCTSTR lpszXMLPath) = 0;
		
		virtual BOOL GetFirstStyleName(LPTSTR lpszStyleName, UINT nMaxLen) = 0;
		virtual BOOL GetNextStyleName(LPTSTR lpszStyleName, UINT nMaxLen) = 0;
		virtual BOOL AddNewStyle(LPCTSTR lpszStyleName) = 0;
		virtual BOOL DeleteStyle(LPCTSTR lpszStyleName) = 0;
		virtual BOOL GetInStyle(LPCTSTR lpszStyleName) = 0;
		virtual BOOL GetItem(LPTSTR lpszItemName, UINT nMaxLen) = 0;
		virtual BOOL GetOutStyle() = 0;
		
		virtual BOOL GetInItem(LPCTSTR lpszClassName) = 0;
		virtual BOOL GetProperty(			
			LPTSTR lpszPropertyName, 
			UINT nMaxNameLen,
			LPTSTR lpszPropertyValue,
			UINT nMaxValueLen) = 0;
		virtual BOOL AddProeprty(LPCTSTR lpszPropertyName, LPCTSTR lpszPropertyValue) = 0;
		virtual BOOL GetOutItem() = 0;

		virtual BOOL DeleteItem(LPCTSTR lpszClassName) = 0;
		virtual BOOL AddItem(LPCTSTR lpszClassName) = 0;

	};
};

#endif

// InterfaceName:StyleParser
// SubFolderName:UILayoutParser
