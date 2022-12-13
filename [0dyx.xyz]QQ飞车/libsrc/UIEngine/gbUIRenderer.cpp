#include "gbUIRenderer.h"
#include "gbTextureManager.h"
#include "gbFontManager.h"
#include "NiDX9Renderer.h"
#include "NiDX9SourceTextureData.h"
#include "NiMain.h"
#include "Utility.h"


//#define UI_CLIPDOWN
//#define UI_SHUTDOWN
//#define UI_LINEDOWN
//#define UI_FONTDOWN

#ifdef ENGINE_NEW
#include "../MemoryCheck.h"
#endif

struct SCREEN_VERTEX
{
	float x, y, z, h;
	D3DCOLOR color;
	float tu, tv;

	static DWORD FVF;
};
DWORD SCREEN_VERTEX::FVF = D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1;

struct MASK_VERTEX
{
	float x, y, z;
	float tu, tv;
    float tuMask, tvMask;
};
#define MASKFVF  D3DFVF_XYZ|D3DFVF_TEX2

#define SAFE_DO(r) if(m_pFontManager->LostDevice())return (r);
#define SAFE_DO_ if(m_pFontManager->LostDevice())return ;

int ISimpleInterface::Release()
{
	m_RefCount--;
	if( m_RefCount <= 0 )
	{
		delete this;
		return 0;
	}
	else return m_RefCount;
}

IUIRenderer *GetUIRenderer()
{
	return gbUIRenderer::GetRenderer();
}

void SetUIRenderer(IUIRenderer *pUIRenderer)
{
	gbUIRenderer::SetUIRenderer(pUIRenderer) ;
}

IUIRenderer *CreateUIRenderer(HWND hRenderWindow)
{
	return gbUIRenderer::GetRenderer(hRenderWindow);
}
bool gbUIRenderer::OnUILostDevice(void* pvData)
{
	gbFontManager::GetManager()->OnLostDevice();
	return true;
}

bool gbUIRenderer::OnUIResetDevice(bool bBeforeReset,void* pvData)
{
	if(bBeforeReset)
	{
		gbFontManager::GetManager()->OnLostDevice();
	}
	else
	{
		gbFontManager::GetManager()->OnRestDevice();
	}
	return true;
}

gbUIRenderer* gbUIRenderer::ms_pUIRender = NULL;
gbUIRenderer::gbUIRenderer()
{
//	assert(gbGfxDevice::GetDevice());
//	gbGfxDevice::GetDevice()->SetCreateUI(true);

	NiDX9Renderer *pDX9Renderer = NULL;
	pDX9Renderer = NiSmartPointerCast(NiDX9Renderer,NiRenderer::GetRenderer());

//	m_uiRenderWidth = gbGfxDevice::GetDevice()->GetWidth();
//	m_uiRenderHeight = gbGfxDevice::GetDevice()->GetHeight();
    InitRenderer();



}

gbUIRenderer::gbUIRenderer(HWND hRenderWindow) 
{
    if ( hRenderWindow )
    {
		RECT kRect;
		GetClientRect(hRenderWindow, &kRect);
		m_uiRenderWidth = kRect.right - kRect.left;
		m_uiRenderHeight = kRect.bottom - kRect.top;
    }
	NiDX9Renderer *pDX9Renderer = NULL;
	pDX9Renderer = NiSmartPointerCast(NiDX9Renderer,NiRenderer::GetRenderer());
    InitRenderer();
}
void gbUIRenderer::InitRenderer()
{
	ms_pUIRender = this;
	m_pTexManager   = gbTextureManager::GetManager();
	m_pFontManager  = gbFontManager::GetManager()   ;

	m_fRScreenY = 1.0f/m_uiRenderWidth;
	m_fRScreenX = 1.0f/m_uiRenderHeight;

	m_ClipRect.iTop    = 0;
	m_ClipRect.iLeft   = 0;
	m_ClipRect.iHeight = m_uiRenderHeight;
	m_ClipRect.iWidth  = m_uiRenderWidth;
	m_bEnableClip      = false;

	m_eApplyMode = NiTexturingProperty::APPLY_MODULATE;

	m_imgScreenUpdateBlock = 0 ;
	m_nNumScreenBlock      = 0 ;

	m_pRender = new CGUIRenderDriver;
	m_pGUISystem = new CGUISystem(m_pRender);

	m_pFontCanvas=  new CGUICanvas;

}
gbUIRenderer::~gbUIRenderer()
{
	if(m_pTexManager)
		delete m_pTexManager;
	if(m_pFontManager)
		delete m_pFontManager;
	delete	m_pFontCanvas;
	CGUIFontManager::GetSingleton( ).DestoryAll();
	delete  m_pGUISystem;
	delete  m_pRender;

}

#ifdef ENGINE_NEW
gbUIRenderer* gbUIRenderer::GetRenderer()
{
	return ms_pUIRender?ms_pUIRender:new gbUIRenderer();
}

#endif
void gbUIRenderer::SetUIRenderer(IUIRenderer *pUIRenderer) 
{
	ms_pUIRender = dynamic_cast<gbUIRenderer*>(pUIRenderer) ;
}
IMGID gbUIRenderer::CreateImage(const char *path)
{
	IMGID imgID;
	m_pTexManager->Load(path,imgID);
	return imgID;
}

IMGID gbUIRenderer::CreateImage(int uiWidth, int uiHeight,int attrib)
{
	IMGID imgID;
	 m_pTexManager->CreateSourceTexture(uiWidth,uiHeight,attrib,imgID);
	return imgID;
}

IMGID gbUIRenderer::CreateRenderImage(int uiWidth, int uiHeight)
{
	return m_pTexManager->CreateRenderImage(uiWidth,uiHeight);
}


IMGID gbUIRenderer::MakeGrayImage(IMGID ImageID)
{
	return  m_pTexManager->MakeGrayImage(ImageID);
}

void gbUIRenderer::SetImageUsedRect(IMGID ImageID,int iTop,int iLeft,int iHeight,int iWidth)
{
	TextureRect* pImgRect = m_pTexManager->GetTextureRect(ImageID);
	pImgRect->iLeft = iLeft;
	pImgRect->iTop = iTop;
	pImgRect->iWidth = iWidth;
	pImgRect->iHeight =iHeight;

}

void gbUIRenderer::SetImageColor(IMGID ImageID,DWORD color)
{
	TextureRect* pImgRect = m_pTexManager->GetTextureRect(ImageID);
	pImgRect->dwColor = color;
}

