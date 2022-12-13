// ZUBaseFile.h: interface for the CZUBaseFile class.
//
// Copyright (C) 2000 Tadeusz Dracz
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ZUBASEFILE_H__AEA985C0_D813_11D3_B7C7_8E2FB86D3347__INCLUDED_)
#define AFX_ZUBASEFILE_H__AEA985C0_D813_11D3_B7C7_8E2FB86D3347__INCLUDED_

#include "zlib.h"
#include "ZipException.h"
class CPtrList;

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


// base class for CUnzipFile and CZipFile
class CZUBaseFile  
{
public:
	CZUBaseFile();
	virtual ~CZUBaseFile();
	// specify whether to control memory allocation and freeing by zlib library
	// strongly suggested to set it to true (default)
	bool m_bDetectZlibMemoryLeaks;

protected:
	CPtrList m_list;
	void CheckForError(int err);
	void ThrowError(int err);
	static void myfree(void* opaque, void* address);
	static void* myalloc(void* opaque, UINT items, UINT size);
	bool IsClosed();
	CFile* m_pFile;
	bool m_bZipFile;
};

#endif // !defined(AFX_ZUBASEFILE_H__AEA985C0_D813_11D3_B7C7_8E2FB86D3347__INCLUDED_)
