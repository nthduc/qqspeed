

#include "Top-KartPCH.h"

#include "Play.h"
#include "MommaTasks.h"
#include "Scheduler.h"
#include "RMOperator.h"
#include "LM_Anim.h"
#include "left_Anim.h"
#include "one_Anim.h"
#include "EventContainer.h"
#include "RMOperator.h"
#include <NiApplication.h>
#include "Footprints.h"
#include "SoundManager.h"
#include "PhysXUserData.h"
#include <NiPhysX.h>
#include "Track/CheckEditor.h"
#include "Track/PlayerPosUpdateTask.h"
#include "PhysKart/AccelTask.h"
#include "Scheduler.h"
#include "NiBillboardNode.h"

#include "NxSceneMgr.h"
#include "SceneLevel.h"
#include "PowerController.h"
#include "variant.h"
#include "stringex.h"
#include "IUIRenderer.h"
#include "./Avatar/ItemDefine.h"
#include "./Avatar/AvatarMgr.h"
#include "./Avatar/ItemDataMgr.h"
#include "./Platform/GlobalInterface.h"
#include "./Platform/MsgDataCache.h"
#include "network/network_i.h"
#include "UIFace.h"
#include "UIFaceElement.h"
#include "swoosh.h"
#include "gprender.h"
#include "changecolor.h"

#include "Utility.h"
#include "GameMain.h"
#include "framework/GameApp_i.h"

void PrepackNode(NiObjectNET* pkRoot)
{
	if(NiIsKindOf(NiAVObject, pkRoot))
	{
		// NiProperties can have time controllers, so search them too
		NiAVObject* pkObj = (NiAVObject*) pkRoot;
		//pkObj->SetSelectiveUpdate(true);
		pkObj->SetSelectiveUpdateTransforms(true);
		pkObj->SetSelectiveUpdatePropertyControllers(true);
		pkObj->SetSelectiveUpdateRigid(true);
	}
	if(NiIsKindOf(NiNode, pkRoot))
	{
		NiNode* pkNode = (NiNode*) pkRoot;
		// Search all of the children
		for(unsigned int ui = 0; ui < pkNode->GetArrayCount(); ui++)
		{
			NiAVObject* pkObj = pkNode->GetAt(ui);
			PrepackNode(pkObj);
		}
	}
}

//---------------------------------------------------------------------------
Play::Play() :
    m_fLastInputTime(0.0f), m_fAutoModeTime(15.0f), 
    m_kAutoGoal(NiPoint2::ZERO),
    m_bRumble(true), m_fPausedTime(0.0f),
    m_fPauseTime(15.0f), m_kPos(NiPoint3::ZERO), m_bAutoWalk(false),
    m_fMaxAutoGoalSeekTime(60.0f),
    m_bExcutingAction(false),m_bIsSpeedUp(false),m_unMovePackageIndex(0),
	m_bIsFinish(false),
	m_bIsWin(false),
	m_bIsStopPhy(false),
	m_fSpeedDescentRatio(0.9),
	m_fRestMileage(99999999.9),
	m_bIsTurnBack(false),
	m_pkSwooshL(NULL),
	m_pkSwooshR(NULL)

{
	m_cQQName[0] = '\0';
	m_pKart = NULL;
	m_pAnimEventMgr = 0;

}
//---------------------------------------------------------------------------
Play::~Play()
{
	Release();
	//清空Avatar数据
	CAvatarMgr::Get()->GetItemDataMgr()->CleanMainAvatarData(GetPlayUin());
}

void Play::Release()
{
	SAFE_NIDELETE(m_pkSwooshL);
	SAFE_NIDELETE(m_pkSwooshR);
	SAFE_DELETE(m_pKart);
	SAFE_NIDELETE(m_pAnimEventMgr);
}

void Play::InitPlayCamera()
{
    NiNodePtr tpNode=(NiNode*) GetKartNIFRoot()->GetObjectByName("camera");
	NiAVObject * GameStartCamera = RMOperator::GetObjectByType(tpNode->GetObjectByName("game start camera"),&NiCamera::ms_RTTI);
	NiAVObject * GameOverCamera = RMOperator::GetObjectByType(tpNode->GetObjectByName("game over camera"),&NiCamera::ms_RTTI);
	NiIsKindOf(NiCamera,GameStartCamera);
	NiIsKindOf(NiCamera,GameOverCamera);
	
}
#include "JointCloth.h"
CJointCloth * g_pBaloon = NULL;

void Play::EnterPhyScene(const NiTransform& kTransform)
{
	if( CLevelManager::Get())
	{
		//g_pBaloon = CJointCloth::CreateJointCloth();

		//AttachDummy("dummy_back", (NiNode*)g_pBaloon->GetAVObject());	
		NxScene * pPhyScene = CLevelManager::Get()->GetPhysXManager()->GetPhyScene();
		if ( !pPhyScene ) return;
		NxVec3 pos(kTransform.m_Translate.x, kTransform.m_Translate.y, kTransform.m_Translate.z);
		m_pKart->createCarWithDesc2(GetKartNIFRoot(),pos, true, true, false, false, false, pPhyScene, this);

		m_pKart->SetMovable(false);		

		CResponseLoginPara * pPara = GlobalInterface::m_pNetwork->GetGameInfo();	
		char varCmd[256];
		if (IsHost())
		{
			if (GlobalInterface::m_pScriptState && pPara)
			{			
				// 喷射
				_snprintf(varCmd, 255, "CaculateAndSetKartAttrib(%d, %d)", 1, pPara->m_stPlayerDBAttributeInfo.m_nSpeed);
				varCmd[255] = 0;
				GlobalInterface::m_pScriptState->DoString(varCmd);

				// 爆发
				_snprintf(varCmd, 255, "CaculateAndSetKartAttrib(%d, %d)", 2, pPara->m_stPlayerDBAttributeInfo.m_nExplosion);
				varCmd[255] = 0;
				GlobalInterface::m_pScriptState->DoString(varCmd);

				// 积气
				_snprintf(varCmd, 255, "CaculateAndSetKartAttrib(%d, %d)", 3, pPara->m_stPlayerDBAttributeInfo.m_nGathering);
				varCmd[255] = 0;
				GlobalInterface::m_pScriptState->DoString(varCmd);

				// 喷射
				_snprintf(varCmd, 255, "CaculateAndSetKartAttrib(%d, %d)", 4, pPara->m_stPlayerDBAttributeInfo.m_nSpout);
				varCmd[255] = 0;
				GlobalInterface::m_pScriptState->DoString(varCmd);

				// 小喷
				_snprintf(varCmd, 255, "CaculateAndSetKartAttrib(%d, %d)", 5, pPara->m_stPlayerDBAttributeInfo.m_nMiniSpout);
				varCmd[255] = 0;
				GlobalInterface::m_pScriptState->DoString(varCmd);
			}
		}
		else
		{
			_snprintf(varCmd, 255, "UnPlaceRemotePlayerInRoom(%d)", GetPlayUin());
			varCmd[255] = 0;
			GlobalInterface::m_pScriptState->DoString(varCmd);
			ShowName(true);
		}
		//隐藏玩家脚部
		ItemUnit*	pItem=CAvatarMgr::Get()->GetItemInfoById(GetKartID());
		if (pItem && pItem->nHideRoleFoot)
		{
			CAvatarMgr::Get()->GetItemDataMgr()->ShowRolePart(this,false);
		}

		SitPos(kTransform);
	}
}

void Play::StopPhy()
{
	m_bIsStopPhy = true;
}
void Play::StartPhy()
{
	m_bIsStopPhy = false;
}

void Play::UpdatePhy(float fTime)
{
	if (g_pBaloon)
		g_pBaloon->Update();
	if ( !m_bIsStopPhy)
	{	
		GetKart()->Control(fTime);		
			
		//GetKart()->SynchronizeModel(NULL);
	}
}

void Play::ChangeTeam(DWORD dwTeamID)
{
	if ( GlobalInterface::m_pDataCache )
	{
		SColorAttr* pkColor = NULL;
		DWORD dwTeamIDPara = dwTeamID;
		if ( dwTeamIDPara == (DWORD)-1 )
		{
			dwTeamIDPara = m_stPlayInfo.m_nTeamID;
		}
		else
		{
			m_stPlayInfo.m_nTeamID = dwTeamID;
		}

		RoomDesc& rd = GlobalInterface::m_pDataCache->m_RoomDesc;
		if ( rd.GameModeSub == EGSI_SUBMODE_TEAM )
		{
		}
		else
		{
			pkColor = FindPaintColor();
		}
		{
			CItemDataMgr* pItemDataMgr = CAvatarMgr::Get()->GetItemDataMgr();
			ItemUnit* pItemUnit = pItemDataMgr->GetItemById(m_stPlayInfo.m_nKartID);
			if ( pItemUnit )
			{
				UINT nCnt=0;
				for (nCnt=0;nCnt<pItemUnit->deChgColorName.size();++nCnt)
				{
					NiGeometry* pColoredGeometry = NiDynamicCast(NiGeometry, m_spKartAM->GetNIFRoot()->GetObjectByName(pItemUnit->deChgColorName[nCnt].c_str()));
					if (!pColoredGeometry)
					{
						XTrace("找不到换色模型");
						continue;
					}
					changeColor(pColoredGeometry,0,dwTeamIDPara, pkColor);
				}
			}
			/*人物不换色了，只换车的
			NiGeometry* pColoredGeometry = NULL;
			if ( m_spHairAM )
			{
				pColoredGeometry = NiDynamicCast(NiGeometry, m_spHairAM->GetNIFRoot()->GetObjectByName("hair"));
				changeColor(pColoredGeometry, 0, dwTeamIDPara, pkColor);
			}
			
			pColoredGeometry = NiDynamicCast(NiGeometry, m_spCharacterAM->GetNIFRoot()->GetObjectByName("body"));
			changeColor(pColoredGeometry, 0, dwTeamIDPara, pkColor);
			pColoredGeometry = NiDynamicCast(NiGeometry, m_spCharacterAM->GetNIFRoot()->GetObjectByName("leg"));
			changeColor(pColoredGeometry, 0, dwTeamIDPara, pkColor);
			*/
		}
	}
}

