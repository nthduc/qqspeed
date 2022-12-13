////////////////////////////////////////////////
//     FileName:    mycom.h
//     Author:      Fish
//     Date:        2006-9-19 11:51:10
//     Description: []
////////////////////////////////////////////////

#pragma once
#include "atlconv.h"
#include "txtCtrlr.h"
#include "assert.h"

#define RELEASE(pv)	if(pv)	{ (pv)->Release(); pv = NULL; }	

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

class CMyComRegistry
{
public:

};