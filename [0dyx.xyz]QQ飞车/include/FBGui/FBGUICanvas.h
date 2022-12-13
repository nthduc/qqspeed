#pragma once
#include "FBGUIRenderDriver.h"

#include "FBGUITexture.h"
#include "FBGUIFontManager.h"



namespace FBGUI
{
    class CGUIFont ;
	//! CGUICanvas ��
	/*!
	CGUICanvas ������һ�黭��������ڹ���CGUICanvas��ʱ��ָ���˹������ڣ���ô��黭��������������������
	�Դ��ڵ����Ͻ�Ϊԭ�㣬�����Զ������е��ð��������������ü������û�й������ڣ���ʹ����ô���߼���Ļ��
	Ϊ����
	*/
	class CGUICanvas
	{
	protected:
		static CGUIRenderDriver*	s_pDriver;			//! ��Ⱦ����������������
		CGUIFont*					m_pFont;				//! ��ǰ����


	public:
		//! CGUICanvas ���캯��
		/*! \param CGUIWindow* pWindow ���������к����ĵ��ò�������(ԭ��),�����ոô������ü� */
		CGUICanvas() 
		{

		}

	protected:
		//! ��Ա���� CanvasToScreen, ��ɴӻ������굽�߼���Ļ�����ת��
		/*! \param const CGUIRect& rRect �߼�����
		\return ��Ļ���� */
		//virtual CGUIRect CanvasToScreen( const CGUIRect& rRect ) const = 0;
		//virtual CGUIPoint CanvasToScreen( const CGUIPoint& rPoint ) const = 0;
		//virtual CGUIRect GetClipRect( const CGUIRect& rRect ) const = 0;

	public:
		// ͨ��ָ����ȵõ�ָ���ı��ĸ�ʽ���ı�
		int GetFormatText( char* pBuffer, const char* pText, int vWidth );


		// ��ͼ	
		int AddTextToRenderBuffer( const CGUIRect& rSrcRect, const CGUIRect& rDestRect, float fHorShear = 0.0f );
		int DrawTextRenderBuffer(RECT *rcClip) ;
		// ����ı�
		int PrintText( const char* pText, const CGUIRect& rDestRect, unsigned int vFormat = GUI_DT_LEFT | GUI_DT_VCENTER );
		int PrintText( const char* pText, unsigned int nDestLeft, unsigned int nDestTop, RECT *rcClip) ;
        int PrintText( const char* pText, const CGUIRect& rDestRect, RECT *rcClip, unsigned int vFormat = GUI_DT_LEFT | GUI_DT_VCENTER) ;

	    int DrawLine(const CGUIPoint& point1, const CGUIPoint& point2, DWORD dwColor);
		// �õ��ı����
		int GetTextWidth( const char* pText, int vTextCount );
		// �õ�����߶�
		int GetFontHeight( );
		// ��������
        CGUIRenderDriver* GetRenderDriver();
		CGUIFont* SetFont( CGUIFont* pFont );
        CGUIFont* GetFont() const ; 
		void BeginRender() const ;
		void EndRender() const ;
	};
}