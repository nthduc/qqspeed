
#pragma once

#include "teniointerface.h"

namespace Tenio
{
	// ����������δ��������ִ�е�, ֻ��ȴ������ȼ�����ִ����Ϳ���ִ�е�����
	#define  ADD_READY				0x00000001

	#define  PERIORITY_HIGH			0x00000002
	#define  PERIORITY_NORMAL		0x00000004
	#define  PERIORITY_LOW			0x00000008

	// ����������ָ��Ҫ�ȴ�ĳ����������, ����ִ�е�����,
	// ������ָ��Ҫ��ʾ����Resume��������ִ�е�����
	#define  ADD_BLOCKED				0xF0000000

	class IActiveTaskManager;
	class IMultiThreadTaskManager;
	class IExecuteThread;
	
	TENIO_MAKE_TYPEID_MAP(IActiveTaskManager, COMPONENTID_ACTIVETASKMANAGER);
	TENIO_MAKE_TYPEID_MAP(IMultiThreadTaskManager, COMPONENTID_MULTITHREADTASKMANAGER);
	TENIO_MAKE_TYPEID_MAP(IExecuteThread, COMPONENTID_EXECUTETHREAD);
	//////////////////////////////////////////////////////////////////////////
	// �����ִ����
	// ������������ִ�У���TaskManager������
	// ���̰߳汾�Ƽ���windows ��Ϣʵ�ֶ�ִ�����̵߳ĸ�Ԥ��Ϊ
	class ITaskExecutor : public IComponent, public IEventProducer
	{
	public:
			virtual int GetTaskID() = 0;

			// �����TaskManager���ã�TaskManager����͹���TaskID
			virtual void SetTaskID(int nTaskID) = 0;

			// ��ʼִ������pTaskParam�Ӿ�����������
			virtual int Execute() = 0;

			// ȡ������
			virtual BOOL Cancel() = 0;

			// ֹͣ����
			virtual void Stop() = 0;

			// ��ʱ��������
			virtual void Suspend() = 0;

			// ������������
			virtual int Resume() = 0;

			virtual BOOL IsFinished() = 0;

			// ��ȡ����Ľ���
			virtual float GetProgressRate() = 0;

			virtual BOOL Finalize() = 0;	

	};

	/**
	 * @ingroup network
	 * @brief ITaskExecutor ���¼��ص��ӿ�
	 * @see ITaskExecutor
	 */
	class ITaskExecutorEvent : public IEvent
	{
	public:		
		/**
		 * @brief ���سɹ�
		 * @param nTaskID [in] ���������
		 */
		virtual void OnTaskSuccess(int nTaskID) = 0;

		/**
		 * @brief ����ʧ��
		 * @param nTaskID [in] ���������
		 * @param nErrorCode [in] �������
		 */
		virtual void OnTaskFail(int nTaskID, int nErrorCode) = 0;

		/**
		 * @brief ���ؽ��ȷ����仯
		 * @param nTaskID [in] ���������
		 * @param dwFileSize [in]�����ļ����ܳ��ȣ���λ(�ֽ�)
		 * @param dwDownloadedSize [in] �����ش�С����λ(�ֽ�)
		 */
		virtual void OnProgressChanged(int nTaskID, DWORD dwFirstParam, DWORD dwSecondParam) = 0;

		// ��Ϊ�ض���֮���Server��·��������ԭ���Ĳ�ͬ�������ض�����IPGroup Downloader�㴦��
		// ����HTTP Downloader��
		//virtual void OnRedirect(int nTaskID, URLARRAY * lpUrlArray) = 0;
	};
   

	//////////////////////////////////////////////////////////////////////////
	// ���������
	// ���������ӡ�ɾ������ȡ�����״̬��������ĵ��ȹ���
	//=============================================================================
	// ������ȼ�����
	// Multilevel queue scheduling
	// ���ö���������У��ֱ��費ͬ�����ȼ����𼶽���, ����1�����ȼ����
	// ����3�����ȼ���������������
	// ���ȼ�0������������ȼ���Ϊ0��������񱻲����һ����������β��
	// ���ȼ�1: ����������ȼ���Ϊ1��������񱻲���ڶ�����������β��
	// ���ȼ�2������������ȼ���Ϊ2��������񱻲����������������β��
	//=============================================================================
	// ��ռʽ����
	// �����ȼ������������ռ�����ȼ����������Ȩ
	//=============================================================================
	//=============================================================================
	//=============================================================================
	// ֻ�����첽���񣬰���AddTask with ADD_READY ���첽����
	// ��AddTask with ADD_BLOCKED ��ͬ�����첽����
	// �����Ƶĵط���û������ִ�н���ķ���
	//=============================================================================
	//=============================================================================
	// ��Ϊ������������������Ƚ϶࣬����������Ⱥ����Ϣ��ʽ֪ͨ�����
	// �Ʊ���ɽϴ���,�����AddTask()ʱ����ص�ָ�룬�������ʱֻ�ص���ָ��
	//=============================================================================
	class ITaskManager : public IComponent
	{
	public:
		// ������� dwAddFlags �����������option�� 
		// �����ADD_SUSPENDED�Ļ�����ô�����ϵ���Execute
 		virtual int AddTask(
 			ITaskExecutor* pTaskExecutor, 
 			DWORD dwAddFlags,
			DWORD dwPriority = PERIORITY_NORMAL,
 			ITaskExecutorEvent* pTaskExecutorEvent = NULL) = 0;

