#ifndef __GBUIINTERFACE_H__
#define __GBUIINTERFACE_H__

#include "NiDX9Renderer.h"
#include "IUIRenderer.h"
#include "NiTexturingProperty.h"
#include "NiScreenElements.h"
#include "d3dx9math.h"
#include <vector>

#include "FBGUI/FBGUI.h"
#pragma warning(disable:4099) 

using namespace FBGUI ;

class gbTextureManager;
class gbFontManager;
class gbRenderHelper;

struct gbRect
{
	int iTop;
	int iLeft;
	int iHeight;
	int iWidth;
};

struct Billboard3DElem
{
	float fX;
	float fY;
	float fZ;
	unsigned int uiID;
	DWORD Color;
	char Info[256];
};

//It's for Dirty Rectangle division
const int DIRTY_RECT_WIDTH = 32 ;
const int DIRTY_RECT_HEIGHT = 32 ;


class gbUIRenderer : public IUIRenderer
{
public:
#ifndef ENGINE_NEW
	static gbUIRenderer* GetRenderer(){ return ms_pUIRender?ms_pUIRender:new gbUIRenderer();}
	static gbUIRenderer* GetRenderer(HWND hRenderWindow){ return ms_pUIRender?ms_pUIRender:new gbUIRenderer(hRenderWindow);}
    static void SetUIRenderer(IUIRenderer *pUIRenderer) ;
#else
	static gbUIRenderer* GetRenderer();
	static gbUIRenderer* GetRenderer(HWND hRenderWindow) ;
    static void SetUIRenderer(IUIRenderer *pUIRenderer) ;
#endif

//	virtual int Release();

	//Image
	virtual IMGID CreateImage(const char *path) ;
	virtual IMGID CreateImage(int uiWidth, int uiHeight,int attrib) ;
	virtual IMGID CreateRenderImage(int uiWidth, int uiHeight) ;

	virtual IMGID MakeGrayImage(IMGID ImageID);

	virtual void SetImageUsedRect(IMGID ImageID,int iTop,int iLeft,int iHeight,int iWidth);
	virtual void SetImageColor(IMGID ImageID,DWORD color);
	virtual void UpdateImage(IMGID ImageID,int iTop,int iLeft,TBitImage* pImg) ;
	virtual void UpdateImage(IMGID ImageID,int iTop,int iLeft,TBitImage* pImg , RECT *rcRegion) ;//局部更新一个纹理
	virtual void ClearImage(IMGID ImageID , DWORD dwColor) ; //clear the image and fill it with the specified color
    virtual bool HaveAlphaChannels(IMGID ImageID) ;

	virtual void ReleaseImage(IMGID ImageID) ;

	virtual int GetHeight(IMGID ImageID) ;
	virtual int GetWidth(IMGID ImageID) ;
	virtual float GetPixelAlpha(IMGID ImageID,unsigned int uiX,unsigned int uiY);
	virtual DWORD GetPixelColor(IMGID ImageID, unsigned int uiX, unsigned int uiY)  ; 

	//Font
	virtual void CreateFont( const char* pFontName, const char* pFontFilePath, int nFontHeight, bool bAnsiAliased, 
	                       	 bool bUnderline, bool bBold, bool bItalic, int nRowSpacing ) ;

	virtual void CreateFont( const char* pFontName, const char* pFontFilePath, int nFontHeight, bool bAnsiAliased, 
		                     bool bUnderline, bool bBold, bool bItalic, int nRowSpacing ,
		                     int nShadowOffsetX,    int nShadowOffsetY, DWORD dwShadowColor) ;

	virtual void CreateFont( const char* pFontName, const char* pFontFilePath, int nFontHeight, bool bAnsiAliased, 
		                     bool bUnderline, bool bBold, bool bItalic, int nRowSpacing ,
		                     unsigned int nEdgeWidth, DWORD EdgeColor ) ;

	virtual void CreateFont( const char* pFontName,  const char* pFontFilePath, int nFontHeight, bool bAnsiAliased, 
		                     bool bUnderline, bool bBold, bool bItalic, int nRowSpacing ,
		                     int nShadowOffsetX,   int nShadowOffsetY, DWORD dwShadowColor, unsigned int nEdgeWidth,   
		                     DWORD EdgeColor) ;

	virtual void SetCurrentFontName(const char* pFontName);
	virtual const char* GetCurrentFontName();
	virtual void DestroyFont(const char* pFontName) ;

	virtual void SetCurrentTextFrontColor(DWORD dwColor);
	virtual void SetCurrentTextBackColor(DWORD dwColor) ;
	virtual void SetCurrentTextShadowColor(DWORD dwColor);
	virtual void SetCurrentTextEdgeColor(DWORD dwColor);
	virtual void SetCurrentTextFrontAndBackAlpha(unsigned int nAlpha) ;

