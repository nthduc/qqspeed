#pragma once

/*-------------------------------------------------------------------
// Create by HoverZhao 2006.1.21
--------------------------------------------------------------------*/

//-----------------------------------------------------------------------

#include <vector>
#include <math.h>
#include <NxPhysics.h>
#include <NxScene.h>
#include "NxBoxController.h"
#include "NxCapsuleController.h"
#include "NxController.h"
#include <queue>

#include "TencentCarCollision.h"
#include "CollisionBase.h"
#include "ChassisSimulator.h"

//-----------------------------------------------------------------------
#define DD_ACCEL		99998
#define DD_JIQI_ID		99999

#define DD_NORMAL_NO2	1
#define DD_XIAOPEN		2001
#define DD_TEAM_NO2		2002


//-----------------------------------------------------------------------

#define DGAngle(x) (x / 180.0f ) * NxPi
#define GDAngle(x) (x * 180.0f ) / NxPi

#define GRAVITYACCEL -20.0f

//-----------------------------------------------------------------------

#define			MAXWHEELNUM			4

#define			TINY_DIST          0.18f
#define			MAXDISTANCE		   1000.0f 
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

#define         NUM_WHEELS		5
#define			SKINWIDTH	0.05f


#define			MIN_DIS					0.1f
#define			CONTACTGRD				1
#define			NONE_CONTACTGRD			2

#define			DRIFT_LEFT				-1
#define			DRIFT_RIGHT				1

#define			SIMPLE_DRIFT			1
#define			DRIFT_SIDE_ANGLE		15.0f

#define CHECKINIT(x)	\
	LogText("CTencentCar::%s",x);	\
	assert(0);	\
	bReturn = false;


typedef struct tagBumpInfo{
	int nPitch;// -1, 下坡， 1，上坡, 0, 无变化
	float fAngle;//角度
}BUMPINFO;

//-----------------------------------------------------------------------

class CCarWheel;
class CCarMotorControl;
class CDriftCenter;
class CCollisionCenter;
class CCarFenceCollision;
class CCarCarCollision;
class Play;
//class ChassisSimulator;
class myRaycastReport;
class Suspension;

class CTencentCar
{
public:
	CTencentCar(NxScene* pScene, NiAVObject* pModel, Play * pPlay);
	virtual ~CTencentCar(void);

	friend class CCarWheel;
	friend class CCollisionCenter;
	friend class CCarCarCollision;
	friend class CCarFenceCollision;

	typedef enum enmForceType
	{
		EFT_NORMAL = 0,
		EFT_GRAVITY,
		EFT_FRICTSLID,
		EFT_FRICTROLL,
		EFT_MAGNET,
		EFT_DRIFT,
		EFT_PULL,
		EFT_ONLYONE,

	}ENMFORCETYPE;

	typedef enum enmCarMotionDir
	{
		ECMD_LEFT		= (1<<0), // 正左转呢
		ECMD_RIGHT		= (1<<1), // 正右转呢
		ECMD_BACK		= (1<<2), // 正后退呢
		ECMD_FORWARD	= (1<<3), // 正前进呢
		ECMD_STOP       = (1<<5), // 呆着凉快呢

	}ENMMOTIONDIR;

	// 以本地坐标系为准 锁定 车体的运动 和 转动
	typedef enum enumLockMotion
	{
		ELM_ROTATE_XAXIS     = (1<<0),
		ELM_ROTATE_YAXIS	 = (1<<1),
		ELM_ROTATE_ZAXIS	 = (1<<2),
		ELM_MOVE_XAXIS		 = (1<<3),
		ELM_MOVE_YAXIS		 = (1<<4),
		ELM_MOVE_ZAXIS		 = (1<<5),
		ELM_LOCKALL			 =  0x3f ,
		ELM_LOCKNONE		 =  0	 ,

	}ENMLOCKMOTION;

	// 绕那个边旋转呢
	typedef enum enmRotateSideAxis
	{
		ERSA_LEFT		 = (1<<0),
		ERSA_RIGHT		 = (1<<1),
		ERSA_UP  		 = (1<<2),
		ERSA_DOWN 		 = (1<<3),
		ERSA_NONE 		 =  0,
	};


protected:
	void SetRotateSideAxis ( int nRotateSideAxis ) 
	{
		m_nRotateSideAxis = nRotateSideAxis;
	}
	int GetRotateSideAxis () { return m_nRotateSideAxis;}

private:
	int  m_nRotateSideAxis;
	float		m_fGravity;

