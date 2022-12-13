#ifndef __IDIBIMAGE_I_H__
#define __IDIBIMAGE_I_H__

#include "../June/image_i.h"

namespace Tenio
{
	class IDIBImage : public IImage, public IComponent
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
		 *  @brief ������豸�޹�λͼ���
		 *  @return ����DIBHandle
		 */	
		virtual HBITMAP GetDIBHandle(int nIndex) = 0;

		/**
		 *  @brief �����DIB buffer, 
		 *  @return ����ָ��
		 */	
		virtual void* GetDIB(int nIndex) = 0;
	};
};

#endif

// InterfaceName:IMifImage
// SubFolderName:MGClock
