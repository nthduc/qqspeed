#include "FBGUIGlyphData.h"
#include "FBGUISystem.h"

namespace FBGUI
{

	// ************************************************************ //
	// CGUIGlyphData
	// ************************************************************ //

	// 得到字形纹理
	CGUITexture* CGUIGlyphData::GetGlyphTexture()
	{
		if ( m_IsCreated == false ) 
		{
			CreateGlyph();
		}
		m_pFont->m_pUsedPool.erase(m_iter);
		m_iter = m_pFont->m_pUsedPool.insert(m_pFont->m_pUsedPool.begin(),this);
		return m_pFont->m_pTexture;
	}
	CGUITexture* CGUIGlyphData::GetGlyphShadowTexture()
	{
		if ( m_IsCreated == false ) 
		{
			CreateGlyph();
		}

		return m_pFont->m_pShadowTexture ;
	}

	CGUITexture* CGUIGlyphData::GetGlyphEdgeTexture()
	{
		if ( m_IsCreated == false ) 
		{
			CreateGlyph();
		}

		return m_pFont->m_pEdgeTexture ;
	}

	// 得到逻辑宽度
	int CGUIGlyphData::GetGlyphWidth()
	{
		if ( m_IsCreated == false ) 
		{
			CreateGlyph();
		}
		return m_AdvanceWidth;
	}

	// 得到字形在纹理中的位置
	CGUIRect CGUIGlyphData::GetGlyphRect()
	{
		if ( m_IsCreated == false ) 
		{
			CreateGlyph();
		}
		return m_GlyphRect;
	}

