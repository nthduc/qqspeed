#ifndef __INIFILEMANIPULATE_T_H__
#define __INIFILEMANIPULATE_T_H__

#define INIFILEMANIPULATE_SECTIONLENGTH		512
#define INIFILEMANIPULATE_KEYLENGTH			512
#define INIFILEMANIPULATE_VALUELENGTH		512
#define USERCONFIGFILEPATH					_T("Config\\Users")

//////////////////////////////////////////////////////////////////////////
// ����WriteUserConfigXXX���������½QQ�Ŷ�Ӧ��Ŀ¼��д�����ļ���
// Ŀ¼������ʱ�Զ�������
// ReadUserConfigXXX����ͬ��
//////////////////////////////////////////////////////////////////////////

template<class TSubclass>
class TINIFileManipulate
{
public:

	TINIFileManipulate()
	{
		m_nMyUIN = -1;
	}

	virtual ~TINIFileManipulate()
	{
	}

//-----------------------------------------------------------------------------
//���ⲿ�ӿڡ���ȡָ��UIN��ConfigĿ¼(����·��), �����������'\\'
//-----------------------------------------------------------------------------

	BOOL GetUserConfigPath(DWORD dwUIN, LPTSTR lpszPath, int nCharCount)
	{
		BOOL bSuccess = FALSE;

		TCHAR szUserConfig[MAX_PATH] = {0};
		
		bSuccess = Me()->SmartStringFormat(szUserConfig, MAX_PATH, FALSE, TEXT("%s\\%u"),
			USERCONFIGFILEPATH, dwUIN);

		bSuccess = bSuccess && Me()->ScalePath2AbsolutePath(szUserConfig, 
			lpszPath, nCharCount);

		return bSuccess;
	}

//-----------------------------------------------------------------------------
//���ⲿ�ӿڡ���ini�ļ�д������, nValue ֧�ָ���. 
//-----------------------------------------------------------------------------
	
	BOOL WriteUserConfigInt(
		LPCTSTR lpszFileName,
		LPCTSTR lpszSection,
		LPCTSTR lpszKey,
		int		nValue)
	{
		BOOL bSuccess = TRUE;
		TCHAR szConfigPath[MAX_PATH] = {0};

		bSuccess = GetUserConfigFilePath(
			szConfigPath,
			MAX_PATH,
			lpszFileName);

		if (bSuccess)
		{
			bSuccess = WriteINIInt(
				szConfigPath,
				lpszSection,
				lpszKey,
				nValue);
		}

		return bSuccess;
	}

	BOOL WriteINIInt(
		LPCTSTR lpszFileScalePath, 
		LPCTSTR lpszSection, 
		LPCTSTR lpszKey, 
		int		nValue)
	{
		BOOL bSuccess = FALSE;
		
		// �γɾ���·��
		TCHAR szAbsulutePath[MAX_PATH] = {0};
		
		bSuccess = Me()->ScalePath2AbsolutePath(
			lpszFileScalePath, 
			szAbsulutePath, 
			MAX_PATH
			);
		
		// Value
		TCHAR szValue[INIFILEMANIPULATE_VALUELENGTH] = {0};
		
		if(bSuccess)
		{
			bSuccess = Me()->SmartStringFormat(
				szValue,
				INIFILEMANIPULATE_VALUELENGTH,
				FALSE,
				TEXT("%d"),
				nValue
				);
		}
		
		// д���ļ���
		if(bSuccess)
		{
			WritePrivateProfileString(
				lpszSection,
				lpszKey,
				szValue,
				szAbsulutePath
				);
		}
		
		return bSuccess;
	}

//-----------------------------------------------------------------------------
//���ⲿ�ӿڡ���ini�ļ�д��unsigned int�������� 
//-----------------------------------------------------------------------------
	BOOL WriteUserConfigUInt(
		LPCTSTR lpszFileName,
		LPCTSTR lpszSection,
		LPCTSTR lpszKey,
		UINT	unValue)
	{
		BOOL bSuccess = TRUE;
		TCHAR szConfigPath[MAX_PATH] = {0};

		bSuccess = GetUserConfigFilePath(
			szConfigPath,
			MAX_PATH,
			lpszFileName);

		if (bSuccess)
		{
			bSuccess = WriteINIUInt(
				szConfigPath,
				lpszSection,
				lpszKey,
				unValue);
		}

		return bSuccess;
	}

