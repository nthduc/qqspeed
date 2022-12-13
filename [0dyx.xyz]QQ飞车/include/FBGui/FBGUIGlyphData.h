#pragma once
#include "FBGUIRect.h"
#include "FBGUITexture.h"
#include "FBGUIFont.h"
namespace FBGUI
{

	// ************************************************************ //
	// CGUIGlyphData
	// ************************************************************ //
    const int FONT_GLYPH_TEXTURE_HEIGHT = 1024 ;
	const int FONT_GLYPH_TEXTURE_WIDTH  = 1024 ;
	class CGUIGlyphData
	{
		friend class CGUIFont;

	protected:
		CGUIRect		m_GlyphRect;		//! 字形在纹理的位置
		int				m_AdvanceWidth;	//! 字形逻辑宽度
		bool			m_IsCreated;		//! 是否字体已经建立
		wchar_t			m_Unicode;		//! UNICODE编码
		CGUIFont*		m_pFont;			//! 字体描述
		std::list<CGUIGlyphData*>::iterator m_iter;

	public:
		CGUIGlyphData( ) :  m_AdvanceWidth( 0 ), m_IsCreated( false ), m_Unicode( 0 ), m_pFont( NULL ) {	}

	protected:
		int CreateGlyph( );
		int FillFont( const CGUIRect& rRect,bool bNeedClean );
		void SetContext( CGUIFont* tpFont, wchar_t tUnicode ) 
		{ 
			m_pFont = tpFont; 
			m_Unicode = tUnicode;
		}

	public:
		/*
		//! \brief 得到字形纹理，pText是多字节，指向一个字符
		\return 得到字形所在的纹理
		*/
		CGUITexture* GetGlyphTexture( );	
		CGUITexture* GetGlyphShadowTexture();
		CGUITexture* GetGlyphEdgeTexture();

		/*
		//! \brief 得到逻辑宽度，pText是多字节，指向一个字符
		\return 得到字形宽度
		*/
		int GetGlyphWidth( );										

		/*
		//! \brief	得到字形在纹理中的位置，pText是多字节，指向一个字符, 
		这里的宽度和GetGlyphWidth( )不一样，因为不是所有的字符一样高，所以GetGlyphRect( )将得到包含最大字符的CGUIRect
		\return 字符的CGUIRect
		*/
		CGUIRect GetGlyphRect( );									
	};
}
