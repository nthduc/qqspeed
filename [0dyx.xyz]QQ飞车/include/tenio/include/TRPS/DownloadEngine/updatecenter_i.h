#ifndef __UPDATECENTER_I_H__
#define __UPDATECENTER_I_H__

#include "../../teniointerface.h"
#include "commandinterpreter_i.h"
#include "downloadenginedef.h"

namespace Tenio
{
	class IUpdateCenter;

	TENIO_MAKE_TYPEID_MAP(IUpdateCenter, COMPONENTID_UPDATECENTER);

	/**
	 * @ingroup network
	 * @brief 更新中心
	 *
	 * @see
	 */
	class IUpdateCenter : public IComponent, public IEventProducer
	{
	public:
		/**
		 * @brief 更新资源
		 *
		 * @param pUpfateTaskInfo [in] 更新任务
		 * @return 任务ID,－1表示更新失败，0表示更新成功，>0表示更新异步进行; 
		 * @see
		 */
		virtual int Update(PUPDATETASKINFO pUpfateTaskInfo) = 0;

		/**
		 * @brief 添加命令解析器
		 *
		 * @param lpszCommandType [in] 命令类型
		 * @param pCommandInterpreter [in] 命令解析器
		 * @see
		 */
		virtual void AddCommandInterpreter(
			LPCTSTR lpszCommandType, 
			ICommandInterpreter* pCommandInterpreter) = 0;

		/**
		 * @brief 删除命令解析器
		 *
		 * @param lpszCommandType [in] 命令类型
		 * @see
		 */
		virtual void RemoveCommandInterpreter(
			LPCTSTR lpszCommandType) = 0;

	};

	/**
	 * @ingroup network
	 * @brief IUpdateCenter 的事件回调接口
	 * @see IUpdateCenter
	 */
	class IUpdateCenterEvent : public IEvent
	{
	public:
		/**
		 * @brief 更新成功
		 * @param nTaskID [in] 任务号
		 */
		virtual void OnUpdateSuccess(int nTaskID) = 0;

		/**
		 * @brief 更新失败
		 * @param nTaskID [in] 任务号
		 */
		virtual void OnUpdateFail(int nTaskID, int nErrorID) = 0;

	};
};

#endif

// InterfaceName:UpdateCenter
// SubFolderName:UpdateCenter