	// 碰撞检测
public:
	void CollisionDetectionFromOtherPlayer(DWORD unID, CTencentCar * other, const NxVec3 & otherVelocity, const NxVec3 & otherPos, const NxMat33 &otherMat);
	void RecvCollisionPacket(void *p);
	void DoImpact(const NxVec3& selfVelocity, const NxVec3& selfPos, CTencentCar * other, const NxVec3& otherVelocity, const NxVec3& otherPos);
	void DoImpactRot();

	// 避震
private:
	bool		m_bIsFall;
	NxVec3		m_vModalVelocity;
	NxVec3		m_vModalPos;
	float		ProcessModalFall(float dt);
	
	// 车身下落的姿态调整
	bool		m_bIsFallPoseAdjust;
	float		m_fMaxFallAngle;
	float		m_fFallAngle;
	float		m_fFallAngleVelocity;
	float		ProcessCarFallPoseRotY(float dt);

	std::deque<float>   m_queAngle;

public:
	void		SetModalFall();
	void		SetCarFallPose();

private:
	typedef struct CollisionData
	{
		DWORD id;
		float nearCollisionTime;

		float collisionTime;
		NxVec3 selfPos;
		NxVec3 selfVec;
		NxVec3 otherPos;
		NxVec3 otherVec;
	};
	CollisionData m_vCollisionData[8];
	bool IsCanCollisionDetection(DWORD id, float time, const NxVec3& selfPos, const NxVec3& selfVec, 
		const NxVec3& otherPos, const NxVec3& otherVec);
	bool IsCanPacketCollisionDetection(DWORD id, float time, const NxVec3& selfPos, const NxVec3& selfVec, 
		const NxVec3& otherPos, const NxVec3& otherVec);


public:
	typedef struct tagForce
	{
		NxVec3 vForce;
		DWORD  dwTime;					//毫秒
		int	   eType;
	}FORCE,	*PFORCE ;
	
	typedef struct tagTwist
	{
		NxVec3	vTwist;
		DWORD	dwTime;
	}TWIST, *PTWIST;

	typedef struct tagKeyInfo
	{
		bool bUp;
		bool bDown;
		bool bRight;
		bool bLeft;
		bool bShift;
		bool bCtrl;

	}KEYINFO, *PKEYINFO;

	typedef struct tagPhysInfo
	{
		DWORD dwPID;
		DWORD dwAffectID;
		float fParam;
		float fLifeTime;

	}PHYSINFO, *PPHYSINFO;

	typedef struct tagCompensateInfo
	{
		float fPercent;
		float fValue;
	}COMPENSTATEINFO, *PCOMPENSATEINFO;


// 添加车体受力 // 默认为一帧处理
public :
	void AddForceLocal   ( NxVec3 vForce ,int eType = 0,float fTime = 0.0f);		 // 单位秒
	void AddForceXLocal  ( NxReal fForce ,int eType = 0,float fTime = 0.0f);
	void AddForceYLocal  ( NxReal fForce ,int eType = 0,float fTime = 0.0f);
	void AddForceZLocal  ( NxReal fForce ,int eType = 0,float fTime = 0.0f);

	void AddForceGlobal	 ( NxVec3 vForce ,int eType = 0,float fTime = 0.0f);
	void AddForceXGlobal ( NxReal fForce ,int eType = 0,float fTime = 0.0f);
	void AddForceYGlobal ( NxReal fForce ,int eType = 0,float fTime = 0.0f);
	void AddForceZGlobal ( NxReal fForce ,int eType = 0,float fTime = 0.0f);

	void GlobalToLocal ( NxVec3 vGlobal, NxVec3 & vLocal  );
	void LocalToGlobal ( NxVec3 vLocal , NxVec3 & vGlobal );
	NxReal GetForceXLocal();
	NxReal GetForceYLocal();
	NxReal GetForceZLocal();

	NxReal GetForceXGlobal();
	NxReal GetForceYGlobal();
	NxReal GetForceZGlobal();


