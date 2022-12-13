
									/*-----------------------------------------------------------------------
											Create by HoverZhao 2007.4.25 for Monster Npc 
									-----------------------------------------------------------------------*/


#include "Top-KartPCH.h"
#include "NxPlayMgr.h"
#include "Play.h"
#include "Monster.h"
#include "stringex.h"
#include "variant.h"
#include "fileop.h"
#include "MovePkgMgr.h"




CMonsterMgr					* CMonsterMgr::ms_pkMonsterMgr = NULL;
CMonsterMgr::CMonsterNet	* CMonsterMgr::ms_pkMonsterNet = NULL;

bool	CMonsterMgr::ms_bIsArbi = false;

//-----------------------------------------------------------------------
bool CMonsterMgr::Create()
{

	if ( !FileExists("Monster"))
		return false;

	if ( FileExists("MonsterArbi"))
		ms_bIsArbi = true;
	else
		ms_bIsArbi = false;


	if ( ms_pkMonsterMgr )
		return true;

	ms_pkMonsterMgr = new CMonsterMgr;
	ms_pkMonsterNet = new CMonsterNet;

	if( !ms_pkMonsterMgr->InitMonsterMgr() ||
		!ms_pkMonsterNet->InitMonsterNet() ) 
	{
		delete ms_pkMonsterMgr ;
		ms_pkMonsterMgr = NULL ;
		delete ms_pkMonsterNet ;
		ms_pkMonsterNet = NULL ;
		return false;
	}


	return true;
}

CMonsterMgr::CMonsterNet::CMonsterNet():
m_iCookie(-1)
{
}

CMonsterMgr::CMonsterNet::~CMonsterNet()
{

}

//-----------------------------------------------------------------------
bool CMonsterMgr::CMonsterNet::InitMonsterNet()
{
	m_iCookie =  NxGamePkgMgr::RegistLisener(this);
	return true;
}

void CMonsterMgr::CMonsterNet::DestorMonsterNet()
{
	NxGamePkgMgr::UnRegistLisener(m_iCookie);
	return;
}

void CMonsterMgr::CMonsterNet::OnNetMessage( UINT MsgID, void * pBody )
{
	CMonsterMgr * pMonsterMgr = CMonsterMgr::Get();

	switch( MsgID )
	{
	case UDP_MSG_PLAYMOVE:
		break;
	case UDP_MSG_COLLSION:
		break;
	default:
		break;
	}
	
	return;
}

//-----------------------------------------------------------------------
// only process hostplay event 
void CMonsterMgr::CMonsterEvent::OnEvent( UINT EventID, void * pBody )
{
	return;
}

//-----------------------------------------------------------------------
void CMonsterMgr::Destory()
{
	if ( ms_pkMonsterMgr )
	{
		ms_pkMonsterMgr->DestoryMonsterMgr();
		delete ms_pkMonsterMgr;
		ms_pkMonsterMgr = NULL;
	}

	if ( ms_pkMonsterNet )
	{
		ms_pkMonsterNet->DestorMonsterNet();
		delete ms_pkMonsterNet;
		ms_pkMonsterNet = NULL;
	}
}


void CMonsterMgr::Update(float fTime)
{
	if ( ms_pkMonsterMgr )
		ms_pkMonsterMgr->UpdateMonsterMgr( fTime );
}


void CMonsterMgr::Draw( NiCamera * pkCamera )
{
	if ( ms_pkMonsterMgr )
		ms_pkMonsterMgr->DrawMonsterMgr( pkCamera );
}
//-----------------------------------------------------------------------
CMonsterMgr::CMonsterMgr()
{
	
}

//-----------------------------------------------------------------------
CMonsterMgr::~CMonsterMgr()
{

}

//-----------------------------------------------------------------------
void CMonsterMgr::UpdateMonsterMgr(float fTime)
{
	std::map<ULONG,CMonster*>::iterator iter = m_mapMonsters.begin();

	for ( ; iter != m_mapMonsters.end(); iter++ )
	{
		iter->second->Update( fTime );
	}
}

