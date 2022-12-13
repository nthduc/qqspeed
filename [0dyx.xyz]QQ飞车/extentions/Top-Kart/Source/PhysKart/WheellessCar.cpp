#include "..\Top-KartPCH.h"

#include "WheellessCar.h"
#include "../EventContainer.h"
#include "../DescConstant.h"
#include "../PowerController.h"
#include "../GPPoint.h"
#include "../GPNetInterCommn.h"

#define NUM_TRAIL_POINTS 1600


CSpring::CSpring()
{
	takeNewSurge(2, 2.0f, 0.005f, -1);
}
void CSpring::takeNewSurge(int type, NxReal cycle, NxReal spring, int loop)
{
	_type = type;

	switch(type) {
	case 0:
	case 1:
		_angle = NxPi;
		break;
	case 2:
		_angle = 2.0f * NxPi;
		break;
	default:
		_angle = 2.0f * NxPi;
	}

	if (type == 0)
		_coeff = 1.0f;
	else
		_coeff = -1.0f;

	_step = _angle / cycle;
	_spring = spring;
	_loop = loop;
	_stopped = false;
	_offset = 0.0f;
	_curAngle = 0.0f;
}

void CSpring::update(NxReal dt)
{
	if (_stopped)
		return;

	_curAngle += dt*_step;

	if (_curAngle < _angle)
	{
		_offset = _coeff * _spring * NxMath::sin(_curAngle);
	}
	else
	{
		if (_loop < 0)
			_curAngle = 0.0f;
		else if ((_loop -1) > 0)
		{
			_loop--;
			_curAngle = 0.0f;
		}
		else
		{
			//_stopped = true;
			//takeNewSurge(2, 0.2f, 0.003f, -1);
		}
	}
}

#define         TINY_DIST          0.3f

CWheel::CWheel(NxScene* pScene, WheellessCar* parent)
{
	_preContactN = NxVec3(0, 0, 1.0f);
	_nxScene = pScene;
	_parent = parent;
}

inline NxVec3 CWheel::getPosGlobal()
{
	if (_parent->_controller)
	{
		NxMat34 trans(_parent->getGlobalOrientation(), _parent->GetPos());
		return trans* _posLocal;
	}
	else
	{
		assert(0);
		return NxVec3(0,0,0);
	}
}

/* 
	rayCast() ��������

	���ӵ�����  , �������ӵ����·�������ʲô �� ��ʵ��֪����ʲô��ֻҪ������CG_ENVIRONMENT Group �еļ�����Ϳ���
	����ʣ�Ҳ�е��档	����Ҳ ֻ֪����������  CT_FAR, CT_NEAR , CTNONE 
*/


void CWheel::rayCast()
{
	if (!_nxScene) {
		return;
	}

	NxVec3 pos = getPosGlobal();

	NxRay ray(pos, -_parent->getGlobalUp());

	NxRaycastHit hit;

	// Group �õıȽ���  ���صļ����屣���� _nxShape �й���һ��ʹ�á� ����Ϊ�β�ֱ�ӵõ���ײ�����أ� 

	_nxShape = _nxScene->raycastClosestShape(ray, NX_ALL_SHAPES,hit, PhysXManager::CG_ENVIRONMENT);

	// �˴�һ��Ҫ��֤���ӵ����� �� ���ӵĵ��̳�ƽ��  TINY_DIST ��ֵ�Ӻζ����أ��� 
	if (_nxShape)
	{
		if (hit.distance > TINY_DIST + 0.125)
			_contactType = CT_FAR;
		else
			_contactType = CT_NEAR;

		_preContactN = _curContactN;
		_curContactN = hit.worldNormal;
		_contactPos = pos + ray.dir * hit.distance;
		XUserData *pUd = (XUserData*)_nxShape->getActor().userData;

		if (pUd)
		{
			_contactedActorType = pUd->_actorType;
		}
		else
		{
			_contactedActorType = XUserData::AT_NODEFINE;
		}
	}
	else
	{
		_contactType = CT_NONE;
		_contactedActorType = XUserData::AT_NODEFINE;
	}

}

void CWheel::bump(NxReal dt)
{
	NxReal angle = NxMath::acos(_preContactN.dot(_curContactN));
	NxReal fvel = _parent->_velocityLocal.magnitude();

	fvel = fvel * fvel * fvel;

	NxReal coeff = fvel / (1.0f + fvel);

	if (angle > NxPi/180.f * 1.0f)
	{
		//_spring.takeNewSurge(2, 0.750f, 0.015*coeff, 1);
		_spring.takeNewSurge(1, 0.750f, 0.015*coeff, 1);
	}
	else
	{
		_spring.update(dt);
	}

	_offset.zero();
	_offset.z = _spring._offset;
}

void CWheel::GetBumpInfo(BUMPINFO& bumpInfo)
{
	if (this->_contactType != CT_NEAR)
		bumpInfo.nPitch = 0;
	else
	{
		NxReal angle = NxMath::acos(_preContactN.dot(_curContactN));
		if (angle > NxMath::degToRad(1.0f))
		{
			bumpInfo.fAngle = angle;

			//�жϳ�����һ���£���������һ����
			NxVec3 velG = _parent->_rotateTotal * _parent->_velocityLocal;

			NxVec3 vCross = _preContactN.cross(_curContactN);
			vCross.normalize();
			NxVec3 vCross2 = _preContactN.cross(velG);
			vCross2.normalize();

			float fTheta = NxMath::acos(vCross.dot(vCross2));

			if (fTheta > NxMath::degToRad(90.0f))
			{
				bumpInfo.nPitch = 1;//����
			}
			else
			{
				bumpInfo.nPitch = -1;//����
			}
		}
		else
		{
			bumpInfo.fAngle = 0.0f;
			bumpInfo.nPitch = 0;
		}
	}

}

///////888888888888888888888888888888888888888888888888888



//#define SKINWIDTH	0.001f
#define SKINWIDTH	0.05f

//static ControllerManager gCM;

#define     MIN_DIS					0.1f
#define     CONTACTGRD				1
#define     NONE_CONTACTGRD			2

#define     DRIFT_LEFT				-1
#define		DRIFT_RIGHT				1

#define     SIMPLE_DRIFT			1
#define     DRIFT_SIDE_ANGLE		15.0f