	BOOL WriteINIUInt(
		LPCTSTR lpszFileScalePath, 
		LPCTSTR lpszSection, 
		LPCTSTR lpszKey, 
		UINT	unValue)
	{
		BOOL bSuccess = FALSE;
		
		// �γɾ���·��
		TCHAR szAbsulutePath[MAX_PATH] = {0};
		
		bSuccess = Me()->ScalePath2AbsolutePath(
			lpszFileScalePath, 
			szAbsulutePath, 
			MAX_PATH
			);
		
		// Value
		TCHAR szValue[INIFILEMANIPULATE_VALUELENGTH] = {0};
		
		if(bSuccess)
		{
			bSuccess = Me()->SmartStringFormat(
				szValue,
				INIFILEMANIPULATE_VALUELENGTH,
				FALSE,
				TEXT("%u"),
				unValue
				);
		}
		
		// д���ļ���
		if(bSuccess)
		{
			WritePrivateProfileString(
				lpszSection,
				lpszKey,
				szValue,
				szAbsulutePath
				);
		}
		
		return bSuccess;
	}
	
//-----------------------------------------------------------------------------
//���ⲿ�ӿڡ���ini�ļ�д������, nValue ֧�ָ���. ��������SectionΪƴ��ֵ������ 
// lpszSectionHeader = "Server", nNumber = "5", ����ini�ļ���[Server5]д����ֵ. 
//-----------------------------------------------------------------------------
	
	BOOL WriteUserConfigInt(
		LPCTSTR lpszFileName,
		LPCTSTR lpszSectionHeader, 
		int		nNumber, 
		LPCTSTR lpszKey, 
		int		nValue)
	{
		BOOL bSuccess = TRUE;
		TCHAR szConfigPath[MAX_PATH] = {0};
		
		bSuccess = GetUserConfigFilePath(
			szConfigPath, 
			MAX_PATH,
			lpszFileName);
		
		if (bSuccess)
		{
			bSuccess = WriteINIInt(
				szConfigPath,
				lpszSectionHeader, 
				nNumber, 
				lpszKey, 
				nValue);
		}
		
		return bSuccess;
	}

	BOOL WriteINIInt(
		LPCTSTR lpszFileScalePath, 
		LPCTSTR lpszSectionHeader, 
		int		nNumber, 
		LPCTSTR lpszKey, 
		int		nValue)
	{
		BOOL bSuccess = FALSE;
		
		// �γɾ���·��
		TCHAR szAbsulutePath[MAX_PATH] = {0};
		
		bSuccess = Me()->ScalePath2AbsolutePath(
			lpszFileScalePath, 
			szAbsulutePath, 
			MAX_PATH
			);

		// ���SectionName
		TCHAR szSection[INIFILEMANIPULATE_SECTIONLENGTH] = {0};

		if(bSuccess)
		{
			bSuccess = Me()->SmartStringFormat(
				szSection, 
				INIFILEMANIPULATE_SECTIONLENGTH,
				FALSE,
				TEXT("%s%d"),
				lpszSectionHeader,
				nNumber
				);
		}
		
		// Value
		TCHAR szValue[INIFILEMANIPULATE_VALUELENGTH] = {0};

		if(bSuccess)
		{
			bSuccess = Me()->SmartStringFormat(
				szValue,
				INIFILEMANIPULATE_VALUELENGTH,
				FALSE,
				TEXT("%d"),
				nValue
				);
		}
		
		// д���ļ���
		if(bSuccess)
		{
			WritePrivateProfileString(
				szSection,
				lpszKey,
				szValue,
				szAbsulutePath
				);
		}

		return bSuccess;
	}

//////////////////////////////////////////////////////////////////////////

