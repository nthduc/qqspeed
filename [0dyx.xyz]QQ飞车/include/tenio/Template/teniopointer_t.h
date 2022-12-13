#ifndef __TENIOPOINTER_T_H__
#define __TENIOPOINTER_T_H__

#define FACTORY_MODULE TEXT("TenFact.dll")
#define MAX_KEVALUE_LEN 1024
#define MAX_NUMBER_LEN	10

#include <tchar.h>
#include "../include/safestring.h"
#include "../Include/teniodefines.h"
#include "../Include/ComponentFactory/teniocomponentfactory_i.h"

#ifdef TENIO_DLL_IMPLEMENT
#include "teniopointerhelper.h"
#endif

namespace Tenio
{
//#pragma warning(push)
#pragma warning(disable:4127)

	template <class T, BOOL bAutoDelete = TRUE>
	class TTenioPtr
	{
	public:

		/** 
		 * @brief Tenio �������ָ��
		 *
		 * @param lpszRegistry ָ�� Tenio.ini ��·��, �����Ǿ���·���������·��.
		 *        ���Ϊ NULL ��ȡĬ��ֵ(����exe����Ŀ¼)
		 *
		 * @param dwComponentId ָ����Ҫ������ ComponentID�����Ϊ0����ʾȡ�ӿ���
		 *  	  ��(��ģ�����T)��Ĭ��ʵ�����ID
		 */

		TTenioPtr()
		{
			m_pObject  = NULL;
			m_pFactory = NULL;
			
			DWORD dwComponentId = Type2ID<T>::ComponentID();
			
#ifdef TENIO_DLL_IMPLEMENT
			LoadFactory(g_szRegistry);

			if(m_pFactory != NULL)
			{
				IComponent* pComponent = m_pFactory->CreateTenioComponent(
					dwComponentId, g_szRegistry);
				
				m_pObject = static_cast<T*>(pComponent);
				if(m_pObject)
					m_pObject->SetGlobalObject(!bAutoDelete);
			}

#else
			LoadFactory(NULL);

			if(m_pFactory != NULL)
			{
				IComponent* pComponent = m_pFactory->CreateTenioComponent(
					dwComponentId, NULL);
				
				m_pObject = static_cast<T*>(pComponent);
				if(m_pObject)
					m_pObject->SetGlobalObject(!bAutoDelete);
			}
#endif
			
		}

		TTenioPtr(LPCTSTR lpszRegistry) 
		{
#ifdef TENIO_DLL_IMPLEMENT
			if(lpszRegistry == NULL || lpszRegistry[0] == TEXT('\0'))
			{
				lpszRegistry = g_szRegistry;
			}
#endif
			m_pObject  = NULL;
			m_pFactory = NULL;
			
			DWORD dwComponentId = Type2ID<T>::ComponentID();
			
			LoadFactory(lpszRegistry);
			
			if(m_pFactory != NULL)
			{
				IComponent* pComponent = m_pFactory->CreateTenioComponent(
					dwComponentId, lpszRegistry);
				
				m_pObject = static_cast<T*>(pComponent);
				if(m_pObject)
					m_pObject->SetGlobalObject(!bAutoDelete);
			}
		}
		
		TTenioPtr(LPCTSTR lpszRegistry, DWORD dwComponentId)
		{
#ifdef TENIO_DLL_IMPLEMENT
			if(lpszRegistry == NULL || lpszRegistry[0] == TEXT('\0'))
			{
				lpszRegistry = g_szRegistry;
			}
#endif
			m_pObject  = NULL;
			m_pFactory = NULL;
			
			if(dwComponentId == 0)
			{
				TTenio_Trace(TEXT("[Tenio_Error] TTenioPtr with ComponentId = 0\n"));
			}

			LoadFactory(lpszRegistry);

			if(m_pFactory != NULL)
			{
				IComponent* pComponent = m_pFactory->CreateTenioComponent(
					dwComponentId, lpszRegistry);

				m_pObject = static_cast<T*>(pComponent);
				
				if(m_pObject)
					m_pObject->SetGlobalObject(!bAutoDelete);
			}
		}

