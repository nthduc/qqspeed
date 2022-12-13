#ifndef __JUNEFONT_I_H__
#define __JUNEFONT_I_H__

#include <windows.h>

namespace Tenio
{
	/**
	 * @ingroup tenuilib
	 * @brief 字体对象抽象基类
	 *
	 * <p>
	 *　　 所有字体对象的基类，各种引擎中的字体对象必须继承它并实现它的方法
	 * </p>
	 *
	 * @sa 
	 */	
	class IFont
	{
	public:
		/**
		 *  @brief 获得字体格式ID
		 *  @return 返回字体格式 值为 enumFONTFORMATID 类型
		 *  @sa enumFONTFORMATID
		 */	
		virtual int GetFormatId() = 0;

		/**
		 *  @brief 获得字符高度
		 *  @return 返回字符高度
		 *  @sa 
		 */	
		virtual int GetCharHeight() = 0;

		/**
		 *  @brief 获得某个字符宽度
		 *  @param uChar 字符
		 *  @return 字符uChar的宽度
		 *  @sa 
		 */	
		virtual int GetCharWidth(UINT uChar) = 0;

		/**
		 *  @brief 获得单行字符串的宽度
		 *  @param lpszText 字符串指针
		 *  @param nLen　字符串字符个数
		 *  @return 返回字符串宽度
		 *  @sa 
		 */
		virtual int GetTextWidth(LPCTSTR lpszText, int nLen) = 0;
		
		/**
		 *  @brief 获得字体信息结构的指针
		 *  @return 返回字体信息结构
		 *  @sa 
		 */		
		virtual const LOGFONT& GetLogFont() = 0;
	};
};

#endif // __JUNEFONT_I_H__