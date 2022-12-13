#pragma once
#include "FBGUITexture.h"
#include "FBGUIRect.h"
#include <D3d9.h>
#include <vector>
#include "FBGUIFontVertex.h"

namespace FBGUI
{
	// ************************************************************ //
	// CGUIRenderDriver 渲染驱动程序
	// ************************************************************ //

	class CGUIRenderDriver
	{
	public:
		CGUIRenderDriver( )	;
	public:
		virtual ~CGUIRenderDriver( ) ;  

	public:
		// 准备渲染状态
		void PrepareRender();
		// 渲染后处理
		// int Render( ) = 0;
		// 填充三角形
		// int FillTriangle( const CGUIPoint& rPoint1, const CGUIPoint& rPoint2, const CGUIPoint& rPoint3, unsigned int vColor ) = 0;
		// 画直线
		int DrawLine( const CGUIPoint& point1, const CGUIPoint& point2) ;
		// 画矩形
		// int DrawRect( const CGUIRect& rDestRect, unsigned int vTopLeftColor, unsigned int vBottomRightColor ) = 0;
		// 填充矩形
		//int FillRect( const CGUIRect& rDestRect, unsigned int vColor ) = 0;
		// 贴图
		int AddTextToRenderBuffer( const CGUIRect& rSrcRect, const CGUIRect& rDestRect, float fHorShear = 0.0f) ;
		int DrawTextRenderBuffer(RECT *rcClip)  ;
		
		// 装载纹理
		// CGUITexture* LoadTexture( const char* pTexName ) = 0;
		// 建立纹理
		CGUITexture* CreateTexture( const char* pTexName, int vWidth, int vHeight ) ;
		void SetCurrentTexture(CGUITexture* pTexture ) ;
		void SetSystemColor(DWORD dwColor);

		void EndRender();

	public:
		LPDIRECT3DDEVICE9       m_pd3dDevice;
        LPDIRECT3DVERTEXBUFFER9 m_pVB      ;
		LPDIRECT3DVERTEXBUFFER9 m_pLine ;
		CGUITexture*            m_pFontTexture  ;
		LPDIRECT3DTEXTURE9      m_TCasheTexture;
		CGUITexture*            m_pRenderTexture  ;
		DWORD                   m_dwSystemColor ;

		std::vector<FONT2DVERTEX> m_vecVertexBuffer ;
	};
}