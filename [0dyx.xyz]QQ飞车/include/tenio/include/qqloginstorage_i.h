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
		 * @brief ָ��һ���ļ���·��, ������Զ���¼�������ļ�����д�뵽���
		 *        �ļ�����
		 *
		 * @param lpszFolderPath �ļ���·��(���Ի����·��)
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