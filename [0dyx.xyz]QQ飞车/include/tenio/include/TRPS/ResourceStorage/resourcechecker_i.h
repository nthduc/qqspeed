#ifndef __RESOURCECHECKER_I_H__
#define __RESOURCECHECKER_I_H__

#include "../../teniointerface.h"
#include "resourcestorage_i.h"
namespace Tenio
{
	class IResourceChecker;

	#define ERROR_RESOURCECHECKER_SYNCDOWNLOADERROR						(0x00000001)
	#define ERROR_RESOURCECHECKER_READFILEERROR						(0x00000002)
	
	TENIO_MAKE_TYPEID_MAP(IResourceChecker, COMPONENTID_RESOURCECHECKER);
	/**
	 * @ingroup network
	 * @brief 检查版本描述文件中记录的每个文件，若不是最新版本则下载
	 *        注：版本描述文件默认是"config\\UpdateVersionFile.xml"
	 *
	 * <p>
	 * Strategy :
	 * 1、读出版本描述文件中的每个文件，比较版本号。如果文件不是最新版本，则下载。
	 * 2、每次最多下载20个文件。
	 * 3、每个文件之间没有联系。
	 * 4、不能同时下载多个文件列表。
	 * </p>
	 * @param
	 * @return
	 * @see
	 */
	class IResourceChecker : public IComponent, public IEventProducer
	{
	public:
		/**
		 * @brief 根据版本描述文件的记录获取资源
		 *
		 * @param pResourceStorage : [in] 资源获取仓库, 由外部生成并传入，一般为全局唯一的对象。
		 *
		 * @return  下载任务号。－1表示下载失败, 0表示所有文件已存在且是最新版本。
		 *          1表示下载在异步进行，下载结果通过回调事件通知。
		 *
		 * @see IResourceStorage
		 */
		virtual int CheckAndGetResource(IResourceStorage* pResourceStorage) = 0;

		/**
		 * @ingroup 将资源获取仓库置为空
		 * @brief 1、该函数可取消下载。
		 *		  2、当外部资源获取仓库析构之前，通知本对象资源获取仓库无效
		 *
		 */
		virtual void ResetResourceStorage() = 0;
	};

	/**
	 * @ingroup network
	 * @brief IResourceStorage 的事件回调接口
	 * @see IResourceStorage   
	 */
	class IResourceCheckerEvent : public IEvent
	{
	public:		
		
		virtual void OnGetResourceSuccess() = 0;
		virtual void OnGetResourceFail(int nErrorCode) = 0;
	};
	
};

#endif

// InterfaceName:ResourceChecker
// SubFolderName:TRPS\ResourceStorage
