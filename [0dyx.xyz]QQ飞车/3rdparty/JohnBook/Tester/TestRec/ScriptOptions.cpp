/*----------------------------------------------------------------------
Debugging Applications for Microsoft .NET and Microsoft Windows
Copyright © 1997-2003 John Robbins -- All rights reserved.
----------------------------------------------------------------------*/

#include "stdafx.h"
#include "TestRec.h"
#include "ScriptOptions.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

////////////////////////////////////////////////////////////////////////
// CScriptOptions dialog


CScriptOptions::CScriptOptions(CWnd* pParent /*=NULL*/)
    : CDialog(CScriptOptions::IDD, pParent)
{
    //{{AFX_DATA_INIT(CScriptOptions)
    m_iMinPixelDrag = 0;
    m_bRecordAllMouseMoves = FALSE;
    m_bRecordFocusChangesWithMouse = FALSE;
    //m_bUseAbsoluteCoordinates = FALSE;
    m_dwSleepTime = 0;
    m_iLanguageChoice = -1;
	m_bDoChildFocus = FALSE;
	m_bMultiMon = FALSE;
	//}}AFX_DATA_INIT
}


void CScriptOptions::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CScriptOptions)
    DDX_Text(pDX, IDC_MINDRAGPIXELS, m_iMinPixelDrag);
    DDV_MinMaxInt(pDX, m_iMinPixelDrag, 0, 100000);
    DDX_Check(pDX, IDC_RECORDALLMOUSEMOVES, m_bRecordAllMouseMoves);
    DDX_Check(pDX, IDC_RECORDFOCUSCHANGESWITHMOUSE, m_bRecordFocusChangesWithMouse);
    //DDX_Check(pDX, IDC_USEABSOLUTECOORDS, m_bUseAbsoluteCoordinates);
    DDX_Text(pDX, IDC_MINSLEEPTIME, m_dwSleepTime);
    DDV_MinMaxInt(pDX, m_dwSleepTime, 0, 1000);
    DDX_Radio(pDX, IDC_JAVASCRIPT, m_iLanguageChoice);
	DDX_Check(pDX, IDC_DOCHILDFOCUSSTATEMENTS, m_bDoChildFocus);
	DDX_Check(pDX, IDC_RECORDMULTIMON, m_bMultiMon);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CScriptOptions, CDialog)
    //{{AFX_MSG_MAP(CScriptOptions)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

////////////////////////////////////////////////////////////////////////
// CScriptOptions message handlers

