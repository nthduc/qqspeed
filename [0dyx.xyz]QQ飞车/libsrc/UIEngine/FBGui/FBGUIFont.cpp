#include "FBGUIFont.h"
#include "FBGUIGlyphData.h"
#include "FBGUICanvas.h"

namespace FBGUI
{
	bool                m_bBold           ;    // Bold?
	bool                m_Italic          ;    // Italic?

	// ************************************************************ //
	// CGUIFont
	// ************************************************************ //
	FT_Library CGUIFont::s_pFTLibrary = NULL;
	int		   CGUIFont::s_FTRef = 0;

	CGUIFont::CGUIFont( const char* pFontName, const char* pFontFilePath, int nFontHeight, bool bAnsiAliased, 
		        CGUICodePage* pCharMap, bool bUnderline, bool bBold, bool bItalic, int nRowSpacing) :
	          m_Dpi( 96 ),            m_nEdgeWidth( 0 ),     m_nShadowOffsetX( 0 ),        m_nShadowOffsetY( 0 ), 
			  m_pCharMap( pCharMap ), m_FontUsed( 0 ), m_FaceFilePath( pFontFilePath ),   m_FontName( pFontName ),
			  m_AntiAliased( bAnsiAliased ), m_pTexture(NULL), m_pShadowTexture(NULL), m_pEdgeTexture(NULL),
	          m_dwTextFrontColor(0), m_dwTextBackColor(0), m_dwShadowColor(0), m_dwEdgeColor(0), m_nRowSpacing(nRowSpacing),
			  m_bHasShadow(false), m_bHasEdge(false),m_nFontHeight(nFontHeight), m_bUnderline(bUnderline), m_bBold(bBold) ,m_bItalic(bItalic)
	{
         InitFreeTypeFont();
	}

	CGUIFont::CGUIFont( const char* pFontName, const char* pFontFilePath, int nFontHeight, bool bAnsiAliased, 
		                CGUICodePage* pCharMap, bool bUnderline, bool bBold, bool bItalic, int nRowSpacing, int nShadowOffsetX,    
		                int nShadowOffsetY, DWORD dwShadowColor) :
	    m_Dpi( 96 ),            m_nEdgeWidth( 0 ),     m_nShadowOffsetX( nShadowOffsetX ),        m_nShadowOffsetY( nShadowOffsetY ), 
		m_pCharMap( pCharMap ), m_FontUsed( 0 ), m_FaceFilePath( pFontFilePath ),   m_FontName( pFontName ),
		m_AntiAliased( bAnsiAliased ), m_pTexture(NULL), m_pShadowTexture(NULL), m_pEdgeTexture(NULL),
		m_dwTextFrontColor(0), m_dwTextBackColor(0), m_dwShadowColor(dwShadowColor), m_dwEdgeColor(0),m_nRowSpacing(nRowSpacing),
		m_bHasShadow(true), m_bHasEdge(false), m_nFontHeight(nFontHeight), m_bUnderline(bUnderline), m_bBold(bBold) ,m_bItalic(bItalic)
	{
			InitFreeTypeFont();
	}

	CGUIFont::CGUIFont( const char* pFontName, const char* pFontFilePath, int nFontHeight, bool bAnsiAliased, 
			            CGUICodePage* pCharMap, bool bUnderline, bool bBold, bool bItalic, int nRowSpacing,  unsigned int nEdgeWidth, DWORD EdgeColor ):
	    m_Dpi( 96 ),            m_nEdgeWidth( nEdgeWidth ),     m_nShadowOffsetX( 0 ),       m_nShadowOffsetY( 0 ), 
		m_pCharMap( pCharMap ), m_FontUsed( 0 ), m_FaceFilePath( pFontFilePath ),   m_FontName( pFontName ),
		m_AntiAliased( bAnsiAliased ), m_pTexture(NULL), m_pShadowTexture(NULL), m_pEdgeTexture(NULL),
		m_dwTextFrontColor(0), m_dwTextBackColor(0), m_dwShadowColor(0), m_dwEdgeColor(EdgeColor),m_nRowSpacing(nRowSpacing),
		m_bHasShadow(false), m_bHasEdge(true), m_nFontHeight(nFontHeight), m_bUnderline(bUnderline), m_bBold(bBold) ,m_bItalic(bItalic)
	{
		InitFreeTypeFont();
	}

