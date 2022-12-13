#ifndef __NOVDEFINE_H__
#define __NOVDEFINE_H__

namespace Tenio
{
	//! CreateImageʱ�õĲ���
	typedef enum enmSelectType
	{
		EST_GRAY		= 0x01 << 0,
		EST_MIRRORX		= 0x01 << 1,
		EST_MIRRORY		= 0x01 << 2
	} ENMSELECTTYPE;
	
	typedef struct tagSelectParam
	{
		ENMSELECTTYPE eSelectType;
		int kx;
		int ky;
	} SELECTPARAM, *PSELECTPARAM;

	typedef enum enmCreateFlag
	{
		ECF_SELECTRELEASE		= 0,
		ECF_SELECTNORELEASE		,
		ECF_NOSELECT
	} ENMCREATEFLAG;
	
	//! ����ʱ�õĲ���
	typedef struct _DRAW_PARA
	{
		int srcx;	//�ٽ�һ��ͼ��һ���ֻ�����Ļ�ϵ�ʱ�������4����������
		int srcy;
		int cx;
		int cy;
		//
		bool	bChangeCenterPt;	// �Ƿ�ı����ĵ�����    //����תʱ�������4��
		int		nkx;			// �����ĵ����� 			//��ת�����ĵ㣬Ĭ�ϣ�0, 0��
		int		nky;
		float	fRotate;			// ��ת, >0.01����Ч���Ƕ�ֵ
		//
		float	fxScale;			// ������>0.01����Ч
		float	fyScale;
		//
		UINT	unSSEffect;		// ------- TextureStageState��ЧĬ�Ϸ�ʽ: eSSEffectNULL
								// eSSEffectNULL	    	// ��Ч��
								// eSSEffectAlphaSet    	// �趨ĳ��Alphaֵ
								// eSSEffectMul			// ��ɫ��alpha ����
								// eSSEffectAdd			// ��ɫ��alpha ����
		DWORD	dwTextureFactor;  // unSSEffect��ΪeSSEffectNULLʱ��Ч
		UINT	unRSEffect;		// ------- Render��ЧĬ�Ϸ�ʽ: eRSEffectAlphaBlit 
								// eRSEffectAlphaBlit      // Blitʱ  ʹ��alpha
								// eRSEffectBlit		    // Blitʱ��ʹ��alpha
								// eRSEffectAlphaAdd	    // Դɫ��Alpha + Ŀ��ɫ
								// eRSEffectAdd		    // Դɫ + Ŀ��ɫ				
		_DRAW_PARA()
		{
			memset(this, 0, sizeof(_DRAW_PARA));
		}
	}DRAW_PARA, *PDRAW_PARA;      
	


}

#endif