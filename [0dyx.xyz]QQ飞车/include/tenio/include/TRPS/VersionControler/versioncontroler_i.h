
#pragma once

#include "teniointerface.h"

namespace Tenio
{  
	#define INVALID_VERSION                             -1
	/**
	 * @ingroup network
	 * @brief �汾������
	 *
	 * <p>
	 * �汾�����������Բ�ѯ�����á��ж��ļ��İ汾 <BR>
	 * ������Ը���Ӧ�ã������Ʋ�ͬ���Եİ汾��������
	 * </p>
	 *
	 * <p>
	 * ���磺
	 *  <ol>
	 *	  <li>�������������Ӱ汾�������󣬻�ȡ����������°汾����ʼ��VersionControler��<BR>
	 *	�ύ��VersionDownloader�� �����Ϳ��Ը��ݰ汾�����������Ƿ���Ҫ�������µİ汾��</li>
	 *	  <li>�̳��������Դ�ļ���������ר�ŵġ����ذ汾�����ļ����������¼��ÿ�������ļ��İ汾��<BR>
	 *	���ҽ�ϴ�Server��ȡ�ġ���Ҫ���µİ汾�����ļ��������ж��Ƿ���Ҫ�������µ��ļ���</li>
	 *	  <li>���� </li>
	 *  </ol>
	 *  �汾���������ļ�����Сд����
	 * </p>
     *
	 * @see IVersionControlerEvent
	 */
	class IVersionControler : public IComponent, public IEventProducer
	{
	public:
		/**
		 * @brief �汾�������Ƿ�׼����
		 * @return �汾������׼���÷���TRUE�����򷵻�FALSE
		 */
		virtual	BOOL IsReady() = 0;
        
		/**
		 * @brief ��ȡ�ļ��İ汾
		 * @param lpszFilename [in] �ļ���
		 * @return �ļ��İ汾�š�INVALID_VERSION ��ʾ��Ч�汾��(û���ļ��汾��Ϣ)
		 */
		virtual int GetLocalFileVersion(LPTSTR lpszFileName) = 0;

		virtual int GetUpdateFileVersion(LPTSTR lpszFileName) = 0;
        
		/**
		 * @brief �����ļ��İ汾���������ļ��İ汾�Ÿ�ΪSERVER���ļ��İ汾��
		 * @param lpszFilename [in] �ļ���
		 */
		virtual void UpdateLocalFileVersion(LPTSTR lpszFileName) = 0;
        
		/**
		 * @brief ���ñ����ļ��İ汾��
		 * @param lpszFilename [in] �ļ���
		 * @param nVersion [in] �ļ��İ汾��
		 */
		virtual void SetLocalFileVersion(LPTSTR lpszFileName, int nVersion) = 0;
        
		/**
		 * @brief ɾ���ļ��汾��Ϣ
		 * @param lpszFilename [in] �ļ���
		 * @return �ɹ�ɾ������TRUE��ʧ�ܷ���FALSE
		 * <p>
	     * �ú���ִ��ʧ����Ҫ������ԭ��
	     *  <ol>
		 *    <li>lpszFileName ����0</li>
	     *	  <li>���ذ汾�����ļ������ڣ����ذ汾�����ļ������·����
		 *        LOCAL_VERSION_FILE_DIR �ж���</li>
	     *	  <li>�ڱ��ذ汾�����ļ��У��Ҳ����ļ���lpszFileName�ļ�¼</li>
	     *  </ol>
	     * </p>
		 */
        virtual BOOL DeleteLocalFileVersion(LPTSTR lpszFileName) = 0;

		/**
		 * @brief �ж��ļ��Ƿ������°汾
		 * @param lpszFilename [in] �ļ���
		 * @return �ļ������°汾ʱ����TRUE�����򷵻�FALSE
		 */
		virtual BOOL IsNewestVersion(LPCTSTR lpszFileName) = 0;

		virtual BOOL IsVersionValid(int nVersion) = 0;
	};

	TENIO_MAKE_TYPEID_MAP(IVersionControler, COMPONENTID_VERSIONCONTROLER)
		
	/**
	 * @ingroup network
	 * @brief IVersionControler ���¼��ص��ӿ�
	 *
	 * <p>
     * �� IVersionControler::IsReady Ϊ FALSE�� �ͻ���Ҫ��������¼���
	 * ��VersionControler׼���õ�ʱ�򴥷���� OnReady �¼�
	 * </p>
	 * @see IVersionControler
	 */
 	class IVersionControlerEvent : public IEvent
	{
	public:

		/**
		 * @brief �汾�������Ѿ�׼����
		 */
		virtual void OnReady(IVersionControler* pVersionControler) = 0;
	};

	class IVersionControlerReadyEvent : public IEvent
	{
	public:

		/**
		 * @brief �汾�������Ѿ�׼����
		 */
		virtual void OnReady() = 0;
	};
};
