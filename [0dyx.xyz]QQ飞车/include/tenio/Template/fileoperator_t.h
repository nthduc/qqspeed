#ifndef __FILEOPERATOR_T_H__
#define __FILEOPERATOR_T_H__

//=============================================================================
// 	BOOL CombineRelativePath(LPCTSTR lpszOriginFile, LPCTSTR lpszRelativePath,
//		LPTSTR lpszCombinedPath, int nCharCount)
//  lpszOriginFile 是一文件路径(绝对或相对路径)
//  lpszRelativePath 是相对于lpszOriginFile的相对路径
//  lpszCombinedPath 是把lpszRelativePath组合到lpszOriginFile后的路径, 例如:
//	CombineRelativePath("C:\\abc\\1.txt", "..\hello\2.txt") 的结果是:
//		"C:\\abc\\..\\hello\\2.txt"
//=============================================================================

#include "io.h"
#include "stringmanipulate_t.h"

#define MAX_CREATEPROCESS_COMMANDLINE_LENGTH	1024

#define OS_UNKNOWN     0
#define OS_WIN95       1
#define OS_WIN98       2
#define OS_WIN98SE     3
#define OS_WINME       4
#define OS_WINNT       5
#define OS_WIN2000     6
#define OS_WINXP       7
#define OS_WIN2003     8

#pragma comment(lib, "Version.lib")

template<class ConcreteClass>
class FileOperator
{
public:

//-----------------------------------------------------------------------------
//	获取二进制执行文件的版本
//-----------------------------------------------------------------------------

	DWORD GetExeFileVersion(LPCTSTR lpszScalePath)
	{
		TCHAR szFullPath[MAX_PATH] = {0};

		ScalePath2AbsolutePath(lpszScalePath, szFullPath, MAX_PATH);
			
		DWORD dwExeFileVersion = 0;
		
		DWORD dwInfoSize = GetFileVersionInfoSize(szFullPath, NULL);

		LPVOID pVersionInfoData = NULL;
			
		if(dwInfoSize > 0 && Me()->AllocMemory(&pVersionInfoData, dwInfoSize))
		{
			UINT unQuerySize = 0;
			
			VS_FIXEDFILEINFO* pFixedFileInfo = NULL;
			void ** ppFixFileInfo = (void ** )&pFixedFileInfo;
			
			if(GetFileVersionInfo(szFullPath, NULL, dwInfoSize, pVersionInfoData))
			{
				if(VerQueryValue(pVersionInfoData, TEXT("\\"), ppFixFileInfo, &unQuerySize))
				{
					DWORD innerVer[5] = {0};
					
					innerVer[0] = pFixedFileInfo->dwFileVersionMS >> 16;
					innerVer[1] = pFixedFileInfo->dwFileVersionMS & 0xffff;
					innerVer[2] = (pFixedFileInfo->dwFileVersionLS >> 16) / 100;
					innerVer[3] = (pFixedFileInfo->dwFileVersionLS >> 16) - innerVer[2] * 100;
					innerVer[4] = pFixedFileInfo->dwFileVersionLS & 0xffff;
					
					dwExeFileVersion =  innerVer[0] * 10000000
						+ innerVer[1] * 100000
						+ innerVer[2] * 10000
						+ innerVer[3] * 100
						+ innerVer[4] ;
				}
			}

			Me()->ReleaseMemory(pVersionInfoData);

			pVersionInfoData = NULL;
		}

		return dwExeFileVersion;
	}
	
//-----------------------------------------------------------------------------
//	浏览本地文件
//-----------------------------------------------------------------------------
	BOOL GetLocalOpenFileName(LPTSTR lpszFileName, int nCharCount)
	{
		if( lpszFileName )
		{
			HWND hwnd = NULL;        
			HANDLE hf = NULL;      
			OPENFILENAME ofn; 
			TCHAR szFile[MAX_PATH] = {0};
			
			ZeroMemory(&ofn, sizeof(ofn));
			ofn.lStructSize = sizeof(ofn);
			ofn.hwndOwner = hwnd;
			ofn.lpstrFile = szFile;
			
			ofn.lpstrFile[0] = '\0';
			ofn.nMaxFile = sizeof(szFile)/sizeof(TCHAR);
			ofn.lpstrFilter = "All\0*.*\0Text\0*.TXT\0";
			ofn.nFilterIndex = 1;
			ofn.lpstrFileTitle = NULL;
			ofn.nMaxFileTitle = 0;
			ofn.lpstrInitialDir = NULL;
			ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
			
			if (GetOpenFileName(&ofn)==TRUE) 
			{
				hf = CreateFile(ofn.lpstrFile, GENERIC_READ,
					0, (LPSECURITY_ATTRIBUTES) NULL,
					OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL,
					(HANDLE) NULL);
				
				if( hf != INVALID_HANDLE_VALUE )
				{	
					CloseHandle(hf);
					return Me()->StringCopy(lpszFileName, nCharCount, szFile);
				}
			}
		}
		
		return FALSE;
	}

//-----------------------------------------------------------------------------
//	文件是否存在
//-----------------------------------------------------------------------------
	
