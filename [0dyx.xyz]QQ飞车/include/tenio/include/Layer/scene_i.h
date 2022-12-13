#ifndef __SCENE_I_H__
#define __SCENE_I_H__

#include "layer_i.h"
#include "june/canvas_i.h"

namespace Tenio
{
	typedef enum enmPlacePos
	{
		EPP_TOP		= 0,	//放置在所有非topmost层的上面
		EPP_TOPMOST,		//放置在最顶层，并带有topmost属性
		EPP_BOTTOM,			//放置在所有非bottommost层的下面
		EPP_BOTTOMMOST		//放置在最底层，并带有bottommost属性
	} ENMPLACEPOS;

	class IJuneScene
	{
	public:
		virtual int GetSceneID() = 0;

		// 在顶层放置一层, 坐标在 (x,y). 
		virtual BOOL PlaceAtTop(IJuneLayer* pLayer, int x, int y) = 0;

		virtual BOOL PlaceAt(IJuneLayer* pLayer, int x, int y, ENMPLACEPOS ePos) = 0;

		// 把 Scene 绘制在Canvas的x,y上, Canvas内的UpdageRegion区域才绘制
		virtual BOOL DrawAtCanvas(IJuneCanvas* pCanvas, int x, int y) = 0;

		// 获取层的位置
		virtual BOOL GetPosition(IJuneLayer* pLayer, int* pnx, int* pny) = 0;

		/**
		 *  @brief 移动层的位置
		 *  @param pLayer 层指针
		 *  @param x 新的x位置，相对于scene
		 *  @param y 新的y位置
		 *  @param bUpdateOrg　是否把原区域放入更新区域列表
		 *  @param bUpdateDst　是否把放置后的区域放入更新区域列表
		 *  @return 成功则返回TRUE.
		 *  @sa 
		 */	
		virtual BOOL MoveLayerTo(IJuneLayer* pLayer, int x, int y, 
			BOOL bUpdateOrg = TRUE, BOOL bUpdateDst = TRUE) = 0;

		// 删除层
		virtual void RemoveLayer(IJuneLayer* pLayer) = 0;

		// 获取指定位置上的层
		virtual IJuneLayer* GetLayerAt(int x, int y) = 0;

		// 移动层到最上层
		virtual BOOL MoveLayerToTop(IJuneLayer* pLayer) = 0;

		// 移动层到最下层
		virtual BOOL MoveLayerToBottom(IJuneLayer* pLayer) = 0;

		// 在pLayer之后插入层
		virtual BOOL InsertLayerAfter(IJuneLayer* pLayer, IJuneLayer* pLayerInsert, int x, int y) = 0;
		// 在pLayer之前插入层
		virtual BOOL InsertLayerBefore(IJuneLayer* pLayer, IJuneLayer* pLayerInsert, int x, int y) = 0;		

		/**
		 *  @brief 设置是否记录更新区域
		 *  @param bCollect　是否记录
		 */	
		virtual void CollectUpdateArea(BOOL bCollect) = 0;

//		/**
//		 *  @brief 设置scene上重画的起始层
//		 *  @param pLayer 起始层指针，当pLayer为NULL时，从最底层开始绘制
//		 */
//		virtual void SetReDrawStartLayer(IJuneLayer* pLayer) = 0;
//
//		/**
//		 *  @brief 显示某层，只管设置重绘，不更改layer属性
//		 *  @param pLayer 需要被显示层的指针
//		 */	
//		virtual void ShowLayer(IJuneLayer* pLayer) = 0;
//
//		/**
//		 *  @brief 隐藏某层，只管设置重绘，不更改layer属性
//		 *  @param pLayer 需要被隐藏层的指针
//		 */
//		virtual void HideLayer(IJuneLayer* pLayer) = 0;
//
//		/**
//		 *  @brief 重绘某层
//		 *  @param pLayer 需要重绘的层
//		 */	
//		virtual void RedrawLayer(IJuneLayer* pLayer) = 0;
	};
};

#endif