class MyReport : public NxUserControllerHitReport
{
public:
	virtual NxControllerAction  onShapeHit(const NxControllerShapeHit& hit)
	{
		//NxBoxController * pControl = NULL;
		NxCapsuleController * pControl = NULL;

		if( NxPlayMgr::GetHostPlay()->GetKart()->GetVehicle()) {
			pControl = NxPlayMgr::GetHostPlay()->GetKart()->GetVehicle()->getBoxController();
		}

		if (!pControl) 
			return NX_ACTION_NONE;

		if(1 && hit.shape)
		{
			//NxController * pColisionController = hit.controller;
			XUserData* pUD = (XUserData*)hit.shape->getActor().userData;
			if (pUD->_actorType != XUserData::AT_TERRAIN )
			{
				if( hit.controller == pControl)
				{
					NxVec3 vVelo = hit.controller->getActor()->getLinearVelocity();

					if (vVelo.magnitude() > 40.0f)
					{
//						EventContainer::Get()->PushPhysicEvent(EventContainer::PHYSIC_HEAVY_HIT);	
						if(NxPlayMgr::GetHostPlay())
							NxPlayMgr::GetHostPlay()->PushPlayerAnimEvent(CAnimEventMgr::EAET_HIT);
					}
					else
					{
//						EventContainer::Get()->PushPhysicEvent(EventContainer::PHYSIC_LIGHT_HIT);
						if(NxPlayMgr::GetHostPlay())
							NxPlayMgr::GetHostPlay()->PushPlayerAnimEvent(CAnimEventMgr::EAET_HIT);
					}

					Play * p = NxPlayMgr::GetHostPlay();
					if (p->GetKart() && p->GetKart()->GetVehicle())
					{
						p->GetKart()->GetVehicle()->StopDrift();
					}
				}
			}
		}


		if(1 && hit.shape)
		{
			XUserData* pUD = (XUserData*)hit.shape->getActor().userData;
			if (pUD)
			{
				if (pUD->_actorType == XUserData::AT_FENCE_MANUAL || pUD->_actorType == XUserData::AT_FENCE_GAMEBRYO)
				{
					XUserData* pudCar = (XUserData*)hit.controller->getActor()->userData;
					if (pudCar)
					{
						if (pUD->_actorType == XUserData::AT_FENCE_GAMEBRYO)
						{
							NxControllerShapeHit hit2 = hit;
							hit2.worldNormal *= -1.0f;
							((WheellessCar*)pudCar->userData)->applyCounterforce(hit2);
						}
						else
							((WheellessCar*)pudCar->userData)->applyCounterforce(hit);//.worldNormal);
					}

				}
				else if (pUD->_actorType == XUserData::AT_PUSHABLE)
				{
					NxActor& actor = hit.shape->getActor();
					if(actor.isDynamic())
					{
						// We only allow horizontal pushes. Vertical pushes when we stand on dynamic objects creates
						// useless stress on the solver. It would be possible to enable/disable vertical pushes on
						// particular objects, if the gameplay requires it.
						//if(hit.dir.y==0.0f)
						{
							NxF32 coeff = actor.getMass() * hit.length * 10.0f;
							actor.addForceAtLocalPos(hit.dir*coeff, NxVec3(0,0,0), NX_IMPULSE);

							XUserData* pudCar = (XUserData*)hit.controller->getActor()->userData;
							if (pudCar)
							{
								WheellessCar* pCar = (WheellessCar*)pudCar->userData;
								pCar->_velocityLocal *= 0.8f;
							}
							//						actor.addForceAtPos(hit.dir*coeff, hit.controller->getPosition(), NX_IMPULSE);
							//						actor.addForceAtPos(hit.dir*coeff, hit.worldPos, NX_IMPULSE);
						}
					}
				}
			}

		}

		return NX_ACTION_NONE;
		//		return NX_ACTION_PUSH;
	}

	virtual NxControllerAction  onControllerHit(const NxControllersHit& hit)
	{
		XUserData* pudCar = (XUserData*)hit.controller->getActor()->userData;
		((WheellessCar*)pudCar->userData)->ProcessControllersHit(hit);//.worldNormal);
		NxSceneMgr::Get()->GetPowerController()->AccumulateFail();
		return NX_ACTION_NONE;
	}

}gMyReport;



/* --------------------------------------------------------------------------------
add by hoverzhao  ��Ҫ����˵��

_steeringAVel 								// �Ǽ��ٶ�
_steeringState								// ʵ���������Ǹ����ٶ� ��1 �� -1 ֮��

_extents 									// ������Գ������ĵ�ƫ��, �����������ӵ�

_driftingAngularAccel 						// ûƽ�����ǵ��� Ư�ƽ��ٶ� �õ�

_isDrifting 
_driftingAngle 								// Ư�ƽǶ� Ư��״̬ʱÿ�� �������ݼ�
_driftingAngleCur 							// ��ʱû���õ�
_driftingSideAngleCur 						// Ư��ʱ��,������б�ĽǶ�


_velDriftingGlobal							// Ư���ٶ�
_wheelRotAngle 								// ���ӵ���ת��
_rotateTotal								// ����Ҫ�ı��� ÿ�嶼Ҫ�����ת���� ( �ɵ���/ _turnangle _driftingAngle  _driftingSideAngleCur �����ع�ͬ����)

_turnAngle = NxPi;							// Ϊ�γ�ʼ��ΪNxPi ? 
desc.slopeLimit								// �����ϵ����б��
desc.skinWidth								// Ƥ��????
desc.stepOffset								// �ܹ������ϰ������߶�
m_pCarAccelControl							// ��ȡ����ǰ������ �ļ��ٶ�  ��� CCarAccelControl ��
----------------------------------------------------------------------------------*/