bool gbUIRenderer::HaveAlphaChannels(IMGID ImageID) 
{
    return m_pTexManager->HaveAlphaChannels(ImageID) ;
}

void gbUIRenderer::UpdateImage(IMGID ImageID,int iTop,int iLeft,TBitImage* pImg)
{
	m_pTexManager->UpdateSourceTexture(iTop,iLeft,pImg,ImageID);
}

 void gbUIRenderer::UpdateImage(IMGID ImageID,int iTop,int iLeft,TBitImage* pImg , RECT *rcRegion) 
{
	m_pTexManager->UpdateSourceTexture(iTop,iLeft,pImg,ImageID,rcRegion);
}

void gbUIRenderer::ReleaseImage(IMGID ImageID)
{
	m_pTexManager->ReleaseTexture(ImageID);
}

int gbUIRenderer::GetHeight(IMGID ImageID)
{
	return (int)m_pTexManager->GetHeight(ImageID);
}

int gbUIRenderer::GetWidth(IMGID ImageID)
{
	return (int)m_pTexManager->GetWidth(ImageID);
}

float gbUIRenderer::GetPixelAlpha(IMGID ImageID,unsigned int uiX,unsigned int uiY)
{
	return m_pTexManager->GetPixelAlpha(ImageID,uiX,uiY);
}

DWORD gbUIRenderer::GetPixelColor(IMGID ImageID,unsigned int uiX,unsigned int uiY)
{
	return m_pTexManager->GetPixelColor(ImageID , uiX , uiY) ;
}


void gbUIRenderer::Reg3DImage(IMGID ImageID,int iHeight,int iWidth,float fX,float fY,float fZ)
{
	Billboard3DElem Elem;
	Elem.fX = fX;
	Elem.fY = fY;
	Elem.fZ = fZ;
	Elem.uiID = ImageID;
	TextureRect* pImgRect = m_pTexManager->GetTextureRect(ImageID);
	Elem.Color = pImgRect->dwColor;

	void* p;
	//use this to record image info
	int* pIter =(int*) Elem.Info;
	*pIter = 0;pIter++; //first is Null to lable it as image : )
	*pIter = iHeight;pIter++;
	*pIter = iWidth;pIter++;
	*pIter = pImgRect->iLeft;pIter++;
	*pIter = pImgRect->iTop;pIter++;
	*pIter = pImgRect->iWidth;pIter++;
	*pIter = pImgRect->iHeight;pIter++;
	p = &fX; *pIter = *((int*)p);pIter++;
	p = &fY; *pIter = *((int*)p);pIter++;
	p = &fZ; *pIter = *((int*)p);
	m_3DSet.push_back(Elem);
}

void gbUIRenderer::Draw3DElem(NiPoint3 CamPos)
{
	SAFE_DO_
	D3DXMATRIX WorldMatrix;
	D3DXMatrixIdentity(&WorldMatrix);
	m_pFontManager->GetDx9Device()->SetTransform(D3DTS_WORLD,&WorldMatrix);
	m_pFontManager->GetSprite()->Begin(D3DXSPRITE_ALPHABLEND|D3DXSPRITE_OBJECTSPACE);
	std::vector<Billboard3DElem>::iterator it;
	for(it = m_3DSet.begin();it!=m_3DSet.end();it++)
	{
		Billboard3DElem Elem = *it;
		NiPoint3 Dir = NiPoint3(Elem.fX,Elem.fY,Elem.fZ) - CamPos;
		float fPitch,fYaw;
		fPitch = NiASin(Dir.z/Dir.Length());
		if(Dir.x>0)
		{
			fYaw = NiATan(Dir.y/Dir.x);
		}
		else
		{
			fYaw = NI_PI + NiATan(Dir.y/Dir.x);
		}

		if(Elem.Info[0] == char(0x0))
		{
			//image
			float fX,fY,fZ;
			int* pIter =(int*) Elem.Info;
			pIter++; //shift first Null
			int iHeight = *pIter;pIter++;
			int iWidth  = *pIter;pIter++;
			int iImgLeft = *pIter;pIter++;
			int iImgTop = *pIter;pIter++;
			int iImgWidth = *pIter;pIter++;
			int iImgHeight = *pIter;pIter++;
			fX = *((float*)pIter);pIter++;
			fY = *((float*)pIter);pIter++;
			fZ = *((float*)pIter);

			TextureRect* pImgRect = m_pTexManager->GetTextureRect(Elem.uiID);
			NiTexture::RendererData * pRenderData = pImgRect->spTexture->GetRendererData();
			NiDX9TextureData* pDx9Data = NiSmartPointerCast(NiDX9TextureData,pRenderData);
			D3DXMATRIX temp,Matrix;
			RECT TempRT;
			FLOAT scaleX = (FLOAT)iWidth/iImgWidth;
			FLOAT scaleY = (FLOAT)iHeight/iImgHeight;
			D3DXMatrixScaling(&temp, scaleX,scaleY, 1.0f);
			D3DXMatrixRotationX(&Matrix, fPitch-NI_PI*0.5f);
			temp *= Matrix;
			D3DXMatrixRotationZ(&Matrix, fYaw-NI_PI*0.5f);
			temp *= Matrix;
			D3DXMatrixTranslation(&Matrix,fX,fY,fZ);
			temp *= Matrix;

			TempRT.left = iImgLeft;
			TempRT.top = iImgTop;
			TempRT.right = iImgLeft + iImgWidth;
			TempRT.bottom = iImgTop + iImgHeight;

			//D3DXVECTOR3 Pos = D3DXVECTOR3(0.0f,0.0f,0.0f);
			D3DXVECTOR3 Pos = D3DXVECTOR3(-float(iWidth)*0.5f/scaleX,0.0f,0.0f);
			//D3DXVECTOR3 Pos = D3DXVECTOR3(fX,fY,fZ);
			m_pFontManager->GetSprite()->SetTransform(&temp);
			m_pFontManager->GetSprite()->Draw(LPDIRECT3DTEXTURE9(pDx9Data->GetD3DTexture())
				,&TempRT,NULL,&Pos,Elem.Color);
		}
		else
		{
			//string
			RECT re;
			m_pFontManager->GetDXFont(Elem.uiID)->DrawText(NULL,Elem.Info,
				-1,&re,DT_NOCLIP | DT_CALCRECT,0x0);
			re.bottom = re.bottom - re.top;
			re.right = (re.right - re.left)/2+1;
			re.top = 0;
			re.left = -re.right;
			
			D3DXMATRIX temp,Matrix;
			D3DXMatrixRotationX(&temp, fPitch-NI_PI*0.5f);
			D3DXMatrixRotationZ(&Matrix, fYaw-NI_PI*0.5f);
			temp *= Matrix;
			D3DXMatrixTranslation(&Matrix,Elem.fX,Elem.fY,Elem.fZ);
			temp *= Matrix;
			m_pFontManager->GetSprite()->SetTransform(&temp);
			m_pFontManager->GetDXFont(Elem.uiID)->DrawText(m_pFontManager->GetSprite(),Elem.Info,
				-1,&re,DT_NOCLIP|DT_CENTER,Elem.Color);
		}
	}
	m_pFontManager->GetSprite()->End();
	m_3DSet.clear();
}

