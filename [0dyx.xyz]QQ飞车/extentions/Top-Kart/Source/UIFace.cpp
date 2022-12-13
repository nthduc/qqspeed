// ===============================================================================
// UIFace.CPP
//
// Purpose   : see header
//
// Latest Design doc link: http://
//
// 
// Author: Pucca Liang(mailto:puccaliang@tencent.com) 
// created:     1 2, 2007   10:36
// filename:    
// file path:   
// file base:   
// file ext:    .cpp
// 
// Copyright Tencent 1.2007
// =============================================================================== 
// <History> 
// 
// =============================================================================== 
// <QA> 
//  TODO: 
// =============================================================================== 

// =============================================================================== 
//Header 
// =============================================================================== 

#include "Top-KartPCH.h"
// System headers: 
#include <NiMaterialProperty.h>
#include <NiWireframeProperty.h>
#include "NiSourceTexture.h"

// Project headers: 
#include "UIFace.h"

// using namespaces:

// =============================================================================== 
//Macros 
// =============================================================================== 

// =============================================================================== 
//Local structure/class 
// =============================================================================== 

// =============================================================================== 
//Variables 
// =============================================================================== 

// External variables declaration: 

// Global variables: 

// Local variables: 


// ===============================================================================
//External/forward function declarations 
// =============================================================================== 

// ===============================================================================
//Local functions
// ===============================================================================

// ===============================================================================
//Global functions
// ===============================================================================

// ===============================================================================
//Class implementation
// ===============================================================================

namespace GFX
{

	CUIFace::CUIFace( float fFaceWidth, float fFaceHeight, NiPoint3 &faceCenterPoint, 
					NiPoint3 &faceNormal,  const char* pBackTexFile ) 
					:m_FaceNormal(faceNormal),m_BackImageID(0xffffffff),m_nCurrentElementID(0),
					m_spRenderTexture(NULL), m_bNeedUpdate(true)
	{
		IUIRenderer *pUIRenderer = GetUIRenderer() ;

		if (pUIRenderer != NULL)
		{
       		m_BackImageID = pUIRenderer->CreateImage(pBackTexFile) ;
		}
	    
		unsigned int uiWidth  = pUIRenderer->GetWidth(m_BackImageID) ;
		unsigned int uiHeight = pUIRenderer->GetHeight(m_BackImageID);

        _ConstructRenderTarget( uiWidth, uiHeight) ;
		_ConstructFace(fFaceWidth, fFaceHeight, faceCenterPoint, faceNormal) ;

	}

	CUIFace::CUIFace( float fFaceWidth, float fFaceHeight, NiPoint3 &faceCenterPoint, NiPoint3 &faceNormal, 
			          int nTexWidth, int nTexHeight  ) 
			          :m_FaceNormal(faceNormal), m_BackImageID(0xffffffff), m_nCurrentElementID(0),
					   m_bNeedUpdate(true)
	{	
        _ConstructRenderTarget( nTexWidth, nTexHeight) ;
		_ConstructFace(fFaceWidth, fFaceHeight, faceCenterPoint, faceNormal) ;
	}

	CUIFace::CUIFace(const char* pBackTexFile) 
		            : m_spFaceGeometry(NULL), m_spRenderTexture(NULL), m_spRenderTargetGroup(NULL),
					  m_FaceNormal(0.0f, 0.0f, 0.0f), m_nCurrentElementID(0), m_bNeedUpdate(true)
	{
		IUIRenderer *pUIRenderer = GetUIRenderer() ;

		if (pUIRenderer != NULL)
		{
			m_BackImageID = pUIRenderer->CreateImage(pBackTexFile) ;
		}

		unsigned int uiWidth  = pUIRenderer->GetWidth(m_BackImageID) ;
		unsigned int uiHeight = pUIRenderer->GetHeight(m_BackImageID);

		_ConstructRenderTarget( uiWidth, uiHeight) ;
	}

	CUIFace::CUIFace(int nTexWidth, int nTexHeight)
		            : m_spFaceGeometry(NULL), m_spRenderTexture(NULL), m_spRenderTargetGroup(NULL),
		              m_FaceNormal(0.0f, 0.0f, 0.0f), m_nCurrentElementID(0), m_bNeedUpdate(true),
					  m_BackImageID(0xffffffff)
	{
        _ConstructRenderTarget( nTexWidth, nTexHeight) ;
	}

