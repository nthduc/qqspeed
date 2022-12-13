#ifndef __DOWNLOADOBJECTPOOL_I_H__
#define __DOWNLOADOBJECTPOOL_I_H__

#include "../../teniointerface.h"

namespace Tenio
{
	class IDownloadObjectPool;

	TENIO_MAKE_TYPEID_MAP(IDownloadObjectPool, COMPONENTID_DOWNLOADOBJECTPOOL);

	class IDownloadObjectPool : public IComponent
	{
	public:
		/**
		 * @ingroup 获取下载器对象
		 * @brief 
		 *
		 * <p>
		 * </p>
		 * @param
		 * @return
		 * @see
		 */
		virtual BOOL GetObject(IDownloader ** ppDownloader, DWORD dwComponentID) = 0;

		/**
		 * @ingroup 释放下载器对象
		 * @brief
		 *
		 * <p>
		 * </p>
		 * @param
		 * @return
		 * @see
		 */
		virtual BOOL ReleaseObject(IDownloader ** ppDownloader) = 0;
	};
};

#endif

// InterfaceName:DownloadObjectPool
// SubFolderName:Trps\Downloader
