#include "Top-KartPCH.h"
#include ".\tencentcar.h"
#include "DriftCenter.h"
#include "NxExportedUtils.h"
#include "stringex.h"
#include "CarMotorControl.h"
#include "CarFenceCollision.h"
#include "CarCarCollision.h"
#include "../Play.h"
#include "../DescConstant.h"
#include "../NO2Controller.h"
#include "../gpcontrol.h"
#include "../PowerController.h"
#include "../Monster.h"


#include "../PhysUtility.h"
#include "../MovePkgMgr.h"
#include "../TerrainEffectMgr.h"
#include "Suspension.h"

/*-------------------------------------------------------------------
// Create by HoverZhao 2006.1.23
--------------------------------------------------------------------*/


//-----------------------------------------------------------------------
CTencentCar::CTencentCar(NxScene* pScene, NiAVObject* pModel, Play * pPlay)
: m_pScene( pScene )
, m_pCarModel( pModel )
, m_pPlay(pPlay)
{
	m_vLastVecGlobal = m_vVecGlobal = NxVec3(0.0f,0.0f,0.0f);
	m_vLastWecGlobal = m_vWecGlobal = NxVec3(0.0f,0.0f,0.0f);
	m_arrForcePool.clear();
	m_arrTwistPool.clear();
	m_vGravity				= NxVec3 ( 0.0f, 0.0f, GRAVITYACCEL);
	m_vTotalForce			= NxVec3 ( 0.0f, 0.0f, 0.0f );
	m_vTotalTwist			= NxVec3 ( 0.0f, 0.0f, 0.0f );
	m_vAngleFromGround		= NxVec3 ( 0.0f, 0.0f, 0.0f );
	m_vVecGlobal.zero();
	m_vVecGlobal.zero();

	m_fFricFactor			= DescConstant::KartFriction.m_fFricFactor;		// 0.5f;
	m_fSlipFrictionForce	= DescConstant::KartFriction.m_fSlideFriction;	// 9.5f;
	m_fRollFrictionForce	= DescConstant::KartFriction.m_fRollFriction;	// 0.2f;
	m_fMass					= 1.0f;

	m_fForwardSquard        = 0.21f;
	m_fSideSquare			= 0.35f;
	m_fUpSquare				= 0.5f;
	m_fAirCoefC				= 0.3f;
	m_fAirDensity			= 1.25f;

	m_fOffsetZ				= 0.0f;
	m_fClockTurnAngle		= 0.0f;
	m_fDeltaAlpha			= 0.0f;
	m_fBeltaAlpha			= 0.0f;
	m_fAdjustXAngle			= 0.0f;
	m_fAdjustYAngle			= 0.0f;
	m_fSteeringAccum		= 0.0f;
	m_fSelfRotateAttenuation= 0.70f;
	m_fWheelTurnAngle		= 0.0f;
	m_fMaxWheelTurnAngle    = NxPi / 4.0f;
	m_fWheelTurnVec			= NxPi / 2.0f;
	m_nRotateSideAxis		= ERSA_NONE;
	m_nLastContactStatus    = ECS_INAIR;

	m_dwDeltaTime			= 0;
	m_arrCarWheelPool.clear();
	m_arrPrivotPoint.clear();
	m_matGlobal.id();
	m_pController					= NULL;
	m_pCarMotorControl				= NULL;
	m_pCarCollisionCenter			= NULL;
	m_pCarFenceCollisionProcessor	= NULL;
	m_pCarCarCollisionProcessor		= NULL;
	m_pSimulator					= NULL;
	m_pSuspension					= NULL;
#if defined(_PHYSX_2_7_0)
	//模型震动相关
	m_fHeadAngleAfterDrift = 0.0f;
	m_bIsDrifting = false;
	
	if (pPlay->IsHost())
	{
		m_pSimulator = new ChassisSimulator(pScene);//, NxVec3(10, 10, -10));
	}
#endif

	if (pPlay->IsHost())
	{
		m_pSuspension = new Suspension(this);
	}
	BumpInit();
	m_matGround.id();  // 加上matGround的初始化， by wincoqin, 2007/2/12
	m_fXiaoPenStartTime				= 0.0f;
	m_fXiaoPenLastTime				= 0.0f;
	m_bXiaoPen						= false;
	m_fXiaoPenEffectTime			= 0.0f;
	m_pDriftCenter = NULL;
	m_fOmiga = DescConstant::KartRotate;//m_fOmiga1 = 0.0f;
	m_fOmiga1 = DescConstant::KartRotate1;
	m_fAltTime = 0.0f;
	m_bJumpIsAccumulating = false;

	m_fArmForce	= 0.0f;

	// 碰撞数据
	for(int i = 0; i<8; i++)
	{
		m_vCollisionData[i].id = -1;
		m_vCollisionData[i].collisionTime = 0.0f;
		m_vCollisionData[i].nearCollisionTime = 0.0f;
		m_vCollisionData[i].selfPos = NxVec3(0, 0, 0);
		m_vCollisionData[i].selfVec = NxVec3(0, 0, 0);
		m_vCollisionData[i].otherPos = NxVec3(0, 0, 0);
		m_vCollisionData[i].otherVec = NxVec3(0, 0, 0);
	}
	m_bIsFall		= false;
	m_bIsFallPoseAdjust	= false;

	m_fCarBodyAngle = 0.0f;

	m_fBoxLength = 0.0f;
	m_fBoxWidth	= 0.0f;
	m_fBoxHeight = 0.0f;
	m_fAdjustOffset = 0.35f;
	m_fAdjustBevelOffset = 0.38f;
	m_fAdjustRemoteOffset = 0.41;
	m_fCompensateParam = 1.0f;
	m_fAdjustPickGap = 0.0f;

	m_fGravity	= -20.0f;

	m_fCollisionRadius = 0.0f;
	m_fCollisionLength = 0.0f;
	m_fCollisionWidth = 0.0f;
	m_fCollisionHeight = 0.0f;

	m_fMaxTurnSpeed	= 11.0f;
	m_fMinTurnSpeed = 2.7f;
	m_fBaseTurnRate = 1.05f;
	m_fVolatileTurnRate = 1.25f;

	m_vecComPensateTable.clear();
	m_vLastTerrainNormal = NxVec3(0.0f,0.0f,1.0f);

	//初始化地面法线
	for (int i = 0; i < WHEELS; i++)
	{
		m_aPickNormalsPre[i] = NxVec3(0, 0, 1);
		m_aPickNormalsCur[i] = NxVec3(0, 0, 1);
	}
}

//-----------------------------------------------------------------------
CTencentCar::~CTencentCar(void)
{
	for ( int i = 0 ; i < m_arrCarWheelPool.size(); i++ )
	{
		delete m_arrCarWheelPool[i];
	}
	m_arrCarWheelPool.clear();
	m_arrPhysInfo.clear();
	m_vecComPensateTable.clear();

	SAFE_DELETE(m_pCarMotorControl);
	SAFE_DELETE(m_pCarFenceCollisionProcessor);
	SAFE_DELETE(m_pCarCarCollisionProcessor);
	SAFE_DELETE(m_pCarCollisionCenter);
	SAFE_DELETE(m_pDriftCenter);

	SAFE_DELETE(m_pSimulator);
	ReleaseRaycastCallback();
	ReleaseCarUserData();
	if (m_pSuspension)
	{
		SAFE_DELETE(m_pSuspension);
	}
	PhysXManager::GetControllerMgr().releaseController(*m_pController);

	m_pController = NULL;
}	


//-----------------------------------------------------------------------
bool CTencentCar::InitController( NxVec3 vPos)
{
	SAFE_DELETE(m_pCarCollisionCenter);
	m_pCarCollisionCenter = new CCollisionCenter;


		if( !IsBoxController() )
		{
			NxCapsuleControllerDesc kDesc;
			kDesc.position.x		= vPos.x;
			kDesc.position.y		= vPos.y;
			kDesc.position.z		= vPos.z;
			kDesc.radius			= 0.15f;
			kDesc.height			= 0.15f;

			kDesc.upDirection		= NX_Z;
			kDesc.slopeLimit		= cosf(NxMath::degToRad(45.0f));
			kDesc.skinWidth			= SKINWIDTH;
			kDesc.stepOffset		= 0.2f;			
			// 非本地玩家不用创建此回调
			// TODO: 在此添加本地玩家判断
			if (m_pPlay == NxPlayMgr::GetHostPlay())
			{
				kDesc.callback		= (NxUserControllerHitReport*)m_pCarCollisionCenter;
			}
			else
				kDesc.callback = 0;

			m_vExtents = NxVec3(0.25f, 0.25f,0.0f);	

			ControllerManager& gCM = PhysXManager::GetControllerMgr();

			m_pController = (NxCapsuleController*)gCM.createController(m_pScene,kDesc);
		}
		else
		{
			NxBoxControllerDesc kDesc;
			kDesc.position.x		= vPos.x;
			kDesc.position.y		= vPos.y;
			kDesc.position.z		= vPos.z;
			kDesc.extents			= NxVec3(m_fBoxLength, m_fBoxWidth, m_fBoxHeight);
			//	kDesc.radius			= 0.15f;
			//	kDesc.height			= 0.15f;
			kDesc.upDirection		= NX_Z;
			kDesc.slopeLimit		= cosf(NxMath::degToRad(45.0f));
			kDesc.skinWidth			= SKINWIDTH;
			kDesc.stepOffset		= 0.2f;			
			// 非本地玩家不用创建此回调
			// TODO: 在此添加本地玩家判断
			if (m_pPlay == NxPlayMgr::GetHostPlay())
			{
				kDesc.callback		= (NxUserControllerHitReport*)m_pCarCollisionCenter;
			}
			else
				kDesc.callback = 0;

			m_vExtents = NxVec3(0.25f, 0.25f,0.0f);	

			ControllerManager& gCM = PhysXManager::GetControllerMgr();

			m_pController = (NxBoxController*)gCM.createController(m_pScene,kDesc);

		}
	
	return m_pController != NULL;
}

//-----------------------------------------------------------------------
bool CTencentCar::InitAccelDrift()
{

	m_pDriftCenter = new CDriftCenter(this);

	if (!m_pDriftCenter->InitDriftCenter())
	{
		return false;
	}
	
	return true;
}

//-----------------------------------------------------------------------
bool CTencentCar::InitCarPhysParam()
{
	m_arrPhysInfo.clear();
	
	Play * pPlay = NxPlayMgr::GetHostPlay();
	
	if ( !pPlay )
		return false;
	
	PLAYERDESC &	stPlayDesc  = pPlay->GetCurPlayDesc();
	CStringEx		CarPath		= (const char * ) stPlayDesc.strKartPath ;
	CStringEx		CarFilePath = CarPath + "/KartPhysParam.lua";
	
	
	CVarArray varrPhysParam("KartPhysParam");
	CVar::LoadVarsFromFile(&varrPhysParam, NiApplication::ConvertMediaFilename(CarFilePath.c_str()));
	UINT nNum = varrPhysParam.GetNum();

	for (unsigned int i = 0; i < nNum; i++ )
	{
		PHYSINFO PhysInfo;
		PhysInfo.dwPID		= varrPhysParam.Element(i)->Element("PID")->Integer();
		PhysInfo.dwAffectID = varrPhysParam.Element(i)->Element("AffectID")->Integer();
		PhysInfo.fParam		= varrPhysParam.Element(i)->Element("Param")->Float();
		PhysInfo.fLifeTime	= varrPhysParam.Element(i)->Element("LifeTime")->Float();
		if( PhysInfo.dwPID == DD_JIQI_ID)
		{
			CALL(CPowerController::Get(), GetSinglePower()->SetKartCoef(PhysInfo.fParam));
			//continue;
		}
		m_arrPhysInfo.push_back(PhysInfo);
	}


	CVarArray varCompensateTable("CompensateParam");
	CVar::LoadVarsFromFile(&varCompensateTable, NiApplication::ConvertMediaFilename(CarFilePath.c_str()));
	nNum = varCompensateTable.GetNum();

	for ( UINT k = 0; k < nNum; k++ )
	{
		COMPENSTATEINFO CompInfo;
		CompInfo.fPercent = varCompensateTable.Element(k)->Element("fPercent")->Float();
		CompInfo.fValue   = varCompensateTable.Element(k)->Element("fValue")->Float();
		m_vecComPensateTable.push_back(CompInfo);
	}

	CVarTable varBoxTable("CollisionBoxParam");
	CVar::LoadVarsFromFile(&varBoxTable, NiApplication::ConvertMediaFilename(CarFilePath.c_str()));
	m_fBoxLength	= varBoxTable.Element("BoxLength")->Float();
	m_fBoxWidth		= varBoxTable.Element("BoxWidth")->Float();
	m_fBoxHeight	= varBoxTable.Element("BoxHeight")->Float();
	m_fAdjustOffset = varBoxTable.Element("AdjustOffset")->Float();
	m_fAdjustRemoteOffset = varBoxTable.Element("AdjustRemoteOffset")->Float();
	m_fAdjustPickGap = varBoxTable.Element("AdjustPickGap")->Float();
	m_fAdjustBevelOffset = varBoxTable.Element("AdjustBevelOffset")->Float();

	CVarTable varCollisionTable("CarToCarCollisionParam");
	CVar::LoadVarsFromFile(&varCollisionTable, NiApplication::ConvertMediaFilename(CarFilePath.c_str()));
	m_fCollisionRadius	= varCollisionTable.Element("CollisionRadius")->Float();
	m_fCollisionLength	= varCollisionTable.Element("CollisionLength")->Float();
	m_fCollisionWidth	= varCollisionTable.Element("CollisionWidth")->Float();
	m_fCollisionHeight	= varCollisionTable.Element("CollisionHeight")->Float();
	
	CVarTable varTurnTable("TurnPhysParam");
	CVar::LoadVarsFromFile(&varTurnTable, NiApplication::ConvertMediaFilename(CarFilePath.c_str()));
	m_fBaseTurnRate		= varTurnTable.Element("BaseTurnRate")->Float();
	m_fVolatileTurnRate	= varTurnTable.Element("VolatileTurnRate")->Float();
	m_fMaxTurnSpeed		= varTurnTable.Element("MaxTurnSpeed")->Float();
	m_fMinTurnSpeed		= varTurnTable.Element("MinTurnSpeed")->Float();

	CVarTable varGravityTable("GravityParam");
	CVar::LoadVarsFromFile(&varGravityTable, NiApplication::ConvertMediaFilename(CarFilePath.c_str()));
	m_fGravity		= varGravityTable.Element("Gravity")->Float();
	m_vGravity		= NxVec3(0, 0, m_fGravity);

	if ( m_pCarMotorControl)
	{
		PHYSINFO PhysInfo;
		PhysInfo.dwPID = DD_ACCEL;
		PhysInfo.dwAffectID = 0;
		PhysInfo.fParam = m_pCarMotorControl->GetAcceParam();
		PhysInfo.fLifeTime = 0.0f;
		m_arrPhysInfo.push_back(PhysInfo);
	}
	m_bIsFall	= false;
	m_bIsFallPoseAdjust = false;

	return true;
}

//-----------------------------------------------------------------------
bool CTencentCar::InitCarWheel( NxVec3 vPos)
{
	bool bResult = true;
	for ( int i = 0; i < MAXWHEELNUM ; i++ )
	{
		// 配置选项
		CCarWheel * pWheel = new CCarWheel(this);
		if ( pWheel->Init())
		{
			m_arrCarWheelPool.push_back(pWheel);
		}
		else
		{
			bResult = false;
		}
	}

	//临时代码将来会可以配置的
	if ( (m_arrCarWheelPool.size() == MAXWHEELNUM) ) //&& (m_pPlay->IsHost()))
	{
		NxVec3 vOffsetPos;
		vOffsetPos = NxVec3(m_vExtents.x, -m_vExtents.y,	 m_vExtents.z);
		m_arrCarWheelPool[0]->SetLocalPos(vOffsetPos);
		m_arrCarWheelPool[0]->SetWheelMode(m_pCarModel->GetObjectByName("RF_W"));
		//m_arrCarWheelPool[0]->SetModelPosOri(m_pCarModel->GetObjectByName("RF_W")->GetTranslate());
		m_arrCarWheelPool[0]->SetModelPosOri(m_pPlay->GetWheelsPos()[0]);


		vOffsetPos = NxVec3(m_vExtents.x, m_vExtents.y, m_vExtents.z);
		m_arrCarWheelPool[1]->SetLocalPos(vOffsetPos);
		m_arrCarWheelPool[1]->SetWheelMode(m_pCarModel->GetObjectByName("LF_W"));
		//m_arrCarWheelPool[1]->SetModelPosOri(m_pCarModel->GetObjectByName("LF_W")->GetTranslate());
		m_arrCarWheelPool[1]->SetModelPosOri(m_pPlay->GetWheelsPos()[1]);

		vOffsetPos = NxVec3(-m_vExtents.x, m_vExtents.y, m_vExtents.z);
		m_arrCarWheelPool[2]->SetLocalPos(vOffsetPos);
		m_arrCarWheelPool[2]->SetWheelMode(m_pCarModel->GetObjectByName("LB_W"));
		m_arrCarWheelPool[2]->SetModelPosOri(m_pCarModel->GetObjectByName("LB_W")->GetTranslate());
		m_arrCarWheelPool[2]->SetModelPosOri(m_pPlay->GetWheelsPos()[2]);

		vOffsetPos = NxVec3(-m_vExtents.x, -m_vExtents.y,m_vExtents.z);
		m_arrCarWheelPool[3]->SetLocalPos(vOffsetPos);
		m_arrCarWheelPool[3]->SetWheelMode(m_pCarModel->GetObjectByName("RB_W"));
		//m_arrCarWheelPool[3]->SetModelPosOri(m_pCarModel->GetObjectByName("RB_W")->GetTranslate());
		m_arrCarWheelPool[3]->SetModelPosOri(m_pPlay->GetWheelsPos()[3]);
	}

	return bResult;	
}

