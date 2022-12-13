/********************************************************************
　created         : 2005/11/07
　filename        : imagedib_i
　author          : season
　version         : 1.0
　description     : 将图片转换成dib内存数据，并使用dib进行图片绘制操作
					包含有alpha通道(Head.PixelDepth=32)的TGA 图片和
					各种格式的bmp图片
　history         :
　1 day	          : 2005/11/07
　　author        : season
　　modification: : created
　2 day	          : 2006/03/28
　　author        : season
　　modification: : adjust it to tenio
*********************************************************************/
#ifndef __IMAGEDIB_I_H__
#define __IMAGEDIB_I_H__

#include "../teniointerface.h"
#include "imagedefine.h"
//////////////////////////////////////////////////////////////////////////

namespace Tenio
{
	class IImageDIB;
	
	TENIO_MAKE_TYPEID_MAP(IImageDIB, COMPONENTID_IMAGEDIB)
		
	class IImageDIB : public IComponent
	{
	public:
		/*	从文件加载，文件名需为全路径,
		 *	bPreMulty仅对TGA有效，表示是否预先计算混合公式中的一部分(只用pre-pixel的都要预先计算)
		 */  
		virtual BOOL LoadFile(
			LPCTSTR lpszFileName, 
			ENMIMAGETYPE nImageType, 
			BOOL bPreMulty) = 0;
		//! 从资源中加载
		virtual BOOL LoadResource(
			HMODULE hModule, 
			UINT nID, 
			ENMIMAGETYPE nImageType, 
			BOOL bPreMulty) = 0;
		//! 创建一个空的32bpp的DIBSection，可将图片绘制(平铺，拉伸)其上
		virtual BOOL CreateEmptyDIB(
			int nWidth, 
			int nHeight,
			COLORREF crBackground, 
			int nAlpha,
			BOOL bPreMulty) = 0;	
		virtual void DeleteDIB() = 0;
		
		/* 直接绘制在DC上。如果是有alpha通道的，则与dc上的已有的背景像素混合,
		 * Stretch操作除外，它不作混合计算。	
		 */ 

		virtual void Draw(
			HDC hdcDst, 
			int nXDst, 
			int nYDst, 
			int nWidth, 
			int nHeight,
			int nXSrc,
			int nYSrc) = 0;
		virtual void Stretch(
			HDC hdcDst,
			int nXOffset,
			int nYOffset,
			int nWidth,
			int nHeight) = 0;
		virtual void Tile(HDC hdcDst, RECT& rc) = 0;

		//! 绘制在DIB中，stretch尚未实现
		virtual void Draw(
			BITMAPINFO* pBMPINFODst, 
			BYTE* pBitsDst,
			int nXDst, 
			int nYDst, 
			int nWidth, 
			int nHeight,
			int nXSrc,
			int nYSrc) = 0;
		virtual void Stretch(
			BITMAPINFO* pBMPINFODst, 
			BYTE* pBitsDst,
			int nXOffset,
			int nYOffset,
			int nWidth,
			int nHeight) = 0;	
		virtual void Tile(BITMAPINFO* pBMPINFODst, BYTE* pBitsDst, RECT& rc) = 0;

		//! 绘制到另一幅图的DIB上，stretch尚未实现
		virtual void Draw(
			IImageDIB* pImageDIBDst,
			int nXDst, 
			int nYDst, 
			int nWidth, 
			int nHeight,
			int nXSrc,
			int nYSrc) = 0;
		virtual void Stretch(
			IImageDIB* pImageDIBDst,
			int nXOffset,
			int nYOffset,
			int nWidth,
			int nHeight) = 0;
		virtual void Tile(IImageDIB* pImageDIBDst, RECT& rc) = 0;

		/*
		 *	在DIB上画text, alpha值设置透明度，范围在0--255之间，0为全透明，不显示。
		 *	\return
		 *		见MSDN　同DrawText()的返回值
		 */ 
		virtual int DrawTextOnItself(
			LPCTSTR lpszText, 
			LPRECT lpRect,
			HFONT hFont,
			COLORREF crText,
			COLORREF crBackground,
			UINT unTextFormat,
			int nBkMode,
			int nAlpha) = 0;
		
		//! 获得图像的size
		virtual void GetImageSize(int& nWidth, int& nHeight) = 0;
		//! 获得像素阵列的指针
		virtual LPBYTE GetDIBits() = 0;
		//! 获得DIB句柄
		virtual HBITMAP GetHBITMAP() = 0;
		//! 获得BITMAPINFO
		virtual LPBITMAPINFO GetBitmapInfo() = 0;
		
		//! 获得某点的值 dwValue = ((rgb值 << 8 ) | alpha值) alpha值在0x00 -0xff之间
 		virtual DWORD GetPixelValue(int x, int y) = 0;
		virtual void  SetPixelValue(int x, int y, DWORD dwValue) = 0;
		
		virtual void  PreMulty() = 0;
	};
};


#endif //__IMAGEDIB_I_H__