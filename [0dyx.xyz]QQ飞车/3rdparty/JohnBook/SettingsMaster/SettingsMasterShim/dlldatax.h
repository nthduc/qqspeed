/*----------------------------------------------------------------------
Debugging Applications for Microsoft .NET and Microsoft Windows
John Robbins

Code Originally By Leo Notenboom Feb 2002 MSDN Magazine.
Thanks for letting me use it, Leo!
----------------------------------------------------------------------*/
#pragma once

#ifdef _MERGE_PROXYSTUB

extern "C"
{
BOOL WINAPI PrxDllMain(HINSTANCE hInstance, DWORD dwReason,
	LPVOID lpReserved);
STDAPI PrxDllCanUnloadNow(void);
STDAPI PrxDllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID* ppv);
STDAPI PrxDllRegisterServer(void);
STDAPI PrxDllUnregisterServer(void);
}

#endif
