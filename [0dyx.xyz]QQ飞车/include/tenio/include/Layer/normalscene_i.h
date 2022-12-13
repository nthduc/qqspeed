#ifndef __NORMALSCENE_I_H__
#define __NORMALSCENE_I_H__

#include "../teniointerface.h"
#include "scene_i.h"
#include "sceneupdate_i.h"
#include "../june/canvasupdate_i.h"

namespace Tenio
{
	class INormalScene;

	TENIO_MAKE_TYPEID_MAP(INormalScene, COMPONENTID_NORMALSCENE);

	class INormalScene : 
		public IJuneScene, 
		public IComponent, 
		public IJuneSceneUpdate
	{
	public:
		// Scene 使用 pCanvasUpdate 触发绘制事件
		virtual void SetCanvasUpdate(IJuneCanvasUpdate* pCanvasUpdate) = 0;
	};
};

#endif