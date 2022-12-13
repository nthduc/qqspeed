// taskmanagerdef.h 
#pragma once
//#include "TRPS/Downloader/downloaderdef.h"
#include "taskmanager_i.h"

namespace Tenio
{
		// 就绪任务是未被阻塞可执行的, 只需等待高优先级任务执行完就可以执行的任务
// 	#define  ADD_READY				0x00000001
// 
// 	#define  PERIORITY_HIGH			0x00000010
// 	#define  PERIORITY_NORMAL		0x00000010
// 	#define  PERIORITY_LOW			0x00000100
// 
// 	// 阻塞任务是指需要等待某种条件满足, 才能执行的任务,
// 	// 在这里指需要显示调用Resume函数才能执行的任务
// 	#define  ADD_BLOCKED				0xF0000000

	#define EXECUTE_ASYN				1
	#define EXECUTE_FAIL				-1
	#define EXECUTE_SUCCESS				0

	#define TASKSYNSUCCESS				0

	#define EXECUTE_THREAD_TIMEOUT		10000

	#define IDT_TIMER					0x99

	// 允许同时执行的最大任务数
	#define MAX_ACTIVEEXECUTE_THREADS			100000
	#define MAX_EXECUTE_THREADS				    5
	#define EXECUTE_THREAD_CLASSNAME					_T("ExecuteThreadWindows")
	#define TASK_MANAGER_CLASSNAME					    _T("TaskManagerWindows")
	#define WM_TASKEXECUTETHREAD_SUCCESS				(0x5555 + 0x01)
	#define WM_TASKEXECUTETHREAD_FAIL					(0x5555 + 0x02)
	#define WM_TASKEXECUTETHREAD_PROGRESSCHANGED	    (0x5555 + 0x03)
	#define WM_TASKEXECUTETHREAD_STOP				    (0x5555 + 0x04)

	typedef struct tagTaskInfo
 	{
 	ITaskExecutor*      pTaskExecutor;
	ITaskExecutorEvent* pTaskExecutorEvent;
	DWORD				dwPriority;
	}TASKINFO, *PTASKINFO; 	


	typedef struct _tagProgressInfo
	{
		DWORD dwFirstParam;
		DWORD dwSecondParam;  
	}PROGRESSINFO, *PPROGRESSINFO;		
}