SColorAttr* Play::FindPaintColor()
{
	CItemDataMgr* pItemDataMgr = CAvatarMgr::Get()->GetItemDataMgr();
	ItemStore* pItemStore = pItemDataMgr->GetCurrEquipStore(m_stPlayInfo.m_unUin, EAIT_CR_PAINT);
	if ( pItemStore )
	{
		ItemUnit* pItemUnit = pItemDataMgr->GetItemById(pItemStore->insInfo.m_iItemID);
		return &pItemUnit->insColor;
	}
	return NULL;
}

void Play::LeavePhyScene()
{
	if (g_pBaloon)
		DetachDummy("dummy_back", (NiNode*)g_pBaloon->GetAVObject());
	SAFE_DELETE(g_pBaloon);
	m_pKart->UnCreateCarWithDesc2();
	//显示玩家脚部
	ItemUnit*	pItem=CAvatarMgr::Get()->GetItemInfoById(GetKartID());
	if (pItem && pItem->nHideRoleFoot)
	{
		CAvatarMgr::Get()->GetItemDataMgr()->ShowRolePart(this,true);
	}
}


//---------------------------------------------------------------------------
bool Play::Initialize(KPlayInfo & Info , unsigned int uiMommaID,
    unsigned int uiPadID)
{   
	m_stPlayInfo = Info;
	m_kPos = Info.m_SpawnPoint;
	/*
	unsigned int unActiveChacaterID = Info.m_nRoleID;
	unsigned int unActiveKartID = Info.m_nKartID;

	switch(m_stPlayInfo.m_nKartID)
	{
	case 1:
	case 2:
		unActiveKartID = 10003;
		break;
	case 3:
		unActiveKartID = 10013;
		break;
	case 4:
		unActiveKartID = 10017;
		break;
	case 5:
		unActiveKartID = 10019;
		break;
	case 6:
		unActiveKartID = 10020;
		break;
	default:
		unActiveKartID = 10003;
	}

	m_stPlayInfo.m_nKartID=unActiveKartID;//??

	switch(m_stPlayInfo.m_nRoleID)
	{
	case 1:
		unActiveChacaterID = 10000;
		break;
	case 2:
		unActiveChacaterID = 10001;
		break;
	case 3:
		unActiveChacaterID = 10012;
		break;
	default:
		assert(0);
		break;
	}

	m_stPlayInfo.m_nRoleID=unActiveChacaterID;//??
*/
	//// 重10018以上都是新人新车 test 
	//if ( unActiveChacaterID <  10018 )
	//{
	//	m_stPlayInfo.m_nKartID = 10003;
	//	unActiveKartID = 10003;
	//}

	SetActiveCharacter(m_stPlayInfo.m_nRoleID);
	SetActiveKart(m_stPlayInfo.m_nKartID);
	//m_spCharacterAM->SetTargetAnimation(m_stCurPlayerDesc.mapCharacterAnim["Idle"]);
	m_spCharacterAM->Update(0.0f);
	m_spKartAM->GetNIFRoot()->SetTranslate(m_kPos);



	strncpy( m_cQQName, Info.m_szQQNickname,MAXNICKNAMELEN);

	Actor::Initialize(9, m_cQQName);


#ifndef _XCART_
	m_pKart = new CKart();
#else
	m_pKart = new XCart();
#endif

	SetPos(m_kPos);
	SetRotate(NiPoint3(0.0f,0.0f,1.0f),90.0f);
	GetKartNIFRoot()->Update(0.0f);

	// 本地玩家不显示自己的名字
	if (IsHost()) 
	{
		ShowName(false);
	}
	ChangeTeam(Info.m_nTeamID);
	m_pAnimEventMgr = NiNew CAnimEventMgr(this);



	//残影特效
	NiAVObject * pObject1 = GetKartNIFRoot()->GetObjectByName("LB_W");
	NiAVObject * pObject2 = GetKartNIFRoot()->GetObjectByName("RB_W");

	CComPtr<IGameSettings> spSettings;
	GlobalInterface::m_pApp->QueryInterface(IID_IGameSettings, (void**)&spSettings);
	BOOL bCarSwoosh=FALSE;
	if ( spSettings )
		spSettings->GetIntValue("CarSwoosh", &bCarSwoosh);
	if (bCarSwoosh)
	{
		m_pkSwooshL = NiNew Swoosh(pObject1,GetMainApp()->GetCurrentTime());
		m_pkSwooshL->SetActivity(true);
		m_pkSwooshR = NiNew Swoosh(pObject2,GetMainApp()->GetCurrentTime());
		m_pkSwooshR->SetActivity(true);
	}

	//if ( IsNewCar() )
	//	NxSceneMgr::Get()->GetCameraController()->LoadConfig("Config/CameraCfgNewCar.lua");
	//else
		NxSceneMgr::Get()->GetCameraController()->LoadConfig("Config/CameraCfg.lua");
	
	InitWheelPos();

	return true;
}

void Play::LoadCharacterDesc(unsigned int unID)
{
	CStringEx strTmp;
	ItemUnit* pItem=CAvatarMgr::Get()->GetItemInfoById(unID);

	if (!pItem)
		return;
/*
	CVarTable* vtCurCharacterDesc;
	CVarTable vtPlayerDesc("player_path");
	CVar::LoadVarsFromFile(&vtPlayerDesc, "Config/player_desc.lua");
	
	CVarArray * varrCharacterDesc = (CVarArray*) vtPlayerDesc.Element("character_path");
	for(int i = 0; i < varrCharacterDesc->GetNum(); ++i)
	{
		if(varrCharacterDesc->Element(i)->Element("id")->Integer() == unID)
		{
			vtCurCharacterDesc = (CVarTable*) varrCharacterDesc->Element(i);
			assert(vtCurCharacterDesc->GetType() == VAR_TABLE);
			break;
		}
	}

	//设置基本信息
	m_stCurPlayerDesc.unCharacterID		= unID; 
	m_stCurPlayerDesc.strCharacterName	= vtCurCharacterDesc->Element("name")->String();
	m_stCurPlayerDesc.strChacaterPath	= vtCurCharacterDesc->Element("path")->String();
*/
	//设置基本信息
	m_stCurPlayerDesc.unCharacterID		= pItem->nId; 
	m_stCurPlayerDesc.strCharacterName	= pItem->strName.c_str();
	m_stCurPlayerDesc.strChacaterPath	= strTmp.FormatPt("Avatar/%s/%u/",
		CAvatarMgr::Get()->GetItemDataMgr()->GetTypeName(pItem->nType),
		pItem->nId);

	//载入动作
	CVarArray varrCharacterAnim("character_anim");
	CVar::LoadVarsFromFile(&varrCharacterAnim, NiApplication::ConvertMediaFilename(
		strTmp.FormatPt("%s/character_anim.lua", m_stCurPlayerDesc.strChacaterPath)));
	m_stCurPlayerDesc.mapCharacterAnim.clear();
	int nNumOfAnim = varrCharacterAnim.GetNum();
	for(int i = 0; i < nNumOfAnim; i++)
	{
		m_stCurPlayerDesc.mapCharacterAnim.insert( make_pair(varrCharacterAnim.Element(i)->Element("anim_name")->String(),
			varrCharacterAnim.Element(i)->Element("anim_id")->Integer()) );
	}

	//载入表情贴图
	CVarArray varrCharacterImage("character_image");
	CVar::LoadVarsFromFile(&varrCharacterImage, NiApplication::ConvertMediaFilename(
		strTmp.FormatPt("%s/character_image.lua", m_stCurPlayerDesc.strChacaterPath)));
	m_stCurPlayerDesc.mapCharacterImage.clear();
	NiSourceTexturePtr spTex;
	int nNumOfImage = varrCharacterImage.GetNum();
	for(int i = 0; i < nNumOfImage; i++)
	{
		spTex = NiSourceTexture::Create(NiApplication::ConvertMediaFilename(
			strTmp.FormatPt("%s/Image/FaceImage/%s", m_stCurPlayerDesc.strChacaterPath,	
			varrCharacterImage.Element(i)->Element("image_name")->String())) );

		m_stCurPlayerDesc.mapCharacterImage.insert( make_pair(varrCharacterImage.Element(i)->Element("anim_id")->Integer(),
			spTex) );
	}
}


