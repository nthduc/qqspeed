#ifndef __TEXTFACTORY_I_H__
#define __TEXTFACTORY_I_H__

#include "../teniointerface.h"
#include "text_i.h"
#include "junestruct.h"

namespace Tenio
{
	class ITextFactory;

	TENIO_MAKE_TYPEID_MAP(ITextFactory, COMPONENTID_TEXTFACTORY);

	/**
	 * @ingroup tenuilib
	 * @brief 文字元素工厂
	 * <p>
	 *　　 创建文字元素的工厂	 
	 * </p>
	 *
	 * @sa 
	 */
	class ITextFactory : public IComponent
	{
	public:
		/**
		 *  @brief 根据文字类型创建文字元素
		 *  @param eId 文字类型，见 enumIMAGEFORMATID 
		 *  @return 返回文字对象指针
		 *  @sa 
		 */	
		virtual IText* CreateInstance(ENUMIMAGEFORMATID eId) = 0;

		/**
		 *  @brief 释放文字对象
		 *  @param 被释放的文字对象指针
		 *  @sa 
		 */
		virtual void Release(IText* pText) = 0;
	};
};

#endif // __TEXTFACTORY_H__

// InterfaceName:NormalTextImageFactory
// SubFolderName:June