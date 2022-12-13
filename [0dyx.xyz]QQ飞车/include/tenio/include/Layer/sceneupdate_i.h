#ifndef __SCENEUPDATE_I_H__
#define __SCENEUPDATE_I_H__

#include "layer_i.h"

namespace Tenio
{
	class IJuneLayer;
	class IJuneSceneUpdate
	{
	public:
		virtual BOOL LockUpdateAction() = 0;

		virtual BOOL Update(IJuneLayer* pLayer) = 0;

		virtual BOOL UnlockUpdateAction() = 0;

		virtual void CleanAllUpdateRegion() = 0;
	};
};

#endif