	void AddDriftForceLocal ( NxVec3 vForce );
	void AddDriftForceGlobal( NxVec3 vForce );

// 添加车体扭矩 --右手螺旋法则
public:
	void AddTwistLocal	( NxVec3 vTwist ,float fTime = 0.0f); 
	void AddTwistXLocal ( NxReal fTwist ,float fTime = 0.0f);
	void AddTwistYLocal ( NxReal fTwist ,float fTime = 0.0f);
	void AddTwistZLocal ( NxReal fTwist ,float fTime = 0.0f);

	void AddTwistGlobal	 ( NxVec3 vTwist ,float fTime = 0.0f);
	void AddTwistXGlobal ( NxReal fTwist ,float fTime = 0.0f);
	void AddTwistYGlobal ( NxReal fTwist ,float fTime = 0.0f);
	void AddTwistZGlobal ( NxReal fTwist ,float fTime = 0.0f);

// 车体位置函数
public:
	NxVec3	GetPosGlobal();
	void	SetPosGlobal( NxVec3 vPos );
	
	NxVec3  GetBackPosGlobal();

// 方向函数
    NxMat33 GetOriGlobal();
	void	SetOriGlobal( NxMat33 matOri);

	NxVec3  GetForwardGlobal();
	NxVec3  GetUpGlobal();
	NxVec3  GetLeftGlobal();

protected:
	void    SetUpGlobal			( NxVec3 vUp );
	void	SetLeftGlobal		( NxVec3 vLeft);
	void    SetForwardGlobal	( NxVec3 vForward);

// 线速度函数
public:	
	NxVec3	GetVecGlobal();
	NxReal  GetVecXGlobal();
	NxReal  GetVecYGlobal();
	NxReal  GetVecZGlobal();

	void	SetVecGlobal  ( NxVec3 vVec );
	void    SetVecXGlobal ( NxReal fVec );
	void    SetVecYGlobal ( NxReal fVec );
	void    SetVecZGlobal ( NxReal fVec );

public: 
	NxVec3 GetVecLocal();
	NxReal GetVecXLocal();
	NxReal GetVecYLocal();
	NxReal GetVecZLocal();

	void   SetVecLocal ( NxVec3 vVec );
	void   SetVecXLocal( NxReal fVec );
	void   SetVecYLocal( NxReal fVec );
	void   SetVecZLocal( NxReal fVec );

// 角速度函数		正值为逆时针 正值为顺时针 右手法则
public: 
	NxVec3 GetWecGlobal();
	NxReal GetWecXGlobal();
	NxReal GetWecYGlobal();
	NxReal GetWecZGlobal();
	
	void   SetWecGlobal( NxVec3 vWec );
	void   SetWecXGlobal ( NxReal fWec );
	void   SetWecYGlobal ( NxReal fWec );
	void   SetWecZGlobal ( NxReal fWec );

	NxVec3 GetWecLocal();
	NxReal GetWecXLocal();
	NxReal GetWecYLocal();
	NxReal GetWecZLocal();

	void   SetWecLocal ( NxVec3 vWec );
	void   SetWecXLocal ( NxReal fWec );
	void   SetWecYLocal ( NxReal fWec );
	void   SetWecZLocal ( NxReal fWec );

public:
	// 轮角度处理函数
	NxReal GetWheelTurnAngle	()		{ return m_fWheelTurnAngle; } 
	NxReal UpdateWheelTurnAngle	(KEYINFO stKeyInfo, float fTime);

protected:
	NxReal m_fMaxWheelTurnAngle;
	NxReal m_fWheelTurnAngle;
	NxReal m_fWheelTurnVec;

// 重力影响函数
public:
	NxVec3 GetGravity()						{ return m_vGravity; } 
	void   SetGravity(NxVec3 vGravity)		{ m_vGravity = vGravity; } 
	NxReal GetGravityValue()				{ return m_fGravity;}
	void   SetGravityValue(float value)		{ m_fGravity = value; }

//忽略所有外力作用函数
//
	void IgnoreForce();
	void IgnoreTwist();

// 锁定函数
	void LockMotion ( int eLockType)          { m_eLockStatus = eLockType ;} 

	void UnLockMotion ( int eLockType )
	{
		eLockType =  ~eLockType;
		m_eLockStatus =  m_eLockStatus & eLockType;
	}


// 漂移 远程玩家处理函数
// TODO: 临时的写法
	bool	IsDrifting() ;
	void    StopDrift() { ; }

