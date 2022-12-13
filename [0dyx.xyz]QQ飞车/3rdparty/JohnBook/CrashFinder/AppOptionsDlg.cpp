/*----------------------------------------------------------------------
"Debugging Applications" (Microsoft Press)
Copyright (c) 1997-2000 John Robbins -- All rights reserved.

With excellent updates from Scott Bloom, Ching Ming Kwok, 
Jeff Shanholtz, and Pablo Presedo!
----------------------------------------------------------------------*/
#include "stdafx.h"
#include "AppOptionsDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE [] = __FILE__ ;
#endif

CAppOptionsDlg :: CAppOptionsDlg ( )
                : CDialog ( CAppOptionsDlg::IDD )
{
    ASSERT ( this ) ;

    m_bConfirmDeletions = TRUE  ;
    m_bShowFullPaths    = FALSE ;
}

void CAppOptionsDlg :: DoDataExchange ( CDataExchange * pDX )
{
    ASSERT ( this ) ;
    CDialog::DoDataExchange ( pDX ) ;
    DDX_Check ( pDX , IDC_SHOWFULLPATHS     , m_bShowFullPaths    ) ;
    DDX_Check ( pDX , IDC_CONFIRMDELETIONS  , m_bConfirmDeletions ) ;
}

BEGIN_MESSAGE_MAP ( CAppOptionsDlg , CDialog )
END_MESSAGE_MAP ( )


