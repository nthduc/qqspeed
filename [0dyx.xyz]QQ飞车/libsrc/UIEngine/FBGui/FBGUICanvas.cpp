#include "FBGUICanvas.h"
#include "FBGUISize.h"
#include "FBGUIRect.h"
#include "FBGUIDef.h"
#include "FBGUISystem.h"
#include "FBGUIFont.h"
#include <vector>
namespace FBGUI
{

	// ************************************************************ //
	// CGUICanvas
	// ************************************************************ //
	CGUIRenderDriver* CGUICanvas::s_pDriver = NULL;

	int CGUICanvas::GetFormatText( char* pBuffer, const char* pText, int vWidth )
	{
		if ( GetTextWidth( pText, int( ::strlen( pText ) ) ) < vWidth )
		{
			::strcpy( pBuffer, pText );
			return GUI_ERR_SUCCESS;
		}

		int tPreWidth = GetTextWidth( "...", 3 );
		int tWidth = 0, tIndex = 0;
		while( pText[ tIndex ] != 0 )
		{
			int tTextLen = ( pText[ tIndex ] & 0x80 ? 2 : 1 );
			tWidth += GetTextWidth( pText + tIndex, tTextLen );
			if ( tWidth + tPreWidth > vWidth )
				break;

			::strncpy( pBuffer + tIndex, pText + tIndex, tTextLen );
			tIndex += tTextLen;
		}

		pBuffer[ tIndex ] = '\0'; 
		::strcat( pBuffer, "..." );
		return GUI_ERR_SUCCESS;
	}

	//! 贴图

	int CGUICanvas::AddTextToRenderBuffer( const CGUIRect& rSrcRect, const CGUIRect& rDestRect, float fHorShear )
	{
		if ( s_pDriver == NULL )
			s_pDriver = CGUISystem::GetSingleton( ).GetRenderDriver( );

		//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		// warning! Flush problem
		s_pDriver->PrepareRender();

		CGUISystem*	tpSystem = CGUISystem::GetSingletonPtr( );

		// 得到目标矩形的屏幕坐标
		CGUIRect tDestRect =  rDestRect ;

		// 得到裁减后的矩形
		CGUIRect tClipRect =  tDestRect ;
		if ( tClipRect.IsRectEmpty( ) )
			return GUI_ERR_SUCCESS;

		// 按照裁减后的区域在裁减前的区域的比例计算源矩形
/*		float tLeft		= ( tClipRect.mTopLeft.mX - tDestRect.mTopLeft.mX ) / (float) tDestRect.Width( );
		float tTop		= ( tClipRect.mTopLeft.mY - tDestRect.mTopLeft.mY ) / (float) tDestRect.Height( );
		float tRight	= ( tClipRect.mBottomRight.mX - tDestRect.mTopLeft.mX ) / (float) tDestRect.Width( );
		float tBottom	= ( tClipRect.mBottomRight.mY - tDestRect.mTopLeft.mY ) / (float) tDestRect.Height( );

		int tSrcWidth	= rSrcRect.Width( );
		int tSrcHeight	= rSrcRect.Height( );
*/
		// 重新计算源矩形
		//CGUIRect tSrcRect( rSrcRect.mTopLeft.mX + int( tLeft * tSrcWidth ), rSrcRect.mTopLeft.mY + int( tTop * tSrcHeight ), 
		//	rSrcRect.mTopLeft.mX + int( tRight * tSrcWidth ), rSrcRect.mTopLeft.mY + int( tBottom * tSrcHeight ) );
		return s_pDriver->AddTextToRenderBuffer( rSrcRect,  tClipRect , fHorShear);
	}

