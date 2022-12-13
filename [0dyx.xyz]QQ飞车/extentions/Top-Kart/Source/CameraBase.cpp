#include "Top-KartPCH.h"

#include "CameraBase.h"
#include "NiPhysXTypes.h"
#include "NiMath.h"
#include "GameMain.h"
#include "gpdefine.h"

extern void FindAndAdd(NiObjectNET * pkRoot);

template<class T> CLimitContainer< T >::CLimitContainer(unsigned int unSize):
m_unSize(unSize),
m_unBackID(0),
m_unCurSize(0)
{
	m_pContainer = NiNew T[m_unSize];
}

template<class T> CLimitContainer< T >::~CLimitContainer()
{
	if(m_pContainer)
	{
		NiDelete []m_pContainer;
	}
}

template<class T> void CLimitContainer< T >::Push(T tData)
{
	if(m_unCurSize < m_unSize)
	{
		++m_unCurSize;
	}
	m_pContainer[m_unBackID] = tData;
	m_unBackID = ++m_unBackID%m_unSize;
}

template <class T> unsigned int CLimitContainer< T >::Count()
{
	return m_unCurSize;
}
//template<class T> void CLimitContainer< T >::ReSize(unsigned int unSize)
//{
//	
//}

template<class T> T CLimitContainer< T >::Sum()
{
	T tSum = 0;
	for(int i = 0; i < m_unCurSize; ++i)
	{
		tSum += m_pContainer[i]; 
	}
	return tSum;
}

template<class T> T CLimitContainer< T >::Average()
{
	T tSum = m_pContainer[0];
	for(int i = 1; i < m_unCurSize; ++i)
	{
		tSum = tSum + m_pContainer[i]; 
	}
	return tSum * (1.0/m_unCurSize);
}
CCameraBase::CCameraBase(ENMCAMTYPE eCamType):
m_spTarget(0),
m_fNearPlane(0),	
m_fFarPlane(0),	
m_fAspectRotio(0),	
m_fFov(0),
m_eCamType(eCamType),
m_bForceSpeedUp(false)
{
	m_vec3Up		= NiPoint3(0, 0, 0);
	m_vec3Right		= NiPoint3(0, 0, 0);
	m_vec3Dir		= NiPoint3(0, 0, 0);
	m_vec3Translate = NiPoint3(0, 0, 0);

	NiCameraPtr spCamera = NiNew NiCamera;
	SetCamera(spCamera);

}

CCameraBase::~CCameraBase()
{
	m_spCamera = 0;
	m_spCameraNode = 0;
	m_spTarget = 0;
}

void CCameraBase::SetTarget(Play* pTarget)
{
	m_spTarget = pTarget;
}

void CCameraBase::SetCamera(NiCameraPtr spCamera)
{
	if(!spCamera)
	{
		return;
	}

	m_spCameraNode = 0;
	m_spCameraNode = NiNew NiNode;

	m_spCamera = 0;
	m_spCamera = spCamera;
	m_spCameraNode->AttachChild(m_spCamera);
	m_spCameraNode->Update(0.0f);

}

void CCameraBase::SetCamera(NiNodePtr spCameraNode)
{
	if(!spCameraNode)
	{
		return;
	}

	m_spCameraNode = 0;
	m_spCameraNode = spCameraNode;

	m_spCamera = 0;
	m_spCamera = NiDynamicCast(NiCamera, spCameraNode->GetAt(0));

	m_spCameraNode->UpdateNodeBound();
	m_spCameraNode->UpdateProperties();
	m_spCameraNode->Update(0.0f);
}

NiCameraPtr CCameraBase::GetCamera()
{
	return m_spCamera;
}

unsigned int CCameraBase::GetCameraType()
{
	return m_eCamType;
}

void CCameraBase::PlayAnim()
{
	FindAndAdd(m_spCameraNode);
	if(NiGetController(NiTimeController,m_spCameraNode))
	{
		NiTimeController::StartAnimations(m_spCameraNode);
	}
}


