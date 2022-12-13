#ifndef __LOGHELPIMPL_H__
#define __LOGHELPIMPL_H__

#include <atlbase.h>

#define LOGHELPIMPL_DUMP_BYTESPERROW	16

#define LOG2FILE_MODE_OFF			0
#define LOG2FILE_MODE_INDIVIDUAL	1
#define LOG2FILE_MODE_SINGLE		2

template<
	class ConcreateClass
	>
class LogHelpImpl
{
public:
	LogHelpImpl()
	{
		m_bOutputTraceInfo    = 0;
		m_bOutputErrorInfo	  = 0;
		m_bOutputVerboseInfo = 0;
	}
	
	void SetLogFlags(BOOL bVerboseLogOpen, BOOL bTraceLogOpen, BOOL bErrorLogOpen)
	{
		m_bOutputTraceInfo   = bTraceLogOpen;
		m_bOutputErrorInfo	 = bErrorLogOpen;
		m_bOutputVerboseInfo = bVerboseLogOpen;
	}
		
	void LoadLogConfig(LPCTSTR lpszSectionName)
	{

//		m_bOutputVerboseInfo = GetPrivateProfileInt(
//			lpszSectionName,
//			TEXT("ShowVerboseInfo"),
//			0,
//			GetDebugINIFileName()) != 0;
//
//		m_bOutputTraceInfo = GetPrivateProfileInt(
//			lpszSectionName,
//			TEXT("ShowTraceInfo"),
//			0,
//			GetDebugINIFileName()) != 0;
//		
//		m_bOutputErrorInfo = GetPrivateProfileInt(
//			lpszSectionName,
//			TEXT("ShowErrorInfo"),
//			0,
//			GetDebugINIFileName()) != 0;

		
		if(m_strLogInfoPrefix.empty())
		{
			m_strLogInfoPrefix = lpszSectionName;
		}
	}
	
	void SetLogInfoPrefixString(LPCTSTR lpszPrefix)
	{
		m_strLogInfoPrefix = lpszPrefix;
	}

	void OutputVerboseInfo(LPCTSTR lpszFormat, ...)
	{
		if(m_bOutputVerboseInfo)
		{
			va_list pArg;
			va_start(pArg, lpszFormat);
			OutputLog(_T("VERBOSE"), lpszFormat, pArg);
			va_end(pArg);
		}
	}

	void OutputTraceInfo(LPCTSTR lpszFormat, ...)
	{
		if(m_bOutputTraceInfo)
		{
			va_list pArg;
			va_start(pArg, lpszFormat);
			OutputLog(_T("TRACE"), lpszFormat, pArg);
			va_end(pArg);
		}
	}
	
	void OutputErrorInfo(LPCTSTR lpszFormat, ...)
	{
		if(m_bOutputErrorInfo || m_bOutputTraceInfo || m_bOutputVerboseInfo)
		{
			va_list pArg;
			va_start(pArg, lpszFormat);
			OutputLog(_T("ERROR"), lpszFormat, pArg);
			va_end(pArg);
		}
	}

	void DumpForTraceInfo(PVOID pData, int nLen)
	{
		PBYTE lpByte = (PBYTE)pData;

		if(m_bOutputTraceInfo)
		{

			int nLoopCount = nLen / LOGHELPIMPL_DUMP_BYTESPERROW;
			int nLeftBytes = nLen % LOGHELPIMPL_DUMP_BYTESPERROW;

			LPBYTE lpBytes = (LPBYTE) pData;

			BOOL bReadException = FALSE;

			for(int nIndex = 0; nIndex < nLoopCount; nIndex++)
			{
				bReadException = DumpMemory(
					lpBytes + nIndex * LOGHELPIMPL_DUMP_BYTESPERROW, 
					LOGHELPIMPL_DUMP_BYTESPERROW);

				if(bReadException)
				{
					break;
				}
			}

			if(!bReadException && nLeftBytes > 0)
			{
				DumpMemory(lpBytes + nLoopCount * LOGHELPIMPL_DUMP_BYTESPERROW, nLeftBytes);
			}
		}
	}

private:
	void OutputLog(LPCTSTR lpszTitle, LPCTSTR lpszFormat, va_list pArgs)
	{
		TCHAR szFormat[1024]  = { 0 };
		TCHAR szMessage[1024] = { 0 };

		_sntprintf(szFormat, 1024 - 1, _T("%s:%s %s\n"),
			lpszTitle, m_strLogInfoPrefix.c_str(), lpszFormat);

		_vsntprintf(szMessage, 1024 - 1, szFormat, pArgs);

		OutputLog(szMessage);
	}

	BOOL DumpMemory(LPBYTE lpBytes, int nCount)
	{
		BOOL bReadException = IsBadReadPtr(lpBytes, nCount);
		TCHAR szBuffer[LOGHELPIMPL_DUMP_BYTESPERROW * 3 + 1] = {0};
		int nIndex = 0;

		if(!bReadException)
		{
			for(nIndex = 0; nIndex < nCount; nIndex++)
			{
				wsprintf(&szBuffer[nIndex * 3], _T("%02x "), lpBytes[nIndex]);
			}

			OutputDebugString(szBuffer);
			Log2File(szBuffer);
		}

		return bReadException;
	}

public:
	static void OutputLog(LPTSTR lpszLogInfo)
	{
		::OutputDebugString(lpszLogInfo);
		Log2File(lpszLogInfo);
	}

