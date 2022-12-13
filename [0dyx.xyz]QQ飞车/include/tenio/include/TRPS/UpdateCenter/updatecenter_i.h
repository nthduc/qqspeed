#pragma once

#include "../../teniointerface.h"
#include "updatecenterdefine.h"

namespace Tenio
{
	enum enmUpdateTargetType
	{
		CLIENT,			// �ͻ���
		PREDOWNLOAD,	// Ԥ������Դ

		// TODO : �����Ҫ����������
	};

	class IUpdateCenter;
	class IUpdateCenterEvent;

	TENIO_MAKE_TYPEID_MAP(IUpdateCenter, COMPONENTID_TRPS_UPDATECENTER)

	/**
	 * @ingroup system 
	 * @brief �ͻ��˽����ڵİ汾�����������
	 *
	 * <p>
	 *���� ��Ƕ�ڿͻ��˽��̵İ汾����������������µĶ��󣬿��԰����ͻ��ˡ�
	 *	Ԥ������Դ����������������ȡ���������¶���İ汾������Ϣ��Ȼ�����
	 *	��Ӧ�����ء������������ԣ����ﵽ�汾������Ŀ�ġ�
	 * </p>
	 *
	 * @sa IUpdateCenterEvent
	 */
	class IUpdateCenter : public IComponent, public IEventProducer
	{
	public:
		/**
		 *  @brief �汾�������ĵĳ�ʼ������
		 *  @return ������ʼ���Ƿ�ɹ�
		 *  @sa 
		 */
		virtual BOOL Initialize() = 0;

		/**
		 *  @brief �������¶���İ汾������Ϣ��ִ�����������������������첽�ģ�������
		 *	ͨ���¼���IUpdateCenterEvent����֪ͨ�����
		 *
		 *  @param [in]nType ���¶�������ͣ������ǿͻ���(client)��Ԥ������Դ(predownload)
		 *	�ȵȣ����Ϳ��Ը���������չ
		 *  @param [in]lpUpdateInfo �Ǹ��¶���İ汾������Ϣbuffer����Server�·������ġ�
		 *	@param [in]dwSize ��lpUpdateInfo��buffer�ֽڴ�С
		 *  @return ����nTaskID ���µ�����ID����� -1 ��������ʧ��
		 *  @sa IUpdateCenterEvent
		 */
		virtual int Update(int nType, LPBYTE lpUpdateInfo, DWORD dwSize) = 0;

		// �Զ�����
		virtual BOOL AutoUpdate() = 0;

		virtual BOOL GetModuleVersion(tstring& strModuleID, MODULEVERSIONINFO& stVersion) = 0;
	};

	/**
	 * @ingroup (network coding system tenuilib business arithmetic)
	 * @brief �汾�������ĵ��¼��ӿ�
	 *
	 * <p>
	 *���� ͨ���汾�������ĵ��¼��ӿڣ���֪ͨ���������ĳɹ�����ʧ��
	 * </p>
	 *
	 * @sa IUpdateCenter
	 */
	class IUpdateCenterEvent : public IEvent
	{
	public:
		/**
		 *  @brief ���³ɹ��¼�
		 *  @param [in]nType ���¶�������ͣ�client��predownload������ 
		 *  @param [in]nTaskID ��������ID
		 *  @return 
		 *  @sa 
		 */
		virtual void OnUpdateSuccess(int nType, int nTaskID) = 0;

		/**
		 *  @brief ����ʧ���¼�
		 *  @param [in]nType ���¶�������ͣ�client��predownload������ 
		 *  @param [in]nTaskID ��������ID
		 *  @param [in]nErrorCode ����ID
		 *  @return 
		 *  @sa 
		 */
		virtual void OnUpdateFail(int nType, int nTaskID, int nErrorCode) = 0;

		virtual void OnNoNeedUpdate() = 0;

		virtual void OnExitToUpdate() = 0;

		virtual void OnContinueUpdate() = 0;
	};
};