#ifndef __SCENE_I_H__
#define __SCENE_I_H__

#include "layer_i.h"
#include "june/canvas_i.h"

namespace Tenio
{
	typedef enum enmPlacePos
	{
		EPP_TOP		= 0,	//���������з�topmost�������
		EPP_TOPMOST,		//��������㣬������topmost����
		EPP_BOTTOM,			//���������з�bottommost�������
		EPP_BOTTOMMOST		//��������ײ㣬������bottommost����
	} ENMPLACEPOS;

	class IJuneScene
	{
	public:
		virtual int GetSceneID() = 0;

		// �ڶ������һ��, ������ (x,y). 
		virtual BOOL PlaceAtTop(IJuneLayer* pLayer, int x, int y) = 0;

		virtual BOOL PlaceAt(IJuneLayer* pLayer, int x, int y, ENMPLACEPOS ePos) = 0;

		// �� Scene ������Canvas��x,y��, Canvas�ڵ�UpdageRegion����Ż���
		virtual BOOL DrawAtCanvas(IJuneCanvas* pCanvas, int x, int y) = 0;

		// ��ȡ���λ��
		virtual BOOL GetPosition(IJuneLayer* pLayer, int* pnx, int* pny) = 0;

		/**
		 *  @brief �ƶ����λ��
		 *  @param pLayer ��ָ��
		 *  @param x �µ�xλ�ã������scene
		 *  @param y �µ�yλ��
		 *  @param bUpdateOrg���Ƿ��ԭ���������������б�
		 *  @param bUpdateDst���Ƿ�ѷ��ú�����������������б�
		 *  @return �ɹ��򷵻�TRUE.
		 *  @sa 
		 */	
		virtual BOOL MoveLayerTo(IJuneLayer* pLayer, int x, int y, 
			BOOL bUpdateOrg = TRUE, BOOL bUpdateDst = TRUE) = 0;

		// ɾ����
		virtual void RemoveLayer(IJuneLayer* pLayer) = 0;

		// ��ȡָ��λ���ϵĲ�
		virtual IJuneLayer* GetLayerAt(int x, int y) = 0;

		// �ƶ��㵽���ϲ�
		virtual BOOL MoveLayerToTop(IJuneLayer* pLayer) = 0;

		// �ƶ��㵽���²�
		virtual BOOL MoveLayerToBottom(IJuneLayer* pLayer) = 0;

		// ��pLayer֮������
		virtual BOOL InsertLayerAfter(IJuneLayer* pLayer, IJuneLayer* pLayerInsert, int x, int y) = 0;
		// ��pLayer֮ǰ�����
		virtual BOOL InsertLayerBefore(IJuneLayer* pLayer, IJuneLayer* pLayerInsert, int x, int y) = 0;		

		/**
		 *  @brief �����Ƿ��¼��������
		 *  @param bCollect���Ƿ��¼
		 */	
		virtual void CollectUpdateArea(BOOL bCollect) = 0;

//		/**
//		 *  @brief ����scene���ػ�����ʼ��
//		 *  @param pLayer ��ʼ��ָ�룬��pLayerΪNULLʱ������ײ㿪ʼ����
//		 */
//		virtual void SetReDrawStartLayer(IJuneLayer* pLayer) = 0;
//
//		/**
//		 *  @brief ��ʾĳ�㣬ֻ�������ػ棬������layer����
//		 *  @param pLayer ��Ҫ����ʾ���ָ��
//		 */	
//		virtual void ShowLayer(IJuneLayer* pLayer) = 0;
//
//		/**
//		 *  @brief ����ĳ�㣬ֻ�������ػ棬������layer����
//		 *  @param pLayer ��Ҫ�����ز��ָ��
//		 */
//		virtual void HideLayer(IJuneLayer* pLayer) = 0;
//
//		/**
//		 *  @brief �ػ�ĳ��
//		 *  @param pLayer ��Ҫ�ػ�Ĳ�
//		 */	
//		virtual void RedrawLayer(IJuneLayer* pLayer) = 0;
	};
};

#endif