	BOOL IsFileExist(LPCTSTR lpszScalePath, BOOL bIncludeDirectory = TRUE)
	{
		BOOL bResult = FALSE;

		if(lpszScalePath != NULL && _tcslen(lpszScalePath) > 0)
		{
			bResult = TRUE;
			TCHAR szFullPath[MAX_PATH] = {0};
			
			ScalePath2AbsolutePath(lpszScalePath, szFullPath, MAX_PATH);
			
			if( FALSE == bIncludeDirectory )
			{
				if( GetFileAttributes(szFullPath) & FILE_ATTRIBUTE_DIRECTORY )
				{
					bResult = FALSE;
				}
			}
			
			bResult = bResult && (_taccess(szFullPath, 0) == 0);
		}

		return bResult;
	}

//-----------------------------------------------------------------------------
//	替换路径中的分隔符 '/' 为 '//', 在源字符串上进行替换
//-----------------------------------------------------------------------------
	BOOL ReplacePathSeparator(LPTSTR lpszPath, int nCharCount)
	{
		BOOL bSucc = FALSE;

		if(lpszPath != NULL && nCharCount > 0)
		{
			TCHAR szReplacePath[MAX_PATH] = {0};
			bSucc = Me()->StringCopy(szReplacePath, MAX_PATH, lpszPath);

			TCHAR* szFind = _tcschr(szReplacePath, TEXT('/'));

			while (szFind != NULL && bSucc)
			{
				int nLeftStr = _tcslen(szReplacePath) - _tcslen(szFind);
				lpszPath[nLeftStr] = TEXT('\0');

				bSucc = bSucc && Me()->StringCat(lpszPath, nCharCount, TEXT("\\"));
				bSucc = bSucc && Me()->StringCat(lpszPath, nCharCount, szFind+1);

				bSucc = bSucc && Me()->StringCopy(szReplacePath, MAX_PATH, lpszPath);
				szFind = _tcschr(szReplacePath, TEXT('/'));
			}
		}

		return bSucc;
	}

//-----------------------------------------------------------------------------
//	目录是否存在
//-----------------------------------------------------------------------------

	BOOL IsDirectoryExist(LPCTSTR lpszScareDirectory)
	{
		TCHAR szFullPath[MAX_PATH] = {0};
		
		ScalePath2AbsolutePath(lpszScareDirectory, szFullPath, MAX_PATH);
		
		return (_taccess(szFullPath, 0) == 0);
	}

//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------

	BOOL SimpleCreateDirectory(LPCTSTR lpszScareDirectory)
	{
		BOOL bSuccess = FALSE;

		if(IsDirectoryExist(lpszScareDirectory))
		{
			bSuccess = TRUE;
		}
		else
		{
			TCHAR szFullPath[MAX_PATH] = {0};
			
			ScalePath2AbsolutePath(lpszScareDirectory, szFullPath, MAX_PATH);
			
			bSuccess = CreateDirectory(szFullPath, NULL);
		}

		return bSuccess;
	}

//-----------------------------------------------------------------------------
//	打开文件进行读取
//-----------------------------------------------------------------------------
	
	HANDLE SimpleOpenFile(LPCTSTR lpszFilePath)
	{
		return CreateFile(
			lpszFilePath, 
			GENERIC_READ, 
			FILE_SHARE_READ, 
			NULL, 
			OPEN_EXISTING, 
			FILE_ATTRIBUTE_ARCHIVE,
			);
	}

//-----------------------------------------------------------------------------
//	lpszScalePath 可以传入相对路径或者绝对路径
//-----------------------------------------------------------------------------

