#pragma once

#include "../../teniointerface.h"
#include "updatecenterdefine.h"

namespace Tenio
{
	enum enmUpdateTargetType
	{
		CLIENT,			// 客户端
		PREDOWNLOAD,	// 预下载资源

		// TODO : 添加需要升级的类型
	};

	class IUpdateCenter;
	class IUpdateCenterEvent;

	TENIO_MAKE_TYPEID_MAP(IUpdateCenter, COMPONENTID_TRPS_UPDATECENTER)

	/**
	 * @ingroup system 
	 * @brief 客户端进程内的版本升级中心组件
	 *
	 * <p>
	 *　　 内嵌于客户端进程的版本升级更新组件。更新的对象，可以包括客户端、
	 *	预下载资源包……。组件负责获取、解析更新对象的版本升级信息，然后根据
	 *	对应的下载、更新升级策略，来达到版本升级的目的。
	 * </p>
	 *
	 * @sa IUpdateCenterEvent
	 */
	class IUpdateCenter : public IComponent, public IEventProducer
	{
	public:
		/**
		 *  @brief 版本升级中心的初始化操作
		 *  @return 表明初始化是否成功
		 *  @sa 
		 */
		virtual BOOL Initialize() = 0;

		/**
		 *  @brief 给出更新对象的版本升级信息，执行升级操作。升级操作是异步的，后续会
		 *	通过事件（IUpdateCenterEvent）来通知结果。
		 *
		 *  @param [in]nType 更新对象的类型，可以是客户端(client)、预下载资源(predownload)
		 *	等等，类型可以根据需求扩展
		 *  @param [in]lpUpdateInfo 是更新对象的版本升级信息buffer，从Server下发过来的。
		 *	@param [in]dwSize 是lpUpdateInfo的buffer字节大小
		 *  @return 返回nTaskID 更新的任务ID，如果 -1 表明调用失败
		 *  @sa IUpdateCenterEvent
		 */
		virtual int Update(int nType, LPBYTE lpUpdateInfo, DWORD dwSize) = 0;

		// 自动升级
		virtual BOOL AutoUpdate() = 0;

		virtual BOOL GetModuleVersion(tstring& strModuleID, MODULEVERSIONINFO& stVersion) = 0;
	};

	/**
	 * @ingroup (network coding system tenuilib business arithmetic)
	 * @brief 版本升级中心的事件接口
	 *
	 * <p>
	 *　　 通过版本升级中心的事件接口，来通知升级操作的成功或者失败
	 * </p>
	 *
	 * @sa IUpdateCenter
	 */
	class IUpdateCenterEvent : public IEvent
	{
	public:
		/**
		 *  @brief 更新成功事件
		 *  @param [in]nType 更新对象的类型（client、predownload……） 
		 *  @param [in]nTaskID 更新任务ID
		 *  @return 
		 *  @sa 
		 */
		virtual void OnUpdateSuccess(int nType, int nTaskID) = 0;

		/**
		 *  @brief 更新失败事件
		 *  @param [in]nType 更新对象的类型（client、predownload……） 
		 *  @param [in]nTaskID 更新任务ID
		 *  @param [in]nErrorCode 错误ID
		 *  @return 
		 *  @sa 
		 */
		virtual void OnUpdateFail(int nType, int nTaskID, int nErrorCode) = 0;

		virtual void OnNoNeedUpdate() = 0;

		virtual void OnExitToUpdate() = 0;

		virtual void OnContinueUpdate() = 0;
	};
};