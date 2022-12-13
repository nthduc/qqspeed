#pragma once

#include "../../teniointerface.h"
#include "../../event_i.h"
#include "TRPS/Downloader/downloaderdef.h"
#include "TaskManager/taskmanager_i.h"
//#include "objecttype2idmap.h"
//#include "../DownloadProxy/DownloadDef.h"

namespace Tenio
{        		
	class IDownloader;
	class IDownloaderEvent;
	class IIPGroupDownloader;
	class IBaseIPGroupDownloader;
	class IHttpDownloader;
	class IP2PDownloader;
	class IDownloadControler;
	class IDownloadCommand;
	class IInstallDownloadLsp;
	class IDownloadLspManager;
	
	TENIO_MAKE_TYPEID_MAP(IIPGroupDownloader, COMPONENTID_IPGROUPDOWNLOADER);
	TENIO_MAKE_TYPEID_MAP(IBaseIPGroupDownloader, COMPONENTID_BASEIPGROUPDOWNLOADER);
	TENIO_MAKE_TYPEID_MAP(IInstallDownloadLsp, COMPONENTID_INSTALLDOWNLOADLSP);
	//	TENIO_MAKE_TYPEID_MAP(IDownloadLspManager, COMPONENTID_IPGROUPDOWNLOADER);
	TENIO_MAKE_TYPEID_MAP(IHttpDownloader, COMPONENTID_HTTPDOWNLOADER);
	TENIO_MAKE_TYPEID_MAP(IP2PDownloader, COMPONENTID_P2PDOWNLOADER);
	TENIO_MAKE_TYPEID_MAP(IDownloadControler, COMPONENTID_DOWNLOADCONTROLER);
	TENIO_MAKE_TYPEID_MAP(IDownloadCommand, COMPONENTID_DOWNLOADCOMMAND);
    
	/**
	* @ingroup network
	* @brief 底层下载模块
	*
	* <p>
	*     IDownloader 是各种下载器组件的父类， IDownloader 有多个子类，分别代表不同类型的
	*     下载器组件，如 IIPGroupDownloader，IHttpDownloader，IP2PDownloader，分别代表IP群下载器
	*     组件，HTTP协议下载器和P2P协议下载器。
	*     不能直接创建 IDownloader 类型的组件，只能创建各子类型下载器组件。
	* </p>
	*
	* <p>
	* IDownloader 的子类包括:
	* <ol>
	*   <li>IIPGroupDownloader － 被指定多个下载服务器的下载器。</li>
	*   <li>IHttpDownloader － 使用HTTP协议的下载器。</li>
	*   <li>IP2PDownloader － 使用P2P协议的下载器。</li>
	* </ol>
	* </p>
	*
	* <p>
	* 使用流程
	* <ol>
	*   <li>选择一种子类型的 Downloader 进行创建</li>
	*   <li>使用 AddEventHandler() 函数传入回调事件指针</li>
	*   <li>使用 Init() 设置下载参数</li>
	*   <li>使用 Execute() 启动下载</li>
	*   <li>在事件回调接口中收到 IDownloaderEvent::OnDownloadSuccess() 消息，表示下载成功完成</li>
	*   <li>在事件回调接口中收到 IDownloaderEvent::OnDownloadError() 消息，表示下载失败</li>
	*   <li>在事件回调接口中收到 IDownloaderEvent::OnProgressChanged() 消息，表示下载正在进行</li>
	*   <li>正在下载时，可以使用 CancelDownload() 取消下载</li>
	*   <li>正在下载时，可以使用 SetBandWidth() 设置带宽</li>
	* </ol>
	* </p>
	*
	* @see IPGroupDownloader, HttpDownloader, P2PDownloader, IDownloaderEvent
	*/
	class IDownloader : public ITaskExecutor
	{
	public:
		
		/**
		* @brief 设置下载参数
		* @param pDownloadParam [in] 下载参数
		* @return 成功返回TRUE,失败返回FALSE
		*/
		virtual BOOL Init(void* pDownloadParam) = 0;
		
		/**
		* @brief 设置带宽
		* @param nBandWidth [in] 带宽
		* @return 成功返回TRUE,失败返回FALSE
		*/
		virtual BOOL SetBandWidth(int nBandWidth) = 0;

