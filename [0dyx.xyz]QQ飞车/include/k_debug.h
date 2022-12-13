
#ifndef __K_DEBUG_H__
#define __K_DEBUG_H__

#include "assert.h"
#include "crtdbg.h"

#if defined(DEBUG) || defined(_DEBUG)

#ifndef ASSERT
#define ASSERT(hr)			_ASSERTE(hr)
#endif

#ifdef assert
#undef assert
#endif
#define assert(hr)	_ASSERTE(hr)

#else

#define ASSERT(x)
#define assert(x)

#endif //defined(DEBUG) || defined(_DEBUG)

extern void ReportError( const char *fmt, ... );
extern void PopErrorBox( const char *fmt, ... );
extern void LogInfo( const char *fmt, ... );
extern void QuitForError( const char *fmt, ... );
extern void SetErrorReporter( const char *pname );

#pragma comment( lib, "kernel.lib" )	

//#ifndef ENABLE_DEBUG_OUTPUT
//#define OutputDebugString	/##/
//#define TRACE	/##/
//#endif
  

#define MemGuard_Size	100
#define MemGuard_Value	99999999

class _XMemGuard
{
private:
	int m_Data[MemGuard_Size];

public:
	_XMemGuard()
	{
		for ( int i = 0; i < MemGuard_Size; i++ )
		{
			m_Data[i] = MemGuard_Value;
		}
	}

	~_XMemGuard()
	{
		Check();
	}

	void Check()
	{
		for ( int i = 0; i < MemGuard_Size; i++ )
		{
			if ( m_Data[i] != MemGuard_Value)
			{
				_asm
				{
					int 3;	//ÖÂÃü´íÎó£¡º¯ÊýÓöµ½Ð´Ô½½ç¡£
				}
				break;
			}
		}
	}
};

#define MemGuard_MonitorPositive(x)	_XMemGuard __xxxMemGuardPositive##x;
#define MemGuard_MonitorNegative(x)	_XMemGuard __xxxMemGuardNegative##x;
#define MemGuard_MonitorPositive_Check(x)	(__xxxMemGuardPositive##x).Check();
#define MemGuard_MonitorNegative_Check(x)	(__xxxMemGuardNegative##x).Check();

#endif //__K_DEBUG_H__