void gbUIRenderer::EstimateCharacter(FONTID fontID,unsigned int uiCharacter,int& iWidth,int& iHeight)
{
	SAFE_DO_
	m_pFontManager->EstimateCharacter(fontID,uiCharacter,iWidth,iHeight);
}

void gbUIRenderer::EstimateString(FONTID fontID,const char* pcString,int& iWidth,int& iHeight) 
{
	SAFE_DO_
	m_pFontManager->EstimateString(fontID,pcString,iWidth,iHeight);
}

void gbUIRenderer::GetCutPoint(FONTID fontID,const char* pcString,int iWidth,int& iPos)
{
	SAFE_DO_
	m_pFontManager->GetCutPoint(fontID,pcString,iWidth,iPos);
}

//clip
void gbUIRenderer::SetClipRect( int iTop, int iLeft, int iHeight, int iWidth)
{
#ifdef UI_CLIPDOWN
	return;
#endif
	SAFE_DO_
	int iRight = iLeft + iWidth;
	int iBottom = iTop + iHeight;
	Clamp(iTop,m_uiRenderHeight,0);
	Clamp(iBottom,m_uiRenderHeight,0);
	Clamp(iLeft,m_uiRenderWidth,0);
	Clamp(iRight,m_uiRenderWidth,0);
	iHeight = iBottom - iTop;
	iWidth = iRight - iLeft;
	assert(iHeight>0 && iWidth>0);
	if(0>=iHeight||0>=iWidth)
		return;
	if(m_ClipRect.iTop == iTop
		&&m_ClipRect.iLeft == iLeft
		&&m_ClipRect.iWidth == iWidth
		&&m_ClipRect.iHeight == iHeight)
		return;
	m_ClipRect.iTop = iTop;
	m_ClipRect.iLeft = iLeft;
	m_ClipRect.iHeight = iHeight;
	m_ClipRect.iWidth = iWidth;
	if(m_bEnableClip)
	{
		m_pFontManager->TryEndSprite();
		D3DVIEWPORT9 kD3DPort;

		kD3DPort.X = (unsigned int)(m_ClipRect.iLeft);
		kD3DPort.Y = (unsigned int)(m_ClipRect.iTop);
		kD3DPort.Width = 
			(unsigned int)(m_ClipRect.iWidth);
		kD3DPort.Height = 
			(unsigned int)(m_ClipRect.iHeight);
		kD3DPort.MinZ = 0.0f;     
		kD3DPort.MaxZ = 1.0f;

		m_pFontManager->GetDx9Device()->SetViewport(&kD3DPort);
	}
}

void gbUIRenderer::GetClipRect( int&iTop, int&iLeft, int&iHeight, int&iWidth)
{
	iTop = m_ClipRect.iTop;
	iLeft = m_ClipRect.iLeft;
	iHeight = m_ClipRect.iHeight;
	iWidth = m_ClipRect.iWidth;
}
void gbUIRenderer::EnableClipRect(bool flag)
{
#ifdef UI_CLIPDOWN
	return;
#endif
	SAFE_DO_
	if(m_bEnableClip == flag)
		return;
	m_bEnableClip= flag;
	if(m_bEnableClip)
	{
		m_pFontManager->TryEndSprite();
		D3DVIEWPORT9 kD3DPort;

		kD3DPort.X = (unsigned int)(m_ClipRect.iLeft);
		kD3DPort.Y = (unsigned int)(m_ClipRect.iTop);
		kD3DPort.Width = 
			(unsigned int)(m_ClipRect.iWidth);
		kD3DPort.Height = 
			(unsigned int)(m_ClipRect.iHeight);
		kD3DPort.MinZ = 0.0f;     
		kD3DPort.MaxZ = 1.0f;

		m_pFontManager->GetDx9Device()->SetViewport(&kD3DPort);
		m_bEnableClip= flag;
	}
	else
	{
		m_pFontManager->TryEndSprite();
		D3DVIEWPORT9 kD3DPort;
		kD3DPort.X = 0;
		kD3DPort.Y = 0;
		kD3DPort.Width = m_uiRenderWidth;
		kD3DPort.Height = m_uiRenderHeight;
		kD3DPort.MinZ = 0.0f;     
		kD3DPort.MaxZ = 1.0f;

		m_pFontManager->GetDx9Device()->SetViewport(&kD3DPort);
	}
}

bool gbUIRenderer::IsClipEnable()
{
	return m_bEnableClip;
}

//draw
void gbUIRenderer::SetDrawFlags(BlendMode flags)
{
	if(flags == APPLY_REPLACE)
	{
		m_eApplyMode = NiTexturingProperty::APPLY_REPLACE;
	}
	else if(flags == APPLY_DECAL)
	{
		m_eApplyMode = NiTexturingProperty::APPLY_DECAL;
	}
	else if(flags == APPLY_MODULATE)
	{
		m_eApplyMode = NiTexturingProperty::APPLY_MODULATE;
	}
}

void gbUIRenderer::DrawLine(int iXTop,int iXLeft,int iYTop,int iYLeft,DWORD color)
{
#ifdef UI_LINEDOWN
	return;
#endif
	SAFE_DO_
	m_pFontManager->TryStartSprite();
	SCREEN_VERTEX vertices[2] =
	{
		(float) iXLeft -0.5f,			(float) iXTop -0.5f,		0.5f, 1.0f, color, 0, 0,
		(float) iYLeft -0.5f,			(float) iYTop -0.5f,		0.5f, 1.0f, color, 0, 0, 
	};

	IDirect3DDevice9* pd3dDevice = m_pFontManager->GetDx9Device();

	// Since we're doing our own drawing here we need to flush the sprites
	m_pFontManager->GetSprite()->Flush();
	//IDirect3DVertexDeclaration9 *pDecl = NULL;
	//pd3dDevice->GetVertexDeclaration( &pDecl );  // Preserve the sprite's current vertex decl
	pd3dDevice->SetFVF( SCREEN_VERTEX::FVF );

	pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_SELECTARG2 );
	pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG2 );

	pd3dDevice->DrawPrimitiveUP( D3DPT_LINESTRIP, 1, vertices, sizeof(SCREEN_VERTEX) );

	pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_MODULATE );
	pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_MODULATE );
	// Restore the vertex decl
	//pd3dDevice->SetVertexDeclaration( pDecl );
	//pDecl->Release();
}

