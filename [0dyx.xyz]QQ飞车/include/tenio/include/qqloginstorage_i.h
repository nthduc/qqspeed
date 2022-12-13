#ifndef __QQLOGIN_STORAGE_I__
#define __QQLOGIN_STORAGE_I__

#include "teniointerface.h"
#include "qqloginstoragestructs.h"

namespace Tenio
{
	class IQQLoginStorage;

	TENIO_MAKE_TYPEID_MAP(IQQLoginStorage, COMPONENTID_QQLOGINSTORAGE)

	class IQQLoginStorage : public IComponent
	{
	public:
		/**
		 * @brief 指定一个文件夹路径, 本组件自动记录的配置文件将被写入到这个
		 *        文件夹中
		 *
		 * @param lpszFolderPath 文件夹路径(绝对或相对路径)
		 */
		virtual void SetCookieRootPath(LPCTSTR lpszFolderPath) = 0;

		virtual BOOL ReadLoginHistory(USERLOGINCONFIGLIST* pConfigList) = 0;

		virtual BOOL ReadLastLoginUIN(DWORD* pdwUIN) = 0;

		virtual BOOL ReadProxySetting(GLOBALNETWORKPROXYSETTING* pProxySetting) = 0;

		virtual BOOL DeleteUserLoginInfo(DWORD dwUIN) = 0;

		virtual BOOL AddLoginInfo(USERLOGINCONFIG* pConfigInfo) = 0;

		virtual BOOL WriteProxyInfo(GLOBALNETWORKPROXYSETTING* pProxySetting) = 0;
	};
};

#endif //__QQLOGIN_STORAGE_I__