#include "ov_os.h"

int os_usleep(int us)
{
#ifdef WIN32
	if( INFINITE==(DWORD)us )
		return SleepEx(INFINITE, 0);
	else
		return SleepEx((DWORD)(us/1000), 0);
#else
	struct timespec stReq;
	
	stReq.tv_sec	=	us/1000000;
	stReq.tv_nsec	=	(us % 1000000)*1000;

	return nanosleep(&stReq, NULL);
#endif /* WIN32 */
}

void os_make_daemon(const char* pszRoot)
{
#ifdef WIN32
#else
	pid_t pid;
	const char* pszPath;

	if( NULL==pszRoot )
		pszPath	=	"/";
	else
		pszPath	=	pszRoot;

	pid = fork() ;

	if (pid != 0)
		exit(0);

	setsid();

	signal(SIGINT,  SIG_IGN);
	signal(SIGHUP,  SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	signal(SIGPIPE, SIG_IGN);
	signal(SIGTTOU, SIG_IGN);
	signal(SIGTTIN, SIG_IGN);
	signal(SIGCHLD, SIG_IGN);
	signal(SIGTERM, SIG_IGN);
	signal(SIGHUP,  SIG_IGN);

	pid = fork() ;

	if (pid != 0)
		exit(0);

	if( chdir(pszRoot) )
		exit(2);

	umask(0);
#endif
}

void os_ignore_pipe(void)
{
#ifdef WIN32
#else
        struct sigaction sa;

        sa.sa_handler = SIG_IGN;
        sa.sa_flags = 0;

        sigemptyset(&sa.sa_mask);
        sigaction(SIGPIPE,&sa,NULL);
#endif
}

void os_file_to_macro(char* pszMacro, int iMacro, const char* pszFile)
{
	const char *pszTitle;
	int i;

	assert( pszMacro && pszFile && iMacro>0 );
	
	pszTitle	=	strrchr(pszFile, OS_DIRSEP);

	if( pszTitle )
		pszTitle++;
	else
		pszTitle=	pszFile;

	i	=	0;

	while(i<iMacro && i<(int)strlen(pszTitle) )
	{
		if( OS_DIRDOT==pszTitle[i] )
			pszMacro[i]	=	OS_DIRBAR;
		else
			pszMacro[i]	=	(char)toupper(pszTitle[i]);

		i++;
	}

	pszMacro[i]	=	'\0';
}

int os_get_pagesize(void)
{
#ifdef WIN32
	SYSTEM_INFO sys;

	GetSystemInfo(&sys);

	return (int) sys.dwPageSize;
#else
	return getpagesize();
#endif /* WIN32 */
}

int os_protect(void* pvBase, size_t dwSize, int iProt)
{
#ifdef WIN32
	DWORD dwOldProt;

	if( VirtualProtect(pvBase, dwSize, (DWORD)iProt, &dwOldProt) )
		return 0;
	else
		return -1;
#else
	return mprotect( pvBase, dwSize, iProt );
#endif /* WIN32 */
}

int os_time(char* pszBuff, int iBuff)
{
#ifdef WIN32
	SYSTEMTIME stSys;

	if( !pszBuff || iBuff<=0 )
		return -1;
	
	GetLocalTime(&stSys);

    snprintf(pszBuff, iBuff, "%04d-%02d-%02d %02d:%02d:%02d.%.6d",
            stSys.wYear, stSys.wMonth, stSys.wDay, stSys.wHour, 
			stSys.wMinute, stSys.wSecond, stSys.wMilliseconds*1000);

	return 0;
#else
	struct timeval stTv;
	struct tm stTm;

	if( !pszBuff || iBuff<=0 )
		return -1;

	gettimeofday(&stTv, NULL);
	localtime_r(&stTv.tv_sec, &stTm);

 	snprintf(pszBuff, iBuff, "%04d-%02d-%02d %02d:%02d:%02d.%.6d",
			stTm.tm_year+1900, stTm.tm_mon+1, stTm.tm_mday,
			stTm.tm_hour, stTm.tm_min, stTm.tm_sec, stTv.tv_usec);

	return 0;

#endif /* WIN32 */
}