		/**
		 * @brief ������������ɾ��������ָ���Ӧ�����
		 */

		virtual ~TTenioPtr()
		{
			if(bAutoDelete && m_pObject != NULL)
			{
				static_cast<IComponent*>(m_pObject)->Free();

				m_pObject = NULL;
			}
		}

		operator T*() const
		{
			return (T*)m_pObject;
		}

		T& operator*() const
		{
			return *m_pObject;
		}

		T* operator->() const
		{
			return m_pObject;
		}

		bool operator!() const
		{
			return (m_pObject == NULL);
		}

		T* Detach()
		{
			T* pTemp = m_pObject;

			m_pObject = NULL;

			return pTemp;
		}
		
	protected:
		T* m_pObject;

		ITenioComponentFactory* m_pFactory;

	protected:

#ifndef TENIO_DLL_IMPLEMENT
		enum EPARSESTATUS
		{
			EPS_FREE	   = 0,
			EPS_INSENTENCE = 1
		};
#endif

	protected:

	   /**
	    * @brief ��ȡ TenioComponentFactory ��ָ��
		* @param lpszRegistry ָ��ע���·��
	    */
		void LoadFactory(LPCTSTR lpszRegistry)
		{
			HMODULE hFact = GetFactoryDLLHandle(lpszRegistry);

			if(hFact != NULL)
			{
				IComponent* pFactory = CreateComponentInModule(hFact, 
					COMPONENTID_TENIOCOMPONENTFACTORY);

				m_pFactory = static_cast<ITenioComponentFactory*>(pFactory);

				if(m_pFactory == NULL)
				{
					TTenio_Trace(TEXT("[Tenio_Error] Create ComponentFactory in TenFact.dll Fail\n"));
				}
			}
			else
			{
				TTenio_Trace(TEXT("[Tenio_Error] Load TenFact.dll Fail.\n"));
			}
		}

#ifndef TENIO_DLL_IMPLEMENT
		
	   /**
	    * @brief �� hModule �д������IDΪ dwComponentId �����
		* @param hModule �ṩ�����DLL�ľ��
		* @param dwComponentId ��� ID
		*/
		IComponent* CreateComponentInModule(HMODULE hModule, DWORD dwComponentId)
		{
			IComponent* pComponent = NULL;
			
			LPCREATECOMPONENT lpCreateMethod = NULL;
			
			lpCreateMethod = (LPCREATECOMPONENT)GetProcAddress(
				hModule, "CreateComponent");
			
			if(lpCreateMethod != NULL)
			{
				pComponent = (*lpCreateMethod)(dwComponentId, 0);

				if(pComponent == NULL)
				{
					TTenio_Trace(TEXT("[Tenio_Error] CreateComponent(ComponentId=%u) return null.\n"), dwComponentId);
				}
			}
			else
			{
				TTenio_Trace(TEXT("[Tenio_Error] Can't find CreateComponent().\n"));
			}

			return pComponent;
		}

	   /**
	    * @brief ��ȡ TenFact.dll �ľ��
		* 
		* �������Ѽ���ģ��������TenFact.dll, ���û���ҵ�, ���� lpszRegistry
		* �ļ���ʾ�������Ϣȥ���� TenFact.dll��
		*
		* @param lpszRegistry ע����ļ�����·��
		* @return ���� TenFact.dll �ľ��
	    */
		HMODULE GetFactoryDLLHandle(LPCTSTR lpszRegistry)
		{
#ifdef TENIO_DLL_IMPLEMENT
			HMODULE hFact = g_hFactory;
#else
			HMODULE hFact = GetModuleHandle(FACTORY_MODULE);
#endif

			if(hFact == NULL) // TenFact.dll ��û�м���
			{
				TCHAR szFinalRegistry[MAX_PATH] = {0}; // ��ʾ׷�ٵ�����ע����·��
				GetRealRegistryFile(lpszRegistry, szFinalRegistry, MAX_PATH);

				TCHAR szTenFactDLLPath[MAX_PATH] = {0};
				GetTenioFactoryDLLPath(szFinalRegistry, szTenFactDLLPath, MAX_PATH);

				hFact = ::LoadLibrary(szTenFactDLLPath);
			}

			return hFact;
		}

