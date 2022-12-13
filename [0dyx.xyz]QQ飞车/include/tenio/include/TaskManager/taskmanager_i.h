
#pragma once

#include "teniointerface.h"

namespace Tenio
{
	// 就绪任务是未被阻塞可执行的, 只需等待高优先级任务执行完就可以执行的任务
	#define  ADD_READY				0x00000001

	#define  PERIORITY_HIGH			0x00000002
	#define  PERIORITY_NORMAL		0x00000004
	#define  PERIORITY_LOW			0x00000008

	// 阻塞任务是指需要等待某种条件满足, 才能执行的任务,
	// 在这里指需要显示调用Resume函数才能执行的任务
	#define  ADD_BLOCKED				0xF0000000

	class IActiveTaskManager;
	class IMultiThreadTaskManager;
	class IExecuteThread;
	
	TENIO_MAKE_TYPEID_MAP(IActiveTaskManager, COMPONENTID_ACTIVETASKMANAGER);
	TENIO_MAKE_TYPEID_MAP(IMultiThreadTaskManager, COMPONENTID_MULTITHREADTASKMANAGER);
	TENIO_MAKE_TYPEID_MAP(IExecuteThread, COMPONENTID_EXECUTETHREAD);
	//////////////////////////////////////////////////////////////////////////
	// 任务的执行者
	// 负责具体任务的执行，由TaskManager来管理
	// 多线程版本推荐用windows 消息实现对执行子线程的干预行为
	class ITaskExecutor : public IComponent, public IEventProducer
	{
	public:
			virtual int GetTaskID() = 0;

			// 这个由TaskManager调用，TaskManager分配和管理TaskID
			virtual void SetTaskID(int nTaskID) = 0;

			// 开始执行任务，pTaskParam视具体的任务而定
			virtual int Execute() = 0;

			// 取消任务
			virtual BOOL Cancel() = 0;

			// 停止任务
			virtual void Stop() = 0;

			// 暂时挂起任务
			virtual void Suspend() = 0;

			// 重现启动任务
			virtual int Resume() = 0;

			virtual BOOL IsFinished() = 0;

			// 获取任务的进度
			virtual float GetProgressRate() = 0;

			virtual BOOL Finalize() = 0;	

	};

	/**
	 * @ingroup network
	 * @brief ITaskExecutor 的事件回调接口
	 * @see ITaskExecutor
	 */
	class ITaskExecutorEvent : public IEvent
	{
	public:		
		/**
		 * @brief 下载成功
		 * @param nTaskID [in] 下载任务号
		 */
		virtual void OnTaskSuccess(int nTaskID) = 0;

		/**
		 * @brief 下载失败
		 * @param nTaskID [in] 下载任务号
		 * @param nErrorCode [in] 错误代码
		 */
		virtual void OnTaskFail(int nTaskID, int nErrorCode) = 0;

		/**
		 * @brief 下载进度发生变化
		 * @param nTaskID [in] 下载任务号
		 * @param dwFileSize [in]下载文件的总长度，单位(字节)
		 * @param dwDownloadedSize [in] 已下载大小，单位(字节)
		 */
		virtual void OnProgressChanged(int nTaskID, DWORD dwFirstParam, DWORD dwSecondParam) = 0;

		// 因为重定向之后的Server端路径可能与原来的不同，所以重定向不在IPGroup Downloader层处理，
		// 放在HTTP Downloader层
		//virtual void OnRedirect(int nTaskID, URLARRAY * lpUrlArray) = 0;
	};
   