	HANDLE SimpleCreateProcess(LPCTSTR lpszScalePath, LPTSTR lpszCommandLine = NULL)
	{
		HANDLE hProcess = NULL;

		TCHAR szFullPath[MAX_PATH] = {0};
		ScalePath2AbsolutePath(lpszScalePath, szFullPath, MAX_PATH);

		if(IsFileExist(szFullPath))
		{
			STARTUPINFO			stStartUpInfo = {0};
			PROCESS_INFORMATION stProcessInfo = {0};

			GetStartupInfo(&stStartUpInfo);

			TCHAR szFolder[MAX_PATH] = {0};
			ExtractPath(szFullPath, szFolder, MAX_PATH);
			
			TCHAR szInvokeCommand[MAX_CREATEPROCESS_COMMANDLINE_LENGTH] = {0};
			Me()->SmartStringFormat(szInvokeCommand, MAX_CREATEPROCESS_COMMANDLINE_LENGTH,
				FALSE, TEXT("%s %s"), szFullPath, lpszCommandLine);

			BOOL bSuccess = CreateProcess(
				NULL,           // name of executable module
				szInvokeCommand,      // command line string
				NULL,				  // SD
				NULL,				  // SD
				FALSE,                // handle inheritance option
				0,					  // creation flags
				NULL,                 // new environment block
				szFolder,			  // current directory name
				&stStartUpInfo,       // startup information
				&stProcessInfo        // process information
				);

			if(bSuccess)
			{
				hProcess = stProcessInfo.hProcess;
			}
		}

		return hProcess;
	}

//-----------------------------------------------------------------------------
// 判断进程是否正在运行
//-----------------------------------------------------------------------------

	BOOL IsProcessRunning(HANDLE hProcess)
	{
		BOOL bRunning = FALSE;

		if(hProcess != NULL)
		{
			DWORD dwResult = WaitForSingleObject(hProcess, 0);

			if(dwResult != WAIT_OBJECT_0)
			{
				bRunning = TRUE;
			}
		}

		return bRunning;
	}

//-----------------------------------------------------------------------------
//	从带文件名路径中解出路径部分, 例如 "C:\\Winnt\\QQ.exe" 解出为 "C:\\Winnt"
//  输入参数 lpszScalePath 可以为相对本进程执行文件目录的相对路径或者绝对路径
//  例如本进程为 C:\\Winnt\\Test.exe, 则输入参数 "abc\\qq\\cc.txt" 解出路径为:
//  "C:\\Winnt\\abc\\qq\\cc"
//-----------------------------------------------------------------------------
	
	BOOL ExtractPath(LPCTSTR lpszScalePath, LPTSTR lpszFolderPath, int nCharCount)
	{
		BOOL bSuccess = FALSE;

		TCHAR szFull[MAX_PATH] = {0};

		if(ScalePath2AbsolutePath(lpszScalePath, szFull, MAX_PATH))
		{
			TCHAR* pSplitter = _tcsrchr(szFull, TEXT('\\'));

			if(pSplitter == NULL)
			{
				pSplitter = szFull;
			}

			TCHAR* pSplitter2 = _tcsrchr(pSplitter, TEXT('/'));

			if(pSplitter2 != NULL)
			{
				pSplitter = pSplitter2;
			}

			if(pSplitter != NULL)
			{
				int nToCopyCount = pSplitter - szFull;

				bSuccess = Me()->StringCopyFix(
					lpszFolderPath, 
					nCharCount, 
					szFull, 
					nToCopyCount);
			}
		}

		return bSuccess;
	}

//-----------------------------------------------------------------------------
//  解出文件名部分, 例如 "C:\QQ.exe" 解出为 "QQ.exe"
//-----------------------------------------------------------------------------

	BOOL ExtractFileName(LPCTSTR lpszScalePath, LPTSTR lpszFileName, int nCharCount)
	{
		BOOL bSuccess = FALSE;

		if(lpszScalePath != NULL && lpszFileName != NULL && nCharCount > 0)
		{
			TCHAR* pSplitter = _tcsrchr(lpszScalePath, TEXT('\\'));

			if(pSplitter == NULL)
			{
				pSplitter = _tcsrchr(lpszScalePath, TEXT('/'));
			}

			LPCTSTR pszParsedFileName = NULL;

			if(pSplitter != NULL)
			{
				pszParsedFileName = pSplitter + 1;
			}
			else
			{
				pszParsedFileName = lpszScalePath;
			}

			bSuccess = Me()->StringCopy(
				lpszFileName, 
				nCharCount, 
				pszParsedFileName);
		}

		return bSuccess;
	}

//-----------------------------------------------------------------------------
//  解出扩展名
//-----------------------------------------------------------------------------