	   /**
	    * @brief �� lpszRegistry ע����ļ�����ȡ TenFact.dll �ľ���·��
	    * @param lpszRegistry Tenio ע����ļ��������Ǿ���·����ע������ļ�����
		*        ���������������Ϣ������ʹ�� Redirect ָ����ת������ע���
		* @param lpszOutput ָ��һ�黺���������ڽ��� TenFact.dll �ľ���·��
		* @param nCharCount lpszOutput �������ĳ��ȣ���λ���ַ�����
	    */
		void GetTenioFactoryDLLPath(LPCTSTR lpszRegistry, LPTSTR lpszOutput,
			int nCharCount)
		{
			// ��ȡ ComponentFactory �����·��(���ID��361)
			TCHAR szFactoryPath[MAX_PATH] = {0};
			ReadIniString(lpszRegistry, TEXT("361"), TEXT("DLL"), NULL, 
				szFactoryPath, MAX_PATH);

			if(!IsRelativePath(szFactoryPath))
			{
				// ����Ǿ���·��, ֱ�Ӱ�·����������
				StringCopy(lpszOutput, nCharCount, szFactoryPath);
			}
			else
			{
				TranslateComponentRelativePath(lpszRegistry, szFactoryPath,
					lpszOutput, nCharCount);
			}
		}

	   /**
		* @brief �������DLL�����·�����������·����������û��ָ��
		*		 "NotUseDefaultDllPath" ��ô�����·��������[DefaultValue]-DllPath 
		*		 ����. Ȼ����� lpszRegistryPath����Ŀ¼�����������·��
		*
		* @param lpszRegistryPath ע����ļ��ľ���·��
		* @param lpszComponentDLLPath ���DLL�����·��
		* @param lpszOutput ���ս���Ļ�����
		* @param nCharCount ������������(��λΪ�ַ�����)
		*/
		inline void TranslateComponentRelativePath(LPCTSTR lpszRegistryPath,
			LPCTSTR lpszComponentDLLPath, LPTSTR lpszOutput, int nCharCount)
		{
			// �ж� Factory ����Ƿ��ʹ�� DefaultDllPath
			TCHAR szNotUseDefaultValue[MAX_NUMBER_LEN] = {0};
			ReadIniString(lpszRegistryPath, TEXT("361"), TEXT("NotUseDefaultDllPath"),
				TEXT("0"), szNotUseDefaultValue, MAX_NUMBER_LEN);
			
			if(szNotUseDefaultValue[0] == TEXT('1'))
			{
				CombineRelativePath(lpszRegistryPath, lpszComponentDLLPath, lpszOutput,
					nCharCount);
			}
			else
			{
				// ��ȡ [DefaultValue] - DllPath
				TCHAR szDefaultDllPath[MAX_PATH] = {0};
				GetPrivateProfileString(TEXT("DefaultValue"), TEXT("DllPath"),
					NULL, szDefaultDllPath, MAX_PATH, lpszRegistryPath);
				
				TCHAR szTempPath[MAX_PATH] = {0};
				CombineWithPathSeparator(szDefaultDllPath, lpszComponentDLLPath,
					szTempPath, MAX_PATH);
				
				if(IsRelativePath(szTempPath))
				{
					CombineRelativePath(lpszRegistryPath, szTempPath, lpszOutput,
						nCharCount);
				}
				else
				{
					StringCopy(lpszOutput, nCharCount, szTempPath);
				}
			}
		}

