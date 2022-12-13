#ifndef __STRINGMANIPULATE_T_H__
#define __STRINGMANIPULATE_T_H__

//-----------------------------------------------------------------------------
//�����ܺ����б�
//
// 1 StringTrim		ȥ��ָ���ַ����������ߵĿո�
//
// 2 String2Int		���ַ�����ʾ������ת��ΪInt����
//
// 3 IsTextEqual	�����Ƿ����, ���Դ�Сд�Ϳո�
//
// 4 Hex2Dword		��16�����ַ���ת��Ϊ����, �� "0x12Fcd" ���� "12F"
//-----------------------------------------------------------------------------

#define SMTIMESTRING_LEN	64
#define STRINGMANIPULATE_NUMBERLENGTH	128
#define SMLOCAL_STRING_LENGTH	1024

template<class ConcreateClass>
class StringManipulate
{
public:
//-----------------------------------------------------------------------------
//������ӿڡ������Ƿ����, ���Դ�Сд�Ϳո�
//-----------------------------------------------------------------------------

	BOOL IsTextEqual(LPCTSTR lpszLeft, LPCTSTR lpszRight)
	{
		TCHAR szString1[SMLOCAL_STRING_LENGTH] = {0};
		TCHAR szString2[SMLOCAL_STRING_LENGTH] = {0};

		StringTrim(lpszLeft,  szString1, CharCount(szString1));
		StringTrim(lpszRight, szString2, CharCount(szString2));

		int nLen1 = _tcslen(szString1);
		int nLen2 = _tcslen(szString2);


		int nCmp = 1;
		
		if(nLen1 == nLen2)
		{
			nCmp = _tcsnicmp(szString1, szString2, nLen1);
		}

		return (nCmp == 0);
	}

//-----------------------------------------------------------------------------
//������ӿڡ������ַ������� Char ����
//-----------------------------------------------------------------------------

	int StringLength(LPCTSTR lpszString)
	{
		return _tcslen(lpszString);
	}

//-----------------------------------------------------------------------------
//������ӿڡ����ַ���lpszToTrime���ҵĿո�ȥ��, ��ȥ���ո��Ľ�����뵽lpszResult
//            �С�lpszResult �� lpszToTrim ������ȡ���:StringTrim(szBuf, szBuf, n)
//            ����ֵΪ���Ƶ� lpszResult �ڵ��ַ���������
//-----------------------------------------------------------------------------
	
	int StringTrim(LPCTSTR lpszToTrim, LPTSTR lpszResult, int nCharCount)
	{
		BOOL bSuccess = FALSE;

		int nCharCopiedCount = 0;

		bSuccess = (
			lpszToTrim != NULL &&
			lpszResult != NULL &&
			nCharCount > 0
			);

		if(bSuccess)
		{
			bSuccess = FALSE;
			
			int nStringLength = _tcslen(lpszToTrim);

			LPCTSTR lpszHead = NULL;

			for(int i=0; i<nStringLength; i++)
			{
				if(lpszToTrim[i] != TEXT(' '))
				{
					lpszHead = lpszToTrim + i;

					break;
				}
			}

			LPCTSTR lpszEnd = NULL;

			for(int k = nStringLength - 1; k>=0; k--)
			{
				if(lpszToTrim[k] != TEXT(' '))
				{
					lpszEnd = lpszToTrim + k;

					break;
				}
			}

			if(lpszEnd != 0 && lpszHead != 0 && lpszEnd >= lpszHead)
			{
				int nTrimedLength = lpszEnd - lpszHead + 1;

				if(nTrimedLength < nCharCount)
				{
					if(lpszResult != lpszHead)
					{
						memmove(lpszResult, lpszHead, sizeof(TCHAR) * nTrimedLength);
					}

					lpszResult[nTrimedLength] = TEXT('\0');

					nCharCopiedCount = nTrimedLength;

					bSuccess = TRUE;
				}
			}

			if(!bSuccess)
			{
				lpszResult[0] = TEXT('\0');
			}
		}

		return nCharCopiedCount;
	}

//-----------------------------------------------------------------------------
//������ӿڡ�������תΪ�ַ���
//-----------------------------------------------------------------------------

