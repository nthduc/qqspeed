
#pragma once

#include "TRPS/ResourceStorage/resourcestorage_i.h"
#include "TaskManager/taskmanager_i.h"
//#include "../DownloadProxy/DownloadProxyObject_i.h"
#include "VersionControler/versioncontroler_i.h"
//#include "../VersionDownload/versiondownload_i.h"

namespace Tenio
{	
   	/**
	 * @ingroup network
	 * @brief ����������
	 *
	 * <p>
	 * ִ�����������������һ���ļ�����ʧ�ܣ���������������ʧ�ܣ�
	 * �����ļ����ֲ��䣬������ǰ��ͬ��
	 * </p>
	 *
	 * <p>
	 * ʹ������
	 * <ol>
	 *   <li>ʹ�� AddEventHandler() ��������ص��¼�ָ��</li>
	 *   <li>ʹ�� SetDownloadConfig() �������ز���</li>
	 *   <li>ʹ�� SetResourceList() ���������ļ��б�</li>
	 *   <li>ʹ�� SetTimeOut() ���ó�ʱʱ��������ʹ��Ĭ�ϳ�ʱʱ��</li>
	 *   <li>ʹ�� SetVersionControler() ���ð汾������</li>
	 *   <li>ʹ�� Execute() ��������</li>
	 *   <li>���¼��ص��ӿ����յ� IDownloadExecutorEvent::OnDownloadSuccess() ��Ϣ����ʾ���سɹ����</li>
	 *   <li>���¼��ص��ӿ����յ� IDownloadExecutorEvent::OnDownloadFail() ��Ϣ����ʾ����ʧ��</li>
	 *   <li>��������ʱ������ʹ�� Stop() ֹͣ����</li>
	 * </ol>
	 * </p>
	 *
	 * @see ITaskExecutor, IDownloadExecutorEvent
	 */
	class IBatchDownloadExecutor : public ITaskExecutor
	{
	public:
		/**
		 * @brief  �������
		 * @param  pDownloadConfig [in] ������
		 * @param  dwDownloaderID [in]  ������ID
		 * @param  dwPriority     [in]  ���ȼ�
		 * @param  nUpdateVersion [in]  �ļ��汾��
		 *
		 * @return �ɹ�����TRUE,ʧ�ܷ���FALSE
		 * @see
		 */
		virtual BOOL AddTask(IDownloadConfig * pDownloadConfig, 
			DWORD dwDownloaderID, 
			DWORD dwPriority,
			int nUpdateVersion = -1) = 0;

		/**
		 * @brief  �������ȼ�
		 * @param  nPeriority     [in]  ���ȼ�
		 *
		 * @see
		 */
		virtual void SetPriority(int nPeriority) = 0;

		/**
		 * @brief  �������ȼ�
		 * @param  nPeriority     [in]  ���ȼ������Ŀ��
		 *
		 * @see
		 */
		virtual void RaisePriorityBy(int nPeriority) = 0;

		/**
		 * @brief  �������������
		 * @param  pTaskManager     [in]  ���������
		 *
		 * @see
		 */
		virtual BOOL SetTaskManager(ITaskManager* pTaskManager) = 0;
		
		/**
		 * @brief  ���ô���
		 * @param  nBandWidth     [in]  ����
		 *
		 * @see
		 */
		virtual BOOL SetBandWidth(int nBandWidth) = 0;

		/**
		 * @brief  ���ô���
		 * @param  pSock5ProxyParam   [in]  ����
		 *
		 * @see
		 */
		virtual void SetDefaultProxy(SOCK5PROXYPARAM * pSock5ProxyParam) = 0;
	};
	
	TENIO_MAKE_TYPEID_MAP(IBatchDownloadExecutor, COMPONENTID_BATCHDOWNLOADEXECUTOR)
	TENIO_MAKE_EVENT_MAP(IBatchDownloadExecutor, IDownloaderEvent)
		
};