#include "StdAfx.h"
#include "DownloadLog.h"

#include <cstdio>
#include <stdlib.h>

#include <windows.h>
#include <time.h>

HWND g_hLogWindow = NULL;
void __qqt_ipc_log(UINT uColor, char* pFormat, ...)
{
	if(!::IsWindow(g_hLogWindow))
	{
		g_hLogWindow = NULL;
	}

	if(!g_hLogWindow)
	{
		if(NULL == (g_hLogWindow = ::FindWindow( NULL , "QQTHelpLog")))
		{
			return;
		}
	}
	
	char x[1024];

	va_list va;
	va_start(va, pFormat);
    _vsnprintf(x, 1024, pFormat, va);
    va_end(va);

	COPYDATASTRUCT cds; 
	cds.dwData = uColor;
	cds.cbData = 1024;
	cds.lpData = (void*)x; 
	::SendMessage( g_hLogWindow, WM_COPYDATA , (WPARAM)NULL , (LPARAM) (LPVOID)&cds );
}




//////////////////////////////////////////////////////////////////////////
CLog * CLog::s_pLog = NULL;

//////////////////////////////////////////////////////////////////////////
CLog * CLog::GetInstance()
{
    static CLog sLog;
    return &sLog;
}

//////////////////////////////////////////////////////////////////////////
CLog::CLog()
{
    strncpy(m_szFilename, "QQTP2PStatis.htm", 256);

    FILE * fp = fopen(m_szFilename, "a+t");
    if (fp == NULL)
    {
        return;
    }

    time_t ttime;
    tm * pCurTime;
    char cLogStart[128];

    time(&ttime);
    pCurTime= localtime(&ttime);
    strftime(cLogStart, 128, "日志日期：%Y年%m月%d日", pCurTime);
    
    fprintf(fp, "<HTML>\n<TITLE>%s</TITLE>\n"
        "<BODY BGCOLOR= #000000>\n"
        "<FONT COLOR= #BFBFBF>QQ堂运行日志 </FONT><BR><BR>\n"
        "<FONT COLOR= #BFBFBF>%s</FONT><BR><BR>\n"
        "</BODY>\n</HTML>", cLogStart, cLogStart);

    fflush(fp);

    fclose(fp);
}

CLog::~CLog()
{
}

//////////////////////////////////////////////////////////////////////////
void CLog::Write(enum CLog::LOG_COLOR logColor, char * szString, ...)
{
    FILE * fp;
    va_list va;
    time_t ttime;
    tm* pCurTime;
    char szParsedString[4000];
    char szLogTime[32];  

    time( &ttime );
    pCurTime= localtime( &ttime );
    strftime( szLogTime, 32, "<PRE>[%H时%M分%S秒]", pCurTime );

    va_start(va, szString);
    _vsnprintf(szParsedString, 4000, szString, va);
    va_end(va);

    fp = fopen(m_szFilename, "r+");
    if (fp == NULL)
    {
        return;
    }

    fseek(fp, -16, SEEK_END);
    fprintf(fp, "<FONT COLOR= \"%s\">%s %s</PRE></FONT>\n"
        "</BODY>\n</HTML>", GetColor(logColor), szLogTime, szParsedString );
    fclose(fp);
}

char * CLog::GetColor(CLog::LOG_COLOR logColor)
{
    switch(logColor)
    {
    case LOG_COLOR_RED:
        return "#FF0000";
        break;
    case LOG_COLOR_GREEN:
        return "#00BF00";
        break;
    case LOG_COLOR_YELLOW:
        return "#BFBF00";
        break; 
    case LOG_COLOR_WHITE:
        return "#FFFFFF";
        break;
    default:
        return "#FFFFFF";
        break;
    }
}

void flog_warning(char *fmt, ...)
{
	va_list va;
	char szParsedString[1024];
	va_start(va, fmt);
	_vsnprintf(szParsedString, 1024, fmt, va);
	va_end(va);
	
	CLog::GetInstance()->Write(CLog::LOG_COLOR_YELLOW, "%s", szParsedString);
}

void flog_error(char *fmt, ...)
{
	va_list va;
	char szParsedString[1024];
	va_start(va, fmt);
	_vsnprintf(szParsedString, 1024, fmt, va);
	va_end(va);
	
	CLog::GetInstance()->Write(CLog::LOG_COLOR_RED, "%s", szParsedString);
}

void flog_success(char *fmt, ...)
{
	va_list va;
	char szParsedString[1024];
	va_start(va, fmt);
	_vsnprintf(szParsedString, 1024, fmt, va);
	va_end(va);
	
	CLog::GetInstance()->Write(CLog::LOG_COLOR_GREEN, "%s", szParsedString);
}