WheellessCar::WheellessCar(NxScene* pScene, NxVec3 pos, NiAVObject* pModel)
{
	_nxScene = pScene;
	_pCarRoot = pModel;
	_velocityLocal = NxVec3(0,0,0);

//	_gravity = -9.8f * 1.0f;
	_gravity = PhysSceneConfig::gravity;
	_gvavityContribution = 0.0f;
	_vGravityContributionDir = NxVec3(0, 0, 1);

	_steeringAVel = 8.0f;								
	_steeringState = 0.0f;								

	_extents = NxVec3(0.25f, 0.25f, 0.125f);			

	_vNormalGrd = NxVec3(0, 0, 1);
	_hasContactGrd = 0;
	_driftingAngularAccel = NxPi/180.0f * 90.0f;		

	_isDrifting = false;
	_driftingAngle = 0.0f;								
	_driftingAngleCur = 0;								
	_driftingSideAngleCur = 0.0f;						
	_driftingSideAngleMax = 0.0f;

	_driftingTime = 0.0f;
	_driftingType = 0;
	_driftDir = 0;

	_velDriftingGlobal = NxVec3(0,0,0);					

	_wheelRotAngle = 0.0f;								

	_rotateTotal.id();								
	m_mt3DModel.MakeIdentity();


	_nextTrailSlot = 0;
	_bCreateNewTrack = true;

	//ģ�������
	m_bIsBumping = false;
	m_bumpTime.x = 0;
	m_bumpTime.y = 0;
	m_bumpTime.z = 0;
	//~
	_turnAngle = NxPi;									// Ϊ�γ�ʼ��ΪNxPi ? 
    
	_initWheels();


	//��¼���ӵ�ԭʼλ�á�
	m_posWheels[0] = _pCarRoot->GetObjectByName("LF_W")->GetTranslate();
	m_posWheels[1] = _pCarRoot->GetObjectByName("RF_W")->GetTranslate();
	m_posWheels[2] = _pCarRoot->GetObjectByName("LB_W")->GetTranslate();
	m_posWheels[3] = _pCarRoot->GetObjectByName("RB_W")->GetTranslate();



	NxCapsuleControllerDesc desc;
	//NxVec3 tmp			= i==0 ? gStartPos : gStartPos + NxVec3(10.0f, 0.0f, 0.0f);
	desc.position.x		= pos.x;
	desc.position.y		= pos.y;
	desc.position.z		= pos.z;
	desc.radius			= 0.15f;
	desc.height			= 0.15f;
	desc.upDirection	= NX_Z;
	desc.slopeLimit		= cosf(NxMath::degToRad(45.0f));
	desc.skinWidth		= SKINWIDTH;
	desc.stepOffset		= 0.1f;							

	desc.callback		= (NxUserControllerHitReport*)(&gMyReport);


	ControllerManager& gCM = PhysXManager::GetControllerMgr();

	_controller = (NxCapsuleController*)gCM.createController(_nxScene,desc);

	assert(_controller != NULL);


	XUserData* pUD = new XUserData;					// ���ܵ��ڴ�й¶
	pUD->userData = this;
	pUD->_actorType = XUserData::AT_WHEELLESSCAR;


	_controllerActor = _controller->getActor();
	_controllerActor->userData = pUD;
	_controllerActor->setGroup(PhysXManager::CG_VEHICLE);

	m_pCarAccelControl = new CCarAccelControl();
	m_pCarDriftControl = new CDriftControl(this);
	m_bCanMove = false;
	//����ߵ�����ص�
	NxSceneMgr::Get()->GetActiveSceneLevel()->GetPhysXManager()->\
		SetGamePropCollisionReport(&m_kGPReport);
}


WheellessCar::~WheellessCar()
{
	if ( m_pCarAccelControl )
	{
		delete m_pCarAccelControl;
		m_pCarAccelControl = NULL;
	}

	if ( m_pCarDriftControl )
	{
		delete m_pCarDriftControl;
		m_pCarDriftControl = NULL;
	}
}

void WheellessCar::_initWheels()
{
	for (int i = 0; i < 5; i++)
		_wheels[i] = new CWheel(_nxScene, this);

	//NxReal radius = 0.12f;
	NxVec3 pos;

	pos = NxVec3(_extents.x, _extents.y, 0);//-_extents.z);
	_wheels[WHEEL0_INDEX]->setPos(pos);

	pos = NxVec3(_extents.x, -_extents.y, 0);//-_extents.z);
	_wheels[WHEEL1_INDEX]->setPos(pos);

	pos = NxVec3(-_extents.x, -_extents.y, 0);//-_extents.z);
	_wheels[WHEEL2_INDEX]->setPos(pos);

	pos = NxVec3(-_extents.x, _extents.y, 0);//-_extents.z);
	_wheels[WHEEL3_INDEX]->setPos(pos);

	pos = NxVec3(0,0,0);
	_wheels[4]->setPos(pos);

}

NxReal WheellessCar::GetGriftingAngleVel()
{
	if ( abs( _velocityLocal.x) < 0.001 )
		return 0.0f;
	else
	{
		float vel = (float)_velocityLocal.x;
		NxReal AngleVel = ((float)pow( abs(vel) ,0.8f) / 2.0f + 0.1f) * NxMath::sign(_velocityLocal.x) * NxMath::sign(m_fSteer);
		return AngleVel;
		//	return __min( AngleVel,NxPi * 2);
	}

}

NxReal WheellessCar::GetNormalAngleVel()
{
	if ( abs( _velocityLocal.x) < 0.001 )
		return 0.0f;
	else
	{
		float vel = (float)_velocityLocal.x;
		if ( NxMath::sign(_velocityLocal.x) < 0)
		{
			int p = 3;
			//assert(0);
		}
		NxReal AngleVel = ((float)pow( abs(vel) ,0.25f) / 2.0f + 0.1f)  * NxMath::sign(_velocityLocal.x) * NxMath::sign(m_fSteer);
		return AngleVel;
	//	return __min( AngleVel,NxPi * 2);
	}

}


NxReal WheellessCar::GetForwardAccel()
{
	NxMat33 matGlobal = _rotateTotal;
	NxVec3  forward  = matGlobal * NxVec3(1.0f, 0, 0);

	NxReal  acceleration = 9.5f;

	if ( NxMath::acos(_vGravityContributionDir.dot(forward)) > (NI_PI/2.0f))
		acceleration = m_pCarAccelControl->PeekCarAccel(_velocityLocal.x,_gravity * _gvavityContribution);
	else
		acceleration = m_pCarAccelControl->PeekCarAccel(_velocityLocal.x,-_gravity * _gvavityContribution);
	return acceleration;
}



