#ifndef __CONTROLCORE_I_H__
#define __CONTROLCORE_I_H__

#include "../teniointerface.h"
#include "../layer/scene_i.h"
#include "../layer/layer_i.h"
#include "../layer/sceneupdate_i.h"
#include "../uilib/control_i.h"
#include "../uilib/variableparam_i.h"

namespace Tenio
{
	class IControlCore 
	{
	public:
		virtual void SetOwnerControl(IControl* pControl) = 0;
		virtual void SetScene(IJuneScene* pScene, IJuneSceneUpdate* pSceneUpdate) = 0;
		virtual IJuneScene* GetScene() = 0;
		virtual IJuneSceneUpdate* GetSceneUpdate() = 0;
		virtual BOOL IsPixelTransparent(int x, int y) = 0;
		virtual BOOL OnMessage(IVariableParam* pParam) = 0;

		// rcΪ�����scene�����꣬rcvisibleΪ�ؼ���client����
		virtual void AddLayersToScene(RECT& rc, RECT& rcVisible) = 0;
		virtual void MoveLayers(RECT& rc, RECT& rcVisible) = 0;	
		virtual void ShowLayers(BOOL bShow, UINT nFadeTime = 0) = 0;
		virtual void RemoveLayers() = 0;

		virtual IJuneLayer* GetFirstLayer() = 0;
		virtual IJuneLayer* GetLastLayer() = 0;
		virtual LPCTSTR GetTipText() = 0;
		virtual BOOL IsTabStop() = 0;

		/**
		 *  @brief ��ýű����¼���Ӧ�ĺ�����
		 *  @param nScriptEventID,�¼�ID����scripteventdefine.h
		 *  @param lpszFuncName �¼���Ӧ������
		 *  @param ��������󳤶�
		 *  @return ����¼�ID��Ӧ�ĺ����������ڻ򳤶�Ϊ0���򷵻�FALSE,��֮����TRUE
		 */
		virtual BOOL GetScriptFuncName(int nScriptEventID, LPTSTR lpszFuncName, UINT nMaxLen) = 0;
	};
}

#endif
