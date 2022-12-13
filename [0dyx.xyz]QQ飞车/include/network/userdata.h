#ifndef __USERDATA_H__
#define __USERDATA_H__

#include "vector"
#include "fileop.h"
#include "txtCtrlr.h"
#include "QQCrypt.h"
using namespace std;

#pragma  warning(disable:4018)

#define PWD_LEN		16
inline void _encode_(UINT Uin, LPCSTR src, LPSTR dst)
{
	const char szKey[] = { '0','1','2','3','4','5','6','7','8','9',
		'a','b','c','d','e','f','g','h','i','j','k','l','m','n', 'o','p','q','r','s','t','u','v','w','x','y','z',
		'A','B','C','D','E','F','G','H','I','J','K','L','M','N', 'O','P','Q','R','S','T','U','V','W','X','Y','Z','\0'};

	int nOffset = Uin % strlen(szKey);
	int nKeyLen = strlen(szKey);
	for ( int i = 0; i < strlen(src); i++ )
	{
		bool bFind = FALSE;
		for ( int j =0; j < strlen(szKey); j++ )
		{
			if ( src[i] == szKey[j] )
			{
				dst[i] = szKey[(j+nOffset) % nKeyLen];
				bFind = TRUE;
				break;
			}
		}
		if ( !bFind )
			dst[i] = src[i];
	}
	dst[strlen(src)] = 0;
}
inline void _decode_(UINT Uin, LPCSTR src, LPSTR dst)
{
	const char szKey[] = { '0','1','2','3','4','5','6','7','8','9',
		'a','b','c','d','e','f','g','h','i','j','k','l','m','n', 'o','p','q','r','s','t','u','v','w','x','y','z',
		'A','B','C','D','E','F','G','H','I','J','K','L','M','N', 'O','P','Q','R','S','T','U','V','W','X','Y','Z','\0'};

	int nOffset = Uin % strlen(szKey);
	int nKeyLen = strlen(szKey);
	for ( int i = 0; i < strlen(src); i++ )
	{
		for ( int j =0; j < strlen(szKey); j++ )
		{
			if ( src[i] == szKey[j] )
			{
				dst[i] = szKey[(j-nOffset+nKeyLen) % nKeyLen];
				break;
			}
		}
	}
	dst[strlen(src)] = 0;
}

#define USERDATA_FILE	"users.dat"
/*
[UIN]		[Pwd]
48071210	
--			--
*/

typedef struct tagUserData
{
	tagUserData(UINT uin, LPCSTR pwd)
	{
		Uin = uin;
		if ( pwd )
			strcpy(Pwd, pwd);
		else
			memset(Pwd, 0, 40);
	}
	UINT	Uin;
	CHAR	Pwd[40];
}UserData;
typedef vector<UserData>	UserDataContainer;

class CUserDataProfile
{
public:
	CUserDataProfile()
	{
		char szProfile[MAX_PATH] = "\0";
		_fullpath(szProfile, USERDATA_FILE, MAX_PATH);
		CtxtCtrlr ctrlr;
		if ( ctrlr.Open(szProfile) )
		{
			while (1)
			{
				if ( !ctrlr.Next() )
					break;
				CtxtRecordset* pRecordset = ctrlr.GetRecordset();
				if ( pRecordset->GetFieldCount() < 1 )
					continue;
				UserData aData(0, 0);
				aData.Uin = atoi(pRecordset->GetField(0));
				if ( pRecordset->GetFieldCount() > 1 )
				{
					strcpy(aData.Pwd, pRecordset->GetField(1));
					
				}
				m_Users.push_back(aData);
			}
		}
	}
	~CUserDataProfile()
	{
		char szProfile[MAX_PATH] = "\0";
		_fullpath(szProfile, USERDATA_FILE, MAX_PATH);
		FILE* fp = fopen(szProfile, "w+");
		if ( !fp )
			return;
		for ( UserDataContainer::const_iterator it = m_Users.begin(); it != m_Users.end(); it++ )
		{
			if ( strlen(it->Pwd) < 1 )
				fprintf(fp, "%d\n", it->Uin);
			else
				fprintf(fp, "%d\t%s\n", it->Uin, it->Pwd);
		}
		fclose(fp);
	}
	int GetUserCount()
	{
		return m_Users.size();
	}

	UserData GetUserData(int index)
	{
		UserData ud = m_Users[index];
		_decode_(ud.Uin, ud.Pwd, ud.Pwd);
		return ud;
	}
	
	UserData FindUserData(UINT uin)
	{
		for ( UserDataContainer::iterator it = m_Users.begin(); it != m_Users.end(); it++ )
		{
			if ( it->Uin == uin )
			{
				UserData ud = *it;
				_decode_(ud.Uin, ud.Pwd, ud.Pwd);
				return ud;
			}
		}
		return UserData(0, 0);
	}

	void SetUserData(UserData* Data)
	{
		for ( UserDataContainer::iterator it = m_Users.begin(); it != m_Users.end(); it++ )
		{
			if ( it->Uin == Data->Uin )
			{
				m_Users.erase(it);
				break;
			}
		}
		char szPwd[40] = "\0";
		char szTmp[40] = "\0";
		strncpy(szTmp, Data->Pwd, PWD_LEN);
		_encode_(Data->Uin, szTmp, szPwd);
		m_Users.push_back(UserData(Data->Uin, szPwd));
	}

private:
	UserDataContainer	m_Users;
};

#endif