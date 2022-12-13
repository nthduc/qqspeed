#ifndef __NORMALFONT_I_H__
#define __NORMALFONT_I_H__

#include "font_i.h"
#include "June/ximagelib_i.h"

namespace Tenio
{
	/**
	 * @ingroup 
	 * @brief 普通字体对象
	 *
	 * <p>
	 *　　 基于windows gdi　HFONT实现
	 * </p>
	 *
	 * @sa 
	 */
	class INormalFont : public IFont
	{
	public:
		/**
		 *  @brief 获取GDI句柄
		 *  @return 返回HFONT
		 *  @sa 
		 */	
		virtual HFONT GetGdiHandle() = 0;

		/**
		 *  @brief 绘制文本
		 *  @param hDC 目的DC
		 *  @param x 目的DC绘制初始x坐标
		 *  @param y 目的DC绘制初始y坐标
		 *  @param rcClip clip区域，相对于dc
		 *  @param lpszText 绘制的文字
		 *  @param nSelStart 选中区域起始字符位置，见 LOCATION 
		 *  @param nSelEnd　选中区域结束字符位置
		 *  @param colSelText 选中文字的颜色
		 *  @param colSelBk 选中文字的背景色
		 *  @return 成功返回TRUE，失败则返回FALSE
		 *  @sa 
		 */		
		virtual BOOL DrawAtDc(HDC hDC, int x, int y, const RECT& rcClip, 
			LPCTSTR lpszText, int nSelStart, int nSelEnd, BYTE byAlpha,
			COLORREF colSelText, COLORREF colSelBk) = 0;

		/**
		 *  @brief 向xImage上绘制文本
		 *  @param hDC 内存DC
		 *  @param pxImage xImage指针
		 *  @param x 目的DC绘制初始x坐标
		 *  @param y 目的DC绘制初始y坐标
		 *  @param rcClip clip区域，相对于dc
		 *  @param lpszText 绘制的文字
		 *  @param nSelStart 选中区域起始字符位置，见 LOCATION 
		 *  @param nSelEnd　选中区域结束字符位置
		 *  @param colSelText 选中文字的颜色
		 *  @param colSelBk 选中文字的背景色
		 *  @return 成功返回TRUE，失败则返回FALSE
		 *  @sa 
		 */		
		virtual BOOL DrawAtxImage(HDC hMemDC, IxImageLib* pxImage, int x, int y, const RECT& rcClip, 
			LPCTSTR lpszText, int nSelStart, int nSelEnd, BYTE byAlpha,
			COLORREF colSelText, COLORREF colSelBk) = 0;
	};
};

#endif // __NORMALFONT_I_H__