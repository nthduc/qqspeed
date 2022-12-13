#include "stdafx.h"
#include "client.h"
#include "stdio.h"
#include "fileop.h"
#include "txtCtrlr.h"
#include "network/QQCrypt.h"

char g_szHashString[33];

void Buf2HexString(BYTE *byBuf, int size, char *szOutBuf)
{
	char s[3];
	for (int i = 0; i < size; ++i)
	{
		_snprintf(s, 3, "%X%X", byBuf[i] >> 4, byBuf[i] & 0x0F);
		strcpy(&szOutBuf[i * 2], s);
	}
}

BOOL GenHashString()
{
	ZeroMemory(g_szHashString, sizeof(g_szHashString));
	
	char strbuf[50 * 33];
	unsigned char hashbuf[16];
	char szFullName[MAX_PATH] = "\0";

	CtxtCtrlr txtCtrl;
	if ( !txtCtrl.Open("ck.txt") )
		return FALSE;
	int i = 0;
	while(txtCtrl.Next())
	{
		CtxtRecordset* rs = txtCtrl.GetRecordset();
		if ( rs->GetFieldCount() > 0 )
		{
			LPCSTR pszName = rs->GetField(0);
			_fullpath(szFullName, pszName, MAX_PATH);

			OFSTRUCT of;
			HFILE hFileHandle = OpenFile(szFullName, &of, OF_READ | OF_SHARE_DENY_WRITE);
			if ( hFileHandle == HFILE_ERROR )
				return FALSE;
			HANDLE hMappingObject = CreateFileMapping((HANDLE)hFileHandle, NULL, PAGE_READONLY, 0, 0, NULL);
			if ( !hMappingObject )
			{
				_lclose(hFileHandle);
				hFileHandle = HFILE_ERROR;
				return FALSE;
			}
			BYTE* pFileCache = (BYTE*)MapViewOfFile(hMappingObject, FILE_MAP_READ, 0, 0, 0);

			Md5HashBuffer(hashbuf, pFileCache, i * 32);
			Buf2HexString(hashbuf, 16, strbuf + i * 32);

			UnmapViewOfFile(pFileCache);
			i++;
		}
	};

	Md5HashBuffer(hashbuf, (unsigned char*)strbuf, i * 32);
	Buf2HexString(hashbuf, 16, g_szHashString);
	g_szHashString[32] = '\0';

	return true;
}

