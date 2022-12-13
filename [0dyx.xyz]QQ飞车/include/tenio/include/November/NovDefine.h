#ifndef __NOVDEFINE_H__
#define __NOVDEFINE_H__

namespace Tenio
{
	//! CreateImage时用的参数
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
	
	//! 绘制时用的参数
	typedef struct _DRAW_PARA
	{
		int srcx;	//再将一张图的一部分画到屏幕上的时候，填充这4个参数即可
		int srcy;
		int cx;
		int cy;
		//
		bool	bChangeCenterPt;	// 是否改变中心点坐标    //在旋转时填充以下4个
		int		nkx;			// 新中心点坐标 			//旋转的中心点，默认（0, 0）
		int		nky;
		float	fRotate;			// 旋转, >0.01的有效，角度值
		//
		float	fxScale;			// 放缩，>0.01的有效
		float	fyScale;
		//
		UINT	unSSEffect;		// ------- TextureStageState特效默认方式: eSSEffectNULL
								// eSSEffectNULL	    	// 特效无
								// eSSEffectAlphaSet    	// 设定某个Alpha值
								// eSSEffectMul			// 颜色、alpha 都乘
								// eSSEffectAdd			// 颜色、alpha 都加
		DWORD	dwTextureFactor;  // unSSEffect不为eSSEffectNULL时有效
		UINT	unRSEffect;		// ------- Render特效默认方式: eRSEffectAlphaBlit 
								// eRSEffectAlphaBlit      // Blit时  使用alpha
								// eRSEffectBlit		    // Blit时不使用alpha
								// eRSEffectAlphaAdd	    // 源色×Alpha + 目标色
								// eRSEffectAdd		    // 源色 + 目标色				
		_DRAW_PARA()
		{
			memset(this, 0, sizeof(_DRAW_PARA));
		}
	}DRAW_PARA, *PDRAW_PARA;      
	


}

#endif