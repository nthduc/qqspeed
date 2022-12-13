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

		// rc为相对于scene的坐标，rcvisible为控件的client坐标
		virtual void AddLayersToScene(RECT& rc, RECT& rcVisible) = 0;
		virtual void MoveLayers(RECT& rc, RECT& rcVisible) = 0;	
		virtual void ShowLayers(BOOL bShow, UINT nFadeTime = 0) = 0;
		virtual void RemoveLayers() = 0;

		virtual IJuneLayer* GetFirstLayer() = 0;
		virtual IJuneLayer* GetLastLayer() = 0;
		virtual LPCTSTR GetTipText() = 0;
		virtual BOOL IsTabStop() = 0;

		/**
		 *  @brief 获得脚本中事件对应的函数名
		 *  @param nScriptEventID,事件ID，由scripteventdefine.h
		 *  @param lpszFuncName 事件对应函数名
		 *  @param 函数名最大长度
		 *  @return 如果事件ID对应的函数名不存在或长度为0，则返回FALSE,反之返回TRUE
		 */
		virtual BOOL GetScriptFuncName(int nScriptEventID, LPTSTR lpszFuncName, UINT nMaxLen) = 0;
	};
}

#endif
