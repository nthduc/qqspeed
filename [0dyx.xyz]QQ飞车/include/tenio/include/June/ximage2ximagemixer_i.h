#ifndef __XIMAGE2XIMAGEMIXER_I_H__
#define __XIMAGE2XIMAGEMIXER_I_H__

#include "mixer_i.h"
#include "../teniointerface.h"

namespace Tenio
{
	class IxImage2xImageMixer;

	TENIO_MAKE_TYPEID_MAP(IxImage2xImageMixer, COMPONENTID_XIMAGE2XIMAGEMIXER);

	/**
	 * @ingroup tenuilib
	 * @brief 将xImage材料绘制到xImage画布的混合器
	 *
	 * <p>
	 *　　 
	 * </p>
	 *
	 * @sa 
	 */	

	class IxImage2xImageMixer : public IJuneMixer, public IComponent
	{
	public:
	};
}; // Tenio

#endif // __XIMAGE2XIMAGEMIXER_I_H__