	int CGUIGlyphData::FillFont( const CGUIRect& rRect,bool bNeedClean )
	{
		FT_Size_Metrics* tpMetrics	=  &m_pFont->m_pFTFace->size->metrics;
		FT_GlyphSlot     tpSlot	    =  m_pFont->m_pFTFace->glyph;
		FT_Load_Char( m_pFont->m_pFTFace, m_Unicode, FT_LOAD_RENDER | FT_LOAD_FORCE_AUTOHINT | 
			          ( m_pFont->m_AntiAliased ? FT_LOAD_TARGET_NORMAL : FT_LOAD_MONOCHROME | FT_LOAD_TARGET_MONO ) ); 

		int tBoxWidth	= tpSlot->bitmap.width;
		int tBoxHeight	= tpSlot->bitmap.rows;
		int tOriginX	= 0;
		int tOriginY	= tpSlot->bitmap_top;
		int tMask		= 0;
		int tPitch		= m_pFont->m_pTexture->GetTexWidth( );
		int tCorrectedX = GUI_MAX( 0, m_pFont->m_nEdgeWidth *2 - m_pFont->m_nShadowOffsetX ) + GUI_MAX( 0, m_pFont->m_nEdgeWidth*2  + m_pFont->m_nShadowOffsetX );
		int tCorrectedY = GUI_MAX( 0, m_pFont->m_nEdgeWidth *2 - m_pFont->m_nShadowOffsetY ) + GUI_MAX( 0, m_pFont->m_nEdgeWidth*2  + m_pFont->m_nShadowOffsetY );

		D3DLOCKED_RECT d3dlr , d3dTexRcShadow, d3dTexRcEdge;
		unsigned int *tpShadowColour = NULL ;
		unsigned int *tpEdgeColour   = NULL ;
		unsigned int* tpTextColour	     = NULL ;

		m_pFont->m_pTexture->m_TextCasheTexture->LockRect(0, &d3dlr, NULL, D3DLOCK_NOSYSLOCK);
		tpTextColour	   = (unsigned int*)d3dlr.pBits;//pTexture->m_TextCasheTexture->get
		
		if (m_pFont->m_bHasShadow && m_pFont->m_pShadowTexture)
		{
             m_pFont->m_pShadowTexture->m_TextCasheTexture->LockRect(0, &d3dTexRcShadow, NULL, D3DLOCK_NOSYSLOCK);  
		     tpShadowColour = (unsigned int*)d3dTexRcShadow.pBits;
		}

		if (m_pFont->m_bHasEdge && m_pFont->m_pEdgeTexture)
		{
			 m_pFont->m_pEdgeTexture->m_TextCasheTexture->LockRect(0, &d3dTexRcEdge, NULL, D3DLOCK_NOSYSLOCK);
			 tpEdgeColour   = (unsigned int*)d3dTexRcEdge.pBits ;
		}
	
		unsigned int tColour	     = 0;
		int tMaxWidth			     = tBoxWidth + tCorrectedX;
		// 如果是空格，要特殊处理一下，认为宽度为字形图片矩形宽度的一半
		if ( m_Unicode == 32 )
		{
			tMaxWidth = m_pFont->m_UnitWidth / 2;
		}
		if(bNeedClean)
		{
			for ( int m = rRect.mTopLeft.mX; m < rRect.mBottomRight.mX; m ++ )
			{
				for ( int n = rRect.mTopLeft.mY; n <rRect.mBottomRight.mY; n ++ )
				{
					tpTextColour[ n* tPitch + m]           = 0 ;

					if (tpShadowColour)
					{
						tpShadowColour[ n* tPitch + m] = 0 ;
					}

					if (tpEdgeColour)
					{
						tpEdgeColour[n* tPitch + m]    = 0 ;
					}
				}
			}
		}

		for ( int x = 0; x < tBoxWidth; x ++ )
		{
			for ( int y = 0; y < tBoxHeight; y ++ )
			{
				int tX = x + tOriginX + GUI_MAX( 0, m_pFont->m_nEdgeWidth  - m_pFont->m_nShadowOffsetX ) + 1 ;
				int tY = int( m_pFont->GetFontHeight( ) - tOriginY + y + tpMetrics->descender / 64.0f ) - GUI_MAX( 0, m_pFont->m_nEdgeWidth  + m_pFont->m_nShadowOffsetY );
				
				if ( tY < 0 || tX < 0 )
				{
					continue;
				}				
				
				switch ( tpSlot->bitmap.pixel_mode )
				{
				case FT_PIXEL_MODE_GRAY:
					tMask = tpSlot->bitmap.buffer[ y * tpSlot->bitmap.pitch + x ];
					break;
				case FT_PIXEL_MODE_MONO:
					tMask = ( ( tpSlot->bitmap.buffer[ ( y * tpSlot->bitmap.pitch ) + x / 8 ] << ( x % 8 ) ) & 0x80 ) ? 255 : 0;
					break;
				}

				if ( tMask >= 1 )
				{
					tpTextColour[ ( tY + rRect.mTopLeft.mY ) * tPitch + tX + rRect.mTopLeft.mX ] = GUI_MAKECOLOUR( tMask, 255, 255, 255 );

					if (m_pFont->m_bHasShadow)
					{
						int tY1 = tY  + m_pFont->m_nShadowOffsetY;
						int tX1 = tX  + m_pFont->m_nShadowOffsetX;
						tMaxWidth = GUI_MAX( tX1, tMaxWidth );
						if ( ( tpShadowColour[ ( tY1 + rRect.mTopLeft.mY ) * tPitch + tX1 + rRect.mTopLeft.mX ] & 0x00FFFFFF ) == 0 )
						{
							tpShadowColour[ ( tY1 + rRect.mTopLeft.mY ) * tPitch + tX1 + rRect.mTopLeft.mX ] = GUI_MAKECOLOUR( 255, 255, 255, 255 );
						}
					}

					if (m_pFont->m_bHasEdge)
					{
						for ( int m = 0; m <= m_pFont->m_nEdgeWidth ; m ++ )
						{
							for ( int n = 0; n <= m_pFont->m_nEdgeWidth ; n ++ )
							{
								int tY1 = tY + n ;
								int tX1 = tX + m ;
								tMaxWidth = GUI_MAX( tX1, tMaxWidth );
								if ( ( tpEdgeColour[ ( tY1 + rRect.mTopLeft.mY ) * tPitch + tX1 + rRect.mTopLeft.mX ] & 0x00FFFFFF ) == 0 )
								{
									tpEdgeColour[ ( tY1 + rRect.mTopLeft.mY ) * tPitch + tX1 + rRect.mTopLeft.mX ] = GUI_MAKECOLOUR( 255, 255, 255, 255 );
								}

								tY1 = tY + n  ;
								tX1 = tX - m  ;
								tMaxWidth = GUI_MAX( tX1, tMaxWidth );
								if ( ( tpEdgeColour[ ( tY1 + rRect.mTopLeft.mY ) * tPitch + tX1 + rRect.mTopLeft.mX ] & 0x00FFFFFF ) == 0 )
								{
									tpEdgeColour[ ( tY1 + rRect.mTopLeft.mY ) * tPitch + tX1 + rRect.mTopLeft.mX ] = GUI_MAKECOLOUR( 255, 255, 255, 255 );
								}

								tY1 = tY - n  ;
								tX1 = tX + m  ;
								tMaxWidth = GUI_MAX( tX1, tMaxWidth );
								if ( ( tpEdgeColour[ ( tY1 + rRect.mTopLeft.mY ) * tPitch + tX1 + rRect.mTopLeft.mX ] & 0x00FFFFFF ) == 0 )
								{
									tpEdgeColour[ ( tY1 + rRect.mTopLeft.mY ) * tPitch + tX1 + rRect.mTopLeft.mX ] = GUI_MAKECOLOUR( 255, 255, 255, 255 );
								}

								tY1 = tY - n ;
								tX1 = tX - m ;
								tMaxWidth = GUI_MAX( tX1, tMaxWidth );
								if ( ( tpEdgeColour[ ( tY1 + rRect.mTopLeft.mY ) * tPitch + tX1 + rRect.mTopLeft.mX ] & 0x00FFFFFF ) == 0 )
								{
									tpEdgeColour[ ( tY1 + rRect.mTopLeft.mY ) * tPitch + tX1 + rRect.mTopLeft.mX ] = GUI_MAKECOLOUR( 255, 255, 255, 255 );
								}
							}
					    }
					}
				}
			}
		}	

		m_pFont->m_pTexture->m_TextCasheTexture->UnlockRect( NULL );
		if (m_pFont->m_pShadowTexture)
		{
		    m_pFont->m_pShadowTexture->m_TextCasheTexture->UnlockRect(NULL) ;
		}
		if (m_pFont->m_pEdgeTexture)
		{
            m_pFont->m_pEdgeTexture->m_TextCasheTexture->UnlockRect(NULL) ;
		}

		m_GlyphRect.SetRect( rRect.TopLeft(), CGUISize( tMaxWidth, m_pFont->GetFontHeight( ) ) );
		m_AdvanceWidth = tMaxWidth;
		return GUI_ERR_SUCCESS;
	}

