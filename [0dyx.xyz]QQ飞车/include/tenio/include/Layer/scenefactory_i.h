#ifndef __SCENEFACTORY_I_H__
#define __SCENEFACTORY_I_H__

#include "../teniointerface.h"
#include "../layer/scene_i.h"
#include "../layer/sceneupdate_i.h"

namespace Tenio
{
	typedef enum enmSceneID
	{
		ESI_NORMAL	= 1
	} ENMSCENEID;
	//////////////////////////////////////////////////////////////////////////
	class ISceneFactory;

	TENIO_MAKE_TYPEID_MAP(ISceneFactory, COMPONENTID_SCENEFACTORY);

	class ISceneFactory : public IComponent
	{
	public:
		virtual IJuneScene* CreateScene(ENMSCENEID eSceneID, IJuneSceneUpdate** pSceneUpdate) = 0;
		virtual void ReleaseScene(IJuneScene* pScene) = 0;
	};
};

#endif

// InterfaceName:SceneFactory
// SubFolderName:layer
