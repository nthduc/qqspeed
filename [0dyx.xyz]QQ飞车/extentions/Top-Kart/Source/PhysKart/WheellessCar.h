#pragma once 

//#include <NxExtended.h>
#include <NxPhysics.h>
#include <NxScene.h>
#include "NxBoxController.h"
#include "CarAccel.h"
#include "NxCapsuleController.h"
#include "DriftControl.h"
#include "../GamePropCollisionReport.h"
#include "../PhysXManager.h"

#define			WHEEL_0						1<<0      //front-left
#define			WHEEL_1						1<<1		//front-right
#define			WHEEL_2						1<<2		//back-right
#define			WHEEL_3						1<<3      //back-left

#define         WHEELS_FRONT				WHEEL_0|WHEEL_1
#define			WHEELS_BACK					WHEEL_2|WHEEL_3	
	
#define         WHEELS_LEFT					WHEEL_0|WHEEL_3
#define			WHEELS_RIGHT				WHEEL_1|WHEEL_2	

#define          WHEELS_ALL					WHEEL_0|WHEEL_1|WHEEL_2|WHEEL_3

#define         WHEEL0_INDEX				0
#define         WHEEL1_INDEX				1
#define         WHEEL2_INDEX				2
#define         WHEEL3_INDEX				3

#define			WHEEL_COUNT					4
#define			NONE_WHEEL					0

///道具点统一回调类
class GPPointReport : public CGamePropCollisionReport
{
public:
	virtual OnTrigger(LPVOID lpGPData,LPVOID lpHitObjData);
};

struct st_vec3i{
	int x;
	int y;
	int z;
};

typedef struct tagBumpInfo{
	int nPitch;// -1, 下坡， 1，上坡, 0, 无变化
	float fAngle;//角度
}BUMPINFO;

class WheellessCar;

//处理重力相关事务

//class CTumbler{
//public:
//	CTumbler(WheellessCar* parent);
//	~CTumbler();
//
//	void applyGravity(NxReal dt);
//
//	WheellessCar* _parent;
//
//	
//};

#define          NUM_WHEELS		5
#define SKINWIDTH	0.05f

//static ControllerManager gCM;

#define     MIN_DIS					0.1f
#define     CONTACTGRD				1
#define     NONE_CONTACTGRD			2

#define     DRIFT_LEFT				-1
#define		DRIFT_RIGHT				1

#define     SIMPLE_DRIFT			1
#define     DRIFT_SIDE_ANGLE		15.0f
class CSpring
{
public:
	CSpring();
	~CSpring();


	void takeNewSurge(int type, NxReal cycle, NxReal spring, int loop);
	void update(NxReal dt);
	//void setSpring(float spring){_spring = spring;}

	NxReal getSpring(){return _spring;}
	NxReal getOffset(){return _offset;}

	bool   isStopped(){return _stopped;}
	void	setStopped(bool bStopped){_stopped = bStopped;}
	NxReal _offset;

protected:
	
	int _type;

	int _loop;

	bool _stopped;
	NxReal _coeff;
	NxReal _step;
	NxReal _angle;
	NxReal _curAngle;
	NxReal _spring;

	    
};


class CWheel
{
public:

	enum CONTACT_TYPE{
		CT_NONE=0,
		CT_NEAR,
		CT_FAR
	};

	CWheel(NxScene* pScene, WheellessCar* parent);
	~CWheel();

	void setPos(const NxVec3& pos){_posLocal = pos;}
	void setRadius(NxReal radius){_radius = radius;}
	NxVec3 getPos(){return _posLocal;}
	NxVec3 getPosGlobal();
	void rayCast();
	void bump(NxReal dt);

	void GetBumpInfo(BUMPINFO& bumpInfo);

	CSpring _spring;
	NxReal _radius;

	NxVec3 _posLocal;
	NxVec3 _offset;
	NxScene* _nxScene;

	NxShape* _nxShape;
	WheellessCar* _parent;
	int _contactType;
	NxVec3 _contactPos;

	NxVec3 _preContactN;//the latest contact normal.
	NxVec3 _curContactN;
	int _contactedActorType;
};




class WheellessCar
{
public:
	WheellessCar(NxScene* pScene, NxVec3 pos, NiAVObject* pModel);
	~WheellessCar();

	void		control(float accel, float steer, NxReal dt, bool keyShift, bool keyUp, bool keyLeft, bool keyRight);

	void		SynchronizeModel(float dt = 0.1f, bool hostPlayer = true);

	void		SetPose(const NxMat34& kPose);
	void		SetPos(const NxVec3& pos);
	NxVec3		GetPos();
	NxReal		GetVelocity();
	void		setVelocityLocal(const  NxVec3& vel);
	NxVec3		GetVelocityLocal();	
	void		handleContactPair(NxContactPair& pair, NxU32 carIndex, NxU32 event);
	void		setAccelTime(NxReal fTime, NxReal fGear = 0.0f);
	NxMat33		getGlobalOrientation(){return _rotateTotal;}
	void		SetGlobalOrientation(NxMat33 ori);

