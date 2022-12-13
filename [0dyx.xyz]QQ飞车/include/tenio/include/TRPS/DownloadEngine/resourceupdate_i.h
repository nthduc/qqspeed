#ifndef __RESOURCEUPDATE_I_H__
#define __RESOURCEUPDATE_I_H__

#include "../../teniointerface.h"
//#include "resourcestoragedef.h"
#include "downloadenginedef.h"
#include "commandinterpreter_i.h"
#include "TaskManager/taskmanager_i.h"
#include "TaskManager/taskmanagerdef.h"

namespace Tenio
{
	class IResourceUpdator;

	TENIO_MAKE_TYPEID_MAP(IResourceUpdator, COMPONENTID_RESOURCEUPDATE);

	/**
	 * @ingroup network
	 * @brief 资源更新器
	 *
	 * @see
	 */
	class IResourceUpdator : public ITaskExecutor
	{
	public:

		/**
		 * @brief 初始化
		 *
		 * <p>
		 * </p>
		 * @param lpszCommand [in] 命令集
		 * @param dwPriority  [in] 优先级
		 * @return 成功返回TRUE,失败返回FALSE
		 * @see
		 */
		virtual BOOL Init(LPCTSTR lpszCommand, DWORD dwPriority) = 0;

		/**
		 * @brief 添加命令解析器
		 *
		 * @param lpszCommandType [in] 命令类型
		 * @return pCommandInterpreter [in] 命令解析器
		 * @see
		 */
		virtual void AddCommandInterpreter(
			LPCTSTR lpszCommandType, 
			ICommandInterpreter* pCommandInterpreter) = 0;

		/**
		 * @brief 设置任务管理器
		 *
		 * @param pTaskManager [in] 任务管理器
		 * @return 成功返回TRUE,失败返回FALSE
		 * @see
		 */
		virtual BOOL SetCommandTaskManager(ITaskManager* pTaskManager) = 0;
	};

	/**
	 * @ingroup network
	 * @brief IResourceUpdator 的事件回调接口
	 * @see IResourceUpdator
	 */
	class IResourceUpdatorEvent : public IEvent
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

// InterfaceName:ResourceUpdate
// SubFolderName:ResourceStorage