	void CUIFace::_ConstructRenderTarget(unsigned int nTexWidth, unsigned int nTexHeight) 
	{
		NiDX9RendererPtr pkNiDX9Renderer = NiDX9Renderer::GetRenderer();

		if((!m_spRenderTexture) && (pkNiDX9Renderer != NULL) )
		{
			//Create RenderTarget
			NiTexture::FormatPrefs  kPrefs;
			kPrefs.m_ePixelLayout = NiTexture::FormatPrefs::TRUE_COLOR_32;
			kPrefs.m_eMipMapped   = NiTexture::FormatPrefs::NO ;
			kPrefs.m_eAlphaFmt    = NiTexture::FormatPrefs::SMOOTH;

			m_spRenderTexture = NiRenderedTexture::Create( nTexWidth, nTexHeight, pkNiDX9Renderer, kPrefs,
				Ni2DBuffer::MULTISAMPLE_NONE );
		}

		//Ni2DBuffer* pkSrcBuffer = m_spRenderTexture->GetBuffer();

		//Ni2DBuffer::RendererData* pkData = m_spRenderTexture->GetBuffer()->GetRendererData();

		m_spRenderTargetGroup = NiRenderTargetGroup::Create(m_spRenderTexture->GetBuffer(),pkNiDX9Renderer,true,true); 

	}
	void CUIFace::_ConstructFace( float fFaceWidth, float fFaceHeight, NiPoint3 &faceCenterPoint, NiPoint3 &faceNormal)
	{
		//加载纹理
		NiTexture::FormatPrefs perf;
		perf.m_ePixelLayout = NiTexture::FormatPrefs::TRUE_COLOR_32;
		perf.m_eMipMapped   = NiTexture::FormatPrefs::YES;
		perf.m_eAlphaFmt    = NiTexture::FormatPrefs::SMOOTH;

		//create the geometry data
		NiPoint3 * pkVertex = NiNew NiPoint3[4];
		float fHalfWidth  = fFaceWidth  / 2.0f ;
		float fHalfHeight = fFaceHeight / 2.0f ;
		pkVertex[0] = NiPoint3( -fHalfWidth,  fHalfHeight, 0.0f);
		pkVertex[1] = NiPoint3( -fHalfWidth, -fHalfHeight, 0.0f);
		pkVertex[2] = NiPoint3(  fHalfWidth, -fHalfHeight, 0.0f);
		pkVertex[3] = NiPoint3(  fHalfWidth,  fHalfHeight, 0.0f);

		NiPoint2 * pkTexcoords = NiNew NiPoint2[4];
		pkTexcoords[0] = NiPoint2(0.0f, 0.0f);
		pkTexcoords[1] = NiPoint2(0.0f, 1.0f);
		pkTexcoords[2] = NiPoint2(1.0f, 1.0f);
		pkTexcoords[3] = NiPoint2(1.0f, 0.0f);

		unsigned short * pusTrilist = (unsigned short *)NiMalloc(6 * sizeof(unsigned short));
		pusTrilist[0] = 0;
		pusTrilist[1] = 1;
		pusTrilist[2] = 2;
		pusTrilist[3] = 0;
		pusTrilist[4] = 2;
		pusTrilist[5] = 3;

		//create geometry
		m_spFaceGeometry = NiNew NiTriShape( 4, pkVertex, NULL, NULL, pkTexcoords, 1, NiGeometryData::NBT_METHOD_NONE, 2, pusTrilist );

		NiTexturingPropertyPtr pTexProperty = NiNew NiTexturingProperty ;
		pTexProperty->SetApplyMode(NiTexturingProperty::APPLY_MODULATE) ;
		//NiTexturePtr spShadowTexture = NiSourceTexture::Create("");
		pTexProperty->SetBaseTexture(m_spRenderTexture);                         

		NiAlphaPropertyPtr pAlphaProperty = NiNew NiAlphaProperty;
		pAlphaProperty->SetAlphaBlending(true);
		pAlphaProperty->SetSrcBlendMode(NiAlphaProperty::ALPHA_SRCALPHA);
		pAlphaProperty->SetDestBlendMode(NiAlphaProperty::ALPHA_INVSRCALPHA);

		NiZBufferPropertyPtr pZBufferProperty = NiNew NiZBufferProperty;
		pZBufferProperty->SetZBufferWrite(true);
		pZBufferProperty->SetZBufferTest(true);

		NiWireframePropertyPtr pWireframeProperty = NiNew NiWireframeProperty;
		pWireframeProperty->SetWireframe(false);

		NiMaterialProperty* pkMat = NiNew NiMaterialProperty;
		pkMat->SetEmittance(NiColor::WHITE);
		pkMat->SetAlpha(1.0f);
		m_spFaceGeometry->AttachProperty(pTexProperty);
		m_spFaceGeometry->AttachProperty(pAlphaProperty);
		m_spFaceGeometry->AttachProperty(pZBufferProperty);
		m_spFaceGeometry->AttachProperty(pkMat);
		m_spFaceGeometry->AttachProperty(pWireframeProperty);
		m_spFaceGeometry->SetWorldRotate(NiMatrix3::IDENTITY);

		//先用默认值来初始化阴影实体，然后检测传入的初始化参数是否合法，再进行实体的修改

		assert(faceNormal.SqrLength() > 0.99f && faceNormal.SqrLength() < 1.01f) ;
		if (!( faceNormal.SqrLength() > 0.99f && faceNormal.SqrLength() < 1.01f ))
		{
			return  ;
		}
		NiPoint3 vY = faceNormal.UnitCross( NiPoint3::UNIT_X );
		NiPoint3 vX = vY.Cross( faceNormal );

		NiMatrix3 mat(vX, vY, faceNormal);

		m_spFaceGeometry->SetRotate( mat ); 
		m_spFaceGeometry->SetTranslate( faceCenterPoint );

		m_spFaceGeometry->UpdateEffects();
		m_spFaceGeometry->UpdateProperties();
		m_spFaceGeometry->UpdateNodeBound();
		m_spFaceGeometry->Update(0.0f);

		return  ;

	}

