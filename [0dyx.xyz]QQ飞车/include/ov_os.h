#ifndef OV_OS_H
#define OV_OS_H

#ifdef WIN32
	#include <tchar.h>
	#include <winsock2.h>
	#include <windows.h>
	#include <time.h>
	#include <sys/types.h>
	#include <sys/stat.h>
	#include <sys/timeb.h>
	#include <fcntl.h>
	#include <io.h>
	#include <signal.h>
#else
	#include <unistd.h>
	#include <fcntl.h>
	#include <sys/types.h>
	#include <sys/time.h>
	#include <sys/select.h>
	#include <sys/socket.h>
	#include <sys/stat.h>
	#include <sys/un.h>
	#include <sys/ipc.h>
	#include <sys/shm.h>
	#include <sys/mman.h>
	#include <sys/sem.h>
	#include <netdb.h>
	#include <netinet/in.h>
	#include <pthread.h>
	#include <signal.h>
#endif

#include <errno.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>
#include <stdarg.h>

#define WAIT_INFINITE		-1

#define UNIX_PATH_MAX		108

#define MS_TO_TV(tv, ms)	do					\
				{					\
					tv.tv_sec = ms/1000; 		\
					tv.tv_usec = (ms % 1000)*1000;	\
				} while(0)

#define TV_TO_MS(ms, tv)	ms = tv.tv_sec*1000 + tv.tv_usec/1000

#define OS_DIRDOT			'.'
#define	OS_DIRBAR			'_'

#ifdef WIN32
	#define PROT_NONE			PAGE_NONE
	#define PROT_EXEC			PAGE_EXECUTE
	#define PROT_READ			PAGE_READONLY
	#define PROT_RDWR			PAGE_READWRITE

	#define PTHREAD_ONCE_INIT		-1
	#define pthread_once_t			long
	#define pthread_key_t			DWORD
	#define getpid				GetCurrentProcessId
	#define pthread_once(once, func)	{			      \
			if( 0==InterlockedIncrement(once) )		      \
				func();					      \
		}
	#define pthread_key_create(pkey, func)	*pkey	= TlsAlloc()
	#define pthread_setspecific(key, arg)	TlsSetValue(key, arg)
	#define pthread_getspecific(key)	TlsGetValue(key)

	#define socketerrno			WSAGetLastError()

	#define SOCKETEWOULDBLOCK		WSAEWOULDBLOCK
	#define SOCKETEAGAIN			WSAEWOULDBLOCK

	#define snprintf			_snprintf
	#define vsnprintf			_vsnprintf
	#define write				_write
	#define read				_read

//	#define	strnlen				_strncnt

	#define	strnlen(p, sz)		strlen(p)

	#define longlong			__int64
	#define	ulonglong			unsigned __int64
	#define	OS_DIRSEP			'\\'

	#define strupr				_strupr
	#define strlwr				_strlwr

	struct timezone
	{
		int  tz_minuteswest; /* minutes W of Greenwich */
		int  tz_dsttime;     /* type of dst correction */
	};

#else
	#define PROT_RDWR			(PROT_READ | PROT_WRITE)

	#define socketerrno			errno
	#define SOCKETEWOULDBLOCK		EWOULDBLOCK
	#define SOCKETEAGAIN			EAGAIN

	#define stricmp				strcasecmp
	#define strnicmp			strncasecmp

	#define	longlong			long long
	#define ulonglong			unsigned long long

	#define	OS_DIRSEP			'/'

	#define strupr(p)			{			      \
			int i=0;					      \
			while(p[i])					      \
			{						      \
				p[i]	=	(char)toupper(p[i]);	      \
				i++;					      \
			}						      \
		}

	#define strlwr(p)			{			      \
			int i=0;					      \
			while(p[i])					      \
			{						      \
				p[i]	=	(char)tolower(p[i]);	      \
				i++;					      \
			}						      \
		}

	#if defined(__GNU_LIBRARY__) && !defined(_SEM_SEMUN_UNDEFINED)
	/* union semun is defined by including <sys/sem.h> */
	#else
	/* according to X/OPEN we have to define it ourselves */
	union semun 
	{
		int val;                  /* value for SETVAL */
		struct semid_ds *buf;     /* buffer for IPC_STAT, IPC_SET */
		unsigned short *array;    /* array for GETALL, SETALL */
					  /* Linux specific part: */
		struct seminfo *__buf;    /* buffer for IPC_INFO */
	};
	#endif

