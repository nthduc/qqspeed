
#pragma once

#include "teniointerface.h"
#include "resourcestoragedef.h"
#include "VersionControler/versioncontroler_i.h"
#include "TRPS/Downloader/downloader_i.h"
#include "TRPS/DownloadEngine/commandinterpreter_i.h"
#include "downloadconfigdef.h"

namespace Tenio
{
	class IResourceStorage;
	class IResourceStorageEvent;
	class IDownloadConfig;
	class IResourceChecker;
	
	TENIO_MAKE_TYPEID_MAP(IResourceStorage, COMPONENTID_RESOURCESTORAGE)
	TENIO_MAKE_EVENT_MAP(IResourceStorage, IResourceStorageEvent)
	TENIO_MAKE_TYPEID_MAP(IDownloadConfig, COMPONENTID_DOWNLOADCONFIG)
	TENIO_MAKE_TYPEID_MAP(IResourceChecker, COMPONENTID_RESOURCECHECKER)
	
   /**
	 * @ingroup network
	 * @brief ��Դ��ȡ�ֿ� ���첽��
	 *
	 * <p>
	 * ������ش������µ��ļ�������߿ͻ�����Դ�Ѿ����� <BR>
	 * �����Դ���ڱ��أ�����Server�и��°汾����ô���ظ��°汾����Դ���ɹ���֪ͨ�ͻ�
	 * </p>
	 *
	 * <p>
	 * ʹ������
	 * <ol>	 
	 *   <li>ʹ�� AddConfig() ����ָ���������͵ĵײ��������ò���(��ѡ), 
	 *       ����, �������Ͷ�Ӧ�����ز����������ļ�(config\\Dltype.xml)�ж�ȡ</li>
	 *   <li>ʹ�� AddVersionControler() ����ָ���������͵ĵİ汾������(��ѡ), 
	 *       ����, ��������ʹ��Ĭ�ϵİ汾������</li>
	 *   <li>ʹ�� AddCommandInterpreter() ����ָ������Ľ�����(��ѡ), 
	 *       ����, ʹ��Ĭ�ϵĽ�����</li>
	 *   <li>ʹ�� SetDefaultProxy() ���ô������(��ѡ), 
	 *       ����, �����ļ���ͨ������</li>
	 *   <li>ʹ�� AddEventHandler() ��������ص��¼�ָ�룬��ͬ�Ļص��¼�ָ��ֻ�����һ��
	 *   <li>ʹ�� GetResource() ��������</li>
	 *   <li>��� GetResource()����ֵ����0���򱾴��������첽������ִ�н��ͨ���ص��¼�֪ͨ</li>
	 *   <li>��� GetResource()����ֵ����0����˵�������Ѿ��������°汾����Դ��
	 *       ������ɣ�֮�󲻻��лص��¼�֪ͨ</li>
	 *   <li>��� GetResource()����ֵ����-1���򱾴�����ʧ�ܣ�֮�󲻻��лص��¼�֪ͨ</li>
	 *   <li>����������첽������
	 *       ���¼��ص��ӿ����յ�IResourceStorageEvent::OnGetResourceSuccess() ��Ϣ��
	 *       ��ʾ���سɹ����
	 *       ���¼��ص��ӿ����յ� IResourceStorageEvent::OnGetResourceFail() ��Ϣ��
	 *       ��ʾ����ʧ��</li>
	 *       ���¼��ص��ӿ����յ� IResourceStorageEvent::OnProgressChanged() ��Ϣ��
	 *       ��ʾ�������أ�����ָʾ���ؽ���</li>
	 *   <li>�Ƽ�ȫ��ά��һ��ResourceStorage���󣬳����˳�������Ҫ����ʱ������ResourceStorage����
	 *       ����ǰ������RemoveConfig()��RemoveCommandInterpreter()��RemoveCommandInterpreter()ɾ��
	 *       ǰ�����õ����������汾�����������������������RemoveEventHandler()ע���ص��¼�ָ�롣
	 * </ol>
	 * </p>
	 *
	 * @see IResourceStorageEvent
	 */
	class IResourceStorage : public IComponent, public IEventProducer
	{
	public:
		/**
		 * @brief ��ȡ��Դ
		 *        �ظ�����һ����Դ, ����߸���Դ���ص����ȼ�.
		 *        �������������Դ�����ύ��lpszFileName��pResourceListΪ��λ�ġ�
		 *
		 * @param lpszDownloadType : [in] ��������, ָ����������ʹ�õĲ�����
		 *        ���û�õ���AddConfig()�������ز����������ز����������ļ�(config\\Dltype.xml)�ж��塣
		 *
		 * @param lpszFileName : [in] �����ļ���, �ɴ�·����·����ʾΪ��б����ʽ���磺test/1.zip
		 *
		 * @param dwMilliSeconds [in] ��ʱʱ������λ�����롣0��ʾʹ��Ĭ��ֵ
		 *
		 * @param dwPriority	 [in] �������ȼ���
		 *	  PERIORITY_HIGH :	 �ߣ��ʺ�ʵʱ����
		 *    PERIORITY_NORMAL : �У��ʺ���ͨ����
		 *    PERIORITY_LOW :	 �ͣ��ʺϺ�̨����
		 *
		 * @return  ��������š���1��ʾ��������ʧ��, 0��ʾ������Դ�Ѵ��ڡ�
		 *          ����0��ʾ�������첽���У����ؽ��ͨ���ص��¼�֪ͨ��
		 *
		 * @see �ṹ�嶨��:PRESOURCELIST (��: tagResourceList)
		 *
		 * <p>
	     * �ú���ִ��ʧ����Ҫ������ԭ��
	     *  <ol>
		 *    <li>lpszDownloadType��lpszFileNameΪ��ָ��</li>
		 *    <li>lpszDownloadTypeδ����</li>
	     *	  <li>�汾��������û׼����</li>
	     *  </ol>
	     * </p>
		 */
		virtual int GetResource(
			LPCTSTR lpszDownloadType, 
			LPCTSTR lpszFileName, 
			DWORD dwMilliSeconds,
			DWORD dwPriority) = 0;

