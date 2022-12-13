#ifndef __HTTPPROTOCOLHANDLER_I_H__
#define __HTTPPROTOCOLHANDLER_I_H__

#include "../../teniointerface.h"
#include "TRPS/Downloader/downloaderdef.h"

namespace Tenio
{
	class IHttpProtocolHandler;

	TENIO_MAKE_TYPEID_MAP(IHttpProtocolHandler, COMPONENTID_HTTPPROTOCOLHANDLER);
    
	/**
	 * @ingroup network
	 * @brief HTTP协议处理器
	 *
	 * <p>
	 * HTTP协议处理器 负责建立与SERVER端的连接，发送请求下载协议包和接收数据包，
	 * 实现超时机制，向本地硬盘写数据，以及进行文件的完整性校验。
	 * </p>
	 *
	 * <p>
	 * 使用流程
	 * <ol>
	 *   <li>使用 AddEventHandler() 函数传入回调事件指针</li>
	 *   <li>使用 SetBandWidth() 设置带宽，否则使用默认带宽</li>
	 *   <li>使用 SetProxy() 设置代理，否则无代理</li>
	 *   <li>使用 SetTimeOut() 设置超时时长，否则使用默认超时时长</li>
	 *   <li>使用 Download() 启动下载</li>
	 *   <li>在事件回调接口中收到 IHttpProtocolHandlerEvent::OnDownloadSuccess() 消息，表示下载成功完成</li>
	 *   <li>在事件回调接口中收到 IHttpProtocolHandlerEvent::OnDownloadError() 消息，表示下载失败</li>
	 *   <li>在事件回调接口中收到 IHttpProtocolHandlerEvent::OnProgressChanged() 消息，表示下载正在进行</li>
	 *   <li>正在下载时，可以使用 CancelDownload() 取消下载</li>
	 * </ol>
	 * </p>
	 *
	 * @see IHttpProtocolHandlerEvent
	 */
	class IHttpProtocolHandler : public IComponent, public IEventProducer
	{
	public:
		/**
		 * @brief 设置下载器带宽
		 * @param nBandWidth [in] 带宽
		 * @return 如果nBandWidth > 0，返回TRUE，否则返回FALSE
		 */
		virtual BOOL SetBandWidth(int nBandWidth) = 0;
		
		/**
		 * @brief 设置代理
		 * @param pSock5ProxyParam [in] 代理结构体指针
		 * @return  如果pSock5ProxyParam != NULL且代理IP不为空，
		 *          返回TRUE，否则返回FALSE
		 * @see 结构体定义:SOCK5PROXYPARAM, (即: tagProxyParam)
		 */
	    virtual BOOL SetProxy(SOCK5PROXYPARAM * pSock5ProxyParam) = 0;

		/**
		 * @brief 设置超时时长
		 * @param dwMilliSeconds [in] 超时时长，单位：毫秒。0表示使用默认值
		 */
		virtual void SetTimeOut(DWORD dwMilliSeconds) = 0;

		/**
		 * @brief 启动下载
		 * @param lpszFilename [in] 下载文件的URL
		 * @param lpszSaveFilename [in] 下载文件的完整保存路径
		 * @return 下载正常启动返回TRUE，否则返回FALSE
		 */
		virtual int Download(LPCTSTR lpszURL, LPCTSTR lpszSaveFilename, int nVersion) = 0;

		/**
		 * @brief 取消下载。不会再提示下载失败
		 * @return 成功返回TURE，失败返回FALSE
		 */
		virtual BOOL CancelDownload() = 0;
	};
    
	/**
	 * @ingroup network
	 * @brief IHttpProtocolHandler 的事件回调接口
	 * @sa IHttpProtocolHandler
	 */
	class IHttpProtocolHandlerEvent : public IEvent
	{
	public:
		/**
		 * @brief 下载成功
		 */
		virtual void OnDownloadSuccess() = 0;

		/**
		 * @brief 下载失败
		 * @param nErrorCode [in] 错误代码
		 */
		virtual void OnDownloadFail(int nErrorCode) = 0;

		/**
		 * @brief 下载进度发生变化
		 * @param dwFileSize [in] 下载文件的总长度，单位(字节)
		 * @param dwDownloadedSize [in] 已下载大小，单位(字节)
		 */
		virtual void OnProgressChanged(DWORD dwFileSize, DWORD dwDownloadedSize) = 0;

		virtual void OnRedirect(URLARRAY * lpUrlArray) = 0;
	};
};

#endif

// InterfaceName:HttpProtocolHandler
// SubFolderName:HttpProtocolHandler
