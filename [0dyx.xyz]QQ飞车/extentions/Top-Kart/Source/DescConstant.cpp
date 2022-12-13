#pragma once
#include "Top-KartPCH.h"
#include "DescConstant.h"
//tolua_begin

namespace DescConstant
{
	int KarType;
	stKarBaseParam KarBaseParam[64];
	stKartCollisionParam KartCollisionParam;
	stFriction           KartFriction;
	float				KartRotate;
	float				KartRotate1;
};

namespace DescSendPkg
{
	stSendPkg	SendPkg[5];
};
namespace DescSuspension
{
	stSuspension suspension;
};

namespace DescCamera
{
	float cameraNormalHeight;
	float cameraNormalMaxDis;
	float cameraNormalMinDis;
	float cameraNormalFocusDis;
	float cameraNormalCoef;
	float cameraNormalInterp;

	float cameraSpeedUpHeight;
	float cameraSpeedUpMaxDis;
	float cameraSpeedUpMinDis;
	float cameraSpeedUpFocusDis;
	float cameraSpeedUpCoef;
	float cameraSpeedUpInterp;

	float cameraHeightIterp;
	float cameraFocusIterp;
	float cameraMaxSpeed;

//	float cameraSpeedUpRatio = 5;


	float CAMERA_FAR;// = 220.0f;
	float CAMERA_NEAR;// = 1.0f;
	float CAMERA_AspectRatio;// = 1;
	float CAMERA_FOV;// = 45;
};

namespace GameSetting
{
	float SpeedLineShowVelo = 20;
	float SpeedDispCoef = 10.0f;
	bool ShowShadow = true;
};

namespace ContactConfig
{
	float smallangle;
	float midangle;
	float bigangle;
	float inversevelocityrate;
};

namespace PhysSceneConfig
{
	float gravity;
};
//tolua_end