void gbUIRenderer::DrawCharacter(unsigned int Glyph,int iTop,int iLeft)
{
#ifdef UI_SHUTDOWN
	return;
#endif
	SAFE_DO_
	m_pFontManager->TryStartSprite();
	NiColorA color = m_pFontManager->GetCurColor();
	D3DXCOLOR tempcolor;
	tempcolor.r=color.r;
	tempcolor.b=color.b;
	tempcolor.g=color.g;
	tempcolor.a=color.a;
	char pcText[3];

	m_pFontManager->Int_Str(Glyph,pcText);
	RECT re;
	re.top = (LONG)iTop;
	re.left = (LONG)iLeft;
	re.bottom = (LONG)m_uiRenderHeight;
	re.right = (LONG)m_uiRenderWidth;

	m_pFontManager->GetDXFont(m_pFontManager->GetCurFont())->DrawText(m_pFontManager->GetSprite(),pcText,
		-1,&re,DT_NOCLIP,tempcolor);
}

void gbUIRenderer::DrawString(const char* pcText,int iTop,int iLeft)
{
#ifdef UI_FONTDOWN
	return;
#endif
	SAFE_DO_
	m_pFontManager->TryStartSprite();
	NiColorA color = m_pFontManager->GetCurColor();
	D3DXCOLOR tempcolor;
	tempcolor.r=color.r;
	tempcolor.b=color.b;
	tempcolor.g=color.g;
	tempcolor.a=color.a;
	RECT re;
	re.top = (LONG)iTop;
	re.left = (LONG)iLeft;
	re.bottom = (LONG)m_uiRenderHeight;
	re.right = (LONG)m_uiRenderWidth;

	D3DXMATRIX temp;
	D3DXMatrixTranslation(&temp,0.0f,0.0f,0.0f);
	m_pFontManager->GetSprite()->SetTransform(&temp);
	m_pFontManager->GetDXFont(m_pFontManager->GetCurFont())->DrawText(m_pFontManager->GetSprite(),pcText,
		-1,&re,DT_NOCLIP,tempcolor);
}

void gbUIRenderer::DrawRect(int iTop,int iLeft,int iHeight,int iWidth,DWORD color,bool bNeedFill)
{
#ifdef UI_LINEDOWN
	return;
#endif
	SAFE_DO_
	m_pFontManager->TryStartSprite();


	SCREEN_VERTEX vertices[5] =
	{
		(float) iLeft -0.5f,			(float) iTop -0.5f,		0.5f, 1.0f, color, 0, 0,
		(float) (iLeft+iWidth) -0.5f,	(float) iTop -0.5f,		0.5f, 1.0f, color, 0, 0, 
		(float) (iLeft+iWidth) -0.5f,	(float) (iTop + iHeight) -0.5f, 0.5f, 1.0f, color, 0, 0, 
		(float) iLeft -0.5f,			(float) (iTop + iHeight) -0.5f, 0.5f, 1.0f, color, 0, 0,
		(float) iLeft -0.5f,			(float) iTop -0.5f,		0.5f, 1.0f, color, 0, 0,
	};

	IDirect3DDevice9* pd3dDevice = m_pFontManager->GetDx9Device();

	// Since we're doing our own drawing here we need to flush the sprites
	m_pFontManager->GetSprite()->Flush();
	//IDirect3DVertexDeclaration9 *pDecl = NULL;
	//pd3dDevice->GetVertexDeclaration( &pDecl );  // Preserve the sprite's current vertex decl

	pd3dDevice->SetFVF( SCREEN_VERTEX::FVF );

	pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_SELECTARG2 );
	pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG2 );

	if(bNeedFill)
		pd3dDevice->DrawPrimitiveUP( D3DPT_TRIANGLEFAN, 2, vertices, sizeof(SCREEN_VERTEX) );
	else
		pd3dDevice->DrawPrimitiveUP( D3DPT_LINESTRIP, 4, vertices, sizeof(SCREEN_VERTEX) );

	pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_MODULATE );
	pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_MODULATE );

	// Restore the vertex decl
	//pd3dDevice->SetVertexDeclaration( pDecl );
	//pDecl->Release();
}

void gbUIRenderer::DrawPolygon(int* pVertexVec,int iNumVertices,DWORD color,bool bNeedFill)
{
#ifdef UI_SHUTDOWN
	return;
#endif
	SAFE_DO_
		m_pFontManager->TryStartSprite();
	SCREEN_VERTEX* vertices = new SCREEN_VERTEX[ iNumVertices ];

	SCREEN_VERTEX* pVertex = vertices;
	
	int* pt = pVertexVec;
	for( int i=0; i < iNumVertices; i++ )
	{
		pVertex->x =(float) pVertexVec[2*i];
		pVertex->y =(float) pVertexVec[2*i+1];
		pVertex->z = 0.5f;
		pVertex->h = 1.0f;
		pVertex->color = color;
		pVertex->tu = 0.0f;
		pVertex->tv = 0.0f;

		pVertex++;
		pt+=2;
	}

	IDirect3DDevice9* pd3dDevice = m_pFontManager->GetDx9Device();

	m_pFontManager->GetSprite()->Flush();
	//IDirect3DVertexDeclaration9 *pDecl = NULL;
	//pd3dDevice->GetVertexDeclaration( &pDecl );  // Preserve the sprite's current vertex decl
	pd3dDevice->SetFVF( SCREEN_VERTEX::FVF );

	pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_SELECTARG2 );
	pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG2 );

	if(bNeedFill)
		pd3dDevice->DrawPrimitiveUP( D3DPT_TRIANGLEFAN, iNumVertices - 2, vertices, sizeof(SCREEN_VERTEX) );
	else
		pd3dDevice->DrawPrimitiveUP( D3DPT_LINESTRIP, iNumVertices - 1, vertices, sizeof(SCREEN_VERTEX) );

	pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_MODULATE );
	pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_MODULATE );

	// Restore the vertex decl
	//pd3dDevice->SetVertexDeclaration( pDecl );
	//pDecl->Release();

	SAFE_DELETE_ARRAY( vertices );
}

