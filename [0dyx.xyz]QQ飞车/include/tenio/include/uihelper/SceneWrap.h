#ifndef __BUFFERCANVASWRAP_H__
#define __BUFFERCANVASWRAP_H__

#pragma warning(disable:4786)
#include "../template/teniopointer_t.h"
#include "../memoryalloctor_i.h"
#include "assert.h"
#include "tenio_t.h"

#include "../layer/scenefactory_i.h"
#include "../layer/normalscene_i.h"
#include "../../components/gdihelper/gdihelper.h"
#include "../june/windccanvas_i.h"
#include "../june/ximagelib_i.h"
#include "../june/ximagecanvas_i.h"

namespace Tenio
{
	class CSceneWrap
	{
	public:
		CSceneWrap()
		{
			m_pScene = NULL;
			m_pSceneUpdate = NULL;
		}

		virtual ~CSceneWrap()
		{
			ReleaseScene();
		}

		void SetCanvasUpdate(IJuneCanvasUpdate* pCanvasUpdate)
		{
			if(m_pScene != NULL)
			{
				if(m_pScene->GetSceneID() == ESI_NORMAL)
				{
					static_cast<INormalScene*>(m_pScene)->SetCanvasUpdate(pCanvasUpdate);				
				}
			}
		}

		virtual BOOL CreateScene(IJuneCanvasUpdate* pCanvasUpdate)
		{
			TTenioPtr<ISceneFactory> pFactory;
			if(m_pScene == NULL && pFactory)
			{
				m_pScene = pFactory->CreateScene(ESI_NORMAL, &m_pSceneUpdate);
				if(m_pScene != NULL && m_pScene->GetSceneID() == ESI_NORMAL)
				{
					static_cast<INormalScene*>(m_pScene)->SetCanvasUpdate(pCanvasUpdate);
				}
			}
			
			return (m_pScene)?TRUE:FALSE;
		}
		
		void ReleaseScene()
		{
			if(m_pScene != NULL)
			{
				TTenioPtr<ISceneFactory> pFactory;
				if(pFactory)
				{
					pFactory->ReleaseScene(m_pScene);
					m_pScene = NULL;
				}
			}			
		}

		void GetScene(IJuneScene** ppScene, IJuneSceneUpdate** ppSceneUpdate)
		{
			if(ppScene != NULL)
			{
				*ppScene = m_pScene;
			}
			if(ppSceneUpdate != NULL)
			{
				*ppSceneUpdate = m_pSceneUpdate;
			}
		}		

	public:
		IJuneScene* m_pScene;
		IJuneSceneUpdate* m_pSceneUpdate;	
	};

	//////////////////////////////////////////////////////////////////////////
	class CCanvasWrap : public TTenio<CCanvasWrap>
	{
	public:
		CCanvasWrap(IJuneScene* pScene)
		{
			m_hWindow = NULL;
			m_pScene = pScene;
		}

