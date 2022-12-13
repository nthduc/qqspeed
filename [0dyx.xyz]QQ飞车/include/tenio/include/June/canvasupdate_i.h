#ifndef __JUNECAVASUPDATE_I_H__
#define __JUNECAVASUPDATE_I_H__

#include "region_i.h"

namespace Tenio
{
	/**
	 * @ingroup tenuilib
	 * @brief Canvas更新回调接口，由用户实现
	 *
	 * <p>
	 *　　 
	 * </p>
	 *
	 * @sa 
	 */
	class IJuneCanvasUpdate
	{
	public:
		/**
		 *  @brief 通知Canvas需要更新
		 *  @param pRegion 通知Canvas需要更新的区域。
		 *  @sa 
		 */	
		virtual void OnJuneCanvasUpdate(IJuneRegion* pRegion) = 0; 
	};
};

#endif