void gbUIRenderer::DrawImage(IMGID ImageID,int iTop,int iLeft,int iHeight,int iWidth)
{
#ifdef UI_SHUTDOWN
	return;
#endif
	SAFE_DO_
	m_pFontManager->TryStartSprite();
	TextureRect* pImgRect = m_pTexManager->GetTextureRect(ImageID);
	
	if(!pImgRect)
		return;

	if(pImgRect->bIsChanged)
	{
		NiTexture::RendererData * pRenderData = pImgRect->spTexture->GetRendererData();
		NiDX9TextureData* pDx9Data = NiSmartPointerCast(NiDX9TextureData,pRenderData);
		pDx9Data->GetAsSourceTexture()->Update();
		pImgRect->bIsChanged = false;
	}

	NiTexture::RendererData * pRenderData = pImgRect->spTexture->GetRendererData();
	NiDX9TextureData* pDx9Data = NiSmartPointerCast(NiDX9TextureData,pRenderData);
	D3DXMATRIX temp,Matrix;
	RECT TempRT;
	FLOAT scaleX = (FLOAT)iWidth/pImgRect->iWidth;
	FLOAT scaleY = (FLOAT)iHeight/pImgRect->iHeight;
	D3DXMatrixScaling(&temp, scaleX,scaleY, 1.0f);
	D3DXMatrixTranslation(&Matrix,(FLOAT)iLeft,(FLOAT)iTop, 0.0f);
	temp *= Matrix;
	
	TempRT.left = pImgRect->iLeft;
	TempRT.top = pImgRect->iTop;
	TempRT.right = pImgRect->iLeft + pImgRect->iWidth;
	TempRT.bottom = pImgRect->iTop +pImgRect->iHeight;

	D3DXVECTOR3 Pos = D3DXVECTOR3(0.0f,0.0f,0.0f);
	D3DXCOLOR Dcolor = ToD3DColor(pImgRect->dwColor);
	m_pFontManager->GetSprite()->SetTransform(&temp);
	m_pFontManager->GetSprite()->Draw(LPDIRECT3DTEXTURE9(pDx9Data->GetD3DTexture()),&TempRT,NULL,&Pos,Dcolor);
}