	//////////////////////////////////////////////////////////////////////////
	// 任务管理器
	// 对任务的添加、删除、获取任务的状态，对任务的调度管理
	//=============================================================================
	// 添加优先级管理
	// Multilevel queue scheduling
	// 设置多个就绪队列，分别赋予不同的优先级，逐级降低, 队列1的优先级最高
	// 共有3个优先级，三个就绪队列
	// 优先级0：如果任务优先级设为0，则该任务被插入第一个就绪对列尾部
	// 优先级1: 如果任务优先级设为1，则该任务被插入第二个就绪对列尾部
	// 优先级2：如果任务优先级设为2，则该任务被插入第三个就绪对列尾部
	//=============================================================================
	// 抢占式任务：
	// 高优先级的任务可以抢占低优先级任务的运行权
	//=============================================================================
	//=============================================================================
	//=============================================================================
	// 只管理异步任务，包括AddTask with ADD_READY 的异步任务
	// 和AddTask with ADD_BLOCKED 的同步和异步任务
	// 待完善的地方：没有任务执行结果的反馈
	//=============================================================================
	//=============================================================================
	// 因为任务管理器管理的任务比较多，所以若采用群发消息方式通知结果，
	// 势必造成较大开销,因此在AddTask()时传入回调指针，任务结束时只回调该指针
	//=============================================================================
	class ITaskManager : public IComponent
	{
	public:
		// 添加任务， dwAddFlags 是增加任务的option， 
		// 如果是ADD_SUSPENDED的话，那么不马上调用Execute
 		virtual int AddTask(
 			ITaskExecutor* pTaskExecutor, 
 			DWORD dwAddFlags,
			DWORD dwPriority = PERIORITY_NORMAL,
 			ITaskExecutorEvent* pTaskExecutorEvent = NULL) = 0;

		// 继续执行等待（Suspended）的任务
		virtual void ResumeAllTask() = 0;			

		// 删除任务
		virtual ITaskExecutor* RemoveTask(int nTaskID) = 0;

		// 获取任务的执行进度
		virtual float GetTaskProgress(int nTaskID) = 0;

		// 继续执行等待（Suspended）的任务
		virtual int ResumeTask(int nTaskID) = 0;

		virtual void StopTask(int nTaskID) = 0;

		// 取消任务执行,需要回滚的任务可能会异步取消
		virtual int CancelTask(int nTaskID) = 0;

		virtual void SetPriority(int nTaskID, int nPeriority) = 0;

		virtual void RaisePriorityBy(int nTaskID, int nPeriority) = 0;

		// 让任务提前执行
		virtual void AdvanceTask(int nTaskID, int nDistance) = 0;

		// 根据任务ID找到任务执行者
		virtual ITaskExecutor* FindTask(int nTaskID) = 0;

		//=============================================================================
		//  FindFirstTask()与FindNextTask()要连续使用
		//=============================================================================
		// 获得第一个任务
		virtual ITaskExecutor* FindFirstTask() = 0;
		
		// 获得下一个任务
		virtual ITaskExecutor* FindNextTask() = 0;

		//virtual ITaskExecutor* FindTask(int nTaskID) = 0;

		virtual void SetMaxThreadPoolSize(int nNumber) = 0;

		virtual int GetMaxThreadPoolSize() = 0;

		virtual int AllocID() = 0;
	};

	/**
	 * @ingroup network
	 * @brief IDownloader 的事件回调接口
	 * @see IDownloader
	 */
// 	class ITaskManagerEvent : public IEvent
// 	{
// 	public:		
// 		/**
// 		 * @brief 下载成功
// 		 * @param nTaskID [in] 下载任务号
// 		 */
// 		virtual void OnTaskSuccess(int nTaskID) = 0;
// 
// 		/**
// 		 * @brief 下载失败
// 		 * @param nTaskID [in] 下载任务号
// 		 * @param nErrorCode [in] 错误代码
// 		 */
// 		virtual void OnTaskFail(int nTaskID, int nErrorCode) = 0;
// 
// 		/**
// 		 * @brief 下载进度发生变化
// 		 * @param nTaskID [in] 下载任务号
// 		 * @param dwFileSize [in]下载文件的总长度，单位(字节)
// 		 * @param dwDownloadedSize [in] 已下载大小，单位(字节)
// 		 */
// 		virtual void OnProgressChanged(int nTaskID, WPARAM wParam, LPARAM lParam) = 0;
// 
// 		// 因为重定向之后的Server端路径可能与原来的不同，所以重定向不在IPGroup Downloader层处理，
// 		// 放在HTTP Downloader层
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

		// ITaskExecutor析构之前调用
		//virtual BOOL ResetTask() = 0;
		virtual BOOL BeginThread(HWND hWnd) = 0;
		//virtual BOOL Cancel() = 0;
		virtual BOOL Finalize() = 0;
	};
};