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
	 * @brief ��Դ������
	 *
	 * @see
	 */
	class IResourceUpdator : public ITaskExecutor
	{
	public:

		/**
		 * @brief ��ʼ��
		 *
		 * <p>
		 * </p>
		 * @param lpszCommand [in] ���
		 * @param dwPriority  [in] ���ȼ�
		 * @return �ɹ�����TRUE,ʧ�ܷ���FALSE
		 * @see
		 */
		virtual BOOL Init(LPCTSTR lpszCommand, DWORD dwPriority) = 0;

		/**
		 * @brief ������������
		 *
		 * @param lpszCommandType [in] ��������
		 * @return pCommandInterpreter [in] ���������
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
	 * @brief IResourceUpdator ���¼��ص��ӿ�
	 * @see IResourceUpdator
	 */
	class IResourceUpdatorEvent : public IEvent
	{
	public:
		/**
		 * @brief ���³ɹ�
		 * @param nTaskID [in] �����
		 */
		virtual void OnUpdateSuccess(int nTaskID) = 0;

		/**
		 * @brief ����ʧ��
		 * @param nTaskID [in] �����
		 */
		virtual void OnUpdateFail(int nTaskID, int nErrorID) = 0;
	};
};

#endif

// InterfaceName:ResourceUpdate
// SubFolderName:ResourceStorage
