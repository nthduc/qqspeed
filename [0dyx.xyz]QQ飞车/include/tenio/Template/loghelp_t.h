#ifndef __LOGHELP_H__
#define __LOGHELP_H__

#include <atlbase.h>
#include "../Include/ComponentFactory/teniocomponentfactory_i.h"

#define LOGHELPIMPL_DUMP_BYTESPERROW	16

#define LOG2FILE_MODE_OFF			0
#define LOG2FILE_MODE_INDIVIDUAL	1
#define LOG2FILE_MODE_SINGLE		2

#define TENIO_LOG_SECTIONNMAME_LEN	16
#define TENIO_LOG_PREFIXSTRING_LEN	64
#define TENIO_LOG_FORMATSTRING_LEN	256
#define TENIO_LOG_OUTPUTSTRING_LEN	1024

#define TENIO_LOG_LOGFLAGSTRING_LEN	16
#define TENIO_LOG_PURELOGDISTANCE	24

#define TENIO_LOG_LINE	TEXT("=====================================================================")

namespace Tenio
{
template
<
	class ConcreateClass,
	DWORD tdwComponentID
>
class TLogHelp
{
public:
	TLogHelp()
	{
		m_bOutputTraceInfo = FALSE;
		m_bOutputErrorInfo = FALSE;
		m_bOutputVerboseInfo = FALSE;
		m_bVerboseInfoToFile = FALSE;
		m_bTraceInfoToFile = FALSE;
		m_bErrorInfoToFile = FALSE;
		m_szPrefixString[0]	= TEXT('\0');
		m_szDefaultOutputFlag[0] = TEXT('\0');
		m_szDefaultLogToFileFlag[0] = TEXT('\0');

		LoadLogConfig(g_szRegistry, tdwComponentID);
	}
	
	ITenioComponentFactory* GetFactory()
	{
		ITenioComponentFactory* pFactory = NULL;
		
		HMODULE hFact = GetModuleHandle(TEXT("TenFact.dll"));
		
		if(hFact != NULL) // TenFact.dll 还没有加载
		{
			IComponent* pComponent = CreateComponentInModule(hFact,
				COMPONENTID_TENIOCOMPONENTFACTORY);

			if(pComponent != NULL)
			{
				pFactory = static_cast<ITenioComponentFactory*>(pComponent);
			}
		}
		
		return pFactory;
	}

	IComponent* CreateComponentInModule(HMODULE hModule, DWORD dwComponentId)
	{
		IComponent* pComponent = NULL;
		
		LPCREATECOMPONENT lpCreateMethod = NULL;
		
		lpCreateMethod = (LPCREATECOMPONENT)GetProcAddress(
			hModule, TEXT("CreateComponent"));
		
		if(lpCreateMethod != NULL)
		{
			pComponent = (*lpCreateMethod)(dwComponentId, 0);
			
			if(pComponent == NULL)
			{
				OutputDebugString(TEXT("Invoke CreateComponent() in TenFact.dll return null.\n"));
			}
		}
		else
		{
			OutputDebugString(TEXT("Can't find CreateComponent() in TenFact.dll or TenFact.dll is missed\n"));
		}
		
		return pComponent;
	}

	void LoadLogConfig(LPCTSTR lpszRegistryFile, DWORD dwComponentID)
	{
		ITenioComponentFactory* pFactory = GetFactory();

		if(pFactory != NULL)
		{
			pFactory->GetLogFlags(g_szRegistry, dwComponentID, m_bOutputVerboseInfo, 
				m_bOutputTraceInfo, m_bOutputErrorInfo);
		}
	}
	
	void MakeLogOutput(BOOL bOutputDebugString, BOOL bToFile, 
		LPCTSTR lpszTitle, LPCTSTR lpszFormat, va_list pArg)
	{
		if(bOutputDebugString || bToFile)
		{
			TCHAR szLogBuffer[TENIO_LOG_OUTPUTSTRING_LEN] = {0};

			LPTSTR lpszPureLog = MakeLogString(szLogBuffer, 
				TENIO_LOG_OUTPUTSTRING_LEN, lpszTitle, lpszFormat, pArg);
			
			if(m_bOutputVerboseInfo)
			{
				OutputDebugString(lpszPureLog);
			}
			
			if(m_bVerboseInfoToFile)
			{
			}
		}
	}

