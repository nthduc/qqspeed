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
	 * @brief ��������
	 *
	 * @see
	 */
	class IUpdateCenter : public IComponent, public IEventProducer
	{
	public:
		/**
		 * @brief ������Դ
		 *
		 * @param pUpfateTaskInfo [in] ��������
		 * @return ����ID,��1��ʾ����ʧ�ܣ�0��ʾ���³ɹ���>0��ʾ�����첽����; 
		 * @see
		 */
		virtual int Update(PUPDATETASKINFO pUpfateTaskInfo) = 0;

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
		 * @brief ɾ�����������
		 *
		 * @param lpszCommandType [in] ��������
		 * @see
		 */
		virtual void RemoveCommandInterpreter(
			LPCTSTR lpszCommandType) = 0;

	};

	/**
	 * @ingroup network
	 * @brief IUpdateCenter ���¼��ص��ӿ�
	 * @see IUpdateCenter
	 */
	class IUpdateCenterEvent : public IEvent
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

// InterfaceName:UpdateCenter
// SubFolderName:UpdateCenter
