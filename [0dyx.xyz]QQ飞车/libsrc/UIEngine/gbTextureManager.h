
#ifndef __gbTextureManager_H__
#define __gbTextureManager_H__

#include "gbUIRenderer.h"
#include "NiTexture.h"
#define WHITE_SIZE 2
#include <windows.h>
#include <string>


struct TextureRect
{
	//unsigned int uiIndex;
	//NiScreenTexturePtr spScreenTexture;

	NiTexturePtr spTexture;
	bool bIsChanged;
	int attrib;
	int iLeft;
	int iTop;
	int iWidth;
	int iHeight;
	DWORD dwColor;
	std::string strFileName ;
};

typedef NiTPrimitiveArray<TextureRect*> TextureArray;
class gbTextureManager
{
public:
	~gbTextureManager();
	static gbTextureManager* GetManager();

	bool Load( const char *path,unsigned int& TextureID );

	bool CreateSourceTexture(unsigned int uiWidth, unsigned int uiHeight,int  attrib,unsigned int& TextureID);//创建一个纹理
	void UpdateSourceTexture(unsigned int uiTop,unsigned int uiLeft,TBitImage* pImg,unsigned int& TextureID);//update一个纹理
	void UpdateSourceTexture(unsigned int uiTop,unsigned int uiLeft,TBitImage* pImg, unsigned int& TextureID ,RECT *rcRegion );//局部update纹理
	void ClearSourceTexture( unsigned int& TextureID , DWORD dwColor ) ;// clear a texture

	IMGID CreateRenderImage(int uiWidth, int uiHeight);
	IMGID MakeGrayImage(IMGID ImageID);
	float GetPixelAlpha(IMGID ImageID,unsigned int uiX,unsigned int uiY);
	DWORD GetPixelColor(IMGID ImageID, unsigned int uiX, unsigned int uiY)  ; 
    bool  HaveAlphaChannels(IMGID ImageID) ;

	bool ReleaseTexture(unsigned int TextureID);

	unsigned int GetHeight(unsigned int TextureID);
	unsigned int GetWidth(unsigned int TextureID);

	TextureRect* GetTextureRect(unsigned int TextureID);

	IMGID CreateWhite();

	void OnLostDevice();
	void OnResetDevice();
private:
	gbTextureManager();

	TextureArray m_TexturePool;
	static gbTextureManager* ms_pManager;
};

#endif //__gbTextureManager_H__