void CCameraBase::StopAnim()
{
	if(m_spCameraNode)
	{
		if(NiGetController(NiTimeController,m_spCameraNode))
		{
			NiTimeController::StopAnimations(m_spCameraNode);
		}
	}
	
}

float CCameraBase::GetAnimTime()
{
	if(m_spCameraNode)
	{
		NiTimeControllerPtr nPtrl = NiGetController(NiTimeController,m_spCameraNode);
		if(nPtrl)
		{
			return nPtrl->GetEndKeyTime() - nPtrl->GetBeginKeyTime();
		}
	}
	return 0.0f;
}

void CCameraBase::AdjustFrustem()
{
	if(m_spCamera)
	{
		float hy = tanf( m_fFov/2.0f );
		float hx = hy * m_fAspectRotio;
		NiFrustum frustum = NiFrustum(-hx, hx, hy, -hy, m_fNearPlane, m_fFarPlane, false);
		m_spCamera->SetViewFrustum(frustum);
	}
}

void CCameraBase::SetTranslate(NiPoint3 vec3Tran)
{
	m_vec3Translate = vec3Tran;
}

NiPoint3 CCameraBase::GetTranslate()
{
	return m_vec3Translate;
}

void CCameraBase::SetRotate(NiMatrix3 matRot)
{
	matRot.GetCol(1, m_vec3Dir);
	matRot.GetCol(2, m_vec3Up);

	m_vec3Dir = -m_vec3Dir;
	m_vec3Dir.Unitize();
	m_vec3Up.Unitize();
	m_vec3Right = m_vec3Dir.UnitCross(m_vec3Up);
}

NiMatrix3 CCameraBase::GetRotate()
{
	return m_spCamera->GetRotate();
}

void CCameraBase::Update(float fTime)
{

}

void CCameraBase::LoadConfig(CScriptState *pstate)
{
	
}
//--------------------------------------------------------------------------------------------

CRaceCamera::CRaceCamera():CCameraBase(CCameraBase::ECT_RACE),
m_fCurDis(0),	
m_fNextDis(0),
m_fRotAngle(0),
m_fAngleScope(0),
m_fMinDis(0),
m_fMaxDis(0),
m_fHeight(0),
m_fSpeedCoef(0),	
m_fDistCoef(0),
m_fHeightCoef(0),
m_fRotCoef(0),
m_fForwardCoef1(0),
m_fForwardCoef2(0),
m_fForwardCoef3(0),
m_fUpCoef1(0),
m_fUpCoef2(0)
{
	m_vec3DemoPreDir = NiPoint3(0, 0, 0);
	m_arrPos = new CLimitContainer<NiPoint3>(5);
	m_arrRotMat = new CLimitContainer<NiMatrix3>(18);
}

CRaceCamera::~CRaceCamera()
{
	if(m_arrPos)
	{
		delete m_arrPos;
	}
	if(m_arrRotMat)
	{
		delete m_arrRotMat;
	}
}

bool CRaceCamera::IsSpeedUp()
{
	GP_DATA_LIST gp_List = m_spTarget->GetAffectGPs();
	GP_DATA_LIST_Iterator gp_ListIt = gp_List.begin();
	for(; gp_ListIt != gp_List.end(); ++gp_ListIt)
	{
		if((*gp_ListIt).dwID == 2001 || (*gp_ListIt).dwID == 1 || (*gp_ListIt).dwID == 2002)
		{
			return true;
		}
	}
	return false;
}


float CRaceCamera::GetAngleDelt(const NiPoint3 & curDir)
{
	float fAngleDelt = NiACos(m_vec3DemoPreDir * curDir );
	m_vec3DemoPreDir = curDir;
	if(fAngleDelt > 0.4)
	{
		fAngleDelt = 0.0;
	}
	return fAngleDelt;
}