       /**
	    * @brief ��·���ָ������������ַ���, ���lpszLeftΪ���ַ���, ��ֻ����
		*        lpszRight �� lpszOutput ��
		*
		* @param lpszLeft ��һ�ַ���
		* @param lpszRight �ڶ��ַ���
		* @param lpszOutput ���ս���Ļ�����
		* @param nCharCount ������������(�ַ�����)
		*/
		void CombineWithPathSeparator(LPCTSTR lpszLeft, LPCTSTR lpszRight,
			LPTSTR lpszOutput, int nCharCount)
		{
			if(lpszOutput != NULL && nCharCount > 0)
			{
				lpszOutput[0] = TEXT('\0');
				
				if(IsStringEmpty(lpszLeft))
				{
					StringCopy(lpszOutput, nCharCount, lpszRight);
				}
				else
				{
					StringCat(lpszOutput, nCharCount, lpszLeft);
					StringCat(lpszOutput, nCharCount, TEXT("\\"));
					StringCat(lpszOutput, nCharCount, lpszRight);
				}
			}
		}

	   /**
	    * @brief ��INI�ļ��ж�ȡ����, �Զ�ɾ�����ԵĿո��\t
		* @param lpszFile INI�ļ��ľ���·��
		* @param lpszSectionName INI�еĽ���
		* @param lpszKeyName ���е�KeyName
		* @param lpszDefaultValue ���û���ҵ���ȷ��Key, ������ֵ���뵽�����������
		* @param lpszValue ���ڽ���Key��ȡֵ
		* @param nCharCount lpszValue������(�ַ�����)
		*/
		inline void ReadIniString(LPCTSTR lpszFile, LPCTSTR lpszSectionName,
			LPCTSTR lpszKeyName, LPCTSTR lpszDefaultValue, LPTSTR lpszValue,
			int nCharCount)
		{
			TCHAR szValue[MAX_KEVALUE_LEN] = {0};
			GetPrivateProfileString(lpszSectionName, lpszKeyName, lpszDefaultValue,
				szValue, MAX_KEVALUE_LEN, lpszFile);

			TrimString(szValue, lpszValue, nCharCount);
		}

	   /**
	    * @brief ɾ���ַ������ߵĿհ��ַ�(�ո�/TAB)
		* @param lpszSource ��Ҫ���в������ַ�������
		* @param lpszDest ����ַ�����ŵĻ�����
		* @param nCharCount lpszDest������(�ַ�����)
		*/
		inline void TrimString(LPCTSTR lpszSource, LPTSTR lpszDest, int nCharCount)
		{
			int nStatus = EPS_FREE;

			LPCTSTR lpszStart = lpszSource;
			LPCTSTR lpszValue = lpszSource;
			LPCTSTR lpszEnd = NULL;

			while(*lpszValue != TEXT('\0'))
			{
				if(!IsWhiteChar(*lpszValue))
				{
					if(nStatus == EPS_FREE)
					{
						nStatus = EPS_INSENTENCE;

						lpszStart = lpszValue;
					}
				}
				else
				{
					if(nStatus == EPS_INSENTENCE)
					{
						lpszEnd = lpszValue;
					}
				}

				lpszValue++;
			}

			// ���Ȱ� lpszOutput ���
			lpszDest[0] = TEXT('\0');

			if(lpszStart != NULL)
			{
				if(lpszEnd == NULL)
				{
					StringCopy(lpszDest, nCharCount, lpszStart);
				}
				else
				{
					StringCopyFix(lpszDest, nCharCount, lpszStart,
						int(lpszEnd - lpszStart));
				}
			}
		}

	   /**
	    * @brief �ж�һ���ַ��Ƿ�һ�� White Char(�ո����TAB)
		* @param ch ��Ҫ�����жϵ��ַ�
		* @return �����White Char, �򷵻� TRUE, ���򷵻� FALSE
		*/
		inline BOOL IsWhiteChar(const TCHAR& ch)
		{
			return ch == TEXT(' ') || ch == TEXT('\t');
		}

