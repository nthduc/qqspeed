// XMLRegistry.cpp: implementation of the CXMLRegistry class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XMLRegistry.h"
#include "common.h"
#include "msxml.h"
#include "MSXMLDID.H"
#include "strex.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CXMLRegistry::CXMLRegistry(BOOL bAutoSave):
m_pXMLDoc(NULL),
m_bAutoSave(bAutoSave)
{

}

CXMLRegistry::~CXMLRegistry()
{
	if ( m_bAutoSave && m_pXMLDoc )
	{
		m_pXMLDoc->save(CComVariant(m_szRegistryFile));
	}
}

void CXMLRegistry::Reset()
{
	strcpy(m_szRegistryFile, "");
	RELEASE(m_pXMLDoc);
}

LONG CXMLRegistry::Open(LPCSTR lpszRegistryFile)
{
	Reset();

	strcpy(m_szRegistryFile, lpszRegistryFile);
	HRESULT hr = E_FAIL;	
	hr = CoCreateInstance(CLSID_DOMDocument, NULL, CLSCTX_INPROC_SERVER, 
		IID_IXMLDOMDocument, (void**)&m_pXMLDoc);
	KK_CHECK(hr);

	VARIANT_BOOL bSuccessed;
	m_pXMLDoc->load(CComVariant(lpszRegistryFile), &bSuccessed);

KK_EXIT:
	return hr;
}

LONG CXMLRegistry::Save(LPCSTR lpszFileName)
{
	if ( m_pXMLDoc )
	{
		return m_pXMLDoc->save(CComVariant(lpszFileName));
	}
	return 0;
}

IXMLDOMNode* CXMLRegistry::FindChildNode(IXMLDOMNode* pNode, LPCSTR lpszChildName)
{
	if ( !pNode )
	{
		CComQIPtr<IXMLDOMNode> spRootNode= m_pXMLDoc;
		pNode = spRootNode;
	}

	char szTmp[MAX_PATH] = "";
	int c = strsplit(lpszChildName, szTmp);
	if ( c == 0 )
		return pNode;
	else if ( c > 1 )
	{
		IXMLDOMNode* pTmpNode = pNode;
		LPSTR pszTmp = szTmp;
		for ( int i = 0; i < c; i++ )
		{
			pTmpNode = FindChildNode(pTmpNode, pszTmp);
			if ( !pTmpNode )
				return NULL;
			pszTmp += (1 + strlen(pszTmp));
		}
		return pTmpNode;
	}

	CComPtr<IXMLDOMNode> spFirstChild;
	pNode->get_firstChild(&spFirstChild);

	CComPtr<IXMLDOMNode> spTmpNode = spFirstChild;
	while(spTmpNode)
	{
		CComBSTR bstrText;
		spTmpNode->get_nodeName(&bstrText);
		if ( bstrText == lpszChildName )
		{
			return spTmpNode.Detach();
		}
		spTmpNode = NULL;
		spFirstChild->get_nextSibling(&spTmpNode);
		spFirstChild = spTmpNode;
	};

	return NULL;
}

IXMLDOMNode* CXMLRegistry::KeyToNode(HKEY hKey)
{
	if ( !m_pXMLDoc )
		return NULL;

	if ( hKey == NULL )
	{
		CComQIPtr<IXMLDOMNode> spNode = m_pXMLDoc;
		return spNode;
	}
	else
	{
		return (IXMLDOMNode*)hKey;
	}
}


LONG CXMLRegistry::RegOpenKey(
				HKEY hKey,        // handle to open key
				LPCTSTR lpSubKey, // name of subkey to open
				PHKEY phkResult   // handle to open key
				)
{
	EnsureDocument();
	if ( !m_pXMLDoc )
		return ERROR_FILE_NOT_FOUND;

	IXMLDOMNode* pNode = KeyToNode(hKey);
	if ( !pNode )
		return ERROR_INVALID_HANDLE;
	IXMLDOMNode* pChildNode = FindChildNode(pNode, lpSubKey);
	if ( !pChildNode )
	{
		return ERROR_PATH_NOT_FOUND;
	}
	*phkResult = (HKEY)pChildNode;
	return ERROR_SUCCESS;
}

LONG CXMLRegistry::RegCloseKey(
				 HKEY hKey   // handle to key to close
				 )
{
	if ( !m_pXMLDoc )
		return ERROR_FILE_NOT_FOUND;

	if ( hKey )
	{
		IXMLDOMNode* pNode = (IXMLDOMNode*)hKey;
		pNode->Release();
	}
	return ERROR_SUCCESS;
}

LONG CXMLRegistry::RegQueryValue(
				   HKEY hKey,        // handle to key to query
				   LPCTSTR lpSubKey, // subkey name
				   LPTSTR lpValue,   // string buffer
				   PLONG lpcbValue   // size of returned string
				   )
{
	EnsureDocument();
	if ( !m_pXMLDoc )
		return ERROR_FILE_NOT_FOUND;

	IXMLDOMNode* pNode = KeyToNode(hKey);
	IXMLDOMNode* pChildNode = FindChildNode(pNode, lpSubKey);
	if ( !pChildNode )
		return ERROR_PATH_NOT_FOUND;

	CComBSTR bstrValue;
	pChildNode->get_text(&bstrValue);
	BOOL bUseDefault;
	int cbMutiByte = *lpcbValue;
	cbMutiByte = WideCharToMultiByte(CP_ACP, 0, bstrValue, bstrValue.Length(), lpValue, cbMutiByte, "", &bUseDefault);
	*lpcbValue = cbMutiByte;
	return ERROR_SUCCESS;
}

