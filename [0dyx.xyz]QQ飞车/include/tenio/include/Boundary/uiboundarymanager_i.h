/********************************************************************
　created         : 2006/6/1
　filename        : boundarymanager_i.h
　author          : season
　version         : 1.0
　description     : 抽象UI节点的父子结构，窗口位置和边界关系管理
					
　history         :
　1 day	          : 2006/6/1
　　author        : season
　　modification: : created	
*********************************************************************/
#ifndef __UIBOUNDARYMANAGER_I_H__
#define __UIBOUNDARYMANAGER_I_H__

#include "../teniointerface.h"

namespace Tenio
{	
	//////////////////////////////////////////////////////////////////////////
	// 其它节点的ID不可等于下面两个已用ID
	#define NODEID_INVALID	(-1)
	#define NODEID_DESKTOP	0

	//////////////////////////////////////////////////////////////////////////
	class IUIBoundaryManager;
	TENIO_MAKE_TYPEID_MAP(IUIBoundaryManager, COMPONENTID_UIBOUNDARYMANAGER)

	class IUIBoundaryManager : public IComponent, public IEventProducer
	{
	public:
		virtual BOOL  InitializeTree() = 0;
		// 以下设置的rc均为相对于父窗口的坐标。
		virtual BOOL  SetRootUINode(DWORD dwRootID, RECT rc) = 0;
		// 在所有子窗口的顶部加入子窗口
		virtual BOOL  AddTopUINode(DWORD dwParentID, DWORD dwUINodeID, RECT rc) = 0;
		// 在所有子窗口的底部加入子窗口
		virtual BOOL  AddBottomUINode(DWORD dwParentID, DWORD dwUINodeID, RECT rc) = 0;
		// 在dwafterid窗口的上部加入子窗口
		virtual BOOL  InsertUINode(DWORD dwParentID, DWORD dwAfterID, DWORD dwUINodeID, RECT rc) = 0;

		//　换一个父结点，所有子孙结点关系保持不变
		virtual BOOL  ChangeUINodeParent(DWORD dwNodeID, DWORD dwNewParent) = 0;
		// 将结点移动到所有兄弟结点的最上层
		virtual BOOL  MoveToTop(DWORD dwNodeID) = 0;
		// 将结点移动到所有兄弟结点的最下层
		virtual BOOL  MoveToBottom(DWORD dwNodeID) = 0;
		// 将结点移动到某个兄弟结点的上层
		virtual BOOL  MoveAfter(DWORD dwNodeID, DWORD dwAfterSiblingNodeID) = 0;
		
		virtual BOOL  DeleteUINode(DWORD dwUINodeID) = 0;
		virtual void  DeleteAllUINode() = 0;

		virtual DWORD GetRootUINode() = 0;
		/**
		 *  @brief 获得该结点的下一个结点（按树的遍历，先父后子再兄）
		 *  @param dwUINodeID 结点ID
		 *  @param bIgnoreSightlessNode 是否忽略不可见结点
		 *  @return 结点dwUINodeID的下一个结点ID
		 *  @sa 
		 */
		virtual DWORD GetNextUINode(DWORD dwUINodeID, BOOL bIgnoreSightlessNode) = 0;
		
		virtual DWORD GetParentUINode(DWORD dwUINodeID) = 0;
		// 得到最上层子窗口
		virtual DWORD GetTopChildUINode(DWORD dwUINodeID) = 0;
		// 得到最下层子窗口
		virtual DWORD GetBottomChildUINode(DWORD dwUINodeID) = 0;
		// 得到该节点的上层兄弟节点
		virtual DWORD GetUpperUINode(DWORD dwUINodeID) = 0;
		// 得到该节点的下层兄弟节点
		virtual DWORD GetNetherUINode(DWORD dwUINodeID) = 0;

		/**
		 *  @brief 判断结点A是否是B的祖宗结点（父结点或祖父结点或祖父的父结点。。。）
		 *  @param dwAncestorID 祖宗结点ID
		 *  @param dwChildID 子孙结点ID
		 *  @return 如果是返回TRUE，不是返回FALSE
		 */		
		virtual BOOL  IsAncestorUINode(DWORD dwAncestorID, DWORD dwChildID) = 0;
		// 该节点是否可见
		virtual BOOL  IsNodeVisible(DWORD dwUINodeID) = 0;
		// 该节点是否处于被show/hide状态
		virtual BOOL  IsNodeShow(DWORD dwUINodeID) = 0;
		// 该节点是否不可操作
		virtual BOOL  IsNodeDisable(DWORD dwUINodeID) = 0;
		// 该节点是否对消息透明
		virtual BOOL  IsNodeTransparent(DWORD dwUINodeID) = 0;
		
		virtual BOOL  ShowUIObject(DWORD dwUINodeID, BOOL bShow) = 0;
		virtual BOOL  DisableUIObject(DWORD dwUINodeID, BOOL bDisable) = 0;
		virtual BOOL  TransparentUIObject(DWORD dwUINodeID, BOOL bTransparent) = 0;
		
		virtual BOOL  MoveUIObject(DWORD dwUINodeID, LPRECT lpRect, BOOL bScreen) = 0;
		virtual BOOL  MoveUIObject(DWORD dwUINodeID, int x, int y, int nWidth, int nHeight, BOOL bScreen) = 0;
		virtual BOOL  OffsetUIObject(DWORD dwUINodeID, int cx, int cy) = 0;
		virtual BOOL  GetUIObjectRect(DWORD dwUINodeID, BOOL bScreen, LPRECT lpRect) = 0;
		// 获得矩形的可见部分，除去被父窗口或祖父窗口遮住的部分，
		//　bClient为true时，rect为客户区坐标;　反之为相对于父窗口坐标
		virtual BOOL  GetUIObjectVisibleRect(DWORD dwUINodeID, BOOL bClient, LPRECT lpVisibleRect) = 0;
		
		// 查询在Rect区域中的所有UI节点ID数组，在内部申请内存，由使用者释放
		virtual int   FindUINodesbyRect(RECT rcScreen, DWORD* pUINodeID) = 0;
		// bIgnoreTransparentNode为true时，不会查找到transparent属性的节点
		virtual DWORD FindUINodebyPoint(POINT ptScreen, BOOL bIgnoreTransparentNode) = 0;
		virtual DWORD FindUINodebyPoint(
			DWORD dwFromID, 
			POINT pt, 
			BOOL bIgnoreTransparentNode) = 0;
		
		// 当UINodeID　为NODEID_DESKTOP时，表示屏幕坐标
		virtual void  MapRect(DWORD dwSrcUINode, DWORD dwDstUINode, LPRECT lpRect) = 0;
		virtual void  MapPoint(DWORD dwSrcUINode, DWORD dwDstUINode, LPPOINT lpPoint) = 0;
	};

	class IUIBoundaryManagerEvent : public IEvent
	{
	public:
		virtual void IsPixelTransparent(DWORD dwNodeID, int x, int y, BOOL* pIsPixelTransparent) = 0;
	};
}


#endif