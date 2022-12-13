// CmdParse_acceptinvote.cpp: implementation of the CCmdParse_acceptinvote class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CmdParse_acceptinvote.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCmdParse_acceptinvote::CCmdParse_acceptinvote()
{

}

CCmdParse_acceptinvote::~CCmdParse_acceptinvote()
{

}

BOOL CCmdParse_acceptinvote::SetCmdLine(LPCTSTR szCmdLine)
{
	int nPos, nPos2;
	CString strCmdLine = szCmdLine;
	CString strDecrypt, strSend, strDecryptOld;

	strDecrypt = GetPlainCmdLine(szCmdLine);

	nPos = strDecrypt.Find('/');
	nPos2 = strDecrypt.Find(' ', nPos);
	CString strQQCmd = strDecrypt.Mid(nPos, nPos2 - nPos);

	if (0 == strQQCmd.Compare("/ACCEPTINVOTE"))
	{
		nPos = strDecrypt.Find("QQUIN:");
		CString strRight;
		strRight = strDecrypt.Mid(nPos + 6);
		strRight.TrimLeft();
		nPos = strRight.Find(' ');
		CString strUin = strRight.Left(nPos);
		m_uUIN = atoi(strUin);  // 获得UIN

		nPos =strDecrypt.Find("PWDHASH2:");
		if (nPos > 0)
		{	
			m_nHashTime = 2;
			strRight = strDecrypt.Mid(nPos + 9);
		}
		else 
		{
			nPos = strDecrypt.Find("PWDHASH:");
			if (nPos > 0)
			{
				m_nHashTime = 1;
				strRight = strDecrypt.Mid(nPos + 8);
			}
			else
			{
				ASSERT(FALSE);
				return FALSE;
			}
		}

		nPos = strRight.Find(' ');
		CString strHashPsw = strRight.Left(nPos);

		BYTE byHash[16];
		int ihashLen;
		CString strPass;
		Base64Decode2(strHashPsw, strHashPsw.GetLength(), byHash, &ihashLen);
		memcpy(strPass.GetBuffer(16), byHash, 16);
		memcpy(m_byHash, byHash, 16);
		strPass.ReleaseBuffer();
		if (m_nHashTime == 1)
		{
			strPass = MD5(strPass); // 如果只有一次Hash,再做一次Hash
		}

		m_sPWHash = strPass;

		nPos = strDecrypt.Find("SERVERID:");
		strRight = strDecrypt.Mid(nPos + 9);
		strRight.TrimLeft();
		nPos = strRight.Find(' ');
		CString strServerID = strRight.Left(nPos);
		m_shServerID = atoi(strServerID);

		// qq 的Room 就是qqt 的section
		nPos = strDecrypt.Find("ROOMID:");
		strRight = strDecrypt.Mid(nPos + 7);
		strRight.TrimLeft();
		nPos = strRight.Find(' ');
		CString strRoomID = strRight.Left(nPos);
		m_shSectionID = atoi(strRoomID);

		nPos = strDecrypt.Find("TABLEID:");
		strRight = strDecrypt.Mid(nPos + 8);
		strRight.TrimLeft();
		nPos = strRight.Find(' ');
		CString strTableID = strRight.Left(nPos);
		m_shRoomID = atoi(strTableID);

		nPos = strDecrypt.Find("WNDHANDLE:");
		strRight = strDecrypt.Mid(nPos + 10);
		nPos = strRight.Find(' ');
		// 可能是最后一个参数
		CString strWndHandle = (nPos == -1 ? strRight : strRight.Left(nPos));
		m_hWndHandle = (DWORD)atoi(strWndHandle);
		return TRUE;
	}

	return FALSE;
}
