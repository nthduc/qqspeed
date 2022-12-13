#ifndef __JUNEIMAGE_I_H__
#define __JUNEIMAGE_I_H__

#include "material_i.h"

namespace Tenio
{
	/**
	 * @ingroup tenuilib
	 * @brief ͼ����󹫹��ӿ�
	 *
	 * <p>
	 *���� 
	 * </p>
	 *
	 * @sa 
	 */	
	class IImage : public IMaterial
	{
	public:
		/**
		 *  @brief ���ͼ����
		 *  @return ����ͼ����
		 *  @sa 
		 */	
		virtual int GetWidth() = 0;

		/**
		 *  @brief ���ͼ��߶�
		 *  @return ����ͼ��߶�
		 *  @sa 
		 */	
		virtual int GetHeight() = 0;

		/**
		 *  @brief �Ƿ���alphaͨ��
		 *  @return �����alphaͨ������TRUE����֮����FALSE
		 *  @nFrame ֡���кš�
		 *  @sa 
		 */		
		virtual BOOL HaveAlphaChannels(UINT nFrame = 0) = 0;

		/**
		 *  @brief ���ĳ���ص���ɫֵ
		 *  @param x ����x����
		 *  @param y ����y����
		 *	@param bGetAlpha �Ƿ���alphaͨ����ֵ
		 *  @nFrame ֡���кš�
		 *  @return ������ɫ��Ϣ,  RGBQUAD Ϊwindows�ṹ
		 *  @sa 
		 */
		virtual RGBQUAD GetPixelColor(int x, int y, BOOL bGetAlpha, UINT nFrame = 0) = 0;

		/**
		 *  @brief ��õ�֡��
		 *  @return ����ͼ�������֡��
		 */		
		virtual UINT GetFrameNum() = 0;

		/**
		 *  @brief ��õ�֡�ӳ�
		 *  @param nFrame ֡����ֵ(0��ʼ)
		 *  @return ����֡�ӳ٣��������ֵ���ڵ���֡��������0
		 */	
		virtual UINT GetFrameDelay(UINT nFrame) = 0;
		
		/**
		 *  @brief ����͸��ɫ
		 *  @param crTransparent ��ʾ͸������ɫֵ
		 */		
		virtual void SetTransColor(COLORREF crTransparent) = 0;

		/**
		 *  @brief �ж�ĳ���Ƿ�͸��
		 *  @param x x������
		 *  @param y y������
		 *  @nFrame ֡���кš�
		 *  @return ���͸���򷵻�TRUE����͸���򷵻�FALSE
		 */		
		virtual BOOL IsPixelTransparent(int x, int y, UINT nFrame = 0) = 0;
	};
};

#endif