//-----------------------------------------------------------------------
bool CTencentCar::InitCarUserDate()
{
	if ( ! m_pController )
	{
		return false;
	}
	XUserData* pUD = new XUserData;					// 可能的内存泄露
	pUD->userData = m_pPlay;
	pUD->_actorType = XUserData::AT_WHEELLESSCAR;
	NxActor * pControllerActor = m_pController->getActor();
	pControllerActor->userData = pUD;
	pControllerActor->setGroup(PhysXManager::CG_VEHICLE);

	return true;
}
void CTencentCar::ReleaseCarUserData()
{
	if (m_pController)
	{
		XUserData* pUd = (XUserData*)m_pController->getActor()->userData;
		if (pUd)
		{
			SAFE_DELETE(pUd);
		}
	}
}

//-----------------------------------------------------------------------
bool CTencentCar::InitCarMotor()
{
	Play * pPlay = NxPlayMgr::GetHostPlay();

	if ( !pPlay )
		return false;

	SAFE_DELETE(m_pCarMotorControl);
	m_pCarMotorControl = new CCarMotorControl;

	PLAYERDESC &	stPlayDesc  = pPlay->GetCurPlayDesc();
	CStringEx		CarPath		= (const char * ) stPlayDesc.strKartPath ;
	CStringEx		CarFilePath = CarPath + "KartAccelConfig.lua";

	//CStringEx strEx;
	//strEx.FormatPt(NiApplication::ConvertMediaFilename("Avatar/kart_base/10002/KartAccelConfig.lua"));
	if (!m_pCarMotorControl || !m_pCarMotorControl->LoadConfig(NiApplication::ConvertMediaFilename(CarFilePath.c_str()))) 
	{
		return false;
	}

	return true;
}

//-----------------------------------------------------------------------
bool CTencentCar::InitCollisionProcessors()
{
	bool bRet = true;
	SAFE_DELETE(m_pCarFenceCollisionProcessor);
	SAFE_DELETE(m_pCarCarCollisionProcessor);


	// 碰撞数据
	for(int i = 0; i<8; i++)
	{
		m_vCollisionData[i].id = -1;
		m_vCollisionData[i].collisionTime = 0.0f;
		m_vCollisionData[i].nearCollisionTime = 0.0f;
		m_vCollisionData[i].selfPos = NxVec3(0, 0, 0);
		m_vCollisionData[i].selfVec = NxVec3(0, 0, 0);
		m_vCollisionData[i].otherPos = NxVec3(0, 0, 0);
		m_vCollisionData[i].otherVec = NxVec3(0, 0, 0);
	}

	m_pCarFenceCollisionProcessor = new CCarFenceCollision(m_pCarCollisionCenter);
    
	bRet = bRet & (m_pCarFenceCollisionProcessor != NULL);

	m_pCarCarCollisionProcessor = new CCarCarCollision(m_pCarCollisionCenter);

	bRet = bRet & (m_pCarCarCollisionProcessor != NULL);

	return bRet;
}

//-----------------------------------------------------------------------
bool CTencentCar::InitTecentCar( NxVec3 vPos)
{
	bool bReturn = true;
	if ( !InitCarMotor()) 
	{
		CHECKINIT("InitCarMotor");
	}

	if ( !InitCarPhysParam() )
	{
		CHECKINIT("InitCarPhysParam");
	}
	if ( !InitController(vPos) )
	{
		CHECKINIT("InitController");
	}

	if ( !InitCarWheel(vPos) )
	{
		CHECKINIT("InitCarWheel");
	}

	if ( !InitAccelDrift() )
	{
		CHECKINIT("InitAccelDrift");
	}

	if ( !InitCarUserDate() )
	{
		CHECKINIT("InitCarUserData");
	}

	if ( !InitCollisionProcessors() ) 
	{
		CHECKINIT("InitCollisionProcessors");
	}

	if ( !InitRaycastCallback() ) 
	{
		CHECKINIT("InitRaycastCallback");
	}
	
	return bReturn;

}

//-----------------------------------------------------------------------
void CTencentCar::RemoteCarUpdate(DWORD dt)
{

	NxVec3 vVecLocal = GetVecLocal();

	//LogText("Remote Play Vec : x = %f, y= %f , z= %f \n",
	//	vVecLocal.x, vVecLocal.y, vVecLocal.z );

	SetDeltaTime(dt);

	// 更新轮子Pick 的地表处理
	//UpdateWheel(dt);

	// 计算重力影响
//	RemoteUpdateGravity(dt);

	TriggerForceTwistPool(dt);

	UpdateRemoteCarVec(dt);

	// 更新角速度 
	//UpdateCarWec(dt);

	//　计算轮子的转动			
	CaculateWheelRotate(dt);

	// 更新远程完家移动
	UpdateRemoteLineMovement(dt);
	// 同步模型
	//SynchronizeModel();
	RemoteSynchronizeModel();
}

//-----------------------------------------------------------------------
void CTencentCar::UpdateRemoteCarVec( DWORD dt )
{
	float fDeltaTime = dt / 1000.0f;
	KEYINFO stInfo = GetKeyInfo();
	NxVec3 vTotalForce = GetTotalForce();
	NxVec3 vVecGlobal  = GetVecGlobal();

	vVecGlobal += fDeltaTime * vTotalForce / m_fMass ;
	SetVecGlobal( vVecGlobal );
	return;
}

//-----------------------------------------------------------------------
void CTencentCar::UpdateRemoteLineMovement(DWORD dt)
{
	float	fTime			= dt /1000.0f ;
	NxVec3	vVecGlobal		= GetVecGlobal();
	NxVec3	vStep(0, 0, 0);

	vStep = vVecGlobal * fTime;

	NxF32	fSharpness		= 0.8f;
	NxU32	unCollisionFlags;

	if ( IsBoxController())
		m_pController->getActor()->moveGlobalOrientation(GetOriGlobal());

	m_pController->move(
		vStep, 
		PhysXManager::CG_ENVIRONMENT, 
		0.001f, 
		unCollisionFlags, 
		fSharpness);
}

//-----------------------------------------------------------------------
void CTencentCar::RemoteSynchronizeModel( float dt)
{
	if (!m_pController || !m_pCarModel)
	{
		return;
	}
	NxMat33   matGlobal		  =	GetOriGlobal();
	NiMatrix3 matModelGlobal;

	NiPhysXTypes::NxMat33ToNiMatrix3( matGlobal,matModelGlobal );
	RotateMatWorldZAxis(matModelGlobal,-NxPi/2.0f);

	NxVec3    vGlobalPos		= GetPosGlobal();
	NiPoint3  vGlobalModelPos	= NiPoint3( vGlobalPos.x,vGlobalPos.y,vGlobalPos.z );

//	float offsetZ = GetOffsetZ();
	float offsetZ = m_fAdjustRemoteOffset;
	vGlobalModelPos -= NiPoint3(0.0f,0.0f,1.0f) * (offsetZ);

	m_pCarModel->SetRotate(matModelGlobal);
	m_pCarModel->SetTranslate(vGlobalModelPos);

	// 处理轮子的转动
	SynchronizeWheel(dt);

	m_pCarModel->Update( 0.0f, false );

}


//-----------------------------------------------------------------------
void CTencentCar::UpdateTencentCar(DWORD dt,KEYINFO stKeyInfo)
{
	if (dt > 100)  {
		dt = 100;
	}


	if ( GetContactStatus() == ECS_FULLCONTACT||
		GetContactStatus() == ECS_TREECONTACT||
		GetContactStatus() == ECS_TWOCONTACT )
	{
		NxVec3   vVecPlaneLocal = GetVecLocal(); 
		vVecPlaneLocal.z = 0.0f;
		NxVec3   vVecPlaneGlobal(0.0f,0.0f,0.0f);
		LocalToGlobal(vVecPlaneLocal , vVecPlaneGlobal);
		if ( vVecPlaneGlobal.z <= 0.0f )
			SetVecZLocal(-3.0f);
	}
//	LogText("GetVecLocal x = %f, y = %f, z = %f \n ", GetVecLocal().x,
//		GetVecLocal().y, GetVecLocal().z );



	SetDeltaTime(dt);

	SetKeyInfo(stKeyInfo);

	UpdatePhysParam();

	// 更新轮子Pick 的地表处理
	UpdateWheel(dt);

	// 根据地表 以及转角调整车的姿态 
#if defined(_PHYSX_2_7_0)
	UpdateCarPose();
#endif

	m_pDriftCenter->Update(dt, stKeyInfo);

	// 计算重力影响
	UpdateGravity(dt);

	// 计算玩家键盘操作所产生的力
	UpdateArm(dt);

	// 滚动摩擦和侧向摩擦力 因为涉及到漂移,暂时加在这里
	UpdateFriction(dt);

	// 在力扭矩池中 计算合力 以及扭矩 这里要对速度进行控制
	// 加速喷射的情况要特殊处理
	TriggerForceTwistPool(dt);

#if defined(_PHYSX_2_7_0)
	this->SynchronizeSimulator(dt/1000.0f);
#endif
	m_pSuspension->Update(dt/1000.0f, stKeyInfo);
	// 更新角速度 
	UpdateCarWec(dt);

	// 更新速度
	UpdateCarVec(dt);

	// 锁定移动 
	CheckLock();

	if ( m_pDriftCenter->GetDriftStatus() 
		== CDriftCenter::DRIFT_NONE)
	{
		//更新车的角度（一个是根据角速度设置姿态矩阵，同时也设置速度方向）
		UpdateTurnMovement(dt);

		// 更新车体运动 ( 应该分为正常运动, 和漂移运动) 
		UpdateLineMovement(dt);

	}

	// 漂移托管处理
	m_pDriftCenter->Run(dt,stKeyInfo);


	// 同步模型
//#if defined(_PHYSX_2_7_0)
	//SynchronizeHostPlayerModel(dt);
//#else
	SynchronizeModel(dt);
//#endif

	// 检查小喷
	CheckXiaoPen	( dt/ 1000.0f);
	CheckJumpMotion ( dt/ 1000.0f);

}


//-----------------------------------------------------------------------
void CTencentCar::UpdatePhysParam()
{
	if ( !m_pCarMotorControl ) 
		return;

	std::list<PHYSINFO>::iterator iter = m_arrPhysInfo.begin();

	for ( ; iter != m_arrPhysInfo.end(); iter++ ) {
		if( (*iter).dwPID == DD_ACCEL )
		{
			m_pCarMotorControl->SetAcceParam((*iter).fParam );
		}
		else if( (*iter).dwPID == DD_JIQI_ID)
		{
			CALL(CPowerController::Get(), GetSinglePower()->SetKartCoef((*iter).fParam));
		}		
	}
}


//-----------------------------------------------------------------------
void CTencentCar::UpdateWheel(DWORD dt)
{
	for ( int i = 0; i < m_arrCarWheelPool.size(); i++)
	{
		// 生成轮子接触点
		m_arrCarWheelPool[i]->RayPick(m_fAdjustPickGap);
	}

#if defined (_PHYSX_2_7_0)
	if (m_pSimulator->GetContactState())
		SetContactStatus(ECS_FULLCONTACT);
	else
		SetContactStatus(ECS_INAIR);
#else
	CaculatePrivotTwistForce(m_arrPrivotPoint);
	AdjustCarPos(m_arrPrivotPoint);
#endif
	CALL(TerrainEffectMgr::Get(), UpdateTerrainProp(m_arrCarWheelPool[0]->GetContactMaterialIndex())); 

	m_arrPrivotPoint.clear();
	// 计算支点
	//CaculatePrivot(m_arrPrivotPoint);

	//计算支点力 和 扭矩


	//// 冲击弹跳 begin add by hoverzhao 

	//			NxReal fLBWheelContactVec = m_arrCarWheelPool[0]->GetContactVec();
	//			NxReal fRBWheelContactVec = m_arrCarWheelPool[1]->GetContactVec();

	//			BUMPINFO BumpInfo1 ; BumpInfo1.nPitch = 0;
	//			BUMPINFO BumpInfo2 ; BumpInfo2.nPitch = 0;
	//			NxVec3   vVecPlaneLocal = GetVecLocal(); 
	//			vVecPlaneLocal.z = 0.0f;
	//			NxVec3   vVecPlaneGlobal(0.0f,0.0f,0.0f);
	//			LocalToGlobal(vVecPlaneLocal , vVecPlaneGlobal);
	//			//LogText ( "VecPlaneGlobal.z = %f \n ", vVecPlaneGlobal);

	//			m_arrCarWheelPool[0]->GetBumpInfo(BumpInfo1);
	//			m_arrCarWheelPool[1]->GetBumpInfo(BumpInfo2);

	//			//	LogText("nPitch1 = %d , nPitch2 = %d, fLBWheelContactVec = %f, fRBWheelContactVec = %f \n " , 
	//			//		BumpInfo1.nPitch , BumpInfo2.nPitch ,fLBWheelContactVec, fLBWheelContactVec);

	//			NxReal RotYWec1 = NxMath::abs(BumpInfo1.fAngle) / GetMainApp()->GetFrameTime();
	//			NxReal RotYWec2 = NxMath::abs(BumpInfo2.fAngle) / GetMainApp()->GetFrameTime();
	//			//LogText( "RotYWec1 = %f , RotYWec2 = %f \n",RotYWec1,RotYWec2 );

	//			if ( (RotYWec1 > 8.0f || RotYWec2 > 8.0f)  && NxMath::abs(vVecPlaneLocal.x) > 8.0f && vVecPlaneGlobal.z >= 0.0f)
	//			//	if ( (BumpInfo1.nPitch == 1 || BumpInfo2.nPitch == 1)  )
	//				{
	//					if( !m_pDriftCenter->IsDrifting() )
	//						SetVecZGlobal(5.0f * NxMath::abs(vVecPlaneLocal.x) / 15.0f);
	//				}

	//				//if ( GetVecGlobal().z > 0.0f && GetVecGlobal().z < 0.5f )
	//				;//		

	//	// 冲击弹跳结束


		//　计算轮子的转动
		CaculateWheelRotate(dt);
}

//-----------------------------------------------------------------------
void CTencentCar::UpdateGravity(DWORD dt)
{
	// 有轮子抬起的情况 已经在 UpdateWheels 函数中
	// 的CaculatePrivotTwistForce 函数处理过了,所以
	// 可以直接返回
	//if ( GetContactStatus() != ECS_FULLCONTACT)
	//{
	//	return;
	//}

	if ( m_pDriftCenter->IsDrifting() )
	{
		return;
	}

	int nContactStatus = GetContactStatus();
	if ( nContactStatus == ECS_INAIR || nContactStatus == ECS_NONECONTACT)
	{
		AddForceGlobal( NxVec3 ( 0.0f,0.0f,m_fGravity ),EFT_GRAVITY );
		return;
	}

	NxVec3 vDirDown			= -GetUpGlobal();
	NxVec3 vGravityGlobal = NxVec3( 0.0f,0.0f,m_fGravity );
	vGravityGlobal = vGravityGlobal - vGravityGlobal.dot(vDirDown) * vDirDown;
	AddForceGlobal(vGravityGlobal, EFT_GRAVITY);
//	m_vGravity = vGravityGlobal;
}

//-----------------------------------------------------------------------
void CTencentCar::UpdateArm(DWORD dt)
{
	float fTime = dt / 1000.f;

	if (GetContactStatus() == ECS_NONECONTACT ||
		GetContactStatus() == ECS_INAIR ) 
	{
		return;
	}

	if ( m_pDriftCenter->IsDrifting())
	{
		return;
	}

	KEYINFO stKeyInfo  = GetKeyInfo();	

	int nAccelstatus = 0;
	

	if (stKeyInfo.bUp) 
	{
		nAccelstatus = 1;
	}
	if (stKeyInfo.bDown) 
	{
		nAccelstatus = -1;
	}


	if (stKeyInfo.bLeft || stKeyInfo.bRight) 
	{
		if (stKeyInfo.bLeft)
		{
			if (m_fSteeringAccum > 0) 
			{
				m_fSteeringAccum += 0.8f * fTime;
			}
			else
			{
				m_fSteeringAccum += 2.5f * fTime;
			}
		}
		else
		{
			if (m_fSteeringAccum < 0) 
			{
				m_fSteeringAccum -= 0.8f * fTime;
			}
			else
			{
				m_fSteeringAccum -= 2.5f * fTime;
			}
		}

		if (fabsf(m_fSteeringAccum) > 1.0f) 
		{
			m_fSteeringAccum = NxMath::sign(m_fSteeringAccum);
		}
	}
	else
	{
		m_fSteeringAccum = 0;//0.5f * fTime;
	}

	float fXLocalForce = m_pCarMotorControl->GetScaleMotorForce(GetVecLocal().x, m_fSteeringAccum, 
		GetCarMotionStatus(), nAccelstatus, 0);
	AddForceXLocal(fXLocalForce);

	m_fArmForce	= fXLocalForce; 
}


//-----------------------------------------------------------------------
void CTencentCar::ProcessCompensateValue(float fValue )
{
	m_fCompensateParam = 1.00f;

	if ( fValue < -0.001f || fValue > 20.0f 
	|| m_vecComPensateTable.empty() ) {
		m_pCarMotorControl->SetAcceCompensateParam(m_fCompensateParam);
		return;
	}

	std::vector<COMPENSTATEINFO>::iterator iter = m_vecComPensateTable.begin();
	int nSize = 0;

	for ( ; iter != m_vecComPensateTable.end() ; iter++ ) {
		if ( fValue < (*iter).fPercent ) {
			m_fCompensateParam = (*iter).fValue;
			break;
		}
		if ( nSize == m_vecComPensateTable.size() - 1) 
			m_fCompensateParam = (*iter).fValue;

		nSize++;
	}

	m_pCarMotorControl->SetAcceCompensateParam(m_fCompensateParam);
	//LogText( "Value CompensateParam = %f \n " ,m_fCompensateParam);
	
}


//-----------------------------------------------------------------------
bool   CTencentCar::IsBoxController()
{
	 if( m_pPlay->GetKartID() < 10017 ) 
		 return false;
	 
	 return true;
}



