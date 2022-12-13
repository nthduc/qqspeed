#ifndef __TENIOAPP_H__
#define __TENIOAPP_H__

#pragma warning(disable:4786)

#include "Tlhelp32.h"
#include "../Template/teniopointer_t.h"

namespace Tenio
{
	inline void WINAPI TenInitialize()
	{		
	}
	
	inline void WINAPI TenUninitialize()
	{
		TTenioPtr<ITenioComponentFactory, FALSE> spFactory;

		HANDLE hSnapshot = ::CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, 0);

		if(hSnapshot != INVALID_HANDLE_VALUE && spFactory)
		{
			MODULEENTRY32 moduleEntry = {0};
			moduleEntry.dwSize = sizeof(MODULEENTRY32);

			// 释放所有的Global对象
			if(::Module32First(hSnapshot, &moduleEntry) )
			{
				do
				{
					LPFREEMAP lpFreeMapFun = NULL;

					lpFreeMapFun = (LPFREEMAP)::GetProcAddress(moduleEntry.hModule,
						TEXT("TenioDllFreeMap"));

					if(lpFreeMapFun != NULL)
					{
						(*lpFreeMapFun)();
					}
				}
				while(::Module32Next(hSnapshot, &moduleEntry));
			}

			//释放所有的单键对象，这个顺序是依赖在Factory.h预先设置好的map顺序，解决依赖关系
			spFactory->ReleaseAllSingleton();        

			// 最后释放factory对象，至此，所有的全局， 单键对象释放结束。
			spFactory->Free(TRUE);

			// 由于上面很多全局对象dll等等都已经释放, 可能有些没有释放的对象dll, 在free的时候需要判断这个 g_bDLLSafe 标志
			if(::Module32First(hSnapshot, &moduleEntry) )
			{
				do
				{
					LPSETDLLSAFE lpSetDllSafeFun = NULL;

					lpSetDllSafeFun = (LPSETDLLSAFE)::GetProcAddress(moduleEntry.hModule,
						TEXT("TenioSetDllSafe"));

					if(lpSetDllSafeFun != NULL)
					{
						(*lpSetDllSafeFun)(FALSE);
					}
				}
				while(::Module32Next(hSnapshot, &moduleEntry));
			}

			CloseHandle(hSnapshot);
		}
	}
};

#endif //__TENIOAPP_H__