
#include "gbFontManager.h"
#include "NiDX9Renderer.h"

#ifdef ENGINE_NEW
#include "../MemoryCheck.h"
#endif

gbFontManager* gbFontManager::ms_pManager = NULL;
gbFontManager* gbFontManager::GetManager()
{
	return ms_pManager?ms_pManager:(new gbFontManager());
}

gbFontManager::gbFontManager()
{
	NiDX9Renderer *pDX9Renderer = NULL;
	pDX9Renderer = NiSmartPointerCast(NiDX9Renderer,NiRenderer::GetRenderer());
	m_pd3dDevice = pDX9Renderer->GetD3DDevice();
	D3DXCreateSprite( m_pd3dDevice, &m_Sprite );
	ms_pManager = this;
	bOnSpriteFly = false;
	m_bLostDevice = false;
}

gbFontManager::~gbFontManager()
{
	FontPool::iterator it;
	for(it = m_FontPool.begin(); it != m_FontPool.end(); it ++)
	{
		(*it)->Release();
		*it = NULL;
	}
	m_FontPool.clear();
	if(m_Sprite)
		m_Sprite->Release();
	m_Sprite = NULL;
	//m_pd3dDevice->Release();
	//m_pd3dDevice = NULL;
}

void gbFontManager::OnRestDevice()
{
	FontPool::iterator it;
	for(it = m_FontPool.begin(); it != m_FontPool.end(); it ++)
	{
		(*it)->OnResetDevice();
	}
	if(m_Sprite)
		m_Sprite->OnResetDevice();
	m_bLostDevice = false;
}

void gbFontManager::OnLostDevice()
{
	FontPool::iterator it;
	if ( m_FontPool.size() > 0 )
		for(it = m_FontPool.begin(); it != m_FontPool.end(); it ++)
		{
			(*it)->OnLostDevice();
		}
	if(m_Sprite)
		m_Sprite->OnLostDevice();
	m_bLostDevice = true;
}

FONTID gbFontManager::NewFont(const char* pcFontName,int iSize,DWORD style)
{
	FONTID FontID = m_FontPool.size();

	//////////////////////////////////////////////////////////////////////////
	
	ID3DXFont* pFont = NULL;
	D3DXCreateFont( m_pd3dDevice,
		iSize, 
		0, 
		style&FONT_BOLD?FW_BOLD:FW_NORMAL, 
		1, 
		style&FONT_ITALIC, 
		DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, 
		DEFAULT_QUALITY, 
		DEFAULT_PITCH | FF_DONTCARE, 
		pcFontName, 
		&pFont );

	m_FontPool.push_back(pFont);
	return FontID;
}

void gbFontManager::SetCurFont(FONTID fontID)
{
	m_CurFontID = fontID;
}

FONTID gbFontManager::GetCurFont()
{
	return m_CurFontID;
}

void gbFontManager::SetFontColor(NiColorA color)
{
	m_CurColor = color;
}

NiColorA gbFontManager::GetCurColor()
{
	return m_CurColor;
}

void gbFontManager::GetCharacterData(FONTID fontID,unsigned int Glyph,NiTexture* & pTex,gbRect& gRect)
{
	//LPDIRECT3DTEXTURE9  pTexture = NULL;
	//RECT BlackBox;
	//GetDXFont(fontID)->GetGlyphData(Glyph,&pTexture,&BlackBox,NULL);

	//NiDX9Renderer *pDX9Renderer = NULL;
	//pDX9Renderer = NiSmartPointerCast(NiDX9Renderer,NiRenderer::GetRenderer());
	//NiTexturePtr spTex = pDX9Renderer->CreateNiTextureFromD3DTexture(pTexture);
	//pTex = spTex;
	//pTex->IncRefCount();
	//gRect.iTop = BlackBox.top;
	//gRect.iLeft = BlackBox.left;
	//gRect.iHeight = BlackBox.bottom - BlackBox.top;
	//gRect.iWidth = BlackBox.right - BlackBox.left;
}