//
void ipc_log_warning(char *fmt, ...)
{
	va_list va;
	char szParsedString[1024];
	va_start(va, fmt);
	_vsnprintf(szParsedString, 1024, fmt, va);
	va_end(va);

	__qqt_ipc_log(0xBFBF00, szParsedString);
}

void ipc_log_error(char *fmt, ...)
{
	va_list va;
	char szParsedString[1024];
	va_start(va, fmt);
	_vsnprintf(szParsedString, 1024, fmt, va);
	va_end(va);

	__qqt_ipc_log(0xFF0000, szParsedString);
}

void ipc_log_success(char *fmt, ...)
{
	va_list va;
	char szParsedString[1024];
	va_start(va, fmt);
	_vsnprintf(szParsedString, 1024, fmt, va);
	va_end(va);
	__qqt_ipc_log(0x00FF00, szParsedString);
}

void double_log_warning(char *fmt, ...)
{
	va_list va;
	char szParsedString[1024];
	va_start(va, fmt);
	_vsnprintf(szParsedString, 1024, fmt, va);
	va_end(va);

	flog_warning(szParsedString);
	ipc_log_warning(szParsedString);
}

void double_log_error(char *fmt, ...)
{
	va_list va;
	char szParsedString[1024];
	va_start(va, fmt);
	_vsnprintf(szParsedString, 1024, fmt, va);
	va_end(va);	

	flog_error(szParsedString);
	ipc_log_error(szParsedString);
}

void double_log_success(char *fmt, ...)
{
	va_list va;
	char szParsedString[1024];
	va_start(va, fmt);
	_vsnprintf(szParsedString, 1024, fmt, va);
	va_end(va);
	
	flog_success(szParsedString);
	ipc_log_success(szParsedString);
}
void printbin_net(FILE *p, byte *pBuf, int iLen)
{
	if(p)
	{
		CString strMsg, strTemp;
		for(int i = 0; i < iLen; i ++)
		{
			strTemp.Format("%02X ", pBuf[i]);
			strMsg += strTemp;
			if(15 == i % 16) strMsg += "\r\n";
		}
		strMsg += "\r\n";
		fwrite(strMsg, strMsg.GetLength(), 1, p);
	}
}

void net_log(BOOL bSend, int nSeq, int msgid, int dataid, int nLen, char *str, char *key, char *fmt, ...)
{
	static BOOL bFileOpened = FALSE;
	va_list va;
	char szParsedString[256];
	va_start(va, fmt);
	_vsnprintf(szParsedString, 256, fmt, va);
	va_end(va);
	
    time_t ttime;
    tm * pCurTime;
    char szLogTime[32];  
	FILE *fp;

    time( &ttime );
    pCurTime= localtime( &ttime );
    strftime( szLogTime, 32, "[%H时%M分%S秒]", pCurTime );

	fp = fopen("qqtnetLog.txt", "a+b");
    if (fp == NULL)
    {
        return;
    }
	static bool bFirst = true;
	if(bFirst)
	{
		char *pp = "==========================================================\r\n\r\n";
		fwrite(pp, strlen(pp), 1, fp);
		bFirst = false;
	}
	
	char strLogString[300];
	_snprintf(strLogString, 300, "\r\ntime = %s, seq = %d, msgid = %d, dataid = %d, len = %d, extrainfo = %s\r\n", szLogTime, nSeq, msgid, dataid, nLen, szParsedString);
	fwrite(strLogString, strlen(strLogString), 1, fp);

	if (bSend)
	{
		fprintf(fp, "msgcontent = \r\n");
		
		//fwrite(str, nLen, 1, fp);
		printbin_net(fp, (byte*)str, nLen);
		fprintf(fp, "key = ");
		printbin_net(fp, (byte*)key, 16);
		char *str2 = "\r\n";
		fwrite(str2, strlen(str2), 1, fp);
	}
	
	fclose(fp);
}

