#ifndef __XIMAGELIB_I_H__
#define __XIMAGELIB_I_H__

#include "../teniointerface.h"
#include "image_i.h"
 
namespace Tenio
{
	class IxImageLib;

	TENIO_MAKE_TYPEID_MAP(IxImageLib, COMPONENTID_XIMAGELIB);

	/**
	 * @ingroup tenuilib
	 * @brief 基于ximage库的图像对象
	 *
	 * <p>
	 *　　 图像元素的一种类型，它的实现是基于ximage系列静态库。\n
	 *	目前可以读取bmp,gif,tga,jpg格式的图像和从图像像素阵列中读入图像。
	 * </p>
	 *
	 * @sa IImage 
	 */
	class IxImageLib : public IImage, public IComponent
	{
	public:
		/**
		 *  @brief 设置透明度
		 *  @param byAlphaSet alpha值，范围在0 ~ 255
		 *  @sa 
		 */	
		virtual void SetPaintAlpha(BYTE byAlphaSet) = 0;

		/**
		 *  @brief 加载图像文件
		 *  @param lpszFileName，图像文件全路径名
		 *  @return 成功则返回TRUE，失败则返回FALSE
		 *  @sa 
		 */	
		virtual BOOL LoadImage(LPCTSTR lpszFileName) = 0;

		/**
		 *  @brief 从文件buffer加载图像
		 *  @param lpszFileName 图像文件名，主要是用于识别图像格式
		 *  @param pbyBuffer 图像文件数据存放的缓冲区指针
		 *  @param nSize　图像缓冲区大小
		 *  @return 成功则返回TRUE，失败则返回FALSE
		 *  @sa 
		 */	
		virtual BOOL LoadImage(LPCTSTR lpszFileName, const BYTE* pbyBuffer, UINT nSize) = 0;		

		/**
		 *  @brief 从像素阵列中读出图像
		 *  @param pArray 像素阵列指针
		 *  @param dwWidth	图像宽度
		 *	@param dwHeight	图像高度
		 *	@param dwBitsPerPixer 每个像素所占位数,可以是 1,4,8,24,32
		 *	@param dwBytesPerLine 行对齐，每行字节数。
		 *	@param bFlipImage 是否需要调整参数，当图像是倒置时需要。
		 *  @return 成功则返回TRUE，失败则返回FALSE
		 *  @sa 
		 */
		virtual BOOL CreateFromArray(
			BYTE* pArray,
			DWORD dwWidth,
			DWORD dwHeight,
			DWORD dwBitsPerPixel, 
			DWORD dwBytesPerLine, 
			BOOL bFlipImage) = 0;
		
		/**
		 *  @brief 将图像绘制到DC上
		 *  @param hDC 目的DC的句柄
		 *  @param nDestX 目的区域起始x坐标
		 *  @param nDestY 目的区域起始y坐标
		 *  @param nSrcX 图像起始x坐标
		 *  @param nSrcY 图像起始y坐标
		 *  @param nWidth 图像被绘制的宽度
		 *  @param nHeight 图像被绘制的高度
		 *  @return 成功则返回TRUE，失败则返回FALSE
		 *  @sa 
		 */
		virtual BOOL DrawAtDc(HDC hDC, int nDestX, int nDestY, int nSrcX, 
			int nSrcY, int nWidth, int nHeight) = 0;

		/**
		 *  @brief 缩放绘制
		 *  @param hDC 目的DC
		 *  @param DstRect 目的矩形
		 *  @param SrcRect 源矩形
		 *  @param dwRop 操作码，详见MSDN BitBlt函数
		 *  @return 成功则返回TRUE，失败则返回FALSE
		 *  @sa 
		 */
		virtual BOOL StretchAtDc(HDC hDC, const RECT& DstRect, const RECT& SrcRect, DWORD dwRop = SRCCOPY) = 0;
		
		/**
		 *  @brief 平铺绘制
		 *  @param hDC 目的DC
		 *  @param rect 目的矩形
		 *  @return 成功则返回TRUE，失败则返回FALSE
		 *  @sa 
		 */
		virtual BOOL TileAtDc(HDC hDC, const RECT& rect) = 0;
		
		/**
		 *  @brief 创建一个空的XImage
		 *  @param nWidth Image的宽
		 *  @param nHeight Image的高
		 *  @return 成功则返回TRUE，否则返回FALSE
		 *  @sa
		 */
		virtual BOOL Create(int nWidth, int nHeight) = 0;

		/**
		 *  @brief 与pxImage源对象进行Alpha混合
		 *  @param lXOffset 目的对象混合开始的x坐标
		 *  @param lYOffset 目的对象混合开始的y坐标
		 *  @param lSrcX 源对象混合开始的y坐标
		 *  @param lSrcY 源对象混合开始的y坐标
		 *  @param nSrcWidth 源对象的实际宽度(支持剪裁)
		 *  @param nSrcHeight 源对象的实际高度(支持剪裁)
		 *  @sa
		 */
		virtual void AlphaBlend(IxImageLib* pxImage, 
			long lDstX, long lDstY, int nDstWidth, int nDstHeight,
			long lSrcX, long lSrcY, int nSrcWidth, int nSrcHeight,
			BOOL bIsXMirror, BOOL bIsYMirror, BOOL bIsStretch) = 0;

		/**
		 *  @brief 在xImage上画线
		 *  @param ptStart 画线的起始位置
		 *  @param ptEnd 画线的结束位置
		 *  @return cr 线的颜色
		 *  @sa
		 */
		virtual void DrawLine(const POINT& ptStart, const POINT& ptEnd, COLORREF cr) = 0;

		/**
		 *  @brief 获得xImage中的象素缓冲区,与D3D中A8R8G8B8格式的表面缓冲区格式兼容
		 *  @param pBuff 将由xImage填充的象素缓冲区
		 *  @param ptEnd 将由xImage填充的象素缓冲区大小
		 */
		virtual void GetPixelBuffer(BYTE* &pBuff, long &lSize) = 0;

		/**
		 *  @brief 在xImage上写字
		 *  @param hMemDC 内存DC, DC中记录了字的格式(颜色,字体等)
		 *  @param x 目的x坐标
		 *  @param y 目的y坐标
		 *  @param rcClip 剪切矩形
		 *  @param lpszText 文本内容
		 *  @param iLen 文本长度
		 */
		virtual void DrawText(HDC hMemDC, int x, int y, const RECT &rcClip, LPCTSTR lpszText, int iLen) = 0;

		/**
		 *  @brief 获得内部CxImage指针,June内部使用
		 *  @return CxImage内部指针
		 */
		virtual HANDLE GetxImage() = 0;

		/**
		 *  @brief 获得与设备无关位图句柄
		 *  @return 返回DIBHandle
		 */	
		virtual HBITMAP GetDIBHandle() = 0;

		/**
		 *  @brief 将RECT所有数据清除，alpha值重赋
		 *	@param lpRect 需要擦除的区域，为NULL时表示全部
		 *  @param cr 背景色
		 *  @param int nAlpha 新的alpha值
		 */
		virtual void Erase(RECT* lpRect, COLORREF cr, int nAlpha) = 0;		
	};
}

#endif