	BOOL Int2String(int nValue, LPTSTR lpszValue, int nCharCount)
	{
		BOOL bSuccess = FALSE;	
	
		if(lpszValue != NULL && nCharCount > 0)
		{
			TCHAR szBuffer[MAX_PATH] = {0};

			_stprintf(szBuffer, TEXT("%d"), nValue);

			if(nCharCount > _tcslen(szBuffer))
			{
				_tcscpy(lpszValue, szBuffer);

				bSuccess = TRUE;
			}
		}

		return bSuccess;
	}

//-----------------------------------------------------------------------------
//������ӿڡ�������תΪ�ַ���
//-----------------------------------------------------------------------------

	BOOL DWORD2String(DWORD dwValue, LPTSTR lpszValue, int nCharCount)
	{
		BOOL bSuccess = FALSE;	
	
		if(lpszValue != NULL && nCharCount > 0)
		{
			TCHAR szBuffer[MAX_PATH] = {0};

			_stprintf(szBuffer, TEXT("%u"), dwValue);

			if(nCharCount > _tcslen(szBuffer))
			{
				_tcscpy(lpszValue, szBuffer);

				bSuccess = TRUE;
			}
		}

		return bSuccess;
	}

//-----------------------------------------------------------------------------
//������ӿڡ����ַ���ת��Ϊ����
//-----------------------------------------------------------------------------

	int String2Int(LPCTSTR lpszNumberString)
	{
		TCHAR szNumber[STRINGMANIPULATE_NUMBERLENGTH] = {0};

		int nValue = 0;

		if(StringTrim(lpszNumberString, szNumber, STRINGMANIPULATE_NUMBERLENGTH))
		{
			nValue = _ttoi(szNumber);
		}

		return nValue;
	}
	
	DWORD String2DWORD(LPCTSTR lpszNumberString)
	{
		TCHAR szNumber[STRINGMANIPULATE_NUMBERLENGTH] = {0};

		DWORD dwValue = 0;

		if(StringTrim(lpszNumberString, szNumber, STRINGMANIPULATE_NUMBERLENGTH))
		{
			dwValue = (DWORD)_ttol(szNumber);
		}

		return dwValue;
	}

//-----------------------------------------------------------------------------
//  ����ַ����Ƿ񳬳�����, lpszString ���ȱ� 
//	nMaxCharCount �� 1 ���ַ�����ʱ���� True
//-----------------------------------------------------------------------------

	BOOL StringCheck(LPCTSTR lpszString, size_t nMaxCharCount)
	{
		BOOL bValid = FALSE;
		
		if(lpszString != NULL && (size_t)_tcslen(lpszString) < nMaxCharCount)
		{
			bValid = TRUE;
		}

		return bValid;
	}

//-----------------------------------------------------------------------------
//  �ַ�������, ��Ŀ�Ļ��������㹻���ȷ���Դ�ַ����� '\0' ��βʱ�Ž��и���
//-----------------------------------------------------------------------------
	
	BOOL StringCopy(LPTSTR lpszDest, int nMaxCharCount, LPCTSTR lpszSrc)
	{
		BOOL bSuccess = FALSE;

		if(lpszDest != NULL && nMaxCharCount > 0 && lpszSrc != NULL)
		{
			if(StringCheck(lpszSrc, nMaxCharCount))
			{
				_tcscpy(lpszDest, lpszSrc);
				
				bSuccess = TRUE;
			}
		}

		return bSuccess;
	}

//-----------------------------------------------------------------------------
//  ����ָ�����ȵ��ַ���, ���ڸ�����ɺ���� '\0'
//-----------------------------------------------------------------------------
	
