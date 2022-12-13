// XMLRegistry.h: interface for the CXMLRegistry class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XMLREGISTRY_H__5A62BF9C_ADF8_4885_BF68_D53498E05F04__INCLUDED_)
#define AFX_XMLREGISTRY_H__5A62BF9C_ADF8_4885_BF68_D53498E05F04__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "vector"
using namespace std;

#define KEY_MAX_OPEN	10

class CXMLRegistry  
{
public:
	CXMLRegistry(BOOL bAutoSave = FALSE);
	virtual ~CXMLRegistry();

	LONG Open(LPCSTR lpszRegistryFile);
	LONG Save(LPCSTR lpszFileName);

	LONG RegOpenKey(
		HKEY hKey,        // handle to open key
		LPCTSTR lpSubKey, // name of subkey to open
		PHKEY phkResult   // handle to open key
		);

	LONG RegCloseKey(
		HKEY hKey   // handle to key to close
		);

	LONG RegQueryValue(
		HKEY hKey,        // handle to key to query
		LPCTSTR lpSubKey, // subkey name
		LPTSTR lpValue,   // string buffer
		PLONG lpcbValue   // size of returned string
		);

	LONG RegCreateKey(
		HKEY hKey,        // handle to an open key
		LPCTSTR lpSubKey, // subkey name
		PHKEY phkResult   // buffer for key handle
		);

	LONG RegSetValue(
		HKEY hKey,         // handle to key
		LPCTSTR lpSubKey,  // subkey name
		DWORD dwType,      // information type
		LPCTSTR lpData,    // value data
		DWORD cbData       // size of value data
		);

	LONG RegDeleteKey(
		HKEY hKey,         // handle to open key
		LPCTSTR lpSubKey   // subkey name
		);

	LONG RegisterServer(LPCSTR lpszProgID, LPCSTR lpszCLSID, LPCSTR lpszModulePath);
	LONG UnRegisterServer(LPCSTR lpszCLSID);

protected:
	IXMLDOMNode* FindChildNode(IXMLDOMNode*, LPCSTR lpszChildName);
	IXMLDOMNode* KeyToNode(HKEY);
	void Reset();
	void EnsureDocument();

protected:
	IXMLDOMDocument*	m_pXMLDoc;
	char				m_szRegistryFile[MAX_PATH];
	BOOL				m_bAutoSave;
};

#endif // !defined(AFX_XMLREGISTRY_H__5A62BF9C_ADF8_4885_BF68_D53498E05F04__INCLUDED_)
