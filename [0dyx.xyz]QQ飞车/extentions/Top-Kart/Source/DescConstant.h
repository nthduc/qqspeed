#pragma once

//tolua_begin
struct FLOAT2
{
	float x;
	float y;
};

struct FLOAT3
{
	float x;
	float y;
	float z;
};
struct stKarBaseParam
{
	float vMass;
	float vDigitalSteeringDelta;
	float vSteeringMaxAngle;
	float vMoterForce;
	float vMaxVelocity;
	float vCameraDistance;
	float vTransmissionEfficiency;
	float vDifferentialRatio;

	float motorMinRpmToGearDown;
	float motorMaxRpmToGearUp;
	float motorMinRpm;
	float motorMaxRpm;

	unsigned int gearNbForwardGears;
	float gearBackwardGearRatio;
	float gearForwardGearRatios[32];

	unsigned int wheelApproximation;
	float wheelWidth;
	float wheelSuspension;
	float wheelSpringRestitution;
	float wheelSpringDamping;
	float wheelSpringBias;
	float wheelmaxBrakeForce;
	float wheelFrictionToFront;
	float wheelFrictionToSide1;
	float wheelFrictionToSide2;

	float radiusCoe1;
	float radiusCoe2;
	float radiusCoe3;
	float radiusCoe4;
	float radiusCoe5;

	FLOAT2 torqueCurCoe[6]; 


	float m_fVMax_NormalAccel	;
	float m_fVMax_UseItemAccel	;	

	float m_fVTopAccel_NormalAccel	;
	float m_fVTopAccel_UseItemAccel	;

	float m_fAcel_OriForward	;		
	float m_fAcel_OriBack		;	

	float m_fAcelMax_NormalAccel	;	
	float m_fAcelMax_UseItemAccel	;	
	float m_fDeAcelMax			;	

	float m_fAcel_Vfu_NormalAccel   ; 
	float m_fAcel_Vfu_UseItemAccel	;

	float m_fVMin				;	

	float m_fAcel_NonePress		;	
	float m_fAcel_Vfu_NonePress		;

	float m_fAcel_AcelToNormal	;	
	
	unsigned int bDreamDrifting;
};

struct stFriction
{
	float m_fFricFactor;	// 摩擦力因子
	float m_fRollFriction;  // 滑动摩擦力
	float m_fSlideFriction; // 滚动摩擦力
};

struct stKartCollisionParam
{
	// 角度为角度制
	// 按照碰撞方向与车的前向的夹角的绝对值alpha,发生如下现象

	// 发生后退, 这个角度为从0到m_fAlpha

	float m_fBackSpeedCoef;// 速度损耗系数

	// 稍微调转车头继续前进, 速度没有损失, 这个角度为m_fAlpha到m_fAlpha1, 有个转弯角速度,有一定的速度损失
	float m_fAlpha;
	float m_fAlpha1;
	float m_fTurnSpeed;
	float m_fLinearSpeedLostRate;

	// 从m_fAlpha1到m_fAlphaDrift1发生漂移
	float m_fAlphaDrift1;
	// 从m_fAlphaDrift1到m_fAlpha2,发生转向
	float m_fAlpha2;
	float m_fTurnSpeed1;
	float m_fLinearSpeedLostRate1;

	//从m_fAlpha2到180, 发生向前移动
	float m_fForwardSpeedCoef;
};

struct stSendPkg
{
	unsigned int _begin_dis;
	unsigned int _end_dis;
	unsigned int _frequence;
};

struct stSuspension
{
	FLOAT3	sim_vPosition;
	FLOAT3	sim_vSpecChassis;
	FLOAT3	sim_vSpecBase;
	float	sim_fSpecWheelRadius;
	float	sim_fMassChassis;
	float	sim_fMassWheel;

	//z
	float sim_fZBases;
	float sim_fZChassis;
	float sim_fZPillar;
	float sim_fZWheels;

	//anchors
	float sim_fAnchorZSuspensions;
	float sim_fAnchorZPillar;
	float sim_fAnchorChassis;

	float sim_fSusSpring;
	float sim_fSusDamping;
	float sim_fSusValue;
	float sim_fSusRestitution;
	float sim_fSusLimitPlane;
};

namespace DescSuspension
{
	extern stSuspension suspension;
};
namespace DescSendPkg
{
	extern stSendPkg SendPkg[5];
};

namespace DescConstant
{
	extern int KarType;
	extern stKarBaseParam KarBaseParam[64];
	extern stKartCollisionParam KartCollisionParam;
	extern stFriction           KartFriction;
	extern float				KartRotate;
	extern float				KartRotate1;
};

namespace DescCamera
{
	extern float cameraNormalHeight;
	extern float cameraNormalMaxDis;
	extern float cameraNormalMinDis;
	extern float cameraNormalFocusDis;
	extern float cameraNormalCoef;
	extern float cameraNormalInterp;

	extern float cameraSpeedUpHeight;
	extern float cameraSpeedUpMaxDis;
	extern float cameraSpeedUpMinDis;
	extern float cameraSpeedUpFocusDis;
	extern float cameraSpeedUpCoef;
	extern float cameraSpeedUpInterp;

	extern float cameraHeightIterp;
	extern float cameraFocusIterp;
	extern float cameraMaxSpeed;
//	extern float cameraSpeedUpRatio;


	extern float CAMERA_FAR;// = 220.0f;
	extern float CAMERA_NEAR;// = 1.0f;
	extern float CAMERA_AspectRatio;// = 1;
	extern float CAMERA_FOV;// = 45;
};


namespace GameSetting
{
	extern float SpeedLineShowVelo;
	extern float SpeedDispCoef;
	extern bool ShowShadow;
};

namespace ContactConfig
{
	extern float smallangle;
	extern float midangle;
	extern float bigangle;
	extern float inversevelocityrate;
};

namespace PhysSceneConfig
{
	extern float gravity;	
};

//tolua_end
