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

			// �ͷ����е�Global����
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

			//�ͷ����еĵ����������˳����������Factory.hԤ�����úõ�map˳�򣬽��������ϵ
			spFactory->ReleaseAllSingleton();        

			// ����ͷ�factory�������ˣ����е�ȫ�֣� ���������ͷŽ�����
			spFactory->Free(TRUE);

			// ��������ܶ�ȫ�ֶ���dll�ȵȶ��Ѿ��ͷ�, ������Щû���ͷŵĶ���dll, ��free��ʱ����Ҫ�ж���� g_bDLLSafe ��־
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