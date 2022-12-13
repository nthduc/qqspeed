#ifndef __FILEPATH_T_H__
#define __FILEPATH_T_H__

template<class TClass>
class TFilePath
{
	public:

	//////////////////////////////////////////////////////////////////////////
	//由相对路径，文件名获得全路径。
	//////////////////////////////////////////////////////////////////////////
	BOOL GetFullFilePathName(
		LPCTSTR lpszRelativeFolder, 
		LPCTSTR lpszFileName,
		LPTSTR lpszFilePath, 
		UINT nMaxLen)
	{
		BOOL bSuccess = FALSE;

		if(lpszFilePath != NULL && lpszRelativeFolder != NULL && lpszFileName != NULL)
		{
			TCHAR szExePath[MAX_PATH] = {0};
			HMODULE hProcess = ::GetModuleHandle(NULL);
			::GetModuleFileName(hProcess, szExePath, MAX_PATH);
			LPTSTR lpszTemp = _tcsrchr(szExePath, _T('\\'));
			
			if(lpszTemp != NULL)
			{
				lpszTemp[1] = _T('\0');
			}
			
			if((_tcslen(szExePath) + _tcslen(lpszRelativeFolder) + 
				_tcslen(lpszFileName) + _tcslen(TEXT("\\"))) < nMaxLen)
			{
				_tcscpy(lpszFilePath, szExePath);
				_tcscat(lpszFilePath, lpszRelativeFolder);
				if(!IsFormatFolder(lpszFilePath))
				{	// 判断文件夹路径后面是否跟了斜杠符号
					_tcscat(lpszFilePath, TEXT("\\"));
				}
				_tcscat(lpszFilePath, lpszFileName);

				bSuccess = TRUE;
			}
		}
		return bSuccess;
	}

	//////////////////////////////////////////////////////////////////////////
	// 是否为全路径
	//////////////////////////////////////////////////////////////////////////
	BOOL IsFullFilePath(LPCTSTR lpszFilePath)
	{
		BOOL bFullFilePath = FALSE;

		if(lpszFilePath != NULL)
		{
			if(_tcslen(lpszFilePath) > 2 && (*(lpszFilePath+1) == ':'))
			{
				bFullFilePath = TRUE;
			}
			else
			{
				TCHAR szTemp[3] = {0};
				_tcsncpy(szTemp, lpszFilePath, 2);
				if(_tcscmp(szTemp, _T("\\\\")) == 0)
				{
					bFullFilePath = TRUE;
				}
			}
		}
		return bFullFilePath;
	}

	//////////////////////////////////////////////////////////////////////////
	//将相对路径转换成全路径，如果输入路径已经为全路径，则不用转换
	//lpszFolder 相对文件夹或绝对文件夹路径
	//////////////////////////////////////////////////////////////////////////
	BOOL TransformToFullFolderPath(
		LPCTSTR lpszFolder, 
		LPTSTR lpszFullFilePath, 
		UINT nMaxLen)
	{
		BOOL bSuccess = FALSE;
		
		if(lpszFolder != NULL && _tcslen(lpszFolder) > 0)
		{
			if(IsFullFilePath(lpszFolder))
			{
				if(_tcslen(lpszFolder) < nMaxLen)
				{
					_tcscpy(lpszFullFilePath, lpszFolder);
					if(!IsFormatFolder(lpszFullFilePath))
					{	// 判断全路径后面是否跟了斜杠符号
						_tcscat(lpszFullFilePath, TEXT("\\"));
					}
					bSuccess = TRUE;
				}
			}
			else
			{
				bSuccess = GetFullFilePathName(
					lpszFolder, 
					_T(""), 
					lpszFullFilePath, 
					nMaxLen);
			}
		}
		return bSuccess;
	}

	//////////////////////////////////////////////////////////////////////////
	// 判断路径后是否带'\'
	//////////////////////////////////////////////////////////////////////////
	BOOL IsFormatFolder(LPTSTR lpszFolder)
	{
		BOOL bHave = FALSE;

		if(lpszFolder != NULL)
		{
			int nFolderPathLen = _tcslen(lpszFolder);
			
			if(nFolderPathLen > 0)
			{
				if(lpszFolder[nFolderPathLen - 1] == TEXT('\\') ||
					lpszFolder[nFolderPathLen - 1] == TEXT('/'))
				{
					bHave = TRUE;
				}
			}
		}
		return bHave;
	}
};

#endif