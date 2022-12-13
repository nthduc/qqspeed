
#pragma once

#include "TRPS/ResourceStorage/resourcestorage_i.h"
#include "TaskManager/taskmanager_i.h"
//#include "../DownloadProxy/DownloadProxyObject_i.h"
#include "VersionControler/versioncontroler_i.h"
//#include "../VersionDownload/versiondownload_i.h"

namespace Tenio
{	
   	/**
	 * @ingroup network
	 * @brief 批量下载器
	 *
	 * <p>
	 * 执行批量下载任务。如果一个文件下载失败，则整个下载任务失败，
	 * 所有文件保持不变，与下载前相同。
	 * </p>
	 *
	 * <p>
	 * 使用流程
	 * <ol>
	 *   <li>使用 AddEventHandler() 函数传入回调事件指针</li>
	 *   <li>使用 SetDownloadConfig() 设置下载参数</li>
	 *   <li>使用 SetResourceList() 设置下载文件列表</li>
	 *   <li>使用 SetTimeOut() 设置超时时长，否则使用默认超时时长</li>
	 *   <li>使用 SetVersionControler() 设置版本控制器</li>
	 *   <li>使用 Execute() 启动下载</li>
	 *   <li>在事件回调接口中收到 IDownloadExecutorEvent::OnDownloadSuccess() 消息，表示下载成功完成</li>
	 *   <li>在事件回调接口中收到 IDownloadExecutorEvent::OnDownloadFail() 消息，表示下载失败</li>
	 *   <li>正在下载时，可以使用 Stop() 停止下载</li>
	 * </ol>
	 * </p>
	 *
	 * @see ITaskExecutor, IDownloadExecutorEvent
	 */
	class IBatchDownloadExecutor : public ITaskExecutor
	{
	public:
		/**
		 * @brief  添加任务
		 * @param  pDownloadConfig [in] 配置器
		 * @param  dwDownloaderID [in]  下载器ID
		 * @param  dwPriority     [in]  优先级
		 * @param  nUpdateVersion [in]  文件版本号
		 *
		 * @return 成功返回TRUE,失败返回FALSE
		 * @see
		 */
		virtual BOOL AddTask(IDownloadConfig * pDownloadConfig, 
			DWORD dwDownloaderID, 
			DWORD dwPriority,
			int nUpdateVersion = -1) = 0;

		/**
		 * @brief  设置优先级
		 * @param  nPeriority     [in]  优先级
		 *
		 * @see
		 */
		virtual void SetPriority(int nPeriority) = 0;

		/**
		 * @brief  提升优先级
		 * @param  nPeriority     [in]  优先级提升的跨度
		 *
		 * @see
		 */
		virtual void RaisePriorityBy(int nPeriority) = 0;

		/**
		 * @brief  设置任务管理器
		 * @param  pTaskManager     [in]  任务管理器
		 *
		 * @see
		 */
		virtual BOOL SetTaskManager(ITaskManager* pTaskManager) = 0;
		
		/**
		 * @brief  设置带宽
		 * @param  nBandWidth     [in]  带宽
		 *
		 * @see
		 */
		virtual BOOL SetBandWidth(int nBandWidth) = 0;

		/**
		 * @brief  设置代理
		 * @param  pSock5ProxyParam   [in]  代理
		 *
		 * @see
		 */
		virtual void SetDefaultProxy(SOCK5PROXYPARAM * pSock5ProxyParam) = 0;
	};
	
	TENIO_MAKE_TYPEID_MAP(IBatchDownloadExecutor, COMPONENTID_BATCHDOWNLOADEXECUTOR)
	TENIO_MAKE_EVENT_MAP(IBatchDownloadExecutor, IDownloaderEvent)
		
};