	BOOL StringCopyFix(
		LPTSTR	lpszDest, 
		int		nMaxCharCount, 
		LPCTSTR lpszSrc,
		int		nToCopyCharCount)
	{
		BOOL bSuccess = FALSE;
		
		if(lpszDest != NULL && nToCopyCharCount > 0 && 
		   lpszSrc  != NULL && nMaxCharCount > nToCopyCharCount)
		{
			_tcsncpy(lpszDest, lpszSrc, nToCopyCharCount);

			lpszDest[nToCopyCharCount] = TEXT('\0');
			
			bSuccess = TRUE;
		}
		
		return bSuccess;
	}
	
//-----------------------------------------------------------------------------
//  �����ַ�����ĳ���, ���� TCHAR szBuf[12]; CharCount(szBuf) ���� 12 
//-----------------------------------------------------------------------------
	
	template <class StringArrayT>
	inline int CharCount(StringArrayT& /*T*/)
	{
		return sizeof(StringArrayT) / sizeof(TCHAR);
	}

//-----------------------------------------------------------------------------
//  �����ַ�����ĳ���, ƫ�ػ��汾
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//  �����ַ���
//-----------------------------------------------------------------------------
	
	template <class StringArrayT>
	BOOL StringCopyToArray(StringArrayT& T, LPCTSTR lpszSrc)
	{
		return StringCopy(T, CharCount(T), lpszSrc);
	}

//-----------------------------------------------------------------------------
//  ƴ���ַ���
//-----------------------------------------------------------------------------
	
	template <class DestT, class SrcT>
	BOOL StringArrayCat(DestT& szDest, const SrcT& szSrc)
	{
		int nCapability = CharCount(szDest);

		return StringCat(szDest, nCapability, szSrc);
	}

//-----------------------------------------------------------------------------
//  ƴ���ַ���
//-----------------------------------------------------------------------------
	
	BOOL StringCat(LPTSTR lpszDest, int nCharCount, LPCTSTR lpszSource)
	{
		BOOL bSuccess = FALSE;
		
		if(lpszDest != NULL && nCharCount > 0 && lpszSource != NULL)
		{
			int nCapability = nCharCount;
			
			int nDestLen   = _tcslen(lpszDest);
			int nSourceLen = _tcslen(lpszSource);
			
			if(nSourceLen + nDestLen < nCapability)
			{
				_tcscat(lpszDest, lpszSource);

				bSuccess = TRUE;
			}
		}
		
		return bSuccess;
	}

//-----------------------------------------------------------------------------
//  ���ַ���ǰ��ƴ���ַ���
//-----------------------------------------------------------------------------
	
