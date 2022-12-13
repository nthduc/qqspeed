#ifndef __FONTFACTORY_I_H__
#define __FONTFACTORY_I_H__

#include "../teniointerface.h"
#include "font_i.h"
#include "junestruct.h"

namespace Tenio
{
	class IFontFactory;

	TENIO_MAKE_TYPEID_MAP(IFontFactory, COMPONENTID_FONTFACTORY);

	/**
	 * @ingroup tenuilib
	 * @brief 字体工厂
	 *
	 * <p>
	 *　　 负责创建字体对象，字体对象对工厂统一销毁，外部不需要销毁
	 * </p>
	 *
	 * @sa 
	 */	
	class IFontFactory : public IComponent
	{
	public:
		/**
		 *  @brief 获得字体对象，相同字体信息返回同一个字体对象指针
		 *  @param LogFont 字体详细信息，LOGFONT是windows结构
		 *  @return 返回字体对象指针
		 *  @sa 
		 */		
		virtual IFont* GetFont(const LOGFONT& LogFont) = 0;
	};
};

#endif // __FONTFACTORY_I_H__

// InterfaceName:FontFactory
// SubFolderName:June