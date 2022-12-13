// vfstoolDoc.cpp : implementation of the CVfstoolDoc class
//

#include "stdafx.h"
#include "vfstool.h"
#include "vfstoolDoc.h"
#include "vfstoolview.h"
#include "fileop.h"
#include "FolderDialog.h"
#include "VfsHelper.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CVfstoolApp theApp;
/////////////////////////////////////////////////////////////////////////////
// CVfstoolDoc

IMPLEMENT_DYNCREATE(CVfstoolDoc, CDocument)

BEGIN_MESSAGE_MAP(CVfstoolDoc, CDocument)
	//{{AFX_MSG_MAP(CVfstoolDoc)
	ON_COMMAND(ID_FILE_SAVE_AS, OnFileSaveAs)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CVfstoolDoc construction/destruction

CVfstoolDoc::CVfstoolDoc():
m_pFileSysMgr(NULL),
m_hRoot(VFS_INVALID_HANDLE_VALUE)
{
	// TODO: add one-time construction code here
}

CVfstoolDoc::~CVfstoolDoc()
{
}


/////////////////////////////////////////////////////////////////////////////
// CVfstoolDoc serialization

void CVfstoolDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CVfstoolDoc diagnostics

#ifdef _DEBUG
void CVfstoolDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CVfstoolDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CVfstoolDoc commands

BOOL CVfstoolDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
//	if (!CDocument::OnOpenDocument(lpszPathName))
//		return FALSE;
	
	// TODO: Add your specialized creation code here
	if ( !m_pFileSysMgr )
	{
		m_pFileSysMgr = ICreateFileSysMgr();
	}
	if ( !m_pFileSysMgr )
	{
		LOGLINE("ICreateFileSysMgr Failed");
		return FALSE;
	}
	if ( !m_pFileSysMgr->Init() )
	{
		LOGLINE("m_pFileSysMgr->Init() Failed");
		return FALSE;
	}
	VROOT_h hRoot = m_pFileSysMgr->OpenRoot(lpszPathName);
	m_hRoot = hRoot;
	if ( hRoot == VFS_INVALID_HANDLE_VALUE )
	{
		LOGLINE("MountRoot Failed");
		return FALSE;
	}

	GetActiveView()->Clear();
	FIND_h hFind = m_pFileSysMgr->FindFirstFile(hRoot, "");
	if ( hFind != VFS_INVALID_HANDLE_VALUE )
	{
		while(1)
		{
			LPCSTR lpszFileName = m_pFileSysMgr->GetFindPathName(hFind);
			GetActiveView()->AddItem(lpszFileName);

			if ( !m_pFileSysMgr->FindNextFile(hFind) )
				break;
		}
	}
	
	return TRUE;
}

CVfstoolView* CVfstoolDoc::GetActiveView()
{
	POSITION pos = GetFirstViewPosition();
	CView* pView = GetNextView(pos);
	return reinterpret_cast<CVfstoolView*>(pView);
}

void CVfstoolDoc::OnFileSaveAs() 
{
	CFolderDialog fd(GetActiveView()->m_hWnd);
	if ( IDOK == fd.DoModal() )
	{
		Extract(fd.GetPathName());
	}
}

BOOL CVfstoolDoc::Extract(LPCSTR lpszPath)
{
	// TODO: Add your command handler code here
	
	if ( !m_pFileSysMgr )
		return FALSE;
	
	if ( !FileExists(lpszPath) )
		CreateDirectory(lpszPath, NULL);
	VROOT_h hRoot = m_pFileSysMgr->OpenRoot(lpszPath, VFS_OPENROOT_MODE_WRITE_ALWAYS);
	if ( hRoot == VFS_INVALID_HANDLE_VALUE )
	{
		LogText("Open Root(%d) Failed\n", lpszPath);
		return FALSE;
	}
	
	FIND_h hFind = m_pFileSysMgr->FindFirstFile(m_hRoot, "");
	if ( hFind != VFS_INVALID_HANDLE_VALUE )
	{
		while(1)
		{
			LPCSTR lpszFileName = m_pFileSysMgr->GetFindPathName(hFind);
			int nRet = m_pFileSysMgr->CopyFile(m_hRoot, lpszFileName, hRoot, lpszFileName);
			if ( VFS_FAILED(nRet) )
			{
				LOGLINE("Copy File Failed");
			}
			if ( !m_pFileSysMgr->FindNextFile(hFind) )
				break;
		}
	}
	return TRUE;	
}


BOOL CVfstoolDoc::OnSaveDocument(LPCTSTR lpszPathName) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDocument::OnSaveDocument(lpszPathName);
}