	void OutputVerboseInfo(LPCTSTR lpszFormat, ...)
	{
		va_list pArg = NULL;
		va_start(pArg, lpszFormat);
		MakeLogOutput(m_bOutputVerboseInfo, m_bVerboseInfoToFile,
			TEXT("Verbose"), lpszFormat, pArg);
		va_end(pArg);
	}

	void OutputTraceInfo(LPCTSTR lpszFormat, ...)
	{
		va_list pArg = NULL;
		va_start(pArg, lpszFormat);
		MakeLogOutput(m_bOutputVerboseInfo, m_bVerboseInfoToFile,
			TEXT("Trace"), lpszFormat, pArg);
		va_end(pArg);
	}
	
	void OutputErrorInfo(LPCTSTR lpszFormat, ...)
	{
		va_list pArg = NULL;
		va_start(pArg, lpszFormat);
		MakeLogOutput(m_bOutputVerboseInfo, m_bVerboseInfoToFile,
			TEXT("Error"), lpszFormat, pArg);
		va_end(pArg);
	}


private:
//=============================================================================
//  lpszBuffer/nCharCount 制定输出到的目的缓冲区 输出格式为 :
//  YYYY-MM-DD HH:MM:SS:MS lpszTitle_m_szPrefixString xxxxxxxxxxxxxx
//  返回值指向lpszBuffer中, lpszTitle 处的字符串指针
//=============================================================================

	LPTSTR MakeLogString(LPTSTR lpszBuffer, int nCharCount, LPCTSTR lpszTitle, 
		LPCTSTR lpszFormat, va_list pArgs)
	{
		SYSTEMTIME stTime = {0};
		GetLocalTime(&stTime);

		TCHAR szFormat[TENIO_LOG_FORMATSTRING_LEN] = {0};
		_sntprintf(szFormat, TENIO_LOG_FORMATSTRING_LEN-1, 
			TEXT("%04d-%02d-%02d %02d:%02d:%02d:%03d %s_%s %s\n"),
			stTime.wYear, stTime.wMonth, stTime.wDay,
			stTime.wHour, stTime.wMinute, stTime.wSecond, stTime.wMilliseconds,
			lpszTitle, m_szPrefixString, lpszFormat);

		if(lpszBuffer != NULL)
		{
			_vsntprintf(lpszBuffer, nCharCount-1, szFormat, pArgs);
		}

		LPTSTR lpszLogNoTime = nCharCount - 1 > TENIO_LOG_PURELOGDISTANCE ?
			lpszBuffer + TENIO_LOG_PURELOGDISTANCE : lpszBuffer;

		return lpszLogNoTime;
	}

	BOOL IsStringContain(LPCTSTR lpszString, char chToFind)
	{
		BOOL bFound = FALSE;
		
		int nIndex = 0;
		
		while(lpszString[nIndex] != TEXT('\0'))
		{
			if(lpszString[nIndex] == chToFind)
			{
				bFound = TRUE;
				break;
			}

			nIndex++;
		}
		
		return bFound;
	}
	
	ConcreateClass* Me()
	{
		return static_cast<ConcreateClass*>(this);
	}

private:
	BOOL m_bOutputVerboseInfo;
	BOOL m_bOutputTraceInfo;
	BOOL m_bOutputErrorInfo;
	BOOL m_bVerboseInfoToFile;
	BOOL m_bTraceInfoToFile;
	BOOL m_bErrorInfoToFile;

	TCHAR m_szPrefixString[TENIO_LOG_PREFIXSTRING_LEN];
	TCHAR m_szDefaultOutputFlag[TENIO_LOG_LOGFLAGSTRING_LEN];
	TCHAR m_szDefaultLogToFileFlag[TENIO_LOG_LOGFLAGSTRING_LEN];
}; // template
}; // namespace

#endif