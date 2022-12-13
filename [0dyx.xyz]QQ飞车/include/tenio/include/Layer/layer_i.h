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

		// ���(x,y)�Ƿ����������, (x,y)������뱾�����Ͻǵ�����
		virtual BOOL HitTest(int x, int y) = 0;

		// ��ȡ�����Alphaֵ(0-255)
		virtual BYTE GetPaintAlpha() = 0;

		// ����Alphaֵ
		virtual void SetPaintAlpha(BYTE byValue) = 0;

		// ���clip region, region�����Ա������Ͻ�Ϊԭ��
		virtual IJuneRegion* GetClipRegion() = 0;

		//���������Ӳ��clip region
		virtual void OnSetClipRegion() = 0;

		// �ò��Ƿ�ɼ�
		virtual BOOL IsVisible() = 0;

		// ����Ϊ���ɼ��������۸ò��Ƿ�show��������Ƴ�����
		virtual void SetVisible(BOOL bVisible) = 0;

		// ��ʾ�����ظò�
		virtual void Show(BOOL bShow) = 0;

		/**
		 *  @brief �ж�ĳ���Ƿ�͸��
		 *  @param x x������
		 *  @param y y������
		 *  @return ���͸���򷵻�TRUE����͸���򷵻�FALSE
		 */		
		virtual BOOL IsPixelTransparent(int x, int y) = 0;

		virtual void SetSceneUpdate(IJuneSceneUpdate* pSceneUpdate) = 0;
	};
};

#endif