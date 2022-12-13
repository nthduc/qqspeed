#ifndef __MATERIAL_I_H__
#define __MATERIAL_I_H__

namespace Tenio
{
	/**
	 * @ingroup tenuilib
	 * @brief 所有绘制对象公共接口
	 *
	 * <p>
	 *　　 
	 * </p>
	 *
	 * @sa 
	 */
	
	class IMaterial
	{
	public:
		/**
		 *  @brief 获得格式ID
		 *  @return 返回格式ID，取值为 enumIMAGEFORMATID 
		 *  @sa 
		 */
		virtual int GetFormatId() = 0;
	};
};

#endif // __MATERIAL_I_H__