		/**
		 * @brief ��ȡ��Դ
		 *        �ظ�����һ����Դ, ����߸���Դ���ص����ȼ�.
		 *        �������������Դ�����ύ��lpszFileName��pResourceListΪ��λ�ġ�
		 * @param pResourceList : [in] �����ļ��б�, �ɴ�·����·����ʾΪ��б����ʽ��
		 *
		 * @param dwMilliSeconds [in] ��ʱʱ������λ�����롣0��ʾʹ��Ĭ��ֵ
		 *
		 * @return  ��������š���1��ʾ��������ʧ��, 0��ʾ������Դ�Ѵ��ڡ�
		 *          ����0��ʾ�������첽���У����ؽ��ͨ���ص��¼�֪ͨ��
		 *
		 * @see �ṹ�嶨��:PRESOURCELIST (��: tagResourceList)
		 *
		 * <p>
	     * �ú���ִ��ʧ����Ҫ������ԭ��
	     *  <ol>
		 *    <li>pResourceList ����0</li>
	     *    <li>pResourceList�е���������δ����</li>
	     *	  <li>�汾��������û׼����</li>
	     *  </ol>
	     * </p>
		 */
		virtual int GetResource(PRESOURCELIST pResourceList, DWORD dwMilliSeconds) = 0;

		/**
		 * @brief ȡ������
		 * @param nTaskID [in] ���������
		 * <p>
	     * �ú���ִ��ʧ����Ҫ������ԭ��
	     *  <ol>
		 *    <li>nTaskID��Ӧ�����񲻴���</li>
		 *    <li>nTaskID��Ӧ��������ִ�н���</li>
	     * </p>
		 */
		virtual BOOL CancelTask(int nTaskID) = 0;
 