	BOOL StringCatHead(LPTSTR lpszDest, int nCharCount, LPCTSTR lpszSource)
	{
		BOOL bSuccess = FALSE;

		LPTSTR lpszTemp = NULL;

		int nAllocSize = nCharCount * sizeof(TCHAR);

		if(Me()->AllocMemory(&lpszTemp, nAllocSize, TRUE))
		{
			lpszTemp[0] = TEXT('\0');

			StringCopy(lpszTemp, nAllocSize, lpszDest);

			lpszDest[0] = TEXT('\0');
			
			StringCat(lpszDest, nCharCount, lpszSource);

			StringCat(lpszDest, nCharCount, lpszTemp);

			bSuccess = TRUE;

			Me()->ReleaseMemory(lpszTemp);
		}

		return bSuccess;
	}

//-----------------------------------------------------------------------------
//  �滻�ַ��������йؼ����ַ��������滻�ؼ��ַ�����
//-----------------------------------------------------------------------------
	int StringAllReplace(
		LPCTSTR lpszSource,
		LPCTSTR lpszToken,
		LPCTSTR lpszReplace,
		LPTSTR lpszOutBuffer,
		int nCharCount)
	{
		int nReplaceCount = 0;
		int nStringLength = 0;
		
		LPTSTR lpszTemp = NULL;
		int nAllocSize = nCharCount * sizeof(TCHAR);
		
		if(Me()->AllocMemory(&lpszTemp, nAllocSize, TRUE))
		{	
			StringCopy(lpszTemp, nCharCount, lpszSource);
			
			do {
				nStringLength = StringSingleReplace(
					lpszTemp,
					lpszToken,
					lpszReplace,
					lpszOutBuffer,
					nCharCount);
				
				if( nStringLength > 0 )
				{
					StringCopy(lpszTemp, nCharCount, lpszOutBuffer);
					nReplaceCount++;
				}
				
			} while(nStringLength > 0);

			Me()->ReleaseMemory(lpszTemp);
		}
		
		return nReplaceCount;
	}

//-----------------------------------------------------------------------------
//  �ַ����滻�������滻���ַ������ȣ�0: ��ʾû���滻�� -1����ʾ�滻ʧ��
//-----------------------------------------------------------------------------
	int StringSingleReplace(
		LPCTSTR lpszSource,
		LPCTSTR lpszToken,
		LPCTSTR lpszReplace,
		LPTSTR lpszOutBuffer,
		int nCharCount)
	{
		int nOutStringLength = -1;
		
		BOOL bSuccess  = (lpszSource != NULL &&
			lpszToken != NULL &&
			lpszReplace != NULL &&
			lpszOutBuffer != NULL);

		if( bSuccess )
		{
			bSuccess = ( _tcslen(lpszToken) != 0 );
		}
		
		int nLength = 0;
		if(bSuccess)
		{
			nLength = _tcslen(lpszSource) - _tcslen(lpszToken) + _tcslen(lpszReplace);
			bSuccess = ( nCharCount > nLength );
		}
		
		if( bSuccess )
		{
			LPTSTR lpszFind = _tcsstr(lpszSource, lpszToken);

			if( NULL != lpszFind )
			{
				int nFindPos = _tcslen(lpszFind);
				int nTokenLength = _tcslen(lpszToken);
				
				StringCopyFix(lpszOutBuffer, nCharCount, lpszSource, _tcslen(lpszSource) - nFindPos);
				StringCat(lpszOutBuffer, nCharCount, lpszReplace);
				StringCat(lpszOutBuffer, nCharCount, (lpszFind + nTokenLength));

				nOutStringLength = nLength;
			}
			else
			{
				nOutStringLength = 0;
				StringCopy(lpszOutBuffer, nCharCount, lpszSource);
			}
		}
		
		return nOutStringLength;
	}

//-----------------------------------------------------------------------------
//  �Ѹ�ʽ���ַ������ָ�����ȵ��ڴ���
//
//  ������Ľ�����ȳ��� nCharCount - 1, �������������
//
//  1 bAutoCut = True ���Զ���ȡǰ�� nCharCount - 1 ���ַ���Ϊ���, ���� True
//  2 bAutoCut = Fals ���� False ��ʾʧ��
//
//  Living 2005-11-05 Tested
//-----------------------------------------------------------------------------

	BOOL SmartStringFormat(
		LPTSTR lpszOutputBuffer,
		int nCharCount,
		BOOL bAutoCut,
		LPCTSTR lpszFormat,
		...)
	{
		BOOL bSuccess = TRUE;

		bSuccess = (
			lpszOutputBuffer != NULL &&
			nCharCount > 0 &&
			lpszFormat != NULL);

		if(bSuccess)
		{
			lpszOutputBuffer[nCharCount - 1] = TEXT('\0');

			va_list pArg;

			va_start(pArg, lpszFormat);

			_vsntprintf(
				lpszOutputBuffer,
				nCharCount,
				lpszFormat,
				pArg);

			va_end(pArg);
		}

		if(bSuccess)
		{
			if(lpszOutputBuffer[nCharCount - 1] != TEXT('\0'))
			{
				// ���� \0 ���ص�, ��������

				lpszOutputBuffer[nCharCount - 1] = TEXT('\0');

				if(!bAutoCut)
				{
					bSuccess = FALSE;
				}
			}
		}

		return bSuccess;
	}

//-----------------------------------------------------------------------------
//  �ж��ַ����Ƿ�Ϊ��(ֻ�пո�ҲΪ��)
//-----------------------------------------------------------------------------