void WheellessCar::_controlNormalState(float accel, float steer, NxReal dt, bool keyShift, bool keyUp, bool keyLeft, bool keyRight, bool enableInput)
{

	//	velLocal.y = 0.0f;

	//TODO:...
	//��ʱ����
#ifndef _DEBUG
	if (dt > 0.1f)
		dt = 0.001;
#endif


	NxMat33 matGlobal = _rotateTotal;

	NxReal  acceleration = GetForwardAccel();

	if (!m_bCanMove) 
	{
		_velocityLocal.x = 0;
		_acceleration    = 0;
	}

	NxVec3 velLocal = _velocityLocal;

	NxVec3 velGlobal = matGlobal * velLocal;

	if(enableInput)
	{
		if (_hasContactGrd == CONTACTGRD)
		{
			
			velLocal.x += (dt * acceleration); 
			velLocal.z = -0.3f;//dt * _gravity * _gvavityContribution;
			velLocal.y = 0.0f;

			velGlobal = matGlobal * velLocal;
		}
		else if (_hasContactGrd == NONE_CONTACTGRD)
		{
			velGlobal.z += dt* _gravity;
		}
		//NiOutputDebugString("�ڡ����������������ա���������������\n");

		NxReal _carRot = 0.0f;

		//steering
		if ((NxMath::abs(steer) > 0.001f) && (NxMath::abs(velLocal.x) > 1.0f))
		{
/*
			float coefficient = 1.0f - 0.95f *velLocal.magnitude()/(1.0f+velLocal.magnitude());

			if (steer * _steeringState < 0.0f)
				_steeringState = 0.0f;

			_steeringState += NxMath::sign(steer)*( dt* _steeringAVel);
			_steeringState = NxMath::clamp(_steeringState, 1.0f, -1.0f);
*/
			_carRot =  (GetNormalAngleVel() * dt) ;

			_turnAngle += _carRot;
			if (NxMath::abs(_turnAngle) > NxPi*2.0f)
				_turnAngle = ::fmodf(_turnAngle, NxPi* 2.0f);

			NxMat34 matRot;
			matRot.M.rotZ(_carRot);//!!!!!!!!!!!!!!!!!!!!!!!!

			NxMat34 matCar(_controllerActor->getGlobalPose());
			matCar = matCar * matRot;

			//_controllerActor->setGlobalPose(matCar);


		}
		else
		{
			_steeringState = 0.0f;
		}
	}


	if (enableInput){

	//�ָ�Ư����б����
		if (_driftingSideAngleCur > 0.0f)
		{
			_driftingSideAngleCur -= (NxPi/180.0f * DRIFT_SIDE_ANGLE * dt *4.0f);
			if (_driftingSideAngleCur < 0.0f)
				_driftingSideAngleCur = 0.0f;
		}
	}



	NxMat33 matInverse = matGlobal;
	if (!matInverse.getInverse(matInverse))
		return;
	velLocal = matInverse * velGlobal;

	NxVec3 displacement = velGlobal *dt;


	NxF32 sharpness = 1.0f;
	//if (_collisionFlags & NXCC_COLLISION_DOWN)
	//	velGlobal.z = 0.0f;

	_velocityLocal = matInverse * velGlobal;
	_velocityLocal.y = 0.0f;

	//�������ӵ�ת��
	if (velLocal.magnitude() > 0.01f){
		NxReal coeff = velLocal.magnitude()/ (1.0f + velLocal.magnitude());
		_wheelRotAngle -= NxMath::sign(_velocityLocal.x)* NxPi * dt * coeff * coeff * 5.0f;
		if (NxMath::abs(_wheelRotAngle)> 2.0f * NxPi)
			_wheelRotAngle = fmodf(_wheelRotAngle, NxPi*2.0f);
	}
	//~

	_controller->move(displacement, PhysXManager::CG_ENVIRONMENT, 0.000001f, _collisionFlags, sharpness);
//#define _DEBUG_POSE
#ifdef _DEBUG_POSE
	NxExtendedVec3 pos = _controller->getPosition();
	static char buf[256];
	sprintf(buf, "\n dt: %f, _hasContactGrd: %d, car pos: %f, %f, %f  \n velGlobal: %f, %f, %f |velLocal: %f, %f, %f\n", dt, _hasContactGrd, pos.x, pos.y, pos.z, velGlobal.x, velGlobal.y, velGlobal.z, velLocal.x, velLocal.y, velLocal.z);
	NiOutputDebugString(buf);
#endif

}



void WheellessCar::control(float accel, float steer, NxReal dt, bool keyShift, bool keyUp, bool keyLeft, bool keyRight)
{
	m_fSteer = steer;
	_hasContactGrd = _caculateContactInfo(_vNormalGrd);

	m_pCarDriftControl->DriftControlUpdate(keyShift,keyUp,keyLeft,keyRight,dt);

	if (m_pCarDriftControl->GetDriftStatus() != CDriftControl::DRIFT_START)
	{
		_controlNormalState(accel, steer, dt, keyShift, keyUp, keyLeft, keyRight);
		_isDrifting = false;
	}
	else
	{
		_isDrifting = true;
	}

	SynchronizeModel(dt);
}


NxVec3 WheellessCar::getGlobalForward()
{
	return _rotateTotal* getForward();
}
NxVec3      WheellessCar::getGlobalUp()
{
	return _rotateTotal* getUp();
}


void WheellessCar::SynchronizeModel(float dt, bool hostPlayer)
{

	if (!_controller)
		return;

	//ͬ����ʾ/����
	NxMat34 posWheel0(_controllerActor->getGlobalPose());

	NxF32 matWheel0[16];
	posWheel0.getRowMajor44(matWheel0);
	NiMatrix3 mx3;// = _rotateTotal;
	NiPhysXTypes::NxMat33ToNiMatrix3(_rotateTotal, mx3);






	NiMatrix3 mxY90;
	mxY90.MakeZRotation(-90.0f/180.0f * 3.1414926f);


	NiAVObject* pWs[4];
	pWs[0] = _pCarRoot->GetObjectByName("LF_W");
	pWs[1] = _pCarRoot->GetObjectByName("RF_W");
	pWs[3] = _pCarRoot->GetObjectByName("LB_W");
	pWs[2] = _pCarRoot->GetObjectByName("RB_W");


	NiAVObject* pChassis = _pCarRoot;
	if (!pChassis)
		return;


	if (pChassis)
	{
		NiPoint3 ptTrans(matWheel0[3], 
			matWheel0[7],
			matWheel0[11]);

		ptTrans -= (mx3 * NiPoint3(0,0,1)* 0.310f);

		mx3 = mx3 * mxY90;// * mx3Drift;



		//��3Dģ��������ģ��ͬ��
		//RotMeTender(m_mt3DModel, mx3, 1.0f);
		if (hostPlayer)
			UpdateWheelsBump(mx3, dt);



		pChassis->SetRotate(m_mt3DModel);
		pChassis->SetTranslate(ptTrans);


		NiMatrix3 rotX;
		rotX.MakeXRotation(_wheelRotAngle);


		//�ܵ�����λ�ƣ�
		NiPoint3 offsetTotal[WHEEL_COUNT];//

		for (int num = 0; num < WHEEL_COUNT; ++num)
		{
			offsetTotal[num].x = 0.0f;
			offsetTotal[num].y = 0.0f;
			offsetTotal[num].z = 0.0f;
		}
		
		//���������µ���ʱ��λ��
		float bumpOffs[WHEEL_COUNT];
		if (GetBumpOffset(bumpOffs))
		{
			for (int j = 0; j < WHEEL_COUNT; ++j)
			{
				offsetTotal[j].x = 0.0f;
				offsetTotal[j].y = 0.0f;
				offsetTotal[j].z = bumpOffs[j];
			}
		}


		if (hostPlayer){

			////Ư��ʱ���ӵ������ƶ�
			//float woffs[4]={0.0f};
			//if (_isDrifting)
			//{
			//	float offsetup = NxMath::sin(_driftingSideAngleCur)* 0.1f;
			//	if (_driftDir == DRIFT_LEFT)
			//	{
			//		woffs[0] = woffs[2] = -offsetup;
			//		woffs[1] = woffs[3] = offsetup;
			//	}
			//	else
			//	{
			//		woffs[0] = woffs[2] = offsetup;
			//		woffs[1] = woffs[3] = -offsetup;
			//	}
			//}

			////ת��ʱ���ֵ�ǰ��ڶ�
			//NxReal woffsX[2] = {0.0f};
			//if (NxMath::abs(_steeringState) > 0.001f)
			//{
			//	float offsetX = NxMath::abs(_steeringState) * 0.02f;
			//	if (_steeringState > 0.0f)
			//	{
			//		woffsX[0] = -offsetX;
			//		woffsX[1] = offsetX;
			//	}
			//	else
			//	{
			//		woffsX[0] = offsetX;
			//		woffsX[1] = -offsetX;
			//	}
			//}
			////~ǰ��ڶ�

			for (int i = 0; i < NUM_WHEELS-1; i++)
			{

				pWs[i]->SetRotate(rotX);

				NiPoint3 pos(m_posWheels[i]);
				pos.z += offsetTotal[i].z;

				//if (_velocityLocal.magnitude() > 0.1)
					pWs[i]->SetTranslate(pos);

				//pWs[i]->SetScale(2.0f);

			}

		}


	}

	pChassis->Update(0.0f,false);	
}

