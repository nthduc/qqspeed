// ===============================================================================
// UIFaceElement.CPP
//
// Purpose   : see header
//
// Latest Design doc link: http://
//
// 
// Author: Pucca Liang(mailto:puccaliang@tencent.com) 
// created:     1 2, 2007   10:36
// filename:    
// file path:   
// file base:   
// file ext:    .cpp
// 
// Copyright Tencent 1.2007
// =============================================================================== 
// <History> 
// 
// =============================================================================== 
// <QA> 
//  TODO: 
// =============================================================================== 

// =============================================================================== 
//Header 
// =============================================================================== 
#include "Top-KartPCH.h"

// System headers: 

// Project headers: 
#include "UIFaceElement.h"

// using namespaces:

// =============================================================================== 
//Macros 
// =============================================================================== 

// =============================================================================== 
//Local structure/class 
// =============================================================================== 

// =============================================================================== 
//Variables 
// =============================================================================== 

// External variables declaration: 

// Global variables: 

// Local variables: 


// ===============================================================================
//External/forward function declarations 
// =============================================================================== 

// ===============================================================================
//Local functions
// ===============================================================================

// ===============================================================================
//Global functions
// ===============================================================================

// ===============================================================================
//Class implementation
// ===============================================================================

namespace GFX
{

	// Text Element------------------------------------------------------------------------------------------------
	
	unsigned int CUIFaceTextElement::s_ClassID = CLASSID_TEXT ;

	CUIFaceTextElement::CUIFaceTextElement( const char* szText, const char* szFontName, DWORD dwTextColor , RECT &rcDestRect,
		                                    RECT *rcClip, unsigned int format, unsigned int nLayer )
										:IUIFaceElement(nLayer),
										 m_strContent(szText), m_strFontName(szFontName),m_dwTextColor(dwTextColor),
										 m_rcDestRect(rcDestRect),      m_rcClip(rcClip),          m_Format(format)
										   
	{

	}

	void CUIFaceTextElement::DrawElement()
	{
		IUIRenderer *pUIRenderer =  GetUIRenderer();
		pUIRenderer->SetCurrentFontName(m_strFontName.c_str());
		pUIRenderer->SetCurrentTextFrontColor(m_dwTextColor) ;
		pUIRenderer->DrawText(m_strContent.c_str(), m_rcDestRect, m_rcClip, m_Format ) ;

	}

	unsigned int CUIFaceTextElement::GetClassID()
	{
		return s_ClassID ;
	}

	unsigned int CUIFaceTextElement::GetTextHeight()
	{
		IUIRenderer *pUIRenderer = GetUIRenderer();
		std::string strCurFont( pUIRenderer->GetCurrentFontName() ) ;
		pUIRenderer->SetCurrentFontName(m_strFontName.c_str()) ;

        unsigned int height = pUIRenderer->GetTextHeight() ;

		pUIRenderer->SetCurrentFontName(strCurFont.c_str()) ;
        
		return height ;
	}
	unsigned int CUIFaceTextElement::GetTextWidth()
	{
		IUIRenderer *pUIRenderer = GetUIRenderer();
		std::string strCurFont( pUIRenderer->GetCurrentFontName() ) ;
		pUIRenderer->SetCurrentFontName(m_strFontName.c_str()) ;
		
		unsigned int width = pUIRenderer->GetTextWidth( m_strContent.c_str(), m_strContent.length()) ;

		pUIRenderer->SetCurrentFontName(strCurFont.c_str()) ;

		return width ;
	}
    //-----------------------------------------------------------------------------------------------------------------

	// Face Element----------------------------------------------------------------------------------------------------

	unsigned int CUIFaceImageElement::s_ClassID = CLASSID_IMAGE ;

	CUIFaceImageElement::CUIFaceImageElement( RECT& rcSrc,RECT& rcDest, unsigned char byImageAlpha, 
											  const char* lpszImageFileName, unsigned int nLayer)
											: IUIFaceElement(nLayer),
											  m_rcScr(rcSrc), m_rcDest(rcDest), m_byImageAlpha(byImageAlpha)
	{
		IUIRenderer *pUIRenderer = GetUIRenderer();
		m_ImageID = pUIRenderer->CreateImage(lpszImageFileName) ;
	}

	void CUIFaceImageElement::DrawElement()
	{
		FillMode euMode = FILLMODE_SCALE ;
		IUIRenderer *pUIRenderer = GetUIRenderer();
		pUIRenderer->DrawImage(m_ImageID, euMode, &m_rcScr, &m_rcDest, m_byImageAlpha) ;
		
	}



	unsigned int CUIFaceImageElement::GetClassID()
	{
		return s_ClassID ;
	}
	
	CUIFaceImageElement::~CUIFaceImageElement()
	{
		IUIRenderer *pUIRenderer = GetUIRenderer();
		pUIRenderer->ReleaseImage(m_ImageID) ;
	}
	//---------------------------------------------------------------------------------------------------------------------
}