		/**
		* @brief 获取带宽
		* @param nBandWidth [in] 带宽
		* @return 带宽
		*/
		virtual int GetBandWidth() = 0;
	};
	
	/**
	* @ingroup network
	* @brief IDownloader 的事件回调接口
	* @see IDownloader
	*/
	class IDownloaderEvent : public IEvent
	{
	public:		
		/**
		* @brief 下载成功
		* @param nTaskID [in] 下载任务号
		*/
		virtual void OnDownloadSuccess(int nTaskID) = 0;
		
		/**
		* @brief 下载失败
		* @param nTaskID [in] 下载任务号
		* @param nErrorCode [in] 错误代码
		*/
		virtual void OnDownloadError(int nTaskID, int nErrorCode) = 0;
		
		/**
		* @brief 下载进度发生变化
		* @param nTaskID [in] 下载任务号
		* @param dwFileSize [in]下载文件的总长度，单位(字节)
		* @param dwDownloadedSize [in] 已下载大小，单位(字节)
		*/
		virtual void OnProgressChanged(int nTaskID, DWORD dwFileSize, DWORD dwDownloadedSize) = 0;
		
		// 因为重定向之后的Server端路径可能与原来的不同，所以重定向不在IPGroup Downloader层处理，
		// 放在HTTP Downloader层
		//virtual void OnRedirect(int nTaskID, URLARRAY * lpUrlArray) = 0;
	};
    
	/**
	* @ingroup network
	* @brief IP群组下载器: 
	*
	* <p>
	* 下载一个文件时, 首先会查询P2P配置文件，如果找到
	* 该文件名对应的TPT文件，则启动P2P下载；否则
	* 启动HTTP下载。HTTP下载会将所有IP试一遍，直到下
	* 载成功为止。如果全部IP试过一遍之后，还是下载失
	* 败，则本次下载任务失败。
	* </p>
	*
	* 使用方法请参考 IDownloader
	*/
	class IIPGroupDownloader : public IDownloader/*, public IEventImpl<IDownloaderEvent, 1>*/
	{
	public:	
		
	};
	
// 	class IBaseIPGroupDownloader : public IIPGroupDownloader
// 	{
// 	public:
// 		virtual int GetBandWidth() = 0;
// 	};
    
	/**
	* @ingroup network
	* @brief HTTP下载器 
	*
	* <p>
	* 使用HTTP协议下载
	* 重定向只随机使用一个IP
	* </p>
	*
	* 使用方法请参考 IDownloader
	*/
	class IHttpDownloader : public IDownloader
	{
	public:
		
	};
    
	/**
	* @ingroup network
	* @brief P2P下载器 
	*
	* <p>
	* 同时使用P2P协议和HTTP协议下载。当文件比较小时，使用HTTP协议下载效率比较高，大部分文件内容通过HTTP协议下载；
	* 当文件比较大时，使用P2P协议下载效率比较高，大部分文件内容通过P2P协议下载。
	* </p>
	*
	* 使用方法请参考 IDownloader
	*/
	class IP2PDownloader : public IDownloader
	{
	public:
		
	};   
		
// 	class ICommand : public IEventProducer, public IComponent
// 	{   
// 	public:
// 		virtual BOOL Execute() = 0;
// 	};
//     
// 	class ICommandEvent : public IEvent
// 	{
// 	public:
//         virtual void OnCommandComplete(ICommand* pCommand, BOOL bSuccess) = 0;
// 	};
// 	
// 	class IDownloadCommand : public ICommand
// 	{
// 	public:
// 		virtual void Initialize(IDownloader* pDownloader) = 0;
// 	};
//     
//     class IDownloadControler : public IComponent
// 	{
// 	public:
// 		virtual void AddCommand(ICommand* pCommand) = 0;
// 	};
// 	
// 	class IInstallDownloadLsp : public IComponent
// 	{
// 	public:
// 		virtual BOOL InstallDownloadLsp(IIPGroupDownloader* pIPGroupDownloader) = 0;
// 	};
	
	// 	class IDownloadLspManager : public IIPGroupDownloader, public IInstallDownloadLsp
	// 	{
	// 
	// 	};
	
};
