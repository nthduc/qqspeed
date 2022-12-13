#include "Top-KartPCH.h"
#include "changeColor.h"
#include "math.h"
#include "NiGeometry.h"
#include "common.h"

#define COLOR_LIST_FILENAME "colorlist.dat"
/*
struct SColorAttr{
    int		hue;
    float	sat;
    float	val;
};

struct SColorAttrCustom
{
	int nIndex;
	SColorAttr colorList[8];
};
*/
#pragma warning(disable: 4305)

#define COLORLIST_CAPACITY	20

static SColorAttr defcolorList[8]={     
	
 /*红*/   0,   1.2,   3.5,   
 /*蓝*/ 200,   2,     3,   
 /*黄*/  40,   1.5,   3,   
 /*绿*/ 125,   1,     1.8,   
 /*桃*/ 320,   1.5,   3,   
 /*橙*/  20,   1.2,   3,   
 /*紫*/ 280,   1.5,   3,   
 /*灰*/   0,   0,	  1.2
 
};

static SColorAttrCustom colorList[COLORLIST_CAPACITY] = { 0 };

SColorAttr GetColorAttr(int nRoleIndex, int nColorIndex)
{
	for ( int i = 0; i < COLORLIST_CAPACITY; i++ )
	{
		if ( colorList[i].nIndex == nRoleIndex )
		{
			return colorList[i].colorList[nColorIndex];
		}
	}
	// 如果没有定制特定人物的，就用0号的(公用)
	for ( i = 0; i < COLORLIST_CAPACITY; i++ )
	{
		if ( colorList[i].nIndex == 0 )
		{
			return colorList[i].colorList[nColorIndex];
		}
	}	
	return defcolorList[nColorIndex];
}

bool LoadColorListFromFile(const char* fname)
{
	static bool bAlreadyLoaded = false;
	static bool bDynamicRefresh = false;

	if ( bAlreadyLoaded && !bDynamicRefresh )
		return false;

	const int nCount = 8;
	static char* szcolors[nCount] = { "红",  "蓝", "黄", "绿", "桃", "橙", "紫", "灰" };
	char inifile[MAX_PATH] = COLOR_LIST_FILENAME;
	char section[20] = "\0";
	::_fullpath(inifile, inifile, _MAX_PATH);
	for ( int i = 0; i < COLORLIST_CAPACITY; i++ )
	{
		_snprintf(section, 20, "Player%d", i);	//[Player0]给大家公用
		for ( int j = 0; j < nCount; j++)
		{
			char returnstring[100] = "\0";
			GetPrivateProfileString(section, szcolors[j], "", returnstring, 100, inifile);
			if ( !strlen(returnstring) )
				break;
			_snscanf(returnstring, 100, "%d,%f,%f", &colorList[i].colorList[j].hue, &colorList[i].colorList[j].sat, &colorList[i].colorList[j].val);
			colorList[i].nIndex = i;
		}
	}
	bAlreadyLoaded = true;
	bDynamicRefresh = GetPrivateProfileInt("Options", "DynamicRefresh", 0, inifile);
	return true;
}

int rgb2gray( int r, int g, int b) {
    return (BYTE)( (r * 30 + g * 59 + b * 11) / 100 );
}