void CRaceCamera::Update(float fTime)
{


	////----------------------------------------------------------------
	////测试
	//NiPoint3 veloDir = m_spTarget->GetKart()->GetVecGlobal();
	//veloDir.Unitize();
	////----------------------------------------------------------------

	m_spTarget->GetKartNIFRoot()->GetRotate().GetCol(1, m_vec3NextDir);
	m_spTarget->GetKartNIFRoot()->GetRotate().GetCol(2, m_vec3NextUp);

	m_vec3NextDir = -m_vec3NextDir;

	m_vec3NextDir.Unitize();
	m_vec3NextUp.Unitize();
	
//--------------------------------------------------------------------------------------------------------------
	NiPoint3 vVertical;
	NiMatrix3 tempRot = NiMatrix3::IDENTITY;
	float fRotAngle;

//---------------------------------------------------------------------------------------------------------------
	float fDifAngleDir = NiACos(m_vec3Dir * m_vec3NextDir);
	//if(fDifAngleDir > 0.01)		//若夹角小于0.033， 则不做处理，钝化镜头。否则要对角度进行插值
	{
		vVertical = m_vec3Dir.UnitCross(m_vec3NextDir);	//当前视点方向和下一视点方向的叉乘
	
		if(fDifAngleDir > m_fAngleScope)		//控制两方向的夹角在一定范围内
		{
			fRotAngle = NiPow(m_fForwardCoef1 + fDifAngleDir, 3) * m_fForwardCoef2 * NiAbs(m_fForwardCoef3 - GetAngleDelt(m_vec3NextDir)) * GetMainApp()->GetFrameTime();
			if(fRotAngle < (fDifAngleDir - m_fAngleScope))
			{
				fRotAngle = fDifAngleDir - m_fAngleScope;
			}
		}
		else
		{	
			//计算此次需要旋转的角度
			fRotAngle = NiPow(m_fForwardCoef1 + fDifAngleDir, 3) * m_fForwardCoef2 * NiAbs(m_fForwardCoef3 - GetAngleDelt(m_vec3NextDir)) * GetMainApp()->GetFrameTime();	
		}
		
		if(fRotAngle > fDifAngleDir)
		{
			fRotAngle = fDifAngleDir;
		}
		tempRot.MakeRotation(fRotAngle, vVertical);	//得到旋转矩阵

		m_vec3Dir = m_vec3Dir * tempRot;			//旋转当前的视点方向
	}
	//else if( fDifAngle > 0.005 )
	//{
	//	m_vec3Dir = m_vec3NextDir;		//当前视点方向直接等于下次的视点方向
	//}
	
//-------------------------------------------------------------------------------------------------------------
    
	float fDifAngleUp = NiACos(m_vec3Up * m_vec3NextUp);	//当前上方向和下一上方向的夹角
	if(fDifAngleUp > 0.01)	//若夹角小于0.04， 则直接等于模型的下一上方向。否则要对角度进行插值
	{
		vVertical = m_vec3Up.UnitCross(m_vec3NextUp);	//当前上方向和下一上方向的叉乘
		//计算此次需要旋转的角度
		fRotAngle = NiPow(m_fUpCoef1 + fDifAngleUp, 2) * m_fUpCoef2 * GetMainApp()->GetFrameTime();
		if(fRotAngle > fDifAngleUp)
		{
			fRotAngle = fDifAngleUp;
		}
		tempRot.MakeIdentity();
		tempRot.MakeRotation(fRotAngle, vVertical);	//得到旋转矩阵

		m_vec3Up = m_vec3Up * tempRot;				//旋转当前的上方向
	}
	else
	{
		m_vec3Up = m_vec3NextUp;				//当前上方向直接等于下次的上方向
	}
	
//-----------------------------------------------------------------------------------------------------------------

	m_vec3Right = m_vec3Dir.UnitCross(m_vec3Up);
	m_vec3Dir = m_vec3Up.UnitCross(m_vec3Right);
	
	if(IsForceSpeedUp() || IsSpeedUp())
	{
		m_fNextDis = m_fMaxDis;
	}
	else
	{
		float fSpeed = m_spTarget->GetKart()->GetVelocity();
		m_fNextDis = m_fMinDis + NiSqrt(fSpeed) * m_fSpeedCoef;
	}
	m_fCurDis += (m_fNextDis - m_fCurDis) * m_fDistCoef * GetMainApp()->GetFrameTime(); 

	if(m_fCurDis < m_fMinDis)
	{
		m_fCurDis = m_fMinDis;
	}

	if(m_fCurDis > m_fMaxDis)
	{
		m_fCurDis = m_fMaxDis;
	}

	if(m_spTarget->GetKart()->IsDrifting())
	{
		m_fCurDis -= m_fDriftCoef * fDifAngleDir;
	}

	fRotAngle = m_fRotAngle - m_fRotCoef * (m_fCurDis - m_fMinDis);
	tempRot.MakeIdentity();
	tempRot.MakeRotation(fRotAngle, m_vec3Up);
	NiMatrix3 matRot(m_vec3Dir, m_vec3Up, m_vec3Right);
	matRot = matRot * tempRot;
	m_vec3Translate = m_spTarget->GetKartNIFRoot()->GetWorldTransform().m_Translate;
//#if !defined(_PHYSX_2_7_0)
	//m_vec3Translate = m_spTarget->GetKartNIFRoot()->GetWorldTransform().m_Translate;
//#else
	//NiPoint3 vTarget(0,0,0),vTargetL(0.0f,0.0f,0.0f),vTargetR(0.0f,0.0f,0.0f) ;

	//vTarget += m_spTarget->GetKartNIFRoot()->GetObjectByName("RF_W")->GetWorldTranslate();
	//vTarget += m_spTarget->GetKartNIFRoot()->GetObjectByName("LF_W")->GetWorldTranslate();
	//vTargetL = m_spTarget->GetKartNIFRoot()->GetObjectByName("LB_W")->GetWorldBound().GetCenter();
	//vTargetR = m_spTarget->GetKartNIFRoot()->GetObjectByName("RB_W")->GetWorldBound().GetCenter();

	

	//NxReal fFactor = 0.5f;

	/*if ( m_spTarget->GetKart())
	{
		CTencentCar * pCar = m_spTarget->GetKart()->GetVehicle();
		NxReal fRealAngle = 0.0f;
		NxReal fAbsAngle  = 0.0f;
		if ( pCar )
		{
			fRealAngle = pCar->GetCarHeadAngle();
			fAbsAngle = NxMath::abs( fRealAngle );
			if ( fAbsAngle > 1.0e-06 )
			{
				fAbsAngle  = NxMath::abs(fAbsAngle);
				fFactor = 0.5 * ( NxPi/2.0f - fAbsAngle ) / (NxPi/2.0f) ;
				if ( fRealAngle < 0.0f )
					fFactor = 1.0f - fFactor;
			}

		}


	}*/
	//vTarget = vTargetL * fFactor + vTargetR * ( 1.0f - fFactor ) ;
//	vTarget = m_spTarget->GetKartNIFRoot()->GetObjectByName("dummy_back")->GetWorldTranslate();



	//m_vec3Translate = vTarget;
//#endif
	//-----------------------------------------------------------------
	//测试(高度插值)(效果好像不太好）
	NiPoint3 tempTran = m_spCamera->GetTranslate();
	NiPoint3 tempNextTran = (m_vec3Translate - (m_vec3Dir * m_fCurDis)) + (m_vec3Up * m_fHeight) ;
	float fDifHeight = tempNextTran.Dot(m_vec3Up)/m_vec3Up.Length() - tempTran.Dot(m_vec3Up)/m_vec3Up.Length();
	float fAdjustVecCof = __min ( NiAbs(fDifHeight)/(m_fHeight - 0.2),NxPi/2.0f);

	//float fAdjustVec =  3.0f * ( sin ( fAdjustVecCof - NxPi/2.0f) + 1.0f ) ;
	float fAdjustVec =  3.0f *  sin ( fAdjustVecCof ) ;

	//if(NiAbs(fDifHeight) > 0.01)
	{
		 float fCurHeight = m_fHeight - fDifHeight * m_fHeightCoef;//* GetMainApp()->GetFrameTime();

		 float fDelta = fCurHeight - m_fCurHeight;
		
			if ( NxMath::abs (fDelta) >  fAdjustVec * GetMainApp()->GetFrameTime() )
				fDelta = NxMath::sign(fDelta) * fAdjustVec * GetMainApp()->GetFrameTime();

		 m_fCurHeight += fDelta;
	}
	/*else
	{
		m_fCurHeight = m_fHeight;
	}*/
	//-----------------------------------------------------------------------------
	
	m_vec3Translate = (m_vec3Translate - (m_vec3Dir * m_fCurDis)) + (m_vec3Up * m_fCurHeight) ;


	m_spCamera->SetTranslate(m_vec3Translate);
	m_spCamera->SetRotate(matRot);

	m_spCamera->Update(0);
	m_spCameraNode->Update(0);

}
/*
void CRaceCamera::Update(float fTime)
{
	m_arrPos->Push( m_spTarget->GetKartNIFRoot()->GetWorldTransform().m_Translate);
	m_arrRotMat->Push(m_spTarget->GetKartNIFRoot()->GetRotate());

	NiMatrix3 matRot = m_arrRotMat->Average();
	NiPoint3 tran = m_arrPos->Average();

	matRot.GetCol(1, m_vec3Dir);
	matRot.GetCol(2, m_vec3Up);
	m_vec3Dir = -m_vec3Dir;
	m_vec3Dir.Unitize();
	m_vec3Up.Unitize();	
	
	m_spTarget->GetKartNIFRoot()->GetRotate().GetCol(1, m_vec3NextDir);
	m_vec3NextDir = - m_vec3NextDir;

	NiMatrix3 tempRot = NiMatrix3::IDENTITY;
	float fDifAngleDir = NiACos(m_vec3Dir * m_vec3NextDir);
	if(fDifAngleDir > m_fAngleScope)		//控制两方向的夹角在一定范围内
	{	
		tempRot.MakeRotation((fDifAngleDir - m_fAngleScope), m_vec3Dir.UnitCross(m_vec3NextDir));
		m_vec3Dir = m_vec3Dir * tempRot;
		m_vec3Dir.Unitize();
	}
	
	m_vec3Right = m_vec3Dir.UnitCross(m_vec3Up );
	m_vec3Up = m_vec3Right.UnitCross(m_vec3Dir);
	
	if(IsSpeedUp())
	{
		m_fNextDis = m_fMaxDis;
	}
	else
	{
		float fSpeed = m_spTarget->GetKart()->GetVelocity();
		m_fNextDis = m_fMinDis + NiSqrt(fSpeed) * m_fSpeedCoef;
	}
	m_fCurDis += (m_fNextDis - m_fCurDis) * m_fDistCoef * GetMainApp()->GetFrameTime(); 
	
	if(m_fCurDis < m_fMinDis)
	{
		m_fCurDis = m_fMinDis;
	}
	
	if(m_fCurDis > m_fMaxDis)
	{
		m_fCurDis = m_fMaxDis;
	}

	if(m_spTarget->GetKart()->IsDrifting())
	{
		m_fCurDis -= m_fDriftCoef * fDifAngleDir * m_fCurDis;
	}

	float fRotAngle = m_fRotAngle - m_fRotCoef * (m_fCurDis - m_fMinDis);
	tempRot.MakeIdentity();
	tempRot.MakeRotation(fRotAngle, m_vec3Up);
	matRot = NiMatrix3(m_vec3Dir, m_vec3Up, m_vec3Right);
	matRot = matRot * tempRot;
	
	m_vec3Translate = m_spTarget->GetKartNIFRoot()->GetWorldTransform().m_Translate;
	m_fCurHeight = m_fHeight - m_vec3Translate.Dot(m_vec3Up)/m_vec3Up.Length() + tran.Dot(m_vec3Up)/m_vec3Up.Length();
	m_vec3Translate = (m_vec3Translate - (m_vec3Dir * m_fCurDis)) + (m_vec3Up * m_fCurHeight) ;

	
	m_spCamera->SetTranslate(m_vec3Translate);
	m_spCamera->SetRotate(matRot);
	m_spCamera->Update(0);
	m_spCameraNode->Update(0);
}
*/