	BOOL ExtractFileExtName(LPCTSTR lpszPath, LPTSTR lpszExt, int nCharCount)
	{
		BOOL bSuccess = FALSE;

		if(lpszPath != NULL && lpszExt != NULL)
		{
			TCHAR* pSplitter = _tcsrchr(lpszPath, TEXT('.'));

			if (pSplitter != NULL) 
			{
				bSuccess = Me()->StringCopy(lpszExt, nCharCount, pSplitter + 1);
			}
		}

		return bSuccess;
	}

	//-----------------------------------------------------------------------------
	//  解出文件名不带扩展名  例如 "C:\QQ.exe" 解出为 "QQ"
	//-----------------------------------------------------------------------------
	
	BOOL ExtractFileNameWithoutExt(LPCTSTR lpszPath, LPTSTR lpszName, int nCharCount)
	{
		BOOL bSuccess = FALSE;
		
		if(lpszPath != NULL && lpszName != NULL)
		{
			TCHAR* pSplitter = _tcsrchr(lpszPath, TEXT('\\'));
			
			if(pSplitter == NULL)
			{
				pSplitter = _tcsrchr(lpszPath, TEXT('/'));
			}
			
			LPCTSTR pszParsedFileName = NULL;
			
			if(pSplitter != NULL)
			{
				pszParsedFileName = pSplitter + 1;
			}
			else
			{
				pszParsedFileName = lpszPath;
			}

			int nExtLen = 0;
			TCHAR* pExtSplitter = _tcsrchr(lpszPath, TEXT('.'));
			if(pExtSplitter != NULL)
			{
				nExtLen = Me()->StringLength(pExtSplitter);
			}
			
			int nNameLen = Me()->StringLength(pszParsedFileName) - nExtLen;

			bSuccess = Me()->StringCopyFix(
				lpszName, 
				nCharCount, 
				pszParsedFileName,
				nNameLen);
		}
		
		return bSuccess;
	}

//-----------------------------------------------------------------------------
//  把 ScalePath 转化为 AbsolutePath
//-----------------------------------------------------------------------------

	BOOL ScalePath2AbsolutePath(
		LPCTSTR lpszScalePath, 
		LPTSTR	lpszAbsolutePath,
		int		nCharCount
		)
	{
		BOOL bSuccess = FALSE;

		if(IsRelativePath(lpszScalePath))
		{
			bSuccess = Relative2AbsolutePath(
				lpszScalePath, 
				lpszAbsolutePath, 
				nCharCount);
		}
		else
		{
			bSuccess = Me()->StringCopy(lpszAbsolutePath, nCharCount, lpszScalePath);
		}

		return bSuccess;
	}

//-----------------------------------------------------------------------------
//  判断是否相对路径
//-----------------------------------------------------------------------------
	
	BOOL IsRelativePath(LPCTSTR lpszPath)
	{
		BOOL bResult = TRUE;

		if(lpszPath != NULL)
		{
			if(_tcschr(lpszPath, TEXT(':')) != NULL)
			{
				bResult = FALSE;
			}
			else if(lpszPath[0] == TEXT('\\'))
			{
				bResult = FALSE;
			}
		}

		return bResult;
	}

//-----------------------------------------------------------------------------
//  相对路径转换为绝对路径, 推荐使用 ScalePath2AbsolutePath
//  此处的输入参数 lpszRelativePath 必须为相对路径, 否则会返回错误结果
//-----------------------------------------------------------------------------
	
	BOOL Relative2AbsolutePath(
		LPCTSTR lpszRelativePath, 
		LPTSTR	lpszOutput,
		int		nCharCount)
	{
		BOOL bSuccess = FALSE;
		
		if(lpszRelativePath != NULL && lpszOutput != NULL && nCharCount > 1)
		{
			bSuccess = TRUE;
			
			_sntprintf(
				lpszOutput, 
				nCharCount - 1,
				TEXT("%s\\%s"),
				GetRootPath(),
				lpszRelativePath);
			
			lpszOutput[nCharCount - 1] = TEXT('\0');
		}
		
		return bSuccess;
	}
	
//-----------------------------------------------------------------------------
//  获取本进程对应的执行文件的路径, 不带最后的 '\\'
//-----------------------------------------------------------------------------
	