	bool    IsAccumulate() ;




public:
	NxReal GetCarHeadAngle(bool bAbs = false, bool bRui = false);
	NxReal GetPlaneVecSize();
	NxReal GetArmForce() { return m_fArmForce;}

	NxReal	GetCarBodyAngle() { return m_fCarBodyAngle; }
	void	SetCarBodyAngle( NxReal fAngle ) { m_fCarBodyAngle = fAngle ;}
	NxReal	m_fCarBodyAngle;

protected:
	NxReal m_fArmForce;
	float  m_fCompensateParam;

	std::vector<COMPENSTATEINFO> m_vecComPensateTable;

// 默认摩擦力不变 ( 摩擦分为滚动摩擦 和侧摩擦 ) 速度方向的摩擦力大小 ＝ 滚动摩擦 ＋ 测摩擦*sin(alpha)
public:
	void	SetFrictionFactor( NxReal fFactor)			{ m_fFricFactor = fFactor; }
	void    SetSlipFrictionForce( NxReal fSlipForce )	{ m_fSlipFrictionForce = fSlipForce; }
	void	SetRollFrictionForce( NxReal fRollForce )	{ m_fRollFrictionForce = fRollForce; }
	NxReal  GetRollFrictionForce()						{ return m_fRollFrictionForce;}
	NxReal  GetSlipFrictionForce() 
	{ 
		return m_fSlipFrictionForce ;
	}

	void  ProcessCompensateValue	(float fValue ) ;


	


public:
	NxReal  GetMass ()						{ return m_fMass;  }
	void    SetMass ( NxReal fMass )		{ m_fMass = fMass; }

// 返回值在0 到 Pi 之间 以Forward 为起点 旋转到 Vel 的角度 平面的情况才是对的
public:
	NxReal  GetAngleBetweenForwardAndVel();

	int     GetCarMotionStatus();  

public:
// 特殊震动处理函数 
	typedef enum enmShakeDir
	{
		ESD_FRONT,
		ESD_LEFT,
		ESD_RIGHT,
		ESD_BEHIND,

	}ENMSHAKEDIR;

	typedef enum enmContactStatus
	{
		ECS_INAIR,
		ECS_NONECONTACT,
		ECS_ONECONTACT,
		ECS_TWOCONTACT,
		ECS_TREECONTACT,
		ECS_FULLCONTACT,

	}ENMCONTACTSTATUS;

// 可以直接播放动画
public:
	void	AddShake(ENMSHAKEDIR eDir);
	NxVec3	GetTerrainNormal();

// 处理流程函数
public:
	bool InitTecentCar( NxVec3 vPos);
protected:
	bool InitController( NxVec3 vPos);
	bool InitCarWheel  ( NxVec3 vPos);
	bool InitAccelDrift();
	bool InitCarUserDate();
	void ReleaseCarUserData();
	// 初始化汽车引擎
	// 初始化碰撞处理器
	bool InitCollisionProcessors();
	bool InitRaycastCallback();
	void ReleaseRaycastCallback();

public:
	bool InitCarPhysParam();
	bool InitCarMotor();

public:

	void UpdateTencentCar(DWORD dt,KEYINFO stKeyInfo);
	void RemoteCarUpdate (DWORD dt);
	void UpdateRemoteCarVec(DWORD dt);
	void RemoteUpdateGravity( DWORD dt);
	void UpdateRemoteLineMovement(DWORD dt);

	void SynchronizeModel(float dt = 0.1f, bool hostPlayer = true);
	void RemoteSynchronizeModel( float dt = 0.1f);



// 
public:
	void TirggerJumpMotion	( );
	void CheckJumpMotion	( float fTime);
	void SetJumpAccumulate () { m_bJumpIsAccumulating = true;}
	bool IsJumpAccumulate  () { return m_bJumpIsAccumulating ; } 

protected:
	float m_fAltTime;
	bool  m_bJumpIsAccumulating;

public:
	//NxReal GetVecTurnAngle();
	NxReal GetVecTurnAngle();


public:
		CCarWheel *  GetWheel(int i )
		{
			if ( i > m_arrCarWheelPool.size() ) 
				return NULL;
			return m_arrCarWheelPool[i];
		}
protected:
	// 转动轮子
	void SynchronizeWheel(float dt);

protected:
	void UpdatePhysParam ();