#include "common.h"
//-----------------------------------------------------------------------
NxReal CTencentCar::BornPoseWec(NxVec3 vVec ,float fTime)
{
	KEYINFO stInfo			= GetKeyInfo();

	// 非左又非由，为0
	if (!stInfo.bLeft && !stInfo.bRight) 
	{
		return 0.0f;
	}

	NxVec3 vForward			= GetForwardGlobal();
	NxVec3 vVecGlobal		= GetVecGlobal();
	vVec.z = 0.0f;
	NxReal fVecSize			= vVec.magnitude();
	NxReal fVecFactor		= 0.5f;
	NxReal fPoseWec			= 1.0f;
	NxReal fMaxAngle		= 0.0f;


	if ( fVecSize < 0.06f)
		return 0.0f;

	// 得到速度方向 到车头方向的夹角	( 顺时针旋转为正) 
	// 正常行驶		与车头方向的夹角	  不该超过fMaxAngle

	NxReal fAngle			= GetAngleBetweenVec3( NxVec3( 1.0f, 0.0f , 0.0f) , vVec);	
	
	//改成用曲线的方式来模拟车头和车身夹角的变化
	
	float fCoef = (fVecSize + 0.01f) / (fVecSize + 3.01f);
	fMaxAngle = 75.0f * NiFastSqrt(fCoef);


	//fMaxAngle = NxMath::sign ( fAngle ) * fMaxAngle;
	if ( NxMath::abs( GDAngle(fAngle) ) > 30 && 
		 NxMath::abs( GDAngle(fAngle) ) < 150 )
	{
		//LogText("车头角达到极限了 fAngle = %f \n",NxMath::abs( GDAngle(fAngle)));
	}

	// 车速向前
	if ( NxMath::abs(fAngle) <= NxPi /2.0f )
	{
		if ((fAngle >  DGAngle( fMaxAngle )				&& !stInfo.bRight ) ||
			(fAngle <  DGAngle(-fMaxAngle )				&& !stInfo.bLeft  )   )
			return 0.0f;
	}
	else // 车速向后
	{
		// 碰撞可能会出现 侧向摩擦应该起作用
		// 并出现胎印 和 轨迹
		// 向左转弯
		if (stInfo.bLeft)
		{
			if (fAngle >  DGAngle( fMaxAngle - 180.0f ))
				return 0.0f;
		}
		else
		{
			if (fAngle > DGAngle(180.0f - fMaxAngle)) 
			{
				return 0.0f;
			}
		}
	}

	NxReal fSign = NxMath::sign(vVec.x);
	NxReal fZoomCoef = m_fOmiga1;

	float fSpeedSize = m_pPlay->GetKart()->GetVelocity();
	if (fSpeedSize > m_fMaxTurnSpeed)
		fSpeedSize = m_fMaxTurnSpeed;
	if (fSpeedSize < m_fMinTurnSpeed)
		fSpeedSize = m_fMinTurnSpeed;

	fSpeedSize -= m_fMinTurnSpeed;
	float fRate = 1.0f - (fSpeedSize*fSpeedSize / ((m_fMaxTurnSpeed - m_fMinTurnSpeed) * (m_fMaxTurnSpeed - m_fMinTurnSpeed)));

	fZoomCoef *= m_fBaseTurnRate + fRate * m_fVolatileTurnRate;

	if ( stInfo.bLeft )
	{
		return  fZoomCoef * fSign;
	}

	if ( stInfo.bRight )
	{
		return -fZoomCoef * fSign;
	}

	return 0.0f;
}

//-----------------------------------------------------------------------
NxReal CTencentCar::BornWec( NxVec3 vVec )
{
	KEYINFO stInfo = GetKeyInfo();
	
	NxVec3 vForward			= GetForwardGlobal();
	NxVec3 vVecGlobal		= GetVecGlobal();
	NxReal fAngle			= GetCarHeadAngle();

	fAngle = NxMath::sign(fAngle);

	float fSteer = 0.0f;
	
	if ( stInfo.bRight )
		fSteer = -1.0f;
	else if ( stInfo.bLeft )
		fSteer = 1.0f;
	else
		fSteer = 0.0f;

	//LogText("Steer = %f\n", fSteer);

	NxReal fCarLen			= 0.48f;
	NxReal fCarWheelAngle	= 0.0f;
	NxReal fMoveRadio		= 0.0f;
	NxReal fWec				= 0.0f;
	vVec.z = 0.0f;
	NxReal fVecSize			= vVec.magnitude() ;

	// 计算车轮角 
	float fCoef = powf(fabsf(m_fSteeringAccum * 1.6f), 0.125f);
	//if (stInfo.bShift == true) 
	//{
	//	fCoef *= 2.2f;
	//}
	fCarWheelAngle  = DGAngle(45.0f)  * ( 1 - NxMath::atan(fVecSize) * 2 / NxPi ) * fCoef;
	if ( fVecSize < 0.5 )
		fCarWheelAngle = DGAngle(45.0f);

	if ( fAngle <= NxPi/2.0f)
		fMoveRadio		= (fCarLen  ) / NxMath::sin( fCarWheelAngle * 0.5f );
	else
		fMoveRadio		= (fCarLen * 2.0f ) / NxMath::sin( fCarWheelAngle * 0.5f );

	fWec			= fSteer * NxMath::sign(GetVecLocal().x) * fVecSize / fMoveRadio ;

	
	m_arrCarWheelPool[0]->SetTurnAngle( -fCarWheelAngle * fSteer );
	m_arrCarWheelPool[1]->SetTurnAngle( -fCarWheelAngle * fSteer );

	if ( fVecSize < 5.0e-02)
		return 0.0f;
	return fWec * m_fOmiga;
}

//-----------------------------------------------------------------------
NxReal CTencentCar::BornWecFromVec( NxVec3 vVec ,float fTime	)
{

	KEYINFO stKeyInfo = GetKeyInfo();
	float m_fSteer = 0.0f;
	if ( stKeyInfo.bRight)
	{
		if( m_fWheelTurnAngle > 0.0f )
			m_fWheelTurnAngle = 0.0f;
		m_fSteer = -1.0f;
	}
	else if ( stKeyInfo.bLeft)
	{
		if ( m_fWheelTurnAngle < 0.0f )
			m_fWheelTurnAngle = 0.0f;
		m_fSteer = 1.0f;
	}
	else {
		if ( NxMath::abs( m_fWheelTurnAngle) > 1.0E-04 )
			m_fSteer = -NxMath::sign(m_fWheelTurnAngle);
		else {
			m_fSteer = 0.0f;
		}
	}
	float fDeltaAngle = m_fWheelTurnVec * fTime * m_fSteer;
	float fAbsDeltaAngle = NxMath::abs( fDeltaAngle );

	m_fWheelTurnAngle += fDeltaAngle ; 
	//LogText( "Wheel Turn Angle = %f \n", m_fWheelTurnAngle );

	if ( NxMath::abs( m_fWheelTurnAngle) < 1.0E-04) 
		m_fWheelTurnAngle = 0.0f;


	NxVec3	vVecGlobal		= GetVecGlobal();
	float	fCarLen			= 1.0f;
	float	fMoveRadio		= 0.0f;
	float   fVecSize		= vVecGlobal.magnitude() * 0.5;

	if ( NxMath::abs( m_fWheelTurnAngle ) > m_fMaxWheelTurnAngle)
		m_fWheelTurnAngle = NxMath::sign ( m_fWheelTurnAngle ) * 
		m_fMaxWheelTurnAngle;

	float   fWheelAbsTurnAngle = NxMath::abs( m_fWheelTurnAngle );

	fMoveRadio = 0.5 *  NxMath::pow(__max(1.0f,fVecSize),0.5f) / NxMath::sin(fWheelAbsTurnAngle * 0.5f);

	float vWec = NxMath::sign(GetVecLocal().x)  * NxMath::sign( m_fWheelTurnAngle )
		* fVecSize / fMoveRadio ; 


	if ( fWheelAbsTurnAngle < 1.0E-04 
		&& (!stKeyInfo.bRight || !stKeyInfo.bLeft))
	{
		m_fWheelTurnAngle = 0.0f;
		vWec = 0.0f;
	}

	if (NxMath::abs( GetVecLocal().x ) < 1.0E-03)
	{
		vWec = 0.0f;
		m_fWheelTurnAngle = 0.0f;
	}
	m_arrCarWheelPool[0]->SetTurnAngle( -m_fWheelTurnAngle  );
	m_arrCarWheelPool[1]->SetTurnAngle( -m_fWheelTurnAngle  );

	//LogText( "Turn Wec  = %f \n", vWec);

	return vWec * 0.5;
}

//-----------------------------------------------------------------------
void CTencentCar::UpdateTurnMovement(DWORD dt)
{

	float fTime = dt / 1000.0f;

	// 自身的角速度只在漂移， 车体倾斜 才会产生
	// 调整姿态矩阵 ( 角速度作用 )
	NxVec3	vWecLocal	= GetWecLocal();
	NxMat33	matGround   = GetMatGround();

	// 推荐 在这里添加 最大角度锁定
	// ..... 
	// 推荐

	RotateMat33XAxis( matGround , vWecLocal.x * fTime);
	RotateMat33YAxis( matGround , vWecLocal.y * fTime);
	RotateMat33ZAxis( matGround , vWecLocal.z * fTime);

	float fScale = vWecLocal.magnitude();

	if (fScale >= 0.001f && !m_pDriftCenter->IsDrifting()) 
	{
		m_fDeltaAlpha = vWecLocal.z  * fTime;
		vWecLocal *= m_fSelfRotateAttenuation;// * fScale / (0.5f + fScale);
		m_vLastWecGlobal = vWecLocal;
		m_fClockTurnAngle += m_fDeltaAlpha;
		m_fDeltaAlpha = 0.0f;
		//RotateMat33ZAxis(matGround, m_fDeltaAlpha);		
		//SetMatGround(matGround);
		SetWecLocal(vWecLocal);		
	}

	//LogText("车头角 fAngle = %f \n", GDAngle(GetCarHeadAngle()));
//	SetWecLocal(NxVec3(0, 0, 0));
	

	// 正常状态下，是没有车体自身的角速度概念的，
	// 只是有个用来调整速度方向的 "假" 角速度
	int nContactStatus = GetContactStatus();
	if ( nContactStatus == ECS_INAIR  || nContactStatus == ECS_NONECONTACT)
	{
		m_fDeltaAlpha = 0.0f;
		m_fBeltaAlpha = 0.0f;
	}
	else
	{
		NxVec3 kVecLocal = GetVecLocal();
		m_fDeltaAlpha = BornWec( kVecLocal ) * fTime;
		m_fBeltaAlpha = BornPoseWec(kVecLocal,fTime) * fTime;	
//		LogText("车头角 Belta = %f \n",( GDAngle(m_fBeltaAlpha)));
//		LogText("车头角 Delta = %f \n",( GDAngle(m_fDeltaAlpha)));
	}
	m_fClockTurnAngle += m_fDeltaAlpha + m_fBeltaAlpha ;

	if (NxMath::abs(m_fClockTurnAngle) > NxPi*2.0f)
		m_fClockTurnAngle = ::fmodf(m_fClockTurnAngle, NxPi* 2.0f);


	RotateMat33ZAxis( matGround, m_fDeltaAlpha);
	RotateMat33ZAxis( matGround, m_fBeltaAlpha);
	SetMatGround( matGround );

}


//-----------------------------------------------------------------------
void CTencentCar::UpdateLineMovement(DWORD dt)
{

	// 调整本地线速度的方向
	NxVec3 vVecLocal = GetVecLocal();	
	RotateVecZAxis( vVecLocal ,m_fDeltaAlpha ) ;
		
	SetVecLocal(vVecLocal);

	//if (GetContactStatus() == ECS_INAIR) 
	/*{
		NxVec3 kForceTotal = GetTotalForce();
		LogText("Z方向速度 = %f, 力 = %f\n", m_vVecGlobal.z, kForceTotal.z);
	}*/

	float	fTime			= dt /1000.0f ;
	NxVec3	vStep(0, 0, 0);
	NxVec3	vVecGlobal		= GetVecGlobal();
	
	if (!m_pPlay->GetKart()->IsKartMoveable() && !m_pPlay->GetKart()->IsGameFinish())
	{
		vVecGlobal.x = vVecGlobal.y = 0.0f;
		SetVecGlobal(vVecGlobal);
		LogText("BeLocked!!\n");
	}

	vStep = vVecGlobal * fTime;

	NxF32	fSharpness		= 0.8f;
	NxU32	unCollisionFlags;


	if ( IsBoxController())
		m_pController->getActor()->moveGlobalOrientation(GetOriGlobal());

	m_pController->move(
		vStep, 
		PhysXManager::CG_ENVIRONMENT, 
		0.001f, 
		unCollisionFlags, 
		fSharpness);
}

//-----------------------------------------------------------------------
void CTencentCar::UpdateCarVec(DWORD dt)
{
	if ( m_pDriftCenter->GetDriftStatus() 
		== CDriftCenter::DRIFT_START)
	{
		return;
	}

	float fDeltaTime = dt / 1000.0f;
	KEYINFO stInfo = GetKeyInfo();

	// 将导致和牛顿第二定律有点出入 
	NxVec3 vTotalForce = GetTotalForce();
	NxVec3 vVecGlobal  = GetVecGlobal();

	
	m_vLastVecGlobal = vVecGlobal;
	vVecGlobal += fDeltaTime * vTotalForce / m_fMass ;
	SetVecGlobal( vVecGlobal );
	//LogText( "TencentCar vec Global z = %f \n ", vVecGlobal.z );
}

//-----------------------------------------------------------------------
void CTencentCar::UpdateCarWec(DWORD dt)
{
	if ( m_pDriftCenter->GetDriftStatus() 
		== CDriftCenter::DRIFT_START)
	{
		return;
	}

	float	fDeltaTime			= dt / 1000.0f;
	NxVec3	vTotalTwist			= GetTotalTwist();
	NxVec3	vLocalTotalTwist	= NxVec3( 0.0f, 0.0f, 0.0f );
	GlobalToLocal( vTotalTwist, vLocalTotalTwist);

	NxVec3 vWecLocal			= GetWecLocal();

	m_vLastWecGlobal			= GetWecGlobal();

	// 这里使用 柱状刚体 角加速度 的计算方式
	vWecLocal += vLocalTotalTwist * fDeltaTime;// /
	//( 0.5 * m_vExtents.x * m_vExtents.x * m_fMass);

	if ( vWecLocal.magnitude() < 1.0e-03)
	{
		vWecLocal = NxVec3( 0.0f, 0.0f, 0.0f );
	}

	SetWecLocal ( vWecLocal );

	/*
	if ( ECS_INAIR == GetContactStatus())
	{
		SetWecLocal( NxVec3 (0.0f,0.0f,0.0f));
	}*/

}

NxVec3 CTencentCar::GetCurrentInputForce()
{
	NxVec3 kTotalForce;
	kTotalForce.zero();
	for ( int i = 0; i < m_arrForcePool.size(); ++i)
	{
		kTotalForce += m_arrForcePool[i].vForce;
	}

	return kTotalForce;
}

