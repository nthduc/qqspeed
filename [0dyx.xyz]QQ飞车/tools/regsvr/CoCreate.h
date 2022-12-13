#pragma once

#include "XMLRegistry.h"
#include "assert.h"

namespace XCO
{
	inline HRESULT CLSID2ModuleName(
		REFCLSID rclsid,     //Class identifier (CLSID) of the object
		LPSTR lpszModuleName
		)
	{
		LPOLESTR lpszCLSID;
		StringFromCLSID(rclsid, &lpszCLSID);
		char szCLSID[MAX_PATH] = "";
		BOOL bUseDefault;
		WideCharToMultiByte(CP_ACP, 0, lpszCLSID, wcslen(lpszCLSID), szCLSID, MAX_PATH, "", &bUseDefault);
		szCLSID[strlen(szCLSID)-1] = 0;
		char szTmp[MAX_PATH] = "";
		sprintf(szTmp, "Registry/CLSID/x%s/InprocServer32", &szCLSID[1]);

		CXMLRegistry reg(TRUE);
		LONG cbValue = MAX_PATH;
		reg.RegQueryValue(NULL, szTmp, lpszModuleName, &cbValue);
		return ERROR_SUCCESS;
	}


	inline HINSTANCE CreateObjectFromFile(const char* szPath, REFCLSID factoryid, REFCLSID interfaceid, LPVOID *pInterface)
	{
		USES_CONVERSION;
		if(NULL == pInterface || NULL == szPath)
		{
			assert(FALSE);
			return NULL;
		}
		typedef HRESULT (__stdcall *GETCLASS_PROC)(REFCLSID, REFIID, LPVOID*);
		*pInterface = NULL;
		HRESULT hr = S_FALSE;
		HINSTANCE hInstance = ::CoLoadLibrary(T2OLE(szPath), FALSE);
		GETCLASS_PROC lpGetClssProc = (GETCLASS_PROC)::GetProcAddress(hInstance, "DllGetClassObject"); 
		if(lpGetClssProc)
		{
			IClassFactory *pfactory = NULL;
			hr = lpGetClssProc(factoryid, IID_IClassFactory, (void**)&pfactory);
			if(pfactory)
			{
				hr = pfactory->CreateInstance(NULL, interfaceid, pInterface);
				pfactory->Release();
			}
		}
		return hInstance;
	}

	inline HRESULT CoCreateInstance(
		REFCLSID rclsid,     //Class identifier (CLSID) of the object
		LPUNKNOWN pUnkOuter, //Pointer to controlling IUnknown
		DWORD dwClsContext,  //Context for running executable code
		REFIID riid,         //Reference to the identifier of the interface
		LPVOID * ppv         //Address of output variable that receives 
		// the interface pointer requested in riid
		)
	{
		HRESULT hr;
		char szModuleName[MAX_PATH] = "";
		hr = CLSID2ModuleName(rclsid, szModuleName);
		if ( hr != S_OK )
			return REGDB_E_CLASSNOTREG;

		CreateObjectFromFile(szModuleName, rclsid, riid, ppv);
		return ERROR_SUCCESS;
	}
};
