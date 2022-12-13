#ifndef __NORMALREGION_I_H__
#define __NORMALREGION_I_H__

#include "region_i.h"

namespace Tenio
{
	class INormalRegion;
	TENIO_MAKE_TYPEID_MAP(INormalRegion, COMPONENTID_NORMALREGION);

	/**
	 * @ingroup tenuilib
	 * @brief 普通区域管理对象实现
	 *
	 * <p>
	 *　　 内部由矩形块表示待更新区域
	 * </p>
	 *
	 * @sa 
	 */
	class INormalRegion : public IJuneRegion, public IComponent
	{
	public:
	};
};

#endif