	void UpdateGravity	(DWORD dt);
	void UpdateFriction	(DWORD dt);
	void UpdateCarVec	(DWORD dt);
	void UpdateCarWec	(DWORD dt);
	void UpdateArm      (DWORD dt);

	// 检查车身锁定 的情况
	void CheckLock();

	bool CheckDrifting();
	bool CheckAcceing();


	void UpdateTurnMovement	(DWORD dt);
	void UpdateLineMovement (DWORD dt);

protected:
	void ClearForcePool() ;
	void ClearTwistPool() ;
	void TriggerForceTwistPool(DWORD dt);
	NxVec3 GetTotalForce()		{ return m_vTotalForce; }
	NxVec3 GetTotalTwist()		{ return m_vTotalTwist; }	
	NxVec3 GetCurrentInputForce();
	
	void   SetTotalForce(NxVec3 vTForce);
	void   SetTotalTwist(NxVec3 vTTwist);


protected:
	DWORD GetDeltaTime () { return m_dwDeltaTime;}
	void  SetDeltaTime ( DWORD dwDelatTime ) { m_dwDeltaTime = dwDelatTime; } 

// 速度和角速度的关系 假设出来的
protected:
	NxReal BornWecFromVec( NxVec3 vVec, float fTime );
	NxReal BornWec( NxVec3 vVec );
	NxReal BornPoseWec(NxVec3 vVec,float fTime);

	// 根据地表 设置车身的调整角度
public:
	void  SetRotateAngleFromGround(NxVec3 vAngle)	{ m_vAngleFromGround = vAngle ; } 
	

protected:
	NxVec3 m_vAngleFromGround;
	

	// 直线运动

	// 曲线运动

	// 加速运动

	// 漂移运动
protected:
	void UpdateWheel(DWORD dt);

	void AdjustCarPos				( std::vector<int> & vecPrivotPoint );
	void AdjustCarPosTwoPrivot      ( std::vector<int> & vecPrivotPoint );
	void AdjustCarPosOnePrivot		( std::vector<int> & vecPrivotPoint );
	void CaculatePrivot				( std::vector<int> & vecPrivotPoint );
	void CaculatePrivotTwistForce	( std::vector<int> & vecPrivotPoint );
	
	// 计算轮子的转动
	void CaculateWheelRotate(DWORD ddt);

public:
	int   GetContactStatus()				{ return m_nContactStatus;}
	int   GetLastContactStatus()			{ return m_nLastContactStatus;}
	void  SetContactStatus( int nStatus )	{ m_nLastContactStatus = m_nContactStatus; m_nContactStatus = nStatus ;}
	void  SetTerrainNormal(const NxVec3 & kTerrainNormal) {m_bGotTerrainNormal = true; m_kTerrainNormal = kTerrainNormal;}

public:
	void	SetTurnAngle( NxReal fAngle)	{ m_fClockTurnAngle = fAngle;	}
	NxReal	GetTurnAngle()					{ return m_fClockTurnAngle;		}

	void    SetWheelVecAngle(NxReal fAngle )	{ m_fWheelAngle = fAngle ;}
	NxReal  GetWheelVecAngle()   { return m_fWheelAngle ;  }

	NxVec3  CaculateAirFriction(const NxVec3& v);

public:
	NxReal  m_fWheelAngle;

	// 切换场景物理要重新创建才可以
public:
	NxScene			* GetScene() { return m_pScene;}

public:
	void	SetKeyInfo( KEYINFO stKeyInfo ) { m_stKeyInfo = stKeyInfo;}
	KEYINFO GetKeyInfo()					{ return m_stKeyInfo	 ;}

public:
	void * GetTencentCarUserData()			{ return m_pCarUserData ;}

	std::list<PHYSINFO> & GetCarPhysParam()	{ return m_arrPhysInfo;}
	void SetCarPhysParam ( std::list<PHYSINFO> & listPhysInfo )
	{
		if ( listPhysInfo.empty())
			return;

		m_arrPhysInfo.clear();
		std::list<PHYSINFO>::iterator iter = listPhysInfo.begin();
		for ( ; iter != listPhysInfo.end(); iter++ ) {
			m_arrPhysInfo.push_back(*iter);
		}
		
	}



public:
	void		SetMatGround(NxMat33 matGround)			{ m_matGround = matGround; }
	NxMat33 &   GetMatGround()							{ return m_matGround;}
	void		SetMaxHeadAngle( NxReal fMaxHeadAngle ) { m_fMaxCarHeadAngle = fMaxHeadAngle;}
	NxReal		GetMaxHeadAngle()						{ return m_fMaxCarHeadAngle; }

public:
	//CCarAccelControl	* 	GetCarAccelControl() { return m_pAccelControl;}
	//CDriftControl		*	GetDriftingControl() { return m_pDriftControl;} 

