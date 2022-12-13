// txtCtrlr.h: interface for the CtxtCtrlr class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TXTCTRLR_H__A60B15A6_49C1_48E5_ACBC_EFB1E077A7BF__INCLUDED_)
#define AFX_TXTCTRLR_H__A60B15A6_49C1_48E5_ACBC_EFB1E077A7BF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "txtRecordset.h"

#define MAX_FIELD	255

class CtxtCtrlr  
{
public:
	BOOL First();
	CtxtRecordset* GetRecordset();
	BOOL Prev();
	BOOL Next();
	BOOL Seek(int index);
	BOOL Open(LPCSTR file);
	BOOL Close();
	CtxtCtrlr();
	virtual ~CtxtCtrlr();

private:
//	HFILE	m_hFileHandle;
//	HANDLE	m_hMappingObject;
	BYTE*	m_pFileCache;

	OFSTRUCT m_of;	
	UINT	m_nCursorPos;
	UINT	m_nCharPos;
	LONG	m_nFileLen;
	CtxtRecordset	m_Recordset;
};

enum
{
	eStart,
	eID,
	eString,
	eEndString,
	eSwitch,
	eFieldEnd,
	eEnd,
};

#endif // !defined(AFX_TXTCTRLR_H__A60B15A6_49C1_48E5_ACBC_EFB1E077A7BF__INCLUDED_)