	BOOL WriteUserConfigString(
		LPCTSTR lpszFileName,
		LPCTSTR lpszSection, 
		LPCTSTR lpszKey, 
		LPCTSTR	lpszValue)
	{
		BOOL bSuccess = TRUE;
		TCHAR szConfigPath[MAX_PATH] = {0};
		
		bSuccess = GetUserConfigFilePath(
			szConfigPath,
			MAX_PATH, 
			lpszFileName);
		
		if (bSuccess)
		{
			bSuccess = 	WritePrivateProfileString(
				lpszSection,
				lpszKey,
				lpszValue,
				szConfigPath
				);
		}
		
		return bSuccess;
	}

//-----------------------------------------------------------------------------
//���ⲿ�ӿڡ���ini�ļ�д���ַ�����������SectionΪƴ��ֵ��
//-----------------------------------------------------------------------------
	BOOL WriteUserConfigString(
		LPCTSTR lpszFileName,
		LPCTSTR lpszSectionHeader, 
		int		nNumber, 
		LPCTSTR lpszKey, 
		LPCTSTR	lpszValue)
	{
		BOOL bSuccess = TRUE;
		TCHAR szConfigPath[MAX_PATH] = {0};
		
		bSuccess = GetUserConfigFilePath(
			szConfigPath, 
			MAX_PATH, 
			lpszFileName);
		
		if (bSuccess)
		{
			bSuccess = WriteINIString(
				szConfigPath,
				lpszSectionHeader, 
				nNumber, 
				lpszKey, 
				lpszValue);
		}
		
		return bSuccess;
	}
	
	BOOL WriteINIString(
		LPCTSTR lpszFileScalePath, 
		LPCTSTR lpszSection, 
		LPCTSTR lpszKey, 
		LPCTSTR	lpszValue)
	{
		BOOL bSuccess = FALSE;
		
		// �γɾ���·��
		TCHAR szAbsulutePath[MAX_PATH] = {0};
		
		bSuccess = Me()->ScalePath2AbsolutePath(
			lpszFileScalePath, 
			szAbsulutePath, 
			MAX_PATH
			);
		
		// д���ļ���
		if(bSuccess)
		{
			WritePrivateProfileString(
				lpszSection,
				lpszKey,
				lpszValue,
				szAbsulutePath
				);
		}
		
		return bSuccess;
	}

	BOOL WriteINIString(
		LPCTSTR lpszFileScalePath, 
		LPCTSTR lpszSectionHeader, 
		int		nNumber, 
		LPCTSTR lpszKey, 
		LPCTSTR	lpszValue)
	{
		BOOL bSuccess = FALSE;
		
		// �γɾ���·��
		TCHAR szAbsulutePath[MAX_PATH] = {0};
		
		bSuccess = Me()->ScalePath2AbsolutePath(
			lpszFileScalePath, 
			szAbsulutePath, 
			MAX_PATH
			);
		
		// ���SectionName
		TCHAR szSection[INIFILEMANIPULATE_SECTIONLENGTH] = {0};
		
		if(bSuccess)
		{
			bSuccess = Me()->SmartStringFormat(
				szSection, 
				INIFILEMANIPULATE_SECTIONLENGTH,
				FALSE,
				TEXT("%s%d"),
				lpszSectionHeader,
				nNumber
				);
		}
		
		// д���ļ���
		if(bSuccess)
		{
			WritePrivateProfileString(
				szSection,
				lpszKey,
				lpszValue,
				szAbsulutePath
				);
		}
		
		return bSuccess;
	}
	
//-----------------------------------------------------------------------------
//���ⲿ�ӿڡ���ini�ļ�д������, Key Ϊƴ��ֵ
//-----------------------------------------------------------------------------
	BOOL WriteUserConfigInt(
		LPCTSTR lpszFileName,
		LPCTSTR lpszSection, 
		LPCTSTR lpszKeyHeader, 
		int		nNumber, 
		int		nValue)
	{
		BOOL bSuccess = TRUE;
		TCHAR szConfigPath[MAX_PATH] = {0};
		
		bSuccess = GetUserConfigFilePath(
			szConfigPath, 
			MAX_PATH, 
			lpszFileName);
		
		if (bSuccess)
		{
			bSuccess = WriteINIInt(
				szConfigPath,
				lpszSection, 
				lpszKeyHeader, 
				nNumber, 
				nValue);
		}
		
		return bSuccess;
	}

