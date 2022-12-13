#ifndef __MEMORY_ALLOCTOR_I_H__
#define __MEMORY_ALLOCTOR_I_H__

#include "teniointerface.h"

namespace Tenio
{
	class IMemoryAlloctor;

	TENIO_MAKE_TYPEID_MAP(IMemoryAlloctor, COMPONENTID_MEMORYALLOCTOR)
		
	class IMemoryAlloctor : public IComponent
	{
	public:
		virtual LPVOID Alloc(DWORD dwCreator, DWORD dwSize) = 0;

		virtual BOOL Free(LPVOID lpMemory) = 0;
	};
};

#endif //__MEMORY_ALLOCTOR_I_H__