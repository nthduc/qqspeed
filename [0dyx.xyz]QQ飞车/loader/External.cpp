// External.cpp : implementation file
//

#include "stdafx.h"
#include "loader.h"
#include "External.h"
#include "loaderdlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CExternal

IMPLEMENT_DYNCREATE(CExternal, CCmdTarget)

CExternal::CExternal()
{
	EnableAutomation();
}

CExternal::~CExternal()
{
}


void CExternal::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CCmdTarget::OnFinalRelease();
}


BEGIN_MESSAGE_MAP(CExternal, CCmdTarget)
	//{{AFX_MSG_MAP(CExternal)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CExternal, CCmdTarget)
	//{{AFX_DISPATCH_MAP(CExternal)
	DISP_FUNCTION(CExternal, "ShowDlg", ShowDlg, VT_EMPTY, VTS_NONE)
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Note: we add support for IID_IExternal to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {016AF104-4062-495C-9083-99D906D3A65D}
static const IID IID_IExternal =
{ 0x16af104, 0x4062, 0x495c, { 0x90, 0x83, 0x99, 0xd9, 0x6, 0xd3, 0xa6, 0x5d } };

BEGIN_INTERFACE_MAP(CExternal, CCmdTarget)
	INTERFACE_PART(CExternal, IID_IExternal, Dispatch)
END_INTERFACE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CExternal message handlers

void CExternal::ShowDlg() 
{
//	((CLoaderDlg *)AfxGetMainWnd())->ShowDlg();
}
