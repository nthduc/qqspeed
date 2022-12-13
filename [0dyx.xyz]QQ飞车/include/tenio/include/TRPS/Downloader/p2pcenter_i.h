#ifndef __P2PCENTER_I_H__
#define __P2PCENTER_I_H__

#include "../../teniointerface.h"
#include "downloaderdef.h"

namespace Tenio
{
	class IP2PCenter;

	TENIO_MAKE_TYPEID_MAP(IP2PCenter, COMPONENTID_P2PCENTER);

	/**
	 * @ingroup
	 * @brief P2P�������ģ���������������������ﴦ��
	 *        P2PCenterά��ȫ��Ψһ��IVqqDownload, ΪIVqqDownload�ṩΨһ�Ļص�����
	 *
	 * @see
	 */
	class IP2PCenter : public IComponent
	{
	public:
		
		/**
		 * @brief �������
		 *
		 * @param pstP2PParam [in] P2P���ز���
		 * @param hNotifyWnd [in]  �ص�����
		 * @return �ɹ�����TRUE,ʧ�ܷ���FALSE
		 * @see
		 */
		virtual  BOOL AddTask(DOWNLOADP2PPARAM * pstP2PParam, HWND hNotifyWnd) = 0;

		/**
		 * @brief ֹͣ����
		 *
		 * @param unTaskID [in] �����
		 * @see
		 */
		virtual BOOL StopP2P(UINT unTaskID) = 0;

		/**
		 * @brief ɾ������
		 *
		 * @param unTaskID [in] �����
		 * @see
		 */
		virtual BOOL DelTask(UINT unTaskID) = 0;

		/**
		 * @brief ����P2P�����ٶ�
		 *
		 * <p>
		 * </p>
		 * @param
		 * @return
		 * @see
		 */
		virtual BOOL SetP2PDownloadSpeed(UINT unTaskID, UINT unSpeed) = 0;

		/**
		 * @brief ����P2P�ϴ��ٶ�
		 *
		 * @param unSpeed [in] �ϴ��ٶ�,��λ(KB/s)
		 * @return �ɹ�����TRUE,ʧ�ܷ���FALSE
		 * @see
		 */
		virtual BOOL AdjustP2PUploadSpeed(UINT unSpeed) = 0;

		/**
		 * @brief ����HTTP�����ٶ�
		 *
		 * @param unSpeed [in] HTTP�����ٶ�,��λ(KB/s)
		 * @return �ɹ�����TRUE,ʧ�ܷ���FALSE
		 * @see
		 */
		virtual BOOL AdjustP2PHttpSpeed(UINT unSpeed) = 0;

	};

	/**
	 * @ingroup network
	 * @brief IP2PCenter ���¼��ص��ӿ�
	 * @see IP2PCenter
	 */
	class IP2PCenterEvent : public IEvent
	{
	public:		
		/**
		 * @brief ���سɹ�
		 * @param unTaskID [in] ���������
		 */
		virtual void OnGetFileSize(UINT unTaskID, DWORD dwFileSize) = 0;

		/**
		 * @brief ���سɹ�
		 * @param unTaskID [in] ���������
		 */
		virtual void OnDownloadSuccess(UINT unTaskID) = 0;
		/**	
		 * @brief ����ʧ��
		 * @param unTaskID [in] ���������
		 * @param nErrorCode [in] �������
		 */
		virtual void OnDownloadError(UINT unTaskID, int nErrorCode) = 0;

		/**
		 * @brief ���ؽ��ȷ����仯
		 * @param unTaskID [in] ���������
		 * @param nPercent [in] �����ļ��Ľ��ȣ���λ(1%)
		 */
		virtual void OnProgressChanged(UINT unTaskID, int nPercent) = 0;
	};
};

#endif

// InterfaceName:P2PCenter
// SubFolderName:Downloader

