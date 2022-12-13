#ifndef __CACHEMANAGER_I_H__
#define __CACHEMANAGER_I_H__

#include "../../teniointerface.h"
#include "TRPS/Downloader/downloaderdef.h"

namespace Tenio
{
	class ICacheManager;

	TENIO_MAKE_TYPEID_MAP(ICacheManager, COMPONENTID_CACHEMANAGER);

	/**
	 * @ingroup network
	 * @brief ���������
	 *
	 * @see
	 */
	class ICacheManager : public IComponent
	{
	public:

		/**
		 * @brief ��黺���Ƿ����°汾
		 *
		 * @param pCacheInfo [in] ������Ϣ
		 * @return ����ʱ�ļ������°汾������ʱ������TRUE;���򣬷���FALSE
		 * @see
		 */
		virtual BOOL CheckCache(PCACHEINFO pCacheInfo) = 0;
		
		/** 
		 * @brief �����ļ���ʵ�ʴ�С(��Ӳ�̶��ļ�����)
		 *
		 * @param lpszFileName [in] �ļ���
		 * @return �ļ���С
		 */
		virtual DWORD GetCacheSize(LPCTSTR lpszFileName) = 0;

		/** 
		 * @brief ���û�����Ϣ
		 *
		 * @param pCacheInfo [in] ������Ϣ
		 * @return �ɹ�����TRUE,ʧ�ܷ���FALSE
		 */
		virtual BOOL  SetCacheInfo(PCACHEINFO pCacheInfo) = 0;

		/** 
		 * @brief ��ȡ������Ϣ
		 *        ������pCacheInfo�е�dwFileSizeΪ�����ļ��м�¼���ļ���С
		 *
		 * @param pCacheInfo [Out] ������Ϣ
		 * @return �ɹ�����TRUE,ʧ�ܷ���FALSE
		 */
		virtual BOOL  GetCacheInfo(PCACHEINFO pCacheInfo) = 0;

		/** 
		 * @brief ɾ��������Ϣ
		 *
		 * @param lpszFileName [in] �ļ���
		 * @return �ɹ�����TRUE,ʧ�ܷ���FALSE
		 */
		virtual BOOL DeleteCache(LPCTSTR lpszFileName) = 0;
	};
};

#endif

// InterfaceName:CacheManager
// SubFolderName:CacheManager