	BOOL IsStringEmpty(LPCTSTR lpszString)
	{
		BOOL bResult = TRUE;

		TCHAR szString[SMLOCAL_STRING_LENGTH] = {0};
		StringTrim(lpszString, szString, CharCount(szString));
		
		bResult = (_tcslen(szString) == 0);

		return bResult;
	}

//-----------------------------------------------------------------------------
//  ��ĸ���н����ָ���ķָ����ֵõ��Ӵ��еĵ�nTokenIndex��(0-based)
//    bCompatibleMode�������Լ����ϴ�����ʽ(�������ָ������������Ӵ�)
//    �����Ա�׼��ʽ(���������ָ���)�������з�
//
//-----------------------------------------------------------------------------

	BOOL GetToken(TCHAR szSource[], const TCHAR szDelimiters[], int nTokenIndex,
		TCHAR szResultToken[], int nMaxTokenLen, BOOL bCompatibleMode = TRUE)
	{
		int nSourceLen = _tcslen(szSource);
		TCHAR* pszTemp = NULL;
		TCHAR** pszarrTokens = NULL;
		BOOL bSucc = Me()->AllocMemory(&pszTemp, nSourceLen + 1, TRUE);
		if(bSucc)
		{
			bSucc = Me()->AllocMemory(&pszarrTokens, sizeof(TCHAR*) * (nTokenIndex + 1), TRUE);

			if(bSucc)
			{
				memset(pszarrTokens, 0, sizeof(TCHAR*) * (nTokenIndex + 1));
				_tcscpy(pszTemp, szSource);
				int nExtracted = Split(pszTemp, szDelimiters,
					pszarrTokens, nTokenIndex + 1, bCompatibleMode);

				bSucc = nExtracted == (nTokenIndex + 1) &&
					_tcslen(pszarrTokens[nTokenIndex]) < nMaxTokenLen;
				if(bSucc)
				{
					_tcscpy(szResultToken, pszarrTokens[nTokenIndex]);
				}

				Me()->ReleaseMemory(pszarrTokens);
			}

			Me()->ReleaseMemory(pszTemp);
		}

		return bSucc;
	}

	BOOL GetFirstToken(TCHAR szSource[], const TCHAR cDelimiter,
		TCHAR szResultToken[], int nMaxTokenLen)
	{
		TCHAR* pTokenEnd = _tcschr(szSource, cDelimiter);
		BOOL bSucc = pTokenEnd != NULL && pTokenEnd > szSource &&
			(pTokenEnd - szSource) < nMaxTokenLen;
		
		if(bSucc)
		{
			*pTokenEnd = _T('\0');
			_tcscpy(szResultToken, szSource);
			*pTokenEnd = cDelimiter;
		}

		return bSucc;
	}

//-----------------------------------------------------------------------------
//  ��ĸ����ָ���ķָ�����Ϊ�����Ӵ�
//    bCompatibleMode�������Լ����ϴ�����ʽ(�������ָ������������Ӵ�)
//    �����Ա�׼��ʽ(���������ָ���)�������з�
//-----------------------------------------------------------------------------

