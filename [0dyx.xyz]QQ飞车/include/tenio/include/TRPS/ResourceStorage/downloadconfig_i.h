
#pragma once

#include "teniointerface.h"
#include "downloadconfigdef.h"
namespace Tenio
{
	class IDownloadConfig;
	
	TENIO_MAKE_TYPEID_MAP(IDownloadConfig, COMPONENTID_DOWNLOADCONFIG)
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
		 * @param stDownloadTypeParam [in] ���ز���
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
		 */
		virtual BOOL Init(PDOWNLOADTYPEPARAM pstDownloadTypeParam) = 0;
		virtual BOOL SetCurrentFile(/*LPCTSTR lpszDownloadType, */LPCTSTR lpszFileName) = 0;
		//virtual BOOL GetCurrentFile(LPTSTR lpszDownloadType, 
		//	int nCharCountForDownloadType, LPTSTR lpszFileName, int nCharCountForFileName) = 0;
		/**
		 * @brief ��ô���
		 * @param nBandWidth [out] ����
		 * @return ���������������¼�Ĵ������0���򷵻�TRUE;
		 *         ���򣬷���FALSE
		 */
		virtual BOOL GetBandWidth(DWORD& dwBandWidth) = 0;

        /**
         * @brief ��ô���
         * @param pSock5ProxyParam [out]  ָ�������Ϣ�Ľṹ���ָ��
         * @return ���pSock5ProxyParam != NULL��������������¼�Ĵ���IP
		 *         �ǿգ��򷵻�TRUE;����,����FALSE
         */
		//virtual BOOL GetProxy(SOCK5PROXYPARAM * pSock5ProxyParam) = 0;	

		/**
		 * @brief ������ط�����Ⱥ���IP��PORT
		 * @param  pIPPortPackets [out] ָ�����ط�����Ⱥ���IP��PORT
		 * �Ľṹ���ָ��
		 * @return ���pIPPortPackets != NULL��������������¼�����ط�����
		 * Ⱥ���IP��PORT��Ч���򷵻�TRUE�����򣬷���FALSE
		 */
		virtual BOOL GetIPPortPackets(IPPORTPACKETS * pIPPortPackets) = 0;

		/**
		 * @brief ���P2P�����ļ���·����
		 * @param lpszConfigFile [out] ����P2P�����ļ���·�������ַ���
		 * @return lpszConfigFile != NULL��P2P�����ļ���·�����ǿգ�
		 *         �򷵻�TRUE�����򣬷���FALSE
		 */
		virtual BOOL GetP2PParam(P2PPARAMLIST* pP2PParamList) = 0;

		/**
		 * @brief ���Server�ϵ��ļ�·��
		 * @param lpszLocalPath [out] ����Server·�����ַ���
		 * @return lpszLocalPath != NULL��Server·���ǿգ�
		 *         �򷵻�TRUE�����򣬷���FALSE
		 */
		virtual BOOL GetServerPath(LPTSTR lpszServerPath, int nCharCount) = 0;

		/**
		 * @brief ��������ļ��ı��ر���·��
		 * @param lpszLocalPath [out] ���������ļ��ı��ر���·�����ַ���
		 * @return lpszLocalPath != NULL�������ļ��ı��ر���·���ǿգ�
		 *         �򷵻�TRUE�����򣬷���FALSE
		 */
		virtual BOOL GetLocalPath(LPTSTR lpszLocalPath, int nCharCount) = 0;

		virtual BOOL GetTemporaryPath(LPTSTR lpszTemPath, int nCharCount) = 0;

		virtual BOOL GetSubstitutePath(LPTSTR lpszSubstitutePath, int nCharCount) = 0;

		virtual BOOL GetCommand(LPTSTR lpszCommand, int nCharCount) = 0;

		virtual BOOL GetDownloaderType(LPTSTR lpszDownloadType) = 0;
	};
	};