	CDriftCenter		*   GetDriftCenter()     { return m_pDriftCenter; }
	
	CCarMotorControl    *   GetCarMotorControl() {return m_pCarMotorControl;}
	CCollisionCenter	*   GetCarCollisionCenter(){return m_pCarCollisionCenter;}
	NxController		*	getBoxController() { return m_pController  ;}
	bool					IsBoxController()  ;
	
	struct	ShooshData
	{
		bool	 bFull;
		NxReal	fValue;
	};

	NxVec3						SwooshRobot( NxVec3 fValue );
	std::queue<NxVec3>			m_arrShooshRobot;

private:
	std::vector<FORCE> m_arrForcePool;
	std::vector<TWIST> m_arrTwistPool;

private:
	std::list<PHYSINFO> m_arrPhysInfo;

private:
	NxVec3 m_vVecGlobal;
	NxVec3 m_vWecGlobal;

	// 上一桢的速度和角速度
	NxVec3 m_vLastVecGlobal;
	NxVec3 m_vLastWecGlobal;

//	NxVec3 m_vVecLocal;
//	NxVec3 m_vWecLocal;


private:
	NxVec3 m_vGravity;

private:
	NxReal m_fFricFactor;
	NxReal m_fSlipFrictionForce;
	NxReal m_fRollFrictionForce;
	NxReal m_fMass;
	NxReal m_fAirCoefC;			// 赛车阻力系数
	NxReal m_fForwardSquard;	// 赛车前／后向面积
	NxReal m_fSideSquare;		// 赛车侧向面积
	NxReal m_fUpSquare;			// 赛车上／下方向面积
	NxReal m_fAirDensity;		// 空气密度
	
private:
	DWORD  m_dwDeltaTime;


private:
	std::vector<CCarWheel*> m_arrCarWheelPool;

private:
	NxMat33 m_matGlobal;
	NxMat33 m_matGround;

// 车地表的　旋转矩阵　是临时记录的，它是最终形成m_matGlobal 的一部分

private:
	NxController * m_pController;
	//NxCapsuleController * m_pController;

public:
	bool		IsShiftKeyDown() { return m_stKeyInfo.bShift; }

public:
	float		m_fBoxWidth;
	float		m_fBoxHeight;
	float		m_fBoxLength;
	float		m_fAdjustOffset;
	float		m_fAdjustBevelOffset;
	float		m_fAdjustRemoteOffset;
	float		m_fAdjustPickGap;			// pick地面的差值

