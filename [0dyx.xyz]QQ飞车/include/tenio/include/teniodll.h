#define TENIO_DLL_IMPLEMENT

#ifndef __TENIODLL_H__
#define __TENIODLL_H__

#pragma warning(disable:4786)
#include <tchar.h>
#include <windows.h>
#include "tstring.h"
#include "teniodllstruct.h"
#include "componentid.h"
#include "memoryalloctor_i.h"
#include "componentimpl_t.h"
#include "stringmanipulate_t.h"
#include "loghelp_t.h"
#include "eventproducerimpl_t.h"
#include "eventimpl_t.h"
#include "fileoperator_t.h"
#include "inifilemanipulate_t.h"
#include "globalerrorcode.h"
#include "teniopointer_t.h"

#define DEFINE_FACTORY_METHOD(ClassName) 	static ClassName::_Creator _ObjectCreator;

extern TCHAR g_szRegistry[MAX_PATH];

// Handle to the instance of the module
extern HMODULE g_hModule;
extern HMODULE g_hFactory;

void AddComponentCreator(DWORD dwComponentID, PCOMPONENTFACTORYMETHOD pMethod);
void RemoveComponent(Tenio::IComponent* pComponent);

#endif