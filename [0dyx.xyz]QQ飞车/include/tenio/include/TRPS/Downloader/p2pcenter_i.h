#ifndef __P2PCENTER_I_H__
#define __P2PCENTER_I_H__

#include "../../teniointerface.h"
#include "downloaderdef.h"

namespace Tenio
{
	class IP2PCenter;

	TENIO_MAKE_TYPEID_MAP(IP2PCenter, COMPONENTID_P2PCENTER);

	/**
	 * @ingroup
	 * @brief P2P下载中心，所有下载请求均发到这里处理
	 *        P2PCenter维护全局唯一的IVqqDownload, 为IVqqDownload提供唯一的回调窗口
	 *
	 * @see
	 */
	class IP2PCenter : public IComponent
	{
	public:
		
		/**
		 * @brief 添加任务
		 *
		 * @param pstP2PParam [in] P2P下载参数
		 * @param hNotifyWnd [in]  回调窗口
		 * @return 成功返回TRUE,失败返回FALSE
		 * @see
		 */
		virtual  BOOL AddTask(DOWNLOADP2PPARAM * pstP2PParam, HWND hNotifyWnd) = 0;

		/**
		 * @brief 停止下载
		 *
		 * @param unTaskID [in] 任务号
		 * @see
		 */
		virtual BOOL StopP2P(UINT unTaskID) = 0;

		/**
		 * @brief 删除任务
		 *
		 * @param unTaskID [in] 任务号
		 * @see
		 */
		virtual BOOL DelTask(UINT unTaskID) = 0;

		/**
		 * @brief 设置P2P下载速度
		 *
		 * <p>
		 * </p>
		 * @param
		 * @return
		 * @see
		 */
		virtual BOOL SetP2PDownloadSpeed(UINT unTaskID, UINT unSpeed) = 0;

		/**
		 * @brief 调整P2P上传速度
		 *
		 * @param unSpeed [in] 上传速度,单位(KB/s)
		 * @return 成功返回TRUE,失败返回FALSE
		 * @see
		 */
		virtual BOOL AdjustP2PUploadSpeed(UINT unSpeed) = 0;

		/**
		 * @brief 调整HTTP下载速度
		 *
		 * @param unSpeed [in] HTTP下载速度,单位(KB/s)
		 * @return 成功返回TRUE,失败返回FALSE
		 * @see
		 */
		virtual BOOL AdjustP2PHttpSpeed(UINT unSpeed) = 0;

	};

	/**
	 * @ingroup network
	 * @brief IP2PCenter 的事件回调接口
	 * @see IP2PCenter
	 */
	class IP2PCenterEvent : public IEvent
	{
	public:		
		/**
		 * @brief 下载成功
		 * @param unTaskID [in] 下载任务号
		 */
		virtual void OnGetFileSize(UINT unTaskID, DWORD dwFileSize) = 0;

		/**
		 * @brief 下载成功
		 * @param unTaskID [in] 下载任务号
		 */
		virtual void OnDownloadSuccess(UINT unTaskID) = 0;
		/**	
		 * @brief 下载失败
		 * @param unTaskID [in] 下载任务号
		 * @param nErrorCode [in] 错误代码
		 */
		virtual void OnDownloadError(UINT unTaskID, int nErrorCode) = 0;

		/**
		 * @brief 下载进度发生变化
		 * @param unTaskID [in] 下载任务号
		 * @param nPercent [in] 下载文件的进度，单位(1%)
		 */
		virtual void OnProgressChanged(UINT unTaskID, int nPercent) = 0;
	};
};

#endif

// InterfaceName:P2PCenter
// SubFolderName:Downloader