	BOOL WriteINIInt(
		LPCTSTR lpszFileScalePath, 
		LPCTSTR lpszSection,
		LPCTSTR lpszKeyHeader,
		int     nNumber,
		int		nValue)
	{
		BOOL bSuccess = FALSE;
		
		// �γɾ���·��
		TCHAR szAbsulutePath[MAX_PATH] = {0};
		
		bSuccess = Me()->ScalePath2AbsolutePath(
			lpszFileScalePath, 
			szAbsulutePath, 
			MAX_PATH
			);
		
		// ���SectionName
		TCHAR szKey[INIFILEMANIPULATE_KEYLENGTH] = {0};
		
		if(bSuccess)
		{
			bSuccess = Me()->SmartStringFormat(
				szKey, 
				INIFILEMANIPULATE_KEYLENGTH,
				FALSE,
				TEXT("%s%d"),
				lpszKeyHeader,
				nNumber
				);
		}
		
		// Value
		TCHAR szValue[INIFILEMANIPULATE_VALUELENGTH] = {0};
		
		if(bSuccess)
		{
			bSuccess = Me()->SmartStringFormat(
				szValue,
				INIFILEMANIPULATE_VALUELENGTH,
				FALSE,
				TEXT("%d"),
				nValue
				);
		}
		
		// д���ļ���
		if(bSuccess)
		{
			WritePrivateProfileString(
				lpszSection,
				szKey,
				szValue,
				szAbsulutePath
				);
		}
		
		return bSuccess;
	}

//////////////////////////////////////////////////////////////////////////
	
	BOOL ReadUserConfigString(
		LPCTSTR lpszFileName,
		LPCTSTR lpszSection, 
		LPCTSTR lpszKey,
		LPCTSTR lpszDefault,
		LPTSTR  lpszReturnedString,
		DWORD   dwSize)
	{
		BOOL bSuccess = TRUE;
		TCHAR szConfigPath[MAX_PATH] = {0};
		
		bSuccess = GetUserConfigFilePath(
			szConfigPath,
			MAX_PATH, 
			lpszFileName);
		
		if (bSuccess)
		{
			bSuccess = GetPrivateProfileString(
				lpszSection,
				lpszKey,
				lpszDefault,
				lpszReturnedString,
				dwSize,
				szConfigPath) != 0;
		}
		
		return bSuccess;
	}

//////////////////////////////////////////////////////////////////////////
	BOOL ReadUserConfigUInt(
		LPCTSTR lpszFileName, 
		LPCTSTR lpszSection,
		LPCTSTR lpszKey, 
		UINT	unDefault,
		UINT&	runReturn)
	{
		BOOL bSuccess = TRUE;
		TCHAR szConfigPath[MAX_PATH] = {0};
		
		bSuccess = GetUserConfigFilePath(
			szConfigPath,
			MAX_PATH, 
			lpszFileName);
		
		if (bSuccess)
		{
			runReturn = GetPrivateProfileInt(
				lpszSection,
				lpszKey,
				unDefault,
				szConfigPath);
		}
	
		return bSuccess;
	}

//////////////////////////////////////////////////////////////////////////
	BOOL ReadUserConfigInt(
		LPCTSTR lpszFileName, 
		LPCTSTR lpszSection,
		LPCTSTR lpszKey, 
		int		nDefault,
		int&	rnReturn)
	{
		BOOL bSuccess = TRUE;
		TCHAR szConfigPath[MAX_PATH] = {0};
		
		bSuccess = GetUserConfigFilePath(
			szConfigPath,
			MAX_PATH, 
			lpszFileName);

		TCHAR szReturnedString[INIFILEMANIPULATE_VALUELENGTH] = {0};
		TCHAR szDefault[INIFILEMANIPULATE_VALUELENGTH] = {0};
		if (bSuccess)
		{
			bSuccess = Me()->SmartStringFormat(
				szDefault, 
				sizeof(szDefault)/sizeof(TCHAR),
				FALSE, 
				_T("%d"),
				nDefault);
		}

		if(bSuccess)
		{
			DWORD dwSize = sizeof(szReturnedString)/sizeof(TCHAR);
			bSuccess = GetPrivateProfileString(
				lpszSection,
				lpszKey,
				szDefault,
				szReturnedString,
				dwSize,
				szConfigPath) != 0;
		}

		if(bSuccess)
		{
			rnReturn = Me()->String2Int(szReturnedString);
		}
		else
		{
			rnReturn = nDefault;
		}
		
		return bSuccess;
	}

//////////////////////////////////////////////////////////////////////////

