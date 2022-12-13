#pragma once

#include "../../teniointerface.h"
#include "TRPS/ResourceStorage/resourcestorage_i.h"
#include "TaskManager/taskmanager_i.h"
#include "TRPS/Downloader/downloaderdef.h"
/*#include "VersionCenter_i.h"*/
#include "downloadenginedef.h"

namespace Tenio
{
	class IDownloadEngine;

	TENIO_MAKE_TYPEID_MAP(IDownloadEngine, COMPONENTID_DOWNLOADENGINE);

	/**
	 * @ingroup network
	 * @brief ��������
	 *
	 */
	class IDownloadEngine : public IComponent, public IEventProducer
	{
	public:
		/**
		 * @brief ��ȡ��Դ������Ŀ������ļ��б�
		 * @param PVERSIONRESOURCELIST : [in] �����ļ��б�, ·����ʾΪ��б����ʽ���磺test/1.zip
		 * @param dwMilliSeconds [in] ��ʱʱ������λ�����롣0��ʾʹ��Ĭ��ֵ
		 * @return  ��������š���1��ʾ��������ʧ��; 
		 * @see �ṹ�嶨��:PVERSIONRESOURCELIST (��: tagVERSIONRESOURCELIST)
		 */
		virtual int Download(PVERSIONRESOURCELIST pVersionResourceList, DWORD dwMilliSeconds) = 0;

		/**
		 * @brief ����һ���ļ�
		 * @param lpszDownloadType : [in] ��������
		 * @param lpszFileName : [in] �����ļ����ƣ�����·����
		 * @param nUpdateVersion : [in] �����ļ��ķ������˰汾��, -1��ʾ�����а汾����
		 * @param dwMilliSeconds [in] ��ʱʱ������λ�����롣0��ʾʹ��Ĭ��ֵ
		 * @return  ��������š���1��ʾ��������ʧ��; 
		 */
		virtual int Download(
			LPTSTR lpszDownloadType, 
			LPTSTR lpszFileName, 
			int    nUpdateVersion,
			DWORD  dwMilliSeconds,
			DWORD  dwPriority) = 0;

		/**
		 * @brief �������ȼ�
		 * @param nTaskID : [in] �����
		 * @param nPeriority : [in] ���ȼ�
		 */
		virtual void SetPriority(int nTaskID, int nPeriority) = 0;

		/**
		 * @brief �������ȼ�
		 * @param nTaskID : [in] �����
		 * @param nPeriority : [in] ���ȼ������Ŀ��
		 */
		virtual void RaisePriorityBy(int nTaskID, int nPeriority) = 0;

		/**
		 * @brief ����ĳ�������͵��������ý��������������������ʹ��Ĭ�ϵ��������ý�����
		 * @param lpszDownloadType : [in] ��������
		 * @param pDownloadConfig : [in] �������ý�����ָ��
		 */
		virtual void AddConfig(LPCTSTR lpszDownloadType, IDownloadConfig* pDownloadConfig) = 0;

		/**
		 * @brief ɾ��ĳ�������͵��������ý�����
		 * @param lpszDownloadType : [in] ��������
		 */
		virtual void RemoveConfig(LPCTSTR lpszDownloadType) = 0;

		/**
		 * @brief ����ĳ����Ľ�����
		 * @param lpszCommandType : [in] ��������
		 * @param pCommandInterpreter : [in] ���������ָ��
		 */
		virtual void AddCommandInterpreter(
			LPCTSTR lpszCommandType, 
			ICommandInterpreter* pCommandInterpreter) = 0;

	   /**
		 * @brief ɾ������Ľ�����
		 * @param lpszCommandType : [in] ��������
		 */
		virtual void RemoveCommandInterpreter(
			LPCTSTR lpszCommandType) = 0;


		/**
		 * @brief ����Ĭ�ϴ��������
		 * @param pSock5ProxyParam : [in] �������������
		 */
		virtual void SetDefaultProxy(SOCK5PROXYPARAM * pSock5ProxyParam) = 0;

		/**
		 * @brief �����ܴ���
		 * @param dwBandWidth : [in] ����
		 */
		virtual void SetTotalBandWidth(DWORD dwBandWidth) = 0;

		/**
		 * @brief ȡ������
		 * @param nTaskID : [in] ���������
		 */
		virtual BOOL CancelTask(int nTaskID) = 0;

		/**
		 * @brief ����Ƿ���δ��ɵĸ�����������У�����и���
		 * �ò�����Ҫ�������ļ����سɹ����޷�ִ�и��ǲ���
		 * ������������ʱ�����øú������и���
		 */
		virtual int CheckUpdate() = 0;
	};


	/**
	 * @ingroup network
	 * @brief IDownloadEngine ���¼��ص��ӿ�
	 * @see IDownloadEngine
	 */
	class IDownloadAndUpdateEvent : public IEvent
	{
	public:
		/**
		 * @brief ���سɹ�
		 * @param nTaskID [in] ���������
		 * @param pVersionResourceList [in] �����ļ��б�
		 */
		virtual void OnDownloadSuccess(int nTaskID, PVERSIONRESOURCELIST pVersionResourceList) = 0;

		/**
		 * @brief ����ʧ��
		 * @param nTaskID [in] ���������
		 * @param nErrorCode [in] �������
		 */
		virtual void OnDownloadError(int nTaskID, int nErrorCode) = 0;

		/**
		 * @brief ���ؽ��ȷ����仯
		 * @param nTaskID [in] ���������
		 * @param dwFileSize [in]�����ļ����ܳ��ȣ���λ(�ֽ�)
		 * @param dwDownloadedSize [in] �����ش�С����λ(�ֽ�)
		 */
		virtual void OnProgressChanged(int nTaskID, DWORD dwFileSize, DWORD dwDownloadedSize) = 0;

		/**
		 * @brief �ļ����³ɹ���ֻ�е���CheckUpdate()ʱ���ſ��ܴ������¼�
		 * @param PUPDATEFILELIST [in] ���³ɹ����ļ��б�
		 */
		virtual void OnUpdateSuccess(PVERSIONRESOURCELIST pVersionResourceList) = 0;

		/**
		 * @brief �ļ�����ʧ�ܣ�ֻ�е���CheckUpdate()ʱ���ſ��ܴ������¼�
		 * @param nErrorID [in] �������
		 * @param pUpdateFileList [in] ���³ɹ����ļ��б�
		 */
		virtual void OnUpdateFail(int nErrorID, PVERSIONRESOURCELIST pVersionResourceList) = 0;
	};	
};


// InterfaceName:DownloadEngine
// SubFolderName:DownloadEngine
