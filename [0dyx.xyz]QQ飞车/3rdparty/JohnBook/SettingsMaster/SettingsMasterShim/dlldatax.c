/*----------------------------------------------------------------------
Debugging Applications for Microsoft .NET and Microsoft Windows
John Robbins

Code Originally By Leo Notenboom Feb 2002 MSDN Magazine.
Thanks for letting me use it, Leo!
----------------------------------------------------------------------*/
// wrapper for dlldata.c

#ifdef _MERGE_PROXYSTUB // merge proxy stub DLL

#define REGISTER_PROXY_DLL //DllRegisterServer, etc.

#define _WIN32_WINNT 0x0500   //for WinNT 4.0 or Win95 with DCOM
#define USE_STUBLESS_PROXY	//defined only with MIDL switch /Oicf

#pragma comment(lib, "rpcndr.lib")
#pragma comment(lib, "rpcns4.lib")
#pragma comment(lib, "rpcrt4.lib")

#define ENTRY_PREFIX	Prx
// 4100 - Unreferenced formal parameter.
// 4211 - Nonstandard extension used - redefined extern to static.
// 4152 - ""                         - fn/data ptr conversion in ex.
// 4232 = ""                         - address not static
#pragma warning ( disable : 4100 4211  4152 4232 )
#include "dlldata.c"
#include "SettingsMasterShim_p.c"

#endif //_MERGE_PROXYSTUB
