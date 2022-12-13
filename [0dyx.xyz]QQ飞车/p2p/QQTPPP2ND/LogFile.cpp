// LogFile.cpp: implementation of the CLogFile class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "LogFile.h"
#include "../client/gamecfg.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLogFile2::CLogFile2()
{
	 logtext.clear();
}

CLogFile2::~CLogFile2(){
	
//	pFile = fopen("qqt_wch_log.txt","w");
//
//	for(int i = 0; i < logtext.size(); i++)
//	{
//		fprintf(pFile,"%s\n",logtext[i].c_str());
//	}
//	
//	if(pFile) fclose(pFile); pFile = NULL; 


	for(int i = 0; i < logtext.size(); i++)
	{
//		QTLINFO("%s\n",logtext[i].c_str());
	}

};

int CTimeLog::log_level = 0;

void CLogFile2::Log(const char* pFormat,...)
{
	char x[1024];
	va_list argList;
	va_start( argList , pFormat );
	_vsnprintf(x, 1024, pFormat,argList);
	va_end( argList );
	
//	std::string temp = x;
//	logtext.push_back(temp);
	OutputDebugString(x);
}
int t1, t2, t3;

class CCPS
{
public:
	CCPS()
	{
		unsigned __int64 time1 = GetTickCount();
		::Sleep(1000);
		time1 = GetTickCount() - time1;
		CTimeLog::log_level = time1 / 1000;
	}
};

static CCPS g_cps;