void Play::LoadKartDesc(unsigned int unID)
{
	CStringEx strTmp;
	ItemUnit* pItem=CAvatarMgr::Get()->GetItemInfoById(unID);

/*
	CVarTable* vtCurKartDesc;
	CVarTable vtPlayerDesc("player_path");
	CVar::LoadVarsFromFile(&vtPlayerDesc, "Config/player_desc.lua");

	CVarArray * varrKartDesc = (CVarArray*) vtPlayerDesc.Element("kart_path");
	for(int i = 0; i < varrKartDesc->GetNum(); ++i)
	{
		if(varrKartDesc->Element(i)->Element("id")->Integer() == unID)
		{
			vtCurKartDesc = (CVarTable*) varrKartDesc->Element(i);
			assert(vtCurKartDesc->GetType() == VAR_TABLE);
			break;
		}
	}
	
	m_stCurPlayerDesc.unKartID		= unID;
	m_stCurPlayerDesc.strKartName	= vtCurKartDesc->Element("name")->String();
	m_stCurPlayerDesc.strKartPath	= vtCurKartDesc->Element("path")->String();
*/
	m_stCurPlayerDesc.unKartID		= pItem->nId; 
	m_stCurPlayerDesc.strKartName	= pItem->strName.c_str();
	m_stCurPlayerDesc.strKartPath	= strTmp.FormatPt("Avatar/%s/%u/",
		CAvatarMgr::Get()->GetItemDataMgr()->GetTypeName(pItem->nType),
		pItem->nId);

	//加载动作列表
	CVarArray varrKartAnim("kart_anim");
	CVar::LoadVarsFromFile(&varrKartAnim, NiApplication::ConvertMediaFilename(
		strTmp.FormatPt("%s/kart_anim.lua", m_stCurPlayerDesc.strKartPath)));
	m_stCurPlayerDesc.mapKartAnim.clear();
	int nNumOfAnim = varrKartAnim.GetNum();
	for(int i = 0; i < nNumOfAnim; i++)
	{
		m_stCurPlayerDesc.mapKartAnim.insert( make_pair(varrKartAnim.Element(i)->Element("anim_name")->String(), 
			varrKartAnim.Element(i)->Element("anim_id")->Integer()));
	}

	//加载表情列表
	CVarArray varrKartImage("kart_image");
	CVar::LoadVarsFromFile(&varrKartImage, NiApplication::ConvertMediaFilename(
		strTmp.FormatPt("%s/kart_image.lua", m_stCurPlayerDesc.strKartPath)));
	m_stCurPlayerDesc.mapKartImage.clear();
	int nNumOfImage = varrKartImage.GetNum();
	NiSourceTexturePtr spTex;
	for(int i = 0; i < nNumOfImage; i++)
	{
		spTex = NiSourceTexture::Create(NiApplication::ConvertMediaFilename(
			strTmp.FormatPt("%s/Image/FaceImage/%s", m_stCurPlayerDesc.strKartPath,
			varrKartImage.Element(i)->Element("image_name")->String())));

		m_stCurPlayerDesc.mapKartImage.insert( make_pair(varrKartImage.Element(i)->Element("anim_id")->Integer(),
			spTex) );
	}

	//加载特效图
	CVarArray varrKartEffect("KartEffect");
	CVar::LoadVarsFromFile(&varrKartEffect, NiApplication::ConvertMediaFilename(
		strTmp.FormatPt("%s/kart_effect.lua", m_stCurPlayerDesc.strKartPath)));	
	m_stCurPlayerDesc.mapKartEffect.clear();
	int nNumOfEffect = varrKartEffect.GetNum();
	for(int i = 0; i < nNumOfEffect; i++)
	{
		m_stCurPlayerDesc.mapKartEffect.insert( make_pair(varrKartEffect.Element(i)->Element("effect_id")->Integer(),
			varrKartEffect.Element(i)->Element("node_name")->String()));		
	}
}


//--------------------------------------------------------------------------------------------------------
bool Play::SetActiveCharacter(unsigned int unID)
{
	LoadCharacterDesc(unID);	
	if (RMOperator::GetUseShaders())
	{
		CStringEx strTmp;
		//最后的状态位设为true才能播放骨骼动画
		m_spCharacterAM = NiActorManager::Create(NiApplication::ConvertMediaFilename(
			strTmp.FormatPt("%s/Model/one.kfm", m_stCurPlayerDesc.strChacaterPath)), true);
		if(!m_spCharacterAM)
		{
			return false;
		}
	}	

	//注册动作开始和结束的回调
	m_spCharacterAM->SetCallbackObject(this);
	StringToIntMap::const_iterator anim_it;
	for(anim_it = m_stCurPlayerDesc.mapCharacterAnim.begin(); anim_it != m_stCurPlayerDesc.mapCharacterAnim.end(); anim_it++)
	{
		m_spCharacterAM->RegisterCallback(NiActorManager::ANIM_ACTIVATED,
			anim_it->second);
		m_spCharacterAM->RegisterCallback(NiActorManager::END_OF_SEQUENCE,
			anim_it->second);
	}

	m_spPlayer = NiDynamicCast(NiNode, m_spCharacterAM->GetNIFRoot()->GetObjectByName("Avatar"));
	assert(m_spPlayer);
	NiTriBasedGeom* pkFaceShape = NiDynamicCast(NiTriBasedGeom, m_spPlayer->GetObjectByName("face"));
	m_spOriginFaceTexProp = (NiTexturingProperty*) pkFaceShape->GetProperty(NiProperty::TEXTURING);
	m_spOriginFaceTex = m_spOriginFaceTexProp->GetBaseTexture();
	m_spCharacterAM->SetTargetAnimation(m_stCurPlayerDesc.mapCharacterAnim["Idle"]);
	return true;
}
//--------------------------------------------------------------------------------------------------------
bool Play::SetActiveKart(unsigned int unID)
{
	LoadKartDesc(unID);
	if (RMOperator::GetUseShaders())
	{
		CStringEx strTmp;
		//最后的状态位设为true才能播放骨骼动画
		m_spKartAM = NiActorManager::Create(NiApplication::ConvertMediaFilename(
			strTmp.FormatPt("%s/Model/car.kfm", m_stCurPlayerDesc.strKartPath)), true);
		if(!m_spKartAM)
		{
			return false;
		}
		PrepackNode(m_spKartAM->GetNIFRoot());		
	}


	//注册动作开始和结束的回调
	m_spKartAM->SetCallbackObject(this);
	StringToIntMap::const_iterator anim_it;
	for(anim_it = m_stCurPlayerDesc.mapKartAnim.begin(); anim_it != m_stCurPlayerDesc.mapKartAnim.end(); anim_it++)
	{
		m_spKartAM->RegisterCallback(NiActorManager::ANIM_ACTIVATED,
			anim_it->second);
		m_spKartAM->RegisterCallback(NiActorManager::END_OF_SEQUENCE,
			anim_it->second);
	}

	return true;
}

bool Play::LoadHairDesc(unsigned int unID)
{
	CStringEx strTmp;
	ItemUnit* pItem=CAvatarMgr::Get()->GetItemInfoById(unID);

	m_stCurPlayerDesc.unHairID		= pItem->nId; 
	m_stCurPlayerDesc.strHairName	= pItem->strName.c_str();
	m_stCurPlayerDesc.strHairPath	= strTmp.FormatPt("Avatar/%s/%u/",
		CAvatarMgr::Get()->GetItemDataMgr()->GetTypeName(pItem->nType),
		pItem->nId);

	//加载动作列表
	CVarArray varrHairAnim("hair_anim");
	bool bSuccess = CVar::LoadVarsFromFile(&varrHairAnim, NiApplication::ConvertMediaFilename(
		strTmp.FormatPt("%s/hair_anim.lua", m_stCurPlayerDesc.strHairPath)));

	if (!bSuccess) 
	{
		return false;
	}
	m_stCurPlayerDesc.mapHairAnim.clear();
	int nNumOfAnim = varrHairAnim.GetNum();
	for(int i = 0; i < nNumOfAnim; i++)
	{
		m_stCurPlayerDesc.mapHairAnim[varrHairAnim.Element(i)->Element("anim_name")->String()] 
			 = varrHairAnim.Element(i)->Element("anim_id")->Integer();
	}	

	return true;
}

bool Play::ChangeHair(unsigned int unID)
{
	// 加载头发描述文件 
	if (!LoadHairDesc(unID))
		return false;
	if (RMOperator::GetUseShaders())
	{
		CStringEx strTmp;
		//最后的状态位设为true才能播放骨骼动画
		m_spHairAM = NiActorManager::Create(NiApplication::ConvertMediaFilename(
			strTmp.FormatPt("%s/Model/hair.kfm", m_stCurPlayerDesc.strHairPath)), true);
		if(!m_spHairAM)
		{
			return false;
		}
		PrepackNode(m_spHairAM->GetNIFRoot());		
	}


	//注册动作开始和结束的回调
	m_spHairAM->SetCallbackObject(this);
	StringToIntMap::const_iterator anim_it;
	StringToIntMap::const_iterator end = m_stCurPlayerDesc.mapHairAnim.end();
	for(anim_it = m_stCurPlayerDesc.mapHairAnim.begin(); anim_it != end; anim_it++)
	{
		m_spHairAM->RegisterCallback(NiActorManager::ANIM_ACTIVATED,
			anim_it->second);
		m_spHairAM->RegisterCallback(NiActorManager::END_OF_SEQUENCE,
			anim_it->second);
	}

	return BindHairToHead();
}

// add by hoverzhao_for new car
bool Play::IsNewCar()
{
	if ( GetKartID() < 10017)
		return false;

	return true;
}

bool Play::BindHairToHead()
{
	
	return true;
}

