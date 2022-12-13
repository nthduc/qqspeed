#ifndef __COMPARE_T_H__
#define __COMPARE_T_H__

#include <memory.h>
#include <string.h>

template<typename TType>
class TCompare
{
	public:
		BOOL ResultCompare(TType* pFactResult,TType* pExepResult)
		{
			int nResult;
			BOOL bResult = TRUE;

			nResult = memcmp(pFactResult,pExepResult,sizeof(TType));

			if(nResult == 0)
			{
				bResult = TRUE;
			}
			else
			{
				bResult = FALSE;
			}
			return bResult;

		};

};

#endif