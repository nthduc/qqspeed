#include "../Top-KartPCH.h"
#include "Config.h"

extern void UpdateCameraSetting();

namespace CameraSet
{
	void SetCamera(float fHeight, float fMaxDist, float fMinDist, float fFocusDist)
	{
		gs_CameraSetting.fCameraHeight		= fHeight;
		gs_CameraSetting.fCameraMaxDist		= fMaxDist;
		gs_CameraSetting.fCameraMinDist		= fMinDist;
		gs_CameraSetting.fCameraFocusDist	= fFocusDist;

		UpdateCameraSetting();
	}

	void GetCamera(float & fHeight, float & fMaxDist,
		float & fMinDist, float & fFocusDist)
	{
		fHeight		= gs_CameraSetting.fCameraHeight	;
		fMaxDist	= gs_CameraSetting.fCameraMaxDist	;
		fMinDist	= gs_CameraSetting.fCameraMinDist	;
		fFocusDist	= gs_CameraSetting.fCameraFocusDist 	;
	}
};