bool Play::ChangeItem(unsigned int unID)
{
	//unsigned int unActiveKartID = unID;
	/*
	switch(unID)
	{
	case 1:
	case 2:
	case 3:
		unID = 10003;
	break;
	case 4:
		unID = 10017;
	break;
	case 5:
		unID = 10019;
	break;
	case 6:
		unID = 10020;
	break;
	default:	
		break;
	}
	*/
	unsigned int unItemBaseType = CAvatarMgr::Get()->GetItemBaseType(unID);
	if(unItemBaseType != EAIBT_ROLE && 
		unItemBaseType != EAIBT_CAR &&
		unItemBaseType != EAIBT_HAIR)
	{
		return false;
	}
	
	bool bDirtyFlag = false;
	//Actor::Release();
	if(unItemBaseType == EAIBT_ROLE)
	{		
		// 已经有了就不要再折磨人了
		if (m_stCurPlayerDesc.unCharacterID == unID)
			return true;
		UnLoadCharacter();
		m_stCurPlayerDesc.unCharacterID = unID;
		m_stPlayInfo.m_nRoleID = unID;
		SetActiveCharacter(m_stPlayInfo.m_nRoleID);
		bDirtyFlag = true;
	}
	else if(unItemBaseType == EAIBT_CAR)
	{		
		// 已经有了就不要再折磨人了
		if (m_stCurPlayerDesc.unKartID == unID)
			return true;
		UnloadKart();
		m_stCurPlayerDesc.unKartID = unID;
		m_stPlayInfo.m_nKartID=unID;
		SetActiveKart(m_stPlayInfo.m_nKartID);	
	 	InitWheelPos();
		bDirtyFlag = true;
	}
	else if (unItemBaseType = EAIBT_HAIR)
	{
		if (m_stCurPlayerDesc.unCharacterID < 10018)
			return true;
		// 已经有了就不要再折磨人了
		if (m_stCurPlayerDesc.unHairID == unID)
			return true;
		/*
		UnLoadHair();
		m_stCurPlayerDesc.unHairID = unID;	

		ChangeHair(m_stCurPlayerDesc.unHairID);
		*/
		m_stCurPlayerDesc.unHairID = unID;
		//使用新方法换头发
		CRequestChangeItemStatus insSta;
		insSta.m_sItemNum=1;
		insSta.m_unUin=m_stPlayInfo.m_unUin;
		insSta.m_unTime=::GetTickCount();
		insSta.m_stItemStatus[0].m_bNewStatus=1;
		insSta.m_stItemStatus[0].m_iItemID=unID;
		ItemStore* pStore=CAvatarMgr::Get()->GetItemDataMgr()->GetItemStore(m_stPlayInfo.m_unUin,unID);
		if (!pStore)
		{//没有则加一下这个头发
			STPlayerDBItemInfo insDbInfo;
			insDbInfo.m_bStatus=0;
			insDbInfo.m_iItemNum=1;
			insDbInfo.m_iItemID=unID;
			pStore=CAvatarMgr::Get()->GetItemDataMgr()->AddItemStore(m_stPlayInfo.m_unUin,&insDbInfo,false);//不更新副本
		}
		CAvatarMgr::Get()->ChangeItemStatus(&insSta,true);//直接装备

		bDirtyFlag = true;
	}
	

	//if ( m_stCurPlayerDesc.unCharacterID < 10018 )
	//	m_stCurPlayerDesc.unKartID = 10003;

	if (!bDirtyFlag)
		return true;
	//Actor::Initialize(9, m_cQQName);
	ReBindAll();
	
	Release();
#ifndef _XCART_
	m_pKart = new CKart();
#else
	m_pKart = new XCart();
#endif

	SetPos(m_kPos);
	SetRotate(NiPoint3(0.0f,0.0f,1.0f),90.0f);
	GetKartNIFRoot()->Update(GetMainApp()->GetCurrentTime());

	// 本地玩家不显示自己的名字
	if (IsHost()) 
	{
		ShowName(false);
	}
	//ChangeTeam(m_stPlayInfo.m_nTeamID);
	m_pAnimEventMgr = NiNew CAnimEventMgr(this);

	//残影特效
	NiAVObject * pObject1 = GetKartNIFRoot()->GetObjectByName("LB_W");
	NiAVObject * pObject2 = GetKartNIFRoot()->GetObjectByName("RB_W");

	BOOL bCarSwoosh=FALSE;
	if ( GlobalInterface::m_pSettings )
	{
		GlobalInterface::m_pSettings->GetIntValue("CarSwoosh", &bCarSwoosh);
		if (bCarSwoosh)
		{
			m_pkSwooshL = NiNew Swoosh(pObject1,GetMainApp()->GetCurrentTime());
			m_pkSwooshL->SetActivity(true);
			m_pkSwooshR = NiNew Swoosh(pObject2,GetMainApp()->GetCurrentTime());
			m_pkSwooshR->SetActivity(true);
		}
	}

	return true;
}

