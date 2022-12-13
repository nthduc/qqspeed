// taskmanagerdef.h 
#pragma once
//#include "TRPS/Downloader/downloaderdef.h"
#include "taskmanager_i.h"

namespace Tenio
{
		// ����������δ��������ִ�е�, ֻ��ȴ������ȼ�����ִ����Ϳ���ִ�е�����
// 	#define  ADD_READY				0x00000001
// 
// 	#define  PERIORITY_HIGH			0x00000010
// 	#define  PERIORITY_NORMAL		0x00000010
// 	#define  PERIORITY_LOW			0x00000100
// 
// 	// ����������ָ��Ҫ�ȴ�ĳ����������, ����ִ�е�����,
// 	// ������ָ��Ҫ��ʾ����Resume��������ִ�е�����
// 	#define  ADD_BLOCKED				0xF0000000

	#define EXECUTE_ASYN				1
	#define EXECUTE_FAIL				-1
	#define EXECUTE_SUCCESS				0

	#define TASKSYNSUCCESS				0

	#define EXECUTE_THREAD_TIMEOUT		10000

	#define IDT_TIMER					0x99

	// ����ͬʱִ�е����������
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