		/**
		 * @brief �����������͵İ汾������ [Option]
		 *
		 * <p>
		 * �����������͵İ汾�����������Ի��ڰ汾�����ļ�����Server��ѯ������
		 * ��������ã�����Ĭ�ϵİ汾�������������ļ������汾�Ƚϲ���
		 * �����ڵ�������ǰ���ã�����������Ч
		 * </p>
		 *
		 * @param lpszDownloadType [in]��������
		 * @param pVersionControler [in] �汾������
		 * @return �ɹ�����TRUE��ʧ�ܷ���FALSE
		 * @see IVersionControler
		 */
		virtual BOOL AddVersionControler(LPCTSTR lpszDownloadType, IVersionControler * pVersionControler) = 0;

		/**
		 * @brief ɾ���������͵İ汾������ [Option]
		 * �ײ㲻���汾�����������٣�������Ҫ�Լ�����
		 *
		 * @param lpszDownloadType ��������
		 */
		virtual void RemoveVersionControler(LPCTSTR lpszDownloadType) = 0;

		/**
		 * @brief �����������͵İ汾������Ч(��Ч) [Option]
		 *
		 * <p>
		 * Ĭ�������������͵İ汾���ƾ���Ч��
		 * ���Ҫ��ĳ����������ȡ���汾����, �Ը��������͵��ñ�����
		 * �����ڵ�������ǰ���ã�����������Ч
		 * </p>
		 *
		 * @param lpszDownloadType[in] ��������
		 * @param bValid [in]   TRUE : �汾������Ч; FALSE : �汾������Ч
		 * @return �ɹ�����TRUE��ʧ�ܷ���FALSE
		 * @see IVersionControler
		 */
		virtual BOOL SetVersionFilter(LPCTSTR lpszDownloadType, BOOL bValid) = 0;

		/**
		 * @brief �����������͵ĵײ��������ò��� [Must]
		 *
		 * <p>
		 * ʹ�ñ��������õ�����������, ���ȼ�Ҫ���������ļ�(config\\Dltype.xml)�ж��������������
		 * �������ʹ�ñ���������ĳ�������͵�������DLCFG����ʵ������ʱ, ���������͵��ļ�ʹ��������DLCFG��
		 * ���������ļ����Ƿ��ж���
		 * 
		 * �����ڵ�������ǰ���ã��������������Ч
		 * </p>
		 * @param lpszDownloadType ��������
		 * @param pDownloadConfig [in] ����������
		 * @see IDownloadConfig
		 */
		virtual void AddConfig(LPCTSTR lpszDownloadType, IDownloadConfig * pDownloadConfig) = 0;

		/**
		 * @brief ɾ���������͵������� [Option]
		 * �ײ㲻�������������٣�������Ҫ�Լ�����
		 *
		 * @param lpszDownloadType [in] ��������
		 */
		virtual void RemoveConfig(LPCTSTR lpszDownloadType) = 0;
		

		/**
		 * @brief ������������� [Must]
		 *
		 * <p>
		 * �����ڵ�������ǰ���ã��������������Ч
		 * </p>
		 * @param lpszCommandType [in] ��������
		 * @param pCommandInterpreter [in] ���������
		 * @see ICommandInterpreter
		 */
		virtual void AddCommandInterpreter(
			LPCTSTR lpszCommandType, 
			ICommandInterpreter* pCommandInterpreter) = 0;


		/**
		 * @brief ɾ����������� [Option]
		 * �ײ㲻����������������٣�������Ҫ�Լ�����
		 *
		 * @param lpszCommandType ��������
		 */
		virtual void RemoveCommandInterpreter(
			LPCTSTR lpszCommandType) = 0;