		void SetScene(IJuneScene* pScene)
		{
			m_pScene = pScene;
		}
	protected:
		HWND m_hWindow;
		IJuneScene* m_pScene;		
	};
	//////////////////////////////////////////////////////////////////////////
	class CBufferCanvasWrap : 
		public CCanvasWrap,
		public IJuneCanvasUpdate
	{
	public:
		CBufferCanvasWrap(IJuneScene* pScene) : CCanvasWrap(pScene)
		{
			m_pBufferDC = NULL;
			m_pDCCanvas = NULL;
		}

		virtual ~CBufferCanvasWrap()
		{
			DeleteBuffer();
		}
			
		IJuneCanvas* GetBufferCanvas()
		{
			return static_cast<IJuneCanvas*>(m_pDCCanvas);
		}

		void CreateBuffer(HWND hWnd)
		{
			m_hWindow = hWnd;

			DeleteBuffer();
			
			if(::IsWindow(m_hWindow))
			{
				if(m_pDCCanvas == NULL)
				{
					GetTenioComponent(&m_pDCCanvas);
				}
				static_cast<INormalScene*>(m_pScene)->SetCanvasUpdate(this);

				HDC hdc = ::GetDC(m_hWindow);
				CClientRect rc(m_hWindow);
				m_pBufferDC = new CBufferDC(hdc, rc, FALSE);
				::ReleaseDC(m_hWindow, hdc);	
				
				IJuneRegion* pCanvasRegion = m_pDCCanvas->GetUpdateRegion();
				if(!pCanvasRegion->IsEmpty())
				{
					m_pDCCanvas->AttachDc(m_pBufferDC->GetSafeHdc());
					m_pScene->DrawAtCanvas(m_pDCCanvas, 0, 0);
					pCanvasRegion->ClearAll();		
				}
			}
		}

		void ResizeBuffer()
		{
			if(m_pBufferDC != NULL)
			{
				HDC hdc = ::GetDC(m_hWindow);
				CClientRect rc(m_hWindow);
				CBufferDC* pBufferDC = new CBufferDC(hdc, rc, FALSE);
				::ReleaseDC(m_hWindow, hdc);
				
				::BitBlt(
					pBufferDC->GetSafeHdc(),
					0, 0, rc.Width(), rc.Height(),
					m_pBufferDC->GetSafeHdc(),
					0, 0, SRCCOPY);
				
				delete m_pBufferDC;
				m_pBufferDC = pBufferDC;
			}
		}

		void DeleteBuffer()
		{
			if(m_pBufferDC != NULL)
			{
				delete m_pBufferDC;
				m_pBufferDC = NULL;
			}
		}	
		
		void RedrawClipBuffer(HDC hDstDC, RECT& rcClip)
		{
			if(m_pBufferDC != NULL)
			{
				::BitBlt(
					hDstDC,
					rcClip.left,
					rcClip.top,
					rcClip.right - rcClip.left,
					rcClip.bottom - rcClip.top,
					m_pBufferDC->GetSafeHdc(),
					rcClip.left,
					rcClip.top,
					SRCCOPY);
			}
		}
		//////////////////////////////////////////////////////////////////////////
		void OnDraw(HDC hdc, RECT& rcClip)
		{
			IJuneRegion* pCanvasRegion = m_pDCCanvas->GetUpdateRegion();
			if(!pCanvasRegion->IsEmpty())
			{
				if(m_pBufferDC != NULL)
				{
					m_pDCCanvas->AttachDc(m_pBufferDC->GetSafeHdc());							
					m_pScene->DrawAtCanvas(m_pDCCanvas, 0, 0);				
					pCanvasRegion->ClearAll();
				}	
			}
			
			RedrawClipBuffer(hdc, rcClip);			
		}

		void OnJuneCanvasUpdate(IJuneRegion* pRegion)
		{
			int nRectCount = pRegion->EnumRects(NULL, 0);
			
			RECT* prcList = NULL;
			
			int nToAllocSize = sizeof(RECT) * nRectCount;
			BYTE* pMemory = new BYTE[nToAllocSize];

			if(pMemory != NULL)
			{
				memset(pMemory, 0, nToAllocSize);
				prcList = (RECT*)pMemory;

				pRegion->EnumRects(prcList, nRectCount);

				IJuneRegion* pCanvasRegion = m_pDCCanvas->GetUpdateRegion();

				for(int i=0; i<nRectCount; i++)
				{
					RECT rc = prcList[i];
					pCanvasRegion->InsertRect(&rc);
					if(m_pBufferDC != NULL)
					{
						::InvalidateRect(m_hWindow, &rc, FALSE);
					}
				}

				delete [] pMemory;
				pMemory = NULL;
				prcList = NULL;
			}

			pRegion->ClearAll();				
		}

	public:
		IWinDcCanvas* m_pDCCanvas;
		CBufferDC* m_pBufferDC;		
	};