void ClearUpObsoleteNodes(NiNode* pNode);
//---------------------------------------------------------------------------
void Play::Update(float fTime, float fFrameTime, const NiPoint2& kDir,
    const float& fStrafeDir, bool bAnimationUpdate)
{
	//为了车后退时转头（可考虑以后去掉）
	if(this->IsHost() && m_pKart)
	{
		if(m_pKart->GetVehicle())
		{
			if(m_pKart->GetVelocityLocal().x * GameSetting::SpeedDispCoef < -0.9f && 
				!m_pKart->IsDrifting())
			{
				m_pAnimEventMgr->PushAnimEvent(CAnimEventMgr::EAET_TURNBACK);
				m_bIsTurnBack = true;
			}
			else
			{
				if(m_bIsTurnBack)
				{
					m_pAnimEventMgr->PushAnimEvent(CAnimEventMgr::EAET_STILL);
					m_bIsTurnBack = false;
				}
			}
		}

	}

	//执行动作队列
	m_pAnimEventMgr->ExecuteEvent();

	CATWearer::Update(fTime);


	ClearUpObsoleteNodes(NiDynamicCast(NiNode, m_spKartAM->GetNIFRoot()->GetObjectByName("center")));
	Actor::Update(fTime, fFrameTime, kDir/*kMommaDir*/, fStrafeDir, 
		bAnimationUpdate);

	//残影
	if (m_pkSwooshL && m_pkSwooshR)
	{
		m_pkSwooshL->TakeSample(GetMainApp()->GetAccumTime());
		m_pkSwooshR->TakeSample(GetMainApp()->GetAccumTime());
	}
}
//---------------------------------------------------------------------------
NiActorManager::EventCode Play::InterpretInput(const NiPoint2& kDir,
    const float& fStrafeDir, float fTime)
{
	return 0;
}
//---------------------------------------------------------------------------
void Play::InitializeRandomIdle()
{
//    m_kRandomIdle.SetSize(10);
//	m_kRandomIdle.SetAt(0, left_Anim::IDLE);
 
}
//---------------------------------------------------------------------------
void Play::HandleTextKeyEvent()
{
}
//---------------------------------------------------------------------------
const NiPoint2 Play::AutoDir(float fTime)
{
    bool bTimeExpired = 
        (fTime - m_fPausedTime > m_fMaxAutoGoalSeekTime);
#ifdef _DEBUG
    if (bTimeExpired)
        NiOutputDebugString("AutoDir time expired, changing goals...\n");
#endif

    if ((m_kAutoGoal - Get2DLenguinTranslate()).Length() < 1000.0f ||
        bTimeExpired)
    {
        // pause for a moment between going from goal to goal
        if (fTime - m_fPausedTime < m_fPauseTime)
        {
            return NiPoint2::ZERO;
        }
        else
        {
            m_fPausedTime = fTime;
        }

        ComputeAutoGoal();
    }

    NiPoint2 kAutoDir = m_kAutoGoal - Get2DLenguinTranslate();
    kAutoDir.Unitize();
    if (m_bAutoWalk)
        kAutoDir *= 0.4f;
    return kAutoDir;
}
//---------------------------------------------------------------------------
void Play::ComputeAutoGoal()
{

}
//---------------------------------------------------------------------------
void Play::AnimActivated(NiActorManager* pkManager,
    NiActorManager::SequenceID eSequenceID, float fCurrentTime, 
    float fEventTime)
{
	if(pkManager == m_spCharacterAM)
	{
		if(NxSceneMgr::Get()->GetGameStatusManager()->GetGameState() & GameStatusManager::GST_IN_GAME)
		{
			if(eSequenceID == m_stCurPlayerDesc.mapCharacterAnim["Still"])
			{
				ChangeAnimFaceImage(eSequenceID);
			}
			else if(eSequenceID == m_stCurPlayerDesc.mapCharacterAnim["Idle"])
			{
				ChangeAnimFaceImage(eSequenceID);
			}
			else if(eSequenceID == m_stCurPlayerDesc.mapCharacterAnim["SeeLeft"])
			{
				ChangeAnimFaceImage(eSequenceID);
			}
			else if(eSequenceID == m_stCurPlayerDesc.mapCharacterAnim["SeeRight"])
			{
				ChangeAnimFaceImage(eSequenceID);
			}
			else if(eSequenceID == m_stCurPlayerDesc.mapCharacterAnim["TurnLeft"])
			{
				ChangeAnimFaceImage(eSequenceID);
			}
			else if(eSequenceID == m_stCurPlayerDesc.mapCharacterAnim["TurnRight"])
			{
				ChangeAnimFaceImage(eSequenceID);
			}
			else if(eSequenceID == m_stCurPlayerDesc.mapCharacterAnim["TurnBack"])
			{
				ChangeAnimFaceImage(eSequenceID);
			}
			else if(eSequenceID == m_stCurPlayerDesc.mapCharacterAnim["SpeedUp"])
			{
				ChangeAnimFaceImage(eSequenceID);
			}
			else if(eSequenceID == m_stCurPlayerDesc.mapCharacterAnim["Hurt"])
			{
				ChangeAnimFaceImage(eSequenceID);
			}
			else if(eSequenceID ==  m_stCurPlayerDesc.mapCharacterAnim["Win"])
			{
				ChangeAnimFaceImage(eSequenceID);
			}
			else if(eSequenceID ==  m_stCurPlayerDesc.mapCharacterAnim["Finish"])
			{
				ChangeAnimFaceImage(eSequenceID);
			}
			else if(eSequenceID ==  m_stCurPlayerDesc.mapCharacterAnim["UnFinish"])
			{
				ChangeAnimFaceImage(eSequenceID);
			}
			else if(eSequenceID ==  m_stCurPlayerDesc.mapCharacterAnim["GetShot"])
			{
				ChangeAnimFaceImage(eSequenceID);
			}
			else if(eSequenceID ==  m_stCurPlayerDesc.mapCharacterAnim["UseItem"])
			{
				ChangeAnimFaceImage(eSequenceID);
			}
			else if(eSequenceID ==  m_stCurPlayerDesc.mapCharacterAnim["Usebanana"])
			{
				ChangeAnimFaceImage(eSequenceID);
			}
		}
		else if(NxSceneMgr::Get()->GetGameStatusManager()->GetGameState() == GameStatusManager::GST_ROOM )
		{
			if(eSequenceID == m_stCurPlayerDesc.mapCharacterAnim["Cry"])
			{
				ChangeAnimFaceImage(eSequenceID);
			}
			else if(eSequenceID == m_stCurPlayerDesc.mapCharacterAnim["SpeechLess"])
			{
				ChangeAnimFaceImage(eSequenceID);
			}
			else if(eSequenceID == m_stCurPlayerDesc.mapCharacterAnim["Tired"])
			{
				ChangeAnimFaceImage(eSequenceID);
			}
			else if(eSequenceID == m_stCurPlayerDesc.mapCharacterAnim["Cute"])
			{
				ChangeAnimFaceImage(eSequenceID);
			}
			else if(eSequenceID == m_stCurPlayerDesc.mapCharacterAnim["Jealousy"])
			{
				ChangeAnimFaceImage(eSequenceID);
			}
			else if(eSequenceID == m_stCurPlayerDesc.mapCharacterAnim["Despair"])
			{
				ChangeAnimFaceImage(eSequenceID);
			}
			else if(eSequenceID == m_stCurPlayerDesc.mapCharacterAnim["ActUp"])
			{
				ChangeAnimFaceImage(eSequenceID);
			}
			else if(eSequenceID == m_stCurPlayerDesc.mapCharacterAnim["Angry"])
			{
				ChangeAnimFaceImage(eSequenceID);
			}
			else if(eSequenceID == m_stCurPlayerDesc.mapCharacterAnim["Love"])
			{
				ChangeAnimFaceImage(eSequenceID);
			}
			else if(eSequenceID == m_stCurPlayerDesc.mapCharacterAnim["Doubt"])
			{
				ChangeAnimFaceImage(eSequenceID);
			}
			else if(eSequenceID == m_stCurPlayerDesc.mapCharacterAnim["Sweat"])
			{
				ChangeAnimFaceImage(eSequenceID);
			}
			else if(eSequenceID == m_stCurPlayerDesc.mapCharacterAnim["Fright"])
			{
				ChangeAnimFaceImage(eSequenceID);
			}
			else if(eSequenceID == m_stCurPlayerDesc.mapCharacterAnim["Freeze"])
			{
				ChangeAnimFaceImage(eSequenceID);
			}
			else if(eSequenceID == m_stCurPlayerDesc.mapCharacterAnim["Moved"])
			{
				ChangeAnimFaceImage(eSequenceID);
			}
			else if(eSequenceID == m_stCurPlayerDesc.mapCharacterAnim["Laugh"])
			{
				ChangeAnimFaceImage(eSequenceID);
			}
			else if(eSequenceID == m_stCurPlayerDesc.mapCharacterAnim["Talk"])
			{
				ChangeAnimFaceImage(eSequenceID);
			}
		}
	}
	else if(pkManager == m_spKartAM)
	{

	}
	
	
}
//---------------------------------------------------------------------------
void Play::AnimDeactivated(NiActorManager* pkManager,
							 NiActorManager::SequenceID eSequenceID, float fCurrentTime, 
							 float fEventTime) 
{
	if(pkManager == m_spCharacterAM)
	{
		if( NxSceneMgr::Get()->GetGameStatusManager()->GetGameState() == GameStatusManager::GST_ROOM )
		{
			RecoverFaceTexture();
			if(eSequenceID != m_stCurPlayerDesc.mapCharacterAnim["stand1"] && eSequenceID != m_stCurPlayerDesc.mapCharacterAnim["stand2"])
			{
				m_spCharacterAM->SetTargetAnimation(m_stCurPlayerDesc.mapCharacterAnim["stand2"]);
			}
			if(eSequenceID == m_stCurPlayerDesc.mapCharacterAnim["Cry"])
			{

			}
			else if(eSequenceID == m_stCurPlayerDesc.mapCharacterAnim["TurnLeft"])
			{

			}
			else if(eSequenceID == m_stCurPlayerDesc.mapCharacterAnim["TurnRight"])
			{

			}
			else if(eSequenceID == m_stCurPlayerDesc.mapCharacterAnim["TurnBack"])
			{

			}
			else if(eSequenceID == m_stCurPlayerDesc.mapCharacterAnim["SpeedUp"])
			{

			}
		}
		else if( NxSceneMgr::Get()->GetGameStatusManager()->GetGameState() & GameStatusManager::GST_IN_GAME )
		{
			RecoverFaceTexture();
			if(eSequenceID == m_stCurPlayerDesc.mapCharacterAnim["Still"])
			{

			}
			else if(eSequenceID == m_stCurPlayerDesc.mapCharacterAnim["Idle"])
			{

			}
			else if(eSequenceID == m_stCurPlayerDesc.mapCharacterAnim["SeeLeft"])
			{

			}
			else if(eSequenceID == m_stCurPlayerDesc.mapCharacterAnim["SeeRight"])
			{

			}
			else if(eSequenceID == m_stCurPlayerDesc.mapCharacterAnim["TurnLeft"])
			{

			}
			else if(eSequenceID == m_stCurPlayerDesc.mapCharacterAnim["TurnRight"])
			{

			}
			else if(eSequenceID == m_stCurPlayerDesc.mapCharacterAnim["TurnBack"])
			{

			}
			else if(eSequenceID == m_stCurPlayerDesc.mapCharacterAnim["SpeedUp"])
			{

				CALL(CGameEffectMgr::Get(), ActiveEffect(IGameEffect::ET_SMOKE, 0));
				m_spCharacterAM->SetTargetAnimation(m_stCurPlayerDesc.mapCharacterAnim["Still"]);

			}
			else if(eSequenceID == m_stCurPlayerDesc.mapCharacterAnim["Hurt"])
			{

				CALL(CGameEffectMgr::Get(), DeActiveEffect(IGameEffect::ET_CRASH));
				m_spCharacterAM->SetTargetAnimation(m_stCurPlayerDesc.mapCharacterAnim["Still"]);
			}	
			else if(eSequenceID ==  m_stCurPlayerDesc.mapCharacterAnim["Win"])
			{

			}
			else if(eSequenceID ==  m_stCurPlayerDesc.mapCharacterAnim["Finish"])
			{

			}
			else if(eSequenceID ==  m_stCurPlayerDesc.mapCharacterAnim["UnFinish"])
			{

			}
			else if(eSequenceID ==  m_stCurPlayerDesc.mapCharacterAnim["GetShot"])
			{
				m_spCharacterAM->SetTargetAnimation(m_stCurPlayerDesc.mapCharacterAnim["Still"]);
			}
			else if(eSequenceID ==  m_stCurPlayerDesc.mapCharacterAnim["UseItem"])
			{
				m_spCharacterAM->SetTargetAnimation(m_stCurPlayerDesc.mapCharacterAnim["Still"]);
			}
			else if(eSequenceID ==  m_stCurPlayerDesc.mapCharacterAnim["Usebanana"])
			{
				m_spCharacterAM->SetTargetAnimation(m_stCurPlayerDesc.mapCharacterAnim["Still"]);
			}

		}
	}
	else if(pkManager == m_spKartAM)
	{
		if(eSequenceID == m_stCurPlayerDesc.mapKartAnim["Hurt"])
		{
			m_spKartAM->SetTargetAnimation(m_stCurPlayerDesc.mapKartAnim["Still"]);
		}
		if(eSequenceID == m_stCurPlayerDesc.mapKartAnim["Banana"])
		{
			m_spKartAM->SetTargetAnimation(m_stCurPlayerDesc.mapKartAnim["Still"]);
		}
	}
	
}
//---------------------------------------------------------------------------
void Play::EndOfSequence(NiActorManager *pkManager, 
                   NiActorManager::SequenceID eSequenceID, float fCurrentTime, 
                   float fEventTime)
{
	if(pkManager == m_spCharacterAM)
	{
		if( NxSceneMgr::Get()->GetGameStatusManager()->GetGameState() == GameStatusManager::GST_ROOM )
		{	
			RecoverFaceTexture();
			if(eSequenceID != m_stCurPlayerDesc.mapCharacterAnim["stand1"] && eSequenceID != m_stCurPlayerDesc.mapCharacterAnim["stand2"])
			{
				m_spCharacterAM->SetTargetAnimation(m_stCurPlayerDesc.mapCharacterAnim["stand2"]);
			}
			if(eSequenceID == m_stCurPlayerDesc.mapCharacterAnim["Cry"])
			{
				
			}
			else if(eSequenceID == m_stCurPlayerDesc.mapCharacterAnim["TurnLeft"])
			{

			}
			else if(eSequenceID == m_stCurPlayerDesc.mapCharacterAnim["TurnRight"])
			{

			}
			else if(eSequenceID == m_stCurPlayerDesc.mapCharacterAnim["TurnBack"])
			{

			}
			else if(eSequenceID == m_stCurPlayerDesc.mapCharacterAnim["SpeedUp"])
			{
				
			}
			else if(eSequenceID == m_stCurPlayerDesc.mapCharacterAnim["SeeLeft"])
			{

			}
		}
		else if( NxSceneMgr::Get()->GetGameStatusManager()->GetGameState() & GameStatusManager::GST_IN_GAME )
		{
			RecoverFaceTexture();
			if(eSequenceID == m_stCurPlayerDesc.mapCharacterAnim["Still"])
			{

			}
			else if(eSequenceID == m_stCurPlayerDesc.mapCharacterAnim["Idle"])
			{

			}
			else if(eSequenceID == m_stCurPlayerDesc.mapCharacterAnim["SeeLeft"])
			{

			}
			else if(eSequenceID == m_stCurPlayerDesc.mapCharacterAnim["SeeRight"])
			{

			}
			else if(eSequenceID == m_stCurPlayerDesc.mapCharacterAnim["TurnLeft"])
			{

			}
			else if(eSequenceID == m_stCurPlayerDesc.mapCharacterAnim["TurnRight"])
			{

			}
			else if(eSequenceID == m_stCurPlayerDesc.mapCharacterAnim["TurnBack"])
			{

			}
			else if(eSequenceID == m_stCurPlayerDesc.mapCharacterAnim["SpeedUp"])
			{
//				RecoverFaceTexture();
				CALL(CGameEffectMgr::Get(), ActiveEffect(IGameEffect::ET_SMOKE, 0));
//				NxSceneMgr::Get()->GetCameraController()->SetCameraParamType(CameraController::CPT_NORMAL);
				m_spCharacterAM->SetTargetAnimation(m_stCurPlayerDesc.mapCharacterAnim["Still"]);
			}
			else if(eSequenceID == m_stCurPlayerDesc.mapCharacterAnim["Hurt"])
			{
				RecoverFaceTexture();
				CALL(CGameEffectMgr::Get(), DeActiveEffect(IGameEffect::ET_CRASH));
				m_spCharacterAM->SetTargetAnimation(m_stCurPlayerDesc.mapCharacterAnim["Still"]);
			}
			else if(eSequenceID ==  m_stCurPlayerDesc.mapCharacterAnim["Win"])
			{
//				RecoverFaceTexture();
			}
			else if(eSequenceID ==  m_stCurPlayerDesc.mapCharacterAnim["Finish"])
			{
//				RecoverFaceTexture();
			}
			else if(eSequenceID ==  m_stCurPlayerDesc.mapCharacterAnim["UnFinish"])
			{
//				RecoverFaceTexture();
			}
			else if(eSequenceID ==  m_stCurPlayerDesc.mapCharacterAnim["GetShot"])
			{
				m_spCharacterAM->SetTargetAnimation(m_stCurPlayerDesc.mapCharacterAnim["Still"]);
//				RecoverFaceTexture();
			}
			else if(eSequenceID ==  m_stCurPlayerDesc.mapCharacterAnim["UseItem"])
			{
				m_spCharacterAM->SetTargetAnimation(m_stCurPlayerDesc.mapCharacterAnim["Still"]);
				//				RecoverFaceTexture();
			}
			else if(eSequenceID ==  m_stCurPlayerDesc.mapCharacterAnim["Usebanana"])
			{
				m_spCharacterAM->SetTargetAnimation(m_stCurPlayerDesc.mapCharacterAnim["Still"]);
				//				RecoverFaceTexture();
			}

		}	
	}
	else if(pkManager == m_spKartAM)
	{
		if(eSequenceID == m_stCurPlayerDesc.mapKartAnim["Hurt"])
		{
			m_spKartAM->SetTargetAnimation(m_stCurPlayerDesc.mapKartAnim["Still"]);
		}
		if(eSequenceID == m_stCurPlayerDesc.mapKartAnim["Banana"])
		{
			m_spKartAM->SetTargetAnimation(m_stCurPlayerDesc.mapKartAnim["Still"]);
		}
	}
	
}
//---------------------------------------------------------------------------
void Play::Restart()
{
	m_bIsFinish = false;
	m_bIsWin = false;
    m_spKartAM->GetNIFRoot()->SetTranslate(m_kPos);
    m_spKartAM->GetNIFRoot()->SetRotate( NiQuaternion(0,0,0,0) );
    m_kBipQuat = NiQuaternion(0,0,0,0);
    m_kLookAtQuat = NiQuaternion(0,0,0,0);
}
//---------------------------------------------------------------------------
void Play::SetPos(const NiPoint3& kPos)
{
	m_kPos = kPos;
	CTencentCar* pCar = GetKart()->GetVehicle();
	if ( pCar )
	{
		pCar->SetPosGlobal(NxVec3(kPos.x, kPos.y, kPos.z));
	}
	/*
	if(m_spKartAM)
	{
	m_spKartAM->GetNIFRoot()->SetTranslate(m_kPos);
	}
	*/
}

