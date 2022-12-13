
#pragma once

#include "wtypes.h"

extern HMODULE g_hModule;

namespace Tenio
{
#define WM_THREADMESSAGE		WM_USER	 + 1000
	
	template<typename TRealSubClass>
	class TThreadMessage 
	{
	public:
		TThreadMessage()
		{
			CreateThreadWindow();
		}
		virtual ~ TThreadMessage()
		{
			DestroyThreadWindow();
		}
		virtual DWORD HostMessageProc(LPBYTE lpData, DWORD dwLength)  = 0;

		void SendDataToHost(LPBYTE lpData, DWORD dwLength)
		{
			PostMessage(m_hThreadWnd, WM_THREADMESSAGE, (WPARAM) lpData, (LPARAM) dwLength);
		}

	protected:
		static LRESULT CALLBACK ThreadMessageProc(HWND hWnd, UINT nMessage, WPARAM wParam, LPARAM lParam)
		{
			TThreadMessage* pThreadMessageObj = (TThreadMessage*) GetWindowLong(hWnd, GWL_USERDATA);
			
			if (pThreadMessageObj != NULL) 
			{
				switch(nMessage) 
				{
				case WM_THREADMESSAGE:
					pThreadMessageObj->HostMessageProc((LPBYTE) wParam, (DWORD) lParam);
					break;
				default:
					break;
				};
			}
			
			return 1;
		}
				
		void CreateThreadWindow()
		{
			if (!IsWindow(m_hThreadWnd)) 
			{
				m_hThreadWnd = CreateWindow(
					_T("Static"),
					NULL,
					WS_POPUP,
					0,
					0,
					0,
					0,
					NULL,
					NULL,
					g_hModule,
					NULL);
				
				if (IsWindow(m_hThreadWnd)) 
				{
					m_pOldWndProc = (WNDPROC) SetWindowLong(m_hThreadWnd, GWL_WNDPROC, (LONG) ThreadMessageProc);
					m_lOldUserData = SetWindowLong(m_hThreadWnd, GWL_USERDATA, (LONG) this);
				}
			}
		}
		
		void DestroyThreadWindow()
		{
			if (IsWindow(m_hThreadWnd)) 
			{
				SetWindowLong(m_hThreadWnd, GWL_WNDPROC, (LONG) m_pOldWndProc);
				SetWindowLong(m_hThreadWnd, GWL_USERDATA, (LONG) m_lOldUserData);
				DestroyWindow(m_hThreadWnd);
				m_hThreadWnd = NULL;
			}
		}
		
		TRealSubClass * Me()
		{
			return static_cast<TRealSubClass*>(this);
		}
	protected:
		HWND	m_hThreadWnd;
		WNDPROC m_pOldWndProc;
		LONG m_lOldUserData;
	};
	
};