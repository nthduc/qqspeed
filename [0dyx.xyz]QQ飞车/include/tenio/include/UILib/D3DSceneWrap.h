#ifndef __D3DCANVASWRAP_H__
#define __D3DCANVASWRAP_H__

#include "teniopointer_t.h"
#include "memoryalloctor_i.h"
#include "layer/scenefactory_i.h"
#include "layer/normalscene_i.h"
#include "june/ximagecanvas_i.h"

namespace Tenio
{
	class CSceneWrap
	{
	public:
		CSceneWrap()
		{
			m_pScene = NULL;
			m_pSceneUpdate = NULL;
			m_pCanvasUpdate = NULL;
		}

		virtual ~CSceneWrap()
		{
			ReleaseScene();
		}

		void SetCanvasUpdate(IJuneCanvasUpdate* pCanvasUpdate)
		{
			m_pCanvasUpdate = pCanvasUpdate;

			if(m_pScene != NULL)
			{
				if(m_pScene->GetSceneID() == ESI_NORMAL)
				{
					static_cast<INormalScene*>(m_pScene)->SetCanvasUpdate(pCanvasUpdate);				
				}
			}
		}

		BOOL CreateScene(IJuneCanvasUpdate* pCanvasUpdate)
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
		IJuneCanvasUpdate* m_pCanvasUpdate;
	};

	// D3DCanvas��װ��
	class CD3DCanvasWrap : public CSceneWrap, public IJuneCanvasUpdate
	{
	public:
		CD3DCanvasWrap()
		{
		}

		virtual ~CD3DCanvasWrap()
		{
		}

		// ��ʼ��Canvas, szCanvasָ��Canvas�Ĵ�С
		void Initialize(const SIZE& szCanvas)
		{			
			m_pxImageLib->Create(szCanvas.cx, szCanvas.cy);	

			// ȡ��buffer��,��ǿ��CxImage����һ��buffer����,�����ڵ���GetPixelBuffer֮ǰ�Ļ��Ʋ���
			// �����ử�ڻ�����
			BYTE* pData = NULL;
			long lSize = 0;
			m_pxImageLib->GetPixelBuffer(pData, lSize);

			m_pxImageLib->Erase(NULL, 0, 0);

			m_pCanvas->AttachXImage(m_pxImageLib);
		}
		
		// ����ˢ��Canvas, rcClipָ�����о���
		void OnDraw()
		{		
			// ���������
			ClearDirtyBuffer();

			if(m_pCanvas != NULL && m_pScene != NULL)
			{
				m_pScene->DrawAtCanvas(m_pCanvas, 0, 0);
			}
		}

		void ClearDirtyBuffer()
		{
			IJuneRegion* pRegion = m_pCanvas->GetUpdateRegion();
			int nRectCount = pRegion->EnumRects(NULL, 0);	
			RECT* prcList = NULL;
			prcList = (RECT*)m_pMemoryAlloctor->Alloc(NULL, sizeof(RECT)*nRectCount);
			memset(prcList, 0, sizeof(RECT) * nRectCount);
			pRegion->EnumRects(prcList, nRectCount);

			for(int i = 0; i < nRectCount; ++i)
			{
				m_pxImageLib->Erase(prcList + i, 0, 0);
			}

			m_pMemoryAlloctor->Free((LPVOID)prcList);
		}

		void OnJuneCanvasUpdate(IJuneRegion* pRegion)
		{
			// ö�ٸ�������
			int nRectCount = pRegion->EnumRects(NULL, 0);	
			RECT* prcList = NULL;
			prcList = (RECT*) m_pMemoryAlloctor->Alloc(NULL, sizeof(RECT)*nRectCount);
			memset(prcList, 0, sizeof(RECT)*nRectCount);
			pRegion->EnumRects(prcList, nRectCount);
			IJuneRegion* pCanvasRegion = m_pCanvas->GetUpdateRegion();
			
			// �����������������б�
			for(int i=0; i<nRectCount; i++)
			{
				RECT rc = prcList[i];
				pCanvasRegion->InsertRect(&rc);
			}

			// �ͷ���Դ
			m_pMemoryAlloctor->Free((LPVOID)prcList);
			pRegion->ClearAll();
		}

		// ������ص���, pBuffΪָ�����ص����ָ��, lSizeΪ���ص���Ĵ�С
		void GetPixelBuffer(BYTE* &pBuff, long &lSize)
		{
			// UILib�ڵ�һ�ε���GetPixerBuffer��Ỻ��Buffer, ������Canvas���ͷź��Զ��ͷ�
			m_pxImageLib->GetPixelBuffer(pBuff, lSize);
		}

		IJuneRegion* GetClipRegion()
		{
			return m_pCanvas->GetUpdateRegion();
		}

	private:
		TTenioPtr<IXImageCanvas> m_pCanvas;           // Canvasָ��
		TTenioPtr<IxImageLib> m_pxImageLib;           // xImageLibָ��
		TTenioPtr<IMemoryAlloctor> m_pMemoryAlloctor; // ���ڼ�¼��������
	};
} // Tenio

#endif // __D3DCANVASWRAP_H__