	int CGUICanvas::PrintText( const char* pText, const CGUIRect& rDestRect, unsigned int vFormat )
	{
		// 目前只支持单行显示

		m_pFont->prerender();
		vFormat |= GUI_DT_SINGLELINE;

		char tWrapText[ 2048 ];
		if ( vFormat & GUI_DT_WRAPTEXT )
			GetFormatText( tWrapText, pText, rDestRect.Width( ) );
		else
			::strcpy( tWrapText, pText );


		int tFontHeight = GetFontHeight( );
		int tTextWidth	= GetTextWidth( tWrapText, int( ::strlen( tWrapText ) ) );
		CGUIPoint tPoint;
		if ( vFormat & GUI_DT_SINGLELINE )
		{
			if ( vFormat & GUI_DT_LEFT )
				tPoint.mX = rDestRect.mTopLeft.mX;
			else if ( vFormat & GUI_DT_RIGHT )
				tPoint.mX = rDestRect.mBottomRight.mX - tTextWidth;
			else if ( vFormat & GUI_DT_HCENTER )
				tPoint.mX = rDestRect.mTopLeft.mX + ( ( rDestRect.Width( ) - tTextWidth ) >> 1 );
			else
				tPoint.mX = rDestRect.mTopLeft.mX;

			if ( vFormat & GUI_DT_TOP )
				tPoint.mY = rDestRect.mTopLeft.mY;
			else if ( vFormat & GUI_DT_BOTTOM )
				tPoint.mY = rDestRect.mBottomRight.mY - tFontHeight;
			else if ( vFormat & GUI_DT_VCENTER )
				tPoint.mY = rDestRect.mTopLeft.mY + ( ( rDestRect.Height( ) - tFontHeight ) >> 1 );
			else
				tPoint.mY = rDestRect.mTopLeft.mY;
		}

		int tIndex = 0;
		while( tWrapText[ tIndex ] != 0 )
		{
			tPoint.mX += m_pFont->PrintText( this, tWrapText + tIndex, tPoint);
			tIndex += tWrapText[ tIndex ] & 0x80 ? 2 : 1;
		}

		return GUI_ERR_SUCCESS;
	}
	int CGUICanvas::PrintText( const char* pText, unsigned int nDestLeft, unsigned int nDestTop, RECT *rcClip)
	{
		// 目前只支持单行显示

 
//		vFormat |= GUI_DT_SINGLELINE;
		char tWrapText[ 2048 ];
//		if ( vFormat & GUI_DT_WRAPTEXT )
//			GetFormatText( tWrapText, pText, rDestRect.Width( ) );
//		else
			::strcpy( tWrapText, pText );

		int tFontHeight = GetFontHeight( );
		int tTextWidth	= GetTextWidth( tWrapText, int( ::strlen( tWrapText ) ) );
		CGUIPoint tPoint;
		int tIndex ;

		if(m_pFont->GetTextBackColor() !=0)
		{
			s_pDriver->SetCurrentTexture(NULL);
			s_pDriver->SetSystemColor(m_pFont->GetTextBackColor()) ;
			tPoint.mX = nDestLeft ;
			tPoint.mY = nDestTop  ;
			tIndex = 0;
			while( tWrapText[ tIndex ] != 0 )
			{
				tPoint.mX += m_pFont->PrintText( this, tWrapText + tIndex, tPoint );
				tIndex += tWrapText[ tIndex ] & 0x80 ? 2 : 1;
			}
		}


		tPoint.mX = nDestLeft ;
		tPoint.mY = nDestTop  ;
		if ( m_pFont->DoesHasShadow())
		{
			s_pDriver->SetCurrentTexture(m_pFont->GetShadowTexture());
			s_pDriver->SetSystemColor(m_pFont->GetTextShadowColor()) ;
			tPoint.mX = nDestLeft ;
			tPoint.mY = nDestTop  ;

			tIndex = 0;
			while( tWrapText[ tIndex ] != 0 )
			{
				tPoint.mX += m_pFont->PrintText( this, tWrapText + tIndex, tPoint );
				tIndex += tWrapText[ tIndex ] & 0x80 ? 2 : 1;
			}
		}

		if ( m_pFont->DoesHasEdge())
		{
			tPoint.mX = nDestLeft ;
			tPoint.mY = nDestTop  ;
			s_pDriver->SetCurrentTexture(m_pFont->GetEdgeTexture());
			s_pDriver->SetSystemColor(m_pFont->GetTextEdgeColor()) ;
			tPoint.mX = nDestLeft ;
			tPoint.mY = nDestTop  ;

			tIndex = 0;
			while( tWrapText[ tIndex ] != 0 )
			{
				tPoint.mX += m_pFont->PrintText( this, tWrapText + tIndex, tPoint );
				tIndex += tWrapText[ tIndex ] & 0x80 ? 2 : 1;
			}
		}

		s_pDriver->SetCurrentTexture(m_pFont->GetTextTexture());
		s_pDriver->SetSystemColor(m_pFont->GetTextFrontColor()) ;
		tPoint.mX = nDestLeft ;
		tPoint.mY = nDestTop  ;

		tIndex = 0;
		while( tWrapText[ tIndex ] != 0 )
		{
			tPoint.mX += m_pFont->PrintText( this, tWrapText + tIndex, tPoint );
			tIndex += tWrapText[ tIndex ] & 0x80 ? 2 : 1;
		}

		if ( m_pFont->IsUnderline())
		{
            CGUIPoint point1 , point2 ;
			point1.mX = nDestLeft ; 
			point1.mY = nDestTop + tFontHeight * 0.85f;
			point2.mX = nDestLeft + tTextWidth ;
			point2.mY = nDestTop + tFontHeight * 0.85f;
			DrawLine(point1, point2, m_pFont->GetTextFrontColor()) ;
		}
		return GUI_ERR_SUCCESS;
	}