	BOOL ReadUserConfigInt( 
		LPCTSTR lpszFileName,
		LPCTSTR lpszSection,
		LPCTSTR lpszKeyHeader,
		int     nNumber,
		int		nDefault,
		int&	rnReturn)
	{
		BOOL bSuccess = TRUE;
		TCHAR szConfigPath[MAX_PATH] = {0};
		
		bSuccess = GetUserConfigFilePath(
			szConfigPath,
			MAX_PATH, 
			lpszFileName);
		
		if (bSuccess)
		{
			bSuccess = ReadINIInt(
				szConfigPath,
				lpszSection,
				lpszKeyHeader,
				nNumber,
				nDefault,
				rnReturn);
		}
		
		return bSuccess;
	}

	BOOL ReadGameConfigInt(short shGameID, LPCTSTR lpszSection,
		LPCTSTR lpszKeyHeader, int nSuffix, int nDefault, int& rnResult)
	{
		TCHAR szGameConfigFileNameAbs[MAX_PATH] = { 0 };
		BOOL bSucc = Me()->GetGamePath(shGameID, NULL, 0,
			szGameConfigFileNameAbs, MAX_PATH);
		if(bSucc)
		{
			bSucc = ReadINIInt(szGameConfigFileNameAbs, lpszSection,
				lpszKeyHeader, nSuffix, nDefault, rnReturn);
		}

		return bSucc;
	}

	BOOL ReadINIInt(
		LPCTSTR lpszFileScalePath, 
		LPCTSTR lpszSection,
		LPCTSTR lpszKeyHeader,
		int     nNumber,
		int		nDefault,
		int&	rnReturn)
	{
		BOOL bSuccess = FALSE;
		rnReturn = nDefault;
		
		// �γɾ���·��
		TCHAR szAbsulutePath[MAX_PATH] = {0};
		
		bSuccess = Me()->ScalePath2AbsolutePath(
			lpszFileScalePath, 
			szAbsulutePath, 
			MAX_PATH
			);
		
		// ���SectionName
		TCHAR szKey[INIFILEMANIPULATE_KEYLENGTH] = {0};
		
		if(bSuccess)
		{
			bSuccess = Me()->SmartStringFormat(
				szKey, 
				INIFILEMANIPULATE_KEYLENGTH,
				FALSE,
				TEXT("%s%d"),
				lpszKeyHeader,
				nNumber
				);
		}
		
		if (bSuccess)
		{
			rnReturn = GetPrivateProfileInt(
				lpszSection,
				szKey,
				nDefault,
				szAbsulutePath);
		}
		
		return bSuccess;
	}
	
