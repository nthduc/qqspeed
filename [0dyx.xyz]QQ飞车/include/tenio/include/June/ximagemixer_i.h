#ifndef __XIMAGEMIXER_I_H__
#define __XIMAGEMIXER_I_H__

#include "mixer_i.h"
#include "../teniointerface.h"

namespace Tenio
{
	class IxImageMixer;

	TENIO_MAKE_TYPEID_MAP(IxImageMixer, COMPONENTID_CXIMAGEMIXER);

	/**
	 * @ingroup tenuilib
	 * @brief 绘制元素类型为 EIF_CXIMAGE 的混和器
	 *
	 * <p>
	 *　　 
	 * </p>
	 *
	 * @sa 
	 */	
	class IxImageMixer : public IJuneMixer, public IComponent
	{
	public:
		
	};
};

#endif