	static void OutputLog(LPCTSTR lpszLogInfo)
	{
		TCHAR szLogInfo[1024] = { 0 };
		_tcsncpy(szLogInfo, lpszLogInfo, 1024 - 1);
		OutputLog((LPTSTR)szLogInfo);
	}

	static LPCTSTR QQGameRootPath()
	{
		static TCHAR s_szPath[MAX_PATH] = {0};
		
		HMODULE hModule = ::GetModuleHandle(NULL);
		::GetModuleFileName(hModule, s_szPath, MAX_PATH);
		
		TCHAR* pTemp = _tcsrchr(s_szPath, TEXT('\\'));
		if(pTemp != NULL)
		{
			pTemp[0] = TEXT('\0');
		}
		
		return s_szPath;
	}

	static FILETIME GetProcessCreationTime()
	{
		FILETIME stCreationTime = { 0 };
		FILETIME stTemp         = { 0 };
		GetProcessTimes(::GetCurrentProcess(),
			&stCreationTime, &stTemp, &stTemp, &stTemp);

		FILETIME stLocalCreationTime = { 0 };
		FileTimeToLocalFileTime(&stCreationTime, &stLocalCreationTime);

		return stLocalCreationTime;
	}

	static LPCTSTR LogFilePath(UINT unLog2FileMode)
	{
		static TCHAR s_szLogFileName[MAX_PATH] = { 0 };

		if(s_szLogFileName[0] == _T('\0'))
		{
			if(unLog2FileMode == LOG2FILE_MODE_INDIVIDUAL)
			{
				FILETIME stCreationTime = GetProcessCreationTime();
				SYSTEMTIME stTime = { 0 };
				FileTimeToSystemTime(&stCreationTime, &stTime);

				_sntprintf(s_szLogFileName, MAX_PATH,
					_T("%s\\Config\\Log\\%04d%02d%02d_%02d%02d%02d.log"),
					QQGameRootPath(), stTime.wYear, stTime.wMonth, stTime.wDay,
					stTime.wHour, stTime.wMinute, stTime.wSecond);
			}
			else if(unLog2FileMode == LOG2FILE_MODE_SINGLE)
			{
				_sntprintf(s_szLogFileName, MAX_PATH,
					_T("%s\\Config\\Log\\QQGameLog.log"), QQGameRootPath());
			}
		}

		return s_szLogFileName;
	}

	static void PreCheckFile(HANDLE hFile, UINT unLog2FileMode)
	{
		if(hFile != INVALID_HANDLE_VALUE && unLog2FileMode == LOG2FILE_MODE_SINGLE)
		{
			FILETIME stCurrentCreationTime = GetProcessCreationTime();

			LONG lOffsetHigh = 0;
			::SetFilePointer(hFile, 0, &lOffsetHigh, FILE_BEGIN);

			BOOL    bNeedInit = TRUE;
			DWORD    dwRead   = 0;
			FILETIME stReadCreationTime    = { 0 };
			if(::ReadFile(hFile, &stReadCreationTime, sizeof(FILETIME), &dwRead, NULL))
			{
				// check if log is created in this run
				bNeedInit = memcmp(
					&stReadCreationTime, &stCurrentCreationTime, sizeof(FILETIME)) != 0;
			}

			if(bNeedInit)
			{
				// if log file is created in previous run
				// write current run time at the beginning and truncate it

				LONG lOffsetHigh = 0;
				::SetFilePointer(hFile, 0, &lOffsetHigh, FILE_BEGIN);

				DWORD dwWrite = 0;
				::WriteFile(hFile, (LPVOID)&stCurrentCreationTime,
					sizeof(FILETIME), &dwWrite, NULL);

				::SetEndOfFile(hFile);
			}
		}
	}
	
	static UINT Log2FileMode()
	{
		TCHAR szDebugIniFileName[MAX_PATH] = { 0 };
		_sntprintf(szDebugIniFileName, MAX_PATH,
			_T("%s\\Config\\Original\\Debug.ini"), QQGameRootPath());
		
		return GetPrivateProfileInt(_T("QQGame"), _T("Log2File"),
			LOG2FILE_MODE_OFF, szDebugIniFileName);
	}
	
	static void Log2File(LPTSTR lpszMessage)
	{
		USES_CONVERSION;
		static BOOL   s_bInitialized   = FALSE;
		static UINT   s_unLog2FileMode = FALSE;
		
		if(!s_bInitialized)
		{
			s_unLog2FileMode = Log2FileMode();
			s_bInitialized   = TRUE;
		}
		
		if(s_unLog2FileMode != LOG2FILE_MODE_OFF)
		{
			HANDLE hFile = ::CreateFile(LogFilePath(s_unLog2FileMode),
				GENERIC_READ | GENERIC_WRITE,
				FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_ARCHIVE, NULL);
			
			if(hFile != INVALID_HANDLE_VALUE)
			{
				PreCheckFile(hFile, s_unLog2FileMode);

				char* szMessage = T2A(lpszMessage);
				LONG lSize = 0;
				::SetFilePointer(hFile, 0, &lSize, FILE_END);

				DWORD dwSize = 0;
				::WriteFile(hFile, (LPCVOID)szMessage, strlen(szMessage), &dwSize, NULL);

				::CloseHandle(hFile);
			}
		}
	}

private:
	BOOL m_bOutputVerboseInfo;
	BOOL m_bOutputTraceInfo;
	BOOL m_bOutputErrorInfo;
	std::tstring m_strLogInfoPrefix;
};

#endif