	int CGUIGlyphData::CreateGlyph()
	{
		// 计算一张纹理上有多少个字形
		int tFontCountInHorz	 = ( FONT_GLYPH_TEXTURE_WIDTH / m_pFont->m_UnitWidth );
		int tFontCountInVert	 = ( FONT_GLYPH_TEXTURE_HEIGHT / m_pFont->m_UnitHeight );
		int tFontCountPerTexture = tFontCountInHorz * tFontCountInVert;

		// 计算一个可以用的“位置”，“位置”代表了纹理和纹理区域
		CGUIRect tFontRect;
		bool bNeedClean = false;
		if(m_pFont->m_FontUsed == tFontCountPerTexture)
		{
			std::list<CGUIGlyphData*>::iterator iter = m_pFont->m_pUsedPool.end();
			iter--;
			CGUIGlyphData* pLastGlyph = (*iter);
			m_pFont->m_pUsedPool.erase(iter);
			tFontRect = pLastGlyph->m_GlyphRect;
			pLastGlyph->m_IsCreated = false;
			bNeedClean = true;
		}
		else
		{
			int tFontIndex		= m_pFont->m_FontUsed % tFontCountPerTexture;	// 纹理区域
			int tTextureIndex	= m_pFont->m_FontUsed / tFontCountPerTexture;	// 纹理
			int tHorzIndex		= tFontIndex % tFontCountInHorz;
			int tVertIndex		= tFontIndex / tFontCountInHorz;
			tFontRect = CGUIRect( tHorzIndex * m_pFont->m_UnitWidth,         tVertIndex * m_pFont->m_UnitHeight, 
				( tHorzIndex + 1 ) * m_pFont->m_UnitWidth, ( tVertIndex + 1 ) * m_pFont->m_UnitHeight );
			m_pFont->m_FontUsed ++;
		}
		m_iter = m_pFont->m_pUsedPool.insert(m_pFont->m_pUsedPool.begin(),this);

		char tTexName[ 255 ] = { 0 }; 
		::sprintf( tTexName, "Font%s", m_pFont->m_FontName.c_str() );
		// 如果纹理没有建立
		CGUIRenderDriver* tpRenderDriver = CGUISystem::GetSingletonPtr()->GetRenderDriver();
		if(!m_pFont->m_pTexture)
		{
			m_pFont->m_pTexture = tpRenderDriver->CreateTexture( tTexName, FONT_GLYPH_TEXTURE_WIDTH, FONT_GLYPH_TEXTURE_HEIGHT );
		}

		if (m_pFont->m_bHasShadow)
		{
			if (!m_pFont->m_pShadowTexture)
			{
				::sprintf( tTexName, "shadow_Font%s", m_pFont->m_FontName.c_str() );
				m_pFont->m_pShadowTexture = tpRenderDriver->CreateTexture( tTexName , FONT_GLYPH_TEXTURE_WIDTH , FONT_GLYPH_TEXTURE_HEIGHT) ;
			}
		}

		if (m_pFont->m_bHasEdge)
		{
			if (!m_pFont->m_pEdgeTexture)
			{
		          ::sprintf( tTexName, "Edge_Font%s", m_pFont->m_FontName.c_str() );
			      m_pFont->m_pEdgeTexture = tpRenderDriver->CreateTexture( tTexName , FONT_GLYPH_TEXTURE_WIDTH , FONT_GLYPH_TEXTURE_HEIGHT) ;
			}
		}

		FillFont( tFontRect ,bNeedClean);

		m_IsCreated = true;
		return GUI_ERR_SUCCESS;
	}
}