//-----------------------------------------------------------------------
void CMonsterMgr::DrawMonsterMgr( NiCamera * pkCamera )
{
	std::map<ULONG,CMonster*>::iterator iter = m_mapMonsters.begin();
	for ( ; iter != m_mapMonsters.end(); iter++ )
	{
		iter->second->Draw( pkCamera );
	}
}

//-----------------------------------------------------------------------
void CMonsterMgr::ActiveMonster( ULONG unID)
{
	std::map<ULONG ,CMonster*>::iterator iter = m_mapMonsters.find( unID );
	if ( iter != m_mapMonsters.end())
	{
		iter->second->Active();
	}
}

//-----------------------------------------------------------------------
CMonster * CMonsterMgr::GetMonster( ULONG unID )
{
	std::map<ULONG ,CMonster*>::iterator iter = m_mapMonsters.find( unID );
	if ( iter != m_mapMonsters.end())
	{
		return iter->second;
	}
	return NULL;
}
//-----------------------------------------------------------------------
void CMonsterMgr::DeactiveMonster( ULONG unID)
{
	std::map<ULONG, CMonster*>::iterator iter = m_mapMonsters.find( unID );
	if ( iter != m_mapMonsters.end())
	{
		iter->second->DeActive();
	}
}

//-----------------------------------------------------------------------
void CMonsterMgr::DestoryMonsterMgr()
{
	std::map<ULONG, CMonster*>::iterator iter = m_mapMonsters.begin();
	while( 0 != m_mapMonsters.size())
	{
		iter->second->UnInitMonster();
		NiDelete iter->second;
		m_mapMonsters.erase(iter);
	}
}

//-----------------------------------------------------------------------
bool CMonsterMgr::InitMonsterMgr()
{
	m_mapMonsters.clear();

	CStringEx		CarFilePath = "Monster/Monster.lua";

	CVarArray varrMonsterList("MonsterList");
	CVar::LoadVarsFromFile(&varrMonsterList, NiApplication::ConvertMediaFilename(CarFilePath.c_str()));
	UINT nNum = varrMonsterList.GetNum();

	for (unsigned int i = 0; i < nNum; i++ )
	{
		Monster MonsterInfo;
		MonsterInfo.unID		= varrMonsterList.Element(i)->Element("PID")->Integer();
		MonsterInfo.fStartPos	= varrMonsterList.Element(i)->Element("SPos")->Float();
		MonsterInfo.fEndPos		= varrMonsterList.Element(i)->Element("EPos")->Float();
	
		CMonster * pMonster  = NULL; 
		if ( NULL != ( pMonster = CreateMonster(&MonsterInfo)))  
		{
			m_mapMonsters[i] = pMonster;
			pMonster->SetID(i);
		}
	}


	return true;
}

//-----------------------------------------------------------------------
CMonster * CMonsterMgr::CreateMonster( Monster * pMonster)
{
	if ( !pMonster ) 
		return NULL;

	CMonster * pkMonster = NiNew CMonster();
	if ( !pkMonster->InitMonster(pMonster) ) 
		return NULL;

	return pkMonster;
	
}


//-----------------------------------------------------------------------
void CMonsterMgr::DispearMonster( ULONG unID )
{
	std::map<ULONG, CMonster*>::iterator iter = m_mapMonsters.find( unID );
	if ( iter != m_mapMonsters.end())
	{
		iter->second->UnInitMonster();
		NiDelete iter->second;
		m_mapMonsters.erase(iter);
	}
}


//-----------------------------------------------------------------------
CMonster::CMonster() : 
m_bIsBrain(false),
m_bIsActive(false),
m_spActorMgr(0),
m_spRootNode(0),
m_spLightNode(0),
m_unMonsterID(0),
m_kMonsterBrain(NULL)
{

}

//-----------------------------------------------------------------------
CMonster::~CMonster()
{

}

