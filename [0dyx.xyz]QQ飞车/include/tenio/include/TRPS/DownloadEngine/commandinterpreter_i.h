#ifndef __COMMANDINTERPRETER_I_H__
#define __COMMANDINTERPRETER_I_H__

#include "../../teniointerface.h"
#include "TaskManager/taskmanager_i.h"
#include "TaskManager/taskmanagerdef.h"
#include "commandinterpreterdef.h"

namespace Tenio
{

	class ICommandInterpreter;
	class IUnzipInterpreter;

	TENIO_MAKE_TYPEID_MAP(ICommandInterpreter, COMPONENTID_COMMANDINTERPRETER);
	TENIO_MAKE_TYPEID_MAP(IUnzipInterpreter, COMPONENTID_UNZIPINTERPRETER);

	#define OPTION_EXECUTE				0x00000001
	#define OPTION_CLEAR				0x00000002
	#define OPTION_ROLLBACK				0x00000004
	#define OPTION_INVALID				0xF0000000

	/**
	 * @ingroup network
	 * @brief 命令解释器
	 */
	class ICommandInterpreter : public ITaskExecutor
	{
	public:

		/**
		 * @brief 初始化，传入命令类型和参数，
		 * 参数为一个完整的字符串，如果该命令需要n个参数，则
		 * 解析该字符串，以空格作为分隔符读出前n个Token，即n个参数
		 *
		 * @param lpszCommandType [in] 命令类型
		 * @param lpszParam [in] 参数
		 * @return 成功返回TRUE,失败返回FALSE
		 */
		virtual BOOL Init(LPCTSTR lpszCommandType, LPCTSTR lpszParam) = 0;

		/**
		 * @brief 设置操作类型：
		 * 1 OPTION_EXECUTE 执行命令
		 * 2 OPTION_CLEAR   删除备份
		 * 3 OPTION_ROLLBACK 回滚
		 * 命令的执行顺序:
		 * 1 执行命令
		 * 2 如果命令执行成功，则删除备份
		 * 3 否则，如果执行失败，则回滚
		 * <p>
		 * </p>
		 * @param dwOperation [in] 操作类型
		 * @see
		 */
		virtual BOOL SetOption(DWORD dwOperation) = 0;

		/**
		 * @brief 获取更新文件集
		 *		  现在只处理带通配符的COPY命令
		 *        目的：记录COPY命令影响的文件，在回滚或删除备份时，
		 *        调SetResourceRecord()告诉命令解析器
		 * @param pUpdateFileList [in] 执行更新的文件记录
		 * @return
		 * @see
		 */
		virtual BOOL GetResourceRecord(UPDATEFILERECORDLIST* pUpdateFileList) = 0;

		/**
		 * @brief 设置更新文件集，告诉
		 *		  带通配符的COPY命令，之前处理的文件，
		 *		  本函数在命令回滚或删除备份时使用
		 *
		 * @param pUpdateFileList [in] 执行更新的文件记录
		 * @return
		 * @see
		 */
		virtual void SetResourceRecord(UPDATEFILERECORDLIST* pUpdateFileList) = 0;
	};

	/**
	 * @ingroup network
	 * @brief ICommandInterpreter 的事件回调接口
	 * @see IResourceStorage
	 */
	class ICommandInterpreterEvent : public IEvent
	{
	public:
		/**
		 * @brief 异步命令执行成功
		 * @param nTaskID [in] 下载任务号
		 */
		virtual void OnInterpreteSuccess(int nTaskID) = 0;

		/**
		 * @brief 异步命令执行失败
		 * @param nTaskID [in] 下载任务号
		 * @param nErrorCode [in] 错误号
		 * 
		 */
		virtual void OnInterpreteFail(int nTaskID, int nErrorCode) = 0;
	};
	
	/**
	 * @ingroup network
	 * @brief 解压命令解释器
	 */
	class IUnzipInterpreter : public ICommandInterpreter
	{
		
	};
};

#endif

// InterfaceName:CommandInterpreter
// SubFolderName:ResourceStorage
