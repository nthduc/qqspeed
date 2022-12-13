#ifndef __UIOBJECTIMPL_T_H__
#define __UIOBJECTIMPL_T_H__

#pragma warning(disable:4786)
#include <map>

#include "uiinterface.h"
#include "commonui/uiobject_i.h"
#include "../componentimpl_t.h"
#include "../eventproducerimpl_t.h"
#include "../loghelpimpl_t.h"
#include "commonui/uievent_i.h"
//////////////////////////////////////////////////////////////////////////

namespace Tenio
{
//////////////////////////////////////////////////////////////////////////
// 保存最后一个收到消息的参数
//////////////////////////////////////////////////////////////////////////
typedef struct tagLastMessage
{
	UINT nMessage;
	WPARAM wParam;
	LPARAM lParam;
} LASTMESSAGE;
//////////////////////////////////////////////////////////////////////////


template<
		DWORD dwObjectID,
		int   nObjectVersion,
		class ConcreteType,
		class IUIObjectClassT,
		class IUIObjectClassEventT
		>
class IUIObjectImpl: 
	public TLogHelp<ConcreteType, dwObjectID>,
	public IEventProducerImpl
	<
		IUIObjectClassEventT,
		TComponentImpl<IUIObjectClassT, ConcreteType, dwObjectID, nObjectVersion>
	>
{
public:
	IUIObjectImpl()
	{
		m_wpOrigProc	  = NULL;
		m_hWindow		  = NULL;
		m_pDockManager    = NULL;
		m_pParentUIObject = NULL;
		m_pOwnerDrawEvent = NULL;	
		m_bExistClass	  = FALSE;
		m_bTracking		  = FALSE;
		m_bHover		  = FALSE;
		m_dwClassStyle	  = 0;
		memset(&m_stLastMessage, 0, sizeof(LASTMESSAGE));
	}

	virtual ~IUIObjectImpl()
	{
		ClearEventHandlers();
		Destroy();
		m_hWindow = NULL;
	}

	virtual void ComponentFinalize()
	{
	}

	//////////////////////////////////////////////////////////////////////////
	// 主create函数 由子类的Create函数调用
	//////////////////////////////////////////////////////////////////////////
	BOOL CreateWnd(
		DWORD dwStyle, 
		DWORD dwStyleEx,
		LPCTSTR lpszClassName,
		IUIObject* pParentUIObject, 
		int nID, 
		const RECT& rc)
	{
		BOOL bSuccess = FALSE;
		if(!::IsWindow(m_hWindow))
		{
			if((dwStyle & WS_CHILD) != 0)
			{
				CreateChildDetailWindow(dwStyle, dwStyleEx, lpszClassName, pParentUIObject, nID, rc);
			}
			else
			{
				HWND hParent = NULL;
				if(pParentUIObject != NULL)
				{
					hParent = pParentUIObject->GetHWnd();
				}
				CreateDetailWindow(dwStyle, dwStyleEx, lpszClassName, hParent, nID, rc);
			}
			
			if(::IsWindow(m_hWindow) && CreateDockManager())
			{
				if(m_bExistClass)
				{
					m_wpOrigProc = (WNDPROC)::SetWindowLong(m_hWindow, GWL_WNDPROC, (LONG)UIOjbectWinProc);
				}
				else
				{
					m_wpOrigProc = NULL;
				}
				
				LONG lWinData = (LONG)(IUIObjectImpl*)this;
				SetWindowLong(m_hWindow, GWL_USERDATA, lWinData);
				
				m_pParentUIObject = pParentUIObject;
				m_pDockManager->SetUIObject((IUIObject*)this);
				bSuccess = TRUE;
			}
		}
		
		return bSuccess;
	}	

	HWND CreateChildDetailWindow(
		DWORD dwStyle, 
		DWORD dwStyleEx,
		LPCTSTR lpClassName,
		IUIObject* pParentUIObject, 
		int nID, 
		const RECT& rc)
	{
		if(pParentUIObject != NULL)
		{
			HWND hParent = pParentUIObject->GetHWnd();
			if(::IsWindow(hParent))
			{
				m_hWindow = CreateDetailWindow(dwStyle, dwStyleEx, lpClassName, hParent, nID, rc);
			}
		}
		else
		{
			OutputErrorInfo(_T("CreateChildDetailWindow() Invalid parent window!"));
		}
		return m_hWindow;
	}

	//////////////////////////////////////////////////////////////////////////
	// 创建窗口，子类可重载此函数
	//////////////////////////////////////////////////////////////////////////
	virtual HWND CreateDetailWindow(
		DWORD dwStyle, 
		DWORD dwStyleEx,
		LPCTSTR lpClassName,
		HWND hParent, 
		int nID, 
		const RECT& rc)
	{
		RegisterWindowClass(m_dwClassStyle|CS_DBLCLKS, lpClassName);

		m_hWindow = ::CreateWindowEx(dwStyleEx, 
			lpClassName, 
			_T(""),
			dwStyle, 
			rc.left, rc.top,
			rc.right - rc.left,
			rc.bottom - rc.top,
			hParent, (HMENU)nID, NULL, (LPVOID)((IUIObjectImpl*)this));

		return m_hWindow;
	}

	//////////////////////////////////////////////////////////////////////////
	virtual void SetParentUIObject(IUIObject* pParentUIObject)
	{
		m_pParentUIObject = pParentUIObject;

		UnDock();

		if(pParentUIObject != NULL)
		{
			if(::IsWindow(pParentUIObject->GetHWnd()))
			{
				::SetParent(GetHWnd(), m_pParentUIObject->GetHWnd());
			}
		}
	}

	virtual void Destroy()
	{
		if(::IsWindow(m_hWindow))
		{
			// 还原wndproc
			if(m_wpOrigProc != NULL)
			{
				::SetWindowLong(m_hWindow, GWL_WNDPROC, (LONG)m_wpOrigProc);
			}

			::DestroyWindow(m_hWindow); 
		}
		m_hWindow = NULL;
	}

	virtual void Show()
	{
		if(::IsWindow(m_hWindow))
		{
			::ShowWindow(m_hWindow, SW_SHOW);
		}
	}

	virtual void Hide()
	{
		if(::IsWindow(m_hWindow))
		{
			::ShowWindow(m_hWindow, SW_HIDE);
		}
	}

	virtual void ReDraw(LPRECT lprc)
	{
		if(::IsWindow(m_hWindow))
		{
			::InvalidateRect(m_hWindow, lprc, FALSE);
		}
	}

	virtual void Enable(BOOL bEnable)
	{
		if(::IsWindow(m_hWindow))
		{
			::EnableWindow(m_hWindow, bEnable);
			ReDraw(NULL);
		}
	}

	virtual void Move(int x,int y, int nWidth, int nHeight)
	{
		if(::IsWindow(m_hWindow))
		{
			CRect rc;
			::GetWindowRect(GetHWnd(), &rc);
			
			if(nWidth == -1) nWidth = rc.Width();
			if(nHeight == -1) nHeight = rc.Height();
			
			::MoveWindow(m_hWindow, x, y,nWidth,nHeight, TRUE);
		}
	}

	virtual void Dock(ENMDOCKMODE nDockMode)
	{
		if(m_pParentUIObject != NULL)
		{
			IDockManager *pDockManager = m_pParentUIObject->GetDockManager();
			if(pDockManager != NULL)
			{
				pDockManager->AddSubDockUIObject((IUIObject*)this, nDockMode);
			}
		}
	}
		
	virtual void UnDock()
	{
		if(m_pDockManager != NULL)
		{
			m_pDockManager->ClearDock();
		}
	}

	inline HWND GetHWnd() 
	{
		return ((::IsWindow(m_hWindow))?m_hWindow:NULL);
	}

	virtual IUIObject* GetParentUIObject()
	{
		return m_pParentUIObject;
	}
		
	void AddOwnerDrawEventHandler(IOwnerDrawEvent *pOwnerDrawEvent)
	{
		m_pOwnerDrawEvent = pOwnerDrawEvent;
	}

	IDockManager* GetDockManager()
	{
		return m_pDockManager;
	}
	 
	virtual void SetToolTipHandle(ISkinToolTip* pTip)
	{
	}
	//////////////////////////////////////////////////////////////////////////
	// 当前消息的缺省处理
	//////////////////////////////////////////////////////////////////////////
	LRESULT Default()
	{
		if(m_wpOrigProc)
		{
			return ::CallWindowProc(m_wpOrigProc, m_hWindow, m_stLastMessage.nMessage, 
				m_stLastMessage.wParam,
				m_stLastMessage.lParam);
		}
		else
		{
			return ::DefWindowProc(
				m_hWindow, 
				m_stLastMessage.nMessage, 
				m_stLastMessage.wParam,
				m_stLastMessage.lParam);
		}
	}

	//////////////////////////////////////////////////////////////////////////
	/// 各控件对某消息的缺省处理
	//////////////////////////////////////////////////////////////////////////
	LRESULT DefWndProc(UINT nMessage, WPARAM wParam, LPARAM lParam)
	{
		if(m_wpOrigProc)
		{
			return ::CallWindowProc(m_wpOrigProc, m_hWindow, nMessage, wParam, lParam);
		}
		else
		{
			return ::DefWindowProc(m_hWindow, nMessage, wParam, lParam);
		}		
	}
	//////////////////////////////////////////////////////////////////////////
	// 统一的绘制接口
	//////////////////////////////////////////////////////////////////////////
	virtual void Draw(HDC hdc, RECT& rc) = 0;
	
	
protected:
	BOOL CreateDockManager()
	{
		GetTenioComponent(&m_pDockManager);
		return (m_pDockManager != NULL);
	}

	void NotifyChildrenReposition()
	{
		if(m_pDockManager != NULL)
		{
			m_pDockManager->NotifyChildReposition();
		}
	}

	//////////////////////////////////////////////////////////////////////////
	// 此方法由各控件自己实现, 用户处理则返回TRUE，交由系统处理则返回FALSE
	//////////////////////////////////////////////////////////////////////////
	virtual BOOL OnWindowMessage(UINT nMessage, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
	{
		*pResult = 0;
		switch(nMessage)
		{
		case WM_SIZE:
			{
 				OnSize(wParam, LOWORD(lParam), HIWORD(lParam));
				break;
			}
		case WM_PAINT:
			{
				OnPaint();
				break;
			}
		case WM_LBUTTONDOWN:
			{
				CPoint pt(lParam);
				OnLButtonDown(wParam, pt);
				break;
			}
		case WM_LBUTTONUP:
			{
				CPoint pt(lParam);
				OnLButtonUp(wParam, pt);
				break;
			}
		case WM_LBUTTONDBLCLK:
			{
				CPoint pt(lParam);
				OnLButtonDblClk(wParam, pt);
				break;
			}
		case WM_RBUTTONDOWN:
			{
				CPoint pt(lParam);
				OnRButtonDown(wParam, pt);
				break;
			}
		case WM_RBUTTONUP:
			{
				CPoint pt(lParam);
				OnRButtonUp(wParam, pt);
				break;
			}
		case WM_MOUSEMOVE:
			{
				CPoint pt(lParam);
				OnMouseMove(wParam, pt);
				break;
			}
		case WM_MOUSELEAVE:
			{
				OnMouseLeave();
				break;
			}
		case WM_MOUSEWHEEL:
			{
				CPoint pt(lParam);
				*pResult = OnMouseWheel(LOWORD(wParam), HIWORD(wParam), pt);
				break;
			}
		case WM_SETCURSOR:
			{
				*pResult = OnSetCursor((HWND)wParam, LOWORD(lParam), HIWORD(lParam));
				break;
			}
		case WM_ERASEBKGND:
			{
				return OnEraseBkgnd((HDC)wParam);
			}
		case WM_CTLCOLORSTATIC:
		case WM_CTLCOLOREDIT:
			{
				*pResult = (LRESULT)OnCtlColor((HDC)wParam, (HWND)lParam, nMessage);	
				break;
			}
		default:
			return FALSE;
		}
		return TRUE;
	}

	//////////////////////////////////////////////////////////////////////////
	// 几个常用的消息处理函数
	//////////////////////////////////////////////////////////////////////////
	virtual void OnSize(UINT nType, int cx, int cy){Default();}
	virtual void OnPaint(){Default();}
	virtual void OnLButtonDown(UINT nFlags, CPoint point){Default();}
	virtual void OnLButtonUp(UINT nFlags, CPoint point){Default();}
	virtual void OnLButtonDblClk(UINT nFlags, CPoint point){Default();}
	virtual void OnRButtonDown(UINT nFlags, CPoint point){Default();}
	virtual void OnRButtonUp(UINT nFlags, CPoint point){Default();}
	virtual void OnMouseMove(UINT nFlags, CPoint point)
	{
		if(!m_bTracking)
		{
			TRACKMOUSEEVENT tme = {0};
			tme.cbSize = sizeof(tme);
			tme.hwndTrack = GetHWnd();
			tme.dwFlags = TME_LEAVE | TME_HOVER;
			tme.dwHoverTime = 1;
			m_bTracking = _TrackMouseEvent(&tme);
			m_bHover = TRUE;
		}		
		Default();
	}
	virtual void OnMouseLeave()
	{
		m_bTracking = FALSE;
		m_bHover = FALSE;
		Default();
	}
	virtual BOOL OnMouseWheel( UINT nFlags, short zDelta, CPoint pt){return Default();}
	virtual BOOL OnSetCursor(HWND HWND, UINT nHitTest, UINT nMessage){return Default();}
	virtual BOOL OnEraseBkgnd(HDC hdc){return Default();}
	virtual HBRUSH OnCtlColor(HDC hdc, HWND hWnd, UINT nMessage){return (HBRUSH)Default();}

	LRESULT WindowProc(HWND hWnd, UINT nMessage, WPARAM wParam, LPARAM lParam)
	{
		LRESULT lResult = 0;
		
		if(nMessage == WM_CREATE)
		{
			m_hWindow = hWnd;
		}
		m_stLastMessage.nMessage = nMessage;
		m_stLastMessage.wParam = wParam;
		m_stLastMessage.lParam = lParam;
		
		BOOL bUserProcess = FALSE;
		FireEvent(
			IUIObjectClassEventT::OnWindowMessage, 
			hWnd, nMessage, wParam, lParam, &lResult, &bUserProcess);
	
		if(!bUserProcess)
		{
			if(!OnWindowMessage(nMessage, wParam, lParam, &lResult))
			{
				lResult = Default();
			}
		}

		return lResult;		
	}
	
	//////////////////////////////////////////////////////////////////////////
	static LRESULT CALLBACK UIOjbectWinProc(
		HWND hWnd, UINT nMessage, WPARAM wParam, LPARAM lParam)
	{
		LRESULT hr = 0;
		if(nMessage == WM_CREATE)
		{
			CREATESTRUCT* pCS = (CREATESTRUCT*)lParam;
			IUIObjectImpl* pUIObject = (IUIObjectImpl*)(pCS->lpCreateParams);
			if(pUIObject != NULL)
			{
				hr = pUIObject->WindowProc(hWnd, nMessage, wParam, lParam);
			}
			else
			{
				return ::DefWindowProc(hWnd, nMessage, wParam, lParam);
			}
		}
		else
		{
			LONG lWindowData = ::GetWindowLong(hWnd, GWL_USERDATA);			
			IUIObjectImpl* pUIObject = (IUIObjectImpl*)(lWindowData);
			if(pUIObject != NULL)
			{
				hr = pUIObject->WindowProc(hWnd, nMessage, wParam, lParam);
			}
			else
			{
				return ::DefWindowProc(hWnd, nMessage, wParam, lParam);
			}
		}
		return hr;			
	}
	
	//////////////////////////////////////////////////////////////////////////
	BOOL RegisterWindowClass(DWORD dwStyle, LPCTSTR lpszClassName)
	{
		WNDCLASS wndclass = {0};
		
		if(GetClassInfo(NULL, lpszClassName, &wndclass))
		{
			m_bExistClass = TRUE;
			return TRUE;
		}
		else if(GetClassInfo(GetModuleHandle(NULL), lpszClassName,&wndclass))
		{
			m_bExistClass = FALSE;
			return TRUE;
		}
		else
		{
			m_bExistClass = FALSE;
			wndclass.style			= dwStyle;
			wndclass.lpfnWndProc	= UIOjbectWinProc;
			wndclass.cbClsExtra		= 0 ;
			wndclass.cbWndExtra		= 0 ;
			wndclass.hInstance		= 0 ;
			wndclass.hIcon			= 0 ;
			wndclass.hCursor		= ::LoadCursor(NULL, IDC_ARROW);
			wndclass.hbrBackground	= 0;
			wndclass.lpszClassName	= lpszClassName;
			return RegisterClass (&wndclass);
		}
	}
	
protected:
	inline void SetRedraw(BOOL bRedraw)
	{ assert(::IsWindow(m_hWindow)); ::SendMessage(m_hWindow, WM_SETREDRAW, bRedraw, 0); }
	inline int GetDlgCtrlID() {return ::GetDlgCtrlID(GetHWnd());}
	inline int IsHover() {return m_bHover;}
	inline void SetRegClassStyle(DWORD dwStyle) {m_dwClassStyle = dwStyle;}
protected:
	HWND m_hWindow;
	IUIObject* m_pParentUIObject;
	IDockManager* m_pDockManager; 
	IOwnerDrawEvent* m_pOwnerDrawEvent;
	LASTMESSAGE m_stLastMessage;
	BOOL m_bTracking;
	BOOL m_bHover;
	BOOL m_bExistClass;
	DWORD m_dwClassStyle;//注册窗口类时所用的style
	WNDPROC m_wpOrigProc;
};
}
#endif