bool CMonster::InitLight(NiAVObject * pkNode )
{
	m_arrMonsterLights.RemoveAll();

	if (!pkNode)
		return false;

	NiNodePtr spLightNode = NiDynamicCast(NiNode, pkNode->GetObjectByName("Scene Root"));

	if (!spLightNode)
		return false;
	
	unsigned int unLightNum = spLightNode->GetChildCount();

	for (unsigned int i = 0; i < unLightNum; ++i)
	{
		if ( NiIsKindOf(NiLight,spLightNode->GetAt(i)))
		{
			NiLightPtr spLight = NiDynamicCast(NiLight, spLightNode->GetAt(i));
			m_arrMonsterLights.Add(spLight);  //会否泄露
			spLight->AttachAffectedNode(m_spRootNode);
			spLight->UpdateEffects();
		}
		m_spRootNode->UpdateEffects();
	}

	return true;
}
//-----------------------------------------------------------------------
bool CMonster::InitMonster( CMonsterMgr::Monster * pMonster)
{
	m_stMonsterInfo = *pMonster;

	const char * pResName = "Wolf";

	CStringEx ResPath ;

	m_spActorMgr	= NiActorManager::Create(	
		NiApplication::ConvertMediaFilename( ResPath.FormatPt("Monster/%s/%s.kfm",pResName,pResName)),
		true );

	if ( !m_spActorMgr )
		return false;

	m_spActorMgr->SetCallbackObject(this);
	
	for ( int i = 0 ; i < CMonsterMgr::MONSTER_ACTION_MAX; i++ )
	{
		m_spActorMgr->RegisterCallback( NiActorManager::ANIM_ACTIVATED ,i);
		m_spActorMgr->RegisterCallback( NiActorManager::END_OF_SEQUENCE,i);
	}

	m_spActorMgr->SetTargetAnimation( CMonsterMgr::MONSTER_ACTION_IDLE );

	m_spRootNode = NiDynamicCast(NiNode,m_spActorMgr->GetNIFRoot()); 

	InitLight(m_spRootNode);

	bool bSelectedUpdate ,bRigid;
	m_spRootNode->SetSelectiveUpdateFlags(bSelectedUpdate, true, bRigid);
	m_spRootNode->SetTranslate(NxPlayMgr::GetHostPlay()->GetPos().x + 20,
		NxPlayMgr::GetHostPlay()->GetPos().y + 20.0f,0.0f );
	m_spRootNode->SetScale( 0.005f );

	m_spRootNode->UpdateNodeBound();
	m_spRootNode->UpdateEffects();
	m_spRootNode->UpdateProperties();
	m_spRootNode->Update(0.0f);


	m_kMonsterBrain = new CMonsterBrain(this);

	InitPhyActor(m_spRootNode);

	return true;
}

//-----------------------------------------------------------------------
void CMonster::UnInitMonster()
{
	m_arrMonsterLights.RemoveAll();
	m_spRootNode = 0;
	m_spActorMgr = 0;
	if ( m_kMonsterBrain )
	{
		delete m_kMonsterBrain;
		m_kMonsterBrain = NULL;
	}
	return;
}


#define TestRegion(MMXX) \
	if ( m_stMonsterInfo.ActiveRegion[i].MMXX < fMinPoint.MMXX)	\
		fMinPoint.MMXX = m_stMonsterInfo.ActiveRegion[i].MMXX;	\
	if ( m_stMonsterInfo.ActiveRegion[i].MMXX > fMaxPoint.MMXX)	\
		fMaxPoint.MMXX = m_stMonsterInfo.ActiveRegion[i].MMXX;	


//-----------------------------------------------------------------------
bool CMonster::MonsterLook()
{

	NiPoint3 MonsterCenter = m_spRootNode->GetTranslate();
	float    fMonsterRadio = 3.0f;



	std::set<Play * >				PlayList	= NxPlayMgr::GetAllPlay();
	std::set< Play * >::iterator	it			= PlayList.begin();

	for ( ; it != PlayList.end(); it++ )
	{
		NiPoint3 kPos = (*it)->GetPos();
		float fDistance = NxMath::pow( 
			( MonsterCenter.x - kPos.x ) * ( MonsterCenter.x - kPos.x ) + 
			( MonsterCenter.y - kPos.y ) * ( MonsterCenter.y - kPos.y ) + 
			( MonsterCenter.z - kPos.z ) * ( MonsterCenter.z - kPos.z ) , 0.5f);
		if ( fDistance < 5.0f * fMonsterRadio )
		{
			Active();
			SetBrain();
			return true;
		}
	}

	DeActive();
	RemoveBrain();

	return false;

}

