////////////////////////////////////////////////
//     FileName:    timer.h
//     Author:      Fish
//     Date:        2007-1-23 12:04:28
//     Description: []
////////////////////////////////////////////////
#ifndef _ADVTIME_H_
#define _ADVTIME_H_

inline DWORD GetHighResolutionTime()
{
	static LONGLONG s_nFreq = 0;
	static BOOL s_bRet = QueryPerformanceFrequency((LARGE_INTEGER*)&s_nFreq);
	if ( s_bRet )
	{
		LONGLONG nFreqCount = 0;
		BOOL bRet = QueryPerformanceCounter((LARGE_INTEGER*)&nFreqCount);
		if ( bRet )
		{
			return (DWORD)(nFreqCount / ( s_nFreq * 0.001) );
		}
	}
	return GetTickCount();
}

#endif