//-----------------------------------------------------------------------
void CTencentCar::UpdateFriction(DWORD dt)
{
	// 即时在空中，空气阻力也是存在的
	NxVec3 kAireFriction = CaculateAirFriction(GetVecLocal());
	AddForceLocal(kAireFriction);
	// 空中时不考虑滑动摩擦
	if ( GetContactStatus() == ECS_NONECONTACT ||
		 GetContactStatus() == ECS_INAIR )
	{
		return ;
	}

	float fTime = dt / 1000.f;

	NxVec3 vRollFrict = GetVecGlobal();
	AddForceGlobal( -vRollFrict * m_fRollFrictionForce);

	if ( GetKeyInfo().bLeft || GetKeyInfo().bRight )
	{
	
		NxVec3 kPos;
		CCarWheel * pWheel = NULL;
		float fAngleBetweenSpeedAndForward = GetCarHeadAngle();
		float fForwardCoef = 0.5f;
		if (m_pDriftCenter->IsDrifting())
			fForwardCoef = 0.20f + 0.6f * sinf(fAngleBetweenSpeedAndForward);


		float fBackwardCoef= 1.0f - fForwardCoef;
		float fWeight      = GetMass() * m_fGravity;
		float fAngle       = 0.0f;
		float fDist		   = 0.0f;//　力臂长度

		
		NxVec3 kVelocityLocal = GetVecLocal();
		int nContactStatus = GetContactStatus();
		if (nContactStatus != ECS_NONECONTACT
			|| nContactStatus != ECS_INAIR) 
		{
			kVelocityLocal.z = 0.0f;
		}


		NxVec3 m_kWheelForce[4];
		NxVec3 kTotalWheelForce;
		kTotalWheelForce.zero();

		float fCoefY = fabsf(kVelocityLocal.y);

		fCoefY = fCoefY / (1.f + fCoefY) * 0.8f;

		float fCoefX = fabsf(kVelocityLocal.x);

		fCoefX = fCoefX / (1.f + fCoefX);
		fCoefX *= fCoefX;

		float fCoefSlipCoef = 7.5f / (0.01f + fabsf(kVelocityLocal.x));
		float fCoefSlip = NxMath::atan(fCoefSlipCoef * fCoefSlipCoef) * 3.f / NxPi;

		NxVec3 arrWheelForce[4];

		for (int i = 0; i < 2; ++i)
		{
			arrWheelForce[i].zero();
			arrWheelForce[i+2].zero();
			pWheel = GetWheel(i);
			fDist = 0.0f;
			kPos   = pWheel->GetLocalPos();
			fAngle = pWheel->GetTurnAngle();
		
			fAngle += NxMath::atan(kVelocityLocal.y/ kVelocityLocal.x) * fCoefX;
			float fDispSpeed = (kVelocityLocal.x * GameSetting::SpeedDispCoef);
			if ((GetKeyInfo().bLeft || GetKeyInfo().bRight) /*&& fDispSpeed > 1.0f*/) 
			{
				NxReal fRealAngle = GetCarHeadAngle();
				if ( NxMath::abs( fRealAngle) >  NxPi/4.0f )
				{
					fRealAngle = fRealAngle * NxMath::sign(fRealAngle);
				}
				fAngle += fRealAngle;
			}
			//fDist = kPos.magnitude();
			// 前轮
			if (fabsf(fAngle) > 0.01f)
			{
				
				float fSinA = 0.0f, fCosA = 0.0f;
				NxMath::sinCos(fAngle, fSinA, fCosA);

				arrWheelForce[i].x =  m_fSlipFrictionForce * fWeight * fForwardCoef * 
					NiFastSqrt(fabsf(fSinA)) * NxMath::sign(kVelocityLocal.x) * fCoefX * 0.2f;
				//if (fabsf(kVelocityLocal.y) > 0.001f)
				arrWheelForce[i].y =  m_fSlipFrictionForce * fWeight * fForwardCoef *
					NxMath::abs(fCosA )* NxMath::sign(kVelocityLocal.y) * fCoefY * 1.2f * fCoefSlip;
				
				
				
				if ( fDispSpeed > 190.f)
					arrWheelForce[i].x += 0.2f;
				else if ( fDispSpeed > 170.f)
					arrWheelForce[i].x += 0.3f;
				else if ( fDispSpeed > 160.0f)
					arrWheelForce[i].x += 0.35f;
				else if ( fDispSpeed > 90.0f)
					arrWheelForce[i].x += 0.4f;
				else if ( fDispSpeed > 45.f)
					arrWheelForce[i].x += 0.5f;
				else if (fDispSpeed > 1.0f)
					arrWheelForce[i].x += 0.6f;
				else
					arrWheelForce[i].x = 0.0f;
			}			

			else
			{
				arrWheelForce[i].x = 0.0f;
				//if (fabsf(kVelocityLocal.y) > 0.001f)
				arrWheelForce[i].y =  m_fSlipFrictionForce * fWeight * fForwardCoef * 
					NxMath::sign(kVelocityLocal.y) * fCoefY * 1.2f;
				// 计算后轮摩擦力
				arrWheelForce[i+2].x = 0.0f;
				arrWheelForce[i+2].y = m_fSlipFrictionForce * fWeight * fBackwardCoef * 
					NxMath::sign(kVelocityLocal.y) * fCoefY * 1.2f;				
			}


			kTotalWheelForce += arrWheelForce[i];
		}

		if (kVelocityLocal.x < 0) 
		{
			kTotalWheelForce.x *= 0.2f;
		}

		float fabsAngle = GetCarHeadAngle(true, true);
		if (m_pDriftCenter->GetDriftStatus() == CDriftCenter::DRIFT_NONE && fabsAngle > NxPi / 6.f && fabsAngle < NxPi / 4.f) 
		{
		;//	kTotalWheelForce.y *= 3.0f;
		}

		NxVec3 kTotalForce = GetCurrentInputForce();
		NxVec3 kLocalForce;
		GlobalToLocal(kTotalForce, kLocalForce);
		kTotalForce = kLocalForce;
	/*
		if (fabsf(kTotalForce.x) > 0.001f)
		{
			if (kTotalForce.x  * (kTotalForce.x + kTotalWheelForce.x) < 0) 
			{
				kTotalWheelForce.x = -kTotalForce.x;
			}
		}*/
		
		float fPreSpeed = kVelocityLocal.y + (kTotalWheelForce.y + kTotalForce.y) * fTime;

		//LogText("PreSpeed = %f, NextSpeed = %f\n", kVelocityLocal.y, fPreSpeed);
		if (kVelocityLocal.y * fPreSpeed < 0) 
		{
			kTotalWheelForce.y = -kVelocityLocal.y / fTime - kTotalForce.y;
		}

		

		//LogText("XFrictionFric = %f\n", kTotalWheelForce.x);
		//LogText("Fraction y = %f \n ", kTotalWheelForce.y);
		AddForceLocal(kTotalWheelForce);
	}
	else
	{
		NxVec3 vLocalVec = GetVecLocal();
		vLocalVec.y *= 0.9f;
		SetVecYLocal(vLocalVec.y);
	}

#ifdef LOG
	{
		for (int i = 0; i < 4; ++i)
			LogText("WheelForce(%d) = (%f, %f, %f)\n", i, m_kWheelForce[i].x, m_kWheelForce[i].y, m_kWheelForce[i].z);
		LogText("TotalForce = (%f, %f, %f)", m_kTotalForce.x, m_kTotalForce.y, m_kTotalForce.z);
		LogText("WheelTorque = (%f, %f, %f)", m_kWheelTorque.x, m_kWheelTorque.y, m_kWheelTorque.z);
	}
#endif
    
}

//-----------------------------------------------------------------------
//#if defined(_PHYSX_2_7_0)
void CTencentCar::SynchronizeWheel(float dt)
{
	//总的轮子位移：
	static float offsetZ[WHEEL_COUNT] = {0.0f};

	if (m_pPlay->IsHost())
	{
		for (int i = 0; i < WHEELS; i++)
		{
			offsetZ[i] = m_pSuspension->GetWheelOffset(i);
		}
	}

	for ( int i = 0 ; i < m_arrCarWheelPool.size(); i++ )
	{
		if (m_pPlay->IsHost())
		{
			NiPoint3 posW = m_arrCarWheelPool[i]->GetModelPosOri();
			posW.z += offsetZ[i];

			m_arrCarWheelPool[i]->GetWheelModel()->SetTranslate(posW);
		}
		m_arrCarWheelPool[i]->SynchronizeWheel();
	}
}


//#else
//void CTencentCar::SynchronizeWheel(float dt)
//{
//	//总的轮子位移：
//	NiPoint3 offsetTotal[WHEEL_COUNT];//
//
//	for (int num = 0; num < WHEEL_COUNT; ++num)
//	{
//		offsetTotal[num].x = 0.0f;
//		offsetTotal[num].y = 0.0f;
//		offsetTotal[num].z = 0.0f;
//	}
//
//	//轮子在上下颠簸时的位移
//	//float bumpOffs[WHEEL_COUNT];
//	if (m_nFullContactBump)
//	{
//		for (int i = 0; i < WHEEL_COUNT; ++i)
//		{
//			m_arrBumpOffs[i] =  m_fFullContactBumpMax * NxMath::sin(m_nFullContactBumpTravel);
//			
//			m_nFullContactBumpTravel += NxMath::degToRad(65.0f)* dt;
//
//			if (m_nFullContactBumpTravel > NxPi)// * 2.0f)
//			{
//				m_nFullContactBump --;
//
//				if (m_nFullContactBump > 0)
//				{
//					m_fFullContactBumpMax *= 0.5f;
//					m_nFullContactBumpTravel = 0.0f;
//				}
//				else
//				{
//					m_nFullContactBumpTravel = 0.0f;
//					m_nFullContactBump = 0;
//				}
//
//			}
//		}
//	}
//	else
//	{
//		if (!GetBumpOffset(m_arrBumpOffs))
//		{
//			for (int i = 0; i < WHEEL_COUNT; ++i)
//			{
//				m_arrBumpOffs[i] = NxMath::sign(m_arrBumpOffs[i]) * 
//					NxMath::abs(m_arrBumpOffs[i]) * 0.6f * dt;
//			}
//		}
//	}
//	for (int j = 0; j < WHEEL_COUNT; ++j)
//	{
//		offsetTotal[j].x = 0.0f;
//		offsetTotal[j].y = 0.0f;
//		offsetTotal[j].z = m_arrBumpOffs[j];
//	}
//
//	//~轮子在上下颠簸时的位移
//
//	// 临时这么写的　　缺轮子的上下移动　和　大小变化
//	for ( int i = 0 ; i < m_arrCarWheelPool.size(); i++ )
//	{
//		//NiMatrix3 rotX;
//		//rotX.MakeXRotation(m_arrCarWheelPool[i]->GetWheelRotateAngle());
//		//m_arrCarWheelPool[i]->GetWheelModel()->SetRotate(rotX);
//		if (m_pPlay->IsHost())
//		{
//			NiPoint3 posW = m_arrCarWheelPool[i]->GetModelPosOri() + offsetTotal[i];
//			m_arrCarWheelPool[i]->GetWheelModel()->SetTranslate(posW);
//		}
//		m_arrCarWheelPool[i]->SynchronizeWheel();
//	}
//}
//#endif
//

//-----------------------------------------------------------------------

void CTencentCar::SynchronizeModel(float dt, bool hostPlayer)
{
	if (!m_pController || !m_pCarModel)
	{
		return;
	}
	dt = dt/1000.0f;

	SetOriGlobal( GetMatGround() );

	//同步显示/物理
	NiMatrix3 matModelGlobal;

#if defined(_PHYSX_2_7_0)
	NxMat33 rot = m_pSimulator->GetChassisOrientationSmooth();

	NiMatrix3 niRot;
	NiPhysXTypes::NxMat33ToNiMatrix3(rot, niRot);

	float fXangle, fYAngle, fZAngle;
	niRot.ToEulerAnglesXYZ(fXangle, fYAngle, fZAngle);
	niRot.FromEulerAnglesZXY(-m_fClockTurnAngle, fXangle, fYAngle);

	NxMat33 matGlobal; 
	NiPhysXTypes::NiMatrix3ToNxMat33(niRot, matGlobal);
#else
	NxMat33   matGlobal		  =	GetOriGlobal();
#endif	
	m_fOffsetZ = GetChassisOffsetZ();

	//if( m_pDriftCenter->IsDrifting() )
	//{
	//	NxReal fAngle = GetCarHeadAngle();
	//	NxReal fDelta = NxMath::sin( fAngle );

	//	NxReal fXAlpha = 10.0f * NxPi / 180.0f * fDelta;
	//	RotateMat33XAxis( matGlobal, fXAlpha );	
	//	SetCarBodyAngle( fXAlpha );

	//	m_fOffsetZ -= 0.1f * fabsf(sinf(fXAlpha));
	//}
	//else
	//{
	//	NxReal fBodyDownSpeed = 1.0f;
	//	NxReal fBodyAngle = GetCarBodyAngle() ;
	//	NxReal fAbsBodyAngle = NxMath::abs(fBodyAngle );
	//	if ( fAbsBodyAngle <= fBodyDownSpeed * dt || fAbsBodyAngle < 1.0E-04)
	//	{
	//		//RotateMat33XAxis( matGlobal, -fBodyAngle );
	//		SetCarBodyAngle( 0.0f );
	//	}
	//	else
	//	{
	//		fBodyAngle -=NxMath::sign(fBodyAngle) * fBodyDownSpeed * dt ; 
	//		RotateMat33XAxis( matGlobal, fBodyAngle );
	//		SetCarBodyAngle( fBodyAngle );
	//	}
	//	m_fOffsetZ -= 0.1f * fabsf(sinf(fBodyAngle));
	//}
	
	//test
	float fXAlpha = m_pSuspension->GetRotX();
    RotateMat33XAxis( matGlobal, fXAlpha );	
	//
	float fYAlpha = ProcessCarFallPoseRotY(dt);
	RotateMat33YAxis( matGlobal, fYAlpha );	

	NiPhysXTypes::NxMat33ToNiMatrix3( matGlobal,matModelGlobal );
	RotateMatWorldZAxis(matModelGlobal,-NxPi/2.0f);

	if (m_nFullContactBump)
	{
		m_fOffsetZ += m_arrBumpOffs[0];
	}

	// 这儿调整一下车身的位置
	NxVec3    vGlobalPos		= GetPosGlobal();
	NiPoint3  vGlobalModelPos	= NiPoint3( vGlobalPos.x,vGlobalPos.y,vGlobalPos.z );

	m_fOffsetZ = m_fAdjustOffset;

	// 这儿晃动车的模型
	// 如果是下落，则上下晃动
	if (m_bIsFall)
	{
		float ZAxisOffset = ProcessModalFall(dt);

		m_fOffsetZ += ZAxisOffset;
	}

	//matModelGlobal * NxVec3(0, 0, 1);
	NiPoint3 vDown = matModelGlobal * NiPoint3(0.0f, 0.0f, -1.0f);
	
	float fAngle = acosf(GetUpGlobal().dot(NxVec3(0, 0, 1)));
	float fAdjust = m_fAdjustBevelOffset * sinf(fAngle);
	if (fAdjust < 0.0f)
		fAdjust = 0.0f;

	// m_fOffsetZ 用来调整车模的高度
	// fAdjust 用来调整模型在斜坡上的高度
	//vGlobalModelPos -= NiPoint3(0.0f,0.0f,1.0f) * (m_fOffsetZ + fAdjust);
	//NiPoint3 vDown = matModelGlobal * NiPoint3(0.0f, 0.0f, -1.0f);

	vGlobalModelPos = vGlobalModelPos + ((m_fOffsetZ + fAdjust)*vDown);


	////将3D模型与物理模型同步
	//RotMeTender(m_mt3DModel, matModelGlobal, dt);

	if (hostPlayer)
		UpdateWheelsBump(matModelGlobal, dt);

	m_pCarModel->SetRotate(m_mt3DModel);
	m_pCarModel->SetTranslate(vGlobalModelPos);

	// 处理轮子的转动
	SynchronizeWheel(dt);

	m_pCarModel->Update( 0.0f, false );

	return;
}

/*-----------------------------------------------------
/\ X
|
|
|
1           0
\/---------\/

Y -------		
2           3
\/---------\/
------------------------------------------------------*/


/* 
此函数 是为了在 所有轮子 pick 到地面的情况下, 对车身做一下调整
车身 会绕着 离地面最近的那个轮子 做旋转,  使其他轮子也接近地面
这样车身就几乎和地面平行了
*/ 

//-----------------------------------------------------------------------
void CTencentCar::ReCaculatPrivot()
{
	std::vector<int>  vecPrivotPoint;
	vecPrivotPoint.clear();

	m_fAdjustXAngle = 0.0f;
	m_fAdjustYAngle = 0.0f;

	int		nMinIndex = -1;
	int		nMaxIndex = -1;
	float	fMinDistance = MAXDISTANCE;
	float   fMaxDistance = 0.0f;

	float fDis[4] = { 0.0f };

	for (  int i = 0; i < m_arrCarWheelPool.size(); i++)
	{
		float fDistance = m_arrCarWheelPool[i]->GetHitDistance();
		fDis[i] = fDistance;

		if ( fMinDistance > fDistance)
		{
			fMinDistance = fDistance;
			nMinIndex = i;
		}
		if ( fMaxDistance <= fDistance)
		{
			fMaxDistance = fDistance;
			nMaxIndex = i;
		}
	}

	float fDelta = fMaxDistance - fMinDistance;
	if ( fDelta < 0)
	{
		assert(0);
	}
	else if (fDelta < 0.005f)
		return;

	switch ( nMinIndex)
	{
	case 0:
		{
			if ( ( fDis[1] - fDis[0]) >= 0.005f)
				m_fAdjustXAngle = -NxMath::asin(( fDis[1] - fDis[0]) / (m_vExtents.x * 2)); 
			if ( ( fDis[3] - fDis[0]) >= 0.005f)
				m_fAdjustYAngle = NxMath::asin(( fDis[3] - fDis[0]) / (m_vExtents.y * 2));
			break;
		}
	case 1:
		{
			if ( ( fDis[0] - fDis[1]) >= 0.005f)
				m_fAdjustXAngle = NxMath::asin(( fDis[0] - fDis[1]) /  (m_vExtents.x * 2)); 
			if ( ( fDis[2] - fDis[1]) >= 0.005f)
				m_fAdjustYAngle = NxMath::asin(( fDis[2] - fDis[1]) / (m_vExtents.y * 2));
			break;
		}
	case 2:
		{
			if ( ( fDis[1] - fDis[2]) >= 0.005f)
				m_fAdjustYAngle = -NxMath::asin(( fDis[1] - fDis[2]) / (m_vExtents.y * 2)); 
			if ( ( fDis[3] - fDis[2]) >= 0.005f)
				m_fAdjustXAngle = NxMath::asin(( fDis[3] - fDis[2]) / (m_vExtents.x * 2));
			break;
		}
	case 3:
		{
			if ( ( fDis[0] - fDis[3]) >= 0.005f)
				m_fAdjustYAngle = -NxMath::asin(( fDis[0] - fDis[3]) / (m_vExtents.y * 2)); 
			if ( ( fDis[2] - fDis[3]) >= 0.005f)
				m_fAdjustXAngle = -NxMath::asin(( fDis[2] - fDis[3]) / (m_vExtents.x * 2));
			break;
		}
	}
	
	m_fAdjustXAngle *= -0.1f;
	m_fAdjustYAngle *= -0.1f;
}
//-----------------------------------------------------------------------
void CTencentCar::CaculatePrivot( std::vector<int> & vecPrivotPoint)
{
	vecPrivotPoint.clear();

	if (this->GetContactStatus() == ECS_FULLCONTACT)
	{
		for (int i = 0; i < 4; ++i)
		{
			vecPrivotPoint.push_back(i);
		}
	}
}

