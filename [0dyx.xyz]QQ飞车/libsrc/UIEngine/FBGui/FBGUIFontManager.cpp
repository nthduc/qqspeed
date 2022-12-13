#include "FBGUIFontManager.h"

namespace FBGUI
{
	// ************************************************************ //
	// CGUIFontManager
	// ************************************************************ //
	template<> CGUIFontManager* CSingleton< CGUIFontManager >::spSingleton = NULL;

	CGUIFont* CGUIFontManager::Create( const char* pFontName, const char* pFontFilePath, int nFontHeight, bool bAnsiAliased,
		                           bool bUnderline, bool bBold, bool bItalic, unsigned int nRowSpacing,
		                           CGUICodePage* pCharMap) 
	{
		CGUIFont* tpFont = new CGUIFont( pFontName, pFontFilePath, nFontHeight, bAnsiAliased , pCharMap, bUnderline, bBold, bItalic ,nRowSpacing) ;
		m_Fonts.push_back( tpFont );
		return tpFont;
	}

	CGUIFont* CGUIFontManager::Create( const char* pFontName, const char* pFontFilePath, int nFontHeight, bool bAnsiAliased, 
		                           bool bUnderline, bool bBold, bool bItalic, unsigned int nRowSpacing,
		                           int nShadowOffsetX,    int nShadowOffsetY, DWORD dwShadowColor, CGUICodePage* pCharMap ) 
	{
		CGUIFont* tpFont = new CGUIFont( pFontName, pFontFilePath, nFontHeight, bAnsiAliased, pCharMap, bUnderline, bBold, bItalic, nRowSpacing,
			                             nShadowOffsetX, nShadowOffsetY , dwShadowColor) ;
		m_Fonts.push_back( tpFont );
		return tpFont;
	}

	CGUIFont* CGUIFontManager::Create( const char* pFontName, const char* pFontFilePath, int nFontHeight, bool bAnsiAliased, 
		                            bool bUnderline, bool bBold, bool bItalic,  unsigned int nRowSpacing,
		                            unsigned int nEdgeWidth, DWORD EdgeColor, CGUICodePage* pCharMap  )
	{
		CGUIFont* tpFont = new CGUIFont( pFontName , pFontFilePath , nFontHeight , bAnsiAliased, pCharMap, bUnderline, bBold, bItalic,nRowSpacing, nEdgeWidth, EdgeColor) ;
		m_Fonts.push_back( tpFont );
		return tpFont;
	}

	CGUIFont* CGUIFontManager::Create( const char* pFontName,  const char* pFontFilePath, int nFontHeight, bool bAnsiAliased, 
		                            bool bUnderline, bool bBold, bool bItalic, unsigned int nRowSpacing,
		                            int nShadowOffsetX,   int nShadowOffsetY, DWORD dwShadowColor, unsigned int nEdgeWidth,   
		                            DWORD EdgeColor, CGUICodePage* pCharMap ) 
	{
		CGUIFont* tpFont = new CGUIFont( pFontName , pFontFilePath , nFontHeight , bAnsiAliased, pCharMap, bUnderline, bBold, bItalic, nRowSpacing, nShadowOffsetX,nShadowOffsetY, dwShadowColor, nEdgeWidth, EdgeColor ) ;
		m_Fonts.push_back( tpFont );
		return tpFont;
	}

	CGUIFont* CGUIFontManager::GetFont( const char* pFontName )
	{
		for ( int i = 0; i < int( m_Fonts.size( ) ); i ++ )
		{
			if ( ::strcmp( m_Fonts[ i ]->GetName( ).c_str( ), pFontName ) == 0 )
				return m_Fonts[ i ];
		}

		return NULL;
	}

	void CGUIFontManager::Destroy( const char* pFontName )
	{
		for ( int i = 0; i < int( m_Fonts.size( ) ); i ++ )
		{
			if ( ::strcmp( m_Fonts[ i ]->GetName( ).c_str( ), pFontName ) == 0 )
			{
				delete m_Fonts[ i ];
				m_Fonts.erase( m_Fonts.begin( ) + i );
				return;
			}
		}
	}

	void CGUIFontManager::DestoryAll( )
	{
		for ( int i = 0; i < int( m_Fonts.size( ) ); i ++ )
			delete m_Fonts[ i ];

		m_Fonts.clear( );
	}
}