//-----------------------------------------------------------------------
void CMonster::Update( float fTime)
{

	if ( CMonsterMgr::IsArbi() )
	{
		MonsterLook();
		if ( IsBrain() )
			m_kMonsterBrain->Think(fTime);
	}

	

	NxPhyActor::Update(fTime);

	fTime  = GetMainApp()->GetAccumTime();
	m_spActorMgr->Update(fTime);
	m_spRootNode->Update(fTime);

	return;
}

void CMonster::CMonsterBrain::Think( float fTime )
{
	Look();
	GetFood();
	FindPath();
	ModifyMotion();
}


//-----------------------------------------------------------------------
void CMonster::CMonsterBrain::Look()
{

}

//-----------------------------------------------------------------------
Play * CMonster::CMonsterBrain::GetFood()
{
	m_kFood =  NxPlayMgr::GetHostPlay();
	return m_kFood;
}

//-----------------------------------------------------------------------
void CMonster::CMonsterBrain::FindPath()
{
	NiPoint3 PlayPos = m_kFood->GetPos();
	NiPoint3 MonsterPos =m_pMonster->m_spRootNode->GetTranslate();
	MonsterPos.z = PlayPos.z;

	NiPoint3 kDir = PlayPos - MonsterPos;
	NxVec3   kRealDir = NxVec3 ( kDir.x, kDir.y, kDir.z );
	kRealDir.normalize();
	kDir.x = kRealDir.x ; kDir.y = kRealDir.y; kDir.z = kRealDir.z;

	m_pMonster->AddForceGlobal( kRealDir);


	NiPoint3 vForward = m_pMonster->GetForwardGlobal();
	NiPoint3 vUp	  = m_pMonster->GetUpGlobal();
	
	float fAdjustAngle = NiACos( vForward * kDir );
	NiPoint3 vAxis;
	NiMatrix3 tempRot = NiMatrix3::IDENTITY;
	vAxis = kDir.UnitCross( vForward );
	vAxis = NiPoint3 ( 0.0f, 0.0f, NxMath::sign(vAxis.z));
	tempRot.MakeRotation(fAdjustAngle,vAxis);


	NxMat33	matGlobal   = m_pMonster->GetOriGlobal();

	NiMatrix3 matModelGlobal;
	NiPhysXTypes::NxMat33ToNiMatrix3( matGlobal,matModelGlobal );

	matModelGlobal = matModelGlobal * tempRot;

	NiPhysXTypes::NiMatrix3ToNxMat33( matModelGlobal ,matGlobal );


	m_pMonster->SetOriGlobal(matGlobal);

}

//-----------------------------------------------------------------------
void CMonster::CMonsterBrain::ModifyMotion()
{

}


//-----------------------------------------------------------------------
void CMonster::Draw(NiCamera* pkCamera)
{
	CCustomCullingProcess & kCuller = GetMainApp()->GetCuller();
	NiDrawScene(pkCamera, m_spRootNode, kCuller);
}
//-----------------------------------------------------------------------

BOOL CMonster::PlayAnimation(DWORD AnimID, BOOL bPlay)
{
	m_spActorMgr->SetTargetAnimation( CMonsterMgr::MONSTER_ACTION_HURT );
	return TRUE;
}

BOOL CMonster::StopAnimation(DWORD AnimID, BOOL bPlay)
{
	return TRUE;
}

void CMonster::Active()
{
	if ( m_bIsActive )
		return;

	//播放出生特效动画
	m_bIsActive = true;
	m_spActorMgr->SetTargetAnimation( CMonsterMgr::MONSTER_ACTION_FRUN );
}


void CMonster::DeActive()
{
	if ( !m_bIsActive )
		return;

	//播放结束动画，并归位
	m_bIsActive = false;
	m_spActorMgr->SetTargetAnimation( CMonsterMgr::MONSTER_ACTION_IDLE );
	SetVecGlobal(NxVec3 ( 0.0f, 0.0f, 0.0f ));
}