	NxReal		GetTurnAngle(){ return _turnAngle;}
	void		SetTurnAngle(NxReal alpha){ _turnAngle = alpha;}
	void        AddTurnAngle(NxReal alpha) { _turnAngle += alpha;}
	void		RemoteCarUpdate(float dt);

	bool		IsInHell();
	bool		IsDrifting();
	void		SetDrifting(bool bDrift);
	NxVec3		GetVelDrifting();
	void		SetVelDrifting(const NxVec3& vel);

	NxVec3		GetTerrainNormal();

	void		StopDrift();
	// Add by wincoqin, Disable move ability when we locked player
	void		SetMovable(bool bCanMove){m_bCanMove = bCanMove;}
	// Add end


	NxActor*	_controllerActor;

//private:
public:

	void _controlNormalState(float accel, float steer, NxReal dt, bool keyShift, bool keyUp, bool keyLeft, bool keyRight, bool enableInput = true);
	void _sortOffset(NiPoint3* offsets, int num);


	void		applyCounterforce(const NxControllerShapeHit& hit);
	void		ProcessControllersHit(const NxControllersHit& hit);

	int 		_caculateContactInfo(NxVec3& normal);
	void		_ajustPose();

	void		_bumpWheeels(NxReal dt);

	void		_initWheels();
	void		_wheelRayCast();

	CWheel *	GetWheel( int i )
	{ 
		if ( i < 0 || i >= 4) 
			return NULL; 
		return _wheels[i];
	}


	NxVec3      getForward(){return NxVec3(1, 0, 0);}
	NxVec3      getLeft(){return NxVec3(0, 1, 0);}
	NxVec3      getUp(){return NxVec3(0, 0, 1);}

	NxVec3      getGlobalForward();
	NxVec3      getGlobalUp();

	NxMat34     getGlobalPos(){ return _controller->getActor()->getGlobalPose();}

	//NxBoxController * getBoxController(){ return _controller;}
	NxCapsuleController* getBoxController(){ return _controller;}

	NxVec3 CaculateTerrainNormal();

	NxReal      GetGravity() { return _gravity; }
	NxReal      _maxVelocity;
	NxReal		m_accelTime;

private:
	//模型震动相关
	NiMatrix3		m_mt3DModel;  //3D赛车模型的旋转矩阵
	bool			m_bIsBumping; //
	st_vec3i		m_bumpTime;   //震动的次数
	NiPoint3		m_vBumpAngle; //模型震动的目标角度
	NiPoint3		m_vBumpAngleCur;//模型震动的当前角度
	NiPoint3		m_vBumpAngularVel;//模型震动的角速度

	void RotMeTender(NiMatrix3& mtSrc, const NiMatrix3& mtDest, float dt);
	void BumpMeTender(const NiMatrix3& matDest, float dt);
	void UpdateWheelsBump(const NiMatrix3& matDest, float dt);
	bool GenerateBump();
	bool GetBumpOffset(float* pfOffsets);
	//~
public:
	NxVec3		_extents;//the size of the car
	NxVec3      _velocityLocal;//local

	NxReal		_acceleration;

	bool		_applyGravity;
	NxReal		_gravity;
	NxReal      _gvavityContribution;
	NxVec3		_vGravityContributionDir;//重力沿斜坡分量的方向

	NxU32		_collisionFlags;

	NxReal				_carRot;// steering angle
	NxReal				_wheelRoll;
	NxReal				_steeringAVel;//steering angular velocity
	NxReal				_steeringState;
	
	int					_hasContactGrd;
	NxVec3				_vNormalGrd;//normal of the ground

	//drifting related:
	bool	_isDrifting;
	NxReal	_driftingAngle;
	NxReal  _driftingAngleCur;
	NxReal	_driftingAngularAccel;
	NxReal  _driftingTime;
	
	NxReal  _driftingSideAngleMax;
	NxReal  _driftingSideAngleCur;
	NxReal  _driftingWheelOffsetZ;



	NxVec3  _velDriftingGlobal;

	int		_driftingType;
	int		_driftDir;


	//~drifting related



	//NxBoxController*	_controller;
	NxCapsuleController* _controller;
	NxScene*			_nxScene;
	

	CWheel*	_wheels[5];
	NxMat33 _rotateTotal;

protected:
	NxReal _turnAngle;
	NiAVObject* _pCarRoot;

	//the original position of the wheels.
	NiPoint3    m_posWheels[4];

	NxReal _wheelRotAngle;

	NxExtendedVec3 _prePos;

	bool  _bCreateNewTrack;
	NxU32 _nextTrailSlot;




protected:

public:
	CCarAccelControl * 	GetCarAccelControl() { return m_pCarAccelControl;}
	CDriftControl *		GetDriftingControl() { return m_pCarDriftControl;} 
	NxReal GetForwardAccel();
	NxReal WheellessCar::GetNormalAngleVel();
	NxReal WheellessCar::GetGriftingAngleVel();

protected:
	CCarAccelControl * m_pCarAccelControl;
	CDriftControl    * m_pCarDriftControl;
	bool			   m_bCanMove;
	float				m_fSteer;

public:
	int HasContactGrd() { return _hasContactGrd;}
protected:
	///道具点静态回调实例
	GPPointReport m_kGPReport;
};