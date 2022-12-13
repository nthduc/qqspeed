
#pragma once

#include "teniointerface.h"

namespace Tenio
{  
	#define INVALID_VERSION                             -1
	/**
	 * @ingroup network
	 * @brief 版本控制器
	 *
	 * <p>
	 * 版本控制器，可以查询、设置、判断文件的版本 <BR>
	 * 这个可以根据应用，来定制不同策略的版本控制器。
	 * </p>
	 *
	 * <p>
	 * 比如：
	 *  <ol>
	 *	  <li>更新主程序，连接版本服务器后，获取主程序的最新版本，初始化VersionControler，<BR>
	 *	提交给VersionDownloader， 这样就可以根据版本策略来决定是否需要下载最新的版本。</li>
	 *	  <li>商城里面的资源文件，可以有专门的“本地版本描述文件”，上面记录了每个本地文件的版本，<BR>
	 *	并且结合从Server获取的“需要更新的版本描述文件”，来判断是否需要下载最新的文件。</li>
	 *	  <li>…… </li>
	 *  </ol>
	 *  版本控制器对文件名大小写敏感
	 * </p>
     *
	 * @see IVersionControlerEvent
	 */
	class IVersionControler : public IComponent, public IEventProducer
	{
	public:
		/**
		 * @brief 版本控制器是否准备好
		 * @return 版本控制器准备好返回TRUE，否则返回FALSE
		 */
		virtual	BOOL IsReady() = 0;
        
		/**
		 * @brief 获取文件的版本
		 * @param lpszFilename [in] 文件名
		 * @return 文件的版本号。INVALID_VERSION 表示无效版本号(没有文件版本信息)
		 */
		virtual int GetLocalFileVersion(LPTSTR lpszFileName) = 0;

		virtual int GetUpdateFileVersion(LPTSTR lpszFileName) = 0;
        
		/**
		 * @brief 更新文件的版本，将本地文件的版本号改为SERVER端文件的版本号
		 * @param lpszFilename [in] 文件名
		 */
		virtual void UpdateLocalFileVersion(LPTSTR lpszFileName) = 0;
        
		/**
		 * @brief 设置本地文件的版本号
		 * @param lpszFilename [in] 文件名
		 * @param nVersion [in] 文件的版本号
		 */
		virtual void SetLocalFileVersion(LPTSTR lpszFileName, int nVersion) = 0;
        
		/**
		 * @brief 删除文件版本信息
		 * @param lpszFilename [in] 文件名
		 * @return 成功删除返回TRUE，失败返回FALSE
		 * <p>
	     * 该函数执行失败主要有以下原因：
	     *  <ol>
		 *    <li>lpszFileName 等于0</li>
	     *	  <li>本地版本描述文件不存在，本地版本描述文件的相对路径在
		 *        LOCAL_VERSION_FILE_DIR 中定义</li>
	     *	  <li>在本地版本描述文件中，找不到文件名lpszFileName的记录</li>
	     *  </ol>
	     * </p>
		 */
        virtual BOOL DeleteLocalFileVersion(LPTSTR lpszFileName) = 0;

		/**
		 * @brief 判断文件是否是最新版本
		 * @param lpszFilename [in] 文件名
		 * @return 文件是最新版本时返回TRUE，否则返回FALSE
		 */
		virtual BOOL IsNewestVersion(LPCTSTR lpszFileName) = 0;

		virtual BOOL IsVersionValid(int nVersion) = 0;
	};

	TENIO_MAKE_TYPEID_MAP(IVersionControler, COMPONENTID_VERSIONCONTROLER)
		
	/**
	 * @ingroup network
	 * @brief IVersionControler 的事件回调接口
	 *
	 * <p>
     * 当 IVersionControler::IsReady 为 FALSE， 客户需要关心这个事件，
	 * 当VersionControler准备好的时候触发这个 OnReady 事件
	 * </p>
	 * @see IVersionControler
	 */
 	class IVersionControlerEvent : public IEvent
	{
	public:

		/**
		 * @brief 版本控制器已经准备好
		 */
		virtual void OnReady(IVersionControler* pVersionControler) = 0;
	};

	class IVersionControlerReadyEvent : public IEvent
	{
	public:

		/**
		 * @brief 版本控制器已经准备好
		 */
		virtual void OnReady() = 0;
	};
};