	struct sSubString_t
	{
		std::string strSingleLine ;
		unsigned int nLeft ; 
		unsigned int nTop  ;
		unsigned int nTextPixelLength ;
	};

	int CGUICanvas::PrintText( const char* pText, const CGUIRect& rDestRect, RECT *rcClip, unsigned int vFormat ) 
	{

		int tFontHeight = GetFontHeight();
		int tTextWidth	= GetTextWidth( pText, int( ::strlen( pText ) ) );
		int nDestRestWidth = rDestRect.Width() ;
		

		std::vector<sSubString_t> vecSubStrings;

		// Divide the text into several sub strings, and the length of the sub strings are not longer than the destRC width. 
		int tIndex = 0;
        int nTextPixelLengthCount = 0;

		float fFontShear = 0 ;
		if (m_pFont->IsItalic())
		{
             fFontShear = FONT_SHEAR_FACTOR ;
		}
		while ( pText[tIndex] != 0)
		{
             nTextPixelLengthCount = 0;
			 sSubString_t aSubString ;

             while ( ( pText[tIndex] != 0) && ( nTextPixelLengthCount + m_pFont->GetTextWidth(pText + tIndex) * (1 + fFontShear ) < nDestRestWidth ) )
             {
				 nTextPixelLengthCount += m_pFont->GetTextWidth(pText + tIndex) ;
                 aSubString.strSingleLine.push_back( pText[tIndex] ) ;
				 if (pText[ tIndex ] & 0x80)
				 {
                      aSubString.strSingleLine.push_back( pText[tIndex+1] );
				 }
				 tIndex += pText[ tIndex ] & 0x80 ? 2 : 1;
             }             

             aSubString.nTextPixelLength = nTextPixelLengthCount ;
			 vecSubStrings.push_back(aSubString) ;
		}

		// adjust the left and top coordinates of every string by the align format 
        int i = 0 ;
		for ( i = 0 ; i < vecSubStrings.size() ; ++i )
		{
			if ( vFormat & GUI_DT_LEFT )
				vecSubStrings[i].nLeft = rDestRect.mTopLeft.mX;
			else if ( vFormat & GUI_DT_RIGHT )
				vecSubStrings[i].nLeft = rDestRect.mBottomRight.mX - vecSubStrings[i].nTextPixelLength;
			else if ( vFormat & GUI_DT_HCENTER )
				vecSubStrings[i].nLeft = rDestRect.mTopLeft.mX + ( ( rDestRect.Width( ) - vecSubStrings[i].nTextPixelLength ) >> 1 );
			else
				vecSubStrings[i].nLeft = rDestRect.mTopLeft.mX;	
		}

		int nTolalTextHeight = tFontHeight * vecSubStrings.size() ;
		int nTextTop = 0;

		if ( vFormat & GUI_DT_TOP )
			nTextTop = rDestRect.mTopLeft.mY;
		else if ( vFormat & GUI_DT_BOTTOM )
			nTextTop = rDestRect.mBottomRight.mY - nTolalTextHeight;
		else if ( vFormat & GUI_DT_VCENTER )
			nTextTop = rDestRect.mTopLeft.mY + ( ( rDestRect.Height( ) - nTolalTextHeight ) >> 1 );
		else
			nTextTop = rDestRect.mTopLeft.mY;

        for ( i = 0 ; i < vecSubStrings.size() ; ++i )
        {
			vecSubStrings[i].nTop = nTextTop + (tFontHeight + m_pFont->GetRowSpacing()) * i ;
        }
     
		// Add to the buffer
		s_pDriver->SetCurrentTexture(m_pFont->GetTextTexture());
		s_pDriver->SetSystemColor(m_pFont->GetTextFrontColor()) ;

		int nSubStringCount = vecSubStrings.size() ;
		for(i = 0 ; i < nSubStringCount ; ++i)
		{
             const char *pString = vecSubStrings[i].strSingleLine.c_str() ;
			 int nIndex = 0;
			 CGUIPoint tPoint ;
			 tPoint.mX = vecSubStrings[i].nLeft ;
			 tPoint.mY = vecSubStrings[i].nTop  ;
			 while( pString[ nIndex ] != 0 )
			 {
				 tPoint.mX += m_pFont->PrintText( this, pString + nIndex, tPoint);
				 nIndex += pString[ nIndex ] & 0x80 ? 2 : 1;
			 }

		}

		if(m_pFont->GetTextBackColor() !=0)
		{
			s_pDriver->SetCurrentTexture(NULL);
			s_pDriver->SetSystemColor(m_pFont->GetTextBackColor()) ;
            DrawTextRenderBuffer(rcClip);
		}

		if ( m_pFont->DoesHasShadow())
		{
			s_pDriver->SetCurrentTexture(m_pFont->GetShadowTexture());
			s_pDriver->SetSystemColor(m_pFont->GetTextShadowColor()) ;
            DrawTextRenderBuffer(rcClip);
		}

		if ( m_pFont->DoesHasEdge())
		{
			s_pDriver->SetCurrentTexture(m_pFont->GetEdgeTexture());
			s_pDriver->SetSystemColor(m_pFont->GetTextEdgeColor()) ;
            DrawTextRenderBuffer(rcClip);
		}

		s_pDriver->SetCurrentTexture(m_pFont->GetTextTexture());
		s_pDriver->SetSystemColor(m_pFont->GetTextFrontColor()) ;
        DrawTextRenderBuffer(rcClip);

		if ( m_pFont->IsUnderline())
		{
			CGUIPoint point1 , point2 ;
			for(i = 0 ; i < vecSubStrings.size() ; ++i)
			{
				point1.mX = vecSubStrings[i].nLeft ; 
				point1.mY = vecSubStrings[i].nTop  + tFontHeight * 0.85f;
				point2.mX = vecSubStrings[i].nLeft + vecSubStrings[i].nTextPixelLength ;
				point2.mY = vecSubStrings[i].nTop  + tFontHeight * 0.85f;
				DrawLine(point1, point2, m_pFont->GetTextFrontColor()) ;
			}
		}
		return GUI_ERR_SUCCESS;
	}