void WheellessCar::_sortOffset(NiPoint3* offsets, int num)
{
	NiPoint3 temp;
	for (int i = 0; i < num -1; i++)
	{
		for (int j = 0; j < num - i-1; j++)
		{
			if (offsets[j].z < offsets[j+1].z)
			{
				temp = offsets[j];
				offsets[j] = offsets[j+1];
				offsets[j+1] = temp;
			}
		}
	}
}

void WheellessCar::_ajustPose()
{

}
void WheellessCar::_wheelRayCast()
{
	for (int i = 0; i < 5; i++)
	{
		_wheels[i]->rayCast();
	}
}

/*---------------------------------------------------------------
��������Ĵ������� 

ÿ��� Update   ���ص��� WheellessCar::Control() ����, ����������һ�崦��.
���� Control() �����ж�����ʲô.

	1. ���� �ر�Ӵ���Ϣ _caculateContactInfo()
	2. ͬ��ģ�� SynchronizeModel()		 (? ���ڵڶ����Ƚ����)
	3. Ư��״̬�ж�_UpdateDriftingState()   
	   ( ����Ư��״̬ ���ⲽ��������ȫ��Ư���ٶ� ,��ʵ���Ǳ��ص��н������ٶ�* ����任��) ( ? Ư�Ƶ�ʱ���ٶȲ����ٵ���??)
	4. Ư�ƴ�����_controlDriftingState()
	5. ����״̬��ʹ�Ĵ����� _controlNormalState();


������ ��ϸһЩ һ������ 

	����ر�Ӵ���Ϣ


----------------------------------------------------------------*/



int WheellessCar::_caculateContactInfo(NxVec3& normal)
{
	_wheelRayCast();

	//�жϳ��Ƿ���Ե���,��ֹ���ܵ���ֱ�Ļ�����ȥ
	/*for (int i = 0; i < NUM_WHEELS - 1; i++)
	{
		if (_wheels[i]->_nxShape)
		{
			XUserData* pUD = (XUserData*)_wheels[i]->_nxShape->getActor().userData;

			if ((pUD->_actorType != XUserData::AT_TERRAIN)) 
			{
				NxVec3 velGlobal = _rotateTotal* _velocityLocal;
				_velocityLocal *= -1.00f;
				return NONE_CONTACTGRD;
			}
		}
	}*/
	_prePos = _controller->getPosition();


	//TODO: ���ݽӴ���Ĳ�ͬ������������̬

	NxVec3 velGlobal = _rotateTotal* _velocityLocal;
	if (((_wheels[0]->_contactType  != CWheel::CT_NEAR)  ||
		 (_wheels[1]->_contactType  != CWheel::CT_NEAR)  ||
		 (_wheels[2]->_contactType  != CWheel::CT_NEAR)  ||
		 (_wheels[3]->_contactType  != CWheel::CT_NEAR)
		) && 
		velGlobal.z > 0.0f)
		return NONE_CONTACTGRD;


	NxVec3 dirUp = CaculateTerrainNormal();
	
	
	//---���ݵ��� ����������������̬���Գ���λ�ý������°���

	NxVec3 vZ = NxVec3(0,0,1);
	vZ.normalize();
	NxVec3 axisRot = vZ.cross(dirUp);
	axisRot.normalize();
	NxReal alpha = NxMath::acos(vZ.dot(dirUp));  // �����µ���ת�Ƕ�


	NxMat33 rotAlpha;
	NiMatrix3 niRot;
	niRot.MakeRotation(-alpha , NiPoint3(axisRot.x, axisRot.y, axisRot.z));
	NiPhysXTypes::NiMatrix3ToNxMat33(niRot, rotAlpha);

	NxReal totalAngle = m_pCarDriftControl->GetDriftingAngle() + _turnAngle;

	NxMat33 rotZ;
	rotZ.rotZ(totalAngle);
	rotAlpha = rotAlpha * rotZ;



	NxMat33 rotDirftSide;
	rotDirftSide.rotX(_driftingSideAngleCur * (-m_pCarDriftControl->GetDriftDir()));

	rotAlpha  = rotAlpha * rotDirftSide;


	_rotateTotal = rotAlpha;


	//������������
//	_gvavityContribution = NxMath::sin(alpha);
	_vNormalGrd = dirUp;

	//NxVec3 dirGravity = NxVec3(0.0f, 0.0f, -1.0f);
	//NxVec3 dirGround = -_vNormalGrd;
	//dirGround = dirGround * NxMath::cos(alpha);
	//_vGravityContributionDir = dirGravity - dirGround;
	//_vGravityContributionDir.normalize();

	NxVec3 dirFwd = this->getGlobalForward();
	if (dirFwd.z > 0.0f)//��ͷ�����ϳ���
	{
		_vGravityContributionDir = _rotateTotal * NxVec3(-1.0f, 0.0f, 0.0f);
	}
	else
	{
		_vGravityContributionDir = _rotateTotal * NxVec3(1.0f, 0.0f, 0.0f);
	}

	//float fThetaG = NxMath::acos(_vGravityContributionDir.dot(NxVec3(0.0f, 0.0f, -1.0f)));
	_gvavityContribution = _vGravityContributionDir.dot(NxVec3(0.0f, 0.0f, -1.0f));


	normal = dirUp;

	bool bContactGround = false;
	for (int i = 0; i < 5; i++)
	{
		if (_wheels[i]->_contactType == CWheel::CT_NEAR)
		{
			bContactGround = true;
		}
	}

	if (bContactGround)
		return CONTACTGRD;//contact the ground
	else
		return NONE_CONTACTGRD;//not
}