	int Split(TCHAR szSource[], const TCHAR szDelimiters[],
		TCHAR* szarrTokens[], int nMaxTokens, BOOL bCompatibleMode = TRUE)
	{
		if(bCompatibleMode)
		{
			return VerboseSplit(szSource, szDelimiters, szarrTokens, nMaxTokens);
		}
		else
		{
			return FastSplit(szSource, szDelimiters, szarrTokens, nMaxTokens);
		}
	}

//-----------------------------------------------------------------------------
//  ��ĸ����ָ���ķָ�����Ϊ�����Ӵ�
//  ���յõ��Ӵ�����Ϊ(nMaxTokens)����(�Ӵ��ܸ���)�н�С��
//
//  ÿ����һ���ָ������ѷָ���ǰֱ��ǰһ���ָ�����������Ϊ�µ��Ӵ�
//    �Էָ�����ͷ�����һ���Ӵ�Ϊ�մ�
//    ��������������ķָ����������һ���¿��Ӵ�
//    �Էָ�����β����������һ�����Ӵ�
//
//  �Ӵ��ܸ���Ϊĸ���зָ����ĸ�����ʵ�ʽ��������Ӵ�����ȡ����nMaxTokens
//
//  �������µĿռ䣬�Ӵ�ʹ��ĸ��������Ϊ�洢
//  ����������޸�ĸ�������֮��ĸ�������ô�������ʹ�ñ�����ǰ���ñ���
//  
//  szDelimiters�Ƿָ����������е�ÿһ���ַ�������һ�������ķָ�������
//
//  ����ֵ��ʵ�ʽ��������Ӵ�����
//
//  balonfan 2005-12-03 Tested
//-----------------------------------------------------------------------------

	int VerboseSplit(TCHAR szSource[], const TCHAR szDelimiters[],
		TCHAR* szarrTokens[], int nMaxTokens)
	{
		int nExtracted = 0;
		TCHAR* szPrev  = szSource;
		TCHAR* szEnd   = szSource + _tcslen(szSource);
		TCHAR* szToken = _tcstok(szSource, szDelimiters);
		while(szToken != NULL && nExtracted < nMaxTokens)
		{
			while(szPrev != szToken && nExtracted < nMaxTokens)
			{
				szarrTokens[nExtracted++] = szPrev;
				*(szPrev++) = _T('\0');
			}
			
			szPrev = szToken + _tcslen(szToken);
			if(szPrev != szEnd && szPrev + 1 != szEnd)
			{
				++szPrev;
			}
			
			if(nExtracted < nMaxTokens)
			{
				szarrTokens[nExtracted++] = szToken;
			}
			
			szToken = _tcstok(NULL, szDelimiters);
		}
		
		if(szPrev != szEnd && nExtracted < nMaxTokens)
		{
			while(*szPrev != _T('\0') && nExtracted < nMaxTokens)
			{
				szarrTokens[nExtracted++] = szPrev;
				*(szPrev++) = _T('\0');
			}
			
			szarrTokens[nExtracted++] = szPrev;
		}
		
		return nExtracted;
	}

//-----------------------------------------------------------------------------
//  ��ĸ����ָ���ķָ�����Ϊ�����Ӵ�
//  ���յõ��Ӵ�����Ϊ(nMaxTokens)����(�Ӵ��ܸ���)�н�С��
//
//  ���������ķָ������������ġ������ָ����Ĳ�����Ϊһ���Ӵ�
//    �Էָ�����β�����治����Ͽ��Ӵ�
//
//  �������µĿռ䣬�Ӵ�ʹ��ĸ��������Ϊ�洢
//  ����������޸�ĸ�������֮��ĸ�������ô�������ʹ�ñ�����ǰ���ñ���
//  
//  szDelimiters�Ƿָ����������е�ÿһ���ַ�������һ�������ķָ�������
//
//  ����ֵ��ʵ�ʽ��������Ӵ�����
//
//  balonfan 2005-12-03 Tested
//-----------------------------------------------------------------------------

	int FastSplit(TCHAR szSource[], const TCHAR szDelimiters[],
		TCHAR* szarrTokens[], int nMaxTokens)
	{
		int nExtracted = 0;
		TCHAR* szToken = _tcstok(szSource, szDelimiters);
		while(szToken != NULL && nExtracted < nMaxTokens)
		{
			szarrTokens[nExtracted++] = szToken;
			szToken = _tcstok(NULL, szDelimiters);
		}
		return nExtracted;
	}