//-----------------------------------------------------------------------
void CTencentCar::CaculatePrivotTwistForce( std::vector<int> & vecPrivotPoint)
{
	float fTime = GetDeltaTime() / 1000.f;

	bool bNear[4] = {false};

	int nContactGroundNum = 0;
	for (int i = 0; i < 4; i++)
	{
		if (m_arrCarWheelPool[i]->GetContactStatus() == CCarWheel::ENS_NEAR)
		{
			bNear[i] = true;
			++nContactGroundNum;
		}
		else
		{
			bNear[i] = false;
		}

	}
	
	if ( nContactGroundNum >=3 )
	{
		//SetVecZGlobal(-1.0f);
		SetContactStatus(ECS_FULLCONTACT);
	}
	else if ( nContactGroundNum == 2)
	{
		// 对角线两点着地 认为全着地
		if ( (bNear[0] && bNear[2]) || bNear[1] && bNear[3])
		{
			SetContactStatus(ECS_FULLCONTACT);
		}
		else
		{
			SetContactStatus(ECS_TWOCONTACT);
			//SetVecZLocal(-25.0f * fTime);
			//	AddForceGlobal(m_vGravity);
		}
	}
	else if ( nContactGroundNum == 1)
	{
		SetContactStatus(ECS_ONECONTACT);
		//SetVecZLocal(-25.0f * fTime);
		//AddForceGlobal(m_vGravity);
		
	}

	else 
	{		
		if ( GetLastContactStatus() != ECS_INAIR )
		{
			//AddForceGlobal(m_vGravity * 0.5f);			
			SetContactStatus(ECS_NONECONTACT);
		}
		if (
			((m_arrCarWheelPool[0]->GetContactStatus() == CCarWheel::ENS_NONE) &&
			(m_arrCarWheelPool[1]->GetContactStatus() == CCarWheel::ENS_NONE)) ||
			((m_arrCarWheelPool[2]->GetContactStatus() == CCarWheel::ENS_NONE) &&
			(m_arrCarWheelPool[3]->GetContactStatus() == CCarWheel::ENS_NONE)))  // bug danger hoverzhao
		{
			SetContactStatus(ECS_INAIR);
		}
		else if 
		(	(m_arrCarWheelPool[0]->GetContactStatus() != CCarWheel::ENS_NEAR) ||
			(m_arrCarWheelPool[1]->GetContactStatus() != CCarWheel::ENS_NEAR) ||
			(m_arrCarWheelPool[2]->GetContactStatus() != CCarWheel::ENS_NEAR) ||
			(m_arrCarWheelPool[3]->GetContactStatus() != CCarWheel::ENS_NEAR))
		{
			SetContactStatus(ECS_NONECONTACT);

		}
		SetRotateSideAxis( ERSA_NONE);

	}
	if (GetCarMotionStatus() == ECS_FULLCONTACT) 
	{
		m_bGotTerrainNormal = false;
	}

	bool A = (GetContactStatus() == ECS_INAIR && GetLastContactStatus() != ECS_INAIR);
	bool B = (GetContactStatus() == ECS_NONECONTACT && (GetLastContactStatus() != ECS_NONECONTACT 
		&& GetLastContactStatus() != ECS_INAIR));


	if (A || B) 
	{
		NxVec3 kVecLocal = GetVecLocal();

		float fDir = 0.0f;
		if (kVecLocal.x > 0.01f)
			fDir = 1.0f;
		else if (kVecLocal.x < -0.01f)
			fDir = -1.0f;
		//SetWecYLocal(fDir / (fabsf(kVecLocal.x) + 1.0f) * 2.f);
		//SetVecZLocal(0);	
	}
}

void CTencentCar::CaculateWheelRotate(DWORD ddt)
{
	if (!m_pPlay->GetKart()->IsKartMoveable() && !m_pPlay->GetKart()->IsGameFinish())
	{
		for ( int i = 0; i < m_arrCarWheelPool.size(); ++i )
			m_arrCarWheelPool[i]->SetWheelRotateAngle(0);
		return;
	}

	float dt = ddt / 1000.0f ;
	//处理轮子的转动  轮子转动的快慢　
	NxReal fAngle		= GetCarHeadAngle();
	NxVec3 vVelRefer	= GetVecLocal() * cos(fAngle);

	NxReal fRotateAngle = 0.0f;
	NxVec3 vVecLocal = GetVecLocal();
	NxReal vVecValue = NxMath::abs(vVecLocal.x);


	for ( int i = 0; i < m_arrCarWheelPool.size(); ++i )
	{
		fRotateAngle = m_arrCarWheelPool[i]->GetWheelRotateAngle();
		if ( vVecValue > 0.02f )
		{
			NxReal cof = vVecValue / ( 0.5f + vVecValue );
			fRotateAngle -= NxMath::sign(vVecLocal.x) * NxPi * dt * cof * 8.f;
			if ( NxMath::abs( fRotateAngle ) > 2.0f * NxPi )
				fRotateAngle = fmodf( fRotateAngle, NxPi * 2.0f );
		}

		m_arrCarWheelPool[i]->SetWheelRotateAngle(fRotateAngle);
	}
}


/*-----------------------------------------------------
/\ X
|
|
|       
1           0 
\/---------\/

Y -------		        
2           3 
\/---------\/
------------------------------------------------------*/

void CTencentCar::AdjustCarPosTwoPrivot( std::vector<int> & vecPrivotPoint )
{
	NxVec3 vPrivotAxis = m_arrCarWheelPool[vecPrivotPoint[1]]->GetContactPos()
		- m_arrCarWheelPool[vecPrivotPoint[0]]->GetContactPos() ;
	vPrivotAxis.normalize();

	NxVec3 vAirAxis = NxVec3 ( 0.0f,0.0f, 0.0f);

	NxVec3 vCarOldUpDir = GetUpGlobal(); 

	if ( GetRotateSideAxis() & ERSA_UP )		//  vPrivotAxis   方向向左 左*后 为上
	{
		vAirAxis = -GetForwardGlobal();
	}
	else if ( GetRotateSideAxis() & ERSA_DOWN )	//  vPrivoitAxis  方向向右 右*前 为上
	{
		vAirAxis =  GetForwardGlobal();
	}
	else if ( GetRotateSideAxis() & ERSA_LEFT ) //  vPrivoitAxis  方向向下 后*右为上
	{
		vAirAxis = -GetLeftGlobal();
	}
	else if ( GetRotateSideAxis() & ERSA_RIGHT ) // vPrivoitAxis  方向向下 后*右为上
	{
		vAirAxis = -GetLeftGlobal();
	}

	NxVec3 vCarNewUpDir = vPrivotAxis.cross( vAirAxis );

	vCarNewUpDir.normalize();


	NxVec3 vZ = NxVec3(0,0,1);
	NxVec3 vAxisRot = vZ.cross(vCarNewUpDir);

	// 可能的bug  车的上方向 严格 水平垂直向上 情况的处理
	if (vAxisRot.magnitude() < 1.0E-06)
	{
		; 
	}
	// 计算上下坡的旋转角度
	//vAxisRot.normalize();
	NxReal fAlpha = NxMath::acos(vZ.dot(vCarNewUpDir)); 

	// 注意 Gamebryo 顺时针转动为正 PhysX 逆时针转动为正
	NxMat33 matrotAlpha;
	NiMatrix3 nimatRot;
	nimatRot.MakeRotation(-fAlpha , NiPoint3(vAxisRot.x, vAxisRot.y, vAxisRot.z));
	NiPhysXTypes::NiMatrix3ToNxMat33(nimatRot, matrotAlpha);

	//-----------------------------------------------------------------------
	RotateMat33ZAxis( matrotAlpha, GetTurnAngle());

	SetMatGround( matrotAlpha );
}

void CTencentCar::AdjustCarPosOnePrivot( std::vector<int> & vecPrivotPoint )
{
	SetMatGround( GetOriGlobal() );
	return;
}
//-----------------------------------------------------------------------
void CTencentCar::AdjustCarPos( std::vector<int> & vecPrivotPoint )
{
//------------------------------------------------------------
	//TODO: 根据接触点的不同，调整车的姿态

	int nStatusPre = GetLastContactStatus();
	NxVec3 velGlobal = GetVecGlobal();
	int nCurContactStatus = GetContactStatus();

#if defined(_PHYSX_2_7_0)
	if ((nCurContactStatus != ECS_INAIR) && (nCurContactStatus != ECS_NONECONTACT))
	{
		m_pSimulator->SetContactState(ChassisSimulator::RS_ONEARTH);
	}
	else 
	{
		m_pSimulator->SetContactState(ChassisSimulator::RS_INAIR);
	}
#endif
	if (nCurContactStatus == ECS_INAIR || nCurContactStatus == ECS_NONECONTACT)
	{
		return;
	}

	NxVec3 dirUp = CaculateTerrainNormal();

	dirUp.normalize();

	
	//---根据地形 调整物理赛车的姿态，对车的位置进行重新安置

	NxVec3 vZ = NxVec3(0,0,1);
	vZ.normalize();
	NxVec3 axisRot = vZ.cross(dirUp);
	axisRot.normalize();
	NxReal alpha = NxMath::acos(vZ.dot(dirUp));  // 上下坡的旋转角度


	NxMat33 rotAlpha;
	NiMatrix3 niRot;
	niRot.MakeRotation(-alpha , NiPoint3(axisRot.x, axisRot.y, axisRot.z));
	NiPhysXTypes::NiMatrix3ToNxMat33(niRot, rotAlpha);

	NxReal totalAngle = m_fClockTurnAngle;

	NxMat33 rotZ;
	rotZ.rotZ(totalAngle);
	rotAlpha = rotAlpha * rotZ;

	m_vLastTerrainNormal  = dirUp;
//#if !defined(_PHYSX_2_7_0)

	SetMatGround(rotAlpha);
//#endif


}

//-----------------------------------------------------------------------
void CTencentCar::RemoteUpdateGravity( DWORD dt)
{
	//AddForceGlobal( NxVec3 ( 0.0f,0.0f,-9.8f),EFT_GRAVITY );
	int nContactStatus = GetContactStatus();
	if ( nContactStatus == ECS_INAIR || nContactStatus == ECS_NONECONTACT)
	{
		float fVecZ = GetVecZGlobal();
		fVecZ += m_fGravity * dt / 1000.f;
		SetVecZGlobal(fVecZ);
	}
	else
	{
		float fVecZ = GetVecZLocal();
		fVecZ += m_fGravity * dt / 1000.f;
		SetVecZLocal(fVecZ);
	}
}


//-----------------------------------------------------------------------
void CTencentCar::TriggerForceTwistPool( DWORD dt)
{
	m_vTotalForce = NxVec3 ( 0.0f, 0.0f, 0.0f);
	m_vTotalTwist = NxVec3 ( 0.0f, 0.0f, 0.0f);

	bool bOnlyOne = true;
	for ( int i = 0; i < m_arrForcePool.size(); ++i)
	{
		if ( m_arrForcePool[i].eType == EFT_ONLYONE )
		{
			if ( bOnlyOne )
			{
				m_vTotalForce += m_arrForcePool[i].vForce;
				bOnlyOne = false;
			}
		}
		else
		{
			m_vTotalForce += m_arrForcePool[i].vForce;
		}
	}
	
	for ( int j = 0; j < m_arrTwistPool.size(); ++j)
	{
		m_vTotalTwist += m_arrTwistPool[j].vTwist;
	}


	ClearForcePool();
	ClearTwistPool();
}

//-----------------------------------------------------------------------

//-----------------------------------------------------------------------

void CTencentCar::ClearForcePool()
{
	bool bIsEnd = false;

	std::vector<FORCE>::iterator iter = m_arrForcePool.begin();
	while( !bIsEnd )
	{
		bIsEnd = true;
		for(; iter != m_arrForcePool.end(); ++iter)
		{
			if ( (*iter).dwTime < GetDeltaTime()) 
			{
				m_arrForcePool.erase(iter);
				bIsEnd = false;
				break;
			}
			else 
			{
				(*iter).dwTime -= GetDeltaTime();
			}
		}
	}
}

//-----------------------------------------------------------------------

void CTencentCar::ClearTwistPool()
{
	bool bIsEnd = false;

	std::vector<TWIST>::iterator iter = m_arrTwistPool.begin();
	while( !bIsEnd )
	{
		bIsEnd = true;
		for(; iter != m_arrTwistPool.end(); ++iter)
		{
			if ( (*iter).dwTime < GetDeltaTime() ) 
			{
				m_arrTwistPool.erase(iter);
				bIsEnd = false;
				break;
			}
			else 
			{
				(*iter).dwTime -= GetDeltaTime();
			}
		}
	}
}
//-----------------------------------------------------------------------
NxVec3 CTencentCar::CaculateTerrainNormal()
{
	if ( GetContactStatus() == ECS_INAIR )
		return GetUpGlobal();
	//---------------------------------
	NxVec3 dirUp(0.0f,0.0f,1.0f);
	NxVec3 dirLocalUp( 0.0f, 0.0f, 1.0f );


	int nValidType = XUserData::AT_TERRAIN | XUserData::AT_FENCE_GAMEBRYO;
	float fWecAdjust = 3.0f;

	if ((m_arrCarWheelPool[2]->GetContactStatus() == CCarWheel::ENS_NEAR &&
		m_arrCarWheelPool[3]->GetContactStatus()  == CCarWheel::ENS_NEAR&&
		m_arrCarWheelPool[0]->GetContactStatus() == CCarWheel::ENS_NEAR) &&
		((m_arrCarWheelPool[2]->GetShootShapetype() & nValidType) && 
		(m_arrCarWheelPool[3]->GetShootShapetype() & nValidType) && 
		(m_arrCarWheelPool[0]->GetShootShapetype() & nValidType)))
	{
		dirUp = (m_arrCarWheelPool[3]->GetContactPos() - m_arrCarWheelPool[0]->GetContactPos()).cross(
			m_arrCarWheelPool[3]->GetContactPos() - m_arrCarWheelPool[2]->GetContactPos());
		dirUp.normalize();

	}
	else if ((m_arrCarWheelPool[2]->GetContactStatus() == CCarWheel::ENS_NEAR &&
		m_arrCarWheelPool[3]->GetContactStatus()  == CCarWheel::ENS_NEAR&&
		m_arrCarWheelPool[1]->GetContactStatus() == CCarWheel::ENS_NEAR) &&
		((m_arrCarWheelPool[2]->GetShootShapetype() & nValidType) && 
		(m_arrCarWheelPool[3]->GetShootShapetype() & nValidType) && 
		(m_arrCarWheelPool[1]->GetShootShapetype() & nValidType)))
	{
		dirUp = (m_arrCarWheelPool[2]->GetContactPos() - m_arrCarWheelPool[3]->GetContactPos()).cross(
			m_arrCarWheelPool[2]->GetContactPos() - m_arrCarWheelPool[1]->GetContactPos());
		dirUp.normalize();

	}
	else if ((m_arrCarWheelPool[0]->GetContactStatus() == CCarWheel::ENS_NEAR &&
		m_arrCarWheelPool[1]->GetContactStatus()  == CCarWheel::ENS_NEAR&&
		m_arrCarWheelPool[2]->GetContactStatus() == CCarWheel::ENS_NEAR) &&
		((m_arrCarWheelPool[0]->GetShootShapetype() & nValidType) && 
		(m_arrCarWheelPool[1]->GetShootShapetype() & nValidType) && 
		(m_arrCarWheelPool[2]->GetShootShapetype() & nValidType)))
	{
		dirUp = (m_arrCarWheelPool[1]->GetContactPos() - m_arrCarWheelPool[2]->GetContactPos()).cross(
			m_arrCarWheelPool[1]->GetContactPos() - m_arrCarWheelPool[0]->GetContactPos());
		dirUp.normalize();

	}
	else if ((m_arrCarWheelPool[0]->GetContactStatus() == CCarWheel::ENS_NEAR &&
		m_arrCarWheelPool[1]->GetContactStatus()  == CCarWheel::ENS_NEAR&&
		m_arrCarWheelPool[3]->GetContactStatus() == CCarWheel::ENS_NEAR) &&
		((m_arrCarWheelPool[0]->GetShootShapetype() & nValidType) && 
		(m_arrCarWheelPool[1]->GetShootShapetype() & nValidType) && 
		(m_arrCarWheelPool[3]->GetShootShapetype() & nValidType)))
	{
		dirUp = (m_arrCarWheelPool[0]->GetContactPos() - m_arrCarWheelPool[1]->GetContactPos()).cross(
			m_arrCarWheelPool[0]->GetContactPos() - m_arrCarWheelPool[3]->GetContactPos());
		dirUp.normalize();

	}


	/*-----------------------------------------------------
	/\ X
	|
	|
	|
	1           0
	\/---------\/

	Y -------		
	2           3
	\/---------\/
	------------------------------------------------------*/

	else if ( (m_arrCarWheelPool[0]->GetContactStatus() == CCarWheel::ENS_NEAR ) &&
		(m_arrCarWheelPool[1]->GetContactStatus() == CCarWheel::ENS_NEAR ) &&
		(m_arrCarWheelPool[0]->GetShootShapetype() & nValidType ) && 
		(m_arrCarWheelPool[1]->GetShootShapetype() & nValidType ) )
	{
		RotateVecYAxis(dirLocalUp ,-fWecAdjust * GetDeltaTime()/1000.0f);
		LocalToGlobal(dirLocalUp,dirUp);
	}
	else if ( (m_arrCarWheelPool[2]->GetContactStatus() == CCarWheel::ENS_NEAR ) &&
		(m_arrCarWheelPool[3]->GetContactStatus() == CCarWheel::ENS_NEAR ) &&
		(m_arrCarWheelPool[2]->GetShootShapetype() & nValidType ) && 
		(m_arrCarWheelPool[3]->GetShootShapetype() & nValidType ) )
	{
		//dirUp = m_vLastTerrainNormal;
		RotateVecYAxis(dirLocalUp,fWecAdjust * GetDeltaTime()/1000.0f);
		LocalToGlobal(dirLocalUp,dirUp);
	}
	else if ( (m_arrCarWheelPool[1]->GetContactStatus() == CCarWheel::ENS_NEAR ) &&
		(m_arrCarWheelPool[2]->GetContactStatus() == CCarWheel::ENS_NEAR ) &&
		(m_arrCarWheelPool[1]->GetShootShapetype() & nValidType ) && 
		(m_arrCarWheelPool[2]->GetShootShapetype() & nValidType ) )
	{

		//dirUp = m_vLastTerrainNormal;
		RotateVecXAxis(dirLocalUp,fWecAdjust * GetDeltaTime()/1000.0f);
		LocalToGlobal(dirLocalUp,dirUp);
	}
	else if ( (m_arrCarWheelPool[0]->GetContactStatus() == CCarWheel::ENS_NEAR ) &&
		(m_arrCarWheelPool[3]->GetContactStatus() == CCarWheel::ENS_NEAR ) &&
		(m_arrCarWheelPool[0]->GetShootShapetype() & nValidType ) && 
		(m_arrCarWheelPool[3]->GetShootShapetype() & nValidType ) )
	{
		//dirUp = m_vLastTerrainNormal;
		RotateVecXAxis(dirLocalUp,-fWecAdjust * GetDeltaTime()/1000.0f);
		LocalToGlobal(dirLocalUp,dirUp);
	}
	else if ( m_arrCarWheelPool[0]->GetContactStatus() == CCarWheel::ENS_NEAR && 
		m_arrCarWheelPool[0]->GetShootShapetype() & nValidType )
	{
		//dirUp = m_vLastTerrainNormal;
		RotateVecXAxis(dirLocalUp, -fWecAdjust * GetDeltaTime() / 1000.0f );
		RotateVecYAxis(dirLocalUp, -fWecAdjust * GetDeltaTime() / 1000.0f );
		LocalToGlobal(dirLocalUp,dirUp);
	}
	else if ( m_arrCarWheelPool[1]->GetContactStatus() == CCarWheel::ENS_NEAR && 
		m_arrCarWheelPool[1]->GetShootShapetype() & nValidType )
	{
		//dirUp = m_vLastTerrainNormal;
		RotateVecXAxis(dirLocalUp, fWecAdjust * GetDeltaTime() / 1000.0f );
		RotateVecYAxis(dirLocalUp, -fWecAdjust * GetDeltaTime() / 1000.0f );
		LocalToGlobal(dirLocalUp,dirUp);
	}
	else if ( m_arrCarWheelPool[2]->GetContactStatus() == CCarWheel::ENS_NEAR && 
		m_arrCarWheelPool[2]->GetShootShapetype() & nValidType )
	{
		//dirUp = m_vLastTerrainNormal;
		RotateVecXAxis(dirLocalUp, -fWecAdjust * GetDeltaTime() / 1000.0f );
		RotateVecYAxis(dirLocalUp, fWecAdjust * GetDeltaTime() / 1000.0f );
		LocalToGlobal(dirLocalUp,dirUp);
	}
	else if ( m_arrCarWheelPool[0]->GetContactStatus() == CCarWheel::ENS_NEAR && 
		m_arrCarWheelPool[0]->GetShootShapetype() & nValidType )
	{
		//dirUp = m_vLastTerrainNormal;
		RotateVecXAxis(dirLocalUp, -fWecAdjust * GetDeltaTime() / 1000.0f );
		RotateVecYAxis(dirLocalUp, fWecAdjust * GetDeltaTime() / 1000.0f );
		LocalToGlobal(dirLocalUp,dirUp);
	}
	else
	{
		dirUp = m_vLastTerrainNormal;
	}

	dirUp.normalize();

	/*	dirUp = m_vLastTerrainNormal;
	bool bFound = false;
	*/	//for (int i = 0; i < WHEEL_COUNT; ++i)
	//{
	//	if ((m_arrCarWheelPool[i]->GetShootShapetype() & nValidType) &&
	//		m_arrCarWheelPool[i]->GetContactStatus() == CCarWheel::ENS_NEAR)
	//	{
	//		dirUp = m_arrCarWheelPool[i]->GetContactNormal() ;
	//		bFound = true;
	//		break;
	//	}

	//}

	//if (!bFound)
	//{
	//	dirUp = m_vLastTerrainNormal;
	//}


	if (NxMath::abs(dirUp.x ) > 1.01 ||
		NxMath::abs(dirUp.y ) > 1.01 ||
		NxMath::abs(dirUp.y ) > 1.01 )
		dirUp = NxVec3(0.0f,0.0f,1.0f);

	dirUp = SwooshRobot(dirUp);

	return dirUp;

}