	   /**
	    * @brief ��ȡ������ע����ļ�
		*
		* ��� lpszRegistry �ַ���Ϊ��(���ߵ���NULL),  ��Ĭ��Ϊ���̸�Ŀ¼�����
		* Tenio.ini Ϊע����ļ�. ���������� Redirect ����,  һֱ׷�ٵ�������ע
		* ����ļ�, �����׷�ٵ���ע����ļ�·������ lpszRealRegistry
		*
		* @param lpszRegistry ��Դ��ע����ļ�
		* @param lpszRealRegistry ����������
		* @param nCharCount lpszRealRegistry ������(��λΪ�ַ�����)
	    */
		void GetRealRegistryFile(LPCTSTR lpszRegistry, LPTSTR lpszRealRegistry, 
			int nCharCount)
		{
			BOOL bUseDefault = (lpszRegistry == NULL) || (lpszRegistry[0] == TEXT('\0'));
			
			if(bUseDefault)
			{
				TCHAR szDefaultRegistry[MAX_PATH] = {0};
				Relative2AbsolutePath(TEXT("Tenio.ini"), szDefaultRegistry, MAX_PATH);
				
				TrackRegistry(szDefaultRegistry, lpszRealRegistry, nCharCount);
			}
			else
			{
				if(IsRelativePath(lpszRegistry))
				{
					TCHAR szFullPath[MAX_PATH] = {0};
					Relative2AbsolutePath(lpszRegistry, szFullPath, MAX_PATH);
					TrackRegistry(szFullPath, lpszRealRegistry, nCharCount);
				}
				else
				{
					TrackRegistry(lpszRegistry, lpszRealRegistry, nCharCount);
				}
			}
		}

	   /**
	    * @brief ���� Redirect ��תָ��׷����������ע����ļ�
	    * @param lpszSource ��Դ��ע����ļ�
		* @param lpszDest ���׷�ٵ���ע���·������ lpszDest ��, Ϊ����·��
		* @param nCharCount lpszDest ������(��λΪ�ַ�����)
	    */
		void TrackRegistry(LPCTSTR lpszSource, LPTSTR lpszDest, int nCharCount)
		{
			TCHAR szRedirect[MAX_PATH] = {0};
			
			GetPrivateProfileString(TEXT("Redirect"), TEXT("Path"), NULL, szRedirect,
				MAX_PATH, lpszSource);
			
			if(!IsStringEmpty(szRedirect))
			{
				if(IsRelativePath(szRedirect))
				{
					CombineRelativePath(lpszSource, szRedirect, lpszDest, nCharCount);
				}
				else
				{
					TrackRegistry(szRedirect, lpszDest, nCharCount);
				}
			}
			else
			{
				StringCopy(lpszDest, nCharCount, lpszSource);
			}
		}
		
	   /**
	    * @brief �ж��ַ����Ƿ�Ϊ��, �ո��TAB�����������ַ�, ����ַ������ɿ�
		*        �ַ����, ����Ϊ����ַ����ǿ��ַ���
		* @param lpszString ��Ҫ�����жϵ��ַ���
		* @return ����ж�Ϊ���ַ��������� TRUE�����򷵻�FALSE
	    */
		BOOL IsStringEmpty(LPCTSTR lpszString)
		{
			BOOL bIsEmpty = TRUE;

			if(lpszString != NULL)
			{
				while(*lpszString != TEXT('\0'))
				{
					if(*lpszString != TEXT(' ') && *lpszString != TEXT('\t'))
					{
						bIsEmpty = FALSE;

						break;
					}

					lpszString++;
				}
			}

			return bIsEmpty;
		}