	int CGUICanvas::GetTextWidth( const char* pText, int vTextCount )
	{
		return m_pFont->GetTextWidth( pText, vTextCount );
	}

	int CGUICanvas::GetFontHeight( )
	{
		return m_pFont->GetFontHeight( );
	}

	CGUIFont* CGUICanvas::SetFont( CGUIFont* pFont )
	{
		CGUIFont* tpOldFont = m_pFont;
		m_pFont = pFont;
		return tpOldFont;
	}

	CGUIFont* CGUICanvas::GetFont() const 
	{
		return m_pFont ;
	}

	void CGUICanvas::BeginRender()  const
	{
		if ( s_pDriver == NULL )
			s_pDriver = CGUISystem::GetSingleton( ).GetRenderDriver( );
        s_pDriver->PrepareRender() ;
	}

	CGUIRenderDriver* CGUICanvas::GetRenderDriver() 
	{
        return s_pDriver ;
	}

    int CGUICanvas::DrawLine(const CGUIPoint& point1, const CGUIPoint& point2, DWORD dwColor)
	{
		s_pDriver->SetSystemColor(dwColor) ;
		s_pDriver->DrawLine(point1, point2) ;
		 return true ;
	}
	
	void CGUICanvas::EndRender() const 
	{
		s_pDriver->EndRender() ;
	}

	int CGUICanvas::DrawTextRenderBuffer(RECT *rcClip) 
	{
		s_pDriver->DrawTextRenderBuffer(rcClip) ;
		return true ;
	}
}