//逐渐将车身旋转与地面平行
void CTencentCar::RotMeTender(NiMatrix3& mtSrc, const NiMatrix3& mtDest, float dt)
{
	//将3D模型与物理模型同步
	float fXAnglePhs,fYAnglePhs,fZAnglePhs;
	mtDest.ToEulerAnglesZXY(fZAnglePhs, fXAnglePhs, fYAnglePhs);

	float fXAngle,fYAngle,fZAngle;
	m_mt3DModel.ToEulerAnglesZXY(fZAngle, fXAngle, fYAngle);


	//TODO: 根据角度差值的大小来调整趋近的角度
	NiMatrix3 mtTemp;
	mtTemp.MakeZRotation(fZAnglePhs);
	NiMatrix3 rotXTmp;
	rotXTmp.MakeXRotation(fXAngle + (fXAnglePhs- fXAngle)*0.3f );
	NiMatrix3 rotYTmp;
	rotYTmp.MakeYRotation(fYAngle + (fYAnglePhs- fYAngle)*0.3f);

	mtTemp = mtTemp * rotXTmp * rotYTmp;

	mtSrc = mtTemp;

}
//增加车轮着地发生变化时的抖动
void CTencentCar::UpdateWheelsBump(const NiMatrix3& matDest, float dt)
{
	//当前无震动


	if (!m_bIsBumping)
	{
		//检测，若需要则产生一个新震动
		m_bIsBumping = GenerateBump();

		//if (!m_bIsBumping)
			RotMeTender(m_mt3DModel, matDest, dt);

	}
	else
	{
		//如果震动没有结束
		RotMeTender(m_mt3DModel, matDest, dt);
		//BumpMeTender(matDest, dt);

	}
}

