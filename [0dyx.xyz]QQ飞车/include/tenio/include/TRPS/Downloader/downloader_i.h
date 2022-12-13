#pragma once

#include "../../teniointerface.h"
#include "../../event_i.h"
#include "TRPS/Downloader/downloaderdef.h"
#include "TaskManager/taskmanager_i.h"
//#include "objecttype2idmap.h"
//#include "../DownloadProxy/DownloadDef.h"

namespace Tenio
{        		
	class IDownloader;
	class IDownloaderEvent;
	class IIPGroupDownloader;
	class IBaseIPGroupDownloader;
	class IHttpDownloader;
	class IP2PDownloader;
	class IDownloadControler;
	class IDownloadCommand;
	class IInstallDownloadLsp;
	class IDownloadLspManager;
	
	TENIO_MAKE_TYPEID_MAP(IIPGroupDownloader, COMPONENTID_IPGROUPDOWNLOADER);
	TENIO_MAKE_TYPEID_MAP(IBaseIPGroupDownloader, COMPONENTID_BASEIPGROUPDOWNLOADER);
	TENIO_MAKE_TYPEID_MAP(IInstallDownloadLsp, COMPONENTID_INSTALLDOWNLOADLSP);
	//	TENIO_MAKE_TYPEID_MAP(IDownloadLspManager, COMPONENTID_IPGROUPDOWNLOADER);
	TENIO_MAKE_TYPEID_MAP(IHttpDownloader, COMPONENTID_HTTPDOWNLOADER);
	TENIO_MAKE_TYPEID_MAP(IP2PDownloader, COMPONENTID_P2PDOWNLOADER);
	TENIO_MAKE_TYPEID_MAP(IDownloadControler, COMPONENTID_DOWNLOADCONTROLER);
	TENIO_MAKE_TYPEID_MAP(IDownloadCommand, COMPONENTID_DOWNLOADCOMMAND);
    
	/**
	* @ingroup network
	* @brief �ײ�����ģ��
	*
	* <p>
	*     IDownloader �Ǹ�������������ĸ��࣬ IDownloader �ж�����࣬�ֱ����ͬ���͵�
	*     ������������� IIPGroupDownloader��IHttpDownloader��IP2PDownloader���ֱ����IPȺ������
	*     �����HTTPЭ����������P2PЭ����������
	*     ����ֱ�Ӵ��� IDownloader ���͵������ֻ�ܴ����������������������
	* </p>
	*
	* <p>
	* IDownloader ���������:
	* <ol>
	*   <li>IIPGroupDownloader �� ��ָ��������ط���������������</li>
	*   <li>IHttpDownloader �� ʹ��HTTPЭ�����������</li>
	*   <li>IP2PDownloader �� ʹ��P2PЭ�����������</li>
	* </ol>
	* </p>
	*
	* <p>
	* ʹ������
	* <ol>
	*   <li>ѡ��һ�������͵� Downloader ���д���</li>
	*   <li>ʹ�� AddEventHandler() ��������ص��¼�ָ��</li>
	*   <li>ʹ�� Init() �������ز���</li>
	*   <li>ʹ�� Execute() ��������</li>
	*   <li>���¼��ص��ӿ����յ� IDownloaderEvent::OnDownloadSuccess() ��Ϣ����ʾ���سɹ����</li>
	*   <li>���¼��ص��ӿ����յ� IDownloaderEvent::OnDownloadError() ��Ϣ����ʾ����ʧ��</li>
	*   <li>���¼��ص��ӿ����յ� IDownloaderEvent::OnProgressChanged() ��Ϣ����ʾ�������ڽ���</li>
	*   <li>��������ʱ������ʹ�� CancelDownload() ȡ������</li>
	*   <li>��������ʱ������ʹ�� SetBandWidth() ���ô���</li>
	* </ol>
	* </p>
	*
	* @see IPGroupDownloader, HttpDownloader, P2PDownloader, IDownloaderEvent
	*/
	class IDownloader : public ITaskExecutor
	{
	public:
		
		/**
		* @brief �������ز���
		* @param pDownloadParam [in] ���ز���
		* @return �ɹ�����TRUE,ʧ�ܷ���FALSE
		*/
		virtual BOOL Init(void* pDownloadParam) = 0;
		
