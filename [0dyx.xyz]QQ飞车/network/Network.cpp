// Network.cpp : Implementation of DLL Exports.


// Note: Proxy/Stub Information
//      To build a separate proxy/stub DLL, 
//      run nmake -f testcomps.mk in the project directory.

#include "stdafx.h"
#include "Network.h"
#include "resource.h"
#include <initguid.h>
#include "NetworkImpl.h"

#include "Network/Network_i.h"
#include "Network/Network_i.c"
#include "framework/GameApp_i.c"

CComModule _Module;

BEGIN_OBJECT_MAP(ObjectMap)
	OBJECT_ENTRY(CLSID_NetworkImpl, CNetworkImpl)
END_OBJECT_MAP()

/////////////////////////////////////////////////////////////////////////////
// DLL Entry Point

CNetworkApp	theApp;
CNetworkApp::CNetworkApp()
{
	_Module.Init(ObjectMap, m_hInstance, &LIBID_NetworkLib);
	DisableThreadLibraryCalls(m_hInstance);
}

CNetworkApp::~CNetworkApp()
{
	_Module.Term();
}
/*
extern "C"
BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID)
{
	if (dwReason == DLL_PROCESS_ATTACH)
	{
		_Module.Init(ObjectMap, hInstance, &LIBID_NetworkLib);
		DisableThreadLibraryCalls(hInstance);
	}
	else if (dwReason == DLL_PROCESS_DETACH)
		_Module.Term();
	return TRUE;    // ok
}
*/
/////////////////////////////////////////////////////////////////////////////
// Used to determine whether the DLL can be unloaded by OLE

STDAPI DllCanUnloadNow(void)
{
	return (_Module.GetLockCount()==0) ? S_OK : S_FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// Returns a class factory to create an object of the requested type

STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID* ppv)
{
//	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return _Module.GetClassObject(rclsid, riid, ppv);
}

/////////////////////////////////////////////////////////////////////////////
// DllRegisterServer - Adds entries to the system registry

STDAPI DllRegisterServer(void)
{
	// registers object, typelib and all interfaces in typelib
	return _Module.RegisterServer(FALSE);
}

/////////////////////////////////////////////////////////////////////////////
// DllUnregisterServer - Removes entries from the system registry

STDAPI DllUnregisterServer(void)
{
	return _Module.UnregisterServer(FALSE);
}


