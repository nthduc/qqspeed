#ifndef __IMAGEBOX_I_H__
#define __IMAGEBOX_I_H__

#include "../teniointerface.h"
#include "layer_i.h"
#include "../uilib/variableparam_i.h"

namespace Tenio
{
	class IJuneImageBox;

	TENIO_MAKE_TYPEID_MAP(IJuneImageBox, COMPONENTID_JUNEIMAGEBOX);

	typedef enum _tagIMAGEPAINTMODE
	{
		IPM_NORMAL = 1,
		IPM_STRETCH,
		IPM_TILE
	}IMAGEPAINTMODE;

	typedef enum _tagIMAGEOVERTURNMODE
	{
		IOM_NONE = 1,
		IOM_HORI,
		IOM_VERT,
		IOM_BOTH
	}IMAGEOVERTURNMODE;

	class IJuneImageBox : public IJuneLayer, public IComponent
	{
	public:
		// ���ļ��ж���ͼƬ, ֧�� bmp jpg tga gif
		virtual BOOL LoadFromFile(LPCTSTR lpszFile) = 0;
		virtual BOOL LoadFromArray(
			BYTE* pArray,
			DWORD dwWidth,
			DWORD dwHeight,
			DWORD dwBitsPerPixel, 
			DWORD dwBytesPerLine, 
			BOOL bFlipImage) = 0;
		
		virtual void SetPaintMode(IMAGEPAINTMODE PaintMode, SIZE sz) = 0;
		
		/* ���ͼ��ɼ������rgn��
		 * �����alphaͨ������ȥ��alphaֵΪ��Ĳ���
		 * ���û�У���ȥ������ֵΪcrMask�Ĳ���,crMask��property����.
		 * ����ֵ�����������ڲ������ⲿ����Ҫ�ͷ�
		 */
		virtual HRGN GetImageRgn() = 0;

		virtual BOOL SetProperty(IVariableParam* pParam) = 0;

		/**
		 *  @brief ��õõ�ǰ����֡
		 *  @return ���ص�ǰ�����Ƶ�֡������ֵ
		 */	
		virtual int GetCurrentFrame() = 0;

		/**
		 *  @brief ���õ�ǰ����֡���������ֵ������֡����������������ã����ƵĶ��ǵ�һ֡��
		 *  @param nNewCurFrame �µĵ�ǰ����֡
		 *  @return ���ؾɵĵ�ǰ����֡
		 */		
		virtual int SetCurrentFrame(int nNewCurFrame) = 0;

		/**
		 *  @brief ��õñ�ͼ����֡��
		 *  @return ���ر�ͼ����֡��
		 */		
		virtual int GetFrameNum() = 0;

		/**
		 *  @brief ���ĳ֡�ӳ�
		 *  @param nFrame��ĳ֡���к�
		 *  @return ���ظ�֡�ӳ�
		 */		
		virtual int GetFrameDelay(int nFrame) = 0;

		/**
		 *  @brief ����֡�ӳ���Ϣ�õ�֡���кţ����ӳ��������⺬��ʱʹ��
		 *  @param nDelay �ӳ���Ϣ
		 *  @return ����֡���кţ���û���ҵ�ʱ����-1
		 */		
		virtual int GetFrameIndexbyDelayInfo(UINT nDelay) = 0;

		virtual void StartAnimate() = 0;
		
		virtual void EndAnimate() = 0;
	};
};

#endif