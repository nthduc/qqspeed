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
#include "levelmanager.h"
#include "PhysXManager.h"


class NxPhyActor
{

public:
	NxPhyActor(){
		m_fMass = 1.0f;
	}
	virtual ~NxPhyActor(){
		//PhysXManager::GetControllerMgr().releaseController(*m_pController);
		//m_pController = NULL;
	}

	virtual bool InitPhyActor(NiAVObject* pModel)
	{
		m_pModel = pModel;
		NiMatrix3 mat =  m_pModel->GetRotate();
		NxMat33  matPhy;
		matPhy.id();
		NiPhysXTypes::NiMatrix3ToNxMat33(mat,matPhy );
		SetOriGlobal(matPhy);

		m_vVecGlobal  = NxVec3( 0.0f, 0.0f,0.0f);
		m_vWecGlobal  = NxVec3( 0.0f, 0.0f,0.0f);
		m_vTotalForce = NxVec3 (0.0f, 0.0f, 0.0f );
		m_vTotalTwist = NxVec3 (0.0f, 0.0f, 0.0f );

		NiPoint3 vPos = pModel->GetTranslate();
		NxCapsuleControllerDesc kDesc;
		kDesc.position.x		= vPos.x;
		kDesc.position.y		= vPos.y;
		kDesc.position.z		= vPos.z;
		kDesc.radius			= 0.15f;
		kDesc.height			= 0.15f;
		kDesc.upDirection		= NX_Z;
		kDesc.slopeLimit		= cosf(NxMath::degToRad(45.0f));
		kDesc.skinWidth			= 0.05f;
		kDesc.stepOffset		= 0.2f;			
		NxScene * pPhyScene = CLevelManager::Get()->GetPhysXManager()->GetPhyScene();
		kDesc.callback = 0;
		ControllerManager& gCM = PhysXManager::GetControllerMgr();
		m_pController = (NxCapsuleController*)gCM.createController(pPhyScene,kDesc);

		return true;
	}

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
	NiPoint3	GetPosGlobal();
	void		SetPosGlobal( NiPoint3 vPos );

	// 方向函数
	NxMat33 GetOriGlobal();
	void	SetOriGlobal( NxMat33 matOri);

	NiPoint3  GetForwardGlobal();
	NiPoint3  GetUpGlobal();
	NiPoint3  GetLeftGlobal();

protected:
	void    SetUpGlobal			( NiPoint3 vUp );
	void	SetLeftGlobal		( NiPoint3 vLeft);
	void    SetForwardGlobal	( NiPoint3 vForward);

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
	NxReal  GetMass ()						{ return m_fMass;  }
	void    SetMass ( NxReal fMass )		{ m_fMass = fMass; }

public:
	NxVec3  GetGravity()					{ return m_vGravity; } 
	void    SetGravity(NxVec3 vGravity)		{ m_vGravity = vGravity; } 

public:
	void ClearForcePool() ;
	void ClearTwistPool() ;

	virtual void TriggerForceTwistPool();
	virtual void Update( float fTime );
	virtual void UpdateWec( float fTime);
	virtual void UpdateVec( float fTime);
	virtual void UpdateLineMovement( float fTime);
	virtual void UpdateTurnMovement( float fTime);

	NxVec3 GetTotalForce()		{ return m_vTotalForce; }
	NxVec3 GetTotalTwist()		{ return m_vTotalTwist; }	


	void   SetTotalForce(NxVec3 vTForce);
	void   SetTotalTwist(NxVec3 vTTwist);


	NxReal  GetPlaneVecSize();
	NxReal  GetHeadAngle( bool bAbs = false ,bool bRui = false);


protected:

	DWORD GetDeltaTime ()						{ return m_dwDeltaTime;}
	void  SetDeltaTime ( DWORD dwDelatTime )	{ m_dwDeltaTime = dwDelatTime; } 
	void  RotateMatWorldZAxis	( NiMatrix3 & mat , NxReal fAngle );
	void  RotateMat33XAxis		( NxMat33 & mat, NxReal fAngle	);
	void  RotateMat33ZAxis		( NxMat33 & mat, NxReal fAngle	);
	void  RotateMat33YAxis		( NxMat33 & mat, NxReal fAngle	);
private:
	std::vector<FORCE> m_arrForcePool;
	std::vector<TWIST> m_arrTwistPool;

private:
	NxVec3 m_vVecGlobal;
	NxVec3 m_vWecGlobal;
	NxMat33 m_matGlobal;


private:
	NxVec3 m_vGravity;
	NxReal m_fMass;

	DWORD m_dwDeltaTime;


private:
	NxScene					* m_pScene;
	NiAVObject				* m_pModel;
	NxCapsuleController		* m_pController;

private:
	NxVec3 m_vTotalForce;
	NxVec3 m_vTotalTwist;

};

#include "NxPhyActor.inl"









