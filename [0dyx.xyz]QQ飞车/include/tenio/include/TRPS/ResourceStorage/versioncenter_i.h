#ifndef __VERSIONCENTER_I_H__
#define __VERSIONCENTER_I_H__

#include "../../teniointerface.h"
#include "VersionControler/versioncontroler_i.h"
#include "resourcestorage_i.h"

namespace Tenio
{
	class IVersionCenter;
	

	TENIO_MAKE_TYPEID_MAP(IVersionCenter, COMPONENTID_VERSIONCENTER);
	
	class IVersionCenter : public IComponent, public IEventProducer
	{
	public:
		
		virtual BOOL AddVersionControler(
			LPCTSTR lpszDownloadType, 
			IVersionControler* pVersionControler) = 0;

		virtual void RemoveVersionControler(LPCTSTR lpszDownloadType) = 0;

		virtual BOOL SetVersionFilter(
			LPCTSTR lpszDownloadType,  
			BOOL	bUseVersionControl) = 0;

		virtual int GetResource(
			PRESOURCELIST pResourceList, 
			DWORD dwMilliSeconds) = 0;

		//virtual BOOL RemoveTask(int nTaskID) = 0;

		virtual void SetPriority(int nTaskID, int nPeriority) = 0;

		virtual void AddConfig(
			LPCTSTR lpszDownloadType, 
			IDownloadConfig* pDownloadConfig) = 0;

		virtual void RemoveConfig(LPCTSTR lpszDownloadType) = 0;

		//virtual void SetDefaultConfig(IDownloadConfig* pDownloadConfig) = 0;
		virtual void SetDefaultProxy(SOCK5PROXYPARAM * pSock5ProxyParam) = 0;
		virtual void SetTotalBandWidth(DWORD dwBandWidth) = 0;
		virtual BOOL CancelTask(int nTaskID) = 0;

		virtual void AddCommandInterpreter(
			LPCTSTR lpszCommandType, 
			ICommandInterpreter* pCommandInterpreter) = 0;

		virtual void RemoveCommandInterpreter(
			LPCTSTR lpszCommandType) = 0;

		virtual BOOL IsNewestVersion(
			LPCTSTR lpszDownloadType, LPCTSTR lpszFileName) = 0;

		virtual int CheckUpdate() = 0;
	};

	class IVersionCenterEvent : public IEvent
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
		/**
		 * @brief ���سɹ�
		 * @param nTaskID [in] ���������
		 */
		virtual void OnUpdateSuccess() = 0;

		/**
		 * @brief ����ʧ��
		 * @param nTaskID [in] ���������
		 */
		virtual void OnUpdateFail(int nErrorID) = 0;
	};
};

#endif

// InterfaceName:VersionCenter
// SubFolderName:VersionCenter
