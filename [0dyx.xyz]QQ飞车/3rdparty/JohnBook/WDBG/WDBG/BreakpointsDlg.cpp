/*----------------------------------------------------------------------
Debugging Applications for Microsoft .NET and Microsoft Windows
Copyright © 1997-2003 John Robbins -- All rights reserved.
----------------------------------------------------------------------*/
#include "stdafx.h"
#include "WDBG.h"
#include "WDBGProjDoc.h"
#include "BreakpointsDlg.h"
#include "NewEditBPDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CBreakpointsDlg :: CBreakpointsDlg ( BPARRAY *      pBpArray ,
                                     CWDBGProjDoc * pDoc     ,
                                     CWnd *         pParent   )
    : CDialog ( CBreakpointsDlg::IDD , pParent )
{
    //{{AFX_DATA_INIT(CBreakpointsDlg)
        // NOTE: the ClassWizard will add member initialization here
    //}}AFX_DATA_INIT
    m_pBpArray = pBpArray ;
    ASSERT ( FALSE == IsBadReadPtr ( pBpArray , sizeof ( BPARRAY * ) ));
    m_pDoc = pDoc ;
    m_bModifiedList = FALSE ;
}


void CBreakpointsDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CBreakpointsDlg)
    DDX_Control(pDX, IDC_BPLIST, m_cBPList);
    DDX_Control(pDX, IDEDIT, m_cEditBtn);
    DDX_Control(pDX, IDREMOVE, m_cRemoveBtn);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBreakpointsDlg, CDialog)
    //{{AFX_MSG_MAP(CBreakpointsDlg)
    ON_BN_CLICKED(IDEDIT, OnEdit)
    ON_BN_CLICKED(IDNEW, OnNew)
    ON_BN_CLICKED(IDREMOVE, OnRemove)
    ON_LBN_SELCHANGE(IDC_BPLIST, OnSelchangeBplist)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CBreakpointsDlg :: OnOK ( )
{
    BPARRAY cUpdatedArray ;
    BPARRAY cDeletedArray ;

    // Loop through the master array.
    INT_PTR iSize = m_pBpArray->GetSize ( ) ;
    for ( INT_PTR i = 0 ; i < iSize ; i++ )
    {
        CLocationBP * pCurrMasterBP = m_pBpArray->GetAt ( i ) ;

        // Loop through the listbox looking for the match.
        BOOL bFound = FALSE ;
        CLocationBP * pListCurrBP = NULL ;
        int iLBCount = m_cBPList.GetCount ( ) ;
        for ( int j = 0 ; j < iLBCount ; j++ )
        {
            pListCurrBP = (CLocationBP *)m_cBPList.GetItemData ( j ) ;
            ASSERT ( NULL != pListCurrBP ) ;

            // Check the names this way so that in the future more info
            // can be placed in the listbox.
            if ( 0 == _tcscmp ( pCurrMasterBP->GetBreakpointName ( ) ,
                                pListCurrBP->GetBreakpointName ( )    ))
            {
                // There's a match, get the enabled state.
                BOOL bEnabled = m_cBPList.GetCheck ( j ) ;

                // Coordinate the state on the master.
                if ( ( TRUE == bEnabled                         ) &&
                     ( eDisabled == pCurrMasterBP->GetState ( ) )    )
                {
                    // The user is re-enabling this BP.
                    pCurrMasterBP->SetToUnresolved ( ) ;
                    m_bModifiedList = TRUE ;
                }
                else if ( ( FALSE == bEnabled                       ) &&
                          ( pCurrMasterBP->GetState ( ) > eDisabled )  )
                {
                    // Remember to clear out the single step so it won't
                    // come through as an exception after this dialog
                    // gets out of the way.
                    if ( eWaitingForTrace ==
                                           pCurrMasterBP->GetState ( ) )
                    {
                        pCurrMasterBP->ClearSingleStep ( ) ;
                    }
                    pCurrMasterBP->SetToDisabled ( ) ;
                    m_bModifiedList = TRUE ;
                }
                // Add this item to the temp list.
                cUpdatedArray.Add ( pCurrMasterBP ) ;

                // No need to keep looking.
                bFound = TRUE ;
                break ;
            }
        }
        if ( FALSE == bFound )
        {
            // This master BP was deleted so put it in the deleted
            // array.
            cDeletedArray.Add ( pCurrMasterBP ) ;
        }
        else
        {
            // Delete the one in the list I just looked at so I do not
            // need to iterate over it again.
            delete pListCurrBP ;
            m_cBPList.DeleteString ( j ) ;
        }
    }

    // The master list was processed.  If there are any remaining in the
    // listbox, they are new so I need to process them as well.  I will
    // just check the state and add them to the updated list.
    CLocationBP * pListNewBP = NULL ;
    int iNewLBBP = m_cBPList.GetCount ( ) ;
    for ( int j = 0 ; j < iNewLBBP ; j++ )
    {
        pListNewBP = (CLocationBP *)m_cBPList.GetItemData ( j ) ;
        ASSERT ( NULL != pListNewBP ) ;

        BOOL bEnabled = m_cBPList.GetCheck ( j ) ;
        if ( FALSE == bEnabled )
        {
            pListNewBP->SetToDisabled ( ) ;
        }
        cUpdatedArray.Add ( pListNewBP ) ;
    }

    // The three lists at this point are:
    // 1.  The master list, m_pBpArray which is still the same as it
    //     came into the dialog.
    // 2.  cUpdatedArray contains the new and updated BPs.
    // 3.  cDeletedArray holds all deleted BPs.

    // Clear out the master list.  All memory it held is now in one of
    // the two other arrays.
    m_pBpArray->RemoveAll ( ) ;

    // Add all the new and updated pointers to the master list.
    INT_PTR iUpdatedSize = cUpdatedArray.GetSize ( ) ;
    for ( INT_PTR u = 0 ; u < iUpdatedSize ; u++ )
    {
        m_pBpArray->Add ( cUpdatedArray.GetAt ( u ) ) ;
    }

    // Finally, run through the deleted items and delete the memory
    // associated with them.
    INT_PTR iDelSize = cDeletedArray.GetSize ( ) ;
    for ( INT_PTR d = 0 ; d < iDelSize ; d++ )
    {
        CLocationBP * pBP = cDeletedArray.GetAt ( d ) ;
        delete pBP ;
    }

    // Clear the two temporary arrays.
    cUpdatedArray.RemoveAll ( ) ;
    cDeletedArray.RemoveAll ( ) ;

    // Whew!  Done!
    CDialog::OnOK();

}

