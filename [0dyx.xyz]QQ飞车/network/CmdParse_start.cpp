// CmdParse_start.cpp: implementation of the CCmdParse_start class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CmdParse_start.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCmdParse_start::CCmdParse_start()
{

}

CCmdParse_start::~CCmdParse_start()
{

}

BOOL CCmdParse_start::SetCmdLine(LPCTSTR szCmdLine)
{
	OutputDebugString(szCmdLine);
	CString sInfo;
	CString strTemp = szCmdLine;
	CString strTest;
	int t1(0),t2(0),tLeng(0),Leng(0);
	CString strTime;
	CString strPass;
	CString strUin;
	CString strDeCryLen;
	CString strPassEnd;
	int tMd5(0);
	
	int ntime = ::GetTickCount();
	CString strCurTime;
	strCurTime.Format("%ld",ntime);

	strTemp.MakeLower();
	
	int L1=strTemp.Find(".exe");
	CString strDecrypt, strSend, strDecryptOld;
	Leng = strTemp.GetLength();
	
	if(L1<0||9>(Leng-L1))
	{
		goto model;
	}
	else
	{
		//memcpy(strSend.GetBuffer(Leng-L1-9),strTemp.Right(Leng-L1-9),(Leng-L1-9));
		//strSend.ReleaseBuffer();
		int nPos = L1 + 4;
		nPos = strTemp.Find(" ", nPos) + 1;
		strSend = strTemp.Mid(nPos);
		OutputDebugString(strSend);
	}

	strDecrypt = SimpleDecrypt(strSend);

	t1=strDecrypt.Find("QQUIN"); 
	if(t1<0)
	{
		goto model;
	}
	
	else
	{
		t2=strDecrypt.Find("PWDHASH", t1);
		tLeng = strDecrypt.GetLength();
		memcpy(strUin.GetBuffer(t2-t1-7),strDecrypt.Right(tLeng-t1-6),(t2-t1-7));
		strUin.ReleaseBuffer();
		m_uUIN = (UINT)atol(strUin); // QQ 号码
	}

	tMd5=strDecrypt.Find("PWDHASH2");
	
	if(tMd5<0)
	{
		t1=strDecrypt.Find("PWDHASH"); 
		if(t1<0)
		{
			goto model;
		}
		else
		{
			BYTE byHash[16];
			int ihashLen;
			t2=strDecrypt.Find("TIME", t1);
			memcpy(strPassEnd.GetBuffer(t2-t1-9),strDecrypt.Right(tLeng-t1-8),(t2-t1-9));
			strPassEnd.ReleaseBuffer();
			Base64Decode2(strPassEnd , strPassEnd.GetLength() , byHash , &ihashLen);
			memcpy(strPass.GetBuffer(16),byHash,16);
			memcpy(m_byHash , byHash , 16);
			strPass.ReleaseBuffer();
			strPass = MD5(strPass); // 如果只有一次Hash,再做一次Hash
			m_nHashTime = 1;
		}
	}
	else
	{
		BYTE byHash[16];
		int ihashLen;
		t2=strDecrypt.Find("TIME", t1);
		memcpy(strPassEnd.GetBuffer(t2-tMd5-10),strDecrypt.Right(tLeng-tMd5-9),(t2-tMd5-10));
		strPassEnd.ReleaseBuffer();
		Base64Decode2(strPassEnd , strPassEnd.GetLength() , byHash , &ihashLen);
		memcpy(strPass.GetBuffer(16),byHash,16);
		memcpy(m_byHash , byHash , 16);
		strPass.ReleaseBuffer();
		m_nHashTime = 2; // 类型为2次Hash
	}

	m_sPWHash = strPass;

	t1=strDecrypt.Find("TIME"); 
	memcpy(strTime.GetBuffer(tLeng-t1-5),strDecrypt.Right(tLeng-t1-5),(tLeng-t1-5));
	strTime.ReleaseBuffer();
	
	
	int ntime1;
	sscanf( strTime, "%d", &ntime1 );
	if(ntime-ntime1>2500)
	{
		//			return false;
	}
	
	sInfo.Format("Command Line : UIN = %s Password = %s\n", strUin, strPass);
	OutputDebugString(sInfo);
	return true;
	
model:
	return false;	
}
