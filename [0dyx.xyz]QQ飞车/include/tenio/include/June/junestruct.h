#ifndef __JUNESTRUCT_H__
#define __JUNESTRUCT_H__

#include <windows.h>

namespace Tenio
{
	//! Canvas����
	typedef enum enumCANVASFORMATID
	{
		ECI_WINDC	= 1,	 /*!< �ײ�Ϊ Windows DC �� Canvas */ 
		ECI_XIMAGE = 2,      /*!< �ײ�Ϊ xImage�� Canvas */
		ECI_MGDISPLAY		 /*!< MiniGame������*/ 
	}ENUMCANVASFORMATID;

	//! Material������
	typedef enum enumIMAGEFORMATID
	{
		EIF_CXIMAGE	= 1,		/*!< CxImage ��ʽ��ͼƬ */ 
		EIF_NORMALTEXT	= 2,    /*!< ��ͨ���� */ 
		EIF_MGIMAGE,				/*!< MiniGame�����õ�ͼ���ʽ*/ 
		EIF_MIFIMAGE			/*!< mif�ļ��ⲿ��չ���ݶ���(�����治֧��mif��ʱ��,����ʹ�����)*/ 
	}ENUMIMAGEFORMATID;

	//! ��������
	typedef enum enumFONTFORMATID
	{
		EFI_GDI	= 1,		/*!< ʹ��gdi��HFONT���Ƶ����� */ 
		EFI_MINIGAME		/*!< MiniGame������*/ 
	}ENUMFONTFORMATID;

	//! ���ģʽ
	typedef enum enumMIXMODE
	{
		EMM_NORMALPAINT = 1	 /*!< �� Image ���Ƶ� Canvas ��(����Ч)  */ 
	}ENUMMIXMODE;

	//! ���Ʋ���
	struct MIXPARAM
	{
		int nMixMode;	/*!< ȡֵΪ enumMIXMODE */ 
	};

	typedef enum enumNORMALPAINTMIXMODE
	{
		ENM_STRETCH = 1,    /*!< ���� */
		ENM_TILE           /*!< ƽ�� */
	}ENUMNORMALPAINTMIXMODE;

	//! ͼ����Ʋ���, �� Image ��(nImageX,nImageY)��ʼ�ľ��λ��Ƶ� Canvas �� (nCanvasX,nCanvasY)����
	struct NORMALPAINTMIXPARAM : public MIXPARAM
	{
		ENUMNORMALPAINTMIXMODE ePaintMode; /*!< ���Ʒ�ʽ */
		int nCanvasX;	/*!< ���Ƶ�CanvasĿ�������xֵ */ 
		int nCanvasY;	/*!< ���Ƶ�CanvasĿ�������yֵ */
		int nImageX;	/*!< ͼ�������ʼx���� */
		int nImageY;    /*!< ͼ�������ʼy���� */
		int nWidth;	    /*!< ���Ƶľ��ο��, -1 ��ʾȡͼƬ��Ĭ�Ͽ�� */
		int nHeight;	/*!< ���Ƶľ��θ߶�, -1 ��ʾȡͼƬ��Ĭ�ϸ߶� */
		unsigned char byImageAlphaSet; /*!< ͼƬ��͸���� 255 ��ʾ��͸�� */
		int nDrawFrame;	/*!< ��ǰ����֡������*/
	};

	//! ͼ����Ʋ�������Imageƽ�̵�canvas��
	struct STRETCHPAINTMIXPARAM : public NORMALPAINTMIXPARAM
	{
		int nImageWidth;
		int nImageHeight;
	};

	//! �ַ����Ʋ���
	struct NORMAL_TEXT_MIXPARAM : public MIXPARAM
	{
		int iCanvasX;		    /*!< ���Ƶ�CanvasĿ�������xֵ */
		int iCanvasY;		    /*!< ���Ƶ�CanvasĿ�������yֵ */
		RECT rcClip;            /*!< �ı���ʾ�ļ��о��� */
		LOGFONT LogFont;        /*!< ������Ϣ */
		COLORREF colText;       /*!< �ı���ɫ */
		COLORREF colSelectText; /*!< ѡ���ı�����ɫ */
		COLORREF colSelectBk;   /*!< ѡ���ı��ı�����ɫ */
		UINT fAlignMode;        /*!< �ı����뷽ʽ */
		BOOL bAutoChangeLine;   /*!< �Ƿ��Զ����� */
		BYTE byTextAlphaSet;    /*!< �����͸���� 255 ��ʾ��͸�� */
		int nLineSpacing;       /*!< �о� */ 
	};
};

#endif