void net_log_recv(int nSeq, int msgid, int dataid, int nLen, char *fmt, ...)
{
	static BOOL bFileOpened = FALSE;
	va_list va;
	char szParsedString[256];
	va_start(va, fmt);
	_vsnprintf(szParsedString, 256, fmt, va);
	va_end(va);
	
    time_t ttime;
    tm * pCurTime;
    char szLogTime[32];  
	FILE *fp;

    time( &ttime );
    pCurTime= localtime( &ttime );
    strftime( szLogTime, 32, "[%H时%M分%S秒]", pCurTime );

	fp = fopen("qqtnetLogRecv.txt", "a+b");
    if (fp == NULL)
    {
        return;
    }
	static bool bFirst = true;
	if(bFirst)
	{
		char *pp = "==========================================================\r\n\r\n";
		fwrite(pp, strlen(pp), 1, fp);
		bFirst = false;
	}
	
	char strLogString[300];
	_snprintf(strLogString, 300, "\r\ntime = %s, seq = %d, msgid = %d, dataid = %d, len = %d, extrainfo = %s\r\n", szLogTime, nSeq, msgid, dataid, nLen, szParsedString);
	fwrite(strLogString, strlen(strLogString), 1, fp);
	
	fclose(fp);

}

void net_log_send(int nSeq, int msgid, int dataid, int nLen, char *str, char *key, char *fmt, ...)
{
	static BOOL bFileOpened = FALSE;
	va_list va;
	char szParsedString[256];
	va_start(va, fmt);
	_vsnprintf(szParsedString, 256, fmt, va);
	va_end(va);
	
    time_t ttime;
    tm * pCurTime;
    char szLogTime[32];  
	FILE *fp;

    time( &ttime );
    pCurTime= localtime( &ttime );
    strftime( szLogTime, 32, "[%H时%M分%S秒]", pCurTime );

	fp = fopen("qqtnetLogSend.txt", "a+b");
    if (fp == NULL)
    {
        return;
    }
	static bool bFirst = true;
	if(bFirst)
	{
		char *pp = "==========================================================\r\n\r\n";
		fwrite(pp, strlen(pp), 1, fp);
		bFirst = false;
	}
	
	char strLogString[300];
	_snprintf(strLogString, 300, "\r\ntime = %s, seq = %d, msgid = %d, dataid = %d, len = %d, extrainfo = %s\r\n", szLogTime, nSeq, msgid, dataid, nLen, szParsedString);
	fwrite(strLogString, strlen(strLogString), 1, fp);

	fprintf(fp, "msgcontent = \r\n");
	//fwrite(str, nLen, 1, fp);
	printbin_net(fp, (byte*)str, nLen);
	fprintf(fp, "key = ");
	printbin_net(fp, (byte*)key, 16);
	char *str2 = "\r\n";
	fwrite(str2, strlen(str2), 1, fp);
	
	fclose(fp);
}

void net_log_send_now(int nLen, char *pBuf)
{	
    time_t ttime;
    tm * pCurTime;
    char szLogTime[32];  
	FILE *fp;

    time( &ttime );
    pCurTime= localtime( &ttime );
    strftime( szLogTime, 32, "[%H时%M分%S秒]", pCurTime );

	fp = fopen("qqtnetLogSend.txt", "a+b");
    if (fp == NULL)
    {
        return;
    }
	static bool bFirst = true;
	if(bFirst)
	{
		char *pp = "==========================================================\r\n\r\n";
		fwrite(pp, strlen(pp), 1, fp);
		bFirst = false;
	}
	
	char strLogString[300];
	_snprintf(strLogString, 300, "\r\ntime = %s, len = %d\r\n", szLogTime, nLen);
	fwrite(strLogString, strlen(strLogString), 1, fp);

	fprintf(fp, "msgcontent = \r\n");
	//fwrite(str, nLen, 1, fp);
	printbin_net(fp, (byte*)pBuf, nLen);
	char *str2 = "\r\n";
	fwrite(str2, strlen(str2), 1, fp);
	
	fclose(fp);
}


void net_log_send_now_other_file(int nLen, char *pBuf)
{	
    time_t ttime;
    tm * pCurTime;
    char szLogTime[32];  
	FILE *fp;

    time( &ttime );
    pCurTime= localtime( &ttime );
    strftime( szLogTime, 32, "[%H时%M分%S秒]", pCurTime );

	fp = fopen("qqtnetLogSendNow.txt", "a+b");
    if (fp == NULL)
    {
        return;
    }
	static bool bFirst = true;
	if(bFirst)
	{
		char *pp = "==========================================================\r\n\r\n";
		fwrite(pp, strlen(pp), 1, fp);
		bFirst = false;
	}
	
	char strLogString[300];
	_snprintf(strLogString, 300, "\r\ntime = %s, len = %d\r\n", szLogTime, nLen);
	fwrite(strLogString, strlen(strLogString), 1, fp);

	fprintf(fp, "msgcontent = \r\n");
	//fwrite(str, nLen, 1, fp);
	printbin_net(fp, (byte*)pBuf, nLen);
	char *str2 = "\r\n";
	fwrite(str2, strlen(str2), 1, fp);
	
	fclose(fp);
}





