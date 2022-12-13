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
	 * @brief �����ļ�����ִ����
	 *
	 */
	class IBatchUpdateExecutor : public ITaskExecutor
	{
	public:
		/**
		 * @brief ��ʼ��
		 *
		 * @param pCommandList [in] ���
		 *
		 * @return �ɹ�����TRUE,ʧ�ܷ���FALSE
		 * @see
		 */
		virtual BOOL Init(PCOMMANDLIST pCommandList) = 0;

		/**
		 * @brief ������������
		 *
		 * @param lpszCommandType [in] ��������
		 * @param pCommandInterpreter [in] ���������
		 * @see
		 */
		virtual void AddCommandInterpreter(
			LPCTSTR lpszCommandType, 
			ICommandInterpreter* pCommandInterpreter) = 0;

		/**
		 * @brief �������������
		 *
		 * @param pTaskManager [in] ���������
		 * @return �ɹ�����TRUE,ʧ�ܷ���FALSE
		 * @see
		 */
		virtual BOOL SetCommandTaskManager(ITaskManager* pTaskManager) = 0;
	};

	/**
	 * @ingroup network
	 * @brief IBatchUpdateExecutor ���¼��ص��ӿ�
	 * @see IBatchUpdateExecutor
	 */
	class IBatchUpdateExecutorEvent : public IEvent
	{
	public:
		/**
		 * @brief ���³ɹ�
		 * @param nTaskID [in] �����
		 */
		virtual void OnUpdateSuccess(int nTaskID/*, PCOMMANDLIST pCommandList*/) = 0;

		/**
		 * @brief ����ʧ��
		 * @param nTaskID [in] �����
		 */
		virtual void OnUpdateFail(int nTaskID, int nErrorID/*, PCOMMANDLIST pCommandList*/) = 0;
	};
};

#endif

// InterfaceName:BatchUpdateExecutor
// SubFolderName:DownloadEngine