void Play::SitPos(NiTransform Trans )
{
	NxMat34 mPose;

	NiMatrix3 kXRot90;
	kXRot90.MakeXRotation(-NI_PI/2.0f);

	NiMatrix3 kYRot90;
	kYRot90.MakeYRotation(NI_PI/2.0f);

	Trans.m_Rotate = Trans.m_Rotate * kXRot90 * kYRot90;
	NiPhysXTypes:: NiTransformToNxMat34(Trans.m_Rotate, Trans.m_Translate, mPose);

	GetKart()->SetKartPose(mPose);

	NiPoint3 kRightDir;

	Trans.m_Rotate.GetCol(0, kRightDir);

	kRightDir.z = 0;

	kRightDir.Unitize();

	float fRad = 0.0f;
	if (fabs(kRightDir.x) >= 0.00001f)
	{
		fRad = NxMath::atan(kRightDir.y/kRightDir.x);
		if (fRad > 0 && kRightDir.x < 0)
		{
			fRad += NI_PI;			
		}
		if (fRad < 0 && kRightDir.x < 0)
		{
			fRad -= NI_PI;
		}

	}
	else
	{
		if (kRightDir.x > 0)
			fRad = 0.0f;
		else
			fRad = NI_PI;
	}
	LogText("●●●●●玩家 %s 初始角度 = %f :)●●●●●\n", GetPlayName(), fRad/NI_PI*180.0f);
#ifndef _XCART_
	CKart * pCart = GetKart();
#else
	XCart * pCart = GetKart();
#endif
	if (pCart)
	{
		if (IsHost())
			pCart->SetTurnAngle(fRad);
		else
		{
			NxMat33 ori;
			ori.id();
			ori.rotZ(fRad);
			pCart->GetVehicle()->SetOriGlobal(ori);
			pCart->GetVehicle()->SetMatGround(ori);
		}
	}

	//SetPos(NiPoint3(mPose.t.x, mPose.t.y, mPose.t.z));
	//m_kPos = Trans.m_Translate;
	//m_spAM->GetNIFRoot()->SetTranslate(Trans.m_Translate);
	//m_spAM->GetNIFRoot()->SetRotate(Trans.m_Rotate);
	//m_spAM->GetNIFRoot()->Update(0.0f);
}

void Play::SetFinish(bool bIsFinsh)
{
	m_bIsFinish  = bIsFinsh;
	InputManager::keys[NiInputKeyboard::KEY_A]		= false;
	InputManager::keys[NiInputKeyboard::KEY_D]		= false;
	InputManager::keys[NiInputKeyboard::KEY_W]		= false;
	InputManager::keys[NiInputKeyboard::KEY_S]		= false;
	InputManager::keys[NiInputKeyboard::KEY_LEFT]	= false;
	InputManager::keys[NiInputKeyboard::KEY_RIGHT]	= false;
	InputManager::keys[NiInputKeyboard::KEY_UP]		= false;
	InputManager::keys[NiInputKeyboard::KEY_DOWN]	= false;
	//CALL(m_pKart, SetMovable(!bIsFinsh));
}

