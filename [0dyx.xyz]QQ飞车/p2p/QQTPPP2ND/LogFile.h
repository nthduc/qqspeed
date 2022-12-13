// LogFile.h: interface for the CLogFile class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGFILE_H__C30D1F79_EF97_4128_9944_7ADD5ABB3D3E__INCLUDED_)
#define AFX_LOGFILE_H__C30D1F79_EF97_4128_9944_7ADD5ABB3D3E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "stdafx.h"
#include "kernel.h"
#include "stdio.h"
#include <vector>
#include <string>
#include "mmsystem.h"

//#define ENABLE_LOG



class CLogFile2 : public CSingleton<CLogFile2>
{
public:
	CLogFile2();
	virtual ~CLogFile2();
	std::vector<std::string> logtext;
	void Log(const char* pFormat,...);

	unsigned int time;
	FILE* pFile;
};


class CTimeLog
{
public :
	int log_limit;
	static int log_level;
	
	CTimeLog(const char* name, const int limit = 5){
		func_name = name;
		time = GetTickCount();		
		log_limit = limit;
		log_limit = 100;
	};

	virtual ~CTimeLog(){
		long del = GetTickCount() - time;

		if(/*log_limit > 0 &&*/del > log_limit)
		{
			CLogFile2* pLog = CLogFile2::GetSingletonPtr();
			if ( pLog )
				pLog->Log("Func %s use %ld mms!",func_name.c_str(),del);
		}

		CLogFile2* pLog = CLogFile2::GetSingletonPtr();
		if ( pLog )
			pLog->Log("Func %s use %ld mms!",func_name.c_str(),del);

	};
		
	std::string func_name;
	unsigned __int64 time;
};

#ifdef ENABLE_LOG
#define LOG_TIME(x, y, t)  CTimeLog  time##x(y, t)
#else
#define LOG_TIME  /##/
#endif


#endif // !defined(AFX_LOGFILE_H__C30D1F79_EF97_4128_9944_7ADD5ABB3D3E__INCLUDED_)
