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
	 * @brief ����ximage���ͼ�����
	 *
	 * <p>
	 *���� ͼ��Ԫ�ص�һ�����ͣ�����ʵ���ǻ���ximageϵ�о�̬�⡣\n
	 *	Ŀǰ���Զ�ȡbmp,gif,tga,jpg��ʽ��ͼ��ʹ�ͼ�����������ж���ͼ��
	 * </p>
	 *
	 * @sa IImage 
	 */
	class IxImageLib : public IImage, public IComponent
	{
	public:
		/**
		 *  @brief ����͸����
		 *  @param byAlphaSet alphaֵ����Χ��0 ~ 255
		 *  @sa 
		 */	
		virtual void SetPaintAlpha(BYTE byAlphaSet) = 0;

		/**
		 *  @brief ����ͼ���ļ�
		 *  @param lpszFileName��ͼ���ļ�ȫ·����
		 *  @return �ɹ��򷵻�TRUE��ʧ���򷵻�FALSE
		 *  @sa 
		 */	
		virtual BOOL LoadImage(LPCTSTR lpszFileName) = 0;

		/**
		 *  @brief ���ļ�buffer����ͼ��
		 *  @param lpszFileName ͼ���ļ�������Ҫ������ʶ��ͼ���ʽ
		 *  @param pbyBuffer ͼ���ļ����ݴ�ŵĻ�����ָ��
		 *  @param nSize��ͼ�񻺳�����С
		 *  @return �ɹ��򷵻�TRUE��ʧ���򷵻�FALSE
		 *  @sa 
		 */	
		virtual BOOL LoadImage(LPCTSTR lpszFileName, const BYTE* pbyBuffer, UINT nSize) = 0;		

		/**
		 *  @brief �����������ж���ͼ��
		 *  @param pArray ��������ָ��
		 *  @param dwWidth	ͼ����
		 *	@param dwHeight	ͼ��߶�
		 *	@param dwBitsPerPixer ÿ��������ռλ��,������ 1,4,8,24,32
		 *	@param dwBytesPerLine �ж��룬ÿ���ֽ�����
		 *	@param bFlipImage �Ƿ���Ҫ������������ͼ���ǵ���ʱ��Ҫ��
		 *  @return �ɹ��򷵻�TRUE��ʧ���򷵻�FALSE
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
		 *  @brief ��ͼ����Ƶ�DC��
		 *  @param hDC Ŀ��DC�ľ��
		 *  @param nDestX Ŀ��������ʼx����
		 *  @param nDestY Ŀ��������ʼy����
		 *  @param nSrcX ͼ����ʼx����
		 *  @param nSrcY ͼ����ʼy����
		 *  @param nWidth ͼ�񱻻��ƵĿ��
		 *  @param nHeight ͼ�񱻻��Ƶĸ߶�
		 *  @return �ɹ��򷵻�TRUE��ʧ���򷵻�FALSE
		 *  @sa 
		 */
		virtual BOOL DrawAtDc(HDC hDC, int nDestX, int nDestY, int nSrcX, 
			int nSrcY, int nWidth, int nHeight) = 0;

		/**
		 *  @brief ���Ż���
		 *  @param hDC Ŀ��DC
		 *  @param DstRect Ŀ�ľ���
		 *  @param SrcRect Դ����
		 *  @param dwRop �����룬���MSDN BitBlt����
		 *  @return �ɹ��򷵻�TRUE��ʧ���򷵻�FALSE
		 *  @sa 
		 */
		virtual BOOL StretchAtDc(HDC hDC, const RECT& DstRect, const RECT& SrcRect, DWORD dwRop = SRCCOPY) = 0;
		
		/**
		 *  @brief ƽ�̻���
		 *  @param hDC Ŀ��DC
		 *  @param rect Ŀ�ľ���
		 *  @return �ɹ��򷵻�TRUE��ʧ���򷵻�FALSE
		 *  @sa 
		 */
		virtual BOOL TileAtDc(HDC hDC, const RECT& rect) = 0;
		
		/**
		 *  @brief ����һ���յ�XImage
		 *  @param nWidth Image�Ŀ�
		 *  @param nHeight Image�ĸ�
		 *  @return �ɹ��򷵻�TRUE�����򷵻�FALSE
		 *  @sa
		 */
		virtual BOOL Create(int nWidth, int nHeight) = 0;

		/**
		 *  @brief ��pxImageԴ�������Alpha���
		 *  @param lXOffset Ŀ�Ķ����Ͽ�ʼ��x����
		 *  @param lYOffset Ŀ�Ķ����Ͽ�ʼ��y����
		 *  @param lSrcX Դ�����Ͽ�ʼ��y����
		 *  @param lSrcY Դ�����Ͽ�ʼ��y����
		 *  @param nSrcWidth Դ�����ʵ�ʿ��(֧�ּ���)
		 *  @param nSrcHeight Դ�����ʵ�ʸ߶�(֧�ּ���)
		 *  @sa
		 */
		virtual void AlphaBlend(IxImageLib* pxImage, 
			long lDstX, long lDstY, int nDstWidth, int nDstHeight,
			long lSrcX, long lSrcY, int nSrcWidth, int nSrcHeight,
			BOOL bIsXMirror, BOOL bIsYMirror, BOOL bIsStretch) = 0;

		/**
		 *  @brief ��xImage�ϻ���
		 *  @param ptStart ���ߵ���ʼλ��
		 *  @param ptEnd ���ߵĽ���λ��
		 *  @return cr �ߵ���ɫ
		 *  @sa
		 */
		virtual void DrawLine(const POINT& ptStart, const POINT& ptEnd, COLORREF cr) = 0;

		/**
		 *  @brief ���xImage�е����ػ�����,��D3D��A8R8G8B8��ʽ�ı��滺������ʽ����
		 *  @param pBuff ����xImage�������ػ�����
		 *  @param ptEnd ����xImage�������ػ�������С
		 */
		virtual void GetPixelBuffer(BYTE* &pBuff, long &lSize) = 0;

		/**
		 *  @brief ��xImage��д��
		 *  @param hMemDC �ڴ�DC, DC�м�¼���ֵĸ�ʽ(��ɫ,�����)
		 *  @param x Ŀ��x����
		 *  @param y Ŀ��y����
		 *  @param rcClip ���о���
		 *  @param lpszText �ı�����
		 *  @param iLen �ı�����
		 */
		virtual void DrawText(HDC hMemDC, int x, int y, const RECT &rcClip, LPCTSTR lpszText, int iLen) = 0;

		/**
		 *  @brief ����ڲ�CxImageָ��,June�ڲ�ʹ��
		 *  @return CxImage�ڲ�ָ��
		 */
		virtual HANDLE GetxImage() = 0;

		/**
		 *  @brief ������豸�޹�λͼ���
		 *  @return ����DIBHandle
		 */	
		virtual HBITMAP GetDIBHandle() = 0;

		/**
		 *  @brief ��RECT�������������alphaֵ�ظ�
		 *	@param lpRect ��Ҫ����������ΪNULLʱ��ʾȫ��
		 *  @param cr ����ɫ
		 *  @param int nAlpha �µ�alphaֵ
		 */
		virtual void Erase(RECT* lpRect, COLORREF cr, int nAlpha) = 0;		
	};
}

#endif