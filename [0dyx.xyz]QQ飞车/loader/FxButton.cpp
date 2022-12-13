// FxButton.cpp : implementation file
//

#include "stdafx.h"
#include "loader.h"
#include "FxButton.h"
#include ".\fxbutton.h"


// CFxButton

IMPLEMENT_DYNAMIC(CFxButton, CButton)
CFxButton::CFxButton():
m_bTracking(FALSE),
m_dwCurrentState(BTN_STATE_NORMAL)
{
	ZeroMemory(m_hBitmaps, sizeof(HBITMAP) * BTN_STATE_MAX);
	ZeroMemory(m_szBkImage, MAX_PATH * sizeof(TCHAR));
}

CFxButton::~CFxButton()
{
	for ( int i = 0; i < BTN_STATE_MAX; i++ )
	{
		HBITMAP &hBitmap = m_hBitmaps[i];
		if ( hBitmap )
		{
			DeleteObject(hBitmap);
			hBitmap = NULL;
		}
	}
}


BEGIN_MESSAGE_MAP(CFxButton, CButton)
	ON_WM_ERASEBKGND()
	ON_WM_DRAWITEM()
	ON_WM_MOUSEMOVE()
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
	ON_MESSAGE(WM_MOUSEHOVER, OnMouseHover)
END_MESSAGE_MAP()



// CFxButton message handlers
void CFxButton::SetBkImage(LPCSTR lpszName)
{
	strncpy(m_szBkImage, lpszName, MAX_PATH);

	CHAR szFileName[MAX_PATH] = "";
	const LPCSTR pszFileNameExt[] = { 
		"_normal.bmp", "_hover.bmp", "_down.bmp", "_disable.bmp" };
	for ( int i = 0; i < BTN_STATE_MAX; i++ )
	{
		strcpy(szFileName, lpszName);
		strcat(szFileName, pszFileNameExt[i]);
		HBITMAP hBitmap = (HBITMAP)LoadImage(NULL, szFileName, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_DEFAULTSIZE);
		ASSERT(hBitmap);
		m_hBitmaps[i] = hBitmap;
	}
}

BOOL CFxButton::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default
	return TRUE;
	//return CButton::OnEraseBkgnd(pDC);
}

void CFxButton::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	if ( !m_bTracking )
	{
		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof(tme);
		tme.hwndTrack = m_hWnd;
		tme.dwFlags = TME_LEAVE | TME_HOVER;
		tme.dwHoverTime = 1;
		m_bTracking = _TrackMouseEvent(&tme);
	}
	CButton::OnMouseMove(nFlags, point);
}

LRESULT CFxButton::OnMouseLeave(WPARAM wParam, LPARAM lParam)
{
	m_bTracking = FALSE;
	if ( m_dwCurrentState == BTN_STATE_HOVER )
	{
		m_dwCurrentState = BTN_STATE_NORMAL;
		InvalidateRect(NULL);
	}
	return S_OK;
}

LRESULT CFxButton::OnMouseHover(WPARAM wParam, LPARAM lParam)
{
	if ( m_dwCurrentState == BTN_STATE_NORMAL )
	{
		m_dwCurrentState = BTN_STATE_HOVER;
		InvalidateRect(NULL);
	}
	return S_OK;
}

void CFxButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	switch ( lpDrawItemStruct->itemState )
	{
	case ODS_CHECKED:
		m_dwCurrentState = BTN_STATE_DOWN;
		break;
	case ODS_DISABLED:
		m_dwCurrentState = BTN_STATE_DISABLE;
		break;
	case ODS_DEFAULT:
		m_dwCurrentState = BTN_STATE_NORMAL;
		break;
	}
	
	HBITMAP hBitmap = m_hBitmaps[m_dwCurrentState];
	if ( hBitmap )
	{
		HDC hMemDC = CreateCompatibleDC(lpDrawItemStruct->hDC);
		HBITMAP hOldBitmap = (HBITMAP)SelectObject(hMemDC, hBitmap);
		BitBlt(lpDrawItemStruct->hDC, 0, 0, 
			lpDrawItemStruct->rcItem.right - lpDrawItemStruct->rcItem.left,
			lpDrawItemStruct->rcItem.bottom - lpDrawItemStruct->rcItem.top,
			hMemDC, 0, 0, SRCCOPY);
		DeleteObject(hMemDC);
	}
	// TODO:  Add your code to draw the specified item
}
