// ************************************************************ //
// CGUIFontManager
// ************************************************************ //
#pragma once

#include "FBGUISingleton.h"
#include "FBGUIFont.h"
namespace FBGUI
{

	class CGUIFontManager : public CSingleton< CGUIFontManager >
	{
	protected:
		std::vector< CGUIFont* >	m_Fonts;

	public:
		~CGUIFontManager( ) { DestoryAll( ); }

	public:


		CGUIFont* Create( const char* pFontName, const char* pFontFilePath, int nFontHeight, bool bAnsiAliased,
		                bool bUnderline, bool bBold, bool bItalic, unsigned int nRowSpacing,
			              CGUICodePage* pCharMap = NULL) ;

		CGUIFont* Create( const char* pFontName, const char* pFontFilePath, int nFontHeight, bool bAnsiAliased, 
			             bool bUnderline, bool bBold, bool bItalic, unsigned int nRowSpacing,
			              int nShadowOffsetX,    int nShadowOffsetY, DWORD dwShadowColor, CGUICodePage* pCharMap = NULL) ;

		CGUIFont* Create( const char* pFontName, const char* pFontFilePath, int nFontHeight, bool bAnsiAliased, 
			             bool bUnderline, bool bBold, bool bItalic, unsigned int nRowSpacing,
			              unsigned int nEdgeWidth, DWORD EdgeColor, CGUICodePage* pCharMap = NULL );

		CGUIFont* Create( const char* pFontName,  const char* pFontFilePath, int nFontHeight, bool bAnsiAliased, 
			             bool bUnderline, bool bBold, bool bItalic, unsigned int nRowSpacing,
			             int nShadowOffsetX,   int nShadowOffsetY, DWORD dwShadowColor, unsigned int nEdgeWidth,   
			             DWORD EdgeColor, CGUICodePage* pCharMap = NULL) ;

		void Destroy( const char* pFontName );
		void DestoryAll( );
		CGUIFont* GetFont( const char* pFontName );
	};
}