#ifndef __STRATEGYMANAGER_I_H__
#define __STRATEGYMANAGER_I_H__

#include "../../teniointerface.h"
#include "TRPS/ResourceStorage/resourcestorage_i.h"

namespace Tenio
{
	class IStrategyManager;

	TENIO_MAKE_TYPEID_MAP(IStrategyManager, COMPONENTID_STRATEGYMANAGER);

	/**
	 * @brief 策略管理器
	 *
	 * @see
	 */
	class IStrategyManager : public IComponent
	{
	public:
		/**
		 * @brief 注册配置器
		 *
		 * @param lpszDownloadType [in] 下载类型
		 * @param pDownloadConfig [in]  配置器
		 * @see
		 */
		virtual void AddDownloadConfig(LPCTSTR lpszDownloadType, IDownloadConfig* pDownloadConfig) = 0;

		/**
		 * @brief 注销配置器
		 *
		 * @param lpszDownloadType [in] 下载类型
		 * @see
		 */
		virtual void RemoveDownloadConfig(LPCTSTR lpszDownloadType) = 0;

		/**
		 * @brief 获取配置器
		 *
		 * @param lpszDownloadType [in] 下载类型
		 * @param ppDownloadConfig [out]  配置器
		 * @see
		 */
		virtual BOOL GetDownloadConfig(LPCTSTR lpszDownloadType, IDownloadConfig** ppDownloadConfig) = 0;

		/**
		 * @brief 获取下载器ID
		 * 
		 * @param lpszDownloadName [in] 下载类型
		 * @see
		 */
		virtual DWORD  GetDownloadID(LPCTSTR lpszDownloadName) = 0;
	};
};

#endif

// InterfaceName:StrategyManager
// SubFolderName:DownloadEngine
