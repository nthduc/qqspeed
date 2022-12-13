#ifndef __NORMALFONT_I_H__
#define __NORMALFONT_I_H__

#include "font_i.h"
#include "June/ximagelib_i.h"

namespace Tenio
{
	/**
	 * @ingroup 
	 * @brief ��ͨ�������
	 *
	 * <p>
	 *���� ����windows gdi��HFONTʵ��
	 * </p>
	 *
	 * @sa 
	 */
	class INormalFont : public IFont
	{
	public:
		/**
		 *  @brief ��ȡGDI���
		 *  @return ����HFONT
		 *  @sa 
		 */	
		virtual HFONT GetGdiHandle() = 0;

		/**
		 *  @brief �����ı�
		 *  @param hDC Ŀ��DC
		 *  @param x Ŀ��DC���Ƴ�ʼx����
		 *  @param y Ŀ��DC���Ƴ�ʼy����
		 *  @param rcClip clip���������dc
		 *  @param lpszText ���Ƶ�����
		 *  @param nSelStart ѡ��������ʼ�ַ�λ�ã��� LOCATION 
		 *  @param nSelEnd��ѡ����������ַ�λ��
		 *  @param colSelText ѡ�����ֵ���ɫ
		 *  @param colSelBk ѡ�����ֵı���ɫ
		 *  @return �ɹ�����TRUE��ʧ���򷵻�FALSE
		 *  @sa 
		 */		
		virtual BOOL DrawAtDc(HDC hDC, int x, int y, const RECT& rcClip, 
			LPCTSTR lpszText, int nSelStart, int nSelEnd, BYTE byAlpha,
			COLORREF colSelText, COLORREF colSelBk) = 0;

		/**
		 *  @brief ��xImage�ϻ����ı�
		 *  @param hDC �ڴ�DC
		 *  @param pxImage xImageָ��
		 *  @param x Ŀ��DC���Ƴ�ʼx����
		 *  @param y Ŀ��DC���Ƴ�ʼy����
		 *  @param rcClip clip���������dc
		 *  @param lpszText ���Ƶ�����
		 *  @param nSelStart ѡ��������ʼ�ַ�λ�ã��� LOCATION 
		 *  @param nSelEnd��ѡ����������ַ�λ��
		 *  @param colSelText ѡ�����ֵ���ɫ
		 *  @param colSelBk ѡ�����ֵı���ɫ
		 *  @return �ɹ�����TRUE��ʧ���򷵻�FALSE
		 *  @sa 
		 */		
		virtual BOOL DrawAtxImage(HDC hMemDC, IxImageLib* pxImage, int x, int y, const RECT& rcClip, 
			LPCTSTR lpszText, int nSelStart, int nSelEnd, BYTE byAlpha,
			COLORREF colSelText, COLORREF colSelBk) = 0;
	};
};

#endif // __NORMALFONT_I_H__