	   /**
	    * @brief �����ļ�B����ļ�A�����·����������ļ�B�ľ���·��
		* @param lpszOriginFile �ļ�A��������
		* @param lpszRelativePath �ļ�B�����·��(�����A�����·��)
		* @param lpszCombinedPath ���ڽ��ս���Ļ��������ļ�B�ľ���·��
		* @param nCharCount lpszCombinedPath������(��λΪ�ַ�����)
		*/
		BOOL CombineRelativePath(LPCTSTR lpszOriginFile, LPCTSTR lpszRelativePath,
			LPTSTR lpszCombinedPath, int nCharCount)
		{
			BOOL bSuccess = FALSE;
			
			BOOL bParamOk = (lpszOriginFile != NULL && lpszRelativePath != NULL &&
				lpszCombinedPath != NULL && nCharCount > 0);
			
			if(bParamOk)
			{
				LPCTSTR lpszSeparator = FindRightSideSeparator(lpszOriginFile);
				
				lpszCombinedPath[0] = TEXT('\0');
				
				if(lpszSeparator != NULL)
				{
					int nPreCount = int(lpszSeparator - lpszOriginFile + 1);
					
					StringCopyFix(lpszCombinedPath, nCharCount, lpszOriginFile,
						nPreCount);
				}
				else
				{
					StringCopy(lpszCombinedPath, nCharCount, lpszOriginFile);
					StringCat(lpszCombinedPath, nCharCount, TEXT("\\"));
				}
				
				StringCat(lpszCombinedPath, nCharCount, lpszRelativePath);
				
				bSuccess = TRUE;
			}
			
			return bSuccess;
		}

	   /**
	    * @brief ��Դ�ַ����п���ָ���������ַ���ΪĿ���ַ���
		* @param lpszDest ���ڴ��Ŀ���ַ����Ļ�����
		* @param nMaxCharCount lpszDest ������(��λΪ�ַ�����)
		* @param lpszSrc Դ�ַ���
		* @param nToCopyCharCount ��Ҫ�������ַ�����
		*/
		BOOL StringCopyFix(LPTSTR lpszDest, int	nMaxCharCount, LPCTSTR lpszSrc,
			int	nToCopyCharCount)
		{
			BOOL bSuccess = FALSE;
			
			if(lpszDest != NULL && nToCopyCharCount > 0 && 
			   lpszSrc  != NULL && nMaxCharCount > nToCopyCharCount)
			{
				_tcsncpy_s(lpszDest, nMaxCharCount, lpszSrc, nToCopyCharCount);

				lpszDest[nToCopyCharCount] = TEXT('\0');
				
				bSuccess = TRUE;
			}
			
			return bSuccess;
		}

	   /**
	    * @brief ƴ���ַ�������lpszSourceƴ����lpszDest����
		* @param lpszDest Ŀ���ַ����������ṩ�㹻�Ŀռ������� lpszSource ƴ���ں��棬���򱾺�������FALSE
		* @param nCharCount lpszDest ���������(��λΪ�ַ�����)
		* @param lpszSource ��Ҫ��ƴ�ӵ�������ַ���
		*/
		BOOL StringCat(LPTSTR lpszDest, int nCharCount, LPCTSTR lpszSource)
		{
			BOOL bSuccess = FALSE;
			
			if(lpszDest != NULL && nCharCount > 0 && lpszSource != NULL)
			{
				size_t nCapability = nCharCount;
				
				size_t nDestLen   = _tcslen(lpszDest);
				size_t nSourceLen = _tcslen(lpszSource);
				
				if(nSourceLen + nDestLen < nCapability)
				{
					_tcscat_s(lpszDest, nCapability, lpszSource);

					bSuccess = TRUE;
				}
			}
			
			return bSuccess;
		}
		
	   /**
	    * @brief �ַ�������
		* @param lpszDest Ŀ�Ļ�����
		* @param nMaxCharCount lpszDest ���������
		* @param lpszSrc ��Ҫ��������Դ�ַ���
		*/
		BOOL StringCopy(LPTSTR lpszDest, int nMaxCharCount, LPCTSTR lpszSrc)
		{
			BOOL bSuccess = FALSE;
			
			if(lpszDest != NULL && nMaxCharCount > 0 && lpszSrc != NULL)
			{
				if(StringCheck(lpszSrc, nMaxCharCount))
				{
					_tcscpy_s(lpszDest, nMaxCharCount, lpszSrc);
					bSuccess = TRUE;
				}
			}
			
			return bSuccess;
		}
		