	float		m_fCollisionRadius;
	float		m_fCollisionLength;
	float		m_fCollisionWidth;
	float		m_fCollisionHeight;

private:
	float		m_fMaxTurnSpeed;
	float		m_fMinTurnSpeed;
	float		m_fBaseTurnRate;
	float		m_fVolatileTurnRate;

public:
	float GetCollisionRadius() { return m_fCollisionRadius; }
	float GetCollisionLenght() { return m_fCollisionLength; }
	float GetCollisonWidth() { return m_fCollisionWidth; }
	float GetCollisonHeight() { return m_fCollisionHeight; }

public: 
	Play * GetPlay() { return m_pPlay;}

private:
	NxScene			* m_pScene;
	NiAVObject		* m_pCarModel;
	Play			* m_pPlay;
	NxVec3			  m_vExtents;
private:
	CDriftCenter		 * m_pDriftCenter;

private:
	//	增加人:	wincoqin
	//  时间:	2007/01/29
	//  目的:	  处理加/减速和刹车等，以及碰撞	
	CCarMotorControl	* m_pCarMotorControl;			// 赛车引擎
	CCollisionCenter	* m_pCarCollisionCenter;		// 处理碰撞专用, 碰撞中转中心
	CCarFenceCollision	* m_pCarFenceCollisionProcessor;// 处理车-墙碰撞， 需要在m_pCollisionCenter之后初始化
	CCarCarCollision	* m_pCarCarCollisionProcessor;	// 处理车-车碰撞
	bool				 m_bGotTerrainNormal;
	NxVec3				 m_kTerrainNormal;

private:
	int				m_nContactStatus;
	int				m_nLastContactStatus;

	
private:
	NxReal	m_fClockTurnAngle;
	NxReal  m_fDeltaAlpha;
	NxReal  m_fBeltaAlpha;
	NxReal  m_fSteeringAccum;			// 转向积累
	NxReal  m_fSelfRotateAttenuation;	// 自旋转衰减系数
	NxReal	m_fOmiga;
	NxReal  m_fOmiga1;
	NxReal  m_fMaxCarHeadAngle;

private:
	std::vector<int> m_arrPrivotPoint;

private:
	NxVec3 m_vTotalForce;
	NxVec3 m_vTotalTwist;
	

private:
	KEYINFO m_stKeyInfo;

protected:
	void  * m_pCarUserData;

private:
	int     m_eLockStatus;

protected:
	void ReCaculatPrivot();

private:
	NxReal m_fAdjustXAngle;
	NxReal m_fAdjustYAngle;

protected:
	void GetAdjustAngle( NxReal & fXAngle, NxReal & fYAngle )
	{
		fXAngle = m_fAdjustXAngle;
		fYAngle = m_fAdjustYAngle;
	}
public:
	//车身运动相关

	myRaycastReport* m_pRayReport[4];

	float m_aPickResults[4];
	NxVec3 m_aPickNormalsPre[4];
	NxVec3 m_aPickNormalsCur[4];
	float m_aPickDistance[4];

	NxVec3 m_carPosition;
	ChassisSimulator* m_pSimulator;
	Suspension* m_pSuspension;

	float m_fHeadAngleAfterDrift;
	float m_fFallingDownAngularVel;
	float m_bIsDrifting;

	void GetWheelBumpInfo(int nIndex, BUMPINFO& bumpInfo);
	void UpdateCarPose();
	void SynchronizeSimulator(float dt);
	void SynchronizeHostPlayerModel(float dt);

	bool Pick(const NxVec3& rayPos, const NxVec3& rayDir, int nIndex);
	typedef struct tagVec3i{
		int x;
		int y;
		int z;
	}st_vec3i;
	NiMatrix3		m_mt3DModel;  //3D赛车模型的旋转矩阵
	bool			m_bIsBumping; //
	st_vec3i		m_bumpTime;   //震动的次数
	NiPoint3		m_vBumpAngle; //模型震动的目标角度
	NiPoint3		m_vBumpAngleCur;//模型震动的当前角度
	NiPoint3		m_vBumpAngularVel;//模型震动的角速度
	float			m_arrBumpOffs[4];//车轮颠簸的位移

	NxReal			m_fOffsetZ;

	void RotMeTender(NiMatrix3& mtSrc, const NiMatrix3& mtDest, float dt);
	void BumpMeTender(const NiMatrix3& matDest, float dt);
	void UpdateWheelsBump(const NiMatrix3& matDest, float dt);
	bool GenerateBump();
	bool GetBumpOffset(float* pfOffsets);
	void BumpInit();

	float	GetChassisOffsetZ();

	float   GetOffsetZ()  { return m_fOffsetZ;}
	void	SetOffsetZ( float fOffSetZ) { m_fOffsetZ = fOffSetZ;}

	bool GetContactedPoint(NxVec3& vPoint);
	void FullContactBump();
	int m_nFullContactBump;
	float m_nFullContactBumpTravel;
	float m_fFullContactBumpMax;
	//~
	NxVec3 CaculateTerrainNormal();
	NxVec3 m_vLastTerrainNormal;


public:
	// 小喷

public:
	void  CheckXiaoPen( float fTime );
	void  TriggerXiaoPen();

	void  SetXiaoPen	( bool bEffect ) 
	{ 
		if ( bEffect )
		{
			m_fXiaoPenStartTime		= 0.0f;
			m_fXiaoPenLastTime		= 0.25f; 
		}
		else
		{
		
			m_fXiaoPenStartTime		= 0.0f;
			m_fXiaoPenLastTime		= 0.0f;
		}
		m_bXiaoPen = bEffect ; 
		m_fKeyDownTime = 0.0f;
	} 


