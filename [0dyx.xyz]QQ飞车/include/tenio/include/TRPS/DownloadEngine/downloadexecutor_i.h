#ifndef __DOWNLOADEXECUTOR_I_H__
#define __DOWNLOADEXECUTOR_I_H__

#include "../teniointerface.h"

namespace Tenio
{
	class IDownloadExecutor;

	TENIO_MAKE_TYPEID_MAP(IDownloadExecutor, COMPONENTID_DOWNLOADEXECUTOR);

	class IDownloadExecutor : public IComponent
	{
	public:		
		/**
		 * @brief 设置下载配置器
		 * @param pDownloadConfig [in] 下载配置器指针
		 * @see IDownloadConfig
		 */
		virtual void SetConfig(LPTSTR lpszDownloadType, IDownloadConfig* pDownloadConfig) = 0;
		
		/**
		 * @brief 设置下载文件列表
		 * @param pRequestFileList [in] 下载文件列表指针
		 * @see 结构体定义: PREQUESTFILELIST (即: tagRequestFileList)
		 */
		virtual void SetResourceList(LPTSTR lpszDownloadType, PRESOURCELIST pResourceList, DWORD dwMilliSeconds) = 0;
		/**
		 * @brief 设置超时时长
		 * @param dwMilliSeconds [in] 超时时长，单位：毫秒。0表示使用默认值
		 */
		//virtual void SetTimeOut(DWORD dwMilliSeconds) = 0;

		/**
		 * @brief 设置版本控制器
		 * @param pVersionControler [in] 版本控制器指针
		 * @see IVersionControler
		 */
	//	virtual void SetVersionControler(IVersionControler * pVersionControler) = 0;

		virtual void SetDefaultProxy(SOCK5PROXYPARAM * pSock5ProxyParam) = 0;
		
		virtual void SetTotalBandWidth(DWORD dwBandWidth) = 0;
	};
};

#endif

// InterfaceName:DownloadExecutor
// SubFolderName:DownloadEngine