		// ����ִ�еȴ���Suspended��������
		virtual void ResumeAllTask() = 0;			

		// ɾ������
		virtual ITaskExecutor* RemoveTask(int nTaskID) = 0;

		// ��ȡ�����ִ�н���
		virtual float GetTaskProgress(int nTaskID) = 0;

		// ����ִ�еȴ���Suspended��������
		virtual int ResumeTask(int nTaskID) = 0;

		virtual void StopTask(int nTaskID) = 0;

		// ȡ������ִ��,��Ҫ�ع���������ܻ��첽ȡ��
		virtual int CancelTask(int nTaskID) = 0;

		virtual void SetPriority(int nTaskID, int nPeriority) = 0;

		virtual void RaisePriorityBy(int nTaskID, int nPeriority) = 0;

		// ��������ǰִ��
		virtual void AdvanceTask(int nTaskID, int nDistance) = 0;

		// ��������ID�ҵ�����ִ����
		virtual ITaskExecutor* FindTask(int nTaskID) = 0;

		//=============================================================================
		//  FindFirstTask()��FindNextTask()Ҫ����ʹ��
		//=============================================================================
		// ��õ�һ������
		virtual ITaskExecutor* FindFirstTask() = 0;
		
		// �����һ������
		virtual ITaskExecutor* FindNextTask() = 0;

		//virtual ITaskExecutor* FindTask(int nTaskID) = 0;

		virtual void SetMaxThreadPoolSize(int nNumber) = 0;

		virtual int GetMaxThreadPoolSize() = 0;

		virtual int AllocID() = 0;
	};

	/**
	 * @ingroup network
	 * @brief IDownloader ���¼��ص��ӿ�
	 * @see IDownloader
	 */
// 	class ITaskManagerEvent : public IEvent
// 	{
// 	public:		
// 		/**
// 		 * @brief ���سɹ�
// 		 * @param nTaskID [in] ���������
// 		 */
// 		virtual void OnTaskSuccess(int nTaskID) = 0;
// 
// 		/**
// 		 * @brief ����ʧ��
// 		 * @param nTaskID [in] ���������
// 		 * @param nErrorCode [in] �������
// 		 */
// 		virtual void OnTaskFail(int nTaskID, int nErrorCode) = 0;
// 
// 		/**
// 		 * @brief ���ؽ��ȷ����仯
// 		 * @param nTaskID [in] ���������
// 		 * @param dwFileSize [in]�����ļ����ܳ��ȣ���λ(�ֽ�)
// 		 * @param dwDownloadedSize [in] �����ش�С����λ(�ֽ�)
// 		 */
// 		virtual void OnProgressChanged(int nTaskID, WPARAM wParam, LPARAM lParam) = 0;
// 
// 		// ��Ϊ�ض���֮���Server��·��������ԭ���Ĳ�ͬ�������ض�����IPGroup Downloader�㴦��
// 		// ����HTTP Downloader��
// 		//virtual void OnRedirect(int nTaskID, URLARRAY * lpUrlArray) = 0;
// 	};

	
	class IMultiThreadTaskManager : public ITaskManager, public IEventProducer
	{
// 	public:
// 
// 		virtual int AddTask(
//  			ITaskExecutor* pTaskExecutor, 
//  			DWORD dwAddFlags,
//  			ITaskExecutorEvent* pTaskExecutorEvent) = 0;
	};

	class IActiveTaskManager : public ITaskManager
	{
// 	public:
// 		virtual int AddTask(
//  			ITaskExecutor* pTaskExecutor, 
//  			DWORD dwAddFlags) = 0;

	};
	
	class IExecuteThread : public IComponent
	{
	public:
		virtual BOOL AddTask(ITaskExecutor* pTaskExecutor) = 0;

		// ITaskExecutor����֮ǰ����
		//virtual BOOL ResetTask() = 0;
		virtual BOOL BeginThread(HWND hWnd) = 0;
		//virtual BOOL Cancel() = 0;
		virtual BOOL Finalize() = 0;
	};
};