#ifndef __COMPONENT_I_H__
#define __COMPONENT_I_H__

#include "objecttypeidmap.h"

namespace Tenio
{
	class IComponent;

	TENIO_MAKE_TYPEID_MAP(IComponent, 0)

	class IComponent
	{
	public:
		virtual DWORD GetComponentID() = 0;

		virtual DWORD GetComponentVersion() = 0;
		
		virtual void Free(BOOL bFreeSingleton = FALSE) = 0;

		virtual BOOL IsGlobalObject() = 0;
	
		virtual void SetGlobalObject(BOOL bIsGlobalObject) = 0;
	};
};

#endif