void gbFontManager::EstimateCharacter(FONTID fontID,unsigned int uiCharacter,int& iWidth,int& iHeight)
{
	//LPDIRECT3DTEXTURE9 * pTexture = NULL;
	//RECT BlackBox;
	//GetDXFont(fontID)->GetGlyphData(uiCharacter,pTexture,&BlackBox,NULL);
	//
	//iHeight = BlackBox.bottom - BlackBox.top;
	//iWidth = BlackBox.right - BlackBox.left;
	char pcText[4];
	Int_Str(uiCharacter,pcText);
	EstimateString(fontID,pcText,iWidth,iHeight);
}

void gbFontManager::EstimateString(FONTID fontID,const char* pcString,int& iWidth,int& iHeight)
{
	
	//////////////////////////////////////////////////////////////////////////
	const char* pcEnd = pcString+strlen(pcString)-1;
	int iSpaceLen = 0;
	while(pcEnd)
	{
		if(pcEnd[0] == ' ')
			iSpaceLen++;
		else if(pcEnd[0] == '\t')
			iSpaceLen += 4;
		else
			break;
		pcEnd --;
	}
	DWORD dwTextFormat = DT_NOCLIP | DT_CALCRECT;
	RECT rcDest;
	RECT* prcDest = &rcDest;
	D3DXFONT_DESC descfont;
	GetDXFont(fontID)->GetDesc(&descfont);
	// Since we are only computing the rectangle, we don't need a sprite.
	iSpaceLen *= (descfont.Height/2);
	GetDXFont(fontID)->DrawText( NULL, pcString, -1, prcDest, dwTextFormat,0x0 );
	iWidth = (int)(prcDest->right - prcDest->left)+iSpaceLen;
	iHeight = (int)(prcDest->bottom - prcDest->top);
}
void gbFontManager::GetCutPoint(FONTID fontID,const char* pcString,int iWidth,int& iPos)
{
	ID3DXFont* pFont = GetDXFont(fontID);
	SIZE OutSize;
	GetTextExtentExPoint(pFont->GetDC(),LPCSTR(pcString),strlen(pcString),iWidth,&iPos,NULL,&OutSize);
}

void gbFontManager::TryStartSprite()
{
	if(!bOnSpriteFly)
	{
		GetSprite()->Begin(D3DXSPRITE_ALPHABLEND);
		m_pd3dDevice->SetRenderState( D3DRS_ZENABLE ,false);
		bOnSpriteFly = true;
	}
}

void gbFontManager::TryEndSprite()
{
	if(bOnSpriteFly)
	{
		GetSprite()->End();
		m_pd3dDevice->SetRenderState( D3DRS_ZENABLE ,true);
		bOnSpriteFly = false;
	}
}

int gbFontManager::Str_Int( const char *str, unsigned int *hz)
{
	BYTE *s= (BYTE *)str;
	int i=0;
	while( *s )	
	{
		i++;	assert(i<255);
		if( *s < 0x81 )
		{	// Ascii×Ö·û
			*hz= *s;
			s++;
			hz++;
		}
		else
		{			// ºº×Ö
			if (0==s[1])
			{	//end at half hanzi
				*hz = '#';	//
				hz++;
				break;
			}
			*hz= s[0]*256L + s[1];		
			s+=2;
			hz++;
		}
	}
	*hz= 0;		//zero end
	return i;	//×Ö·û¸öÊý£¬
}

void gbFontManager::Int_Str(DWORD hz,char* cstr)
{
	BYTE* str = (BYTE*)(cstr);
	if(hz < 0x81)
	{
		str[0] = (unsigned char)hz; 
		str[1] = '\0';
	}
	else
	{
		str[1] = (BYTE)hz&0xff;hz >>= 8;
		str[0] = (BYTE)hz&0xff;
		str[2] = '\0';
	}
}

