#ifndef __TENIO_T__
#define __TENIO_T__

#include <map>
#include <list>
#include "teniopointer_t.h"

// ������, ��ģ����Ϊ�˼�����ʽʹ�÷���������, ��ʹ�� TTenioPtr<> ����

namespace Tenio
{
	typedef std::map<DWORD, TTenioPtr<IComponent>*> STOCKOBJECTMAP;

	typedef std::list<TTenioPtr<IComponent>*> SAVEDOBJECTLIST;

	template<class TSubclass>
	class TTenio
	{
	public:

		virtual ~TTenio()
		{
			ReleaseSavedObjects();
			
			ReleaseStockTenioComponents();
		}

		//=============================================================================
		//  ��ȡ TDetail ���͵����, ͨ����������ȡ�������ָ��ͬһ�����, �ڵ�һ�ε���
		//  ������ʱ������Żᱻ����
		//=============================================================================

		template<class TDetail>
		BOOL GetTenioComponent(TDetail** ppDetail, LPCTSTR lpszRegistry = NULL)
		{
			BOOL bSuccess = FALSE;

			if(ppDetail != NULL)
			{
				DWORD dwComponentId = Type2ID<TDetail>::ComponentID();

				STOCKOBJECTMAP::iterator it = m_mapStockObjects.find(dwComponentId);

				if(it != m_mapStockObjects.end())
				{
					TTenioPtr<IComponent>* pspComponent = it->second;

					*ppDetail = static_cast<TDetail*>((IComponent*)*pspComponent);

					bSuccess = TRUE;
				}
				else
				{
					TTenioPtr<IComponent>* pspComponent = new TTenioPtr<IComponent>(
						lpszRegistry, dwComponentId);

					if(*pspComponent)
					{
						m_mapStockObjects[dwComponentId] = pspComponent;
						
						*ppDetail = static_cast<TDetail*>((IComponent*)(*pspComponent));

						bSuccess = TRUE;
					}
					else
					{
						delete pspComponent;
					}
				}
			}

			return bSuccess;
		}
		
		//=============================================================================
		//  ���� TDetail ���͵����, lpszRegistry ָ��ע���λ��, ���lpszRegistry=NULL
		//  ��ʾʹ��Ĭ��ע���(����ǰ���̸�Ŀ¼�µ�Tenio.ini). ʹ�ñ������������������
		//	Ҫ�û�ɾ��, ģ��������ʱ����������ͳһ�ͷ�. �û�Ҳ���Ե���ReleaseStockTenioComponents()
		//  ɾ�����д��������
		//=============================================================================

		template<class TDetail>
		BOOL GetNewTenioComponent(TDetail** ppDetail, LPCTSTR lpszRegistry = NULL)
		{
			BOOL bSuccess = FALSE;

			if(ppDetail != NULL)
			{
				TTenioPtr<IComponent>* pspComponent = new TTenioPtr<IComponent>(
					lpszRegistry, Type2ID<TDetail>::ComponentID());

				if(*pspComponent)
				{
					m_lstSavedObjects.push_back(pspComponent);

					*ppDetail = static_cast<TDetail*>((IComponent*)(*pspComponent));

					bSuccess = TRUE;
				}
				else
				{
					delete pspComponent;
				}
			}

			return bSuccess;
		}

		//=============================================================================
		//  ���� TDetail ���͵����, lpszRegistry ָ��ע���λ��, ���lpszRegistry=NULL
		//  ��ʾʹ��Ĭ��ע���(����ǰ���̸�Ŀ¼�µ�Tenio.ini). ʹ�ñ����������������ģ
		//	�岻����ɾ��, �û���Ҫ�Լ���������������������
		//=============================================================================
		
		template<class TDetail>
		BOOL CreateTenioComponentByID(TDetail** ppDetail, DWORD dwComponentID, LPCTSTR lpszRegistry = NULL)
		{
			BOOL bSuccess = FALSE;

			if(ppDetail != NULL)
			{
				TTenioPtr<TDetail> spComponent(lpszRegistry, dwComponentID);

				if(spComponent)
				{
					*ppDetail = spComponent.Detach();
					
					bSuccess = TRUE;
				}
			}

			return bSuccess;
		}	

		//=============================================================================
		//  ���� TDetail ���͵����, lpszRegistry ָ��ע���λ��, ���lpszRegistry=NULL
		//  ��ʾʹ��Ĭ��ע���(����ǰ���̸�Ŀ¼�µ�Tenio.ini). ʹ�ñ����������������ģ
		//	�岻����ɾ��, �û���Ҫ�Լ���������������������
		//=============================================================================

		template<class TDetail>
		BOOL CreateTenioComponent(TDetail** ppDetail, LPCTSTR lpszRegistry = NULL)
		{
			return CreateTenioComponentByID(ppDetail, Type2ID<TDetail>::ComponentID(), lpszRegistry);
		}