void CRaceCamera::LoadConfig(CScriptState *pstate)
{
	CVarTable vtRaceCam("RaceCam");
	vtRaceCam.GetFromScript(pstate);

	m_fNearPlane = vtRaceCam.Element("NearPlane")->Float();
	m_fFarPlane	= vtRaceCam.Element("FarPlane")->Float();	
	m_fAspectRotio = vtRaceCam.Element("AspectRatio")->Float();
	m_fFov = vtRaceCam.Element("Fov")->Float();

	m_fMinDis = vtRaceCam.Element("MinDis")->Float();
	m_fMaxDis = vtRaceCam.Element("MaxDis")->Float();
	m_fHeight = vtRaceCam.Element("Height")->Float();	
	m_fRotAngle = vtRaceCam.Element("RotAngle")->Float();	
	m_fSpeedCoef = vtRaceCam.Element("SpeedCoef")->Float();
	m_fDistCoef = vtRaceCam.Element("DistCoef")->Float();	
	m_fRotCoef = vtRaceCam.Element("RotCoef")->Float();	
	m_fHeightCoef = vtRaceCam.Element("HeightCoef")->Float();
	m_fDriftCoef = vtRaceCam.Element("DriftCoef")->Float();

	m_fAngleScope = vtRaceCam.Element("AngleScope")->Float();

	m_fForwardCoef1 = vtRaceCam.Element("ForwardCoef1")->Float();
	m_fForwardCoef2 = vtRaceCam.Element("ForwardCoef2")->Float();
	m_fForwardCoef3 = vtRaceCam.Element("ForwardCoef3")->Float();
	m_fUpCoef1 = vtRaceCam.Element("UpCoef1")->Float();
	m_fUpCoef2 = vtRaceCam.Element("UpCoef2")->Float();	

	m_fCurDis = m_fMinDis;
	m_fCurHeight = m_fHeight;


	CCameraBase::AdjustFrustem();
	
}
//------------------------------------------------------------------------