		/**
		 * @brief ���ô������ [Option]
		 *
		 * <p>
		 * �����ڵ�������ǰ���ã��������������Ч
		 * </p>
		 * @param pSock5ProxyParam [in] ������Ϣ
		 * @see SOCK5PROXYPARAM
		 */
		virtual void SetDefaultProxy(SOCK5PROXYPARAM * pSock5ProxyParam) = 0;

		/**
		 * @brief �����ܴ��� [Option]
		 *
		 * <p>
		 * �����ô�����ʹ��Ĭ��ֵ
		 * �����ڵ�������ǰ���ã��������������Ч
		 * </p>
		 * @param nBandWidth [in] ����, ��λ(KB/S)
		 * @see SOCK5PROXYPARAM
		 */
		virtual void SetTotalBandWidth(int nBandWidth) = 0;

		/**
		 * @brief ������ [Option]
		 *
		 * <p>
		 * �ļ�����ʧ�ܣ������سɹ�������ʧ�ܣ����������д���ű��ļ���
		 * ���ñ���������ӽű��ļ�������ִ����������
		 * </p>
		 * @return  ���½����
		 *          ��1��ʾ����ʧ��,
		 *			0��ʾ���³ɹ�,
		 *          1��ʾ�������첽���У����½��ͨ���ص��¼�֪ͨ��
		 */
		virtual int CheckUpdate() = 0;

		/**
		 * @ingroup ���������������ȼ�
		 * @brief �����������������������������߳���ʱ�������������ھ������С�
		 *        ʹ�ñ���������ĳ��������ȼ������Ըı�������ھ������е�λ�ã�
		 *        Ӱ��������ִ��ʱ��
		 *
		 * @param  nTaskID [in] �����
		 * @param  nPeriority [in] ���ȼ���
		 *         ADD_READY_1 : �����ȼ���ֻҪ�п����̣߳��ü��������������ִ��
		 *         ADD_READY_2 : �����ȼ���
		 *         ADD_READY_3 : �����ȼ���
		 */
		virtual void SetPriority(int nTaskID, int nPeriority) = 0;
	};
    
	/**
	 * @ingroup network
	 * @brief IResourceStorage ���¼��ص��ӿ�
	 * @see IResourceStorage   
	 */
	class IResourceStorageEvent : public IEvent
	{
	public:
		/**
		 * @brief ���سɹ�
		 * @param nTaskID [in] ���������
		 */
		virtual void OnGetResourceSuccess(int nTaskID) = 0;

		/**
		 * @brief ����ʧ��
		 * @param nTaskID [in] ���������
		 * @param nErrorCode �������
		 */
		virtual void OnGetResourceFail(int nTaskID, int nErrorCode) = 0;

		/**
		 * @brief ���ؽ��ȷ����仯
		 * @param nTaskID [in] ���������
		 * @param dwFileSize [in]�����ļ����ܳ��ȣ���λ(�ֽ�)
		 * @param dwDownloadedSize [in] �����ش�С����λ(�ֽ�)
		 */
		virtual void OnProgressChanged(
			int nTaskID, DWORD dwFileSize, DWORD dwDownloadedSize) = 0;

		/**
		 * @brief ������ִ�гɹ�
		 *        ֻ�е���CheckUpdate()ʱ���ſ����յ�����Ϣ
		 */
		virtual void OnUpdateSuccess() = 0;

		/**
		 * @brief ������ִ��ʧ��
		 *        ֻ�е���CheckUpdate()ʱ���ſ����յ�����Ϣ
		 * @param nErrorCode [in] �������
		 */
		virtual void OnUpdateFail(int nErrorID) = 0;
	};

	/**
	 * @ingroup network
	 * @brief ���������� 
	 *
	 * <p>
	 * ��Ź������ز����������������ط�����Ⱥ���IP��PORT
	 * P2P�����ļ���·�����������ļ��ڷ������˵Ĺ���·����
	 * �����ļ��ڿͻ��˵Ĺ�������·��
	 * </p>
	 */
	class IDownloadConfig : public IComponent
	{
	public:
		
