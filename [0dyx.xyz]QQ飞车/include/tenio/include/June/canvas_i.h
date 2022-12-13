#ifndef __JUNECANVAS_I_H__
#define __JUNECANVAS_I_H__

#include "region_i.h"
#include "canvasdefine.h"

namespace Tenio
{
	/**
	 * @ingroup tenuilib
	 * @brief 画布
	 *
	 * <p>
	 *　　 ICanvas接口是所有类型Canvas模块的基类。\n
	 *	Canvas模块提供图像和文字绘制的目的地，同时也实现几种常用的图形绘制方法。
	 * </p>
	 *
	 * @sa 
	 */	
	class IJuneCanvas
	{
	public:
		/**
		 *  @brief 获最Canvas的格式ID
		 *  @return 返回值参见 enumCANVASFORMATID 
		 *  @sa 
		 */		
		virtual int GetCanvasFormatId() = 0;

		/**
		 *  @brief 用纯色填充区域
		 *  @param rc 填充矩形区域
		 *  @param cr 填充的颜色值
		 *  @sa 
		 */	
		virtual void FillRect(const RECT& rc, COLORREF cr) = 0;

		/**
		 *  @brief 使用带透明值的纯色填充区域
		 *  @param rc 填充矩形区域
		 *  @param cr 填充的颜色值
		 *  @param byAlpha alpha值，取值范围是0~255
		 *  @return 
		 *  @sa 
		 */	
		virtual void FillRect(const RECT& rc, COLORREF cr, BYTE byAlpha) = 0;

		/**
		 *  @brief 绘制矩形区域边框
		 *  @param rc 区域矩形
		 *  @param cr 边框颜色值
		 *  @return 
		 *  @sa 
		 */	
		virtual void FrameRect(const RECT& rc, COLORREF cr) = 0;

		/**
		 *  @brief 绘制矩形区域边框
		 *  @param rc 区域矩形
		 *  @param pi 画笔信息，见 tagPenInfo 
		 *  @return 
		 *  @sa 
		 */	
		virtual void FrameRectIndirect(const RECT& rc, PENINFO& pi) = 0;

		/**
		 *  @brief 绘制拉杆，用半透明区域填充
		 *  @param rc 移动前矩形区域
		 *  @param size 边框宽度
		 *  @param rc 移动后矩形区域
		 *  @param size 移动后边框厚度
		 *  @return 
		 *  @sa 
		 */	
		virtual void DragHalftoneRect(
			const RECT& rc, 
			SIZE size, 
			const RECT& rcLast, 
			SIZE sizeLast) = 0;

		/**
		 *  @brief 画直线
		 *  @param ptBegin 起始位置
		 *  @param ptEnd 结束位置
		 *  @param cr 颜色值
		 *  @return 
		 *  @sa 
		 */	
		virtual void Line(const POINT& ptBegin, const POINT& ptEnd, COLORREF cr) = 0;	

		/**
		 *  @brief 画直线
		 *  @param ptBegin 起始位置
		 *  @param ptEnd 结束位置
		 *  @param pi 画笔信息，见 tagPenInfo 
		 *  @return 
		 *  @sa 
		 */	
		virtual void LineIndirect(const POINT& ptBegin, const POINT& ptEnd, PENINFO& pi) = 0;
		
		/**
		 *  @brief 获得Canvas的更新区域
		 *  @return Canvas的更新区域对象指针
		 *  @sa 
		 */		
		virtual IJuneRegion* GetUpdateRegion() = 0;
	};
};

#endif