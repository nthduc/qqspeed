#ifndef __NORMALTEXTMIXER_I_H__
#define __NORMALTEXTMIXER_I_H__

#include "../teniointerface.h"
#include "mixer_i.h"

namespace Tenio
{
	class INormalTextMixer;

	TENIO_MAKE_TYPEID_MAP(INormalTextMixer, COMPONENTID_NORMALTEXTMIXER);

	/**
	 * @ingroup tenuilib
	 * @brief 普通文本元素混和器
	 *
	 * <p>
	 *　　 处理在dc上的使用HFONT绘制文本。
	 * </p>
	 *
	 * @sa 
	 */	
	class INormalTextMixer : public IJuneMixer, public IComponent
	{
	public:
	};
};

#endif

// InterfaceName:NormalTextImageMixer
// SubFolderName:June