void WheellessCar::applyCounterforce(const NxControllerShapeHit& hit)
{
	NxVec3 vecZ(1, 0, 0.0f);
	NxVec3 carDir = this->getGlobalForward();
	carDir.normalize();
	carDir = -carDir;

	NxVec3 wall(hit.worldNormal);
	wall.normalize();

	NxReal delta = acos(carDir.dot(wall));


	if ( m_pCarDriftControl->GetDriftStatus() == CDriftControl::DRIFT_START )
	{
		NxVec3 dirU = wall.cross(carDir);

		NxMat33 rotz;
		
		if ( delta > NxPi/2 )
			delta = NxPi - delta;
		
		rotz.rotZ(NxMath::sign(dirU.z) * abs(NxPi - delta));
		_velocityLocal = rotz * _velocityLocal ;
		_velocityLocal *= ContactConfig::inversevelocityrate;
		return;
	}

	//if ( delta < NxMath::degToRad(30.0f))//���ų������򵯻���
	if ( delta < NxMath::degToRad(ContactConfig::smallangle) )
	{

#ifdef _DEBUG_APPLYFORCE
		NiOutputDebugString("������������������\n");
#endif
		m_pCarDriftControl->EndDrift();
		_isDrifting = false;
//		_velocityLocal.x *= -0.50f;
		_velocityLocal.x *= ContactConfig::inversevelocityrate;
	}
	//else if (delta > NxMath::degToRad(30.0f) && delta < NxMath::degToRad(85.0f))
	else if ( delta < NxMath::degToRad(ContactConfig::midangle) )
	{
		NxReal coeff = _velocityLocal.magnitude()/ (1.0f + _velocityLocal.magnitude());

		NxVec3 dirU = wall.cross(carDir);
		_turnAngle += NxMath::sign(dirU.z)* NxMath::degToRad(2.0f + 5.0f * coeff);

		_velocityLocal *= (1.0f - 0.2f* coeff);

		//////////////////////////////////////////////////////////////////////////
		// ����Ư�ƣ�
	}
	//else if(delta > NxPi * 2.f/3.f) // ��ƨ��
	else if ( delta > NxMath::degToRad(ContactConfig::bigangle) )
	{
		m_pCarDriftControl->EndDrift();
		_isDrifting = false;

#ifdef _DEBUG_APPLYFORCE
		NiOutputDebugString("��ƨ�ɣ���������\n");
#endif
//		_velocityLocal.x *= -0.50f;
		_velocityLocal.x *= ContactConfig::inversevelocityrate;
	}

}

void WheellessCar::handleContactPair(NxContactPair& pair, NxU32 carIndex, NxU32 event)
{
	//return;
	XUserData * pD = (XUserData*)pair.actors[1-carIndex]->userData;

	if (pD->_actorType == XUserData::AT_TERRAIN)
	{
		//AdjustPose(pair, carIndex, event);
	}
	else if (pD->_actorType == XUserData::AT_FENCE_MANUAL||pD->_actorType == XUserData::AT_FENCE_GAMEBRYO)
	{
		//HandleCollision(pair, carIndex, event);
	}

}

void WheellessCar::_bumpWheeels(NxReal dt)
{
	for (int i = 0; i < NUM_WHEELS; i++)
	{
		_wheels[i]->bump(dt);
	}
}

void WheellessCar::SetPose(const NxMat34& kPose)
{
	//TODO:�ı䳵�ĳ���
	//	

	_hasContactGrd = NONE_CONTACTGRD;

	NxExtendedVec3 position(kPose.t.x, kPose.t.y, kPose.t.z);// + 0.520f);
	_controller->setPosition(position);

	NxExtendedVec3 pos = _controller->getPosition();

	_velocityLocal.zero();


}
void WheellessCar::SetPos(const NxVec3& pos)
{
	_hasContactGrd = NONE_CONTACTGRD;
	NxExtendedVec3 position(pos.x, pos.y, pos.z);
	_controller->setPosition(position);
}
NxVec3 WheellessCar::GetPos()
{
	NxExtendedVec3 position = _controller->getPosition();
	NxVec3 pos(position.x, position.y, position.z);
	return pos;

}


NxReal WheellessCar::GetVelocity()
{
	//if (_isDrifting)
	//	return _velDriftingGlobal.magnitude();
	//else
		return _velocityLocal.magnitude();
}
void WheellessCar::setVelocityLocal(const  NxVec3& vel)
{
	_velocityLocal = vel;
}
NxVec3 WheellessCar::GetVelocityLocal()
{
	return _velocityLocal;
}
void WheellessCar::setAccelTime(NxReal fTime, NxReal fGear)
{
	m_accelTime = fTime;
}
bool WheellessCar::IsInHell()
{
	bool ret = true;
	for (int i = 0; i < NUM_WHEELS; i++)
	{
		if (_wheels[i]->_contactType != CWheel::CT_NONE)
		{
			ret = false;
			break;
		}
	}
	return ret;
}

void WheellessCar::RemoteCarUpdate(float dt)
{
	_caculateContactInfo(NxVec3(0,0,0));
	SynchronizeModel(dt, false);

	if (_isDrifting)
	{
		;//_controlDriftingState(false, false, false, false, dt, true);
	}
	else
	{
		_controlNormalState(0.0, 0.0, dt, false, false, false, false, true);
	}
}
bool WheellessCar::IsDrifting()
{
	if( m_pCarDriftControl->GetDriftStatus() == CDriftControl::DRIFT_START )
		return true;
	return false;
}
void WheellessCar::SetDrifting(bool bDrift)
{
	_isDrifting = bDrift;
	// Add by wincoqin
	if (!bDrift)
		m_pCarDriftControl->EndDrift();
}
NxVec3 WheellessCar::GetVelDrifting()
{
	return _velDriftingGlobal;
}
void WheellessCar::SetVelDrifting(const NxVec3& vel)
{
	_velDriftingGlobal = vel;
}
void WheellessCar::SetGlobalOrientation(NxMat33 ori)
{
//	_controllerActor->setGlobalOrientation(ori);
	_rotateTotal = ori;
}

NxVec3 WheellessCar::GetTerrainNormal()
{
	if (_wheels[4]->_nxShape)
	{
		return _wheels[4]->_curContactN;
	}
	else
	{
		return NxVec3(0, 0, 1);
	}
}