// From QColor.
void rgb2hsv(int r, int g, int b, int & h, int & s, int & v)
{
    
    unsigned int mx = r;                          // mximum RGB component
    int whatmx = 0;                               // r=>0, g=>1, b=>2
    if ( (unsigned int)g > mx ) {
        mx = g;
        whatmx = 1;
    }
    if ( (unsigned int)b > mx ) {
        mx = b;
        whatmx = 2;
    }
    unsigned int mn = r;                          // find mnimum value
    if ( (unsigned int)g < mn ) mn = g;
    if ( (unsigned int)b < mn ) mn = b;
    int delta = mx-mn;
    v = mx;                                       // calc value
    s = mx ? (510*delta+mx)/(2*mx) : 0;
    if ( s == 0 ) {
        h = -1;                             // undefined hue
    } else {
        switch ( whatmx ) {
        case 0:                             // red is mx component
            if ( g >= b )
                h = (120*(g-b)+delta)/(2*delta);
            else
                h = (120*(g-b+delta)+delta)/(2*delta) + 300;
            break;
        case 1:                             // green is mx component
            if ( b > r )
                h = 120 + (120*(b-r)+delta)/(2*delta);
            else
                h = 60 + (120*(b-r+delta)+delta)/(2*delta);
            break;
        case 2:                             // blue is mx component
            if ( r > g )
                h = 240 + (120*(r-g)+delta)/(2*delta);
            else
                h = 180 + (120*(r-g+delta)+delta)/(2*delta);
            break;
        }
    }
}

//bool hsv2rgb( int h, int s, int v ,int& r,int& g, int& b)
bool hsv2rgb( int h, int s, int v ,BYTE& r,BYTE& g, BYTE& b)
{
    if (s > 255) s = 255;
    if (v > 255) v = 255;
    
    if ( h < -1 || (unsigned int)s > 255 || (unsigned int)v > 255 ) {
        r = g = b = 0;
        return false;
    }
    r=v;
    g=v;
    b=v;
    if ( s == 0 || h == -1 ) {                    // achromatic case
        // Ignore
    } else {                                      // chromatic case
        if ( (unsigned int)h >= 360 ) h %= 360;
        unsigned int f = h%60;
        h /= 60;
        unsigned int p = (unsigned int)(2*v*(255-s)+255)/510;
        unsigned int q, t;
        if ( h&1 ) {
            q = (unsigned int)(2*v*(15300-s*f)+15300)/30600;
            switch( h ) {
            case 1: r=(int)q; g=(int)v, b=(int)p; break;
            case 3: r=(int)p; g=(int)q, b=(int)v; break;
            case 5: r=(int)v; g=(int)p, b=(int)q; break;
            }
        } else {
            t = (unsigned int)(2*v*(15300-(s*(60-f)))+15300)/30600;
            switch( h ) {
            case 0: r=(int)v; g=(int)t, b=(int)p; break;
            case 2: r=(int)p; g=(int)v, b=(int)t; break;
            case 4: r=(int)t; g=(int)p, b=(int)v; break;
            }
        }
    }
    return true;
}

