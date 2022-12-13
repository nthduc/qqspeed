#ifndef __NORMALTEXT_I_H__
#define __NORMALTEXT_I_H__

#include "../teniointerface.h"
#include "text_i.h"

namespace Tenio
{
	class INormalText;

	TENIO_MAKE_TYPEID_MAP(INormalText, COMPONENTID_NORMALTEXT);

	/**
	 * @ingroup tenuilib
	 * @brief 普通文本元素
	 *
	 * <p>
	 *　　 即类型为 EIF_NORMALTEXT 的绘制元素
	 * </p>
	 *
	 * @sa enumIMAGEFORMATID
	 */	
	class INormalText : public IText, public IComponent
	{
	};
};

#endif // __NORMALTEXT_I_H__