void CMonster::SetBrain()
{
	if ( CMonsterMgr::Get() && CMonsterMgr::IsArbi() )
		m_bIsBrain = true;
	else
		m_bIsBrain = false;
		
}

void CMonster::AnimActivated(NiActorManager* pkManager, NiActorManager::SequenceID eSequenceID, float fCurrentTime, float fEventTime)
{
	switch (eSequenceID )
	{
	case CMonsterMgr::MONSTER_ACTION_IDLE:
		
		break;
	case CMonsterMgr::MONSTER_ACTION_FRUN:
		
		break;
	}
	return;
}

void CMonster::AnimDeactivated(NiActorManager* pkManager, NiActorManager::SequenceID eSequenceID, float fCurrentTime, float fEventTime)
{
	switch (eSequenceID )
	{
	case CMonsterMgr::MONSTER_ACTION_IDLE:

		break;
	case CMonsterMgr::MONSTER_ACTION_FRUN:

		break;
	}
	return;
}

//-----------------------------------------------------------------------
void CMonster::EndOfSequence(	NiActorManager* pkManager, 
								NiActorManager::SequenceID 
								eSequenceID, 
								float fCurrentTime, 
								float fEventTime	)
{
	switch (eSequenceID )
	{
	case CMonsterMgr::MONSTER_ACTION_IDLE:
		m_spActorMgr->SetTargetAnimation( CMonsterMgr::MONSTER_ACTION_IDLE );
		break;
	case CMonsterMgr::MONSTER_ACTION_FRUN:
		m_spActorMgr->SetTargetAnimation( CMonsterMgr::MONSTER_ACTION_FRUN );
		break;
	case CMonsterMgr::MONSTER_ACTION_HURT:
		if (IsActive())
				m_spActorMgr->SetTargetAnimation( CMonsterMgr::MONSTER_ACTION_FRUN );
		else
				m_spActorMgr->SetTargetAnimation( CMonsterMgr::MONSTER_ACTION_IDLE );
		break;
	}
		return;
}







//-----------------------------------------------------------------------

 BOOL CMonster::Attach(NiAVObject * spObj)
{
	assert(spObj);
	NiNode * pkHint = m_spRootNode;

	pkHint->SetSelectiveUpdateTransforms(true);
	pkHint->SetSelectiveUpdatePropertyControllers(true);
	pkHint->SetSelectiveUpdateRigid(true);
	pkHint->AttachChild(spObj);
	pkHint->UpdateNodeBound();

	NiTimeController::StartAnimations(spObj, GetMainApp()->GetAccumTime());			

	pkHint->UpdateNodeBound();
	pkHint->UpdateProperties();
	pkHint->UpdateEffects();
	pkHint->Update(GetMainApp()->GetAccumTime());

	return TRUE;
}

 BOOL CMonster::Detach(NiAVObject * pObj)
{
	if (!pObj) 
	{
		return TRUE;
	}

	NiNode * pkHint =  m_spRootNode;

	pObj->Update(0.0f);
	NiTimeController::StopAnimations(pObj);
	pkHint->DetachChild(pObj);
	pkHint->UpdateNodeBound();
	pkHint->UpdateEffects();
	pkHint->UpdateProperties();
	pkHint->UpdateControllers(0.0f);
	pkHint->UpdateNodeBound();
	pkHint->UpdateEffects();
	pkHint->UpdateProperties();

	pkHint->Update(0.0f);

	return TRUE;

}

BOOL CMonster::ToggleSim(BOOL bToggle)
{
	if ( !bToggle )
	{
		m_spActorMgr->SetTargetAnimation( CMonsterMgr::MONSTER_ACTION_HURT );
		SetVecYLocal(0.0f);
		SetVecXLocal(0.0f);
	}
	else
	{
		m_spActorMgr->SetTargetAnimation( CMonsterMgr::MONSTER_ACTION_IDLE );
	}
	return TRUE;
}
