// commandinterpreterdef.h 
#pragma once
//#include "InterpreterAdapter.h"

namespace Tenio
{
	//=============================================================================
	//下面算法只对连续两个异步调用有效：
	//为了在两个异步调用都结束时返回结果，对回调事件计数(EventCount)
	//初始值：EventCount = 6
	//收到一个成功事件：EventCount -= 3
	//收到一个失败事件：EventCount -= 2
	//回调时状态：
	//两个异步调用均成功：EventCount  == 0
	//至少一个调用失败：0 < EventCount < 3
	//=============================================================================
#define  EVENTINITIALVALUE			3
#define  ALLEVENTISSUCCESS(n)       (n == 0)
#define  NOTALLEVENTISSUCCESS(n)    (( n > 0) && (n < 3))
#define  SUCCESSEVENT(n)			(n -= 3)
#define  FAILEVENT(n)				(n -= 2)

	#define  OPERATION_REPLACEFILE			0x00000001
	#define  OPERATION_UNREPLACEFILE		0x00000002
	#define  OPERATION_CLEARREPLACEFILE		0x00000004
	#define  OPERATION_DELETEFILE			0x00000008
	#define  OPERATION_UNDELETEFILE			0x00000010
	#define  OPERATION_CLEARDELETEFILE		0x00000020

	#define  EVENTINITIALVALUE			3
	#define  EVENTINITIALVALUE			3
// 	class CCommandInterpreter;
//	class InterpreterAdapter;

	//typedef int (Tenio::CCommandInterpreter::*ONEPARAMFUNCTION)(LPCTSTR);
	//typedef int (Tenio::CCommandInterpreter::*TWOPARAMFUNCTION)(LPCTSTR, LPCTSTR);
	//typedef int (CCommandInterpreter::*THREEPARAMFUNCTION)(LPCTSTR, LPCTSTR, LPCTSTR);

// 	
// 	typedef struct tagFunctionTable
// 	{
// 		InterpreterAdapter<CCommandInterpreter>* pfnExecute;
// 		InterpreterAdapter<CCommandInterpreter>* pfnCancel;
// 		InterpreterAdapter<CCommandInterpreter>* pfnClear;
//	}FUNCTIONTABLE, *PFUNCTIONTABLE;

//	typedef std::map<std::string, FUNCTIONTABLE> COMMANDTYPE2FUNCTIONTABLE;
//	typedef std::list<COMPRESSFILEINFO>  COMPRESSFILELIST;	


	// 执行COPY命令的文件
	typedef struct tagUpdateFileRecord
 	{
 		TCHAR szSourceFile[MAX_PATH];
		TCHAR szDestFile[MAX_PATH];
 	}
 	UPDATEFILERECORD, *PUPDATEFILERECORD;
	
	typedef std::list<UPDATEFILERECORD>		UPDATEFILERECORDLIST;	
}