		/**
		 * @brief ��ʼ������������ִ�в�����Ч�Լ��
		 * @param pstDownloadTypeParam [in] ���ز���
		 * @return ������Ч���򷵻�TRUE;
		 *         ���򣬷���FALSE
		 * <p>
	     * ���²�����Ч
	     *  <ol>
		 *    <li>��������·��Ϊ��</li>
		 *    <li>��ʱ����·��Ϊ��</li>
		 *    <li>����·��Ϊ��</li>
		 *    <li>������IP�б�Ϊ��</li>
	     *  </ol>
	     * </p>
		 * @see PDOWNLOADTYPEPARAM
		 */
		virtual BOOL Init(PDOWNLOADTYPEPARAM pstDownloadTypeParam) = 0;

		/**
		 * @brief ���õ�ǰ�ļ�
		 *        �ú������ļ��������·��ƴ����һ���γ�����·��
		 *
		 * <p>
		 * ������Ҫ�������GETϵ�к���֮ǰ����
		 * </p>
		 * @param lpszFileName [in] �ļ��������Դ�·��
		 * @return �ɹ�����TRUE,ʧ�ܷ���FALSE
		 */
		virtual BOOL SetCurrentFile(LPCTSTR lpszFileName) = 0;

		/**
		 * @brief ������ش���
		 *
		 * @param dwBandWidth [out] ����
		 * @return ������Ч����TRUE,��Ч����FALSE
		 */
		virtual BOOL GetBandWidth(DWORD& dwBandWidth) = 0;

		/**
		 * @brief ������ط�����Ⱥ���IP��PORT
		 * @param  pIPPortPackets [out] ָ�����ط�����Ⱥ���IP��PORT
		 * �Ľṹ���ָ��
		 * @return ���pIPPortPackets != NULL��������������¼�����ط�����
		 * Ⱥ���IP��PORT��Ч���򷵻�TRUE�����򣬷���FALSE
		 */
		virtual BOOL GetIPPortPackets(IPPORTPACKETS * pIPPortPackets) = 0;

		/**
		 * @brief ���P2P���ò���
		 * @param pP2PParamList [out] P2P���ò���
		 * @return pP2PParamList != NULL��P2P������Ч
		 *         �򷵻�TRUE�����򣬷���FALSE
		 */
		virtual BOOL GetP2PParam(P2PPARAMLIST* pP2PParamList) = 0;

		/**
		 * @brief ���Server�ϵ������ļ�·��
		 * @param lpszLocalPath [out] ����Server·�����ַ���
		 * @return lpszLocalPath != NULL��Server·���ǿգ�
		 *         �򷵻�TRUE�����򣬷���FALSE
		 */
		virtual BOOL GetServerPath(LPTSTR lpszServerPath, int nCharCount) = 0;

		/**
		 * @brief ��������ļ��ı��ر���·��
		 * �ļ������ع����������ص�LocalPath���ٸ��ǵ�SubstitutePath
		 * @param lpszLocalPath [out] ���������ļ��ı��ر���·�����ַ���
		 * @return lpszLocalPath != NULL�������ļ��ı��ر���·���ǿգ�
		 *         �򷵻�TRUE�����򣬷���FALSE
		 */
		virtual BOOL GetLocalPath(LPTSTR lpszLocalPath, int nCharCount) = 0;

		/**
		 * @brief ���δ����ļ�����ʱ�ļ���
		 
		 * @param lpszTemPath [out] ������ʱ�ļ������ַ���
		 * @return lpszTemPath != NULL����ʱ�ļ����ǿգ�
		 *         �򷵻�TRUE�����򣬷���FALSE
		 */
		virtual BOOL GetTemporaryPath(LPTSTR lpszTemPath, int nCharCount) = 0;

