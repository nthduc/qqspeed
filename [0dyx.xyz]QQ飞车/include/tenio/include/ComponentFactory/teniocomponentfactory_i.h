#ifndef __TENIOCOMPONENTFACTORY_I_H__
#define __TENIOCOMPONENTFACTORY_I_H__

#include "../teniointerface.h"

namespace Tenio
{
	class ITenioComponentFactory;

	TENIO_MAKE_TYPEID_MAP(ITenioComponentFactory, COMPONENTID_TENIOCOMPONENTFACTORY);

	class ITenioComponentFactory : public IComponent
	{
	public:
		virtual IComponent* CreateTenioComponent(DWORD dwComponentId, 
			LPCTSTR lpszRegistryFile) = 0;
		
		virtual void GetLogFlags(LPCTSTR lpszRegistry, DWORD dwComponentId,
			BOOL& bVerboseLog, BOOL& bTraceLog, BOOL& bErrorLog) = 0;

		virtual void RegisterSingletonObject(IComponent* pObj) = 0;

		virtual void ReleaseAllSingleton() = 0;

		virtual BOOL UnLoadLibrary(HMODULE hModule) = 0;

		virtual void RegisterLibraryUnloading(HMODULE hModule) = 0;
	};
};

#endif

// InterfaceName:TenioComponentFactory
// SubFolderName:ComponentFactory
