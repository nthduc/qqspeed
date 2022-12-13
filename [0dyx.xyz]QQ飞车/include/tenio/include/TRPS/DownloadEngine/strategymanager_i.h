#ifndef __STRATEGYMANAGER_I_H__
#define __STRATEGYMANAGER_I_H__

#include "../../teniointerface.h"
#include "TRPS/ResourceStorage/resourcestorage_i.h"

namespace Tenio
{
	class IStrategyManager;

	TENIO_MAKE_TYPEID_MAP(IStrategyManager, COMPONENTID_STRATEGYMANAGER);

	/**
	 * @brief ���Թ�����
	 *
	 * @see
	 */
	class IStrategyManager : public IComponent
	{
	public:
		/**
		 * @brief ע��������
		 *
		 * @param lpszDownloadType [in] ��������
		 * @param pDownloadConfig [in]  ������
		 * @see
		 */
		virtual void AddDownloadConfig(LPCTSTR lpszDownloadType, IDownloadConfig* pDownloadConfig) = 0;

		/**
		 * @brief ע��������
		 *
		 * @param lpszDownloadType [in] ��������
		 * @see
		 */
		virtual void RemoveDownloadConfig(LPCTSTR lpszDownloadType) = 0;

		/**
		 * @brief ��ȡ������
		 *
		 * @param lpszDownloadType [in] ��������
		 * @param ppDownloadConfig [out]  ������
		 * @see
		 */
		virtual BOOL GetDownloadConfig(LPCTSTR lpszDownloadType, IDownloadConfig** ppDownloadConfig) = 0;

		/**
		 * @brief ��ȡ������ID
		 * 
		 * @param lpszDownloadName [in] ��������
		 * @see
		 */
		virtual DWORD  GetDownloadID(LPCTSTR lpszDownloadName) = 0;
	};
};

#endif

// InterfaceName:StrategyManager
// SubFolderName:DownloadEngine