void WheellessCar::ProcessControllersHit(const NxControllersHit& hit)
{
	WheellessCar* oneCar = NULL;
	WheellessCar* otherCar = NULL;

	assert(hit.controller);	

	XUserData* pUdOneCar = (XUserData*)hit.controller->getActor()->userData;
	if (pUdOneCar)
		oneCar = (WheellessCar*)pUdOneCar->userData;


	if (!hit.other)
	{
		otherCar = NxPlayMgr::GetNearPlayFromHostPlay()->GetKart()->GetVehicle();
		if (!otherCar)
			return;
	}
	else
	{
		XUserData* pUD = (XUserData*)hit.other->getActor()->userData;
		if (pUD)
			otherCar = (WheellessCar*)pUD->userData;
	}

	if (oneCar && otherCar)
	{
		//one car
		NxVec3 dirCenter = (otherCar->GetPos() - oneCar->GetPos());
		dirCenter.normalize();
		NxVec3 fwOneCar = oneCar->getGlobalForward();
		NxReal theta = NxMath::acos(fwOneCar.dot(dirCenter));

		NxVec3 vel = oneCar->GetVelocityLocal();
		NxVec3 vel2 = otherCar->GetVelocityLocal();
		float dir = 0.0f;
		float ftemp = fwOneCar.cross(dirCenter).z;
		if (ftemp > 0.01f)
			dir = 1.0f;
		else if (ftemp < -0.01f)
			dir = -1.0f;		

		if (theta < NxMath::degToRad(DescConstant::KartCollisionParam.m_fAlpha))
		{			
			vel.x = -NxMath::sign(vel.x) * fabs(vel.x - vel2.x) * DescConstant::KartCollisionParam.m_fBackSpeedCoef;
			if (fabs(vel.x) < 1.0f) 
			{
				vel.x -= NxMath::sign(vel.x) * 10.f;
			}
			oneCar->setVelocityLocal(vel);
		}
		else if (theta < NxMath::degToRad(DescConstant::KartCollisionParam.m_fAlpha1))
		{
			// ����
			vel.x *= vel.x/(1+vel.x) * ( 1.0f - DescConstant::KartCollisionParam.m_fLinearSpeedLostRate);
			oneCar->SetTurnAngle(NxMath::radToDeg(dir * DescConstant::KartCollisionParam.m_fTurnSpeed));
		}
		// ����Ư������ж�
		/*		
		else if ( theta < NxMath::degToRad(DescConstant::KartCollisionParam.m_fAlphaDrift1))
		{
		// Drift
		}
		*/

		else if (theta < NxMath::degToRad(DescConstant::KartCollisionParam.m_fAlpha2))
		{
			vel.x *= vel.x/(1+vel.x) * ( 1.0f - DescConstant::KartCollisionParam.m_fLinearSpeedLostRate);
			oneCar->SetTurnAngle(NxMath::radToDeg(dir * DescConstant::KartCollisionParam.m_fTurnSpeed));
		}
		else
		{
			vel.x *= NxMath::sign(vel.x) * fabs(vel.x - vel2.x) * DescConstant::KartCollisionParam.m_fForwardSpeedCoef;
			if (fabs(vel.x) < 1.0f) 
			{
				vel.x += NxMath::sign(vel.x) * 10;
			}
			oneCar->setVelocityLocal(vel);
		}

		otherCar->setVelocityLocal(NxVec3(0, 0, 0));

	}

}



void WheellessCar::StopDrift()
{
	_isDrifting = false;
	// Add by wincoqin
	_driftingAngle   = 0;
	_driftingAngleCur = 0;
}

//�𽥽�������ת�����ƽ��
void WheellessCar::RotMeTender(NiMatrix3& mtSrc, const NiMatrix3& mtDest, float dt)
{
	//��3Dģ��������ģ��ͬ��
	float fXAnglePhs,fYAnglePhs,fZAnglePhs;
	mtDest.ToEulerAnglesZXY(fZAnglePhs, fXAnglePhs, fYAnglePhs);

	float fXAngle,fYAngle,fZAngle;
	m_mt3DModel.ToEulerAnglesZXY(fZAngle, fXAngle, fYAngle);


	//TODO: ���ݽǶȲ�ֵ�Ĵ�С�����������ĽǶ�
	NiMatrix3 mtTemp;
	mtTemp.MakeZRotation(fZAnglePhs);
	NiMatrix3 rotXTmp;
	rotXTmp.MakeXRotation(fXAngle + (fXAnglePhs- fXAngle)*0.3f );
	NiMatrix3 rotYTmp;
	rotYTmp.MakeYRotation(fYAngle + (fYAnglePhs- fYAngle)*0.3f);

	mtTemp = mtTemp * rotXTmp * rotYTmp;

	mtSrc = mtTemp;

}
//���ӳ����ŵط����仯ʱ�Ķ���
void WheellessCar::UpdateWheelsBump(const NiMatrix3& matDest, float dt)
{
	//��ǰ����
	if (!m_bIsBumping)
	{
		//��⣬����Ҫ�����һ������
		m_bIsBumping = GenerateBump();

		if (!m_bIsBumping)
			RotMeTender(m_mt3DModel, matDest, dt);

	}
	else
	{
		//�����û�н���
		BumpMeTender(matDest, dt);
        
	}
}

