#pragma once

#define BTN_STATE_NORMAL	0
#define BTN_STATE_HOVER		1
#define BTN_STATE_DOWN		2
#define BTN_STATE_DISABLE	3
#define BTN_STATE_MAX		4

// CFxButton

class CFxButton : public CButton
{
	DECLARE_DYNAMIC(CFxButton)

public:
	CFxButton();
	virtual ~CFxButton();

	void SetBkImage(LPCSTR lpszName);

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

public:
	virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMouseHover(WPARAM wParam, LPARAM lParam);

private:
	BOOL	m_bTracking;
	DWORD	m_dwCurrentState;
	CHAR	m_szBkImage[MAX_PATH];
	HBITMAP	m_hBitmaps[BTN_STATE_MAX];
};


