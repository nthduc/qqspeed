#include ".\fbguirenderdriver.h"

#include "NiDX9Renderer.h"

#include <d3d9.h>
#include <D3dx9math.h>
namespace FBGUI
{
	const unsigned int MAX_D3D_VERTEX_BUFFER = 600 ;
	void CGUIRenderDriver::PrepareRender()
	{
		m_pd3dDevice->SetVertexShader(NULL);
		m_pd3dDevice->SetPixelShader(NULL);

		m_pd3dDevice->SetRenderState( D3DRS_LIGHTING, FALSE ); 
		m_pd3dDevice->SetRenderState(D3DRS_ZENABLE,FALSE); 
		m_pd3dDevice->SetRenderState(D3DRS_ZWRITEENABLE, false); 

		m_pd3dDevice->SetRenderState ( D3DRS_CULLMODE ,D3DCULL_NONE );
		m_pd3dDevice->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_POINT );
		m_pd3dDevice->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_POINT );
		m_pd3dDevice->SetSamplerState( 0, D3DSAMP_MIPFILTER, D3DTEXF_POINT);
		m_pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE,TRUE );
		m_pd3dDevice->SetRenderState( D3DRS_SRCBLEND,  D3DBLEND_SRCALPHA);
		m_pd3dDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

		m_pd3dDevice->SetRenderState( D3DRS_STENCILENABLE, FALSE);
		m_pd3dDevice->SetRenderState( D3DRS_CLIPPLANEENABLE, FALSE);

		m_pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );

		m_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
		m_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );
		m_pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE );
		m_pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
		m_pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );

		// Render the vertex buffer contents

		m_pd3dDevice->SetFVF( FONT2DVERTEX::FVF_2D );
	}
	int CGUIRenderDriver::AddTextToRenderBuffer( const CGUIRect& rSrcRect, const CGUIRect& rDestRect,  float fHorShear) 
	{
		float tx1 = 0.0f;
		float ty1 = 0.0f;
		float tx2 = 0.0f;
		float ty2 = 0.0f;

		if (m_pRenderTexture)
		{
			tx1 = (float)rSrcRect.TopLeft().mX/m_pRenderTexture->GetTexWidth();
			ty1 = (float)rSrcRect.TopLeft().mY/m_pRenderTexture->GetTexHeight();
			tx2 = (float)rSrcRect.BottomRight().mX/m_pRenderTexture->GetTexWidth();
			ty2 = (float)rSrcRect.BottomRight().mY/m_pRenderTexture->GetTexHeight();
		}

		float  w  = (float)rDestRect.Width();
		float  h  = (float)rDestRect.Height();

		float  sx = (float)rDestRect.TopLeft().mX;
		float  sy = (float)rDestRect.TopLeft().mY;
        FONT2DVERTEX pVertices[6];
		pVertices[0] = FONT2DVERTEX(sx ,     sy + h    , 0.0f,1.0f, m_dwSystemColor, tx1, ty2);
		pVertices[1] = FONT2DVERTEX(sx + fHorShear * h ,     sy,     0.0f,1.0f, m_dwSystemColor, tx1, ty1);
		pVertices[2] = FONT2DVERTEX(sx + w , sy + h    , 0.0f,1.0f, m_dwSystemColor, tx2, ty2);
		pVertices[3] = FONT2DVERTEX(sx + w + + fHorShear * h , sy,     0.0f,1.0f, m_dwSystemColor, tx2, ty1);
		pVertices[4] = FONT2DVERTEX(sx + w , sy + h    , 0.0f,1.0f, m_dwSystemColor, tx2, ty2);
		pVertices[5] = FONT2DVERTEX(sx + fHorShear * h ,     sy,     0.0f,1.0f, m_dwSystemColor, tx1, ty1);
        
		int i = 0 ;
		for ( i = 0 ; i < 6 ; ++i)
		{
            m_vecVertexBuffer.push_back( pVertices[i] ) ;
		}

		return true;
	}

	CGUITexture* CGUIRenderDriver::CreateTexture( const char* pTexName, int vWidth, int vHeight ) 
	{
		HRESULT hr;
		static bool bIsTexCreated = false;

		if (FAILED(hr = D3DXCreateTexture(m_pd3dDevice, vWidth, vHeight, 1, 0, D3DFMT_A8B8G8R8, D3DPOOL_MANAGED, &m_TCasheTexture)))
		{ 
			return NULL;
		}

		m_pFontTexture = new CGUITexture;
		m_pFontTexture->m_TextCasheTexture = m_TCasheTexture;
		m_pFontTexture->mTexWidth = vWidth;
		m_pFontTexture->mTexHeight = vHeight;
		//m_pFontTexture->mTexName.c_str() =pTexName;
		return m_pFontTexture;
	}
	CGUIRenderDriver::CGUIRenderDriver( ):m_pRenderTexture(NULL), m_dwSystemColor(0)	
	{
		NiDX9Renderer *pDX9Renderer = NULL;
		pDX9Renderer = NiSmartPointerCast(NiDX9Renderer,NiRenderer::GetRenderer());
		m_pd3dDevice = pDX9Renderer->GetD3DDevice();
		m_TCasheTexture =NULL;

		m_pd3dDevice->CreateVertexBuffer( MAX_D3D_VERTEX_BUFFER * sizeof(FONT2DVERTEX), 0, FONT2DVERTEX::FVF_2D,
			                              D3DPOOL_MANAGED, &m_pVB, NULL ) ;
		m_pd3dDevice->CreateVertexBuffer( 2 * sizeof(FONT2DVERTEX), 0 , FONT2DVERTEX::FVF_2D,
			                              D3DPOOL_MANAGED, &m_pLine, NULL ) ;
	}

	 CGUIRenderDriver::~CGUIRenderDriver( ) 
	 {
		 if( m_pVB != NULL )
			 m_pVB->Release();
		 if (m_pLine != NULL)
		 {
			 m_pLine->Release() ;
		 }
	 }
	void CGUIRenderDriver::SetCurrentTexture(CGUITexture* pTexture ) 
	{
		if (pTexture)
		{
			m_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_MODULATE   );
		    m_pd3dDevice->SetTexture(0 , pTexture->m_TextCasheTexture);
		}
		else
		{
			m_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_DISABLE   );
		}

        m_pRenderTexture = pTexture ;
	}
	void CGUIRenderDriver::SetSystemColor(DWORD dwColor)
	{
		m_dwSystemColor = dwColor ;
		int i = 0 ;
		for( i = 0 ; i <  m_vecVertexBuffer.size() ; ++i )
		{
             m_vecVertexBuffer[i].color = m_dwSystemColor ;
		}

	}
	int CGUIRenderDriver::DrawLine( const CGUIPoint& point1, const CGUIPoint& point2 ) 
	{
		FONT2DVERTEX* pVertices;
		if( FAILED( m_pLine->Lock( 0, 0, (void**)&pVertices, 0 ) ) )
			return E_FAIL;
        pVertices[0] = FONT2DVERTEX((float)point1.mX, (float)point1.mY, 0.0f,1.0f, m_dwSystemColor, 0.0, 0.0);
		pVertices[1] = FONT2DVERTEX((float)point2.mX, (float)point2.mY, 0.0f,1.0f, m_dwSystemColor, 0.0, 0.0);
		m_pLine->Unlock();

		m_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_DISABLE );
		m_pd3dDevice->SetStreamSource( 0, m_pLine, 0, sizeof(FONT2DVERTEX) );
		m_pd3dDevice->DrawPrimitive( D3DPT_LINELIST, 0, 1 );
		m_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_MODULATE   );
		m_pd3dDevice->SetRenderState( D3DRS_SCISSORTESTENABLE, FALSE);
	}

	int CGUIRenderDriver::DrawTextRenderBuffer(RECT *rcClip) 
	{

		if (rcClip)
		{
		    m_pd3dDevice->SetRenderState( D3DRS_SCISSORTESTENABLE, TRUE);
		    m_pd3dDevice->SetScissorRect(rcClip);
		}
		else
		{
		    m_pd3dDevice->SetRenderState( D3DRS_SCISSORTESTENABLE, FALSE);
		}
		m_pd3dDevice->SetStreamSource( 0, m_pVB, 0, sizeof(FONT2DVERTEX) );

		int nPrimitiveCount = m_vecVertexBuffer.size();
		int i = 0 ;
        FONT2DVERTEX *pVertices ;
        for( i = 0 ; i < nPrimitiveCount / MAX_D3D_VERTEX_BUFFER ; ++i )
		{
            m_pVB->Lock(0, 0, (void**)&pVertices , 0) ;
			memcpy(pVertices, &m_vecVertexBuffer[i*MAX_D3D_VERTEX_BUFFER], sizeof(FONT2DVERTEX) * MAX_D3D_VERTEX_BUFFER) ;
			m_pVB->Unlock() ;

			m_pd3dDevice->DrawPrimitive( D3DPT_TRIANGLELIST, 0, MAX_D3D_VERTEX_BUFFER / 3 );
      
		}

		HRESULT hr = m_pVB->Lock(0, 0, (void**)&pVertices , 0) ;
		memcpy( pVertices ,&m_vecVertexBuffer[i*MAX_D3D_VERTEX_BUFFER], sizeof(FONT2DVERTEX) * (nPrimitiveCount - i*MAX_D3D_VERTEX_BUFFER)) ;
        int k = i * MAX_D3D_VERTEX_BUFFER  ;  
		m_pVB->Unlock() ;

	    hr = m_pd3dDevice->DrawPrimitive( D3DPT_TRIANGLELIST, 0, (nPrimitiveCount - k) / 3 );

       

       return 1 ;
	}

	void CGUIRenderDriver::EndRender()
	{
		m_vecVertexBuffer.clear() ;
		m_pd3dDevice->SetRenderState( D3DRS_SCISSORTESTENABLE, false);
	}
}