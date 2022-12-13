#ifndef __MIXER_I_H__
#define __MIXER_I_H__

#include "canvas_i.h"
#include "material_i.h"
#include "normaltext_i.h"
#include "junestruct.h"

namespace Tenio
{
	/**
	 * @ingroup tenuilib
	 * @brief 混和器公共接口
	 *
	 * <p>
	 *　　 
	 * </p>
	 *
	 * @sa 
	 */
	class IJuneMixer
	{
	public:
		/**
		 *  @brief 将被绘制对象绘制到画布上
		 *  @param pCanvas 目的画布
		 *  @param pMaterial 绘制对象
		 *  @param pMixParam 见 MIXPARAM , NORMALPAINTMIXPARAM , NORMAL_TEXT_MIXPARAM 
		 *  @return 成功则返回TRUE，失败则返回FALSE
		 *  @sa 
		 */	
		virtual BOOL Mix(IJuneCanvas* pCanvas, IMaterial* pMaterial, MIXPARAM* pMixParam) = 0;
	};
};

#endif