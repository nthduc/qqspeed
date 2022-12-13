/*---------------------------------------------------------------------
    Debugging .NET and Windows-based Applications - John Robbins
---------------------------------------------------------------------*/
#ifndef _CFGDBGCHOOSER_H
#define _CFGDBGCHOOSER_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CCfgDbgChooser : public CDialog
{
// Construction
public:
    CCfgDbgChooser(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
    //{{AFX_DATA(CCfgDbgChooser)
    enum { IDD = IDD_CONFIGUREDBGCHOOSER };
    CEdit   m_cWinDBGCfg;
    CEdit   m_cVCSevenDbgCfg ;
    CEdit   m_cVCDbgCfg;
    CEdit   m_cDrWatsonCfg;
    CString m_sVCSevenDbgCfg ;
    CString m_sDrWatsonCfg;
    CString m_sVCDbgCfg;
    CString m_sWinDBGCfg;
    //}}AFX_DATA


// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CCfgDbgChooser)
    protected:
    virtual void DoDataExchange(CDataExchange* pDX);
    //}}AFX_VIRTUAL

// Implementation
protected:
    HICON m_hIcon;

    // Generated message map functions
    //{{AFX_MSG(CCfgDbgChooser)
    virtual void OnOK();
    afx_msg void OnHelp();
    virtual BOOL OnInitDialog();
    afx_msg void OnPaint();
    afx_msg HCURSOR OnQueryDragIcon();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}

#endif // _CFGDBGCHOOSER_H
