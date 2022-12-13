#ifndef __RESOURCEOPERATOR_T_H__
#define __RESOURCEOPERATOR_T_H__
class ResourceOperator
{
public:

//-----------------------------------------------------------------------------
//	替换一组文件
//-----------------------------------------------------------------------------
	BOOL SubstituteFile(
		RESOURCELIST* pstDestResourceList, 
		RESOURCELIST* pstSourceResourceList,
		BOOL bAtomic)
	{
		CALLSTACKGUARD;
		BOOL bReturn = (pstDestResoureList != NULL)
			&& (pstSourceResoureList != NULL)
			&& (pstDestResoureList->nFileCount > 0)
			&& (pstSourceResoureList->nFileCount > 0)
			&& (pstDestResoureList->nFileCount == pstSourceResoureList->nFileCount);
		
		TCHAR szPath[MAX_PATH] = {0};
		TCHAR szSingleCommand[MAXLENGTH_COMMAND] = {0};
		
		if(bReturn)
		{			
			// 相对路径转换为绝对路径
			for(int nIndex = 0; nIndex < pstDestResourceList->nFileCount; ++nIndex )
			{			
				ScalePath2AbsolutePath(pstDestResourceList->szFileNameList[nIndex], szPath, MAX_PATH);
				_tcscpy(pstDestResourceList->szFileNameList[nIndex], szPath);
			}
			
			for(int nIndex = 0; nIndex < pstSourceResourceList->nFileCount; ++nIndex )
			{			
				ScalePath2AbsolutePath(pstSourceResourceList->szFileNameList[nIndex], szPath, MAX_PATH);
				_tcscpy(pstSourceResourceList->szFileNameList[nIndex], szPath);
			}
			
			if(bAtomic)
			{
				//替换文件
				bReturn = bReturn 
					&& BackUpResource(pstSourceResourceList)
					&& MoveResource(pstDestResourceList, pstSourceResourceList);
				
				// 删除临时文件，执行命令
				if(bReturn)
				{
					DeleteBackupResource(pstDestResourceList);									
				}
				else
				{
					RestoreResource(pstDestResourceList);
					
					CreateScript(pstDestResourceList, 
						pstSourceResourceList);
				}
			}
			else
			{
				BOOL bSuccess = TRUE;
				for(int nIndex = 0; nIndex < pstDestResourceList->nFileCount; ++nIndex)
				{
					bSuccess = SubstituteFile(pstDestResourceList->szFileNameList[nIndex],
						pstSourceResourceList->szFileNameList[nIndex], lpszCommmand);
					
					if(!bSuccess)
					{
						bReturn = FALSE;
					}
				}
			}
		}

		if(bReturn)
		{
			nTaskId = ++ m_nCurrentTaskID;
		}
		
		return nTaskId;
	}
}

int SubstituteFile(
			LPCTSTR  lpszDestFileName, 
			LPCTSTR  lpszSourceFileName,
			LPCTSTR  lpszCommmand)
	{
		int nTaskId = -1;
		RESOURCELIST stDestResoureList = {0};
		RESOURCELIST stSourceResoureList = {0};
       
		if(lpszDestFileName != NULL
			&& lpszSourceFileName != NULL)
		{
			stDestResoureList.nFileCount = 1;
			_tcscpy(stDestResoureList.szFileNameList[0], lpszDestFileName);
			
			stSourceResoureList.nFileCount = 1;
			_tcscpy(stSourceResoureList.szFileNameList[0], lpszSourceFileName);

			nTaskId = SubstituteFile(
			stDestResoureList, 
			stSourceResoureList,
			lpszCommmand,
			TRUE);
		}
		return nTaskId;
	}
	
	BOOL BackUpResource(RESOURCELIST* pstResoureList)
	{
		BOOL bReturn = pstResoureList != NULL;
		
		TCHAR   szFileName[MAX_PATH] = {0};
		
		int nIndex = 0;
		
		if(bReturn)
		{
			for(; (nIndex < pstResoureList->nFileCount) && bReturn; ++nIndex)
			{
				SmartStringFormat(szFileName, MAX_PATH,
					FALSE, _T("%s%s"), 
					pstResoureList->szFileNameList[nIndex],
					TEMP_DOWNLOADFILENAME_SUFFIX);
				
				bReturn = MoveFile(pstResoureList->szFileNameList[nIndex], szFileName);			
			}
			
			if(!bReturn)
			{
				BOOL bSuccess = TRUE;
				for(--nIndex; (nIndex >= 0) && bSuccess; --nIndex)
				{
					SmartStringFormat(szFileName, MAX_PATH,
						FALSE, _T("%s%s"), 
						pstResoureList->szFileNameList[nIndex],
						TEMP_DOWNLOADFILENAME_SUFFIX);
					
					bSuccess = MoveFile(szFileName, pstResoureList->szFileNameList[nIndex]);				
				}
				
				if(!bSuccess)
				{
					OutputTraceInfo(_T("不可恢复的错误：文件无法改回原来的名字"));
				}
			}
		}
		
		return bReturn;
	}
	
	BOOL MoveResource(RESOURCELIST* pstSourceResourceList, 
		RESOURCELIST* pstDestResourceList)
	{
		BOOL bReturn = pstSourceResourceList != NULL
			&& pstDestResourceList != NULL;
		
		BOOL bSuccess = TRUE;
		int nIndex = 0;
		for(; (nIndex < pstDestResourceList->nFileCount) && bReturn; ++nIndex)
		{
			bReturn = MoveFile(pstSourceResourceList->szFileNameList[nIndex], 
				pstDestResourceList->szFileNameList[nIndex]);
		}
		
		if(!bReturn)
		{
			for(--nIndex; (nIndex >= 0) && bSuccess; --nIndex)
			{
				bSuccess = MoveFile(pstDestResourceList->szFileNameList[nIndex],
					pstSourceResourceList->szFileNameList[nIndex]);
				
			}
			
			if(!bSuccess)
			{
				OutputTraceInfo(_T("不可恢复的错误，文件无法改回原来的名字"));
			}
			
		}
		return bReturn;
	}
	
	BOOL DeleteBackupResource(
		RESOURCELIST* pstResoureceList
		)
	{
		BOOL bReturn = pstResoureceList != NULL;
		
		for(int nIndex = 0; (nIndex < pstResoureceList->nFileCount) && bReturn; ++nIndex)
		{
			bReturn = DeleteFile(pstResoureceList->szFileNameList[nIndex]);
		}
		
		if(!bSuccess)
		{
			OutputTraceInfo(_T("不可恢复的错误，无法删除临时文件"));
		}
		return bReturn;
	}
#endif  //__RESOURCEOPERATOR_T_H__