	   /**
	    * @brief ����ַ����ĳ����Ƿ�С��ָ������󳤶�(��λ���ַ�����)
		* @param lpszString �������ַ���
		* @param nMaxCharCount ��󳤶�
		* @return Ŀ���ַ���С����󳤶��򷵻� TRUE�����򷵻� FALSE
		*/
		BOOL StringCheck(LPCTSTR lpszString, size_t nMaxCharCount)
		{
			BOOL bValid = FALSE;
			
			if(lpszString != NULL && (size_t)_tcslen(lpszString) < nMaxCharCount)
			{
				bValid = TRUE;
			}
			
			return bValid;
		}
		
	   /**
	    *
		*
		*
		*/
		BOOL Relative2AbsolutePath(LPCTSTR lpszRelativePath, LPTSTR	lpszOutput,
			int	nCharCount)
		{
			BOOL bSuccess = FALSE;
			
			if(lpszRelativePath != NULL && lpszOutput != NULL && nCharCount > 1)
			{
				bSuccess = TRUE;

				_sntprintf_s(lpszOutput, nCharCount, nCharCount - 1,
					TEXT("%s\\%s"), GetRootPath(), lpszRelativePath);

				lpszOutput[nCharCount - 1] = TEXT('\0');
			}
			
			return bSuccess;
		}
		
	   /**
	    * @brief ��ȡ��ǰ���������ļ��е�·��, ����������'\\'
		* @return ����ָ��·����ָ��
		*/
		LPCTSTR GetRootPath()
		{
			static TCHAR s_szPath[MAX_PATH] = {0};
			
			if(s_szPath[0] == 0)
			{
				HMODULE hModule = ::GetModuleHandle(NULL);
				
				GetModuleFileName(hModule, s_szPath, MAX_PATH);
				
				TCHAR* pTemp = _tcsrchr(s_szPath, TEXT('\\'));
				
				if(pTemp != NULL)
				{
					pTemp[0] = TEXT('\0');
				}
			}
			
			return s_szPath;
		}
		
	   /**
	    * @brief �������һ��·���ָ�������λ��
		* @param lpszPath ��Ҫ���в��ҵ�·���ַ���
		* @return ����ָ�����һ��·���ָ�����ָ��
		*/
		LPCTSTR FindRightSideSeparator(LPCTSTR lpszPath)
		{
			LPCTSTR lpszSeparator = _tcsrchr(lpszPath, TEXT('\\'));
			
			if(lpszSeparator == NULL)
			{
				lpszSeparator = _tcsrchr(lpszPath, TEXT('/'));
			}
			
			return lpszSeparator;
		}
		
	   /**
	    * @brief �ж��Ƿ����·��
		* @param lpszPath ��Ҫ�����жϵ�·��
		* @return ��������·���򷵻�TRUE�����򷵻�FALSE��
		*/
		BOOL IsRelativePath(LPCTSTR lpszPath)
		{
			BOOL bResult = TRUE;
			
			if(lpszPath != NULL)
			{
				if(_tcschr(lpszPath, TEXT(':')) != NULL)
				{
					bResult = FALSE;
				}
				else if(lpszPath[0] == TEXT('\\'))
				{
					bResult = FALSE;
				}
			}
			
			return bResult;
		}
	
	   /**
		* @brief ʹ�� OutputDebugString �����Ϣ�������� printf һ��
		* @param lpszFormat ��ʽ���ַ�������printfһ����
		*/
		void TTenio_Trace(LPCTSTR lpszFormat, ...)
		{
			TCHAR szMsg[1024] = {0};
			va_list pArg = NULL;
			
			va_start(pArg, lpszFormat);
			_vsntprintf_s(szMsg, _countof(szMsg), _countof(szMsg) - 1, lpszFormat, pArg);
			va_end(pArg);
			
			OutputDebugString(szMsg);
		}
#endif
	};
//#pragma warning(pop)

};

#endif