	// lpszLeft��lpszRight Ϊ NULL ʱ��Ϊ���ַ�������
	BOOL IsStringSame(LPCTSTR lpszLeft, LPCTSTR lpszRight)
	{
		lpszLeft = (lpszLeft == NULL) ? TEXT("") : lpszLeft;
		lpszRight = (lpszRight == NULL) ? TEXT("") : lpszRight;

		BOOL bSame = FALSE;

		int nLeftCharCount  = _tcslen(lpszLeft) + 1;
		int nRightCharCount = _tcslen(lpszRight) + 1;
		int nLeftLength  =  nLeftCharCount  * sizeof(TCHAR);
		int nRightLength =  nRightCharCount * sizeof(TCHAR);

		LPTSTR lpszTrimedLeft  = NULL;
		LPTSTR lpszTrimedRight = NULL;

		Me()->AllocMemory(&lpszTrimedLeft, nLeftLength, TRUE);
		Me()->AllocMemory(&lpszTrimedRight, nRightLength, TRUE);

		if(lpszTrimedLeft != NULL)
		{
			StringTrim(lpszLeft, lpszTrimedLeft, nLeftCharCount);
		}

		if(lpszTrimedRight != NULL)
		{
			StringTrim(lpszRight, lpszTrimedRight, nRightCharCount);
		}

		if(lpszTrimedLeft != NULL && lpszTrimedRight != NULL)
		{
			bSame = (_tcsicmp(lpszTrimedLeft, lpszTrimedRight) == 0);
		}

		if(lpszTrimedLeft != NULL)
		{
			Me()->ReleaseMemory(lpszTrimedLeft);

			lpszTrimedLeft = NULL;
		}

		if(lpszTrimedRight != NULL)
		{
			Me()->ReleaseMemory(lpszTrimedRight);

			lpszTrimedRight = NULL;
		}

		return bSame;
	}

	// �� lpszString �в��� lpszCharSet �����ִ�Сд
	LPCTSTR StringFind(LPCTSTR lpszString, LPCTSTR lpszCharSet)
	{
		LPCTSTR lpszResult = NULL;
		
		if(lpszString != NULL && lpszCharSet != NULL)
		{
			TCHAR szLocalString[SMLOCAL_STRING_LENGTH] = {0};
			TCHAR szToFindString[SMLOCAL_STRING_LENGTH] = {0};
			
			_tcsncpy(szLocalString,  lpszString,  SMLOCAL_STRING_LENGTH - 1);
			_tcsncpy(szToFindString, lpszCharSet, SMLOCAL_STRING_LENGTH - 1);
			
			_tcslwr(szLocalString);
			_tcslwr(szToFindString);
			
			LPCTSTR lpszResultInLocal = _tcsstr(szLocalString, szToFindString);
			
			if(lpszResultInLocal != NULL)
			{
				lpszResult = lpszString + (lpszResultInLocal - szLocalString);
			}
		}
		
		return lpszResult;
	}

//=============================================================================
//  �� 2005-12-20 15:10:02 �����ַ���ת��Ϊ SYSTEMTIME ��ʽ 
//=============================================================================

	BOOL String2SystemTime(LPCTSTR lpszTimeString, SYSTEMTIME* pSystemTime)
	{
		BOOL bSuccess = FALSE;

		if(lpszTimeString != NULL && pSystemTime != NULL)
		{
			TCHAR szTimeString[SMTIMESTRING_LEN] = {0};
			StringCopy(szTimeString, CharCount(szTimeString), lpszTimeString);

			LPTSTR arlpszDateAndTime[2] = {0};
			int nSplit1 = Split(szTimeString, TEXT(" "), arlpszDateAndTime, 2);

			if(nSplit1 == 2)
			{
				LPTSTR arlpszDates[3] = {0};
				LPTSTR arlpszTimes[3] = {0};

				int nSplitDate = Split(arlpszDateAndTime[0], TEXT("-"), 
					arlpszDates, 3);

				int nSplitTime = Split(arlpszDateAndTime[1], TEXT(":"),
					arlpszTimes, 3);

				if(nSplitDate == 3 && nSplitTime == 3)
				{
					memset(pSystemTime, 0, sizeof(SYSTEMTIME));

					pSystemTime->wYear  = String2Int(arlpszDates[0]);
					pSystemTime->wMonth = String2Int(arlpszDates[1]);
					pSystemTime->wDay   = String2Int(arlpszDates[2]);

					pSystemTime->wHour    = String2Int(arlpszTimes[0]);
					pSystemTime->wMinute  = String2Int(arlpszTimes[1]);
					pSystemTime->wSecond  = String2Int(arlpszTimes[2]);

					if(pSystemTime->wYear > 0 && pSystemTime->wMonth > 0 &&
						pSystemTime->wDay > 0)
					{
						bSuccess = TRUE;
					}
				}
			}
		}

		return bSuccess;
	}