void CTencentCar::BumpMeTender(const NiMatrix3& matDest, float dt)
{

	float rotX = 0.0f;
	float rotY = 0.0f;

	if (m_bumpTime.x > 0)
	{

		if (NxMath::abs(m_vBumpAngleCur.x + m_vBumpAngularVel.x * dt) 
			< NxMath::abs(m_vBumpAngle.x))
		{
			//TODO:可以添加反向的阻尼震荡
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
			//TODO:可以添加反向的阻尼震荡
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
	//将3D模型与物理模型同步
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
bool CTencentCar::GenerateBump()
{

	//TODO:确定沿速度方向两侧的轮子
	int nWheelA, nWheelB;
	if (this->GetVecLocal().x > 0.0f)
	{
		nWheelA = WHEEL1_INDEX;
		nWheelB = WHEEL0_INDEX;
	}
	else
	{
		nWheelA = WHEEL3_INDEX;
		nWheelB = WHEEL2_INDEX;
	}

	BUMPINFO infoA, infoB;
	m_arrCarWheelPool[nWheelA]->GetBumpInfo(infoA);
	m_arrCarWheelPool[nWheelB]->GetBumpInfo(infoB);

	float fCoeff = GetVecLocal().magnitude()/ 15.0f;

	fCoeff = NxMath::clamp(fCoeff, 1.0f, 0.5f);
	//float fDumpeAngle = NxMath::degToRad(5.0) * fCoeff;
	//float fAngularVel = NxMath::degToRad(15.0f) * fCoeff;
	float fDumpeAngle = NxMath::degToRad(8.0) * fCoeff;
	float fAngularVel = NxMath::degToRad(10.0f) * fCoeff;

	int nBumptime = 1;

	if (infoA.nPitch == 1 && infoB.nPitch == 1)
	{
		m_bumpTime.x = nBumptime;
		m_vBumpAngle.x = fDumpeAngle;
		m_vBumpAngularVel.x = fAngularVel;
		m_vBumpAngleCur.x = 0.0f;
	}
	else if (infoA.nPitch == -1 && infoB.nPitch == -1)
	{
		//向上
		m_bumpTime.x = nBumptime;
		m_bumpTime.y = 0;
		m_vBumpAngle.x = -fDumpeAngle;
		m_vBumpAngularVel.x = -fAngularVel;
		m_vBumpAngleCur.x = 0.0f;

	}
	else if (infoA.nPitch != 0)
	{
		float coeff = (nWheelA == WHEEL1_INDEX)? -1.0f : 1.0f;
		//左前轮		
		m_bumpTime.x = nBumptime;
		m_vBumpAngle.x = fDumpeAngle * coeff;
		m_vBumpAngularVel.x = fAngularVel* coeff;
		m_vBumpAngleCur.x = 0.0f;

		m_bumpTime.y = nBumptime;
		m_vBumpAngle.y = -fDumpeAngle* coeff;
		m_vBumpAngularVel.y = -fAngularVel* coeff;
		m_vBumpAngleCur.y = 0.0f;


	}else if (infoB.nPitch != 0)
	{
		float coeff = (nWheelB == WHEEL0_INDEX)? 1.0f : -1.0f;
		//右前轮
		m_bumpTime.x = nBumptime;
		m_vBumpAngle.x = fDumpeAngle * coeff;
		m_vBumpAngularVel.x = fAngularVel* coeff;
		m_vBumpAngleCur.x = 0.0f;

		m_bumpTime.y = nBumptime;
		m_vBumpAngle.y = fDumpeAngle* coeff;
		m_vBumpAngularVel.y = fAngularVel* coeff;
		m_vBumpAngleCur.y = 0.0f;

	}
	else
		return false;

	return true;
}

bool CTencentCar::GetBumpOffset(float* pfOffsets)
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
		NxVec3 pos = trans* (m_arrCarWheelPool[i]->GetLocalPos());
		pfOffsets[i] = -pos.z;
		//Log("%d : %f\n", i, pos.z);
	}

	//Log("angle [%f] [%f] , offset : [%f] [%f] [%f] [%f]\n", m_vBumpAngleCur.x, m_vBumpAngleCur.y, pfOffsets[0],pfOffsets[1],pfOffsets[2],pfOffsets[3]);

	return true;
}

float CTencentCar::GetChassisOffsetZ()
{
	float fOffset = 0.0f;

	NxVec3 dirGround = CaculateTerrainNormal();
	NxVec3 posCtr = GetPosGlobal();
	NxVec3 dirZ = NxVec3(0.0, 0.0, 1.0f);


	NxVec3 ptOnEarth;
	if (GetContactedPoint(ptOnEarth))
	{
		//controller 到地面的距离
		NxVec3 vTemp = posCtr - ptOnEarth;
		float d0 = dirGround.dot(vTemp);

		fOffset = d0 / (dirZ.dot(dirGround));
		fOffset += 0.06f;
		
	}
	else
	{
		fOffset = 0.31f;
	}

	return fOffset;

}
bool CTencentCar::GetContactedPoint(NxVec3& vPoint)
{
	bool bFound = false;
	for (int i = 3; i >=0 ; i--)
	{
		if (m_arrCarWheelPool[i]->GetContactStatus() == CCarWheel::ENS_NEAR && 
			m_arrCarWheelPool[i]->GetShootShapetype() == XUserData::AT_TERRAIN)
		{
			vPoint = m_arrCarWheelPool[i]->GetContactPos();
			bFound = true;
			break;
		}
	}
	return bFound;
}


//-----------------------------------------------------------------------
CCarWheel::CCarWheel( CTencentCar * pCar)
: m_pCar(pCar)
, m_pWheelMode( NULL )
, m_fHitDistance( 0 )
, m_fTurnAngle( 0 )
, m_fRotateAngle( 0 )
, m_fContactDistance(0.0f)
, m_fContactVec(0.0f)
{
}

CCarWheel::~CCarWheel()
{
}

//-----------------------------------------------------------------------
bool CCarWheel::Init()
{
	return true;
}

//-----------------------------------------------------------------------
NxVec3 CCarWheel::GetGlobalPos()
{
	NxMat34 matTrans(m_pCar->GetOriGlobal(), m_pCar->GetPosGlobal());
	return  matTrans * m_vLocalPos;
}


//-----------------------------------------------------------------------
int CCarWheel::Pick(NxRay ray, NxRaycastHit &hit, NxShape *&pShape)
{
	NxScene * pScene = m_pCar->GetScene();
	pShape = NULL;
	pShape = pScene->raycastClosestShape(
		ray,
		NX_ALL_SHAPES,
		hit,
		PhysXManager::CG_ENVIRONMENT);

	if (pShape)
	{
		XUserData * pUd = (XUserData*) hit.shape->getActor().userData;
		if ( pUd->_actorType == XUserData::AT_FENCE_MANUAL		||
			pUd->_actorType == XUserData::AT_FENCE_GAMEBRYO	)//||
			//pUd->_actorType == XUserData::AT_OBSTACLE_BIRD		) 
		{
			return ENS_OTHER;
		}


		if ( hit.distance < TINY_DIST) 
		{
			return ENS_NEAR;
		}
		else if ( hit.distance <  TINY_DIST + 2.0f )
		{
			return ENS_FAR;
		}
		else 
		{
			return ENS_NONE;
		}
	}
	return ENS_NONE;	
}

//-----------------------------------------------------------------------
void CCarWheel::RayPick(float fPickGap)
{

	NxVec3 pos = GetGlobalPos();

	NxRay ray(pos, -m_pCar->GetUpGlobal());

	NxRaycastHit hit;

	NxScene * pScene = m_pCar->GetScene();


	m_pShootShape = pScene->raycastClosestShape(ray, NX_ALL_SHAPES,hit, PhysXManager::CG_ENVIRONMENT);
	if ( m_pShootShape )
	{
		XUserData* pD = (XUserData*)m_pShootShape->getActor().userData;
		NxReal fDist = m_fContactDistance - hit.distance;
		if ((pD->_actorType == XUserData::AT_TERRAIN))

		{
			// 证明上行呢 ,也许是上坡,也许是向上弹跳
			if ( fDist > 0.0f )
			{
				m_fContactVec = fDist / GetMainApp()->GetFrameTime();
				//LogText ( "m_fContactVec = %f \n " , m_fContactVec );
			}
			else
			{
				m_fContactVec = 0.0f;
			}
		}
		m_fContactDistance = hit.distance;
	}



	// 此处一定要保证轮子的轴心 和 车子的底盘持平，  TINY_DIST 的值从何而来呢？？ 
	if (m_pShootShape)
	{
		//	LogText( "hit.distance = %f \n", hit.distance);
		if (hit.distance > TINY_DIST + 0.125 + fPickGap && 
			hit.distance < 1.0)
			m_nContactStaus = ENS_FAR;
		else if ( hit.distance <= TINY_DIST + 0.125f + fPickGap)
		{
			m_nContactMaterialIndex = hit.materialIndex;
			m_nContactStaus = ENS_NEAR;
		}
		else
			m_nContactStaus = ENS_NONE;

		m_vPreContactN = m_vCurContactN;
		m_vCurContactN = hit.worldNormal;
		m_vContactPos = pos + ray.dir * hit.distance;
		XUserData *pUd = (XUserData*)m_pShootShape->getActor().userData;


		if (pUd)
		{
			m_nShootShapetype = pUd->_actorType;
		}
		else
		{
			m_nShootShapetype = XUserData::AT_NODEFINE;
		}
	}
	else
	{
		m_nContactStaus = ENS_NONE;
		m_nShootShapetype = XUserData::AT_NODEFINE;
	}

	//if (!m_pCar) 
	//{
	//	return;
	//}
	//float fOldHitDistance = m_fHitDistance;

	//m_pShootShape = NULL;
	//m_fHitDistance = MAXDISTANCE;

	//// 这是上一桢的车体位置
	//NxVec3	vPos = GetGlobalPos();	

	//NxScene * pScene = m_pCar->GetScene();
	//NxRay rRay(vPos, -m_pCar->GetUpGlobal());

	//NxRaycastHit nxHit;


	//int nResult = Pick( rRay, nxHit, m_pShootShape );

	//m_nContactStaus = nResult;

	//switch ( nResult )
	//{
	//case ENS_NEAR:
	//	{
	//		m_vPreContactN	= m_vCurContactN;
	//		m_vCurContactN	= nxHit.worldNormal;
	//		m_vContactPos	= vPos + rRay.dir * nxHit.distance;
	//		XUserData * pUD = (XUserData*) m_pShootShape->getActor().userData;
	//		m_nShootShapetype = pUD->_actorType;
	//		m_fHitDistance = nxHit.distance;
	//		break;
	//	}
	//case ENS_FAR:
	//	{
	//		m_vPreContactN	= m_vCurContactN;
	//		m_vCurContactN	= nxHit.worldNormal;
	//		m_vContactPos	= vPos + rRay.dir * nxHit.distance;
	//		XUserData * pUD = (XUserData*) m_pShootShape->getActor().userData;
	//		m_nShootShapetype = pUD->_actorType;
	//		m_fHitDistance = nxHit.distance;
	//		break;
	//	}
	//case ENS_NONE:
	//	{
	//		m_nShootShapetype = XUserData::AT_NODEFINE;
	//		m_fHitDistance = fOldHitDistance;
	//		break;
	//	}
	//case ENS_OTHER:
	//	{
	//		m_nShootShapetype = XUserData::AT_NODEFINE;
	//		m_fHitDistance = fOldHitDistance;
	//		break;
	//	}
	//}


	////  bug 未完成  引入射线轮转处理 即摆动射线 不停 Pick
	///*	

	//for ( int i = -2  ; i < 3; ++i)
	//{
	//NxVec3 vRay = RotateVec
	//}
	//*/

	//return ;
}

//-----------------------------------------------------------------------
void CCarWheel::GetBumpInfo(BUMPINFO& bumpInfo)
{
	if (this->m_nContactStaus != ENS_NEAR)
		bumpInfo.nPitch = 0;
	else
	{
		NxReal angle = NxMath::acos(m_vPreContactN.dot(m_vCurContactN));
		if (angle > NxMath::degToRad(1.0f))
		{
			bumpInfo.fAngle = angle;

			//判断车上了一个坡，还是下了一个坡
			//NxVec3 velG = _parent->_rotateTotal * _parent->_velocityLocal;
			NxVec3 velG = m_pCar->GetVecGlobal();

			NxVec3 vCross = m_vPreContactN.cross(m_vCurContactN);
			vCross.normalize();
			NxVec3 vCross2 = m_vPreContactN.cross(velG);
			vCross2.normalize();

			float fTheta = NxMath::acos(vCross.dot(vCross2));

			if (fTheta > NxMath::degToRad(90.0f))
			{
				bumpInfo.nPitch = 1;//上坡
			}
			else
			{
				bumpInfo.nPitch = -1;//下坡
			}
		}
		else
		{
			bumpInfo.fAngle = 0.0f;
			bumpInfo.nPitch = 0;
		}
	}

}

//-----------------------------------------------------------------------
void CCarWheel::SynchronizeWheel()
{
	m_fRotateAngle = NxMath::mod(m_fRotateAngle, NxPi * 2.f);
	NiMatrix3 rotX;
	rotX.MakeXRotation(m_fRotateAngle);
	// Z方向偏转
	if (NxMath::abs(m_fTurnAngle) > 0.001f)
	{
		NiMatrix3 rotY;
		rotY.MakeZRotation(m_fTurnAngle);
		rotX = rotY * rotX;
	}
	m_pWheelMode->SetRotate(rotX);
}




//-----------------------------------------------------------------------
void CTencentCar::BumpInit()
{
	//模型震动相关
	m_mt3DModel.MakeIdentity();
	m_bIsBumping		= false;
	m_bumpTime.x		= 0;
	m_bumpTime.y		= 0;
	m_bumpTime.z		= 0;
	
	for (int i = 0; i < WHEEL_COUNT; ++i)
	{
		m_arrBumpOffs[i] = 0.0f;
	}
	m_nFullContactBump = 0;
	m_nFullContactBumpTravel = 0.0f;
	m_fFullContactBumpMax = 0.0f;
}

void CTencentCar::TriggerXiaoPen()
{
	//CGameEffectMgr::Get()->ActiveEffect(IGameEffect::ET_ACCLINE,0);
	//NxSceneMgr::Get()->GetCameraController()->SetCameraParamType(CameraController::CPT_SPEEDUP);
	m_fXiaoPenEffectTime = 2.0f;
	NxVec3 vForWard = GetForwardGlobal();
	AddForceGlobal ( vForWard * 3.0f ,EFT_NORMAL, 1.0f);
}

void CTencentCar::CheckXiaoPen( float fTime )
{

	if ( m_fXiaoPenLastTime > 0.0f )
	{
		m_fXiaoPenLastTime -= fTime;
	}

	if ( m_fXiaoPenStartTime > 0.0f )
	{
		m_fXiaoPenStartTime -= fTime;
		return;
	}

	if ( m_fXiaoPenLastTime < 0.0f )
	{
		SetXiaoPen(false);
		return;
	}


	if ( !CanXiaoPen() )
	{
		return;
	}


	if ( m_pDriftCenter->IsDrifting() )
	{
		SetXiaoPen(false);
		m_pDriftCenter->SetTrack(true);
		
	}

	CALL(CNO2Controller::Get(), ActivateNO2());
	SetXiaoPen(false);
}
void CTencentCar::FullContactBump()
{
	m_nFullContactBump = 2;
	m_fFullContactBumpMax = 0.025f;

	for (int i = 0; i < WHEEL_COUNT; ++i)
	{
		m_arrBumpOffs[i] = 0.0f;//-0.07f * NxMath::sin(m_nFullContactBumpTravel);
	}
}

bool CTencentCar::IsDrifting()
{
	return m_pDriftCenter->IsDrifting();
}

NxVec3 CTencentCar::CaculateAirFriction(const NxVec3& v)
{
	NxVec3 vForceRet = -v;

	int nContactStatus = GetContactStatus();
	if (nContactStatus != ECS_INAIR && nContactStatus != ECS_NONECONTACT) 
	{
		vForceRet.z = 0.0f;
	}
	float fScaleVelo = vForceRet.magnitude();
	if (fScaleVelo * GameSetting::SpeedDispCoef < 1.0f)
		vForceRet.zero();
	else
	{
		// 下面是空气阻力的计算
		fScaleVelo = 0.5f * m_fAirDensity * m_fAirCoefC;

		vForceRet.x = vForceRet.x * fabsf(vForceRet.x) * m_fForwardSquard;
		vForceRet.y = vForceRet.y * fabsf(vForceRet.y) * m_fSideSquare;
		vForceRet.z = vForceRet.z * fabsf(vForceRet.z) * m_fUpSquare;

		vForceRet *= fScaleVelo;
	}
	if (nContactStatus == ECS_INAIR || nContactStatus == ECS_NONECONTACT) 
	{
		vForceRet.x *= 0.12f;
	}

	return vForceRet;
}



void  CTencentCar::CheckJumpMotion( float fTime)
{
	if  (m_bJumpIsAccumulating) 
	{
		m_fAltTime += fTime;
		//AddForceGlobal(NxVec3(0.0f,0.0f,15.0f));
	}
	m_fAltTime = __min( 3.0f, m_fAltTime );
}

void  CTencentCar::TirggerJumpMotion()
{
	float fJumpVecSize = m_fAltTime * 5.0f;
	
	if ( m_fAltTime >= 0.00f && !(GetContactStatus() == ECS_INAIR|| GetContactStatus() == ECS_NONECONTACT))
	{
		SetVecZLocal(7.0f);
		ULONG unUin = NxPlayMgr::GetHostPlay()->GetID();
		if ( CMonsterMgr::Get() )
		{
			IGPTarget * pTarget = (IGPTarget*) CMonsterMgr::Get()->GetMonster(0);
			GP_ID gpID;
			gpID.dwID = 3;
			gpID.dwIndex = 3;
			if ( pTarget )
				CGPControl::Get()->OnUseGP(gpID,unUin,0,true,pTarget );
			
		}

		GP_ID gpIDD;
		gpIDD.dwID = 1;
		gpIDD.dwIndex = 1;
		CGPControl::Get()->OnUseGP(gpIDD,unUin,0);
	//	NxSceneMgr::Get()->GetNO2Controller()->ActivateNO2();
			//CGPControl::Get()->OnUseGP(gpID,unUin, 0);
	}
	
	m_fAltTime = 0.0f;
	m_bJumpIsAccumulating = false;
	return;
}

 void CTencentCar::AddDriftForceGlobal( NxVec3 vForce )
{
	NxVec3 vForceLocal = NxVec3 ( 0.0f, 0.0f, 0.0f );
	GlobalToLocal( vForce ,vForceLocal );
	m_pDriftCenter->AddLDF( vForceLocal );
}

//-----------------------------------------------------------------------
 void CTencentCar::AddDriftForceLocal( NxVec3 vForce )
{
	m_pDriftCenter->AddLDF( vForce );
}

//-----------------------------------------------------------------------
bool CTencentCar::IsAccumulate()
{
	return m_pDriftCenter->IsAccumulate();
}

//-----------------------------------------------------------------------
NxReal CTencentCar::GetVecTurnAngle()
{
	return m_pDriftCenter->GetVecTurnAngle();
}
void CTencentCar::SynchronizeSimulator(float dt)
{

	NxVec3 wPos[4];

	NxMat34 trans(m_pController->getActor()->getGlobalOrientation(), m_pController->getActor()->getGlobalPosition());

	NxVec3 posW;
	posW.set(m_fBoxWidth, -m_fBoxLength, -m_fBoxHeight);
	posW = trans * posW;
	posW.z += m_fBoxHeight;//向上移动一些，保证ray的原点总在地面之上
	wPos[0] = posW;


	posW.set(m_fBoxWidth, m_fBoxLength, -m_fBoxHeight);
	posW = trans * posW;
	posW.z += m_fBoxHeight;//向上移动一些，保证ray的原点总在地面之上
	wPos[1] = posW;

	posW.set(-m_fBoxWidth, m_fBoxLength, -m_fBoxHeight);
	posW = trans * posW;
	posW.z += m_fBoxHeight;//向上移动一些，保证ray的原点总在地面之上
	wPos[2] = posW;

	posW.set(-m_fBoxWidth, -m_fBoxLength, -m_fBoxHeight);
	posW = trans * posW;
	posW.z += m_fBoxHeight;//向上移动一些，保证ray的原点总在地面之上
	wPos[3] = posW;

	//NiPhysXTypes::NiPoint3ToNxVec3( m_pCarModel->GetObjectByName("RF_W")->GetWorldTranslate(),wPos[0]);
	//NiPhysXTypes::NiPoint3ToNxVec3( m_pCarModel->GetObjectByName("LF_W")->GetWorldTranslate(),wPos[1]);
	//NiPhysXTypes::NiPoint3ToNxVec3( m_pCarModel->GetObjectByName("LB_W")->GetWorldTranslate(),wPos[2]);
	//NiPhysXTypes::NiPoint3ToNxVec3( m_pCarModel->GetObjectByName("RB_W")->GetWorldTranslate(),wPos[3]);

	//NxVec3 vGround[4];

	//static float height[4];
	for (int i = 0; i < 4; ++i )
	{
//		NxVec3 groundPos;
		NxVec3 pos;// = m_pSimulator->GetWheelPosition(i);

		pos = m_arrCarWheelPool[i]->GetGlobalPos();
//		pos.z += 0.3f;
		bool bRet = Pick(pos, NxVec3(0, 0, -1), i);
	}
	m_pSimulator->SetHeight(m_aPickResults, 4);
	m_pSimulator->SetDistanceToGround(m_aPickDistance);

	//////////////////-----------	
	//NxVec3(m_fBoxLength, m_fBoxWidth, m_fBoxHeight);
	/*static float zs[WHEELS];
	NxMat34 trans(m_pController->getActor()->getGlobalOrientation(), m_pController->getActor()->getGlobalPosition());
	
	NxVec3 posW;
	posW.set(m_fBoxWidth, -m_fBoxLength, -m_fBoxHeight);
	posW = trans * posW;
	zs[0] = posW.z;
		
	posW.set(m_fBoxWidth, m_fBoxLength, -m_fBoxHeight);
	posW = trans * posW;
	zs[1] = posW.z;

	posW.set(-m_fBoxWidth, m_fBoxLength, -m_fBoxHeight);
	posW = trans * posW;
	zs[2] = posW.z;

	posW.set(-m_fBoxWidth, -m_fBoxLength, -m_fBoxHeight);
	posW = trans * posW;
	zs[3] = posW.z;*/

	//m_pSimulator->SetWheelsZ(zs);
	//////////////////-----------
	int nDriftState = 0;

	if (m_pDriftCenter->GetDriftStatus() == CDriftCenter::DRIFT_NONE)
		nDriftState = NOT_DRIFTING;
	else
	{
		if (m_pDriftCenter->GetHeadAngle() > 0.0f)
			nDriftState = LEFT_DRIFTING;
		else
			nDriftState = RIGHT_DRIFTING;
	}

	m_pSimulator->SetLifterValue(nDriftState, m_pDriftCenter->GetHeadAngle());


	//计算一下当前pick到的地面法线：
	//NxVec3 vNormalGrd = (vGround[1] - vGround[2]).cross(
	//	vGround[1] - vGround[0]);
	//vNormalGrd.normalize();

	m_pSimulator->Update(dt, GetVecGlobal(),GetVecLocal(), NxVec3(0,0,1), nDriftState);
}

void CTencentCar::SynchronizeHostPlayerModel(float dt)
{
	if (!m_pCarModel)
		return;

	SetOriGlobal( GetMatGround() );
	//同步显示/物理
	NiMatrix3 matModelGlobal;

	NxMat33   matGlobal	 =	GetOriGlobal();

	NiPhysXTypes::NxMat33ToNiMatrix3( matGlobal,matModelGlobal );
	RotateMatWorldZAxis(matModelGlobal,-NxPi/2.0f);
	//m_pCarModel->SetRotate(matModelGlobal);

	NxVec3    vGlobalPos		= GetPosGlobal();
	//vGlobalPos.z = m_pSimulator->m_pActorChassis->getGlobalPosition().z;
	NiPoint3  vGlobalModelPos	= NiPoint3( vGlobalPos.x,vGlobalPos.y,vGlobalPos.z );

	float offsetZ = GetChassisOffsetZ() + 0.01f;
	
	//漂移的转动
	if( m_pDriftCenter->IsDrifting() )
	{
		//LogText("\n$  -  $ : %f\n", NxMath::sign(m_pDriftCenter->GetHeadAngle())*
		//	m_pDriftCenter->GetHeadAngle(true, true));

		NxReal fAngle = GetCarHeadAngle();
		NxReal fDelta = NxMath::sin( fAngle );

		NxReal fXAlpha = 10.0f * NxPi / 180.0f * fDelta;
		//RotateMatWorldXAxis( matModelGlobal, fXAlpha );
		//LogText("alpha : %f\n", fXAlpha);

		NiMatrix3 rotX;
		rotX.MakeYRotation(fXAlpha);
		matModelGlobal = matModelGlobal* rotX;
		/*NxReal fYAlpha = 5.0f * NxPi / 180.f * fDelta;
		KEYINFO kKeyInfo = GetKeyInfo();
		if (kKeyInfo.bLeft)
		fYAlpha = -fYAlpha; 
		RotateMat33YAxis( matGlobal, fYAlpha);*/

		offsetZ -= 0.1f * fabsf(sinf(fXAlpha));
		
	}
	else
	{
		//if (NxMath::abs(m_fHeadAngleAfterDrift) > 0.001f)
		//{
		//	LogText("\n$  -  $ : %f\n", m_fHeadAngleAfterDrift);
		//	float angle = NxMath::sign(m_fHeadAngleAfterDrift)*
		//		(NxMath::abs(m_fHeadAngleAfterDrift) - m_fFallingDownAngularVel* dt / 1000.0f);

		//	if (m_fHeadAngleAfterDrift * angle < 0.0f)
		//		m_fHeadAngleAfterDrift = 0.0f;
		//	else
		//		m_fHeadAngleAfterDrift = angle;

		//	NxReal fAngle = m_fHeadAngleAfterDrift;
		//	NxReal fDelta = NxMath::sin( fAngle );

		//	NxReal fXAlpha = 10.0f * NxPi / 180.0f * fDelta;
		//	LogText("alpha : %f\n", fXAlpha);
		//	//RotateMatWorldXAxis( matModelGlobal, fXAlpha );
		//	NiMatrix3 rotX;
		//	rotX.MakeYRotation(fXAlpha);
		//	matModelGlobal = matModelGlobal* rotX;
		//	/*NxReal fYAlpha = 5.0f * NxPi / 180.f * fDelta;
		//	KEYINFO kKeyInfo = GetKeyInfo();
		//	if (kKeyInfo.bLeft)
		//	fYAlpha = -fYAlpha; 
		//	RotateMat33YAxis( matGlobal, fYAlpha);*/

		//	offsetZ -= 0.1f * fabsf(sinf(fXAlpha));
		//}
	}

	if (m_bIsDrifting && !m_pDriftCenter->IsDrifting())//漂移刚刚结束
	{
		m_fHeadAngleAfterDrift =m_pDriftCenter->GetHeadAngle();
		//NxMath::sign(m_pDriftCenter->GetHeadAngle())*
		//	m_pDriftCenter->GetHeadAngle(true, true);
		m_fFallingDownAngularVel = NxMath::degToRad(45.0f);//TODO: 角速度的规律？
	}

	m_bIsDrifting = m_pDriftCenter->IsDrifting();
	//~漂移的转动
	
	m_pCarModel->SetRotate(matModelGlobal);


	//if (m_nFullContactBump)
	//{
	//	offsetZ += m_arrBumpOffs[0];
	//}
	vGlobalModelPos -= NiPoint3(0.0f,0.0f,1.0f) * (offsetZ);

	m_pCarModel->SetTranslate(vGlobalModelPos);

	// 处理轮子的转动
	SynchronizeWheel(dt);

	m_pCarModel->Update( 0.0f, false );

	m_fOffsetZ = offsetZ;
}


void CTencentCar::UpdateCarPose()
{
	NxMat33 rot = m_pSimulator->GetChassisOrientationSmooth();

	NiMatrix3 niRot;
	NiPhysXTypes::NxMat33ToNiMatrix3(rot, niRot);

	float fXangle, fYAngle, fZAngle;
	niRot.ToEulerAnglesXYZ(fXangle, fYAngle, fZAngle);

	niRot.FromEulerAnglesZXY(-m_fClockTurnAngle, fXangle, fYAngle);

	NxMat33 orientation; 
	NiPhysXTypes::NiMatrix3ToNxMat33(niRot, orientation);

	//SetOriGlobal(orientation);
	SetMatGround(orientation);
}
bool CTencentCar::Pick(const NxVec3& rayPos, const NxVec3& rayDir, int nIndex)
{

	NxScene* pScene = GetScene();
	NxRay ray(rayPos, rayDir);
	pScene->raycastAllShapes(ray, *m_pRayReport[nIndex], NX_ALL_SHAPES);

	return true;
}


void CTencentCar::DoImpact(const NxVec3& selfVelocity, const NxVec3& selfPos, CTencentCar * other, const NxVec3& otherVelocity, const NxVec3& otherPos)
{
	// 发生了碰撞执行碰撞反应
	NxVec3 kSpeedSelf  = selfVelocity;
	NxVec3 kSpeedOther = otherVelocity;

	NxVec3 kRestSpeedSelf = kSpeedSelf;
	NxVec3 kRestSpeedOther = kSpeedOther;

	NxVec3 kPosSelf = selfPos;
	NxVec3 kPosOther = otherPos;

	// 确定碰撞的法线方向
	NxVec3 kNorm = kPosOther - kPosSelf;
	kNorm.normalize();

	// 将速度在法线方向进行分解
	float fSpeedNormSelf = kSpeedSelf.dot(kNorm);
	float fSpeedNormOther= kSpeedOther.dot(kNorm);

	// 其他方向如果摩擦为０，则保持不变
	// 否则，且向有个衰减
	float fSlideAttenuation = 0.8f;
	kRestSpeedSelf -= fSpeedNormSelf * kNorm;
	kRestSpeedOther-= fSpeedNormOther * kNorm;

	float fTangentSelfVelo = kRestSpeedSelf.magnitude();
	float fTangentOtherVelo= kRestSpeedOther.magnitude();

	kRestSpeedSelf *= (1.0f - fSlideAttenuation * fTangentSelfVelo / (0.5f + fTangentSelfVelo));
	kRestSpeedOther*= (1.0f - fSlideAttenuation * fTangentOtherVelo/ (0.5f + fTangentOtherVelo));

	float fMassSelf  = this->GetMass();
	float fMassOther = other->GetMass();

	float fSpeedSelfRet	 = 0.0f;
	float fSpeedOtherRet = 0.0f;

	// 根据动量守恒定理和能量守恒定理计算碰撞后的速度
	PhysUtility::SolveLinearGlobalCollision(fSpeedNormSelf,  fMassSelf,  fSpeedSelfRet,
		fSpeedNormOther, fMassOther, fSpeedOtherRet);


	// 得到碰撞后的速度
	kRestSpeedSelf += fSpeedSelfRet * kNorm;
	kRestSpeedOther+= fSpeedOtherRet* kNorm;


	// 回设
	this->SetVecGlobal(kRestSpeedSelf);
	other->SetVecGlobal(kRestSpeedOther);
	LogText( "RemotePlay Collision Speed x = %f, y = %f, z = %f \n", kRestSpeedOther.x,kRestSpeedOther.y,kRestSpeedOther.z );
}

void CTencentCar::CollisionDetectionFromOtherPlayer(DWORD unID, CTencentCar * other, const NxVec3 & otherVelocity, const NxVec3 & otherPos, const NxMat33 &otherMat)
{
	const float collisionRadius = 0.35f;

	NxVec3 velocity = m_vVecGlobal;
	NxExtendedVec3 tmpPos = m_pController->getPosition();
	NxVec3 pos = NxVec3(tmpPos.x, tmpPos.y, tmpPos.z);

	// 首先做第一级碰撞检测
	NxVec3 realPos = otherPos - pos;
	float len = sqrtf(realPos.x*realPos.x + realPos.y*realPos.y);
	if (len > (m_fCollisionRadius + other->GetCollisionRadius() ))
		return;

	// 做第二级碰撞检测
	realPos.normalize();
	NxVec3 realVec = otherVelocity - velocity;
	float dotVec = realPos.dot(realVec);
	if (dotVec > 0.0f || fabsf(dotVec) < 0.1f)
		return;

	// 做第三级精确的碰撞检测 矩形间的碰撞检测
	NxVec3 extern0 = NxVec3(m_fCollisionLength, m_fCollisionWidth, m_fCollisionHeight);
	NxVec3 extern1 = NxVec3(other->GetCollisionLenght(), other->GetCollisonWidth(), other->GetCollisonHeight());

	if (!PhysUtility::RectCollision(extern0, pos, GetOriGlobal(), extern1, otherPos, otherMat))
		return;

	// 检查发来的封包是否产生重复碰撞
	if (!CLevelManager::Get())
		return;

	float colTime =  CLevelManager::Get()->GetLevelTime();
	LogText("车车碰撞前检测<=== %f %f %f %f %f %f %f\n", colTime, velocity.x, velocity.y, velocity.z, otherVelocity.x, otherVelocity.y, otherVelocity.z);
	if (!IsCanCollisionDetection(unID, colTime, pos, velocity, otherPos, otherVelocity))
		return;

	LogText("发生了车车碰撞===> %f %f %f %f %f %f %f\n", colTime, velocity.x, velocity.y, velocity.z, otherVelocity.x, otherVelocity.y, otherVelocity.z);
	DoImpactRot();
	DoImpact(velocity, pos, other, otherVelocity, otherPos);

	// 发送碰撞封包给对方
	STUDPPlayCollision packet;
	memset(&packet,sizeof(STUDPPlayCollision),0);
	packet.m_unSelfUin = NxPlayMgr::GetHostPlay()->GetID();
	packet.m_nSelfLinearVel[0] = velocity.x * SCALE_FACTOR;
	packet.m_nSelfLinearVel[1] = velocity.y * SCALE_FACTOR;
	packet.m_nSelfLinearVel[2] = velocity.z * SCALE_FACTOR;
	packet.m_nSelfPos[0] = pos.x * SCALE_FACTOR;
	packet.m_nSelfPos[1] = pos.y * SCALE_FACTOR;
	packet.m_nSelfPos[2] = pos.z * SCALE_FACTOR;
	packet.m_unOtherUin = unID;
	packet.m_nOtherLinearVel[0] = otherVelocity.x * SCALE_FACTOR;
	packet.m_nOtherLinearVel[1] = otherVelocity.y * SCALE_FACTOR;
	packet.m_nOtherLinearVel[2] = otherVelocity.z * SCALE_FACTOR;
	packet.m_nOtherPos[0] = otherPos.x * SCALE_FACTOR;
	packet.m_nOtherPos[1] = otherPos.y * SCALE_FACTOR;
	packet.m_nOtherPos[2] = otherPos.z * SCALE_FACTOR;
	packet.m_unTime	= CLevelManager::Get()->GetLevelTime() * SCALE_FACTOR;

	NxGamePkgMgr::Get()->SendUdpPkgImmt((void *)&packet, NxGamePkgMgr::EPT_PLAYCOLLISON);
}

bool CTencentCar::IsCanCollisionDetection(DWORD id, float time, const NxVec3& selfPos, const NxVec3& selfVec, 
										  const NxVec3& otherPos, const NxVec3& otherVec)
{
	for(int i = 0; i<8; i++)
	{
		if (id == m_vCollisionData[i].id)
		{
			if (fabsf(time - m_vCollisionData[i].nearCollisionTime) < 0.5f)
			{
				if (time < m_vCollisionData[i].nearCollisionTime)
				{
					m_vCollisionData[i].nearCollisionTime = time;

					m_vCollisionData[i].collisionTime = time;
					m_vCollisionData[i].selfPos = selfPos;
					m_vCollisionData[i].otherPos = otherPos;
					m_vCollisionData[i].selfVec = selfVec;
					m_vCollisionData[i].otherVec = otherVec;
					return true;
				}
				else
				{
					return false;
				}
			}
			else
			{
				m_vCollisionData[i].nearCollisionTime = time;

				m_vCollisionData[i].collisionTime = time;
				m_vCollisionData[i].selfPos = selfPos;
				m_vCollisionData[i].otherPos = otherPos;
				m_vCollisionData[i].selfVec = selfVec;
				m_vCollisionData[i].otherVec = otherVec;
				return true;
			}
		}
	}

	// 如果碰撞者不存在
	for(int i = 0; i<8; i++)
	{
		if (m_vCollisionData[i].id == -1)
		{
			m_vCollisionData[i].nearCollisionTime = time;
			m_vCollisionData[i].id = id;

			m_vCollisionData[i].collisionTime = time;
			m_vCollisionData[i].selfPos = selfPos;
			m_vCollisionData[i].otherPos = otherPos;
			m_vCollisionData[i].selfVec = selfVec;
			m_vCollisionData[i].otherVec = otherVec;
			return true;
		}
	}

	return false;
}

bool CTencentCar::IsCanPacketCollisionDetection(DWORD id, float time, const NxVec3& selfPos, const NxVec3& selfVec, 
												const NxVec3& otherPos, const NxVec3& otherVec)
{
	for(int i = 0; i<8; i++)
	{
		if (id == m_vCollisionData[i].id)
		{
			if (fabsf(time - m_vCollisionData[i].nearCollisionTime) < 0.5f)
			{
				if (time < m_vCollisionData[i].nearCollisionTime)
				{
					m_vCollisionData[i].nearCollisionTime = time;
					m_vCollisionData[i].id = id;

					m_vCollisionData[i].collisionTime = time;
					m_vCollisionData[i].selfPos = selfPos;
					m_vCollisionData[i].otherPos = otherPos;
					m_vCollisionData[i].selfVec = selfVec;
					m_vCollisionData[i].otherVec = otherVec;

					return true;

				}
				else
				{
					return false;
				}

			}
			else
			{
				m_vCollisionData[i].nearCollisionTime = time;

				m_vCollisionData[i].collisionTime = time;
				m_vCollisionData[i].selfPos = selfPos;
				m_vCollisionData[i].otherPos = otherPos;
				m_vCollisionData[i].selfVec = selfVec;
				m_vCollisionData[i].otherVec = otherVec;
				return true;
			}
		}
	}

	// 如果碰撞者不存在
	for(int i = 0; i<8; i++)
	{
		if (m_vCollisionData[i].id == -1)
		{
			m_vCollisionData[i].nearCollisionTime = time;
			m_vCollisionData[i].id = id;

			m_vCollisionData[i].collisionTime = time;
			m_vCollisionData[i].selfPos = selfPos;
			m_vCollisionData[i].otherPos = otherPos;
			m_vCollisionData[i].selfVec = selfVec;
			m_vCollisionData[i].otherVec = otherVec;
			return true;
		}
	}

	return false;

}

void CTencentCar::RecvCollisionPacket(void *p)
{
	STUDPPlayCollision* pPacket = (STUDPPlayCollision*)p;
	if (pPacket->m_unOtherUin != NxPlayMgr::GetHostPlay()->GetID())
		return;

	NxVec3 selfVec, selfPos, otherVec, otherPos;
	DWORD otherID = pPacket->m_unSelfUin;
	CTencentCar *other = NULL;
	if (NxPlayMgr::GetPlay(otherID) && NxPlayMgr::GetPlay(otherID)->GetKart())
		other = NxPlayMgr::GetPlay(otherID)->GetKart()->GetVehicle();

	if (!other)
		return;

	float collisionTime = pPacket->m_unTime / SCALE_FACTOR;
	selfVec.x = pPacket->m_nOtherLinearVel[0] / SCALE_FACTOR;
	selfVec.y = pPacket->m_nOtherLinearVel[1] / SCALE_FACTOR;
	selfVec.z = pPacket->m_nOtherLinearVel[2] / SCALE_FACTOR;
	selfPos.x = pPacket->m_nOtherPos[0] / SCALE_FACTOR;
	selfPos.y = pPacket->m_nOtherPos[1] / SCALE_FACTOR;
	selfPos.z = pPacket->m_nOtherPos[2] / SCALE_FACTOR;
	otherVec.x = pPacket->m_nSelfLinearVel[0] / SCALE_FACTOR;
	otherVec.y = pPacket->m_nSelfLinearVel[1] / SCALE_FACTOR;
	otherVec.z = pPacket->m_nSelfLinearVel[2] / SCALE_FACTOR;
	otherPos.x = pPacket->m_nSelfPos[0] / SCALE_FACTOR;
	otherPos.y = pPacket->m_nSelfPos[1] / SCALE_FACTOR;
	otherPos.z = pPacket->m_nSelfPos[2] / SCALE_FACTOR;

	LogText("收到封包碰撞前检测<=== %f %f %f %f %f %f %f\n", collisionTime, selfVec.x, selfVec.y, selfVec.z, otherVec.x, otherVec.y, otherVec.z);

	if (!IsCanPacketCollisionDetection(otherID, collisionTime, selfPos, selfVec, otherPos, otherVec))
		return;

	LogText("收到封包发生了碰撞===> %f %f %f %f %f %f %f\n", collisionTime, selfVec.x, selfVec.y, selfVec.z, otherVec.x, otherVec.y, otherVec.z);
	DoImpact(selfVec, selfPos, other, otherVec, otherPos);
}

NxVec3 CTencentCar::SwooshRobot( NxVec3 vValue )
{
	if ( m_arrShooshRobot.size() > 5 ) 
	{
		m_arrShooshRobot.pop();
	}

	m_arrShooshRobot.push(vValue);
	NxVec3 fFinalValue(0.0f,0.0f,0.0f);

	std::queue<NxVec3> arrTemp; 
	while ( !m_arrShooshRobot.empty() )
	{
		NxVec3 fValue = m_arrShooshRobot.front();
		fFinalValue += fValue;
		m_arrShooshRobot.pop();
		arrTemp.push(fValue);
	}

	m_arrShooshRobot = arrTemp;
	fFinalValue = fFinalValue / m_arrShooshRobot.size();

	return fFinalValue;

}

bool CTencentCar::InitRaycastCallback()
{
	for (int i = 0; i < 4; i++)
	{
		m_aPickResults[i] = 0.0f;
		m_pRayReport[i] = new myRaycastReport(this, i);
		if (!m_pRayReport[i])
			return false;
	}

	return true;
}
void CTencentCar::ReleaseRaycastCallback()
{
	for (int i = 0; i < 4; i++)
	{
		SAFE_DELETE(m_pRayReport[i]);
	}
}

bool myRaycastReport::onHit(const NxRaycastHit& hit)
{
	//NxActor &hitActor = hit.shape->getActor();
	XUserData* pD = (XUserData*)hit.shape->getActor().userData;
    
	if ( !pD )
	{
		LogText(" error myRaycastReport::OnHit XUserData = NULL \n");
		return false;
	}

	if (pD->_actorType == XUserData::AT_TERRAIN)
	{
		m_pTCar->m_aPickResults[m_nIndex] = hit.worldImpact.z;
		m_pTCar->m_aPickNormalsPre[m_nIndex] = m_pTCar->m_aPickNormalsCur[m_nIndex];
		m_pTCar->m_aPickNormalsCur[m_nIndex] = hit.worldNormal;
		m_pTCar->m_aPickDistance[m_nIndex] = hit.distance - m_pTCar->m_fBoxHeight;//射线的顶点在pick时被移高，现在移回来
		//LogText("\n !!!!!!!!!!!!!!!!!!!!!!!!pick到地面！！！\n");
	}
	//else
	//{
	//	ASSERT(0);
	//	//LogText("\n 没pick到地面，加宽！！！\n");
	//}

	return true;
}

void CTencentCar::SetModalFall()
{
	if (!m_pPlay->GetKart()->IsKartMoveable() && !m_pPlay->GetKart()->IsGameFinish())
		return;

	m_bIsFall	= true;
	m_bIsFallPoseAdjust = false;

	// 设置模型弹起的速度和位置
	NxVec3 vec = GetVecGlobal();
	if (vec.z <= -18.0f)
		vec.z = -18.0f;

	m_vModalVelocity	= NxVec3(0.0f, 0.0f, vec.z * 0.6f);
	m_vModalPos			= NxVec3(0.0f, 0.0f, 0.0f);
}

float CTencentCar::ProcessModalFall(float dt)
{
	// 检测是否真正落地
	{
		NxVec3 pos = GetPosGlobal();
		pos.z -= m_fBoxHeight;
		NxRay ray(pos, -GetUpGlobal());
		NxRaycastHit hit;
		NxShape *pShootShape = m_pScene->raycastClosestShape(ray, NX_ALL_SHAPES,hit, PhysXManager::CG_ENVIRONMENT);
		if ( pShootShape )
		{
			if (hit.distance >= (SKINWIDTH + 0.01f))
				return 0.0f;
		}				
	}

	// 如果速度为0，则下落晃动模型结束
	float vecLen = m_vModalVelocity.magnitude();
	float posLen = m_vModalPos.magnitude();

	if (fabsf(vecLen) < 0.1f && fabsf(posLen) < 0.01f)
	{
		m_bIsFall = false;
		return 0.0f;
	}

	// 否则更新速度和更新位置
	const NxVec3 G = NxVec3(0.0f, 0.0f, -10.0f);
	NxVec3 newVelocity = m_vModalVelocity + G * dt;
	NxVec3 newPos = m_vModalPos + (newVelocity + m_vModalVelocity) * dt / 2.0f;
	if (newPos.z <= 0.0f)
	{
		m_vModalVelocity = NxVec3(0.0f, 0.0f, -m_vModalVelocity.z * 0.45f);
		m_vModalPos = NxVec3(0.0f, 0.0f, 0.0f);
	}
	else
	{
		m_vModalVelocity = newVelocity;
		m_vModalPos = newPos;
	}

	return -newPos.z;
}

void CTencentCar::GetWheelBumpInfo(int nIndex, BUMPINFO& bumpInfo)
{
	int nContactStatus = GetContactStatus();
	if ( nContactStatus == ECS_INAIR || nContactStatus == ECS_NONECONTACT)
		bumpInfo.nPitch = 0;
	else
	{
		NxVec3 vNPre, vNCur;
		vNCur = m_aPickNormalsCur[nIndex];
		vNPre = m_aPickNormalsPre[nIndex];

		NxReal angle = NxMath::acos(vNPre.dot(vNCur));
		if (angle > NxMath::degToRad(1.0f))
		{
			bumpInfo.fAngle = angle;

			//判断车上了一个坡，还是下了一个坡
			NxVec3 velG = GetVecGlobal();

			NxVec3 vCross = vNPre.cross(vNCur);
			vCross.normalize();
			NxVec3 vCross2 = vNPre.cross(velG);
			vCross2.normalize();

			float fTheta = NxMath::acos(vCross.dot(vCross2));

			if (fTheta > NxMath::degToRad(90.0f))
			{
				bumpInfo.nPitch = 1;//上坡
			}
			else
			{
				bumpInfo.nPitch = -1;//下坡
			}
		}
		else
		{
			bumpInfo.fAngle = 0.0f;
			bumpInfo.nPitch = 0;
		}
	}
}

void CTencentCar::SetCarFallPose()
{
	// 如果在漂移状态，则不做车头的动作
	if (m_pDriftCenter->IsDrifting())
		return;

	// 设置在空中的状态
	float fForward = 1.0f;
	int nStatus = GetCarMotionStatus();
	if (nStatus == ECMD_STOP)
		return;
	if (nStatus == ECMD_BACK)
		fForward = -1.0f;

	m_bIsFallPoseAdjust = true;	
	m_fMaxFallAngle = 30.0f * fForward;
	m_fFallAngleVelocity = 40.0f * fForward;

	float angle = 0.0f;
	for(int i = 0; i < m_queAngle.size(); i++)
	{
		if (angle < m_queAngle[i])
			angle = m_queAngle[i];
	}

	m_fFallAngle = -angle * fForward;
}

float CTencentCar::ProcessCarFallPoseRotY(float dt)
{
	if (m_queAngle.size() >= 10)
		m_queAngle.pop_front();

	NxVec3 up  = m_matGround * NxVec3(0, 0, 1);
	up.normalize();
	m_queAngle.push_back(NxMath::radToDeg(acos(up.dot(NxVec3(0, 0, 1)))));

	if (!m_bIsFallPoseAdjust)
		return 0.0f;

	m_fFallAngle += m_fFallAngleVelocity * dt;
	
	if (m_fFallAngle > m_fMaxFallAngle)
		m_fFallAngle = m_fMaxFallAngle;

	return NxMath::degToRad(m_fFallAngle);
}

void CTencentCar::DoImpactRot()
{

}