void CBreakpointsDlg::OnCancel()
{
    // Nothing gets updated.  Just loop through the listbox and delete
    // all the allocated memory.
    int iCount = m_cBPList.GetCount ( ) ;
    for ( int i = 0 ; i < iCount ; i++ )
    {
        CLocationBP * pBP = (CLocationBP *)m_cBPList.GetItemData ( i ) ;
        ASSERT ( NULL != pBP ) ;
        delete pBP ;
    }
    m_bModifiedList = FALSE ;

    CDialog::OnCancel();
}

void CBreakpointsDlg::OnEdit()
{

    int iVal = m_cBPList.GetCurSel ( ) ;
    ASSERT ( LB_ERR != iVal ) ;

    // Get the BP.
    CLocationBP * pBP = (CLocationBP *)m_cBPList.GetItemData ( iVal ) ;

    CNewEditBPDlg cDlg ( FALSE , m_pDoc ) ;

    cDlg.m_sLocation = pBP->GetBreakpointName ( ) ;
    cDlg.m_eType = pBP->GetType ( ) ;

    if ( eCounted == cDlg.m_eType )
    {
        // TODO TODO
        // Add the pass count here!
    }
    if ( IDOK == cDlg.DoModal ( ) )
    {
        if ( cDlg.m_sLocation != pBP->GetBreakpointName ( ) )
        {
            m_bModifiedList = TRUE ;
        }
        if ( cDlg.m_eType != pBP->GetType ( ) )
        {
            m_bModifiedList = TRUE ;
        }
        // TODO TODO
        // Check the pass count!

        if ( TRUE == m_bModifiedList )
        {
            // Create a new BP to replace the one being edited.
            CLocationBP * pNewLocBP = new CLocationBP ( ) ;

            BOOL bEnabled = m_cBPList.GetCheck ( iVal ) ;

            // Set up the important stuff.
            pNewLocBP->SetBreakpointName ( cDlg.m_sLocation ) ;


            // Delete the old data.
            CLocationBP * pOldBP =
                     (CLocationBP *)m_cBPList.GetItemDataPtr ( iVal ) ;
            delete pOldBP ;

            // Delete the existing string.
            m_cBPList.DeleteString ( iVal ) ;

            // Insert the new stuff.
            m_cBPList.InsertString ( iVal ,
                                     pNewLocBP->GetBreakpointName ( ) );
            m_cBPList.SetItemDataPtr ( iVal , (void*)pNewLocBP ) ;
            m_cBPList.SetCheck ( iVal , bEnabled ) ;
            m_cBPList.SetCurSel ( iVal ) ;

            // Force the update.
            OnSelchangeBplist( ) ;
        }
    }
}

