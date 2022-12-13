#ifndef __LAYER_I_H__
#define __LAYER_I_H__

#include "../june/canvas_i.h"
#include "sceneupdate_i.h"

namespace Tenio
{
	class IJuneLayer;
	class IJuneSceneUpdate;
	class IJuneLayer
	{
	public:
		virtual BOOL DrawAt(IJuneCanvas* pCanvas, int x, int y) = 0;

		virtual int GetWidth() = 0;

		virtual int GetHeight() = 0;

		// 检查(x,y)是否在这个层上, (x,y)是相对与本层左上角的坐标
		virtual BOOL HitTest(int x, int y) = 0;

		// 获取层的总Alpha值(0-255)
		virtual BYTE GetPaintAlpha() = 0;

		// 设置Alpha值
		virtual void SetPaintAlpha(BYTE byValue) = 0;

		// 获得clip region, region坐标以本层左上角为原点
		virtual IJuneRegion* GetClipRegion() = 0;

		//重设置其子层的clip region
		virtual void OnSetClipRegion() = 0;

		// 该层是否可见
		virtual BOOL IsVisible() = 0;

		// 设置为不可见，既无论该层是否被show都不会绘制出来。
		virtual void SetVisible(BOOL bVisible) = 0;

		// 显示或隐藏该层
		virtual void Show(BOOL bShow) = 0;

		/**
		 *  @brief 判断某点是否透明
		 *  @param x x轴坐标
		 *  @param y y轴坐标
		 *  @return 如果透明则返回TRUE，不透明则返回FALSE
		 */		
		virtual BOOL IsPixelTransparent(int x, int y) = 0;

		virtual void SetSceneUpdate(IJuneSceneUpdate* pSceneUpdate) = 0;
	};
};

#endif