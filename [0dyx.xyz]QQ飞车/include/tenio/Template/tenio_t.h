#ifndef __TENIO_T__
#define __TENIO_T__

#include <map>
#include <list>
#include "teniopointer_t.h"

// 请留意, 本模版是为了兼容老式使用方法而保留, 请使用 TTenioPtr<> 代替

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
		//  获取 TDetail 类型的组件, 通过本函数获取的组件都指向同一个组件, 在第一次调用
		//  本函数时该组件才会被生成
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
		//  创建 TDetail 类型的组件, lpszRegistry 指定注册表位置, 如果lpszRegistry=NULL
		//  表示使用默认注册表(即当前进程根目录下的Tenio.ini). 使用本函数创建的组件不需
		//	要用户删除, 模板在析构时会把这类组件统一释放. 用户也可以调用ReleaseStockTenioComponents()
		//  删除所有此类型组件
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
		//  创建 TDetail 类型的组件, lpszRegistry 指定注册表位置, 如果lpszRegistry=NULL
		//  表示使用默认注册表(即当前进程根目录下的Tenio.ini). 使用本函数创建的组件本模
		//	板不负责删除, 用户需要自己管理这个组件的生命周期
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
		//  创建 TDetail 类型的组件, lpszRegistry 指定注册表位置, 如果lpszRegistry=NULL
		//  表示使用默认注册表(即当前进程根目录下的Tenio.ini). 使用本函数创建的组件本模
		//	板不负责删除, 用户需要自己管理这个组件的生命周期
		//=============================================================================

		template<class TDetail>
		BOOL CreateTenioComponent(TDetail** ppDetail, LPCTSTR lpszRegistry = NULL)
		{
			return CreateTenioComponentByID(ppDetail, Type2ID<TDetail>::ComponentID(), lpszRegistry);
		}

		//=============================================================================
		//  释放组件
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
		//  释放 Stock 组件, 返回 TRUE 表示在 Stock Component 中找到了这个组件
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
		//  释放 Saved 组件
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
		//  释放所有的 StockObject
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
		//  释放所有的 SavedObject
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
	//  内存申请和释放
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