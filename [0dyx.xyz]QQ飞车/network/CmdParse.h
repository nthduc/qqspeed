#ifndef _CMDPARSE_HEADER_
#define _CMDPARSE_HEADER_

#include "network/QQCrypt.h"

//#############################################################################
// �� �� : CCmdParse ���ڽ�����QQGame����QQBattleZoneʱ�������в���
// �� ʷ : 2004-08-16 Living 
// �� �� : 1 SetExeName(".exe")
//         2 SetCmdLine(������)
//         3 GetPWHashTime()					��ȡ���뾭��MD5 Hash�Ĵ���
//         4 GetUIN()							��ȡUIN
//         5 GetPWHash()						����Hash�������
//#############################################################################

class CCmdParse
{
public:

	CCmdParse() : m_nHashTime(0), m_uUIN(0)
	{
	}
	
	UINT GetUIN()
	{
		return m_uUIN;
	};
	
	CString GetPWHash()
	{
		ASSERT(m_sPWHash.GetLength() == 16);
		return m_sPWHash;
	};

	BOOL GetHash(BYTE* byHash)
	{
		memcpy(byHash , m_byHash , 16);
		return TRUE;
	}
	
	int GetPWHashTime()
	{
		return m_nHashTime;
	};

	virtual BOOL SetCmdLine(LPCTSTR szCmdLine);
	
public: 
	static CString SimpleDecrypt(const CString strEn);
	static CString HexStrToData(const CString strHex);
	static BOOL Base64Decode2(LPCSTR szSrc, int nSrcLen, BYTE *pbDest, int *pnDestLen);
	static CString MD5(const CString strData);
	static int DecodeBase64Char2(unsigned int ch);
	static CString GetCmdStr(LPCTSTR szCmdLine);
	static CString GetPlainCmdLine(LPCTSTR szCmdLine);

protected:

	int		m_nHashTime;		// ���뾭�� MD5 hash �Ĵ���
	UINT	m_uUIN;
	CString m_sPWHash;
	BYTE	m_byHash[16];
	CString m_strCmdLine;
};

#endif //_CMDPARSE_HEADER_