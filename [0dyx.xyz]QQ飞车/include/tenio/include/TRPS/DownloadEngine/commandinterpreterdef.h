// commandinterpreterdef.h 
#pragma once
//#include "InterpreterAdapter.h"

namespace Tenio
{
	//=============================================================================
	//�����㷨ֻ�����������첽������Ч��
	//Ϊ���������첽���ö�����ʱ���ؽ�����Իص��¼�����(EventCount)
	//��ʼֵ��EventCount = 6
	//�յ�һ���ɹ��¼���EventCount -= 3
	//�յ�һ��ʧ���¼���EventCount -= 2
	//�ص�ʱ״̬��
	//�����첽���þ��ɹ���EventCount  == 0
	//����һ������ʧ�ܣ�0 < EventCount < 3
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


	// ִ��COPY������ļ�
	typedef struct tagUpdateFileRecord
 	{
 		TCHAR szSourceFile[MAX_PATH];
		TCHAR szDestFile[MAX_PATH];
 	}
 	UPDATEFILERECORD, *PUPDATEFILERECORD;
	
	typedef std::list<UPDATEFILERECORD>		UPDATEFILERECORDLIST;	
}