LONG CXMLRegistry::RegCreateKey(
				  HKEY hKey,        // handle to an open key
				  LPCTSTR lpSubKey, // subkey name
				  PHKEY phkResult   // buffer for key handle
				  )
{
	EnsureDocument();
	if ( !m_pXMLDoc )
		return ERROR_FILE_NOT_FOUND;

	IXMLDOMNode* pNode = KeyToNode(hKey);
	IXMLDOMNode* pChildNode = FindChildNode(pNode, lpSubKey);
	if ( pChildNode )
	{
		*phkResult=  (HKEY)pChildNode;
		return ERROR_FILE_EXISTS;
	}
	else
	{
		char szTmp[MAX_PATH] = "";
		int c = strsplit(lpSubKey, szTmp);
		if ( c == 0 )
			return ERROR_INVALID_HANDLE;
		else if ( c == 1 )
		{
			CComPtr<IXMLDOMElement> spNewElement;
			m_pXMLDoc->createElement(CComBSTR(szTmp), &spNewElement);

			CComQIPtr<IXMLDOMNode> spNewNode = spNewElement;
			CComQIPtr<IXMLDOMNode> spNewedNode;
			pNode->appendChild(spNewNode, &spNewedNode);
			*phkResult = (HKEY)spNewedNode.Detach();
			return ERROR_SUCCESS;
		}
		else
		{
			HKEY hKeyTmp = hKey;
			LPSTR pszTmp = szTmp;
			for ( int i = 0; i < c; i++ )
			{
				RegCreateKey(hKeyTmp, pszTmp, &hKeyTmp);
				pszTmp += (1 + strlen(pszTmp));
			}
			*phkResult = hKeyTmp;
			return ERROR_SUCCESS;
		}
	}
}

LONG CXMLRegistry::RegSetValue(
				 HKEY hKey,         // handle to key
				 LPCTSTR lpSubKey,  // subkey name
				 DWORD dwType,      // information type
				 LPCTSTR lpData,    // value data
				 DWORD cbData       // size of value data
				 )
{
	EnsureDocument();
	if ( !m_pXMLDoc )
		return ERROR_FILE_NOT_FOUND;
	
	IXMLDOMNode* pNode = KeyToNode(hKey);
	if ( !pNode )
		return ERROR_FILE_NOT_FOUND;

	HKEY hSubKey;
	RegCreateKey(hKey, lpSubKey, &hSubKey);

	pNode = KeyToNode(hSubKey);
	pNode->put_text(CComBSTR(lpData));
	return ERROR_SUCCESS;
}

LONG CXMLRegistry::RegDeleteKey(
				  HKEY hKey,         // handle to open key
				  LPCTSTR lpSubKey   // subkey name
				  )
{
	EnsureDocument();
	if ( !m_pXMLDoc )
		return ERROR_FILE_NOT_FOUND;

	IXMLDOMNode* pNode = KeyToNode(hKey);
	pNode = FindChildNode(pNode, lpSubKey);
	if ( !pNode )
		return ERROR_FILE_NOT_FOUND;
	CComPtr<IXMLDOMNode> spParentNode;
	pNode->get_parentNode(&spParentNode);
	if ( spParentNode )
	{
		CComPtr<IXMLDOMNode> spOldChild;
		spParentNode->removeChild(pNode, &spOldChild);
	}
	else
	{
		m_pXMLDoc->putref_documentElement(NULL);
	}
	return ERROR_SUCCESS;
}

LONG CXMLRegistry::RegisterServer(LPCSTR lpszProgID, LPCSTR lpszCLSID, LPCSTR lpszModulePath)
{
	EnsureDocument();
	if ( !m_pXMLDoc )
	{
		return ERROR_INVALID_HANDLE;
	}
	char szTmp[MAX_PATH] = "";
	sprintf(szTmp, "Registry/CLSID/x%s", lpszCLSID);
	HKEY hAppKey;
	RegCreateKey(NULL, szTmp, &hAppKey);
	RegSetValue(hAppKey, "InprocServer32", 0, lpszModulePath, 0);
	RegSetValue(hAppKey, "ProgID", 0, lpszProgID, 0);
	return 0;
}

LONG CXMLRegistry::UnRegisterServer(LPCSTR lpszCLSID)
{
	EnsureDocument();
	if ( !m_pXMLDoc )
	{
		return ERROR_INVALID_HANDLE;
	}

	char szTmp[MAX_PATH] = "";
	sprintf(szTmp, "Registry/CLSID/x%s", lpszCLSID);
	return RegDeleteKey(NULL, szTmp);
}

void CXMLRegistry::EnsureDocument()
{
	if ( !m_pXMLDoc )
	{
		Open("Registry.xml");
	}
}