void Play::SetWin(bool bWin)
{
	m_bIsWin = bWin;


	if( m_bIsWin) 
		m_pAnimEventMgr->PushAnimEvent(CAnimEventMgr::EAET_FINISH);
	
	else
		m_pAnimEventMgr->PushAnimEvent(CAnimEventMgr::EAET_UNFINISH);

	m_spCharacterAM->Update(0.0f);


}
//---------------------------------------------------------------------------
void Play::SetRotate(const NiPoint3& kVer,const float& fAngle)
{
	if(m_spKartAM)
	{
		m_spKartAM->GetNIFRoot()->SetRotate(fAngle,kVer.x,kVer.y,kVer.z);
	}
};
//---------------------------------------------------------------------------
void Play::SetRotateInc(const NiPoint3& kVer,const float& fAngle)
{
	if(m_spKartAM)
	{
		NiMatrix3 m1,m2;
		
		m1.MakeZRotation(fAngle);
		m2 = m_spKartAM->GetNIFRoot()->GetRotate();
		m2 = m1 * m2;

		m_spKartAM->GetNIFRoot()->SetRotate(m2);
	}
}
//---------------------------------------------------------------------------
#ifndef _XCART_
void Play::SetKart(CKart* pKart)
#else
void Play::SetKart(XCart* pKart)
#endif
{
	m_pKart = pKart;
}
//---------------------------------------------------------------------------
bool Play::StartAnimation(NiActorManager::EventCode eCode)
{
	m_spCharacterAM->SetTargetAnimation(eCode);
	return false;
}
//-----------------------------------------------------------------------

void Play::SetKartParam(void * pData , float fSwooshFactor )
{
	return ;
}

void Play::GetKartParam(void * pData)
{
	STUDPPlayMove * aData = ( STUDPPlayMove * )pData;
#ifndef _XCART_
	CKart * pKart  = GetKart();
#else
	XCart* pKart  = GetKart();
#endif
	if( pKart ) 
	{
		//行驶状态：是否漂移
		aData->m_cisDrifting = false; 

		//速度：
		NxVec3 vel;

		vel = pKart->GetVelocityLocal();

		aData->m_unLinearVelocity[0] = vel.x * SCALE_FACTOR;
		aData->m_unLinearVelocity[1] = vel.y * SCALE_FACTOR;
		aData->m_unLinearVelocity[2] = vel.z * SCALE_FACTOR;

		//位置：
		
		NxVec3 vPos = pKart->GetPos();
		aData->m_unArrPosition[0] = vPos.x * SCALE_FACTOR;
		aData->m_unArrPosition[1] = vPos.y * SCALE_FACTOR;
		aData->m_unArrPosition[2] = vPos.z * SCALE_FACTOR;

		aData->m_unAngle = pKart->GetTurnAngle() * SCALE_FACTOR;



		//朝向:
		if ( pKart->GetVehicle() )
		{
			NxMat33 ori = pKart->GetVehicle()->GetOriGlobal();
			for ( int i = 0; i < 3; i++ )
			{ 
				aData->m_unArrMat[3 * i + 0] = ori.getColumn(i).x * SCALE_FACTOR;
				aData->m_unArrMat[3 * i + 1] = ori.getColumn(i).y * SCALE_FACTOR;
				aData->m_unArrMat[3 * i + 2] = ori.getColumn(i).z * SCALE_FACTOR;
			}

			aData->m_unOffSetZ = pKart->GetVehicle()->GetOffsetZ() * SCALE_FACTOR;
		}


	

		aData->m_unTimeStamp = ::GetTickCount();
		aData->m_unUin		= GetPlayUin();
		aData->m_unPackageIndex = m_unMovePackageIndex;
		aData->m_unPlayAnimID = m_spCharacterAM->GetTargetAnimation();
		aData->m_unKartAnimID = m_spKartAM->GetTargetAnimation();

		if (CLevelManager::Get() && CLevelManager::Get()->GetLinks())
		{
			m_fRestMileage = CLevelManager::Get()->GetLinks()->GetRestMileage();
			aData->m_nRestDistance = m_fRestMileage * SCALE_FACTOR;
		}

		m_unMovePackageIndex++;
	}
}


void Play::UseWinCamera()
{
	
	NiNodePtr spCamNode = NiDynamicCast(NiNode, m_spKartAM->GetNIFRoot()->GetObjectByName("game over camera"));
//	NxSceneMgr::Get()->GetCameraController()->SetCamera(spCamNode);
	NxSceneMgr::Get()->GetCameraController()->GetLogicCamera(CCameraBase::ECT_MODEL)->SetCamera(spCamNode);
	NxSceneMgr::Get()->GetCameraController()->SetForeCam(CCameraBase::ECT_MODEL);
}

void Play::UseHeadCamra()
{
	NiNodePtr spCamNode = NiDynamicCast(NiNode, m_spKartAM->GetNIFRoot()->GetObjectByName("head camera"));
	NxSceneMgr::Get()->GetCameraController()->GetLogicCamera(CCameraBase::ECT_MODEL)->SetCamera(spCamNode);
	NxSceneMgr::Get()->GetCameraController()->SetForeCam(CCameraBase::ECT_MODEL);
}

void Play::UseStartCamera()
{

	NiNodePtr spCamNode = NiDynamicCast(NiNode, m_spKartAM->GetNIFRoot()->GetObjectByName("game start camera"));
	NxSceneMgr::Get()->GetCameraController()->GetLogicCamera(CCameraBase::ECT_MODEL)->SetCamera(spCamNode);
	NxSceneMgr::Get()->GetCameraController()->SetForeCam(CCameraBase::ECT_MODEL);
}

void Play::SetCharaterAnimation(UINT seqID)
{
	//m_kCriticalSection.Lock();
	seqID = AnimTranform(seqID);
	m_spCharacterAM->SetTargetAnimation(seqID);

	if (m_spHairAM)
		m_spHairAM->SetTargetAnimation(seqID);
	//m_kCriticalSection.Unlock();

}
//-----------------------------------------------------------------------------------------------------
void Play::SetCharaterAnimation(NiFixedString strAnimName)
{
	//m_kCriticalSection.Lock();

	string strName(strAnimName);
	StringToIntMap::const_iterator map_it = m_stCurPlayerDesc.mapCharacterAnim.find(strName);
	if(map_it != m_stCurPlayerDesc.mapCharacterAnim.end())
	{
		UINT unSeqID = AnimTranform(map_it->second);
		m_spCharacterAM->SetTargetAnimation(unSeqID);
	}
	if (m_spHairAM)
	{
		StringToIntMap::const_iterator hair_it = m_stCurPlayerDesc.mapHairAnim.find(strName);
		if (hair_it != m_stCurPlayerDesc.mapHairAnim.end())
		{
			UINT unSeqID = AnimTranform(hair_it->second);
			m_spHairAM->SetTargetAnimation(unSeqID);
		}
	}	
	//m_kCriticalSection.Unlock();

}
//-------------------------------------------------------------------------------------------------------
void Play::SetKartAnimation(UINT seqID)
{
	//m_kCriticalSection.Lock();

	m_spKartAM->SetTargetAnimation(seqID);
	//m_kCriticalSection.Unlock();

}
//-------------------------------------------------------------------------------------------------------
void Play::SetKartAnimation(NiFixedString strAnimName)
{
	//m_kCriticalSection.Lock();

	string strName(strAnimName);
	StringToIntMap::const_iterator map_it = m_stCurPlayerDesc.mapKartAnim.find(strName);
	if(map_it != m_stCurPlayerDesc.mapKartAnim.end())
	{
		m_spKartAM->SetTargetAnimation(map_it->second);
	}
	//m_kCriticalSection.Unlock();
}
//-------------------------------------------------------------------------------------------------------
void Play::PushPlayerAnimEvent(CAnimEventMgr::ENMANIMEVENTTYPE unEvent)
{
	m_pAnimEventMgr->PushAnimEvent(unEvent);
}
//-------------------------------------------------------------------------------------------------------
CAnimEventMgr* Play::GetAnimEventMgr()
{
	if(m_pAnimEventMgr)
		return m_pAnimEventMgr;
	return NULL;
}
//-------------------------------------------------------------------------------------------------------
void Play::ChangeAnimFaceImage(int ID)
{
	CStringEx strTmp;
	IntToSpTexMap::const_iterator map_it = m_stCurPlayerDesc.mapCharacterImage.find(ID);
	if(map_it != m_stCurPlayerDesc.mapCharacterImage.end())
	{
		ChangeFaceTexture(map_it->second);
	}
}

void Play::CollisionDetectionFromOtherPlayer(DWORD unID, CTencentCar * car, const NxVec3& velocity, const NxVec3& pos, const NxMat33& mat)
{
	if (GetKart() && GetKart()->GetVehicle())
		GetKart()->GetVehicle()->CollisionDetectionFromOtherPlayer(unID, car, velocity, pos, mat);
}
//--------------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------------

//#define OUTPUT_REMOTEPLAYER_INFO
#ifdef OUTPUT_REMOTEPLAYER_INFO
	FILE * g_pFile = 0;
	int    g_nCounter = 0;
#endif
RemotePlay::RemotePlay()
{
	m_cQQName[0] = '\0';
#ifdef OUTPUT_REMOTEPLAYER_INFO
	g_pFile = fopen(NiApplication::ConvertMediaFilename("remote_playerinfo.dat"), "w");
#endif
	
}

RemotePlay::~RemotePlay()
{
#ifdef OUTPUT_REMOTEPLAYER_INFO
	if (g_pFile)
		fclose(g_pFile);
	g_pFile = NULL;
#endif
}

