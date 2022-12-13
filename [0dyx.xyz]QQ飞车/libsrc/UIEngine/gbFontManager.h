#pragma once
#include <d3dx9core.h>
#include <vector>
#include "NiColor.h"
#include "gbUIRenderer.h"

typedef std::vector<ID3DXFont*> FontPool;
class gbFontManager
{
public:
	static gbFontManager* GetManager();
	~gbFontManager(void);

	FONTID NewFont(const char* pcFontName,int iSize,DWORD style);
	void SetCurFont(FONTID fontID);
	FONTID GetCurFont();
	void SetFontColor(NiColorA color);
	NiColorA GetCurColor();

	void GetCharacterData(FONTID fontID,unsigned int Glyph,NiTexture* & pTex,gbRect& gRect);
	void EstimateCharacter(FONTID fontID,unsigned int uiCharacter,int& iWidth,int& iHeight);
	void EstimateString(FONTID fontID,const char* pcString,int& iWidth,int& iHeight);
	void GetCutPoint(FONTID fontID,const char* pcString,int iWidth,int& iPos);

	int Str_Int( const char *str, unsigned int *hz);
	void Int_Str( DWORD hz,char* str);

	void TryStartSprite();
	void TryEndSprite();

	void OnRestDevice();
	void OnLostDevice();
	bool LostDevice(){return m_bLostDevice;}

	ID3DXFont* GetDXFont(unsigned int uiFontID){
		assert(uiFontID>=0&&uiFontID<m_FontPool.size());
		return m_FontPool[uiFontID];
	}
	ID3DXSprite* GetSprite(){return m_Sprite;}
	IDirect3DDevice9* GetDx9Device(){return m_pd3dDevice;}
	
protected:
	gbFontManager(void);


	FONTID m_CurFontID;
	NiColorA m_CurColor;
	FontPool  m_FontPool;
	bool bOnSpriteFly;
	ID3DXSprite * m_Sprite;
	IDirect3DDevice9* m_pd3dDevice;

	bool m_bLostDevice;
	static gbFontManager* ms_pManager;
};							