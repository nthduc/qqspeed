// FolderDialog.h: interface for the CFolderDialog class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FOLDERDIALOG_H__ED684DF5_03F1_4A2F_8B5A_8E9B7A3A2580__INCLUDED_)
#define AFX_FOLDERDIALOG_H__ED684DF5_03F1_4A2F_8B5A_8E9B7A3A2580__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CFolderDialog  
{
public:
	CString GetPathName();
	INT DoModal();
	CFolderDialog(HWND hWndParent = NULL);
	virtual ~CFolderDialog();

	BROWSEINFO		m_BrowseInfo;
	LPITEMIDLIST	m_pIDList;

protected:
	BOOL Reset();
};

#endif // !defined(AFX_FOLDERDIALOG_H__ED684DF5_03F1_4A2F_8B5A_8E9B7A3A2580__INCLUDED_)
