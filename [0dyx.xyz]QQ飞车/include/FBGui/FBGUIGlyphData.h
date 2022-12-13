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
		CGUIRect		m_GlyphRect;		//! �����������λ��
		int				m_AdvanceWidth;	//! �����߼����
		bool			m_IsCreated;		//! �Ƿ������Ѿ�����
		wchar_t			m_Unicode;		//! UNICODE����
		CGUIFont*		m_pFont;			//! ��������
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
		//! \brief �õ���������pText�Ƕ��ֽڣ�ָ��һ���ַ�
		\return �õ��������ڵ�����
		*/
		CGUITexture* GetGlyphTexture( );	
		CGUITexture* GetGlyphShadowTexture();
		CGUITexture* GetGlyphEdgeTexture();

		/*
		//! \brief �õ��߼���ȣ�pText�Ƕ��ֽڣ�ָ��һ���ַ�
		\return �õ����ο��
		*/
		int GetGlyphWidth( );										

		/*
		//! \brief	�õ������������е�λ�ã�pText�Ƕ��ֽڣ�ָ��һ���ַ�, 
		����Ŀ�Ⱥ�GetGlyphWidth( )��һ������Ϊ�������е��ַ�һ���ߣ�����GetGlyphRect( )���õ���������ַ���CGUIRect
		\return �ַ���CGUIRect
		*/
		CGUIRect GetGlyphRect( );									
	};
}