void CBreakpointsDlg::OnNew()
{
    CNewEditBPDlg cDlg ( TRUE , m_pDoc ) ;

    if ( IDOK == cDlg.DoModal ( ) )
    {
        m_bModifiedList = TRUE ;

        // Create the BP.
        CLocationBP * pLocBP = new CLocationBP ( ) ;

        pLocBP->SetBreakpointName ( cDlg.m_sLocation ) ;

        // Put the string into the listbox to show the new BP.
        int iVal = m_cBPList.AddString ( pLocBP->GetBreakpointName ( ));
        // Go ahead and enable the check box.
        m_cBPList.SetCheck ( iVal , 1 ) ;
        // Stick the BP as the extra data.
        m_cBPList.SetItemDataPtr ( iVal , (void*)pLocBP ) ;
        // Set the selection to the one I just put in there.
        m_cBPList.SetCurSel ( iVal ) ;
        // Force the update.
        OnSelchangeBplist( ) ;
    }
}

void CBreakpointsDlg::OnRemove()
{
    m_bModifiedList = TRUE ;

    int iVal = m_cBPList.GetCurSel ( ) ;
    ASSERT ( LB_ERR != iVal ) ;

    // Get the BP.
    CLocationBP * pBP = (CLocationBP *)m_cBPList.GetItemData ( iVal ) ;
    ASSERT ( NULL != pBP ) ;

    delete pBP ;

    m_cBPList.DeleteString ( iVal ) ;

    m_cBPList.SetCurSel ( 0 ) ;

    // Force the reevaluation of the edit and remove buttons.
    OnSelchangeBplist ( ) ;
}

BOOL CBreakpointsDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    // Loop through and add all the existing BPs to the listbox.  Notice
    // that I not sticking the real memory in the list box items.
    INT_PTR iCount = m_pBpArray->GetSize ( ) ;
    for ( INT_PTR i = 0 ; i < iCount ; i++ )
    {
        CLocationBP * pOldBP = m_pBpArray->GetAt ( i ) ;

        CLocationBP * pNewBP = new CLocationBP ( *pOldBP ) ;
        int iVal = m_cBPList.AddString(pNewBP->GetBreakpointName());

        if ( pNewBP->GetState ( ) > eDisabled )
        {
            m_cBPList.SetCheck ( iVal , 1 ) ;
        }
        m_cBPList.SetItemDataPtr ( iVal , (void*)pNewBP ) ;

    }

    m_cBPList.SetCurSel ( 0 ) ;

    OnSelchangeBplist ( ) ;
    return TRUE;
}

void CBreakpointsDlg::OnSelchangeBplist()
{
    int iVal = m_cBPList.GetCurSel ( ) ;
    if ( LB_ERR == iVal )
    {
        m_cRemoveBtn.EnableWindow ( FALSE ) ;
        m_cEditBtn.EnableWindow ( FALSE ) ;
    }
    else
    {
        m_cRemoveBtn.EnableWindow ( ) ;
        m_cEditBtn.EnableWindow ( ) ;
    }
}