#endif


#define ov_swap64(x) \
     ((((x) & 0xff00000000000000ull) >> 56)                                   \
      | (((x) & 0x00ff000000000000ull) >> 40)                                 \
      | (((x) & 0x0000ff0000000000ull) >> 24)                                 \
      | (((x) & 0x000000ff00000000ull) >> 8)                                  \
      | (((x) & 0x00000000ff000000ull) << 8)                                  \
      | (((x) & 0x0000000000ff0000ull) << 24)                                 \
      | (((x) & 0x000000000000ff00ull) << 40)                                 \
      | (((x) & 0x00000000000000ffull) << 56))

#ifdef OV_LITTLEENDIAN
#define ntohq(x)                                ov_swap64(x)
#define htonq(x)                                ov_swap64(x)
#else
#define ntohq(x)                                (x)
#define htonq(x)                                (x)
#endif

#define STRNCPY(pszDst, pszSrc, iLen)					      \
	do								      \
	{								      \
		strncpy(pszDst, pszSrc, (iLen)-1);			      \
		pszDst[(iLen)-1] = 0;					      \
	}								      \
	while(0)

#define MEMCPY(d, s, size, min)			{			      \
	int i;								      \
	if( size<=min )							      \
	{								      \
		for(i=0; i<size; i++)					      \
		{							      \
			d[0]	=	s[0];				      \
			d++; s++;					      \
		}							      \
	}								      \
	else								      \
	{								      \
		memcpy(d, s, size);					      \
		s +=	size;						      \
		d +=	size;						      \
	}								      \
}

#define NTOHNS(d, s, size)			{			      \
	int i;								      \
	for(i=0; i<size; i++)						      \
	{								      \
		*(unsigned short*)d	=	ntohs(*(unsigned short*)s);   \
		d	+=	sizeof(unsigned short);			      \
		s	+=	sizeof(unsigned short);			      \
	}								      \
}

#define HTONNS(d, s, size)			{			      \
	int i;								      \
	for(i=0; i<size; i++)						      \
	{								      \
		*(unsigned short*)d	=	htons(*(unsigned short*)s);   \
		d	+=	sizeof(unsigned short);			      \
		s	+=	sizeof(unsigned short);		 	      \
	}								      \
}

#define NTOHNL(d, s, size)			{			      \
	int i;								      \
	for(i=0; i<size; i++)						      \
	{								      \
		*(unsigned long*)d	=	ntohl(*(unsigned long*)s);    \
		d	+=	sizeof(unsigned long);			      \
		s	+=	sizeof(unsigned long);			      \
	}								      \
}

#define HTONNL(d, s, size)			{			      \
	int i;								      \
	for(i=0; i<size; i++)						      \
	{								      \
		*(unsigned long*)d	=	htonl(*(unsigned long*)s);    \
		d	+=	sizeof(unsigned long);			      \
		s	+=	sizeof(unsigned long);			      \
	}								      \
}

#define NTOHNQ(d, s, size)			{			      \
	int i;								      \
	for(i=0; i<size; i++)						      \
	{								      \
		*(ulonglong*)d	=	ntohq(*(ulonglong*)s);		      \
		d	+=	sizeof(ulonglong);			      \
		s	+=	sizeof(ulonglong);			      \
	}								      \
}

#define HTONNQ(d, s, size)			{			      \
	int i;								      \
	for(i=0; i<size; i++)						      \
	{								      \
		*(ulonglong*)d	=	htonq(*(ulonglong*)s);		      \
		d	+=	sizeof(ulonglong);			      \
		s	+=	sizeof(ulonglong);			      \
	}								      \
}

int os_usleep(int us);

void os_make_daemon(const char* pszRoot);
void os_ignore_pipe(void);

void os_file_to_macro(char* pszMacro, int iMacro, const char* pszFile);

int os_get_pagesize(void);

int os_protect(void* pvBase, size_t dwSize, int iProt);

int os_time(char* pszBuff, int iBuff);

#endif /* OV_OS_H */
