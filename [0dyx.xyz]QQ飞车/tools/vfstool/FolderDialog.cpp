// FolderDialog.cpp: implementation of the CFolderDialog class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FolderDialog.h"
#include "objbase.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFolderDialog::CFolderDialog(HWND hWndParent)
{
	CoInitializeEx(0, COINIT_APARTMENTTHREADED);
	memset(&m_BrowseInfo, 0, sizeof(m_BrowseInfo));
	m_BrowseInfo.hwndOwner = hWndParent;
	Reset();
}

CFolderDialog::~CFolderDialog()
{
	Reset();
	CoUninitialize();
}

BOOL CFolderDialog::Reset()
{
	if ( m_pIDList )
	{
		IMalloc* pMalloc = NULL;
		HRESULT hr = SHGetMalloc(&pMalloc);
		if ( hr == S_OK )
		{
			pMalloc->Free(m_pIDList);
			pMalloc->Release();
		}
		m_pIDList = NULL;
	}
	return TRUE;
}

INT CFolderDialog::DoModal()
{
	Reset();
	m_pIDList = SHBrowseForFolder(&m_BrowseInfo);
	if ( !m_pIDList )
		return IDCANCEL;
	return IDOK;
}

CString CFolderDialog::GetPathName()
{
	CString strPathName;
	if ( !m_pIDList )
		return strPathName;
	TCHAR lpszPathName[MAX_PATH];
	if ( SHGetPathFromIDList(m_pIDList, lpszPathName) )
		strPathName = lpszPathName;
	return strPathName;
}
