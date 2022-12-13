#ifndef __DOWNLOADEXECUTOR_I_H__
#define __DOWNLOADEXECUTOR_I_H__

#include "../teniointerface.h"

namespace Tenio
{
	class IDownloadExecutor;

	TENIO_MAKE_TYPEID_MAP(IDownloadExecutor, COMPONENTID_DOWNLOADEXECUTOR);

	class IDownloadExecutor : public IComponent
	{
	public:		
		/**
		 * @brief ��������������
		 * @param pDownloadConfig [in] ����������ָ��
		 * @see IDownloadConfig
		 */
		virtual void SetConfig(LPTSTR lpszDownloadType, IDownloadConfig* pDownloadConfig) = 0;
		
		/**
		 * @brief ���������ļ��б�
		 * @param pRequestFileList [in] �����ļ��б�ָ��
		 * @see �ṹ�嶨��: PREQUESTFILELIST (��: tagRequestFileList)
		 */
		virtual void SetResourceList(LPTSTR lpszDownloadType, PRESOURCELIST pResourceList, DWORD dwMilliSeconds) = 0;
		/**
		 * @brief ���ó�ʱʱ��
		 * @param dwMilliSeconds [in] ��ʱʱ������λ�����롣0��ʾʹ��Ĭ��ֵ
		 */
		//virtual void SetTimeOut(DWORD dwMilliSeconds) = 0;

		/**
		 * @brief ���ð汾������
		 * @param pVersionControler [in] �汾������ָ��
		 * @see IVersionControler
		 */
	//	virtual void SetVersionControler(IVersionControler * pVersionControler) = 0;

		virtual void SetDefaultProxy(SOCK5PROXYPARAM * pSock5ProxyParam) = 0;
		
		virtual void SetTotalBandWidth(DWORD dwBandWidth) = 0;
	};
};

#endif

// InterfaceName:DownloadExecutor
// SubFolderName:DownloadEngine
