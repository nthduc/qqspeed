#pragma once
#include "FBGUIRenderDriver.h"

#include "FBGUITexture.h"
#include "FBGUIFontManager.h"



namespace FBGUI
{
    class CGUIFont ;
	//! CGUICanvas 类
	/*!
	CGUICanvas 代表了一块画布，如果在构造CGUICanvas的时候指定了关联窗口，那么这块画布代表了整个窗口区域
	以窗口的左上角为原点，并会自动地所有调用按照整个窗口做裁减。如果没有关联窗口，将使用怎么个逻辑屏幕作
	为画布
	*/
	class CGUICanvas
	{
	protected:
		static CGUIRenderDriver*	s_pDriver;			//! 渲染驱动，完成物理绘制
		CGUIFont*					m_pFont;				//! 当前字体


	public:
		//! CGUICanvas 构造函数
		/*! \param CGUIWindow* pWindow 将决定所有函数的调用参数意义(原点),并按照该窗口做裁减 */
		CGUICanvas() 
		{

		}

	protected:
		//! 成员函数 CanvasToScreen, 完成从画布坐标到逻辑屏幕坐标的转换
		/*! \param const CGUIRect& rRect 逻辑矩形
		\return 屏幕矩形 */
		//virtual CGUIRect CanvasToScreen( const CGUIRect& rRect ) const = 0;
		//virtual CGUIPoint CanvasToScreen( const CGUIPoint& rPoint ) const = 0;
		//virtual CGUIRect GetClipRect( const CGUIRect& rRect ) const = 0;

	public:
		// 通过指定宽度得到指定文本的格式化文本
		int GetFormatText( char* pBuffer, const char* pText, int vWidth );


		// 贴图	
		int AddTextToRenderBuffer( const CGUIRect& rSrcRect, const CGUIRect& rDestRect, float fHorShear = 0.0f );
		int DrawTextRenderBuffer(RECT *rcClip) ;
		// 输出文本
		int PrintText( const char* pText, const CGUIRect& rDestRect, unsigned int vFormat = GUI_DT_LEFT | GUI_DT_VCENTER );
		int PrintText( const char* pText, unsigned int nDestLeft, unsigned int nDestTop, RECT *rcClip) ;
        int PrintText( const char* pText, const CGUIRect& rDestRect, RECT *rcClip, unsigned int vFormat = GUI_DT_LEFT | GUI_DT_VCENTER) ;

	    int DrawLine(const CGUIPoint& point1, const CGUIPoint& point2, DWORD dwColor);
		// 得到文本宽度
		int GetTextWidth( const char* pText, int vTextCount );
		// 得到字体高度
		int GetFontHeight( );
		// 设置字体
        CGUIRenderDriver* GetRenderDriver();
		CGUIFont* SetFont( CGUIFont* pFont );
        CGUIFont* GetFont() const ; 
		void BeginRender() const ;
		void EndRender() const ;
	};
}