	BOOL ReadINIInt(
		LPCTSTR lpszFileScalePath, 
		LPCTSTR lpszSectionHeader,
		int     nNumber,
		LPCTSTR lpszKey,
		int		nDefault,
		int&	rnReturn)
	{
		BOOL bSuccess = FALSE;
		rnReturn = nDefault;
		
		// �γɾ���·��
		TCHAR szAbsulutePath[MAX_PATH] = {0};
		
		bSuccess = Me()->ScalePath2AbsolutePath(
			lpszFileScalePath, 
			szAbsulutePath, 
			MAX_PATH
			);
		
		// ���SectionName
		TCHAR szSection[INIFILEMANIPULATE_KEYLENGTH] = {0};
		
		if(bSuccess)
		{
			bSuccess = Me()->SmartStringFormat(
				szSection, 
				INIFILEMANIPULATE_KEYLENGTH,
				FALSE,
				TEXT("%s%d"),
				lpszSectionHeader,
				nNumber
				);
		}
		
		if (bSuccess)
		{
			rnReturn = GetPrivateProfileInt(
				szSection,
				lpszKey,
				nDefault,
				szAbsulutePath);
		}
		
		return bSuccess;
	}

//-----------------------------------------------------------------------------
//������ӿڡ����������ļ��е�����, ��������ڻ��߶���ʧ���򷵻� FALSE
//-----------------------------------------------------------------------------

	BOOL ReadGameConfigInt(short shGameID, LPCTSTR lpszSection,
		LPCTSTR lpszKeyHeader, int& rnResult)
	{
		TCHAR szGameConfigFileNameAbs[MAX_PATH] = { 0 };
		BOOL bSucc = Me()->GetGamePath(shGameID, NULL, 0,
			szGameConfigFileNameAbs, MAX_PATH);
		if(bSucc)
		{
			bSucc = ReadINIInt(szGameConfigFileNameAbs,
				lpszSection, lpszKey, &rnReturn);
		}
		
		return bSucc;
	}

	BOOL ReadINIInt(LPCTSTR lpszFileScalePath, LPCTSTR lpszSection, 
			LPCTSTR lpszKey, int* pnValue)
	{
		BOOL bSuccess = FALSE;

		// �γɾ���·��
		TCHAR szAbsulutePath[MAX_PATH] = {0};

		bSuccess = Me()->ScalePath2AbsolutePath(
			lpszFileScalePath, 
			szAbsulutePath, 
			MAX_PATH
			);
		
		if(bSuccess && pnValue != NULL)
		{
			 UINT uNum1 = GetPrivateProfileInt(lpszSection, lpszKey,
				0, szAbsulutePath);

			 UINT uNum2 = GetPrivateProfileInt(lpszSection, lpszKey,
				1, szAbsulutePath);

			if(uNum1 == uNum2)
			{
				bSuccess = TRUE;

				*pnValue = uNum1;
			}
		}
		
		return bSuccess;
	}

//-----------------------------------------------------------------------------
//������ӿڡ������ַ��� Section Ϊƴ��ֵ
//-----------------------------------------------------------------------------

	BOOL ReadGameConfigString(short shGameID,
		LPCTSTR lpszSectionHeader, int nSectionSuffix, LPCTSTR lpszKey,
		LPTSTR lpszValue, int nValueCharCount)
	{
		TCHAR szGameConfigFileNameAbs[MAX_PATH] = { 0 };
		BOOL bSucc = Me()->GetGamePath(shGameID, NULL, 0,
			szGameConfigFileNameAbs, MAX_PATH);

		if(bSucc)
		{
			bSucc = ReadINIString(szGameConfigFileNameAbs, lpszSectionHeader,
				nSectionSuffix, lpszKey, lpszValue, nValueCharCount);
		}

		return bSucc;
	}