	LPCTSTR GetRootPath()
	{
		static TCHAR s_szPath[MAX_PATH] = {0};
		
		HMODULE hModule = ::GetModuleHandle(NULL);
		
		GetModuleFileName(hModule, s_szPath, MAX_PATH);
		
		TCHAR* pTemp = _tcsrchr(s_szPath, TEXT('\\'));
		
		if(pTemp != NULL)
		{
			pTemp[0] = TEXT('\0');
		}
		
		return s_szPath;
	}

//-----------------------------------------------------------------------------
//【对外接口】在目录路径后面接上文件名, 自动识别目录路径后面是否有 '\\'. 例如:
//            CatFolderPathAndFileName(_T("C:\\Abc"), _T("1.txt").....) 则返回
//            C:\Abc\1.txt
//-----------------------------------------------------------------------------
	
	BOOL CatFolderPathAndFileName(
		LPCTSTR lpszFolderPath,
		LPCTSTR lpszFileName,
		LPTSTR  lpszResult,
		int nCharCount)
	{
		BOOL bSuccess = TRUE;

		bSuccess = (
			lpszFolderPath != NULL &&
			lpszFileName != NULL &&
			lpszResult != NULL &&
			nCharCount > 0);

		if(bSuccess)
		{
			int nFolderPathLen = _tcslen(lpszFolderPath);

			if(lpszFolderPath[nFolderPathLen - 1] == TEXT('\\'))
			{
				bSuccess = Me()->SmartStringFormat(
					lpszResult,
					nCharCount,
					FALSE,
					TEXT("%s%s"),
					lpszFolderPath,
					lpszFileName);
			}
			else
			{
				bSuccess = Me()->SmartStringFormat(
					lpszResult,
					nCharCount,
					FALSE,
					TEXT("%s\\%s"),
					lpszFolderPath,
					lpszFileName);
			}
		}

		return bSuccess;
	}

//-----------------------------------------------------------------------------
// 检查文件全路径是否有效,检查是否有无效字符 <>"|*? ,
// "d:\3_alert_index?type=1.50.5\12.5中含?,为无效文件名
// made by weigong
//-----------------------------------------------------------------------------
	BOOL IsFilePathValid(LPCTSTR lpszFilePath)
	{
		BOOL bReturn = FALSE;
		TCHAR szInvalidFileNameChar[] = _T("<>\"|*?");

		if (lpszFilePath != NULL)
		{
			bReturn = TRUE;
		}

		for (int i = 0; i < sizeof(szInvalidFileNameChar)/sizeof(TCHAR) - 1; i++)
		{
			TCHAR szChar = szInvalidFileNameChar[i];
			TCHAR * pSearch = NULL;

			pSearch = _tcschr(lpszFilePath, szChar);

			if (pSearch != NULL)
			{
				bReturn = FALSE;
				break;
			}
		}

		return bReturn;
	}

//-----------------------------------------------------------------------------
// 检查文件名是否有效,检查是否有无效字符 <>"|*?\/ ,
// "3_alert_index?type=1.50.5\12.5中含?和\,为无效文件名
// made by weigong
//-----------------------------------------------------------------------------
	BOOL IsFileNameValid(LPCTSTR lpszFileName)
	{
		BOOL bReturn = FALSE;
		TCHAR szInvalidFileNameChar[] = _T("/\\<>\"|*?");

		if (lpszFileName != NULL)
		{
			bReturn = TRUE;
		}

		for (int i = 0; i < sizeof(szInvalidFileNameChar)/sizeof(TCHAR) - 1; i++)
		{
			TCHAR szChar = szInvalidFileNameChar[i];
			TCHAR * pSearch = NULL;

			pSearch = _tcschr(lpszFileName, szChar);

			if (pSearch != NULL)
			{
				bReturn = FALSE;
				break;
			}
		}

		return bReturn;
	}
//-----------------------------------------------------------------------------
// 创建该文件的全路径,比如输入"d:\\qqgame\qqgame.exe",如果没有qqgame目录,这里可
// 协助创建之,同时检查目录有效性
// made by weigong
//-----------------------------------------------------------------------------
	
	BOOL CreateFileDirectory(LPCTSTR lpszFileName)
	{
		BOOL bReturn = FALSE;
		TCHAR szPathOfSaveFilename[MAX_PATH] = {0};

		// 1 check validity of file path
		if (lpszFileName != NULL && _tcslen(lpszFileName) > 0)
		{
			bReturn = IsFilePathValid(lpszFileName);
		}

		// 2 get path part of full path
		if (bReturn)
		{
			bReturn = ExtractPath(lpszFileName, szPathOfSaveFilename, 
				sizeof(szPathOfSaveFilename)/sizeof(TCHAR));
		}

		// 3 create it
		if (bReturn)
		{
			bReturn = SimpleCreateDirectory(szPathOfSaveFilename);
		}

		return bReturn;
	}

//-----------------------------------------------------------------------------
// 检查磁盘空间是否已满,目前仅支持了DWORD大小的比较,如果输入文件过大,麻烦你老人
// 家增加一个函数
// made by weigong
//-----------------------------------------------------------------------------
	
