#ifndef __JUNEIMAGE_I_H__
#define __JUNEIMAGE_I_H__

#include "material_i.h"

namespace Tenio
{
	/**
	 * @ingroup tenuilib
	 * @brief 图像对象公共接口
	 *
	 * <p>
	 *　　 
	 * </p>
	 *
	 * @sa 
	 */	
	class IImage : public IMaterial
	{
	public:
		/**
		 *  @brief 获得图像宽度
		 *  @return 返回图像宽度
		 *  @sa 
		 */	
		virtual int GetWidth() = 0;

		/**
		 *  @brief 获得图像高度
		 *  @return 返回图像高度
		 *  @sa 
		 */	
		virtual int GetHeight() = 0;

		/**
		 *  @brief 是否有alpha通道
		 *  @return 如果有alpha通道返回TRUE，反之返回FALSE
		 *  @nFrame 帧序列号　
		 *  @sa 
		 */		
		virtual BOOL HaveAlphaChannels(UINT nFrame = 0) = 0;

		/**
		 *  @brief 获得某像素的颜色值
		 *  @param x 像素x坐标
		 *  @param y 像素y坐标
		 *	@param bGetAlpha 是否获得alpha通道的值
		 *  @nFrame 帧序列号　
		 *  @return 返回颜色信息,  RGBQUAD 为windows结构
		 *  @sa 
		 */
		virtual RGBQUAD GetPixelColor(int x, int y, BOOL bGetAlpha, UINT nFrame = 0) = 0;

		/**
		 *  @brief 获得得帧数
		 *  @return 返回图像包含的帧数
		 */		
		virtual UINT GetFrameNum() = 0;

		/**
		 *  @brief 获得得帧延迟
		 *  @param nFrame 帧索引值(0开始)
		 *  @return 返回帧延迟，如果索引值大于等于帧数，返回0
		 */	
		virtual UINT GetFrameDelay(UINT nFrame) = 0;
		
		/**
		 *  @brief 设置透明色
		 *  @param crTransparent 表示透明的颜色值
		 */		
		virtual void SetTransColor(COLORREF crTransparent) = 0;

		/**
		 *  @brief 判断某点是否透明
		 *  @param x x轴坐标
		 *  @param y y轴坐标
		 *  @nFrame 帧序列号　
		 *  @return 如果透明则返回TRUE，不透明则返回FALSE
		 */		
		virtual BOOL IsPixelTransparent(int x, int y, UINT nFrame = 0) = 0;
	};
};

#endif