		/**
		 * @brief ��������ļ��ĸ���·��
		 * @param lpszSubstitutePath [out] ���ո���·�����ַ���
		 * @return lpszSubstitutePath != NULL�������ļ��ĸ���·���ǿգ�
		 *         �򷵻�TRUE�����򣬷���FALSE
		 */
		virtual BOOL GetSubstitutePath(LPTSTR lpszSubstitutePath, int nCharCount) = 0;


		/**
		 * @brief ����ļ����سɹ���ִ�е������ַ���
		 * @param lpszCommand [out] ����������ַ���
		 * @return lpszSubstitutePath != NULL������ǿգ�
		 *         �򷵻�TRUE�����򣬷���FALSE
		 */
		virtual BOOL GetCommand(LPTSTR lpszCommand, int nCharCount) = 0;

		/**
		 * @brief ���ʹ�õ�����������
		 *
		 * @param lpszDownloadType [out] �������������Ƶ��ַ���
		 *
		 * ���ڵ�����������������:
		 * 1��HTTPDOWNLOADER  ִ��http����
		 * 2��IPGROUPDOWNLOADER 
		 * �ȳ���P2P���أ�ʧ�ܺ���http����,
		 * ����ж��IP,��ÿ��ip�Թ��󣬲ű�������ʧ��
		 * 3��P2PDOWNLOADER  ִ��P2P����
		 *
		 * @return �ɹ�����TRUE,ʧ�ܷ���FALSE
		 * @see
		 */
		virtual BOOL GetDownloaderType(LPTSTR lpszDownloadType) = 0;
	};

	/**
	 * @ingroup network
	 * @brief ���汾�����ļ��м�¼��ÿ���ļ������������°汾������
	 *        ע���汾�����ļ�Ĭ����"config\\UpdateVersionFile.xml"
	 * <p>
	 * Strategy :
	 * 1�������汾�����ļ��е�ÿ���ļ����Ƚϰ汾�š�����ļ��������°汾�������ء�
	 * 2��ÿ���������20���ļ���
	 * 3��ÿ���ļ�֮��û����ϵ��
	 * 4������ͬʱ���ض���ļ��б�
	 * </p>
	 * @param
	 * @return
	 * @see
	 */
	class IResourceChecker : public IComponent, public IEventProducer
	{
	public:
		/**
		 * @brief ���ݰ汾�����ļ��ļ�¼��ȡ��Դ
		 *
		 * @param pResourceStorage : [in] ��Դ��ȡ�ֿ�, ���ⲿ���ɲ����룬һ��Ϊȫ��Ψһ�Ķ���
		 *
		 * @return  ��������š���1��ʾ����ʧ��, 0��ʾ�����ļ��Ѵ����������°汾��
		 *          1��ʾ�������첽���У����ؽ��ͨ���ص��¼�֪ͨ��
		 *
		 * @see IResourceStorage
		 */
		virtual int CheckAndGetResource(IResourceStorage* pResourceStorage) = 0;

		/**
		 * @ingroup ����Դ��ȡ�ֿ���Ϊ��
		 * @brief 1���ú�����ȡ�����ء�
		 *		  2�����ⲿ��Դ��ȡ�ֿ�����֮ǰ��֪ͨ��������Դ��ȡ�ֿ���Ч
		 *
		 */
		virtual void ResetResourceStorage() = 0;
	};

	/**
	 * @ingroup network
	 * @brief IResourceStorage ���¼��ص��ӿ�
	 * @see IResourceStorage   
	 */
	class IResourceCheckerEvent : public IEvent
	{
	public:	
		
		/**
		 * @brief ���سɹ�
		 */
		virtual void OnGetResourceSuccess() = 0;

		/**
		 * @brief ����ʧ��
		 * @param nErrorCode �������
		 */
		virtual void OnGetResourceFail(int nErrorCode) = 0;
	};

	};