bool gbUIRenderer::DrawImage(IMGID ImageID, FillMode enFillMode, RECT* rcScr, RECT* rcDest, unsigned char byImageAlphaSet) 
{
 	SAFE_DO(false) ;
		m_pFontManager->TryStartSprite();
	TextureRect* pImgRect = m_pTexManager->GetTextureRect(ImageID);

	if(!pImgRect)
		return false;
	if(pImgRect->bIsChanged)
	{
		NiTexture::RendererData * pRenderData = pImgRect->spTexture->GetRendererData();
		NiDX9TextureData* pDx9Data = NiSmartPointerCast(NiDX9TextureData,pRenderData);
		pDx9Data->GetAsSourceTexture()->Update();
		pImgRect->bIsChanged = false;

	}

	NiTexture::RendererData * pRenderData = pImgRect->spTexture->GetRendererData();
	NiDX9TextureData* pDx9Data = NiSmartPointerCast(NiDX9TextureData,pRenderData);

	D3DXVECTOR3 Pos  = D3DXVECTOR3(0.0f,0.0f,0.0f);
	D3DXCOLOR Dcolor = ToD3DColor(pImgRect->dwColor);
	Dcolor.a = (float)byImageAlphaSet/255.0f ;

	int nSrcHeight  = rcScr->bottom  - rcScr->top ;
	int nSrcWidth   = rcScr->right   - rcScr->left ;
	int nDestHeight = rcDest->bottom - rcDest->top ;
	int nDestWidth  = rcDest->right  - rcDest->left ;

	if ((nSrcWidth < 1) || (nSrcHeight < 1) || (nDestWidth < 1) || (nDestHeight < 1))
	{
		return false ;
	}
	
	if ( enFillMode == FILLMODE_SCALE )
	{
		D3DXMATRIX temp,Matrix;
		RECT TempRT;
		FLOAT scaleX = (FLOAT)nDestWidth / nSrcWidth;         //pImgRect->iWidth;
		FLOAT scaleY = (FLOAT)nDestHeight / nSrcHeight ;  //pImgRect->iHeight;
		D3DXMatrixScaling( &temp, scaleX,scaleY, 1.0f );
		D3DXMatrixTranslation( &Matrix,(FLOAT)rcDest->left, (FLOAT)rcDest->top, 0.0f );
		temp *= Matrix;
		m_pFontManager->GetSprite()->SetTransform(&temp);
		m_pFontManager->GetSprite()->Draw(LPDIRECT3DTEXTURE9(pDx9Data->GetD3DTexture()), rcScr, NULL, &Pos, Dcolor);
	}
	else if ( enFillMode == FILLMODE_TILE)
	{
		int _x , _y ;
		int nScreenBlockXCount , nScreenBlockYCount ;
		RECT rcDraw ;
		nScreenBlockXCount = nDestWidth / nSrcWidth + 1 ;
		nScreenBlockYCount = nDestHeight / nSrcHeight + 1 ;

		for ( _x = 0 ; _x < nScreenBlockXCount ; ++_x )
		{                  
			for ( _y = 0 ; _y < nScreenBlockYCount ; ++_y )
			{
				rcDraw.left   =  _x  * nSrcWidth  + rcDest->left    ; 
				rcDraw.top    =  _y  * nSrcHeight + rcDest->top     ;

				int nDrawWidth  = nSrcWidth  ;
				int nDrawHeight = nSrcHeight ;
				if ( rcDraw.left + nSrcWidth > rcDest->right )
				{
					nDrawWidth = rcDest->right - rcDraw.left ;
				} 
				if ( rcDraw.top + nSrcHeight > rcDest->bottom )
				{
					nDrawHeight = rcDest->bottom - rcDraw.top ;  
				}

				rcDraw.right  = rcDraw.left + nDrawWidth  - 1;
				rcDraw.bottom = rcDraw.top  + nDrawHeight - 1;

				D3DXMATRIX temp,Matrix;
				FLOAT scaleX =  1.0f ; //(FLOAT)nSrcWidth / pImgRect->iWidth;
				FLOAT scaleY =  1.0f ; //(FLOAT)nSrcHeight / pImgRect->iHeight;
				D3DXMatrixScaling( &temp, scaleX, scaleY, 1.0f );
				D3DXMatrixTranslation( &Matrix,(FLOAT)rcDraw.left, (FLOAT)rcDraw.top, 0.0f );
				temp *= Matrix;
				rcDraw.left = rcScr->left ;
				rcDraw.top = rcScr->top ;
				rcDraw.right = rcScr->left + nDrawWidth ;
				rcDraw.bottom = rcScr->top + nDrawHeight ;
				m_pFontManager->GetSprite()->SetTransform(&temp);
		        m_pFontManager->GetSprite()->Draw(LPDIRECT3DTEXTURE9(pDx9Data->GetD3DTexture()), &rcDraw, NULL, &Pos, Dcolor);
			}
		}
	}
	else
	{
		return false ;
	}

	//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	// warning! Flush problem
	m_pFontManager->GetSprite()->Flush();
	return true ;

}
void gbUIRenderer::DrawImageWithMask(IMGID ImageID, IMGID ImageIDMask,
									 int iLeftDes,int iTopDes, int iWidthDes, int iHeightDes )
{
#ifdef UI_SHUTDOWN
	return;
#endif
	SAFE_DO_
		m_pFontManager->TryStartSprite();
	TextureRect* pImgRect = m_pTexManager->GetTextureRect(ImageID);
	TextureRect* pImgRectMask = m_pTexManager->GetTextureRect(ImageIDMask);

	if(pImgRect->bIsChanged)
	{
		NiTexture::RendererData * pRenderData = pImgRect->spTexture->GetRendererData();
		NiDX9TextureData* pDx9Data = NiSmartPointerCast(NiDX9TextureData,pRenderData);
		pDx9Data->GetAsSourceTexture()->Update();
		pImgRect->bIsChanged = false;
	}
	if(pImgRectMask->bIsChanged)
	{
		NiTexture::RendererData * pRenderData = pImgRectMask->spTexture->GetRendererData();
		NiDX9TextureData* pDx9Data = NiSmartPointerCast(NiDX9TextureData,pRenderData);
		pDx9Data->GetAsSourceTexture()->Update();
		pImgRectMask->bIsChanged = false;
	}

	NiTexture::RendererData * pRenderData = pImgRect->spTexture->GetRendererData();
	NiDX9TextureData* pDx9Data = NiSmartPointerCast(NiDX9TextureData,pRenderData);

	NiTexture::RendererData * pRenderDataMask = pImgRectMask->spTexture->GetRendererData();
	NiDX9TextureData* pDx9DataMask = NiSmartPointerCast(NiDX9TextureData,pRenderDataMask);


	float fOrgWidth, fOrgHeight;
	fOrgWidth = (float)pDx9Data->GetWidth();
	fOrgHeight = (float)pDx9Data->GetHeight();
	int iLeft, iTop, iWidth, iHeight;
	iLeft = pImgRect->iLeft;
	iTop = pImgRect->iTop;
	iWidth = pImgRect->iWidth;
	iHeight = pImgRect->iHeight;

	D3DVECTOR arrTC[4];
	arrTC[0].x = (float)iLeft /	fOrgWidth;
	arrTC[0].y = (float)iTop /	fOrgHeight;

	arrTC[1].x = (float)(iLeft+ iWidth)/ fOrgWidth;
	arrTC[1].y = (float)iTop / fOrgHeight;

	arrTC[2].x = (float)(iLeft+ iWidth)/ fOrgWidth;
	arrTC[2].y = (float)(iTop + iHeight)/ fOrgHeight;

	arrTC[3].x = (float)iLeft /	fOrgWidth;
	arrTC[3].y = (float)(iTop + iHeight)/ fOrgHeight;

	fOrgWidth = (float)pDx9DataMask->GetWidth();
	fOrgHeight = (float)pDx9DataMask->GetHeight();
	iLeft = pImgRectMask->iLeft;
	iTop = pImgRectMask->iTop;
	iWidth = pImgRectMask->iWidth;
	iHeight = pImgRectMask->iHeight;
	D3DVECTOR arrTC_A[4];
	arrTC_A[0].x = (float)iLeft /	fOrgWidth;
	arrTC_A[0].y = (float)iTop /	fOrgHeight;

	arrTC_A[1].x = (float)(iLeft+ iWidth)/ fOrgWidth;
	arrTC_A[1].y = (float)iTop / fOrgHeight;

	arrTC_A[2].x = (float)(iLeft+ iWidth)/ fOrgWidth;
	arrTC_A[2].y = (float)(iTop + iHeight)/ fOrgHeight;

	arrTC_A[3].x = (float)iLeft /	fOrgWidth;
	arrTC_A[3].y = (float)(iTop + iHeight)/ fOrgHeight;

	MASK_VERTEX vertices[6] =
	{
		(float) iLeftDes -0.5f, (float) iTopDes -0.5f, 0.5f,  arrTC[0].x, arrTC[0].y, arrTC_A[0].x, arrTC_A[0].y,
			(float) (iLeftDes+iWidthDes) -0.5f,	(float) iTopDes -0.5f, 0.5f,  arrTC[1].x, arrTC[1].y, arrTC_A[1].x, arrTC_A[1].y,
			(float) (iLeftDes+iWidthDes) -0.5f,	(float) (iTopDes + iHeightDes) -0.5f, 0.5f,  arrTC[2].x, arrTC[2].y, arrTC_A[2].x, arrTC_A[2].y,
			(float) (iLeftDes+iWidthDes) -0.5f,	(float) (iTopDes + iHeightDes) -0.5f, 0.5f,  arrTC[2].x, arrTC[2].y, arrTC_A[2].x, arrTC_A[2].y,
			(float) iLeftDes -0.5f, (float) (iTopDes + iHeightDes) -0.5f, 0.5f,  arrTC[3].x, arrTC[3].y, arrTC_A[3].x, arrTC_A[3].y,
			(float) iLeftDes -0.5f, (float) iTopDes -0.5f, 0.5f,  arrTC[0].x, arrTC[0].y, arrTC_A[0].x, arrTC_A[0].y,
	};

	IDirect3DDevice9* pd3dDevice = m_pFontManager->GetDx9Device();
	m_pFontManager->GetSprite()->Flush();
	pd3dDevice->SetFVF( MASKFVF );

	pd3dDevice->SetTexture( 0, LPDIRECT3DTEXTURE9(pDx9Data->GetD3DTexture()) );
	pd3dDevice->SetTextureStageState( 0, D3DTSS_TEXCOORDINDEX,0);
	pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
	pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_SELECTARG1  );

	pd3dDevice->SetTexture( 1, LPDIRECT3DTEXTURE9(pDx9DataMask->GetD3DTexture()) );
	pd3dDevice->SetTextureStageState( 1, D3DTSS_TEXCOORDINDEX,1);
	pd3dDevice->SetTextureStageState( 1, D3DTSS_COLORARG1, D3DTA_CURRENT);
	pd3dDevice->SetTextureStageState( 1, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
	pd3dDevice->SetTextureStageState( 1, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	pd3dDevice->SetTextureStageState( 1, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1  );

	pd3dDevice->DrawPrimitiveUP( D3DPT_TRIANGLELIST, 2, vertices, sizeof(MASK_VERTEX) );


}
void gbUIRenderer::ChangePointCoordWithClip(float& fX,float& fY)
{
	fX -= (float)m_ClipRect.iLeft;
	fY -= (float)m_ClipRect.iTop;
	fX /= (float)m_ClipRect.iWidth*m_fRScreenX;
	fY /= (float)m_ClipRect.iHeight*m_fRScreenY;
}

void gbUIRenderer::ChangeLengthtCoordWithClip(float& fW,float& fH)
{
	fW /= m_ClipRect.iWidth*m_fRScreenX;
	fH /= m_ClipRect.iHeight*m_fRScreenY;
}
void gbUIRenderer::BeginUI()
{
	SAFE_DO_
	m_pFontManager->TryStartSprite();
	if(m_bEnableClip)
	{
		m_bEnableClip =false;
		EnableClipRect(true);
	}
}

void gbUIRenderer::EndUI()
{
	SAFE_DO_
	m_pFontManager->TryEndSprite();
}

NiColorA gbUIRenderer::ToNiColorA(DWORD color)
{
	//((DWORD)((((a)&0xff)<<24)|(((r)&0xff)<<16)|(((g)&0xff)<<8)|((b)&0xff)))
	NiColorA nicol;
	float fMax = 1.0f/255.0f;
	nicol.b = fMax*(color&0xff);color >>= 8;
	nicol.g = fMax*(color&0xff);color >>= 8;
	nicol.r = fMax*(color&0xff);color >>= 8;
	nicol.a = fMax*(color&0xff);
	return nicol;
}

D3DXCOLOR gbUIRenderer::ToD3DColor(DWORD color)
{
	D3DXCOLOR nicol;
	float fMax = 1.0f/255.0f;
	nicol.b = fMax*(color&0xff);color >>= 8;
	nicol.g = fMax*(color&0xff);color >>= 8;
	nicol.r = fMax*(color&0xff);color >>= 8;
	nicol.a = fMax*(color&0xff);
	return nicol;
}
void gbUIRenderer::Clamp(int& iSrc,int iMax, int iMin)
{
	iSrc = (iSrc > iMax)?iMax:iSrc;
	iSrc = (iSrc < iMin)?iMin:iSrc;
}

//const int DIRTY_RECT_WIDTH = 32 ;
//const int DIRTY_RECT_HEIGHT = 32 ;
void gbUIRenderer::CreateUIScreenBlock(int nScreenWidth, int nScreenHeight) 
{
	if (( nScreenWidth <= 0 )&&( nScreenHeight <= 0 ))
	{
		return ;
	}
    m_imgScreenUpdateBlock = CreateImage(nScreenWidth, nScreenHeight, 1);

//	m_imgScreenUpdateBlock = CreateImage(DIRTY_RECT_WIDTH, DIRTY_RECT_HEIGHT, 1);

	m_nScreenBlockXCount = nScreenWidth  / DIRTY_RECT_WIDTH  + 1 ;
	m_nScreenBlockYCount = nScreenHeight / DIRTY_RECT_HEIGHT + 1 ;

    m_vScreenBlockUpdateMark.resize( m_nScreenBlockXCount * m_nScreenBlockYCount) ;
	ClearImage( m_imgScreenUpdateBlock , 0x00000000 ) ;

}

void gbUIRenderer::UpdateUIScreen( RECT *pDirtyRectList, int nRectCount, TBitImage *pScreenBuffer) 
{
    if ( !pDirtyRectList && !pScreenBuffer && nRectCount <= 0 )
    {
		return ;
    }

    int  i = 0 ;
	int _x = 0 ;
	int _y = 0 ;
    RECT rc;

	for ( i = 0 ; i < m_nNumScreenBlock ; ++i )
	{
        m_vScreenBlockUpdateMark[i] = false ;
	}
	for ( i = 0 ; i < nRectCount ; ++i )
	{
		rc = pDirtyRectList[i] ;

		int nX1 = rc.left   / DIRTY_RECT_WIDTH  ;
		int nX2 = rc.right  / DIRTY_RECT_WIDTH  ;
		int nY1 = rc.top    / DIRTY_RECT_HEIGHT ;
		int nY2 = rc.bottom / DIRTY_RECT_HEIGHT ;

       for ( _x = nX1 ; _x <= nX2 ; ++_x )
       {
		   for ( _y = nY1 ; _y <= nY2 ; ++_y )
		   {
               m_vScreenBlockUpdateMark[ _y * m_nScreenBlockXCount + _x] = true ;
		   }
       }
	}

	for ( _x = 0 ; _x < m_nScreenBlockXCount ; ++_x )
	{
		for ( _y = 0 ; _y < m_nScreenBlockYCount ; ++_y )
		{
			if (m_vScreenBlockUpdateMark[ _y * m_nScreenBlockXCount + _x])
			{

				rc.left   =  _x  * DIRTY_RECT_WIDTH      ;
				rc.top    =  _y  * DIRTY_RECT_HEIGHT     ;

				int nDrawWidth  = DIRTY_RECT_WIDTH  ;
				int nDrawHeight = DIRTY_RECT_HEIGHT ;
				if ( rc.left + DIRTY_RECT_WIDTH > pScreenBuffer->w )
				{
					nDrawWidth = pScreenBuffer->w - rc.left ;
				} 
				if ( rc.top + DIRTY_RECT_HEIGHT > pScreenBuffer->h )
				{
					nDrawHeight = pScreenBuffer->h - rc.bottom ;
				}

				rc.right  = rc.left + nDrawWidth  - 1;
				rc.bottom = rc.top  + nDrawHeight - 1;
				UpdateImage( m_imgScreenUpdateBlock, 0, 0, pScreenBuffer , &rc );

			//	DrawImage( m_imgScreenUpdateBlock, rc.left , rc.top, nDrawWidth, nDrawHeight ) ;
			}
		}
	}
	DrawImage( m_imgScreenUpdateBlock, 0 , 0, pScreenBuffer->h, pScreenBuffer->w );

}
void gbUIRenderer::ClearImage(IMGID ImageID , DWORD dwColor) 
{
	if (ImageID == 0xffffffff)
	{
		return ;
	}
      m_pTexManager->ClearSourceTexture(ImageID , dwColor) ;
}

void gbUIRenderer::DrawText(unsigned int nDestLeft, unsigned int nDestTop, const char* szText, RECT *rcClip) 
{
	if (szText != NULL)
	{
		m_pFontCanvas->PrintText( szText, nDestLeft, nDestTop , rcClip );
	}
}
void gbUIRenderer::DrawText( const char* pText, const RECT& rDestRect, RECT *rcClip, unsigned int vFormat = GUI_DT_LEFT | GUI_DT_VCENTER) 
{
	CGUIRect aRect(rDestRect.left, rDestRect.top, rDestRect.right, rDestRect.bottom) ;
    m_pFontCanvas->PrintText(pText, aRect, rcClip, vFormat) ;
}

void gbUIRenderer::CreateFont( const char* pFontName, const char* pFontFilePath, int nFontHeight, bool bAnsiAliased, 
							   bool bUnderline, bool bBold, bool bItalic, int nRowSpacing ) 
{
	if( (pFontName !=NULL ) && (pFontFilePath != NULL) && (nFontHeight > 0) )
	{
		CGUIFontManager::GetSingleton( ).Create( pFontName, pFontFilePath, nFontHeight, bAnsiAliased, bUnderline, bBold, bItalic, nRowSpacing);
	}
}

void gbUIRenderer::CreateFont( const char* pFontName, const char* pFontFilePath, int nFontHeight, bool bAnsiAliased, 
							    bool bUnderline, bool bBold, bool bItalic,int nRowSpacing ,
						       int nShadowOffsetX,    int nShadowOffsetY, DWORD dwShadowColor) 
{
	if( (pFontName !=NULL ) && (pFontFilePath != NULL) && (nFontHeight > 0) )
	{
        CGUIFontManager::GetSingleton( ).Create( pFontName, pFontFilePath, nFontHeight, bAnsiAliased, bUnderline, bBold,bItalic,nRowSpacing,
		                                     nShadowOffsetX, nShadowOffsetY, dwShadowColor) ;
	}
}

void gbUIRenderer::CreateFont( const char* pFontName, const char* pFontFilePath, int nFontHeight, bool bAnsiAliased, 
							   bool bUnderline, bool bBold, bool bItalic, int nRowSpacing ,
						       unsigned int nEdgeWidth, DWORD EdgeColor )
{
	if( (pFontName !=NULL ) && (pFontFilePath != NULL) && (nFontHeight > 0) )
	{
        CGUIFontManager::GetSingleton( ).Create(pFontName, pFontFilePath, nFontHeight, bAnsiAliased, bUnderline, bBold, bItalic, nRowSpacing, nEdgeWidth, EdgeColor);
	}
}

void gbUIRenderer::CreateFont( const char* pFontName,  const char* pFontFilePath, int nFontHeight, bool bAnsiAliased, 
							   bool bUnderline, bool bBold, bool bItalic, int nRowSpacing ,
						       int nShadowOffsetX,   int nShadowOffsetY, DWORD dwShadowColor, unsigned int nEdgeWidth,   
						       DWORD EdgeColor) 
{
	if( (pFontName !=NULL ) && (pFontFilePath != NULL) && (nFontHeight > 0) )
	{
         CGUIFontManager::GetSingleton( ).Create(pFontName, pFontFilePath, nFontHeight, bAnsiAliased,  bUnderline, bBold,bItalic, nRowSpacing, nShadowOffsetX
		                                    , nShadowOffsetY, dwShadowColor, nEdgeWidth, EdgeColor ) ;
	}
}

void gbUIRenderer::SetCurrentFontName(const char* pFontName)
{
	m_pFontCanvas->SetFont(CGUIFontManager::GetSingleton().GetFont(pFontName));
}
const char* gbUIRenderer::GetCurrentFontName()
{
	return m_pFontCanvas->GetFont()->GetName().c_str() ;

}
void gbUIRenderer::DestroyFont(const char* pFontName) 
{
	CGUIFontManager::GetSingleton().Destroy(pFontName);
}

void gbUIRenderer::SetCurrentTextFrontColor(DWORD dwColor)
{
	m_pFontCanvas->GetFont()->SetTextFrontColor( dwColor );
}
void gbUIRenderer::SetCurrentTextBackColor(DWORD dwColor) 
{
	m_pFontCanvas->GetFont()->SetTextBackColor( dwColor );
}
void gbUIRenderer::SetCurrentTextShadowColor(DWORD dwColor)
{
	m_pFontCanvas->GetFont()->SetTextShadowColor( dwColor );
}
void gbUIRenderer::SetCurrentTextEdgeColor(DWORD dwColor)
{
	m_pFontCanvas->GetFont()->SetTextEdgeColor( dwColor );
}

void gbUIRenderer::SetCurrentTextFrontAndBackAlpha(unsigned int nAlpha) 
{
    m_pFontCanvas->GetFont()->SetTextFrontAndBackAlpha( nAlpha) ;
}

int  gbUIRenderer::GetTextWidth( const char* pText, int nTextCount ) 
{
	if (( !pText ) || (!m_pFontCanvas))
	{
		return -1 ;
	}

    return m_pFontCanvas->GetFont()->GetTextWidth( pText, nTextCount) ;
}
int  gbUIRenderer::GetTextHeight() 
{
   if (!m_pFontCanvas)
   {
	   return -1 ;
   }
   return m_pFontCanvas->GetFont()->GetFontHeight();
}

 void gbUIRenderer::BeginRenderText()  
 {
	 m_pFontCanvas->BeginRender();
 }
 void gbUIRenderer::EndRenderText() 
 {
     m_pFontCanvas->EndRender() ;
 }

DWORD gbUIRenderer::GetCurrentTextFrontColor()
{
    return m_pFontCanvas->GetFont()->GetTextFrontColor();
}
DWORD gbUIRenderer::GetCurrentTextBackColor() 
{
    return m_pFontCanvas->GetFont()->GetTextBackColor();
}
DWORD gbUIRenderer::GetCurrentTextShadowColor()
{
	return m_pFontCanvas->GetFont()->GetTextShadowColor();
}
DWORD gbUIRenderer::GetCurrentTextEdgeColor()
{
	return m_pFontCanvas->GetFont()->GetTextEdgeColor() ;
}
DWORD gbUIRenderer::GetCurrentTextAlpha()  
{
	DWORD dwFontColor = m_pFontCanvas->GetFont()->GetTextFrontColor() ;
    dwFontColor = dwFontColor>>24 ;
	return dwFontColor;
}