//------------------------------------------------------------------------
//	模型自身相机
CModelCamera::CModelCamera(): CCameraBase(CCameraBase::ECT_MODEL)
{

}

CModelCamera::~CModelCamera()
{

}

void CModelCamera::Update(float fTime)
{

}

void CModelCamera::LoadConfig(CScriptState *pstate)
{
	CVarTable vtModelCam("ModelCam");
	vtModelCam.GetFromScript(pstate);
	m_fNearPlane = vtModelCam.Element("NearPlane")->Float();
	m_fFarPlane	= vtModelCam.Element("FarPlane")->Float();	
	m_fAspectRotio = vtModelCam.Element("AspectRatio")->Float();
	m_fFov = vtModelCam.Element("Fov")->Float();

	AdjustFrustem();
}
//------------------------------------------------------------------------



//------------------------------------------------------------------------
//视点跟随相机，不改变相机位置，只改变角度
CRadarCamera::CRadarCamera() : CCameraBase(CCameraBase::ECT_RADAR)
{

}
CRadarCamera::~CRadarCamera()
{

}
void CRadarCamera::Update(float fTime)
{

}
void CRadarCamera::LoadConfig(CScriptState *pstate)
{

}
//--------------------------------------------------------------------------




//--------------------------------------------------------------------------
//鸟瞰相机
CAirScapeCamera::CAirScapeCamera() : CCameraBase(CCameraBase::ECT_AIRSCAPE),
m_fHeight(0)
{
	/*NiMatrix3 matRot(NiPoint3(0, 0, -1), NiPoint3(0, 1, 0), NiPoint3(1, 0, 0)); 
	m_spCamera->SetRotate(matRot);
	m_spCamera->Update(0);
	m_spCameraNode->Update(0);*/

}
CAirScapeCamera::~CAirScapeCamera()
{

}
void CAirScapeCamera::Update(float fTime)
{
	if(m_spTarget)
	{
		m_vec3Translate = m_spTarget->GetKartNIFRoot()->GetWorldTransform().m_Translate;
		m_vec3Translate = m_vec3Translate + NiPoint3(0, 0, 1) * m_fHeight;

		m_spTarget->GetKartNIFRoot()->GetRotate().GetCol(0, m_vec3Right);

		m_vec3Right = - m_vec3Right;

		m_vec3Up = m_vec3Right.UnitCross(NiPoint3(0, 0, -1));

		NiMatrix3 matRot(NiPoint3(0, 0, -1), m_vec3Up, m_vec3Right);


		m_spCamera->SetRotate(matRot);

		m_spCamera->SetTranslate(m_vec3Translate);
		m_spCamera->Update(0);
		m_spCameraNode->Update(0);
	}
}
void CAirScapeCamera::LoadConfig(CScriptState *pstate)
{
	CVarTable vtAirCam("AirScapeCam");
	vtAirCam.GetFromScript(pstate);
	m_fNearPlane = vtAirCam.Element("NearPlane")->Float();
	m_fFarPlane	= vtAirCam.Element("FarPlane")->Float();	
	m_fAspectRotio = vtAirCam.Element("AspectRatio")->Float();
	m_fFov = vtAirCam.Element("Fov")->Float();

	m_fHeight = vtAirCam.Element("Height")->Float();

	AdjustFrustem();

}
//--------------------------------------------------------------------------


