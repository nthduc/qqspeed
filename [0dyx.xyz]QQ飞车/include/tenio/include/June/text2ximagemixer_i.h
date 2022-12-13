#ifndef __TEXT2XIMAGEMIXER_I_H__
#define __TEXT2XIMAGEMIXER_I_H__

#include "../teniointerface.h"
#include "mixer_i.h"

namespace Tenio
{
	class IText2xImageMixer;
	TENIO_MAKE_TYPEID_MAP(IText2xImageMixer, COMPONENTID_TEXT2XIMAGEMIXER);

	/**
	 * @ingroup tenuilib
	 * @brief 将文字混合到xImage的混和器
	 *
	 * <p>
	 *　　 处理在xImage上使用HFONT绘制文本。
	 * </p>
	 *
	 * @sa 
	 */	
	class IText2xImageMixer : public IJuneMixer, public IComponent
	{
	public:
	};
}; // Tenio

#endif // __TEXT2XIMAGEMIXER_I_H__
