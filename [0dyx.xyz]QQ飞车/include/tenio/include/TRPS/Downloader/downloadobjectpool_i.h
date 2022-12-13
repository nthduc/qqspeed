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
		 * @ingroup ��ȡ����������
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
		 * @ingroup �ͷ�����������
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
