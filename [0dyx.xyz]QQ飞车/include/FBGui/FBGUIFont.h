#pragma once
#include "FBGUIBase.h"
#include "FBGUICodePage.h"
#include <list>



namespace FBGUI
{
	class CGUIGlyphData ;
	class CGUICanvas ;
	// ************************************************************ //
	// CGUIFont
	// ************************************************************ //

	const float FONT_SHEAR_FACTOR = 0.5f ;
	class CGUIFont
	{
		friend class CGUIGlyphData;

	protected:
		static int			s_FTRef;				//! freetype字体库引用计数
		static FT_Library	s_pFTLibrary;		//! freetype字体库


	protected:
		CGUITexture*	    m_pTexture;		    //! 字形纹理
		CGUITexture*        m_pShadowTexture;   //! 文本特效shadow的纹理
		CGUITexture*        m_pEdgeTexture;     //! 文本特效Edge的纹理

		FT_Face				m_pFTFace;			//! FreeType字体表面
		CGUICodePage*		m_pCharMap;			//! 代码页
		CGUIGlyphData*		m_pGlyph;			//! 字符集
		std::list<CGUIGlyphData*> m_pUsedPool;			
		std::string			m_FaceFilePath;		//! 字体file path
		std::string			m_FontName;			//! 字体唯一标识
		int					m_Dpi;				//! 解析度
		int					m_nEdgeWidth;				//! 边宽
		int					m_nShadowOffsetX;			//! shadow横向偏移
		int					m_nShadowOffsetY;			//! shadow纵向偏移
		int					m_UnitWidth;		//! 字体单元宽度
		int					m_UnitHeight;		//! 字体单元高度
		int					m_FontUsed;			//! 创建了多少个字体
		bool				m_AntiAliased;		//! 是否抗锯齿

		bool                m_bHasShadow      ;    //! has a shadow ?
		bool                m_bHasEdge        ;    //! has an Edge  ?
		bool                m_bUnderline      ;    // underline?
		bool                m_bBold           ;    // Bold?
		bool                m_bItalic         ;    // Italic?

		DWORD               m_dwTextFrontColor;
		DWORD               m_dwTextBackColor ;
		DWORD               m_dwShadowColor   ;
		DWORD               m_dwEdgeColor     ;
		int                 m_nFontHeight     ;

	    unsigned int        m_nRowSpacing     ;  // row Spacing, count in pixels

	public:
        
		CGUIFont( const char* pFontName, const char* pFontFilePath, int nFontHeight, bool bAnsiAliased, 
			      CGUICodePage* pCharMap, bool bUnderline, bool bBold, bool bItalic, int nRowSpacing) ;
		
		CGUIFont( const char* pFontName, const char* pFontFilePath, int nFontHeight, bool bAnsiAliased, 
			      CGUICodePage* pCharMap, bool bUnderline, bool bBold, bool bItalic, int nRowSpacing, int nShadowOffsetX,    
				  int nShadowOffsetY, DWORD dwShadowColor) ;
		
		CGUIFont( const char* pFontName, const char* pFontFilePath, int nFontHeight, bool bAnsiAliased, 
			      CGUICodePage* pCharMap, bool bUnderline, bool bBold, bool bItalic, int nRowSpacing, unsigned int nEdgeWidth, DWORD EdgeColor );
		
		CGUIFont( const char* pFontName,  const char* pFontFilePath, int nFontHeight, bool bAnsiAliased, 
			      CGUICodePage* pCharMap,  bool bUnderline, bool bBold, bool bItalic, int nRowSpacing, int nShadowOffsetX, int nShadowOffsetY,    
				  DWORD dwShadowColor,    unsigned int nEdgeWidth,   DWORD EdgeColor) ;

	public:
		~CGUIFont( );

	public:
		/*
		//! \brief 得到一个字符字形, 
		\param const char* pText 指向字符的指针，自动处理多字节
		\return 得到pText指向的第一个字符字形　
		*/
		CGUIGlyphData* GetFontGlyph( const char* pText );						

		/*
		//! \brief 所有的字符都一样高，GetFontHeight( )得到字符高度
		\return 返回字符高度
		*/
		int GetFontHeight( );													

		/*
		//! \brief 得到pText指向的第一个字符文本宽度
		\return 返回字符宽度
		*/
		int GetTextWidth( const char* pText );									

		/*
		//! \brief 得到指定文本宽度
		\return 返回字符宽度
		*/
		int GetTextWidth( const char* pText, int vTextCount );		

		unsigned int GetRowSpacing() const ;

		/*
		//! \brief 输出一个字符
		\return 返回错误码
		*/
		int PrintText( CGUICanvas* pCanvas, const char* pText, const CGUIPoint& rPoint );


    	void prerender();

		std::string GetName( )	{ return m_FontName; }

		void ClearShadow();
		void ClearEdge();

		void SetTextFrontColor(DWORD dwColor) ;
		void SetTextBackColor(DWORD dwColor) ;
		void SetTextShadowColor(DWORD dwColor) ;
		void SetTextEdgeColor(DWORD dwColor) ;
		void SetTextFrontAndBackAlpha(unsigned int nAlpha) ;

		DWORD GetTextFrontColor()  const;
		DWORD GetTextBackColor()   const;
		DWORD GetTextShadowColor() const;
		DWORD GetTextEdgeColor()   const;		

		CGUITexture* GetTextTexture() const;	
		CGUITexture* GetShadowTexture() const;   
		CGUITexture* GetEdgeTexture() const;     		

		bool DoesHasShadow()  const ;
		bool DoesHasEdge()    const ;
		bool IsUnderline()    const ;
		bool IsItalic()       const ;

	protected:
		void InitFreeTypeFont();
	};

}