	BOOL ReadGameConfigString(short shGameID,
		LPCTSTR lpszSection, LPCTSTR lpszKeyHeader, int nKeySuffix,
		LPTSTR lpszValue, int nValueCharCount)
	{
		TCHAR szGameConfigFileNameAbs[MAX_PATH] = { 0 };
		BOOL bSucc = Me()->GetGamePath(shGameID, NULL, 0,
			szGameConfigFileNameAbs, MAX_PATH);
		
		if(bSucc)
		{
			bSucc = ReadINIString(szGameConfigFileNameAbs, lpszSection,
				lpszKeyHeader, nKeySuffix, lpszValue, nValueCharCount);
		}
		
		return bSucc;
	}

	BOOL ReadGameConfigString(short shGameID, LPCTSTR lpszSection,
		LPCTSTR lpszKey, LPTSTR lpszValue, int nValueCharCount)
	{
		TCHAR szGameConfigFileNameAbs[MAX_PATH] = { 0 };
		BOOL bSucc = Me()->GetGamePath(shGameID, (LPTSTR)NULL, 0,
			szGameConfigFileNameAbs, MAX_PATH);
		
		if(bSucc)
		{
			bSucc = ReadINIString(szGameConfigFileNameAbs, lpszSection,
				lpszKey, lpszValue, nValueCharCount);
		}
		
		return bSucc;
	}

	BOOL ReadINIString(LPCTSTR lpszFileScalePath, LPCTSTR lpszSectionHeader,
			int nNumber, LPCTSTR lpszKey, LPTSTR lpszValue, int nCharCount)
	{
		BOOL bSuccess = FALSE;

		bSuccess = (lpszFileScalePath != NULL && lpszSectionHeader != NULL &&
			lpszKey != NULL && lpszValue != NULL && nCharCount > 0);

		TCHAR szSection[INIFILEMANIPULATE_SECTIONLENGTH] = {0};

		if(bSuccess)
		{
			bSuccess = Me()->SmartStringFormat(szSection, 
				INIFILEMANIPULATE_SECTIONLENGTH, FALSE, TEXT("%s%d"),
				lpszSectionHeader, nNumber);

			if(bSuccess)
			{
				ReadINIString(lpszFileScalePath, szSection, 
					lpszKey, lpszValue, nCharCount);
			}
		}

		return bSuccess;
	}

	BOOL ReadINIString(LPCTSTR lpszFileScalePath, LPCTSTR lpszSection,
			LPCTSTR lpszKeyHeader, int nNumber, LPTSTR lpszValue, 
			int nCharCount)
	{
		BOOL bSuccess = FALSE;

		bSuccess = (lpszFileScalePath != NULL && lpszSection != NULL &&
			lpszKeyHeader != NULL && lpszValue != NULL && nCharCount > 0);

		TCHAR szKey[INIFILEMANIPULATE_KEYLENGTH] = {0};

		if(bSuccess)
		{
			bSuccess = Me()->SmartStringFormat(szKey, 
				INIFILEMANIPULATE_KEYLENGTH, FALSE, TEXT("%s%d"),
				lpszKeyHeader, nNumber);

			if(bSuccess)
			{
				ReadINIString(lpszFileScalePath, lpszSection, 
					szKey, lpszValue, nCharCount);
			}
		}

		return bSuccess;
	}

//-----------------------------------------------------------------------------
//������ӿڡ����������ļ��е�����, ��������ڻ��߶���ʧ���򷵻� FALSE
//-----------------------------------------------------------------------------

	BOOL ReadINIString(
		LPCTSTR		lpszFileScalePath,
		LPCTSTR		lpszSection,
		LPCTSTR		lpszKey,
		LPTSTR		lpszValue,
		int			nCharCount)
	{
		BOOL bSuccess = FALSE;

		bSuccess = (
			lpszFileScalePath != NULL &&
			lpszSection != NULL &&
			lpszKey != NULL &&
			lpszValue != NULL &&
			nCharCount > 0);

		// �γɾ���·��
		TCHAR szAbsulutePath[MAX_PATH] = {0};

		if(bSuccess)
		{
			bSuccess = Me()->ScalePath2AbsolutePath(
				lpszFileScalePath, 
				szAbsulutePath, 
				MAX_PATH
				);
		}
		
		if(bSuccess)
		{
			DWORD dwCount = GetPrivateProfileString(
				lpszSection,
				lpszKey,
				TEXT(""),
				lpszValue,
				nCharCount,
				szAbsulutePath
				);

			bSuccess = (dwCount > 0);
		}
		
		return bSuccess;
	}

private:

	inline TSubclass* Me()
	{
		return static_cast<TSubclass*> (this);
	}

	int GetMyUIN()
	{
		int nUIN = -1;
		ILogin* pLogin = NULL;

		if (Me()->GetGlobalInterface(INTERFACEID_LOGIN, &pLogin))
		{
			QQBASEUSERINFO stBaseInfo = {0};
			stBaseInfo.ushSize = sizeof(QQBASEUSERINFO);
			if (pLogin->GetBaseUserInfo(&stBaseInfo))
			{
				nUIN = stBaseInfo.unUIN;
			}
		}

		return nUIN;
	}

	BOOL GetUserConfigFilePath(
		LPTSTR lpszFilePath, 
		int nBufLen,
		LPCTSTR lpszFileName)
	{
		BOOL bSuccess = TRUE;

		if (m_nMyUIN == -1)
		{
			m_nMyUIN = GetMyUIN();
			bSuccess = m_nMyUIN != -1;
		}

		if (bSuccess)
		{
			bSuccess = Me()->SmartStringFormat(
				lpszFilePath,
				nBufLen,
				FALSE,
				_T("%s\\%s\\%d"),
				Me()->GetRootPath(),
				USERCONFIGFILEPATH,
				m_nMyUIN);
		}

		if (bSuccess)
		{
			bSuccess = Me()->SimpleCreateDirectory(lpszFilePath);
		}

		if (bSuccess)
		{
			bSuccess = Me()->SmartStringFormat(
				lpszFilePath,
				nBufLen,
				FALSE,
				_T("%s\\%s"),
				lpszFilePath,
				lpszFileName);
		}
		
		return bSuccess;
	}

private:
	int m_nMyUIN;

public:

//=============================================================================
//��ע�⡿Getxxxx �ĺ������ǿ���ָ��Ĭ��ֵ��, �ڶ�ȡʧ��ʱʹ��Ĭ��ֵ
//=============================================================================

	DWORD GetINIDWORD(LPCTSTR lpszFileScalePath, LPCTSTR lpszSection, 
		LPCTSTR lpszKey, DWORD dwDefaultValue)
	{
		DWORD dwResult = 0;

		int nValue = 0;

		if(ReadINIInt(lpszFileScalePath, lpszSection, lpszKey, &nValue))
		{
			dwResult = (DWORD)nValue;
		}
		else
		{
			dwResult = dwDefaultValue;
		}

		return dwResult;
	}

	DWORD GetINIDWORD(LPCTSTR lpszFileScalePath, LPCTSTR lpszSectionHeader,
		int nNumber, LPCTSTR lpszKey, DWORD dwDefaultValue)
	{
		TCHAR szSection[INIFILEMANIPULATE_SECTIONLENGTH] = {0};
		Me()->SmartStringFormat(szSection, INIFILEMANIPULATE_SECTIONLENGTH,
			FALSE, TEXT("%s%d"), lpszSectionHeader, nNumber);

		return GetINIDWORD(lpszFileScalePath, szSection, nNumber, 
			lpszKey, dwDefaultValue);
	}

	BOOL GetINIBOOL(LPCTSTR lpszFileScalePath, LPCTSTR lpszSection, 
			LPCTSTR lpszKey, BOOL bDefaultValue)
	{
		return (BOOL) GetINIDWORD(lpszFileScalePath, lpszSection,
			lpszKey, (DWORD)bDefaultValue);
	}
};

#endif