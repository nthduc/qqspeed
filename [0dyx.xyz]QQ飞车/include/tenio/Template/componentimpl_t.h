#ifndef __COMPONENTIMPL_T_H__
#define __COMPONENTIMPL_T_H__

#include <windows.h>
#include "tenio_t.h"
#include "../Include/component_i.h"
#include "../Include/teniodll.h"
#include "../Include/teniodefines.h"

#define CALLSTACKGUARD	if(!m_bComponentIsValid){DebugBreak();} \
	_CallStackCounter _CallStackCountObject( \
	&m_dwCallStackCount, &m_bComponentIsValid ,static_cast<IComponent*>(this)); \

namespace Tenio
{
template
	<
		class TInterface, 
		class TSubclass, 
		DWORD tdwComponentID, 
		DWORD tdwVersion,
		BOOL  tbSingleton = FALSE
	>
class TComponentImpl : 
	public TInterface,
	public TTenio<TSubclass>
{
public:
	TComponentImpl()
	{
		m_dwCallStackCount   = 0;
		m_bComponentIsValid  = TRUE;
		m_bIsSingletonObject = tbSingleton;
		m_bIsGlobalObject	 = FALSE;
	}

	virtual ~TComponentImpl()
	{
		if(m_bComponentIsValid || m_dwCallStackCount != 0)
		{
			DebugBreak();	// �������ͨ��Free()�ͷ�, ��Break;
		}
	}

//=============================================================================
//  �����ʵ�����������������. ��Ӧ�ð��������ŵ�����������.
//=============================================================================

	virtual void ComponentFinalize() = 0;

//=============================================================================
//  ʵ�� IComponent �ӿ�
//=============================================================================

	DWORD GetComponentID()
	{
		return tdwComponentID;
	}

	DWORD GetComponentVersion()
	{
		return tdwVersion;
	}

    BOOL IsGlobalObject()
    {
        return m_bIsGlobalObject;
    }

    void SetGlobalObject(BOOL bIsGlobalObject)
    {
        m_bIsGlobalObject = bIsGlobalObject;
    }

	void Free(BOOL bFreeSingleton = FALSE)
	{
		if(g_bDLLSafe == FALSE)
			return;

		BOOL bFree = (!m_bIsSingletonObject || bFreeSingleton);

		if(bFree && m_bComponentIsValid)
		{
			ComponentFinalize();
			ReleaseStockTenioComponents();

			m_bComponentIsValid = FALSE;
		}

		if(bFree && m_dwCallStackCount == 0)
		{
			IComponent* pComponent = static_cast<IComponent*>(this);
            
			BOOL bNeedRequestUnloadDll = (
				pComponent->GetComponentID() != COMPONENTID_TENIOCOMPONENTFACTORY);

			// factory������removecompoenentʱ��ע��dll�ã����ǵȽ���RemoveComponentʱ������Ѿ�delete
			// ����ȡComponentID�ˣ���������������
			//if(pComponent->GetComponentID() == COMPONENTID_TENIOCOMPONENTFACTORY)
			//{
			//	pComponent = NULL;
			//}

			delete this;

			RemoveComponent(pComponent, bNeedRequestUnloadDll);
		}
	}

//=============================================================================
//  �ڴ�������ͷ�
//=============================================================================

	template<class TDetail>
	BOOL AllocMemory(TDetail** ppDetail, DWORD dwSize, BOOL bZeroInitialize)
	{
		return TTenio<TSubclass>::TTenio_AllocMemory(ppDetail, dwSize, bZeroInitialize, tdwComponentID);
	}

	BOOL ReleaseMemory(LPVOID lpMemory)
	{
		return TTenio<TSubclass>::TTenio_ReleaseMemory(lpMemory);
	}

public:
//=============================================================================
//  _Creatorʵ���Զ���DLL��"���������б�"��ע�Ṥ�������Ĺ���
//=============================================================================
	class _Creator
	{
	public:
		_Creator()
		{
			AddComponentCreator(tdwComponentID, CreateDetailComponent);
		}

		static IComponent* CreateDetailComponent()
		{
			IComponent* pResult = NULL;

			if(tbSingleton)
			{
				static IComponent* s_pSingleton = NULL;

				if(s_pSingleton == NULL)
				{
					s_pSingleton = new TSubclass;
				}

				pResult = s_pSingleton;

				// ע����factory�ϣ� ���Ӧ���˳�����factoryͳһ��dllж��ǰ�ͷţ�������dllж�ص����������쳣��
                if(pResult->GetComponentID() != COMPONENTID_TENIOCOMPONENTFACTORY)
                {
					TTenioPtr<ITenioComponentFactory> pFactory;
					pFactory->RegisterSingletonObject(pResult);
                }
			}
			else
			{
				pResult = new TSubclass;
			}
			
			return pResult;
		}
	};

//=============================================================================
//  ��ջ���ü�����
//=============================================================================

	class _CallStackCounter
	{
	public:
		_CallStackCounter(DWORD* pdwCallStackCount, BOOL* pbIsObjectValid, IComponent* pComponent)
			: m_pdwCallStackCount(pdwCallStackCount), 
			  m_pbIsObjectValid(pbIsObjectValid), 
			  m_pComponent(pComponent)
		{
			if(pdwCallStackCount != NULL)
			{
				(*pdwCallStackCount)++;
			}
		}

		virtual ~_CallStackCounter()
		{
			if(m_pdwCallStackCount != NULL && m_pbIsObjectValid != NULL)
			{
				(*m_pdwCallStackCount)--;

				if(*m_pdwCallStackCount == 0 && 
				   !(*m_pbIsObjectValid) && 
				   m_pComponent != NULL)
				{
					m_pComponent->Free();
				}
			}
		}
	protected:
		BOOL* m_pbIsObjectValid;
		DWORD* m_pdwCallStackCount;
		IComponent* m_pComponent;
	};

	// ������ TTenio, ���lpszRegistryΪNULL����Ϊ��������g_szRegistry
	virtual void TTenio_ConvertRegistryPath(LPCTSTR lpszRegistry, 
		LPTSTR lpszResultRegistry, int nCharCount)
	{
		BOOL bIsNull = (lpszRegistry == NULL);
		BOOL bIsEmpty = (lpszRegistry != NULL && lpszRegistry[0] == TEXT('\0'));

		if(bIsNull || bIsEmpty)
		{
			if(lpszResultRegistry != NULL && nCharCount > 0)
			{
				_tcscpy(lpszResultRegistry, g_szRegistry);
			}
		}
		else
		{
			if(lpszRegistry != NULL && lpszResultRegistry != NULL && nCharCount > 0)
			{
				_tcscpy(lpszResultRegistry, lpszRegistry);
			}
		}
	}

	template<class TClass>
	BOOL IsValidPointer(TClass* pTClass)
	{
#ifdef _DEBUG
		return ((DWORD)pTClass > 1000);
#else
		return (pTClass != NULL);
#endif
	}

protected:
	BOOL  m_bComponentIsValid;
	DWORD m_dwCallStackCount;
	BOOL  m_bIsSingletonObject;
	BOOL  m_bIsGlobalObject;

}; // template
}; // namespace

#endif