	CGUIFont::CGUIFont( const char* pFontName,  const char* pFontFilePath, int nFontHeight, bool bAnsiAliased, 
			            CGUICodePage* pCharMap,  bool bUnderline, bool bBold, bool bItalic, int nRowSpacing,  int nShadowOffsetX, int nShadowOffsetY,    
			            DWORD dwShadowColor,    unsigned int nEdgeWidth,   DWORD EdgeColor) :
	m_Dpi( 96 ),            m_nEdgeWidth( nEdgeWidth ),     m_nShadowOffsetX( nShadowOffsetX ),       m_nShadowOffsetY( nShadowOffsetY ), 
	m_pCharMap( pCharMap ), m_FontUsed( 0 ), m_FaceFilePath( pFontFilePath ),   m_FontName( pFontName ),
	m_AntiAliased( bAnsiAliased ), m_pTexture(NULL), m_pShadowTexture(NULL), m_pEdgeTexture(NULL),
	m_dwTextFrontColor(0), m_dwTextBackColor(0), m_dwShadowColor(dwShadowColor), m_dwEdgeColor(EdgeColor),m_nRowSpacing(nRowSpacing),
	m_bHasShadow(true), m_bHasEdge(true), m_nFontHeight(nFontHeight), m_bUnderline(bUnderline), m_bBold(bBold) ,m_bItalic(bItalic)
	{
            InitFreeTypeFont();
	}

	CGUIFont::~CGUIFont( )
	{
		// 释放CodePage
		if ( m_pCharMap != NULL )
			delete m_pCharMap;

		// 释放字体表面
		delete [] m_pGlyph;
		if(m_pTexture)		
		{
			m_pTexture->m_TextCasheTexture->Release();
			delete m_pTexture;
			m_pTexture = NULL;
		}
		if (m_pShadowTexture)
		{
			m_pShadowTexture->m_TextCasheTexture->Release() ;
			delete m_pShadowTexture;
			m_pShadowTexture = NULL ;
		}
		if (m_pEdgeTexture)
		{
			m_pEdgeTexture->m_TextCasheTexture->Release() ;
			delete m_pEdgeTexture;
			m_pEdgeTexture = NULL ;
		}
		// 释放FreeType字体
		FT_Done_Face( m_pFTFace );

		// 释放FreeType
		if ( --s_FTRef == 0 )
			FT_Done_FreeType( s_pFTLibrary );
	}

	void CGUIFont::InitFreeTypeFont()
	{
		// 如果没有CodePage, 使用GBK作为默认CodePage
		if ( m_pCharMap == NULL )
			m_pCharMap = new CGUIGB2312Page;

		// 初始化FreeType
		if ( s_pFTLibrary == NULL )
			FT_Init_FreeType( &s_pFTLibrary );              

		// 创建字体
		int nError = FT_New_Face( s_pFTLibrary, m_FaceFilePath.c_str( ), 0, &m_pFTFace );
		if (nError == FT_Err_Unknown_File_Format)
		{
		}  

		// 设置字体解析度，字体大小
		FT_Set_Char_Size( m_pFTFace, m_nFontHeight * 64, 0, m_Dpi, m_Dpi );

		int tCorrectedX = GUI_MAX( 0, m_nEdgeWidth *2  - m_nShadowOffsetX ) + GUI_MAX( 0, m_nEdgeWidth *2 + m_nShadowOffsetX );
		int tCorrectedY = GUI_MAX( 0, m_nEdgeWidth *2  - m_nShadowOffsetY ) + GUI_MAX( 0, m_nEdgeWidth *2+ m_nShadowOffsetY );
		m_UnitWidth	 = m_pFTFace->size->metrics.max_advance / 64.0f + tCorrectedX ;
		m_UnitHeight = m_pFTFace->size->metrics.height / 64.0f + tCorrectedY ;
		
		// 创建表面
		m_pGlyph = new CGUIGlyphData[ m_pCharMap->GetCodeSpace( ) ];
		for ( int i = 0; i < m_pCharMap->GetCodeSpace( ); i ++ )
		{
			m_pGlyph[ i ].SetContext( this, m_pCharMap->GetUnicode( i ) );
		}
		s_FTRef ++;
	}

	CGUIGlyphData* CGUIFont::GetFontGlyph( const char* pText )
	{
		int tGlyphIndex = m_pCharMap->GetGlyphIndex( pText );
		if ( tGlyphIndex == GUI_ERR_OUTOFCODEPAGE )
			return NULL;

		return &m_pGlyph[ tGlyphIndex ];
	}

	int CGUIFont::GetFontHeight( )
	{
		return m_UnitHeight;
	}

	int CGUIFont::GetTextWidth( const char* pText )
	{
		int tGlyphIndex = m_pCharMap->GetGlyphIndex( pText );
		if ( tGlyphIndex == GUI_ERR_OUTOFCODEPAGE )
			return GUI_ERR_OUTOFCODEPAGE;

		return m_pGlyph[ tGlyphIndex ].GetGlyphWidth( );
	}

