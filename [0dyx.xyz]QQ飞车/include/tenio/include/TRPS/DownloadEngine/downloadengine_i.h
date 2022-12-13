#pragma once

#include "../../teniointerface.h"
#include "TRPS/ResourceStorage/resourcestorage_i.h"
#include "TaskManager/taskmanager_i.h"
#include "TRPS/Downloader/downloaderdef.h"
/*#include "VersionCenter_i.h"*/
#include "downloadenginedef.h"

namespace Tenio
{
	class IDownloadEngine;

	TENIO_MAKE_TYPEID_MAP(IDownloadEngine, COMPONENTID_DOWNLOADENGINE);

	/**
	 * @ingroup network
	 * @brief 下载引擎
	 *
	 */
	class IDownloadEngine : public IComponent, public IEventProducer
	{
	public:
		/**
		 * @brief 获取资源，传入的可以是文件列表
		 * @param PVERSIONRESOURCELIST : [in] 下载文件列表, 路径表示为正斜杠形式。如：test/1.zip
		 * @param dwMilliSeconds [in] 超时时长，单位：毫秒。0表示使用默认值
		 * @return  下载任务号。－1表示下载启动失败; 
		 * @see 结构体定义:PVERSIONRESOURCELIST (即: tagVERSIONRESOURCELIST)
		 */
		virtual int Download(PVERSIONRESOURCELIST pVersionResourceList, DWORD dwMilliSeconds) = 0;

		/**
		 * @brief 下载一个文件
		 * @param lpszDownloadType : [in] 下载类型
		 * @param lpszFileName : [in] 下载文件名称，不含路径名
		 * @param nUpdateVersion : [in] 下载文件的服务器端版本号, -1表示不进行版本控制
		 * @param dwMilliSeconds [in] 超时时长，单位：毫秒。0表示使用默认值
		 * @return  下载任务号。－1表示下载启动失败; 
		 */
		virtual int Download(
			LPTSTR lpszDownloadType, 
			LPTSTR lpszFileName, 
			int    nUpdateVersion,
			DWORD  dwMilliSeconds,
			DWORD  dwPriority) = 0;

		/**
		 * @brief 设置优先级
		 * @param nTaskID : [in] 任务号
		 * @param nPeriority : [in] 优先级
		 */
		virtual void SetPriority(int nTaskID, int nPeriority) = 0;

		/**
		 * @brief 提升优先级
		 * @param nTaskID : [in] 任务号
		 * @param nPeriority : [in] 优先级提升的跨度
		 */
		virtual void RaisePriorityBy(int nTaskID, int nPeriority) = 0;

		/**
		 * @brief 设置某下载类型的下载配置解析器，否则该下载类型使用默认的下载配置解析器
		 * @param lpszDownloadType : [in] 下载类型
		 * @param pDownloadConfig : [in] 下载配置解析器指针
		 */
		virtual void AddConfig(LPCTSTR lpszDownloadType, IDownloadConfig* pDownloadConfig) = 0;

		/**
		 * @brief 删除某下载类型的下载配置解析器
		 * @param lpszDownloadType : [in] 下载类型
		 */
		virtual void RemoveConfig(LPCTSTR lpszDownloadType) = 0;

		/**
		 * @brief 设置某命令的解析器
		 * @param lpszCommandType : [in] 命令名称
		 * @param pCommandInterpreter : [in] 命令解析器指针
		 */
		virtual void AddCommandInterpreter(
			LPCTSTR lpszCommandType, 
			ICommandInterpreter* pCommandInterpreter) = 0;

	   /**
		 * @brief 删除命令的解析器
		 * @param lpszCommandType : [in] 命令名称
		 */
		virtual void RemoveCommandInterpreter(
			LPCTSTR lpszCommandType) = 0;


		/**
		 * @brief 设置默认代理服务器
		 * @param pSock5ProxyParam : [in] 代理服务器参数
		 */
		virtual void SetDefaultProxy(SOCK5PROXYPARAM * pSock5ProxyParam) = 0;

		/**
		 * @brief 设置总带宽
		 * @param dwBandWidth : [in] 带宽
		 */
		virtual void SetTotalBandWidth(DWORD dwBandWidth) = 0;

		/**
		 * @brief 取消下载
		 * @param nTaskID : [in] 下载任务号
		 */
		virtual BOOL CancelTask(int nTaskID) = 0;

		/**
		 * @brief 检查是否有未完成的更新任务，如果有，则进行更新
		 * 该操作主要发生在文件下载成功后，无法执行覆盖操作
		 * 当主程序重启时，调用该函数进行更新
		 */
		virtual int CheckUpdate() = 0;
	};


	/**
	 * @ingroup network
	 * @brief IDownloadEngine 的事件回调接口
	 * @see IDownloadEngine
	 */
	class IDownloadAndUpdateEvent : public IEvent
	{
	public:
		/**
		 * @brief 下载成功
		 * @param nTaskID [in] 下载任务号
		 * @param pVersionResourceList [in] 下载文件列表
		 */
		virtual void OnDownloadSuccess(int nTaskID, PVERSIONRESOURCELIST pVersionResourceList) = 0;

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

		/**
		 * @brief 文件更新成功，只有调用CheckUpdate()时，才可能触发该事件
		 * @param PUPDATEFILELIST [in] 更新成功的文件列表
		 */
		virtual void OnUpdateSuccess(PVERSIONRESOURCELIST pVersionResourceList) = 0;

		/**
		 * @brief 文件更新失败，只有调用CheckUpdate()时，才可能触发该事件
		 * @param nErrorID [in] 错误代码
		 * @param pUpdateFileList [in] 更新成功的文件列表
		 */
		virtual void OnUpdateFail(int nErrorID, PVERSIONRESOURCELIST pVersionResourceList) = 0;
	};	
};


// InterfaceName:DownloadEngine
// SubFolderName:DownloadEngine
