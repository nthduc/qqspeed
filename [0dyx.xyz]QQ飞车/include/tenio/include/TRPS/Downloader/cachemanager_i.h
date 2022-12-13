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
	 * @brief 缓存管理器
	 *
	 * @see
	 */
	class ICacheManager : public IComponent
	{
	public:

		/**
		 * @brief 检查缓存是否最新版本
		 *
		 * @param pCacheInfo [in] 缓存信息
		 * @return 当临时文件是最新版本且最新时，返回TRUE;否则，返回FALSE
		 * @see
		 */
		virtual BOOL CheckCache(PCACHEINFO pCacheInfo) = 0;
		
		/** 
		 * @brief 读出文件的实际大小(从硬盘读文件属性)
		 *
		 * @param lpszFileName [in] 文件名
		 * @return 文件大小
		 */
		virtual DWORD GetCacheSize(LPCTSTR lpszFileName) = 0;

		/** 
		 * @brief 设置缓存信息
		 *
		 * @param pCacheInfo [in] 缓存信息
		 * @return 成功返回TRUE,失败返回FALSE
		 */
		virtual BOOL  SetCacheInfo(PCACHEINFO pCacheInfo) = 0;

		/** 
		 * @brief 读取缓存信息
		 *        读出的pCacheInfo中的dwFileSize为配置文件中记录的文件大小
		 *
		 * @param pCacheInfo [Out] 缓存信息
		 * @return 成功返回TRUE,失败返回FALSE
		 */
		virtual BOOL  GetCacheInfo(PCACHEINFO pCacheInfo) = 0;

		/** 
		 * @brief 删除缓存信息
		 *
		 * @param lpszFileName [in] 文件名
		 * @return 成功返回TRUE,失败返回FALSE
		 */
		virtual BOOL DeleteCache(LPCTSTR lpszFileName) = 0;
	};
};

#endif

// InterfaceName:CacheManager
// SubFolderName:CacheManager