void changeColor( NiGeometry* pGeometry, int roleIdx, int colorIdx,SColorAttr* pClrAttr/*=NULL*/)
{
    if (!pGeometry)	return;
    if (colorIdx<0 || colorIdx>=9) return;

	LoadColorListFromFile(COLOR_LIST_FILENAME);
	SColorAttr ca;
	if (pClrAttr)
	{
		ca=*pClrAttr;
	} 
	else
	{
		ca = GetColorAttr(roleIdx, colorIdx);
	}

	NiDX9Renderer* pRender = (NiDX9Renderer*) NiRenderer::GetRenderer();
	LPDIRECT3DDEVICE9 pD3DDevice9 = pRender->GetD3DDevice();
	LPDIRECT3DTEXTURE9 pNewD3DTexture = NULL;
	NiTexturingProperty* pTextureProp = NiDynamicCast(NiTexturingProperty, pGeometry->GetProperty(NiProperty::TEXTURING));
	{
		KK_CHECK_POINTER(pTextureProp);
		NiSourceTexture* pTexture = NiDynamicCast(NiSourceTexture, pTextureProp->GetBaseTexture());
		const NiFixedString kTextureNameStr = pTexture->GetPlatformSpecificFilename();
		char strMaskTextureName[MAX_PATH] = "";
		strcat(strMaskTextureName, kTextureNameStr);
		strcat(strMaskTextureName, ".a.dds");

		if ( FAILED(D3DXCreateTextureFromFile(pD3DDevice9, 
			strMaskTextureName, 
			&pNewD3DTexture)) )
		{
			XWaring("D3DXCreateTextureFromFile(%s) failed, cannot change color", kTextureNameStr);
			goto KK_EXIT;
		}
		
		NiDX9TextureData* pTextureData = NiDynamicCast(NiDX9TextureData, pTexture->GetRendererData());
		KK_CHECK_POINTER(pTextureData);
		IDirect3DTexture9* pD3DTexture = (IDirect3DTexture9*)pTextureData->GetD3DTexture();
		KK_CHECK_POINTER(pD3DTexture);
		if ( pD3DTexture->GetLevelCount() != pNewD3DTexture->GetLevelCount() )
		{
			XWaring("GetLevelCount() not equal, cannot change color");
			goto KK_EXIT;
		}
		for ( int i = 0; i < pD3DTexture->GetLevelCount(); i++ )
		{
			D3DSURFACE_DESC kSurfDesc;
			pD3DTexture->GetLevelDesc(i, &kSurfDesc);
			D3DSURFACE_DESC kNewSurfDesc;
			pNewD3DTexture->GetLevelDesc(i, &kNewSurfDesc);

			bool b32BitSrc = false;
			if ( kSurfDesc.Format == D3DFMT_A8R8G8B8 
				|| kSurfDesc.Format == D3DFMT_X8R8G8B8 )
			{
				b32BitSrc = true;
			}
			bool b32BitDest = false;
			if ( kNewSurfDesc.Format == D3DFMT_A8R8G8B8 
				|| kNewSurfDesc.Format == D3DFMT_X8R8G8B8 )
			{
				b32BitDest = true;
			}
			if ( !b32BitSrc || !b32BitDest ||
				( kSurfDesc.Width != kNewSurfDesc.Width ) ||
				( kSurfDesc.Height != kNewSurfDesc.Height ) )
			{
				XWaring("mask texture not the same, cannot change color");
				goto KK_EXIT;
			}

			D3DLOCKED_RECT kLockedRect;
			if ( FAILED(pD3DTexture->LockRect(i, &kLockedRect, NULL, 0)) )
			{
				XWaring("pD3DTexture->LockRect() failed, cannot change color");
				goto KK_EXIT;
			}

			D3DLOCKED_RECT kNewLockedRect;
			if ( FAILED(pNewD3DTexture->LockRect(i, &kNewLockedRect, NULL, 0)) )
			{
				XWaring("pNewD3DTexture->LockRect() failed, cannot change color");
				goto KK_EXIT;
			}

			DWORD* pBits = (DWORD*)kLockedRect.pBits;
			DWORD* pNewBits = (DWORD*)kNewLockedRect.pBits;
			for ( int y = 0; y < kSurfDesc.Height; y++ )
			{
				for ( int x = 0; x < kLockedRect.Pitch / 4; x++ )
				{
					DWORD oc = pBits[y * kLockedRect.Pitch / 4 + x];
					DWORD mc = pNewBits[y * kLockedRect.Pitch / 4 + x];
					DWORD oa = oc & 0xff000000;
					BYTE a = (mc & 0xff000000) >> 24;
					BYTE r = (mc & 0xff0000) >> 16;
					BYTE g = (mc & 0xff00) >> 8;
					BYTE b = mc & 0xff;
					int gray = rgb2gray( r,g,b);    //方案1        
					//BYTE gray = a;
					if ( gray < 250 )
					{
						hsv2rgb(ca.hue, (255 - gray) * ca.sat, gray * ca.val, r, g, b);  
						DWORD c = oa + (r << 16) + (g << 8) + b;
						pBits[y * kLockedRect.Pitch / 4 + x] = c;
					}
				}
			}
			pD3DTexture->UnlockRect(i);
			pNewD3DTexture->UnlockRect(i);
		}
	}
KK_EXIT:
	if ( pNewD3DTexture )
	{
		pNewD3DTexture->Release();
		pNewD3DTexture = NULL;
	}
	return;
}