	virtual DWORD GetCurrentTextFrontColor();
	virtual DWORD GetCurrentTextBackColor() ;
	virtual DWORD GetCurrentTextShadowColor();
	virtual DWORD GetCurrentTextEdgeColor();
	virtual DWORD GetCurrentTextAlpha()  ;

	virtual void DrawText(unsigned int nDestLeft, unsigned int nDestTop, const char* szText, RECT *rcClip) ;
	virtual void DrawText( const char* pText, const RECT& rDestRect, RECT *rcClip, unsigned int vFormat ) ;

	virtual int  GetTextWidth( const char* pText, int nTextCount )  ;
	virtual int  GetTextHeight() ;

	virtual void Reg3DImage(IMGID ImageID,int iHeight,int iWidth,float fX,float fY,float fZ);

	virtual void EstimateCharacter(FONTID fontID,unsigned int uiCharacter,int& iWidth,int& iHeight) ;
	virtual void EstimateString(FONTID fontID,const char* pcString,int& iWidth,int& iHeight) ;
	virtual void GetCutPoint(FONTID fontID,const char* pcString,int iWidth,int& iPos);

	//clip
	virtual void SetClipRect( int iTop, int iLeft, int iHeight, int iWidth);
	virtual void GetClipRect( int&iTop, int&iLeft, int&iHeight, int&iWidth);
	virtual void EnableClipRect(bool flag); 
	virtual bool IsClipEnable(); 

	//draw
	virtual void SetDrawFlags(BlendMode flags);
	virtual void DrawLine(int iXTop,int iXLeft,int iYTop,int iYLeft,DWORD color);
	virtual void DrawCharacter(unsigned int Glyph,int iTop,int iLeft);
	virtual void DrawString(const char* pcText,int iTop,int iLeft);
	virtual void DrawRect(int iTop,int iLeft,int iHeight,int iWidth,DWORD color,bool bNeedFill);
	virtual void DrawPolygon(int* pVertex,int iNumVertices,DWORD color,bool bNeedFill);
	virtual void DrawImage(IMGID ImageID,int iTop,int iLeft,int iHeight,int iWidth);
	virtual bool DrawImage(IMGID ImageID, FillMode enFillMode, RECT* rcScr, RECT* rcDest, unsigned char byImageAlphaSet) ;
	virtual void DrawImageWithMask(IMGID ImageID, IMGID ImageIDMask, int iLeftDes,int iTopDes, int iWidthDes, int iHeightDes );

	virtual void BeginRenderText()  ;
	virtual void EndRenderText() ;

	NiColorA  ToNiColorA(DWORD color);
	D3DXCOLOR ToD3DColor(DWORD color);

	virtual void BeginUI();
	virtual void EndUI();
	void Draw3DElem(NiPoint3 CamPos);

	bool OnUILostDevice(void* pvData);
	bool OnUIResetDevice(bool bBeforeReset,void* pvData);

	// Dirty Rectangle solution for User Interface Rendering
	void CreateUIScreenBlock(int nScreenWidth, int nScreenHeight) ;
    void UpdateUIScreen( RECT *pDirtyRectList, int nRectCount, TBitImage *pScreenBuffer) ;

	gbUIRenderer();
	gbUIRenderer(HWND hRenderWindow) ;
	~gbUIRenderer();

protected:

	 void InitRenderer() ;

	void ChangePointCoordWithClip(float& fX,float& fY);
	void ChangeLengthtCoordWithClip(float& fW,float& fH);
	void Clamp(int& iSrc,int iMax, int iMin);
	gbTextureManager* m_pTexManager;
	gbFontManager* m_pFontManager;
	gbRenderHelper* m_pRenderHelper;

	gbRect m_ClipRect;
	bool m_bEnableClip;
	NiTexturingProperty::ApplyMode m_eApplyMode;
	
	std::vector<Billboard3DElem> m_3DSet;
	float m_fRScreenY;
	float m_fRScreenX;

	unsigned int m_uiRenderHeight;
	unsigned int m_uiRenderWidth;

	IMGID m_uiWhite;
	static gbUIRenderer* ms_pUIRender;

	CGUIRenderDriver* m_pRender;
	CGUICanvas*  m_pFontCanvas;
	CGUISystem*  m_pGUISystem;

	//for Dirty rectangle screen update
	IMGID    m_imgScreenUpdateBlock ;
	int      m_nNumScreenBlock ;
	int      m_nScreenBlockXCount ;
	int      m_nScreenBlockYCount ;
	std::vector<bool> m_vScreenBlockUpdateMark ; 
};

IUIRenderer *GetUIRenderer();
void SetUIRenderer(IUIRenderer *pUIRenderer);

IUIRenderer *CreateUIRenderer(HWND hRenderWindow) ;
#endif //__GBUIINTERFACE_H__