	class CxImageCanvasWrap : 
		public CCanvasWrap,
		public IJuneCanvasUpdate
	{
	public:
		CxImageCanvasWrap(IJuneScene* pScene):CCanvasWrap(pScene)
		{			
			m_pxImage = NULL;
			m_pCanvas = NULL;
		}

		~CxImageCanvasWrap()
		{
		}

		void OnJuneCanvasUpdate(IJuneRegion* pRegion)
		{
			assert(m_pCanvas != NULL && m_pScene);
			
			int nRectCount = pRegion->EnumRects(NULL, 0);
			
			RECT* prcList = NULL;
			
			int nToAllocSize = sizeof(RECT) * nRectCount;
			BYTE* pMemory = new BYTE[nToAllocSize];

			if(pMemory != NULL)
			{
				memset(pMemory, 0, nToAllocSize);
				prcList = (RECT*)pMemory;

				pRegion->EnumRects(prcList, nRectCount);
			
				IJuneRegion* pCanvasRegion = m_pCanvas->GetUpdateRegion();
			
				for(int i=0; i<nRectCount; i++)
				{
					RECT rc = prcList[i];
					pCanvasRegion->InsertRect(&rc);
					if(m_pCanvas->GetXImage() != NULL)
					{	
						IJuneRegion* pCanvasRegion = m_pCanvas->GetUpdateRegion();
						if(!pCanvasRegion->IsEmpty())
						{
							if(m_pCanvas->GetXImage() != NULL)
							{
								m_pxImage->Erase(&rc, 0, 0);
								m_pScene->DrawAtCanvas(m_pCanvas, 0, 0);				
								pCanvasRegion->ClearAll();
							}	
						}
					}
				}
			
				HBITMAP hBitmap = m_pxImage->GetDIBHandle();
				UpdateLayerWindow(hBitmap);
			
				delete [] pMemory;

				pMemory = NULL;
				prcList = NULL;
			
				pRegion->ClearAll();		
			}
		}

		void CreateCanvas(HWND hWindow)
		{
			m_hWindow = hWindow;
			if(hWindow != NULL && ::IsWindow(hWindow))
			{
				if(m_pxImage == NULL)
				{
					GetTenioComponent(&m_pxImage);
				}
				
				if(m_pCanvas == NULL)
				{
					GetTenioComponent(&m_pCanvas);
				}
				
				if(m_pCanvas != NULL && m_pxImage != NULL)
				{
					RECT rc = {0};
					::GetClientRect(hWindow, &rc);
					
					m_pxImage->Create(rc.right, rc.bottom);	
					m_pCanvas->AttachXImage(m_pxImage);
					
					static_cast<INormalScene*>(m_pScene)->SetCanvasUpdate(this);
				}
			}		
		}

		BOOL UpdateLayerWindow(HBITMAP hBitmap)
		{
			assert(m_hWindow != NULL);
			BOOL bSuccess = FALSE;
			
			::GdiFlush();
			
			HDC hScreenDC = ::GetDC(NULL);
			HDC hMemoryDC = ::CreateCompatibleDC(hScreenDC);
			HBITMAP hOldBitmap = (HBITMAP)::SelectObject(hMemoryDC, hBitmap);
			
			BLENDFUNCTION blendPixelFunction= { 0x00, 0, 0xFF, 1 };
			
			RECT rect;		
			::GetWindowRect(m_hWindow, &rect);
			
			POINT pt = {rect.left, rect.top};
			SIZE size = {rect.right - rect.left, rect.bottom - rect.top};
			POINT ptSrc = {0, 0};
			
			typedef BOOL (WINAPI* lpfnUpdateLayeredWindow)(
				HWND hWnd, 
				HDC hdcDst,
				POINT* pptDst,
				SIZE* psize,
				HDC hdcSrc,
				POINT* pptSrc,
				COLORREF crKey, 
				BLENDFUNCTION* pblend,
				DWORD dwFlags);
			
			// 检查User32动态连接库是否已经装入
			static lpfnUpdateLayeredWindow pFnUpdateLayeredWindow  = NULL;
			if(pFnUpdateLayeredWindow == NULL)
			{
				HMODULE hUser32Dll = ::GetModuleHandle(_T("USER32.dll"));
				if(hUser32Dll)
				{
					pFnUpdateLayeredWindow  = (lpfnUpdateLayeredWindow)GetProcAddress(hUser32Dll, "UpdateLayeredWindow");
				} 
			}
			
			if(pFnUpdateLayeredWindow != NULL)
			{
				bSuccess = pFnUpdateLayeredWindow(
					m_hWindow, hScreenDC, &pt, &size, hMemoryDC,
					&ptSrc, 0, &blendPixelFunction, 2);
				
//				int nErr = GetLastError();
//				if(!bSuccess)
//				{
//					OutputErrorInfo(_T("UpdateBitmapWindow Failed last error = %d"), nErr);
//				}
			}
			
			::SelectObject(hMemoryDC, hOldBitmap);
			::DeleteDC(hMemoryDC);
			::ReleaseDC(NULL, hScreenDC);
			
			return bSuccess;
		}

	private:
		IxImageLib* m_pxImage;
		IXImageCanvas* m_pCanvas;
	};
}

#endif