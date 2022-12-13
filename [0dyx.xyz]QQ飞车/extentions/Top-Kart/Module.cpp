////////////////////////////////////////////////
//     FileName:    Module.cpp
//     Author:      Fish
//     Date:        2006-12-8 10:02:09
//     Description: []
////////////////////////////////////////////////

#include "Top-KartPCH.h"
#include "Top-Kart.h"
#include "Source/Platform/RoomLogic.h"
#include "Source/Platform/LoginLogic.h"

#ifdef USE_TENIOUI
#include "Source/UI/uiimpl.h"
#else
#include "Source/UI/GUIImpl.h"
#endif

#include "framework/GameApp_i.c"
#include "framework/gui_i.c"
#include "network/network_i.c"
#include "framework/vfs_i.c"
#include "topkart_i.c"

CComModule _Module;

BEGIN_OBJECT_MAP(ObjectMap)
	OBJECT_ENTRY(CLSID_TopKart,		CTopKart)
	OBJECT_ENTRY(CLSID_RoomLogic,	CRoomLogic)
	OBJECT_ENTRY(CLSID_LoginLogic,	CLoginLogic)

#ifdef USE_TENIOUI
	OBJECT_ENTRY(CLSID_GUI,			CUIImpl)
#else
	OBJECT_ENTRY(CLSID_GUI,			CGUIImpl)
#endif

END_OBJECT_MAP()

extern "C"
BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID)
{
	if (dwReason == DLL_PROCESS_ATTACH)
	{
		_Module.Init(ObjectMap, hInstance, &LIBID_TopKart);
		DisableThreadLibraryCalls(hInstance);
	}
	else if (dwReason == DLL_PROCESS_DETACH)
		_Module.Term();
	return TRUE;    // ok
}

/////////////////////////////////////////////////////////////////////////////
// Used to determine whether the DLL can be unloaded by OLE

STDAPI DllCanUnloadNow(void)
{
	//	AFX_MANAGE_STATE(AfxGetStaticModuleState());
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