		//=============================================================================
		//  �ͷ����
		//=============================================================================

		template<class TDetail>
		BOOL ReleaseTenioComponent(TDetail** ppDetail)
		{
			BOOL bSuccess = FALSE;

			if(ppDetail != NULL)
			{
				IComponent* pToRelease = static_cast<IComponent*>(*ppDetail);

				bSuccess = ReleaseObjectInStock(pToRelease);

				if(!bSuccess)
				{
					bSuccess = ReleaseObjectInSavedList(pToRelease);
				}

				if(!bSuccess)
				{
					if(*ppDetail != NULL)
					{
						(*ppDetail)->Free();

						bSuccess = TRUE;
					}
				}

				if(bSuccess)
				{
					*ppDetail = NULL;
				}
			}

			return bSuccess;
		}

		//=============================================================================
		//  �ͷ� Stock ���, ���� TRUE ��ʾ�� Stock Component ���ҵ���������
		//=============================================================================

		BOOL ReleaseObjectInStock(IComponent* pComponent)
		{
			BOOL bReleased = FALSE;

			STOCKOBJECTMAP::iterator it = m_mapStockObjects.begin();

			while(it != m_mapStockObjects.end())
			{
				TTenioPtr<IComponent>* pspComponent = it->second;

				if(pspComponent != NULL)
				{
					IComponent* pComponentInStock = *pspComponent;

					if(pComponent == pComponentInStock && pComponent != NULL)
					{
						pComponent->Free();

						m_mapStockObjects.erase(it);

						bReleased = TRUE;

						break;
					}
				}

				++it;
			}

			return bReleased;
		}

		//=============================================================================
		//  �ͷ� Saved ���
		//=============================================================================

		BOOL ReleaseObjectInSavedList(IComponent* pComponent)
		{
			BOOL bSuccess = FALSE;

			SAVEDOBJECTLIST::iterator it = m_lstSavedObjects.begin();

			while(it != m_lstSavedObjects.end())
			{
				TTenioPtr<IComponent>* pspComponent = *it;

				if(pspComponent != NULL)
				{
					IComponent* pComponentInSavedList = *pspComponent;

					if(pComponentInSavedList == pComponent && pComponent != NULL)
					{
						pComponentInSavedList->Free();

						bSuccess = TRUE;

						m_lstSavedObjects.erase(it);

						break;
					}
				}

				++it;
			}

			return bSuccess;
		}

		//============================================================================
		//  �ͷ����е� StockObject
		//============================================================================

		void ReleaseStockTenioComponents()
		{
			STOCKOBJECTMAP::iterator it = m_mapStockObjects.begin();

			while(it != m_mapStockObjects.end())
			{
				TTenioPtr<IComponent>* pspComponent = it->second;

				if(pspComponent != NULL)
				{
					delete pspComponent;
				}

				++it;
			}

			m_mapStockObjects.clear();
		}

		//============================================================================
		//  �ͷ����е� SavedObject
		//============================================================================

		void ReleaseSavedObjects()
		{
			SAVEDOBJECTLIST::iterator it = m_lstSavedObjects.begin();

			while(it != m_lstSavedObjects.end())
			{
				TTenioPtr<IComponent>* pspComponent = *it;

				if(pspComponent != NULL)
				{
					delete pspComponent;
				}

				++it;
			}
		}
		
	//=============================================================================
	//  �ڴ�������ͷ�
	//=============================================================================
	
	template<class TDetail>
	BOOL TTenio_AllocMemory(TDetail** ppDetail, DWORD dwSize, BOOL bZeroInitialize,
		DWORD tdwComponentID = 0)
	{
		BOOL bSuccess = FALSE;
		
		IMemoryAlloctor* pMemoryAlloctor = NULL;
		GetTenioComponent(&pMemoryAlloctor, g_szRegistry);
		
		if(pMemoryAlloctor != NULL)
		{
			LPVOID lpMemory = pMemoryAlloctor->Alloc(tdwComponentID, dwSize);
			
			if(lpMemory != NULL)
			{
				*ppDetail = (TDetail*)lpMemory;
				
				if(bZeroInitialize)
				{
					memset(lpMemory, 0, dwSize);
				}
				
				bSuccess = TRUE;
			}
		}
		
		return bSuccess;
	}
	
	BOOL TTenio_ReleaseMemory(LPVOID lpMemory)

	{
		BOOL bSuccess = FALSE;
		
		IMemoryAlloctor* pMemoryAlloctor = NULL;
		GetTenioComponent(&pMemoryAlloctor, g_szRegistry);
		
		if(pMemoryAlloctor != NULL)
		{
			bSuccess = pMemoryAlloctor->Free(lpMemory);
		}
		
		return bSuccess;
	}

	private:
		STOCKOBJECTMAP m_mapStockObjects;
		SAVEDOBJECTLIST m_lstSavedObjects;
	};
};

#endif