void RemotePlay::Update(float fTime, float fFrameTime, const NiPoint2& kDir, 
					 const float& fStrafeDir, bool bAnimationUpdate)
{
	fTime = GetMainApp()->GetAccumTime();

	/*NiActorManager::EventCode eCode = InterpretInput(NiPoint2(0, 0), fStrafeDir,
		fTime);

	if (bAnimationUpdate)
	{
		m_spCharacterAM->SetTargetAnimation(eCode);
		m_spCharacterAM->Update(fTime);
	}*/
	Play::Update(fTime, fFrameTime, kDir, fStrafeDir, bAnimationUpdate);

	// 计算名字离镜头距离，根据距离进行缩放处理
	NiCamera * pkCamera = NxSceneMgr::Get()->GetCameraController()->GetForeCam()->GetCamera();
	if(m_bShowName && pkCamera) 
	{		
		const NiFrustum & kFrustum = pkCamera->GetViewFrustum();


		float fNear = kFrustum.m_fNear;
		float fWidth = kFrustum.m_fRight - kFrustum.m_fLeft;

		NiPoint3 kDistance = (pkCamera->GetWorldTranslate() - m_spkNameBoard->GetWorldTranslate());
		float fDistance = kDistance.Length();

		if (fDistance < 3.5f) {
			fDistance = 3.5f;
		}

		float fRate = fDistance / 3.5f;

		m_spkNameBoard->SetTranslate(0, 0, -fRate * 0.7f  + 0.7f);

		m_spkNameBoard->SetScale(fRate);
		m_spkNameBoard->UpdateNodeBound();
	}

//	NiActorManager::SequenceID eSequence = m_spAM->GetTargetAnimation();
	// TO DO: Consider bAnimationUpdate
#if !defined(WIN32)
	m_spKartAM->GetNIFRoot()->Update(fTime);
#else
	m_spKartAM->GetNIFRoot()->Update(fTime);
#endif	
	ClearUpObsoleteNodes(NiDynamicCast(NiNode, m_spKartAM->GetNIFRoot()->GetObjectByName("center")));
}

bool RemotePlay::Initialize(KPlayInfo & Info, unsigned int uiMommaID /* = 0 */, unsigned int uiPadID /* = 0 */)
{
	return Play::Initialize(Info, uiMommaID, uiPadID);
}


void RemotePlay::GetKartParam(void * pData)
{
	return;	
}

void RemotePlay::SetKartParam(void * pData, float fSwooshFactor)
{

#ifndef _XCART_
	CKart * pKart = GetKart();
#else
	XCart* pKart = GetKart();
#endif

	if ( pData == NULL ) 
		return;

	STUDPPlayMove * aData = ( STUDPPlayMove * )pData;

	//行驶状态：是否漂移
	bool isDrifting = (aData->m_cisDrifting == 1)? true: false;

	//速度：
	NxVec3 vel;
	vel.x = aData->m_unLinearVelocity[0]/SCALE_FACTOR;
	vel.y = aData->m_unLinearVelocity[1]/SCALE_FACTOR;
	vel.z = aData->m_unLinearVelocity[2]/SCALE_FACTOR;

	//位置：
	NiPoint3 vPos;
	vPos.x = aData->m_unArrPosition[0]/SCALE_FACTOR;
	vPos.y = aData->m_unArrPosition[1]/SCALE_FACTOR;
	vPos.z = aData->m_unArrPosition[2]/SCALE_FACTOR;

	m_spCharacterAM->SetTargetAnimation(aData->m_unPlayAnimID);
	m_spKartAM->SetTargetAnimation(aData->m_unKartAnimID);

	//朝向：
	NxMat33 ori;

	if ( pKart->GetVehicle() )
	{
		for ( int i = 0 ; i < 3 ; i ++ )
		{
			NxVec3 vec;
			vec.x = aData->m_unArrMat[3 * i + 0]/SCALE_FACTOR;
			vec.y = aData->m_unArrMat[3 * i + 1]/SCALE_FACTOR;
			vec.z = aData->m_unArrMat[3 * i + 2]/SCALE_FACTOR;
			ori.setColumn(i,vec);
		}
		pKart->GetVehicle()->SetOriGlobal(ori);
		pKart->GetVehicle()->SetMatGround(ori);
		pKart->GetVehicle()->SetOffsetZ(aData->m_unOffSetZ/SCALE_FACTOR);
	}
	

	// 修改人: wincoqin
	// 时间: 2007/2/5
	// 原因： 在小范围内的误差忽略，通过速度（力 ） 的方式来进行补偿位置上的差异
	CTencentCar * pCar = pKart->GetVehicle();
	if( pKart && pCar)
	{
		NiPoint3 kCurPos = pKart->GetKartTranslate();
		NiPoint3 kDir = vPos - kCurPos;
		float fVerticalDist = (vPos.z - kCurPos.z);
		float fDist = kDir.Unitize();
		NxVec3 kkDir(kDir.x, kDir.y, kDir.z);

		NxVec3 kVelGlobalNet;
		
		pCar->LocalToGlobal(vel, kVelGlobalNet);
		NxVec3 kCurVelo =pCar->GetVecGlobal();

#define MIN_SET_DISTANCE 0.25f
#define MIN_VERTICAL_DISTANCE 0.08f

#ifdef OUTPUT_REMOTEPLAYER_INFO
		fprintf(g_pFile, "%d\t %f %f\n", g_nCounter++, fDist, fVerticalDist);
#endif
		//如果距离不是很大
		if (fDist < MIN_SET_DISTANCE * 3.0f * fSwooshFactor)
		{
			NxVec3 kDir2 = NxVec3( kDir.x, kDir.y, kDir.z );
			kDir2.normalize();
			
			//NxVec3 kAccel = (kVelGlobalNet - kCurVelo) * 0.1f / pCar->GetMass();
			NxVec3 kAccel = NxVec3(kDir2.x, kDir2.y, kDir2.z) * fDist * 50.0f / pCar->GetMass();
			// 这个位置在当前位置的前面
			pCar->AddForceGlobal(kAccel,CTencentCar::EFT_ONLYONE);
			//LogText("RemotePlay fDist = %f \n",fDist);
		}


		pKart->setVelocityLocal(vel);

		// 如果距离比较大，就直接set位置吧 ：） 
		if (fDist > MIN_SET_DISTANCE * 3.0f * fSwooshFactor|| fabsf(fVerticalDist) > MIN_VERTICAL_DISTANCE * 5.0f * fSwooshFactor)
		{
			//LogText("如果距离比较大，就直接set位置吧  %f \n",fDist);
			pKart->SetKartPos(vPos);

		}
//		pKart->SetTurnAngle(aData->m_unAngle/SCALE_FACTOR);
		NxVec3 otherVec;
		pCar->LocalToGlobal(vel, otherVec);
		NxPlayMgr::GetHostPlay()->CollisionDetectionFromOtherPlayer(GetID(), pCar, otherVec, NxVec3(vPos.x, vPos.y, vPos.z), ori);

	}

	m_fRestMileage = aData->m_nRestDistance / SCALE_FACTOR;
}



void RemotePlay::UpdatePhy(float fTime)
{
	if ( !m_bIsStopPhy)
	{
		GetKart()->RemoteCarUpdate(fTime);
	}
}

NiActorManager::EventCode RemotePlay::InterpretInput(const NiPoint2& kDir, const float& fStrafeDir, float fTime)
{
	UINT anim = m_spCharacterAM->GetTargetAnimation();
	if ( anim == 0 )
		return one_Anim::STILL;
	return anim;
}

void RemotePlay::AnimActivated(NiActorManager* pkManager,
											NiActorManager::SequenceID eSequenceID, float fCurrentTime,
											float fEventTime)
{

	Play::AnimActivated(pkManager, eSequenceID, fCurrentTime, fEventTime);
/*
	if(NxSceneMgr::Get()->GetGameStatusManager()->GetGameState() == GameStatusManager::SELECT_CAR )
	{
		switch(eSequenceID)
		{
		case one_Anim::CRY:
			{				
				if(Play::m_uiActiveCharacterID == 0)
					ChangeFaceTexture( MakeString("%s/Image/FaceImage/face2.dds", Play::m_strPlayPath[m_uiActiveCharacterID]));
			}
		}

	}
*/
}
void RemotePlay::EndOfSequence(NiActorManager *pkManager, 
                   NiActorManager::SequenceID eSequenceID, float fCurrentTime, 
                   float fEventTime)
{

	Play::EndOfSequence(pkManager, eSequenceID, fCurrentTime, fEventTime);
/*		m_spCharacterAM->SetTargetAnimation( one_Anim::STILL ); 
	if(NxSceneMgr::Get()->GetGameStatusManager()->GetGameState() == GameStatusManager::SELECT_CAR )
	{
		switch(eSequenceID)
		{
	case one_Anim::CRY:
			{				
				if(m_uiActiveCharacterID == 0)
					RecoverFaceTexture();
			}
		}

	}
*/
}

void Play::InitWheelPos()
{
	//if (!IsHost())
	//	return;

	NiNodePtr tpNode=(NiNode*) GetKartNIFRoot();
	m_arWheelPos[0] = tpNode->GetObjectByName("RF_W")->GetTranslate();
	m_arWheelPos[1] = tpNode->GetObjectByName("LF_W")->GetTranslate();
	m_arWheelPos[2] = tpNode->GetObjectByName("LB_W")->GetTranslate();
	m_arWheelPos[3] = tpNode->GetObjectByName("RB_W")->GetTranslate();
}
void RemotePlay::InitWheelPos()
{
	Play::InitWheelPos();
}