		/**
		* @brief ���ô���
		* @param nBandWidth [in] ����
		* @return �ɹ�����TRUE,ʧ�ܷ���FALSE
		*/
		virtual BOOL SetBandWidth(int nBandWidth) = 0;

		/**
		* @brief ��ȡ����
		* @param nBandWidth [in] ����
		* @return ����
		*/
		virtual int GetBandWidth() = 0;
	};
	
	/**
	* @ingroup network
	* @brief IDownloader ���¼��ص��ӿ�
	* @see IDownloader
	*/
	class IDownloaderEvent : public IEvent
	{
	public:		
		/**
		* @brief ���سɹ�
		* @param nTaskID [in] ���������
		*/
		virtual void OnDownloadSuccess(int nTaskID) = 0;
		
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
		
		// ��Ϊ�ض���֮���Server��·��������ԭ���Ĳ�ͬ�������ض�����IPGroup Downloader�㴦��
		// ����HTTP Downloader��
		//virtual void OnRedirect(int nTaskID, URLARRAY * lpUrlArray) = 0;
	};
    
	/**
	* @ingroup network
	* @brief IPȺ��������: 
	*
	* <p>
	* ����һ���ļ�ʱ, ���Ȼ��ѯP2P�����ļ�������ҵ�
	* ���ļ�����Ӧ��TPT�ļ���������P2P���أ�����
	* ����HTTP���ء�HTTP���ػὫ����IP��һ�飬ֱ����
	* �سɹ�Ϊֹ�����ȫ��IP�Թ�һ��֮�󣬻�������ʧ
	* �ܣ��򱾴���������ʧ�ܡ�
	* </p>
	*
	* ʹ�÷�����ο� IDownloader
	*/
	class IIPGroupDownloader : public IDownloader/*, public IEventImpl<IDownloaderEvent, 1>*/
	{
	public:	
		
	};
	
// 	class IBaseIPGroupDownloader : public IIPGroupDownloader
// 	{
// 	public:
// 		virtual int GetBandWidth() = 0;
// 	};
    
	/**
	* @ingroup network
	* @brief HTTP������ 
	*
	* <p>
	* ʹ��HTTPЭ������
	* �ض���ֻ���ʹ��һ��IP
	* </p>
	*
	* ʹ�÷�����ο� IDownloader
	*/
	class IHttpDownloader : public IDownloader
	{
	public:
		
	};
    
	/**
	* @ingroup network
	* @brief P2P������ 
	*
	* <p>
	* ͬʱʹ��P2PЭ���HTTPЭ�����ء����ļ��Ƚ�Сʱ��ʹ��HTTPЭ������Ч�ʱȽϸߣ��󲿷��ļ�����ͨ��HTTPЭ�����أ�
	* ���ļ��Ƚϴ�ʱ��ʹ��P2PЭ������Ч�ʱȽϸߣ��󲿷��ļ�����ͨ��P2PЭ�����ء�
	* </p>
	*
	* ʹ�÷�����ο� IDownloader
	*/
	class IP2PDownloader : public IDownloader
	{
	public:
		
	};   
		
// 	class ICommand : public IEventProducer, public IComponent
// 	{   
// 	public:
// 		virtual BOOL Execute() = 0;
// 	};
//     
// 	class ICommandEvent : public IEvent
// 	{
// 	public:
//         virtual void OnCommandComplete(ICommand* pCommand, BOOL bSuccess) = 0;
// 	};
// 	
// 	class IDownloadCommand : public ICommand
// 	{
// 	public:
// 		virtual void Initialize(IDownloader* pDownloader) = 0;
// 	};
//     
//     class IDownloadControler : public IComponent
// 	{
// 	public:
// 		virtual void AddCommand(ICommand* pCommand) = 0;
// 	};
// 	
// 	class IInstallDownloadLsp : public IComponent
// 	{
// 	public:
// 		virtual BOOL InstallDownloadLsp(IIPGroupDownloader* pIPGroupDownloader) = 0;
// 	};
	
	// 	class IDownloadLspManager : public IIPGroupDownloader, public IInstallDownloadLsp
	// 	{
	// 
	// 	};
	
};
