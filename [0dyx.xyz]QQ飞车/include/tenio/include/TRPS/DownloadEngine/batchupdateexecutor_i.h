#ifndef __BATCHUPDATEEXECUTOR_I_H__
#define __BATCHUPDATEEXECUTOR_I_H__

#include "../../teniointerface.h"
#include "downloadenginedef.h"
#include "TaskManager/taskmanager_i.h"
#include "TaskManager/taskmanagerdef.h"
#include "commandinterpreter_i.h"
namespace Tenio
{
	class IBatchUpdateExecutor;

	TENIO_MAKE_TYPEID_MAP(IBatchUpdateExecutor, COMPONENTID_BATCHUPDATEEXECUTOR);

	/**
	 * @ingroup network
	 * @brief 批量文件更新执行器
	 *
	 */
	class IBatchUpdateExecutor : public ITaskExecutor
	{
	public:
		/**
		 * @brief 初始化
		 *
		 * @param pCommandList [in] 命令集
		 *
		 * @return 成功返回TRUE,失败返回FALSE
		 * @see
		 */
		virtual BOOL Init(PCOMMANDLIST pCommandList) = 0;

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
	 * @brief IBatchUpdateExecutor 的事件回调接口
	 * @see IBatchUpdateExecutor
	 */
	class IBatchUpdateExecutorEvent : public IEvent
	{
	public:
		/**
		 * @brief 更新成功
		 * @param nTaskID [in] 任务号
		 */
		virtual void OnUpdateSuccess(int nTaskID/*, PCOMMANDLIST pCommandList*/) = 0;

		/**
		 * @brief 更新失败
		 * @param nTaskID [in] 任务号
		 */
		virtual void OnUpdateFail(int nTaskID, int nErrorID/*, PCOMMANDLIST pCommandList*/) = 0;
	};
};

#endif

// InterfaceName:BatchUpdateExecutor
// SubFolderName:DownloadEngine