void WheellessCar::BumpMeTender(const NiMatrix3& matDest, float dt)
{

	float rotX = 0.0f;
	float rotY = 0.0f;

	if (m_bumpTime.x > 0)
	{
		
		if (NxMath::abs(m_vBumpAngleCur.x + m_vBumpAngularVel.x * dt) 
			< NxMath::abs(m_vBumpAngle.x))
		{
			//TODO:������ӷ����������
			//~
			rotX = m_vBumpAngularVel.x * dt;
			m_vBumpAngleCur.x += rotX;

		}
		else
		{
			m_bumpTime.x--;
			m_vBumpAngle.x *= -0.8f;
			m_vBumpAngleCur.x = 0.0f;
			m_vBumpAngularVel *= -0.8f;
		}

	}

	if (m_bumpTime.y > 0)
	{

		if (NxMath::abs(m_vBumpAngleCur.y + m_vBumpAngularVel.y * dt) 
			< NxMath::abs(m_vBumpAngle.y))
		{
			//TODO:������ӷ����������
			//~
			rotY = m_vBumpAngularVel.y * dt;
			m_vBumpAngleCur.y += rotY;

		}
		else
		{
			m_bumpTime.y--;
			m_vBumpAngle.y *= -0.8f;
			m_vBumpAngleCur.y = 0.0f;
			m_vBumpAngularVel *= -0.8f;
		}

	}
	//��3Dģ��������ģ��ͬ��
	float fXAnglePhs,fYAnglePhs,fZAnglePhs;
	matDest.ToEulerAnglesZXY(fZAnglePhs, fXAnglePhs, fYAnglePhs);

	float fXAngle,fYAngle,fZAngle;
	m_mt3DModel.ToEulerAnglesZXY(fZAngle, fXAngle, fYAngle);


	NiMatrix3 mtTemp;
	mtTemp.MakeZRotation(fZAnglePhs);
	NiMatrix3 rotXTmp;
	rotXTmp.MakeXRotation(fXAngle);
	NiMatrix3 rotYTmp;
	rotYTmp.MakeYRotation(fYAngle);

	mtTemp = mtTemp * rotXTmp * rotYTmp;


	NiMatrix3 mtRotX;
	mtRotX.MakeXRotation(rotX);
	NiMatrix3 mtRotY;
	mtRotY.MakeYRotation(rotY);


	m_mt3DModel = mtTemp * mtRotX * mtRotY;


	if (m_bumpTime.x == 0 &&  m_bumpTime.y == 0)
	{
		m_bIsBumping = false;
	}

}
bool WheellessCar::GenerateBump()
{
	//TODO:ȷ�����ٶȷ������������
	int nWheelA, nWheelB;
	if (_velocityLocal.x > 0.0f)
	{
		nWheelA = WHEEL0_INDEX;
		nWheelB = WHEEL1_INDEX;
	}
	else
	{
		nWheelA = WHEEL2_INDEX;
		nWheelB = WHEEL3_INDEX;
	}

	BUMPINFO infoA, infoB;
	_wheels[nWheelA]->GetBumpInfo(infoA);
	_wheels[nWheelB]->GetBumpInfo(infoB);

	float fCoeff = _velocityLocal.magnitude()/ 15.0f;

	fCoeff = NxMath::clamp(fCoeff, 1.0f, 0.5f);
	float fDumpeAngle = NxMath::degToRad(5.0) * fCoeff;
	float fAngularVel = NxMath::degToRad(15.0f) * fCoeff;

	if (infoA.nPitch == 1 && infoB.nPitch == 1)
	{
		m_bumpTime.x = 2;
		m_vBumpAngle.x = fDumpeAngle;
		m_vBumpAngularVel.x = fAngularVel;
		m_vBumpAngleCur.x = 0.0f;

	}
	else if (infoA.nPitch == -1 && infoB.nPitch == -1)
	{
		//����
		m_bumpTime.x = 2;
		m_bumpTime.y = 0;
		m_vBumpAngle.x = -fDumpeAngle;//NxMath::degToRad(-8.0f);
		m_vBumpAngularVel.x = -fAngularVel;//NxMath::degToRad(-25.0f);
		m_vBumpAngleCur.x = 0.0f;

	}
	else if (infoA.nPitch != 0)
	{
		float coeff = (nWheelA == WHEEL0_INDEX)? 1.0f : -1.0f;
		//��ǰ��		
		m_bumpTime.x = 2;
		m_vBumpAngle.x = fDumpeAngle * coeff;//NxMath::degToRad(8.0f * coeff);
		m_vBumpAngularVel.x = fAngularVel* coeff;//NxMath::degToRad(25.0f * coeff);
		m_vBumpAngleCur.x = 0.0f;

		m_bumpTime.y = 2;
		m_vBumpAngle.y = -fDumpeAngle* coeff;
		m_vBumpAngularVel.y = -fAngularVel* coeff;
		m_vBumpAngleCur.y = 0.0f;


	}else if (infoB.nPitch != 0)
	{
		float coeff = (nWheelA == WHEEL1_INDEX)? 1.0f : -1.0f;
		//��ǰ��
		m_bumpTime.x = 2;
		m_vBumpAngle.x = fDumpeAngle * coeff;//NxMath::degToRad(8.0f * coeff);
		m_vBumpAngularVel.x = fAngularVel* coeff;//NxMath::degToRad(25.0f * coeff);
		m_vBumpAngleCur.x = 0.0f;

		m_bumpTime.y = 2;
		m_vBumpAngle.y = fDumpeAngle* coeff;
		m_vBumpAngularVel.y = fAngularVel* coeff;
		m_vBumpAngleCur.y = 0.0f;

	}
	else
		return false;

	//return true;
}

bool WheellessCar::GetBumpOffset(float* pfOffsets)
{
	if (!m_bIsBumping)
		return false;


	NxMat33 rot;
	rot.id();

	if (m_bumpTime.x > 0)
	{
		NxMat33 rotX;
		rotX.rotY(m_vBumpAngleCur.x);
		rot = rot * rotX;
	}

	if (m_bumpTime.y > 0)
	{
		NxMat33 rotY;
		rotY.rotX(m_vBumpAngleCur.y);
		rot = rot * rotY;
	}

	NxMat34 trans(rot, NxVec3(0.0f, 0.0f, 0.0f));

	for (int i = 0; i < WHEEL_COUNT; ++i)
	{
		NxVec3 pos = trans* (_wheels[i]->_posLocal);
		pfOffsets[i] = -pos.z;
		//Log("%d : %f\n", i, pos.z);
	}

	//Log("angle [%f] [%f] , offset : [%f] [%f] [%f] [%f]\n", m_vBumpAngleCur.x, m_vBumpAngleCur.y, pfOffsets[0],pfOffsets[1],pfOffsets[2],pfOffsets[3]);
}

GPPointReport::OnTrigger(LPVOID lpGPData,LPVOID lpHitObjData)
{
	//���ߺ���ײ����
	CGPPoint* pGpTmp=CGPPoint::GetPoint((USHORT)lpGPData);
	//_ASSERTE(pGpTmp);
	//WheellessCar* pWcTmp=(WheellessCar*)lpHitObjData;
	//������趨ָ���Ļص����������
	if(pGpTmp && pGpTmp->GetReport())
	{
		pGpTmp->GetReport()->OnTrigger(lpGPData,lpHitObjData);
	}
}
NxVec3 WheellessCar::CaculateTerrainNormal()
{
	NxVec3 dirUp;
	if ((_wheels[2]->_contactType == CWheel::CT_NEAR &&
		_wheels[3]->_contactType  == CWheel::CT_NEAR&&
		_wheels[0]->_contactType == CWheel::CT_NEAR) &&
		( _wheels[2]->_contactedActorType == XUserData::AT_TERRAIN && 
		_wheels[3]->_contactedActorType == XUserData::AT_TERRAIN && 
		_wheels[0]->_contactedActorType == XUserData::AT_TERRAIN  ))
	{
		dirUp = (_wheels[2]->_contactPos - _wheels[0]->_contactPos).cross(
			_wheels[2]->_contactPos - _wheels[3]->_contactPos);
		dirUp.normalize();

	}
	else 
	{
		bool bFound = false;
		for (int i = 0; i < WHEEL_COUNT; ++i)
		{
			if (_wheels[i]->_contactedActorType == XUserData::AT_TERRAIN &&
				_wheels[i]->_contactType == CWheel::CT_NEAR)
			{
				dirUp = _wheels[i]->_curContactN;
				bFound = true;
				break;
			}

		}

		if (!bFound)
		{
			dirUp = NxVec3(0, 0, 1);
		}
	}

	return dirUp;

}
