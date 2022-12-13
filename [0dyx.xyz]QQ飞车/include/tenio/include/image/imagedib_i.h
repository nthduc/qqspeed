/********************************************************************
��created         : 2005/11/07
��filename        : imagedib_i
��author          : season
��version         : 1.0
��description     : ��ͼƬת����dib�ڴ����ݣ���ʹ��dib����ͼƬ���Ʋ���
					������alphaͨ��(Head.PixelDepth=32)��TGA ͼƬ��
					���ָ�ʽ��bmpͼƬ
��history         :
��1 day	          : 2005/11/07
����author        : season
����modification: : created
��2 day	          : 2006/03/28
����author        : season
����modification: : adjust it to tenio
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
		/*	���ļ����أ��ļ�����Ϊȫ·��,
		 *	bPreMulty����TGA��Ч����ʾ�Ƿ�Ԥ�ȼ����Ϲ�ʽ�е�һ����(ֻ��pre-pixel�Ķ�ҪԤ�ȼ���)
		 */  
		virtual BOOL LoadFile(
			LPCTSTR lpszFileName, 
			ENMIMAGETYPE nImageType, 
			BOOL bPreMulty) = 0;
		//! ����Դ�м���
		virtual BOOL LoadResource(
			HMODULE hModule, 
			UINT nID, 
			ENMIMAGETYPE nImageType, 
			BOOL bPreMulty) = 0;
		//! ����һ���յ�32bpp��DIBSection���ɽ�ͼƬ����(ƽ�̣�����)����
		virtual BOOL CreateEmptyDIB(
			int nWidth, 
			int nHeight,
			COLORREF crBackground, 
			int nAlpha,
			BOOL bPreMulty) = 0;	
		virtual void DeleteDIB() = 0;
		
		/* ֱ�ӻ�����DC�ϡ��������alphaͨ���ģ�����dc�ϵ����еı������ػ��,
		 * Stretch�������⣬��������ϼ��㡣	
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

		//! ������DIB�У�stretch��δʵ��
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

		//! ���Ƶ���һ��ͼ��DIB�ϣ�stretch��δʵ��
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
		 *	��DIB�ϻ�text, alphaֵ����͸���ȣ���Χ��0--255֮�䣬0Ϊȫ͸��������ʾ��
		 *	\return
		 *		��MSDN��ͬDrawText()�ķ���ֵ
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
		
		//! ���ͼ���size
		virtual void GetImageSize(int& nWidth, int& nHeight) = 0;
		//! ����������е�ָ��
		virtual LPBYTE GetDIBits() = 0;
		//! ���DIB���
		virtual HBITMAP GetHBITMAP() = 0;
		//! ���BITMAPINFO
		virtual LPBITMAPINFO GetBitmapInfo() = 0;
		
		//! ���ĳ���ֵ dwValue = ((rgbֵ << 8 ) | alphaֵ) alphaֵ��0x00 -0xff֮��
 		virtual DWORD GetPixelValue(int x, int y) = 0;
		virtual void  SetPixelValue(int x, int y, DWORD dwValue) = 0;
		
		virtual void  PreMulty() = 0;
	};
};


#endif //__IMAGEDIB_I_H__