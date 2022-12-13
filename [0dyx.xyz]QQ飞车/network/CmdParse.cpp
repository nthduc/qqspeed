#include "stdafx.h"
#include "CmdParse.h"

CString CCmdParse::SimpleDecrypt(const CString strEn)
{
	CString s = HexStrToData(strEn);
	
	CString strPlain;
	int nLen = s.GetLength();
	BOOL bSuccess = oi_symmetry_decrypt2((const BYTE*)(const char*)s, nLen,
		(const BYTE*)"1234567890abcdef",
		(BYTE*)strPlain.GetBuffer(nLen), &nLen);
	strPlain.ReleaseBuffer(nLen);
	
	if (!bSuccess) strPlain = "";
	
	return strPlain;
}

CString CCmdParse::HexStrToData(const CString strHex)
{
	const BYTE *p = (const BYTE*)(const char*)strHex;
	CString strResult;
	int nLen = strHex.GetLength() / 2;
	for(int i=0; i<nLen; i++)
	{
		CString s((const char*)p, 2);
		BYTE chValue;
		sscanf(s, "%x", &chValue);
		CString strTemp = (char*)&chValue;		
		strResult += strTemp;
		p+=2;
	}
	return strResult;
};

CString CCmdParse::GetPlainCmdLine(LPCTSTR szCmdLine)
{
	CString strCmdLine = szCmdLine;

	strCmdLine.MakeLower();

	int nPos = strCmdLine.Find(".exe");
	if (-1 == nPos)
	{
		return "";
	}
	nPos = strCmdLine.Find(" ", nPos) + 1; 
	if (-1 == nPos)
	{
		return "";
	}

	CString strSend = strCmdLine.Mid(nPos);
	CString strDecrypt = SimpleDecrypt(strSend);  // 得到解码部分
	return strDecrypt;
}

BOOL CCmdParse::Base64Decode2(LPCSTR szSrc, int nSrcLen, BYTE *pbDest, int *pnDestLen)
{
	// walk the source buffer
	// each four character sequence is converted to 3 bytes
	// CRLFs and =, and any characters not in the encoding table
	// are skiped
	
	if (!szSrc || !pbDest || !pnDestLen)
	{
		return FALSE;
	}
	
	LPCSTR szSrcEnd = szSrc + nSrcLen;
	int nWritten = 0;
	while (szSrc < szSrcEnd)
	{
		DWORD dwCurr = 0;
		int i;
		int nBits = 0;
		for (i=0; i<4; i++)
		{
			if (szSrc >= szSrcEnd)
				break;
			int nCh = DecodeBase64Char2(*szSrc);
			szSrc++;
			if (nCh == -1)
			{
				// skip this char
				i--;
				continue;
			}
			dwCurr <<= 6;
			dwCurr |= nCh;
			nBits += 6;
		}
		// dwCurr has the 3 bytes to write to the output buffer
		// left to right
		dwCurr <<= 24-nBits;
		for (i=0; i<nBits/8; i++)
		{
			*pbDest = (BYTE) ((dwCurr & 0x00ff0000) >> 16);
			dwCurr <<= 8;
			pbDest++;
			nWritten++;
		}
	}
	
	*pnDestLen = nWritten;
	return TRUE;
};

CString CCmdParse::MD5(const CString strData)
{
	CString s;
	Md5HashBuffer( (BYTE*)s.GetBuffer(16), (const BYTE *)(const char*)strData, strData.GetLength());
	s.ReleaseBuffer(16);
	return s;
}

int CCmdParse::DecodeBase64Char2(unsigned int ch)
{
	// returns -1 if the character is invalid
	// or should be skipped
	// otherwise, returns the 6-bit code for the character
	// from the encoding table
	if (ch >= 'A' && ch <= 'Z')
		return ch - 'A' + 0;	// 0 range starts at 'A'
	if (ch >= 'a' && ch <= 'z')
		return ch - 'a' + 26;	// 26 range starts at 'a'
	if (ch >= '0' && ch <= '9')
		return ch - '0' + 52;	// 52 range starts at '0'
	if (ch == '+')
		return 62;
	if (ch == '/')
		return 63;
	return -1;
}

CString CCmdParse::GetCmdStr(LPCTSTR szCmdLine)
{
	int nPos, nPos2;
	CString strCmdLine = szCmdLine;

	CString strDecrypt = GetPlainCmdLine(szCmdLine);

	nPos = strDecrypt.Find('/');
	nPos2 = strDecrypt.Find(' ', nPos);

	return 	strDecrypt.Mid(nPos, nPos2 - nPos);
}

BOOL CCmdParse::SetCmdLine(LPCTSTR szCmdLine)
{
	m_strCmdLine = szCmdLine;
	return TRUE;
};
	