	int  CUIFace::AddFaceElement(IUIFaceElement *pElement) 
	{
		if ( !pElement )
		{
			return 0 ;
		}
		pElement->m_nElementID = GenerateElementID();

		std::vector <IUIFaceElement*>::iterator it ;

		for ( it = m_vecElements.begin(); it != m_vecElements.end() ; it++)
		{
			if ( pElement->m_nLayer < (*it)->m_nLayer)
			{
				break;
			}
		}
		m_vecElements.insert(it, pElement) ;
		return 1 ;
	}


	void CUIFace::UpdateFace(bool bForece)
	{
		if (!m_bNeedUpdate && !bForece)
		{
             return ;
		}

        _SortElementsByLayerID() ;

		NiDX9RendererPtr pkNiDX9Renderer = NiDX9Renderer::GetRenderer();

		IUIRenderer *pUIRenderer = GetUIRenderer() ;
		pkNiDX9Renderer->SetBackgroundColor(NiColorA(0, 0, 0, 0));
		pkNiDX9Renderer->BeginUsingRenderTargetGroup(m_spRenderTargetGroup,NiRenderer::CLEAR_ALL);
	    
		pUIRenderer->BeginUI();
		pUIRenderer->BeginRenderText();
		
		if(m_BackImageID != 0xffffffff)
		{
			RECT rcScr, rcDest ;
			rcScr.left   = rcDest.left   = 0 ;
			rcScr.top    = rcDest.top    = 0 ;
			rcScr.right  = rcDest.right  = pUIRenderer->GetWidth(m_BackImageID) ;
			rcScr.bottom = rcDest.bottom = pUIRenderer->GetHeight(m_BackImageID) ;
			
			pUIRenderer->DrawImage(m_BackImageID, FILLMODE_SCALE, &rcScr, &rcDest, 255) ;
		}
		for ( size_t i = 0 ; i < m_vecElements.size() ; ++i)
		{
			m_vecElements[i]->DrawElement() ;
		}
        pUIRenderer->EndRenderText();
		pUIRenderer->EndUI() ;
     
		//pkNiDX9Renderer->EndUsingRenderTargetGroup();

		if (m_spFaceGeometry)
		{
			m_spFaceGeometry->UpdateEffects();
			m_spFaceGeometry->UpdateProperties();
			m_spFaceGeometry->UpdateNodeBound();
			m_spFaceGeometry->Update(0.0f);

		}		

		pkNiDX9Renderer->EndUsingRenderTargetGroup();

		m_bNeedUpdate = false ;

	}

	unsigned int CUIFace::GenerateElementID()
	{
		return m_nCurrentElementID++ ;
	}