	bool  CanXiaoPen	()				 { return m_bXiaoPen;}


protected:
	float m_fXiaoPenStartTime;
	float m_fXiaoPenLastTime;
	bool  m_bXiaoPen;
	float  m_fXiaoPenEffectTime;
	float m_fKeyDownTime;

};



//-----------------------------------------------------------------------

struct st_vec3i{
	int x;
	int y;
	int z;
};



class CCarWheel
{
public:
	CCarWheel( CTencentCar * pCar);
	~CCarWheel();

	typedef struct tagWheel
	{
		NxReal fOffsetX;
		NxReal fOffsetY;
		NxReal fOffsetZ;
	}WHEEL, *PWHEEL;

	typedef enum enmStatus
	{
		ENS_NONE = 0,
		ENS_OTHER ,
		ENS_NEAR,
		ENS_FAR
	}ENMSTATUS;
public:
	bool Init();
	void RayPick(float fPickGap);
	void SynchronizeWheel();

protected:
	int   Pick (NxRay ray, NxRaycastHit &hit ,NxShape *&Shape);

public:
	NxShape  *	GetShootShape		()		{ return m_pShootShape;		}
	int			GetShootShapetype	()		{ return m_nShootShapetype;	}

	int			GetContactStatus	()		{ return m_nContactStaus;	}
	NxVec3		GetLocalPos			()		{ return m_vLocalPos;		}
	NxVec3      GetContactPos		()		{ return m_vContactPos;		}
	NxReal		GetContactVec		()		{ return m_fContactVec;		}
	
	NxVec3      GetContactNormal    ()      { return m_vCurContactN;	}


	void		SetContactStatus	(int nStatus) { m_nContactStaus = nStatus;}

	void		SetLocalPos	( NxVec3 vPos )		{ m_vLocalPos = vPos ; }

	void        SetWheelMode( NiAVObject * pMode) 
	{ 
		m_pWheelMode = pMode;
		m_vModePos   = pMode->GetTranslate();
	}

	NiAVObject * GetWheelModel()	{ return m_pWheelMode; } 

	void		SetModePos	( NiPoint3 vPoint)	{ m_vModePos = vPoint; }  // 相对于父亲节点的位置

	NiPoint3		GetModelPosOri(){return m_vModelPosOri;}
	void		SetModelPosOri(NiPoint3 vPoint) {m_vModelPosOri = vPoint;}
	NxVec3		GetGlobalPos ();
	void		SetTurnAngle(NxReal fTurnAngle)    { m_fTurnAngle = fTurnAngle;}
	NxReal		GetTurnAngle()					   { return m_fTurnAngle;}
	void		SetWheelRotateAngle(NxReal fAngle) { m_fRotateAngle = fAngle ;}
	NxReal      GetWheelRotateAngle()			   { return m_fRotateAngle   ;}
	NxMaterialIndex GetContactMaterialIndex()	   { return m_nContactMaterialIndex;}

public:
	NxReal  GetHitDistance()					   { return m_fHitDistance;	  }


protected:
	NxReal			m_fHitDistance;
	NxVec3			m_vLocalPos;
	NiPoint3		m_vModePos;
	CTencentCar *	m_pCar;
	NxShape		*	m_pShootShape;
	int				m_nShootShapetype;
	int				m_nContactStaus;
	NiAVObject  *   m_pWheelMode ; 
protected:
	NxVec3		m_vPreContactN;
	NxVec3      m_vCurContactN;
	NxVec3      m_vContactPos;
	NxReal      m_fContactVec;
	NxReal      m_fContactDistance;
	NxMaterialIndex m_nContactMaterialIndex;

private:
	NxReal     m_fRotateAngle;
	NxReal	   m_fTurnAngle;
	NiPoint3		m_vModelPosOri;//车轮的原始位置

public:
	void GetBumpInfo(BUMPINFO& bumpInfo);
};


class myRaycastReport : public NxUserRaycastReport
{
public:
	myRaycastReport(CTencentCar* pTC,int nIndex)
	{
		m_pTCar = pTC;
		m_nIndex = nIndex;	
	}
	virtual bool onHit(const NxRaycastHit& hit);

private:
	CTencentCar* m_pTCar;
	int m_nIndex;

};

#include "TencentCar.inl"









