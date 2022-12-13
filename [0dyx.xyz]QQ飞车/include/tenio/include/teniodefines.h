#ifndef __TENIODEFINES_H__
#define __TENIODEFINES_H__

#include "component_i.h"

#define TENIO_SHORTSTRING	36

typedef Tenio::IComponent* (*LPCREATECOMPONENT) (DWORD dwComponentID, DWORD dwCreateorID);
typedef BOOL (*PSETREGISTRY) (LPCTSTR lpszRegistry);
typedef void (*LPFREEMAP) ();
typedef BOOL (*LPTENIODLLCANUNLOADNOW) ();
typedef void (*LPSETFACTORY)(HMODULE hFactory);
typedef void (*LPSETDLLSAFE)(BOOL bSafe);
#endif