	CUIFace::~CUIFace()
	{
		m_spFaceGeometry      = NULL ;
		m_spRenderTargetGroup = NULL ;
		m_spRenderTexture     = NULL ;

		std::vector <IUIFaceElement*>::iterator it ;
		IUIFaceElement* pElement = NULL ;
		int a = m_vecElements.size() ;
		for ( it = m_vecElements.begin(); it != m_vecElements.end() ; it++)
		{
			pElement = *it ;
			NiDelete pElement ;
		}
		m_vecElements.clear() ;

		IUIRenderer* pUIRenderer = GetUIRenderer() ;
		pUIRenderer->ReleaseImage(m_BackImageID) ;

	}

	NiTriShapePtr CUIFace::GetGeometry() const
	{
		return m_spFaceGeometry ;
	}

	void CUIFace::RenderFace(NiRenderer* pRender)
	{
		if ( (m_spFaceGeometry) && (pRender) )
		{
			m_spFaceGeometry->RenderImmediate(pRender) ;
		}
	}

	void CUIFace::SetCenterPoint( NiPoint3 &faceCenterPoint) 
	{
        m_spFaceGeometry->SetTranslate(faceCenterPoint) ;
	}

	void CUIFace::ChangeBackImage( const char* pBackTexFile) 
	{
		IUIRenderer *pUIRenderer = GetUIRenderer() ;         
		pUIRenderer->ReleaseImage(m_BackImageID) ;
		m_BackImageID = pUIRenderer->CreateImage(pBackTexFile) ;
	}

	void CUIFace::ClearTexture() 
	{
		NiDX9RendererPtr pkNiDX9Renderer = NiDX9Renderer::GetRenderer();

		IUIRenderer *pUIRenderer = GetUIRenderer() ;
		pkNiDX9Renderer->BeginUsingRenderTargetGroup(m_spRenderTargetGroup,NiRenderer::CLEAR_ALL);
		
        pkNiDX9Renderer->EndUsingRenderTargetGroup();
	}

	NiRenderedTexturePtr CUIFace::GetRenderTarget()
	{
		return m_spRenderTexture ;
	}

	unsigned int CUIFace::CreateImageElement( RECT& rcSrc, RECT& rcDest, unsigned char byImageAlpha, 
		                                      const char* lpszImageFileName, unsigned int nLayer )
	{
         CUIFaceImageElement* pImageElement = NiNew CUIFaceImageElement( rcSrc, rcDest, byImageAlpha, lpszImageFileName, nLayer) ;
		 AddFaceElement(pImageElement) ;
		 return pImageElement->m_nElementID ;
	}

	unsigned int CUIFace::CreateTextElement(  const char* szText, const char* szFontName, DWORD dwTextColor , 
		                                      RECT &rcDestRect, RECT *rcClip, unsigned int format, unsigned int nLayer  ) 
	{
         CUIFaceTextElement* pTextElement = NiNew CUIFaceTextElement( szText, szFontName, dwTextColor, rcDestRect, rcClip, format, nLayer) ;
		 AddFaceElement(pTextElement) ;
		 return pTextElement->m_nElementID ;
	}

	void CUIFace::DestroyElement(unsigned int nElementID ) 
	{
		std::vector <IUIFaceElement*>::iterator it ;

		for ( it = m_vecElements.begin(); it != m_vecElements.end() ; it++)
		{
			if ( (*it)->m_nElementID == nElementID)
			{
				break;
			}
		}
		NiDelete (*it) ;
		m_vecElements.erase(it) ;  
		int a = m_vecElements.size() ;

	}

    IUIFaceElement* CUIFace::GetFaceElement(unsigned int hElementID) 
	{
         if ( hElementID >= m_vecElements.size())
         {
			 return NULL ;
         }
		 return m_vecElements[hElementID] ;
	}

    void CUIFace::MarkUpdate()
	{
		m_bNeedUpdate = true ;
	}

	void CUIFace::_SortElementsByLayerID()
	{ 
		int i,j;
	    bool bExchange; 
		IUIFaceElement *pTemp ;
		int n = m_vecElements.size() ;

	    for( i = 0 ; i < n ; i++ )
		{
		    bExchange = false ;
			for( j = n - 1 ; j > i ; j-- )
			{
				if( m_vecElements[j]->m_nLayer < m_vecElements[j-1]->m_nLayer )
				{
					pTemp              = m_vecElements[j]; 
					m_vecElements[j] = m_vecElements[j-1]  ; 
					m_vecElements[j-1]   = pTemp  ;
					bExchange          = true; 
				}
				if( !bExchange )
				{
					return;
				}
			}
		}

	} 
        

}