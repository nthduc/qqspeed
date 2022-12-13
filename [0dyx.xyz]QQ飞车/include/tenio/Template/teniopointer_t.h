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
		 * @brief Tenio 组件智能指针
		 *
		 * @param lpszRegistry 指定 Tenio.ini 的路径, 可以是绝对路径或者相对路径.
		 *        如果为 NULL 则取默认值(进程exe所在目录)
		 *
		 * @param dwComponentId 指定需要创建的 ComponentID，如果为0，表示取接口类
		 *  	  型(即模板参数T)的默认实现组件ID
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
		 * @brief 在析构函数中删除本智能指针对应的组件
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
	    * @brief 获取 TenioComponentFactory 的指针
		* @param lpszRegistry 指定注册表路径
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
	    * @brief 从 hModule 中创建组件ID为 dwComponentId 的组件
		* @param hModule 提供组件的DLL的句柄
		* @param dwComponentId 组件 ID
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
	    * @brief 获取 TenFact.dll 的句柄
		* 
		* 首先在已加载模块中搜索TenFact.dll, 如果没有找到, 则按照 lpszRegistry
		* 文件标示的组件信息去加载 TenFact.dll。
		*
		* @param lpszRegistry 注册表文件绝对路径
		* @return 返回 TenFact.dll 的句柄
	    */
		HMODULE GetFactoryDLLHandle(LPCTSTR lpszRegistry)
		{
#ifdef TENIO_DLL_IMPLEMENT
			HMODULE hFact = g_hFactory;
#else
			HMODULE hFact = GetModuleHandle(FACTORY_MODULE);
#endif

			if(hFact == NULL) // TenFact.dll 还没有加载
			{
				TCHAR szFinalRegistry[MAX_PATH] = {0}; // 表示追踪到最后的注册表的路径
				GetRealRegistryFile(lpszRegistry, szFinalRegistry, MAX_PATH);

				TCHAR szTenFactDLLPath[MAX_PATH] = {0};
				GetTenioFactoryDLLPath(szFinalRegistry, szTenFactDLLPath, MAX_PATH);

				hFact = ::LoadLibrary(szTenFactDLLPath);
			}

			return hFact;
		}

	   /**
	    * @brief 从 lpszRegistry 注册表文件中提取 TenFact.dll 的绝对路径
	    * @param lpszRegistry Tenio 注册表文件，必须是绝对路径，注意这个文件必须
		*        包含真正的组件信息，不能使用 Redirect 指令跳转到其他注册表
		* @param lpszOutput 指向一块缓冲区，用于接收 TenFact.dll 的绝对路径
		* @param nCharCount lpszOutput 缓冲区的长度，单位是字符个数
	    */
		void GetTenioFactoryDLLPath(LPCTSTR lpszRegistry, LPTSTR lpszOutput,
			int nCharCount)
		{
			// 提取 ComponentFactory 的组件路径(组件ID是361)
			TCHAR szFactoryPath[MAX_PATH] = {0};
			ReadIniString(lpszRegistry, TEXT("361"), TEXT("DLL"), NULL, 
				szFactoryPath, MAX_PATH);

			if(!IsRelativePath(szFactoryPath))
			{
				// 如果是绝对路径, 直接把路径填入结果中
				StringCopy(lpszOutput, nCharCount, szFactoryPath);
			}
			else
			{
				TranslateComponentRelativePath(lpszRegistry, szFactoryPath,
					lpszOutput, nCharCount);
			}
		}

	   /**
		* @brief 根据组件DLL的相对路径计算出绝对路径，如果组件没有指定
		*		 "NotUseDefaultDllPath" 那么组件的路径首先与[DefaultValue]-DllPath 
		*		 相结合. 然后参照 lpszRegistryPath所在目录，计算出绝对路径
		*
		* @param lpszRegistryPath 注册表文件的绝对路径
		* @param lpszComponentDLLPath 组件DLL的相对路径
		* @param lpszOutput 接收结果的缓冲区
		* @param nCharCount 缓冲区的容量(单位为字符个数)
		*/
		inline void TranslateComponentRelativePath(LPCTSTR lpszRegistryPath,
			LPCTSTR lpszComponentDLLPath, LPTSTR lpszOutput, int nCharCount)
		{
			// 判断 Factory 组件是否会使用 DefaultDllPath
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
				// 提取 [DefaultValue] - DllPath
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
	    * @brief 以路径分隔符连接两个字符串, 如果lpszLeft为空字符串, 则只拷贝
		*        lpszRight 到 lpszOutput 中
		*
		* @param lpszLeft 第一字符串
		* @param lpszRight 第二字符串
		* @param lpszOutput 接收结果的缓冲区
		* @param nCharCount 缓冲区的容量(字符个数)
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
	    * @brief 从INI文件中读取内容, 自动删除两旁的空格和\t
		* @param lpszFile INI文件的绝对路径
		* @param lpszSectionName INI中的节名
		* @param lpszKeyName 节中的KeyName
		* @param lpszDefaultValue 如果没有找到正确的Key, 则把这个值填入到输出缓冲区中
		* @param lpszValue 用于接收Key的取值
		* @param nCharCount lpszValue的容量(字符个数)
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
	    * @brief 删除字符串两边的空白字符(空格/TAB)
		* @param lpszSource 需要进行操作的字符串对象
		* @param lpszDest 结果字符串存放的缓冲区
		* @param nCharCount lpszDest的容量(字符个数)
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

			// 首先把 lpszOutput 清空
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
	    * @brief 判断一个字符是否一个 White Char(空格或者TAB)
		* @param ch 需要进行判断的字符
		* @return 如果是White Char, 则返回 TRUE, 否则返回 FALSE
		*/
		inline BOOL IsWhiteChar(const TCHAR& ch)
		{
			return ch == TEXT(' ') || ch == TEXT('\t');
		}

	   /**
	    * @brief 获取真正的注册表文件
		*
		* 如果 lpszRegistry 字符串为空(或者等于NULL),  则默认为进程根目录下面的
		* Tenio.ini 为注册表文件. 本函数跟踪 Redirect 命令,  一直追踪到真正的注
		* 册表文件, 把最后追踪到的注册表文件路径填入 lpszRealRegistry
		*
		* @param lpszRegistry 起源的注册表文件
		* @param lpszRealRegistry 最终输出结果
		* @param nCharCount lpszRealRegistry 的容量(单位为字符个数)
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
	    * @brief 根据 Redirect 跳转指令追踪最终最后的注册表文件
	    * @param lpszSource 起源的注册表文件
		* @param lpszDest 最后追踪到的注册表路径填入 lpszDest 中, 为绝对路径
		* @param nCharCount lpszDest 的容量(单位为字符个数)
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
	    * @brief 判断字符串是否为空, 空格和TAB都被视作空字符, 如果字符串是由空
		*        字符组成, 则认为这个字符串是空字符串
		* @param lpszString 需要进行判断的字符串
		* @return 如果判断为空字符串，返回 TRUE，否则返回FALSE
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
	    * @brief 根据文件B相对文件A的相对路径，计算出文件B的绝对路径
		* @param lpszOriginFile 文件A，参照物
		* @param lpszRelativePath 文件B的相对路径(相对于A的相对路径)
		* @param lpszCombinedPath 用于接收结果的缓冲区，文件B的绝对路径
		* @param nCharCount lpszCombinedPath的容量(单位为字符个数)
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
	    * @brief 从源字符串中拷贝指定个数的字符成为目的字符串
		* @param lpszDest 用于存放目的字符串的缓冲区
		* @param nMaxCharCount lpszDest 的容量(单位为字符个数)
		* @param lpszSrc 源字符串
		* @param nToCopyCharCount 需要拷贝的字符个数
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
	    * @brief 拼接字符串，把lpszSource拼接在lpszDest后面
		* @param lpszDest 目的字符串，必须提供足够的空间以容纳 lpszSource 拼接在后面，否则本函数返回FALSE
		* @param nCharCount lpszDest 的最大容量(单位为字符个数)
		* @param lpszSource 需要被拼接到后面的字符串
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
	    * @brief 字符串拷贝
		* @param lpszDest 目的缓冲区
		* @param nMaxCharCount lpszDest 的最大容量
		* @param lpszSrc 将要被拷贝的源字符串
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
	    * @brief 检查字符串的长度是否小于指定的最大长度(单位是字符个数)
		* @param lpszString 被检查的字符串
		* @param nMaxCharCount 最大长度
		* @return 目标字符串小于最大长度则返回 TRUE，否则返回 FALSE
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
	    * @brief 获取当前进程所在文件夹的路径, 不包括最后的'\\'
		* @return 返回指向路径的指针
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
	    * @brief 查找最后一个路径分隔符所在位置
		* @param lpszPath 需要进行查找的路径字符串
		* @return 返回指向最后一个路径分隔符的指针
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
	    * @brief 判断是否相对路径
		* @param lpszPath 需要进行判断的路径
		* @return 如果是相对路径则返回TRUE，否则返回FALSE。
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
		* @brief 使用 OutputDebugString 输出信息，参数和 printf 一样
		* @param lpszFormat 格式化字符串，与printf一样。
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