	int CGUIFont::GetTextWidth( const char* pText, int vTextCount )
	{
		int tTextWidth = 0;
		for ( int tIndex = 0; tIndex < vTextCount; )
		{
			tTextWidth += GetTextWidth( pText + tIndex );
			pText[ tIndex ] & 0x80 ? tIndex += 2 : tIndex ++;
		}
		return tTextWidth;
	}

	unsigned int CGUIFont::GetRowSpacing() const 
	{
		return m_nRowSpacing ;
	}

	int CGUIFont::PrintText( CGUICanvas* pCanvas, const char* pText, const CGUIPoint& rPoint )
	{
		int tGlyphIndex = m_pCharMap->GetGlyphIndex( pText );
		if ( tGlyphIndex == GUI_ERR_OUTOFCODEPAGE )
			return GUI_ERR_OUTOFCODEPAGE;

		const CGUIRect& rSrcRect = m_pGlyph[ tGlyphIndex ].GetGlyphRect( );


		if (m_bItalic)
		{
		    pCanvas->AddTextToRenderBuffer(  rSrcRect, CGUIRect( rPoint, rSrcRect.Size( ) ),  FONT_SHEAR_FACTOR);
		}
		else
		{
            pCanvas->AddTextToRenderBuffer(  rSrcRect, CGUIRect( rPoint, rSrcRect.Size( ) ));
		}

		return m_pGlyph[ tGlyphIndex ].GetGlyphWidth();
	}

	void CGUIFont::prerender()
	{
		// pCanvas->PrepareRender();
	}

	void CGUIFont::ClearShadow()
	{
		m_bHasShadow    = false ;
		m_nShadowOffsetX = 0;			
		m_nShadowOffsetY = 0;
		m_dwShadowColor = 0 ;

		if(m_pShadowTexture) 
		{
			m_pShadowTexture->m_TextCasheTexture->Release() ;
		}
		m_pShadowTexture = NULL ;

	}
	void CGUIFont::ClearEdge()
	{
		m_bHasEdge    = false ;
		m_nEdgeWidth  = 0 ;
		m_dwEdgeColor = 0 ;

		if(m_pEdgeTexture) 
		{
			m_pEdgeTexture->m_TextCasheTexture->Release() ;
		}
		m_pEdgeTexture = NULL ;
	}

	void CGUIFont::SetTextFrontColor(DWORD dwColor) 
	{
        m_dwTextFrontColor = dwColor ;
	}
	void CGUIFont::SetTextBackColor(DWORD dwColor) 
	{
		m_dwTextBackColor = dwColor ;
	}
	void CGUIFont::SetTextShadowColor(DWORD dwColor) 
	{
        m_dwShadowColor = dwColor ;
	}
	void CGUIFont::SetTextEdgeColor(DWORD dwColor) 
	{
		m_dwEdgeColor = dwColor ;
	}
	void CGUIFont::SetTextFrontAndBackAlpha(unsigned int nAlpha) 
	{
		unsigned int nBackColorAlpha = (0xff000000 & m_dwTextBackColor) ;
		nBackColorAlpha= nBackColorAlpha >>24 ;

		if (nBackColorAlpha > 0)
		{
            m_dwTextBackColor = (0x00ffffff & m_dwTextBackColor) |  (nAlpha<<24) ;
		}
		m_dwTextFrontColor = (0x00ffffff & m_dwTextFrontColor) | (nAlpha<<24) ;

		if (m_bHasEdge)
		{
			m_dwEdgeColor = (0x00ffffff & m_dwEdgeColor) | (nAlpha<<24) ;
		}

		if (m_bHasShadow)
		{
			m_dwShadowColor = (0x00ffffff & m_dwShadowColor) | (nAlpha<<24) ;
		}
	}

	CGUITexture* CGUIFont::GetTextTexture() const
	{
		return m_pTexture;
	}
	CGUITexture* CGUIFont::GetShadowTexture()  const
	{
		return m_pShadowTexture ;
	}
	CGUITexture* CGUIFont::GetEdgeTexture()  const   		
	{
		return m_pEdgeTexture ;
	}

	bool CGUIFont::DoesHasShadow() const
	{
		return m_bHasShadow ;
	}
	bool CGUIFont::DoesHasEdge() const
	{
		return m_bHasEdge ;
	}

	DWORD CGUIFont::GetTextFrontColor() const
	{
		return m_dwTextFrontColor ;
	}
	DWORD CGUIFont::GetTextBackColor() const
	{
		return m_dwTextBackColor ;
	}
	DWORD CGUIFont::GetTextShadowColor() const
	{
		return m_dwShadowColor ;
	}
	DWORD CGUIFont::GetTextEdgeColor() const
	{
		return m_dwEdgeColor ;
	}

	bool CGUIFont::IsUnderline()    const 
	{
		return m_bUnderline ;
	}

	bool CGUIFont::IsItalic()       const 
	{
		return m_bItalic ;
	}
}