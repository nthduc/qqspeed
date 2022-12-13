#ifndef __HTTPPROTOCOLHANDLER_I_H__
#define __HTTPPROTOCOLHANDLER_I_H__

#include "../../teniointerface.h"
#include "TRPS/Downloader/downloaderdef.h"

namespace Tenio
{
	class IHttpProtocolHandler;

	TENIO_MAKE_TYPEID_MAP(IHttpProtocolHandler, COMPONENTID_HTTPPROTOCOLHANDLER);
    
	/**
	 * @ingroup network
	 * @brief HTTPЭ�鴦����
	 *
	 * <p>
	 * HTTPЭ�鴦���� ��������SERVER�˵����ӣ�������������Э����ͽ������ݰ���
	 * ʵ�ֳ�ʱ���ƣ��򱾵�Ӳ��д���ݣ��Լ������ļ���������У�顣
	 * </p>
	 *
	 * <p>
	 * ʹ������
	 * <ol>
	 *   <li>ʹ�� AddEventHandler() ��������ص��¼�ָ��</li>
	 *   <li>ʹ�� SetBandWidth() ���ô�������ʹ��Ĭ�ϴ���</li>
	 *   <li>ʹ�� SetProxy() ���ô��������޴���</li>
	 *   <li>ʹ�� SetTimeOut() ���ó�ʱʱ��������ʹ��Ĭ�ϳ�ʱʱ��</li>
	 *   <li>ʹ�� Download() ��������</li>
	 *   <li>���¼��ص��ӿ����յ� IHttpProtocolHandlerEvent::OnDownloadSuccess() ��Ϣ����ʾ���سɹ����</li>
	 *   <li>���¼��ص��ӿ����յ� IHttpProtocolHandlerEvent::OnDownloadError() ��Ϣ����ʾ����ʧ��</li>
	 *   <li>���¼��ص��ӿ����յ� IHttpProtocolHandlerEvent::OnProgressChanged() ��Ϣ����ʾ�������ڽ���</li>
	 *   <li>��������ʱ������ʹ�� CancelDownload() ȡ������</li>
	 * </ol>
	 * </p>
	 *
	 * @see IHttpProtocolHandlerEvent
	 */
	class IHttpProtocolHandler : public IComponent, public IEventProducer
	{
	public:
		/**
		 * @brief ��������������
		 * @param nBandWidth [in] ����
		 * @return ���nBandWidth > 0������TRUE�����򷵻�FALSE
		 */
		virtual BOOL SetBandWidth(int nBandWidth) = 0;
		
		/**
		 * @brief ���ô���
		 * @param pSock5ProxyParam [in] ����ṹ��ָ��
		 * @return  ���pSock5ProxyParam != NULL�Ҵ���IP��Ϊ�գ�
		 *          ����TRUE�����򷵻�FALSE
		 * @see �ṹ�嶨��:SOCK5PROXYPARAM, (��: tagProxyParam)
		 */
	    virtual BOOL SetProxy(SOCK5PROXYPARAM * pSock5ProxyParam) = 0;

		/**
		 * @brief ���ó�ʱʱ��
		 * @param dwMilliSeconds [in] ��ʱʱ������λ�����롣0��ʾʹ��Ĭ��ֵ
		 */
		virtual void SetTimeOut(DWORD dwMilliSeconds) = 0;

		/**
		 * @brief ��������
		 * @param lpszFilename [in] �����ļ���URL
		 * @param lpszSaveFilename [in] �����ļ�����������·��
		 * @return ����������������TRUE�����򷵻�FALSE
		 */
		virtual int Download(LPCTSTR lpszURL, LPCTSTR lpszSaveFilename, int nVersion) = 0;

		/**
		 * @brief ȡ�����ء���������ʾ����ʧ��
		 * @return �ɹ�����TURE��ʧ�ܷ���FALSE
		 */
		virtual BOOL CancelDownload() = 0;
	};
    
	/**
	 * @ingroup network
	 * @brief IHttpProtocolHandler ���¼��ص��ӿ�
	 * @sa IHttpProtocolHandler
	 */
	class IHttpProtocolHandlerEvent : public IEvent
	{
	public:
		/**
		 * @brief ���سɹ�
		 */
		virtual void OnDownloadSuccess() = 0;

		/**
		 * @brief ����ʧ��
		 * @param nErrorCode [in] �������
		 */
		virtual void OnDownloadFail(int nErrorCode) = 0;

		/**
		 * @brief ���ؽ��ȷ����仯
		 * @param dwFileSize [in] �����ļ����ܳ��ȣ���λ(�ֽ�)
		 * @param dwDownloadedSize [in] �����ش�С����λ(�ֽ�)
		 */
		virtual void OnProgressChanged(DWORD dwFileSize, DWORD dwDownloadedSize) = 0;

		virtual void OnRedirect(URLARRAY * lpUrlArray) = 0;
	};
};

#endif

// InterfaceName:HttpProtocolHandler
// SubFolderName:HttpProtocolHandler
