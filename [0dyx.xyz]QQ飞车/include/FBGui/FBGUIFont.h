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
		static int			s_FTRef;				//! freetype��������ü���
		static FT_Library	s_pFTLibrary;		//! freetype�����


	protected:
		CGUITexture*	    m_pTexture;		    //! ��������
		CGUITexture*        m_pShadowTexture;   //! �ı���Чshadow������
		CGUITexture*        m_pEdgeTexture;     //! �ı���ЧEdge������

		FT_Face				m_pFTFace;			//! FreeType�������
		CGUICodePage*		m_pCharMap;			//! ����ҳ
		CGUIGlyphData*		m_pGlyph;			//! �ַ���
		std::list<CGUIGlyphData*> m_pUsedPool;			
		std::string			m_FaceFilePath;		//! ����file path
		std::string			m_FontName;			//! ����Ψһ��ʶ
		int					m_Dpi;				//! ������
		int					m_nEdgeWidth;				//! �߿�
		int					m_nShadowOffsetX;			//! shadow����ƫ��
		int					m_nShadowOffsetY;			//! shadow����ƫ��
		int					m_UnitWidth;		//! ���嵥Ԫ���
		int					m_UnitHeight;		//! ���嵥Ԫ�߶�
		int					m_FontUsed;			//! �����˶��ٸ�����
		bool				m_AntiAliased;		//! �Ƿ񿹾��

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
		//! \brief �õ�һ���ַ�����, 
		\param const char* pText ָ���ַ���ָ�룬�Զ�������ֽ�
		\return �õ�pTextָ��ĵ�һ���ַ����Ρ�
		*/
		CGUIGlyphData* GetFontGlyph( const char* pText );						

		/*
		//! \brief ���е��ַ���һ���ߣ�GetFontHeight( )�õ��ַ��߶�
		\return �����ַ��߶�
		*/
		int GetFontHeight( );													

		/*
		//! \brief �õ�pTextָ��ĵ�һ���ַ��ı����
		\return �����ַ����
		*/
		int GetTextWidth( const char* pText );									

		/*
		//! \brief �õ�ָ���ı����
		\return �����ַ����
		*/
		int GetTextWidth( const char* pText, int vTextCount );		

		unsigned int GetRowSpacing() const ;

		/*
		//! \brief ���һ���ַ�
		\return ���ش�����
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