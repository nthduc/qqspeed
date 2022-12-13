// txtCtrlr.cpp: implementation of the CtxtCtrlr class.
//
//////////////////////////////////////////////////////////////////////

#include "txtCtrlr.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CtxtCtrlr::CtxtCtrlr():
//m_hFileHandle(HFILE_ERROR),
//m_hMappingObject(0),
m_pFileCache(NULL),
m_nCursorPos(0),
m_nCharPos(0),
m_nFileLen(0)
{

}

CtxtCtrlr::~CtxtCtrlr()
{
	Close();
}

BOOL CtxtCtrlr::Close()
{
	m_nCursorPos = 0;
	m_nCharPos = 0;
	m_nFileLen = 0;

	if ( m_pFileCache )
	{
		delete[] m_pFileCache;
		m_pFileCache = NULL;
	}
	/*
	if ( m_hMappingObject )
	{
		UnmapViewOfFile(m_pFileCache);
		m_pFileCache = NULL;
		CloseHandle(m_hMappingObject);
		m_hMappingObject = 0;
	}
	if ( m_hFileHandle != HFILE_ERROR )
	{
		_lclose(m_hFileHandle);
		m_hFileHandle = HFILE_ERROR;
	}
	*/
	return TRUE;
}

BOOL CtxtCtrlr::Open(LPCSTR file)
{
	Close();

	FILE* fp = fopen(file, "r");
	if ( !fp )
		return FALSE;
	fseek(fp, 0, SEEK_END);
	long nPos = ftell(fp);
	m_nFileLen = nPos;
	if ( nPos < 0 )
	{
		fclose(fp);
		return FALSE;
	}
	fseek(fp, 0, SEEK_SET);
	m_pFileCache = new BYTE[nPos+1];
	memset(m_pFileCache, 0, nPos+1);

	const int nBlock = 1024;
	int nOffset = 0;
	while(1)
	{
		int nRead = fread(m_pFileCache+nOffset,1, nBlock, fp);
		if ( nRead != nBlock )
			break;
		nOffset += nRead;
	}
	fclose(fp);

	/*
	m_hFileHandle = OpenFile(file, &m_of, OF_READ | OF_SHARE_DENY_WRITE);
	if ( m_hFileHandle == HFILE_ERROR )
		return FALSE;
	m_hMappingObject = CreateFileMapping((HANDLE)m_hFileHandle, NULL, PAGE_READONLY, 0, 0, NULL);
	if ( !m_hMappingObject )
	{
		_lclose(m_hFileHandle);
		m_hFileHandle = HFILE_ERROR;
		return FALSE;
	}
	m_pFileCache = (BYTE*)MapViewOfFile(m_hMappingObject, FILE_MAP_READ, 0, 0, 0);
	if ( !m_pFileCache )
	{
		Close();
		return FALSE;
	}
	*/
	return TRUE;
}

BOOL CtxtCtrlr::Seek(int index)
{
	if ( !m_pFileCache )
		return FALSE;
	return TRUE;
}

BOOL CtxtCtrlr::Next()
{
	m_Recordset.Reset();
	int nFieldIndex = 0;
	char szField[MAX_FIELD] = "\0";
	UINT nIndex = 0;
	int nState = eStart;
	UINT nPos = 0;

	if ( m_nCharPos >= m_nFileLen )
		return FALSE;
	BYTE* pStartCache = &m_pFileCache[m_nCharPos];
	if ( pStartCache[0] == 0 )
		return FALSE;
	while( nState != eEnd)
	{
		BOOL bSave = TRUE;
		BYTE c = pStartCache[nPos++];
		switch(c)
		{
		case ' ':
		case '\t':
		case ';':
			if ( nState == eStart )
			{
				bSave = FALSE;
			}
			else if ( nState != eString )
			{
				nState = eFieldEnd;
				bSave = FALSE;
			}
			break;
		case 0x0d:
			bSave = FALSE;
			break;
		case 0x0a:
//			if ( nState != eStart )
				nState = eEnd;
			bSave = FALSE;
			break;
		case 0x0:
			nState = eEnd;
			bSave = FALSE;
			break;
		case '\"':
			if ( nState != eString )
				nState = eString;
			else
				nState = eEndString;
			bSave = FALSE;
			break;
		default:
			nState = eID;
			break;
		}
		if ( bSave )
		{
			szField[nIndex++] = c;
		}
		switch(nState)
		{
		case eFieldEnd:
			m_Recordset.SetField(nFieldIndex++, szField);
			memset(szField, 0, MAX_FIELD);
			nIndex = 0;
			nState = eStart;
			break;
		case eEnd:
			if ( strlen(szField) )
				m_Recordset.SetField(nFieldIndex++, szField);
			break;		
		}
	}
	m_nCharPos += nPos;
	m_nCursorPos++;
	return TRUE;
}

BOOL CtxtCtrlr::Prev()
{
	return FALSE;
}

BOOL CtxtCtrlr::First()
{
	m_nCursorPos = 0;
	m_nCharPos = 0;
	return Next();
}

CtxtRecordset* CtxtCtrlr::GetRecordset()
{
	return &m_Recordset;
}

