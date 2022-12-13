/********************************************************************
��created         : 2006/6/1
��filename        : boundarymanager_i.h
��author          : season
��version         : 1.0
��description     : ����UI�ڵ�ĸ��ӽṹ������λ�úͱ߽��ϵ����
					
��history         :
��1 day	          : 2006/6/1
����author        : season
����modification: : created	
*********************************************************************/
#ifndef __UIBOUNDARYMANAGER_I_H__
#define __UIBOUNDARYMANAGER_I_H__

#include "../teniointerface.h"

namespace Tenio
{	
	//////////////////////////////////////////////////////////////////////////
	// �����ڵ��ID���ɵ���������������ID
	#define NODEID_INVALID	(-1)
	#define NODEID_DESKTOP	0

	//////////////////////////////////////////////////////////////////////////
	class IUIBoundaryManager;
	TENIO_MAKE_TYPEID_MAP(IUIBoundaryManager, COMPONENTID_UIBOUNDARYMANAGER)

	class IUIBoundaryManager : public IComponent, public IEventProducer
	{
	public:
		virtual BOOL  InitializeTree() = 0;
		// �������õ�rc��Ϊ����ڸ����ڵ����ꡣ
		virtual BOOL  SetRootUINode(DWORD dwRootID, RECT rc) = 0;
		// �������Ӵ��ڵĶ��������Ӵ���
		virtual BOOL  AddTopUINode(DWORD dwParentID, DWORD dwUINodeID, RECT rc) = 0;
		// �������Ӵ��ڵĵײ������Ӵ���
		virtual BOOL  AddBottomUINode(DWORD dwParentID, DWORD dwUINodeID, RECT rc) = 0;
		// ��dwafterid���ڵ��ϲ������Ӵ���
		virtual BOOL  InsertUINode(DWORD dwParentID, DWORD dwAfterID, DWORD dwUINodeID, RECT rc) = 0;

		//����һ������㣬�����������ϵ���ֲ���
		virtual BOOL  ChangeUINodeParent(DWORD dwNodeID, DWORD dwNewParent) = 0;
		// ������ƶ��������ֵܽ������ϲ�
		virtual BOOL  MoveToTop(DWORD dwNodeID) = 0;
		// ������ƶ��������ֵܽ������²�
		virtual BOOL  MoveToBottom(DWORD dwNodeID) = 0;
		// ������ƶ���ĳ���ֵܽ����ϲ�
		virtual BOOL  MoveAfter(DWORD dwNodeID, DWORD dwAfterSiblingNodeID) = 0;
		
		virtual BOOL  DeleteUINode(DWORD dwUINodeID) = 0;
		virtual void  DeleteAllUINode() = 0;

		virtual DWORD GetRootUINode() = 0;
		/**
		 *  @brief ��øý�����һ����㣨�����ı������ȸ��������֣�
		 *  @param dwUINodeID ���ID
		 *  @param bIgnoreSightlessNode �Ƿ���Բ��ɼ����
		 *  @return ���dwUINodeID����һ�����ID
		 *  @sa 
		 */
		virtual DWORD GetNextUINode(DWORD dwUINodeID, BOOL bIgnoreSightlessNode) = 0;
		
		virtual DWORD GetParentUINode(DWORD dwUINodeID) = 0;
		// �õ����ϲ��Ӵ���
		virtual DWORD GetTopChildUINode(DWORD dwUINodeID) = 0;
		// �õ����²��Ӵ���
		virtual DWORD GetBottomChildUINode(DWORD dwUINodeID) = 0;
		// �õ��ýڵ���ϲ��ֵܽڵ�
		virtual DWORD GetUpperUINode(DWORD dwUINodeID) = 0;
		// �õ��ýڵ���²��ֵܽڵ�
		virtual DWORD GetNetherUINode(DWORD dwUINodeID) = 0;

		/**
		 *  @brief �жϽ��A�Ƿ���B�����ڽ�㣨�������游�����游�ĸ���㡣������
		 *  @param dwAncestorID ���ڽ��ID
		 *  @param dwChildID ������ID
		 *  @return ����Ƿ���TRUE�����Ƿ���FALSE
		 */		
		virtual BOOL  IsAncestorUINode(DWORD dwAncestorID, DWORD dwChildID) = 0;
		// �ýڵ��Ƿ�ɼ�
		virtual BOOL  IsNodeVisible(DWORD dwUINodeID) = 0;
		// �ýڵ��Ƿ��ڱ�show/hide״̬
		virtual BOOL  IsNodeShow(DWORD dwUINodeID) = 0;
		// �ýڵ��Ƿ񲻿ɲ���
		virtual BOOL  IsNodeDisable(DWORD dwUINodeID) = 0;
		// �ýڵ��Ƿ����Ϣ͸��
		virtual BOOL  IsNodeTransparent(DWORD dwUINodeID) = 0;
		
		virtual BOOL  ShowUIObject(DWORD dwUINodeID, BOOL bShow) = 0;
		virtual BOOL  DisableUIObject(DWORD dwUINodeID, BOOL bDisable) = 0;
		virtual BOOL  TransparentUIObject(DWORD dwUINodeID, BOOL bTransparent) = 0;
		
		virtual BOOL  MoveUIObject(DWORD dwUINodeID, LPRECT lpRect, BOOL bScreen) = 0;
		virtual BOOL  MoveUIObject(DWORD dwUINodeID, int x, int y, int nWidth, int nHeight, BOOL bScreen) = 0;
		virtual BOOL  OffsetUIObject(DWORD dwUINodeID, int cx, int cy) = 0;
		virtual BOOL  GetUIObjectRect(DWORD dwUINodeID, BOOL bScreen, LPRECT lpRect) = 0;
		// ��þ��εĿɼ����֣���ȥ�������ڻ��游������ס�Ĳ��֣�
		//��bClientΪtrueʱ��rectΪ�ͻ�������;����֮Ϊ����ڸ���������
		virtual BOOL  GetUIObjectVisibleRect(DWORD dwUINodeID, BOOL bClient, LPRECT lpVisibleRect) = 0;
		
		// ��ѯ��Rect�����е�����UI�ڵ�ID���飬���ڲ������ڴ棬��ʹ�����ͷ�
		virtual int   FindUINodesbyRect(RECT rcScreen, DWORD* pUINodeID) = 0;
		// bIgnoreTransparentNodeΪtrueʱ��������ҵ�transparent���ԵĽڵ�
		virtual DWORD FindUINodebyPoint(POINT ptScreen, BOOL bIgnoreTransparentNode) = 0;
		virtual DWORD FindUINodebyPoint(
			DWORD dwFromID, 
			POINT pt, 
			BOOL bIgnoreTransparentNode) = 0;
		
		// ��UINodeID��ΪNODEID_DESKTOPʱ����ʾ��Ļ����
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