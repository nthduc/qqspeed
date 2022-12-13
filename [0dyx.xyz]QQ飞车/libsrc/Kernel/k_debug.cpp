
#include "k_debug.h"
#include <stdio.h>
#include <stdarg.h>
#include "mail.h"
#include <vector>
#include <windows.h>

static bool thisfile_bError = false;
static char thisfile_ErrorBuffer[1024];
static char *logfilename = "client.log";
static char s_ErrorReporter[256] = "...";
static FILE *s_LogFP = NULL;

static CMail s_MailMgr;

class CLogMgr
{
public:
	CLogMgr();
	~CLogMgr();
	bool FindError( const char *perror );
	void AddError( const char *perror );

private:
	std::vector<char *>m_AllError;
};

static int MyGetFileSize( const char *path )
{
	int len = 0;
	FILE *fp = fopen( path, "rb" );
	if( fp != NULL )
	{
		fseek( fp, 0, SEEK_END );
		len = ftell(fp);
		fclose(fp);
	}
	return len;
}

CLogMgr::CLogMgr()
{
	if( MyGetFileSize(logfilename) > 1024*1024 ) //太大的log文件， 自动删除
	{
		DeleteFile( logfilename );
	}

	s_LogFP = fopen( logfilename, "at" );
	if( s_LogFP != NULL ) fprintf( s_LogFP, "\n\n" );
}

CLogMgr::~CLogMgr()
{
	if( s_LogFP != NULL )
	{
		fclose( s_LogFP );
		s_LogFP = NULL;
	}

	for( int i=0; i<m_AllError.size(); i++ )
	{
		free( m_AllError[i] );
	}
}

bool CLogMgr::FindError( const char *perror )
{
	for( int i=0; i<m_AllError.size(); i++ )
	{
		if( strcmp(perror, m_AllError[i]) == 0 ) return true;
	}
	return false;
}

void CLogMgr::AddError( const char *perror )
{
	m_AllError.push_back( _strdup(perror) );
}

static CLogMgr s_LogMgr;

static bool SendMail( const char *perror )
{
	char curdir[1024];
	GetCurrentDirectory( sizeof(curdir), curdir );

	char subject[512];
	sprintf( subject, "幻想online错误报告by%s", s_ErrorReporter );
	s_MailMgr.SetSubject( subject );
	s_MailMgr.SetMessage( perror );
	s_MailMgr.SetRecipient( "delta@tencent.com", "delta" );
	bool ret = s_MailMgr.Send();

	SetCurrentDirectory( curdir );
	return ret;
}

void LogInfo( const char *fmt, ... )
{
	va_list		argptr;
	
	va_start( argptr, fmt );
	vsprintf( thisfile_ErrorBuffer, fmt, argptr );
	va_end( argptr );

	if( s_LogFP != NULL )
	{
		SYSTEMTIME mytime;
		GetLocalTime( &mytime );
		fprintf( s_LogFP, "[%d-%.2d-%.2d %.2d:%.2d:%.2d.%.3d]%s\n", mytime.wYear, mytime.wMonth, mytime.wDay, mytime.wHour,
			mytime.wMinute, mytime.wSecond, mytime.wMilliseconds, thisfile_ErrorBuffer );

		fflush( s_LogFP );
	}
}

void ReportError( const char *fmt, ... )
{
	va_list		argptr;
	
	va_start( argptr, fmt );
	vsprintf( thisfile_ErrorBuffer, fmt, argptr );
	va_end( argptr );

	thisfile_bError = true;

	if( s_LogMgr.FindError(thisfile_ErrorBuffer) ) return;
	else
	{
		s_LogMgr.AddError(thisfile_ErrorBuffer);
		//SendMail(thisfile_ErrorBuffer);

		if( s_LogFP != NULL )
		{
			SYSTEMTIME mytime;
			GetLocalTime( &mytime );
			fprintf( s_LogFP, "[%d-%.2d-%.2d %.2d:%.2d:%.2d.%.3d]%s\n", mytime.wYear, mytime.wMonth, mytime.wDay, mytime.wHour,
				mytime.wMinute, mytime.wSecond, mytime.wMilliseconds, thisfile_ErrorBuffer );

			fflush( s_LogFP );
		}
	}
}

#include <windows.h>
void PopErrorBox( const char *fmt, ... )
{
	va_list		argptr;
	
	va_start( argptr, fmt );
	vsprintf( thisfile_ErrorBuffer, fmt, argptr );
	va_end( argptr );

	thisfile_bError = true;
	SendMail( thisfile_ErrorBuffer );
	MessageBox( NULL, thisfile_ErrorBuffer, "error", MB_OK );
}

void QuitForError( const char *fmt, ... )
{
	va_list		argptr;
	
	va_start( argptr, fmt );
	vsprintf( thisfile_ErrorBuffer, fmt, argptr );
	va_end( argptr );

	thisfile_bError = true;
	SendMail( thisfile_ErrorBuffer );
	MessageBox( NULL, thisfile_ErrorBuffer, "error", MB_OK );
	exit(1);
}

void SetErrorReporter( const char *pname )
{
	int n = sizeof(s_ErrorReporter);
	strncpy( s_ErrorReporter, pname, n );
	s_ErrorReporter[n-1] = 0;
}