	DWORD Hex2Dword(LPCTSTR lpszHex)
	{
		DWORD dwResult = 0;
		
		LPCTSTR lpszNumbers1 = TEXT("0123456789ABCDEF");
		LPCTSTR lpszNumbers2 = TEXT("0123456789abcdef");
		
		if(lpszHex != NULL)
		{
			LPCTSTR lpsz0x = _tcsstr(lpszHex, TEXT("0x"));
			
			if(lpsz0x == NULL)
			{
				lpsz0x = _tcsstr(lpszHex, TEXT("0X"));
			}
			
			LPCTSTR lpszNumbers = lpszHex;
			
			if(lpsz0x != NULL)
			{
				lpszNumbers = lpsz0x + 2;
			}
			
			if(lpszNumbers != NULL)
			{
				int nIndex = 0;
				TCHAR chNumber = lpszNumbers[nIndex];
				
				while(chNumber != 0)
				{
					BOOL bInNumber1 = TRUE;
					LPCTSTR lpszPos = _tcschr(lpszNumbers1, chNumber);
					
					if(lpszPos == NULL)
					{
						bInNumber1 = FALSE;
						lpszPos = _tcschr(lpszNumbers2, chNumber);
					}
					
					if(lpszPos != NULL)
					{
						int nByteValue = bInNumber1 ? lpszPos - lpszNumbers1 : lpszPos - lpszNumbers2;
						
						dwResult = dwResult << 4;
						dwResult += nByteValue;
					}
					
					nIndex++;
					chNumber = lpszNumbers[nIndex];
				}
			}
		}
		
		return dwResult;
	}

	void RemoveInvisibleCharacter(LPTSTR lpszString)
	{
		if(lpszString != NULL)
		{
			TCHAR* pWritePointer = lpszString;
			TCHAR* pReadPointer  = lpszString;
			
			while(*pReadPointer != TEXT('\0'))
			{
				if(*pReadPointer >= 32)
				{
					if(pReadPointer != pWritePointer)
					{
						*pWritePointer = *pReadPointer;
					}
					
					pReadPointer++;
					pWritePointer++;
				}
				else
				{
					pReadPointer++;
				}
			}

			*pWritePointer = TEXT('\0');
		}
	}

   /**
    * @brief ���ַ���ת��ΪСд
	*/
	BOOL MakeLower(LPTSTR lpszString)
	{
		BOOL bSuccess = FALSE;

		if(lpszString != NULL)
		{
			TCHAR ch = *lpszString;

			while(ch != '\0')
			{
				char chDelta = 'A' - 'a';
				
				if(ch >= 'A' && ch <= 'Z')
				{
					*lpszString = ch - chDelta;
				}
				
				lpszString++;

				ch = *lpszString;
			}
			
			bSuccess = TRUE;
		}
		
		return bSuccess;
	}

private:

//-----------------------------------------------------------------------------
//  ����ʵ��������ָ��
//-----------------------------------------------------------------------------

	ConcreateClass* Me()
	{
		ConcreateClass* pConcreateClass = static_cast<ConcreateClass*>(this);

		return pConcreateClass;
	}
};

#endif