	BOOL IsDiskFreeSpaceEnough(DWORD dwFileSize)
	{
		BOOL bReturn = FALSE;
		ULARGE_INTEGER uFreeBytesAvailable;		// bytes available to caller
		ULARGE_INTEGER uTotalNumberOfBytes;		// bytes on disk
		ULARGE_INTEGER uTotalNumberOfFreeBytes; // free bytes on disk

		bReturn = GetDiskFreeSpaceEx(NULL, &uFreeBytesAvailable, &uTotalNumberOfBytes, 
			&uTotalNumberOfFreeBytes) != 0;

		if (bReturn)
		{
			bReturn = uFreeBytesAvailable.QuadPart > dwFileSize;
		}

		return bReturn;
	}

//-----------------------------------------------------------------------------
//【对外接口】打开一个随机存储文件
//-----------------------------------------------------------------------------

	HANDLE OpenRandomFile(LPCTSTR lpszScalePath)
	{
		TCHAR szFullPath[MAX_PATH] = {0};
		ScalePath2AbsolutePath(lpszScalePath, szFullPath, MAX_PATH);

		HANDLE hFile = NULL;
		
		hFile = CreateFile(szFullPath, GENERIC_READ|GENERIC_WRITE,
			0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

		DWORD dwErr = GetLastError();

		return hFile;
	}

	HANDLE SmartOpenFileToWrite(LPCTSTR lpszFile)
	{
		HANDLE hFile = INVALID_HANDLE_VALUE;

		if(!Me()->IsStringEmpty(lpszFile))
		{
			if(IsFileExist(lpszFile))
			{
				Me()->OutputTraceInfo(TEXT("创建文件进行写入, 已存在, 准备覆盖:%s"), 
					lpszFile);
			}
			else
			{
				Me()->OutputTraceInfo(TEXT("创建文件进行写入, 没有同名文件, 新建一个:%s"), 
					lpszFile);
			}

			hFile = CreateFile(lpszFile, GENERIC_WRITE,
				0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

			if(hFile == INVALID_HANDLE_VALUE)
			{
				Me()->OutputErrorInfo(
					TEXT("创建输出文件出错 FilePath:%s ErrorCode:%u"), 
					lpszFile, GetLastError());
			}
		}

		return hFile;
	}

	HANDLE SmartOpenFileToRead(LPCTSTR lpszFile)
	{
		HANDLE hFile = INVALID_HANDLE_VALUE;
		
		if(!Me()->IsStringEmpty(lpszFile))
		{
			if(IsFileExist(lpszFile))
			{
				Me()->OutputTraceInfo(TEXT("打开文件进行读取, 存在:%s"), 
					lpszFile);

				hFile = CreateFile(lpszFile, GENERIC_READ,
					0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
				
				if(hFile == INVALID_HANDLE_VALUE)
				{
					Me()->OutputErrorInfo(
						TEXT("打开文件进行读取出错 FilePath:%s ErrorCode:%u"), 
						lpszFile, GetLastError());
				}
			}
			else
			{
				Me()->OutputTraceInfo(TEXT("打开文件进行读取, 文件不存在:%s"), 
					lpszFile);
			}
		}
		
		return hFile;
	}

	BOOL SmartCloseFile(HANDLE hFile)
	{
		BOOL bSuccess = FALSE;

		if(hFile != INVALID_HANDLE_VALUE)
		{
			bSuccess = CloseHandle(hFile);

			if(!bSuccess)
			{
				Me()->OutputErrorInfo(
					TEXT("关闭文件出错 Handle:%u ErrorCode:%u"), 
					hFile, GetLastError());
			}
		}

		return bSuccess;
	}

	BOOL SmartDeleteFile(LPCTSTR lpszScalePath)
	{
		BOOL bSuccess = FALSE;
		
		if(lpszScalePath != NULL)
		{
			TCHAR szFullPath[MAX_PATH] = {0};
			
			ScalePath2AbsolutePath(lpszScalePath, szFullPath, MAX_PATH);

			if(IsFileExist(szFullPath))
			{   
				DWORD dwAttributes = GetFileAttributes(szFullPath);
				
				if(dwAttributes & FILE_ATTRIBUTE_READONLY)
				{
					SetFileAttributes(szFullPath,  FILE_ATTRIBUTE_NORMAL);
				}
				
				bSuccess = DeleteFile(szFullPath);								
			}
		}
		
		return bSuccess;
	}

	BOOL SmartIsValidFileHandle(HANDLE hFile)
	{
		return (hFile != INVALID_HANDLE_VALUE);
	}

	BOOL SmartWriteToFile(HANDLE hFile, PVOID pData, DWORD dwLen)
	{
		BOOL bSuccess = FALSE;

		if(SmartIsValidFileHandle(hFile) && pData != NULL)
		{
			DWORD dwActualWritten = 0;

			bSuccess = WriteFile(hFile, pData, dwLen, &dwActualWritten, NULL);

			if(!bSuccess)
			{
				Me()->OutputErrorInfo(
					TEXT("写入文件出错 Handle:%u ErrorCode:%u"), 
					hFile, GetLastError());
			}
		}

		return bSuccess;
	}

	DWORD SmartReadFile(HANDLE hFile, PVOID pData, DWORD dwLen)
	{
		DWORD dwDataRead = 0;

		if(SmartIsValidFileHandle(hFile) && pData != NULL)
		{
			BOOL bSuccess = ReadFile(hFile, pData, dwLen, &dwDataRead, NULL);

			if(!bSuccess)
			{
				Me()->OutputErrorInfo(
					TEXT("读取文件出错 Handle:%u ErrorCode:%u"), 
					hFile, GetLastError());
			}
		}

		return dwDataRead;
	}

	BOOL SmartWriteToFile16(HANDLE hFile, PVOID pData)
	{
		return SmartWriteToFile(hFile, pData, 2);
	}

	BOOL SmartWriteToFile32(HANDLE hFile, PVOID pData)
	{
		return SmartWriteToFile(hFile, pData, 4);
	}

	DWORD SmartReadFile16(HANDLE hFile, PVOID pData)
	{
		return SmartReadFile(hFile, pData, 2);
	}

	DWORD SmartReadFile32(HANDLE hFile, PVOID pData)
	{
		return SmartReadFile(hFile, pData, 4);
	}
	
	// 移动文件的读写指针, nStep 为相对当前位置的偏移量
	BOOL SmartFilePointerStepTo(HANDLE hFile, int nStep)
	{
		BOOL bSuccess = FALSE;

		if(SmartIsValidFileHandle(hFile))
		{
			SetFilePointer(hFile, nStep, NULL, FILE_CURRENT);

			bSuccess = TRUE;
		}

		return bSuccess;
	}

//-----------------------------------------------------------------------------
//【对外接口】从一个随机存储文件读取内容, 并移动指针. 返回读取的实际长度.
//-----------------------------------------------------------------------------

	DWORD ReadFromRandomFile(HANDLE hFile, LPVOID lpBuffer, DWORD dwToReadLen)
	{
		DWORD dwActualRead = 0;

		if(ReadFile(hFile, lpBuffer, dwToReadLen, &dwActualRead, NULL))
		{
			/*
			if(dwActualRead > 0)
			{
				SetFilePointer(hFile, dwActualRead, NULL, FILE_CURRENT);
			}*/
		}

		return dwActualRead;
	}

	int GetOsVer()
	{
		int nSystemVersion = OS_UNKNOWN;

		OSVERSIONINFO osVerInfo = {0};
		osVerInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	
		::GetVersionEx(&osVerInfo);
		
		DWORD dwPlatId = osVerInfo.dwPlatformId;
		DWORD majorVer = osVerInfo.dwMajorVersion;
		DWORD minorVer = osVerInfo.dwMinorVersion;
	
		if(dwPlatId == VER_PLATFORM_WIN32_NT) 
		{
			//Windows NT/2000
			if( majorVer <= 4 )
			{
				nSystemVersion = OS_WINNT;
			}
			else if( majorVer == 5 && minorVer == 0)
			{
				nSystemVersion = OS_WIN2000;
			}
			else if( majorVer == 5 && minorVer ==1 )
			{
				nSystemVersion = OS_WINXP;
			}
			else if( majorVer == 5 && minorVer == 2)
			{
				nSystemVersion = OS_WIN2003;
			}
			else
			{
				nSystemVersion = OS_UNKNOWN;
			}
		
		}
		else if(dwPlatId == VER_PLATFORM_WIN32_WINDOWS)
		{
			//Windows 9x/ME
			if( majorVer == 4 && minorVer ==0)
			{
				nSystemVersion = OS_WIN95;
			}
			else if( majorVer == 4 && minorVer == 10)
			{
				if( osVerInfo.szCSDVersion[1] == 'A' )
				{
					nSystemVersion = OS_WIN98SE;
				}
				else
				{
					nSystemVersion = OS_WIN98;
				}
			}
			else if( majorVer == 4 && minorVer == 90 )
			{
				nSystemVersion = OS_WINME;
			}
			else
			{
				nSystemVersion = OS_UNKNOWN;
			}
		}
		else
		{
			nSystemVersion = OS_UNKNOWN;
		}

		return nSystemVersion;
	}

	LPCTSTR FindRightSideSeparator(LPCTSTR lpszPath)
	{
		LPCTSTR lpszSeparator = _tcsrchr(lpszPath, TEXT('\\'));

		if(lpszSeparator == NULL)
		{
			lpszSeparator = _tcsrchr(lpszPath, TEXT('/'));
		}

		return lpszSeparator;
	}

	LPTSTR FindRightSideSeparator(LPTSTR lpszPath)
	{
		LPCTSTR lpszSeparator = _tcsrchr(lpszPath, TEXT("\\"));

		if(lpszSeparator == NULL)
		{
			lpszSeparator = _tcsrchr(lpszPath, TEXT("/"));
		}

		return lpszSeparator;
	}

	BOOL CombineRelativePath(LPCTSTR lpszOriginFile, LPCTSTR lpszRelativePath,
		LPTSTR lpszCombinedPath, int nCharCount)
	{
		BOOL bSuccess = FALSE;

		BOOL bParamOk = (lpszOriginFile != NULL && lpszRelativePath != NULL &&
			lpszCombinedPath != NULL && nCharCount > 0);

		if(bParamOk)
		{
			LPCTSTR lpszSeparator = FindRightSideSeparator(lpszOriginFile);

			lpszCombinedPath[0] = TEXT('\0');

			if(lpszSeparator != NULL)
			{
				int nPreCount = lpszSeparator - lpszOriginFile + 1;

				Me()->StringCopyFix(lpszCombinedPath, nCharCount, lpszOriginFile,
					nPreCount);
			}
			else
			{
				Me()->StringCopy(lpszCombinedPath, nCharCount, lpszOriginFile);
				Me()->StringCat(lpszCombinedPath, nCharCount, TEXT("\\"));
			}

			Me()->StringCat(lpszCombinedPath, nCharCount, lpszRelativePath);

			bSuccess = TRUE;
		}

		return bSuccess;
	}

//-----------------------------------------------------------------------------
// 遍历 lpszPath 目录下面的文件
// 使用者需要自己实现 OnFindFile 这个函数，来处理遍历中的每一个文件
//-----------------------------------------------------------------------------
	void EnumWindowFiles(LPCTSTR lpszPath, DWORD dwParam)
	{
		TCHAR szFindNamePattern[MAX_PATH] = {0};
		_stprintf(szFindNamePattern,_T("%s*"),lpszPath);
		
		WIN32_FIND_DATA   findData = {0};
		HANDLE	hFindHandle = NULL;
		BOOL bFinished = FALSE;
		
		hFindHandle = FindFirstFile(szFindNamePattern,&findData);
		
		if (hFindHandle != INVALID_HANDLE_VALUE) 
		{
			TCHAR szPluginConfigFile[MAX_PATH] = {0};
			
			while (bFinished == FALSE) 
			{
				// 每遇到一个文件，调用OnFindFile
				OnFindFile(findData, dwParam);
				
				if (FindNextFile(hFindHandle,&findData) == FALSE) 
				{
					bFinished = TRUE;
				}
			}
			
			FindClose(hFindHandle);
		}
		
	}
	
	// 使用者需要自己实现 OnFindFile 这个函数，来处理遍历中的每一个文件
	virtual void OnFindFile(WIN32_FIND_DATA& stFindData, DWORD dwParam)
	{
		
	}

private:

//-----------------------------------------------------------------------------
//  获取实现类类型的指针
//-----------------------------------------------------------------------------

	ConcreteClass* Me()
	{
		return static_cast<ConcreteClass*> (this);
	}
};

#endif