//--------------------------------------------------------------------------
//小地图相机
CEagleMapCamera::CEagleMapCamera():CCameraBase(CCameraBase::ECT_EAGLEMAP),
m_fHeight(0)
{
	m_vec3Dir = NiPoint3(0, 0, -1);
	m_vec3Right = NiPoint3(0, 1, 0);
}

CEagleMapCamera::~CEagleMapCamera()
{

}

void CEagleMapCamera::Update(float fTime)
{
	if(m_spTarget)
	{
		m_vec3Translate = m_spTarget->GetKartNIFRoot()->GetWorldTransform().m_Translate;
		m_vec3Translate = m_vec3Translate + NiPoint3(0, 0, 1) * m_fHeight;

		m_spTarget->GetKartNIFRoot()->GetRotate().GetCol(0, m_vec3NextRight);

		m_vec3NextRight =  -m_vec3NextRight;

		m_vec3NextRight = m_vec3NextRight - m_vec3Dir.Dot(m_vec3NextRight)/m_vec3Dir.Length()*m_vec3Dir;
		m_vec3NextRight.Unitize();


		
		NiMatrix3 tempMat = NiMatrix3::IDENTITY;;
		float fDifAngleRight = NiACos(m_vec3Right * m_vec3NextRight);	//当前右方向和下一右方向的夹角
		if(fDifAngleRight > 0.01)
		{
			float fRotAngle = fDifAngleRight * 0.05;			
			NiPoint3 vVertical = m_vec3Right.UnitCross(m_vec3NextRight);
			tempMat.MakeRotation(fRotAngle, vVertical);
			m_vec3Right = m_vec3Right * tempMat;
//			m_vec3Right = m_vec3Right - m_vec3Dir.Dot(m_vec3Right)/m_vec3Dir.Length()*m_vec3Dir;
			m_vec3Right.Unitize();
		}
		

		m_vec3Up = m_vec3Right.UnitCross(m_vec3Dir);

		tempMat.MakeIdentity();
		tempMat.MakeRotation(0.6, m_vec3Dir);
		NiMatrix3 matRot(m_vec3Dir, m_vec3Up, m_vec3Right);
		matRot = matRot * tempMat;
		m_spCamera->SetRotate(matRot);

		m_spCamera->SetTranslate(m_vec3Translate);
		m_spCamera->Update(0);
		m_spCameraNode->Update(0);
	}
}

void CEagleMapCamera::LoadConfig(CScriptState *pstate)
{
	CVarTable vtEagleCam("EagleMapCam");
	vtEagleCam.GetFromScript(pstate);
	m_fNearPlane = vtEagleCam.Element("NearPlane")->Float();
	m_fFarPlane	= vtEagleCam.Element("FarPlane")->Float();	
	m_fAspectRotio = vtEagleCam.Element("AspectRatio")->Float();
	m_fFov = vtEagleCam.Element("Fov")->Float();

	m_fHeight = vtEagleCam.Element("Height")->Float();

	AdjustFrustem();

}