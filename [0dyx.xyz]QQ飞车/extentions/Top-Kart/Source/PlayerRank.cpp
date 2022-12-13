#include "Top-KartPCH.h"
#include "PlayerRank.h"
#include "./Platform/GlobalInterface.h"
#include "MessageHouse.h"
#include "EventViewer.h"
#include "one_Anim.h"
#include "common.h"
#include "EventContainer.h"
#include "screendraw.h"
#include "nifop.h"
#include "./Platform/GlobalInterface.h"
#include "display.h"
extern IDisplay* g_pDisplay;

#define TURN_BACK_MIN_DIST (-15.f)

bool	CPlayerRankMgr::m_bTurnBack = false;

CRemovePlayerRankItem::CRemovePlayerRankItem(char * playNickName, NiSourceTexturePtr tIcon):
m_unPropEletNum(0)
{

	/*
	NiRendererPtr pkRenderer = NiRenderer::GetRenderer();	

	
//点头像标志

	m_fTitleWidth	=	float(tIcon->GetWidth())/GameMain::GetScreenWidth();
	m_fTitleHeight =	float(tIcon->GetHeight())/GameMain::GetScreenHeight();

	NiTexturingPropertyPtr pkTitleTexPro = NiNew NiTexturingProperty();
	NiAlphaPropertyPtr pkTitleAlphaPro = NiNew NiAlphaProperty();

	pkTitleTexPro->SetBaseTexture(tIcon);
	pkTitleTexPro->SetApplyMode(NiTexturingProperty::APPLY_REPLACE);
	pkTitleTexPro->SetBaseClampMode(NiTexturingProperty::CLAMP_S_CLAMP_T);

	pkTitleAlphaPro->SetAlphaBlending(true);
	pkTitleAlphaPro->SetSrcBlendMode(NiAlphaProperty::ALPHA_SRCALPHA);
	pkTitleAlphaPro->SetDestBlendMode(NiAlphaProperty::ALPHA_INVSRCALPHA);

	m_spTitleIcon = NiNew NiScreenElements(NiNew NiScreenElementsData(false, false, 1));
	m_spTitleIcon->Insert(4);	

	m_spTitleIcon->SetTextures(0, 0, 0, 0, 1, 1);
	m_spTitleIcon->AttachProperty(pkTitleTexPro);
	m_spTitleIcon->AttachProperty(pkTitleAlphaPro);

	m_spTitleIcon->UpdateProperties();
	m_spTitleIcon->UpdateEffects();
	m_spTitleIcon->Update(0);
	*/
	
//昵称
	m_uiIDLen = strlen(playNickName) * 8;
	m_strNickName = playNickName;


//网络连接标志
	/*
	NiTexturingPropertyPtr pkTexPro = NiNew NiTexturingProperty();
	NiAlphaPropertyPtr pkAlphaPro = NiNew NiAlphaProperty();
	static const char * strNetSituFileName[enColorNum] = 
	{
		"ScreenOverlay\\net_good.tga",
		"ScreenOverlay\\net_bad.tga",
		"ScreenOverlay\\net_unreach.tga",
	};
	for(int i = 0; i < enNetSituNum; i++)
	{
		m_aspNetSituTexs[i] = NiSourceTexture::Create(NiApplication::ConvertMediaFilename(strNetSituFileName[i]));

	}


	m_fGridWidthX = float(m_aspNetSituTexs[enGood]->GetWidth())/(GameMain::GetScreenWidth()*GRID_NUM);
	m_fGridWidthU = 1.0f/GRID_NUM;

	m_fNetIconWidth = float(m_aspNetSituTexs[enUnReach]->GetWidth())/GameMain::GetScreenWidth();
	m_fNetIconHeight = float(m_aspNetSituTexs[enUnReach]->GetHeight())/GameMain::GetScreenHeight(); 


	pkTexPro->SetBaseTexture(m_aspNetSituTexs[enUnReach]);
	pkTexPro->SetApplyMode(NiTexturingProperty::APPLY_REPLACE);
	pkTexPro->SetBaseClampMode(NiTexturingProperty::CLAMP_S_CLAMP_T);

	pkAlphaPro->SetAlphaBlending(true);
	pkAlphaPro->SetSrcBlendMode(NiAlphaProperty::ALPHA_SRCALPHA);
	pkAlphaPro->SetDestBlendMode(NiAlphaProperty::ALPHA_INVSRCALPHA);

	m_spNetIcon = NiNew NiScreenElements(NiNew NiScreenElementsData(false, false, 1));
	m_spNetIcon->Insert(4);	
	m_spNetIcon->AttachProperty(pkTexPro);
	m_spNetIcon->AttachProperty(pkAlphaPro);

	m_spNetIcon->UpdateProperties();
	m_spNetIcon->UpdateEffects();
	m_spNetIcon->Update(0);

	for(unsigned int i = 0; i < 2; ++i)
	{
		NiTexturingPropertyPtr  spTexPro	= NiNew NiTexturingProperty();
		NiAlphaPropertyPtr		spAlphaPro	= NiNew NiAlphaProperty();

		spTexPro->SetBaseTexture(NULL);
		spTexPro->SetApplyMode(NiTexturingProperty::APPLY_REPLACE);
		spTexPro->SetBaseClampMode(NiTexturingProperty::CLAMP_S_CLAMP_T);

		spAlphaPro->SetAlphaBlending(true);
		spAlphaPro->SetSrcBlendMode(NiAlphaProperty::ALPHA_SRCALPHA);
		spAlphaPro->SetDestBlendMode(NiAlphaProperty::ALPHA_INVSRCALPHA);

		m_aspPropElet[i] = NiNew NiScreenElements(NiNew NiScreenElementsData(false, false, 1));
		m_aspPropElet[i]->Insert(4);	

		m_aspPropElet[i]->SetTextures(0, 0, 0, 0, 1, 1);

		m_aspPropElet[i]->AttachProperty(spTexPro);
		m_aspPropElet[i]->AttachProperty(spAlphaPro);

		m_aspPropElet[i]->UpdateBound();
		m_aspPropElet[i]->UpdateProperties();
		m_aspPropElet[i]->UpdateEffects();
		m_aspPropElet[i]->Update(0.0f);
	}

	*/
}

CRemovePlayerRankItem::~CRemovePlayerRankItem()
{
	/*
	m_spNetIcon = 0;
	m_spTitleIcon = 0;
	for(int i =0; i < enNetSituNum; i++)
		m_aspNetSituTexs[i] = 0;
	for(int i = 0; i < 2; ++i)
	{
		m_aspPropElet[i] = 0;
	}
	*/
}

void CRemovePlayerRankItem::SetTitleIcon(NiSourceTexturePtr pTex)
{

}


void CRemovePlayerRankItem::ChangePropTex(unsigned int unEletID, NiSourceTexturePtr spTex)
{
	/*
	NiTexturingPropertyPtr spTexPro = NiDynamicCast(NiTexturingProperty, m_aspPropElet[unEletID]->GetProperty(NiProperty::TEXTURING));

	spTexPro->SetBaseTexture(spTex);

	m_aspPropElet[unEletID]->UpdateProperties();
	m_aspPropElet[unEletID]->UpdateEffects();
	m_aspPropElet[unEletID]->Update(0);	
	*/
}

void CRemovePlayerRankItem::SetPropNum(unsigned int unProp)
{
	m_unPropEletNum = unProp;
}

void CRemovePlayerRankItem::SetNetSituIcon(NiSourceTexturePtr pTex)
{
	/*
	NiTexturingPropertyPtr pTexPro = NiDynamicCast(NiTexturingProperty, m_spNetIcon->GetProperty(NiProperty::TEXTURING));
	pTexPro->SetBaseTexture(pTex);
	m_spNetIcon->UpdateProperties();
	m_spNetIcon->UpdateEffects();
	m_spNetIcon->Update(0);	
	*/
}

void CRemovePlayerRankItem::MoveTo(unsigned int x, unsigned int y)
{
	/*
	m_fNetIconX = float(x + m_uiIDLen + 20) / GameMain::GetScreenWidth() + m_fTitleWidth;
	m_fNetIconY = float(y) / GameMain::GetScreenHeight()+ 0.0075;

	m_spTitleIcon->SetRectangle(0, (float)x/ GameMain::GetScreenWidth(),
		m_fNetIconY - 0.009, m_fTitleWidth, m_fTitleHeight );
	m_spTitleIcon->UpdateBound();
	
	m_unNickNameX = x + m_fTitleWidth*GameMain::GetScreenWidth() + 10;
	m_unNickNameY = y + 5;
	
	m_spNetIcon->SetVertex(0, 0, NiPoint2(m_fNetIconX, m_fNetIconY));
	m_spNetIcon->SetTexture(0, 0, 0, NiPoint2(0, 0));

	m_spNetIcon->SetVertex(0, 1, NiPoint2(m_fNetIconX, m_fNetIconY + m_fNetIconHeight));
	m_spNetIcon->SetTexture(0, 1, 0, NiPoint2(0, 1));
	*/
}

void CRemovePlayerRankItem::UpdateNetSitu(unsigned int uiTime)
{
	/*
	int iSteps;
	if( uiTime < NORMAL_NETWORK_SPEED)
	{
		SetNetSituIcon(m_aspNetSituTexs[enGood]);

		iSteps = (uiTime < IDEAL_NETWORK_SPEED) ? GRID_NUM 
				: (NORMAL_NETWORK_SPEED - uiTime)/NETSPEED_PER_GRID + 1;
	
	}
	else if(uiTime >= NORMAL_NETWORK_SPEED && uiTime < NETWORK_UNREACH)
	{
		SetNetSituIcon(m_aspNetSituTexs[enBad]);
		if(uiTime < HARD_NETWOEK_SPEED)
			iSteps = (uiTime - NORMAL_NETWORK_SPEED)/NETSPEED_PER_GRID + 1;
		else
			iSteps = GRID_NUM;
	}
	else 
	{
		SetNetSituIcon(m_aspNetSituTexs[enUnReach]);
		iSteps = GRID_NUM;
	}

	float fNetIconWidth = iSteps * m_fGridWidthX;

	m_spNetIcon->SetVertex(0, 2, NiPoint2(m_fNetIconX + iSteps * m_fGridWidthX, m_fNetIconY + m_fNetIconHeight));
	m_spNetIcon->SetTexture(0, 2, 0, NiPoint2(iSteps * m_fGridWidthU, 1.0));

	m_spNetIcon->SetVertex(0, 3, NiPoint2(m_fNetIconX + iSteps * m_fGridWidthX, m_fNetIconY));
	m_spNetIcon->SetTexture(0, 3, 0, NiPoint2(iSteps * m_fGridWidthU, 0));
	m_spNetIcon->UpdateBound();

	m_aspPropElet[0]->SetRectangle(0, m_fNetIconX + fNetIconWidth + 0.01f, m_fNetIconY - 0.003f, 0.03f, 0.03f);
	m_aspPropElet[1]->SetRectangle(0, m_fNetIconX + fNetIconWidth + 0.046f, m_fNetIconY- 0.003f, 0.03f, 0.03f);

	m_aspPropElet[0]->UpdateBound();
	m_aspPropElet[1]->UpdateBound();
	*/
}




void CRemovePlayerRankItem::DrawItem(NiRendererPtr pkRenderer )
{
//	GlobalInterface::m_pDisplay->ClearScreen(0);
	
//	GlobalInterface::m_pDisplay->UpdateScreen();

	/*
	m_spTitleIcon->Draw(pkRenderer);
	m_spNetIcon->Draw(pkRenderer);
	for(int i = 0; i < m_unPropEletNum; ++i)
	{
		m_aspPropElet[i]->Draw(pkRenderer);
	}
	*/
}

void CRemovePlayerRankItem::DrawUI()
{
//	GlobalInterface::m_pDisplay->DrawText(m_unNickNameX, m_unNickNameY, m_strNickName.c_str(), 0xffffffff, 16);
}

CHostRankItem::CHostRankItem()
{
	//NxPlayMgr::GetHostPlay()->SetVisible2DPortrait(true);
	
}

CHostRankItem::~CHostRankItem()
{
	/*
	if ( NxPlayMgr::GetHostPlay() )
		NxPlayMgr::GetHostPlay()->SetVisible2DPortrait(false);
		*/
}

void CHostRankItem::MoveTo(unsigned int x, unsigned int y)
{
	m_ox = x;
	m_oy = y;
}



void CHostRankItem::DrawItem(NiRendererPtr pkRenderer)
{
//	DWORD dwScreenObject = NxPlayMgr::GetHostPlay()->m_dwScreenObjectGame;
//	g_pDisplay->ClearScreen(0);
//	CScreenObjectsMgr::Get()->DrawScreenObject(dwScreenObject, m_ox, m_oy, 150, 160, 0.5);
//	g_pDisplay->UpdateScreen();
}

void CHostRankItem::DrawUI()
{
	/*
	NiNode* pkNodeKart = NxPlayMgr::GetHostPlay()->GetKartNIFRoot();
	NiNode* pkNode = NxPlayMgr::GetHostPlay()->GetCharacterNIFRoot();
	CScreenDraw::Draw(pkNode, CNifOp::FindCameraByName(pkNodeKart, "head camera"), m_ox, m_oy, 80, 60);
	*/
}



CPlayerRankMgr::CPlayerRankMgr():
m_iCurNumOfPlayers(0),
m_uiCurHeight(0),
m_ushHostCurRank(0),
m_nPreviousInterval(0),
m_bIsTeamRace(false),
m_fCompensateDis(0.0f),
m_fCompensateValue(1.0f),
m_nFinishPlayerNum(0)
{
	for(int i = 0; i < MAX_PLAYER_NUM; i++)
	{
		m_apPRankItems[i] = 0;
	}
}

CPlayerRankMgr::~CPlayerRankMgr()
{
	for(int i = 0; i < enColorNum; i++)
		m_aspColorTexs[i] = 0;

	for(int i = 0; i < m_iCurNumOfPlayers; i++)
	{
		SAFE_NIDELETE(m_apPRankItems[i]);
	}
	UIntKeyToTextureMap::iterator map_it;	//依次施放tex
	for( map_it = m_mapAllGPTex.begin(); map_it != m_mapAllGPTex.end(); map_it++)
	{
		map_it->second = 0;
	}
}

void CPlayerRankMgr::Init(unsigned int* unPlayerUin, int curNumPlayer)
{
	m_bTurnBack = false;
//	m_bIsTeamRace = false;
	m_fCompensateValue = 1.0f;
	m_fCompensateDis   = 0.0f;

	/*
	static const char * strColorFileName[enColorNum] = 
	{
		"ScreenOverlay\\title_red.tga",
		"ScreenOverlay\\title_blue.tga",
		"ScreenOverlay\\title_green.tga",

	};
	for(int i = 0; i < enColorNum; i++)
	{
		m_aspColorTexs[i] = NiSourceTexture::Create(NiApplication::ConvertMediaFilename(strColorFileName[i]));
	}
	*/
	m_iCurNumOfPlayers = curNumPlayer;


	CPlayerRankInfo info;

	int j = 0;
	for(int i = 0; i < m_iCurNumOfPlayers; i++)
	{
		for(; j < MAX_PLAYER_NUM; j++)
		{	
			if(unPlayerUin[j] > 0)	
			{
				info.m_unUin = unPlayerUin[j];
				j++;
				break;
			}

		}

		Play* pPlay = NxPlayMgr::GetPlay(info.m_unUin);
		info.m_iIndex = i;		
		info.m_enColor = enBlue;
		info.m_nColor = pPlay->FindPaintColor()->hue;
		info.m_fRestMiles = 99999999.0f;	//初始化剩余路程为最大值
		info.m_bIsFinish = false;
		info.m_unTeamID = pPlay->GetTeam();
		info.m_dwGPs[0] = 0;
		info.m_dwGPs[1] = 0;

		//给名字赋值
		strcpy(info.m_cNickName, NxPlayMgr::GetPlay(info.m_unUin)->GetPlayName());


		//提供得到本地玩家当前排名
		if(info.m_unUin == NxPlayMgr::GetHostPlay()->GetPlayUin())
		{
			m_ushHostCurRank = i + 1; 
			m_apPRankItems[i] = NiNew CHostRankItem();
			info.m_uiItemHeight = 95;
		}
		else
		{
			info.m_uiItemHeight = 33;
			m_apPRankItems[i] = NiNew CRemovePlayerRankItem(info.m_cNickName, 
				m_aspColorTexs[info.m_enColor]);
		}

		m_vPlayerRank.push_back(info);
	}

	m_nPreviousInterval = 0;		//与前一玩家距离初始化为零

	/*
	CVarTable vtImageCfg("GPImageConfig");
	CVar::LoadVarsFromFile( &vtImageCfg, "config/gp_image_cfg.lua");

	NiFixedString strImagePath			= vtImageCfg.Element("ImagePath")->String();
	CVarArray* arrImage	= (CVarArray*) vtImageCfg.Element("gpImageMap");

	NiSourceTexturePtr spTex;
	CStringEx strTmp;

	int nNumOfGPImage = arrImage->GetNum();
	for(int i = 0; i < nNumOfGPImage; ++i)		//载入各道具图像
	{
		spTex = NiSourceTexture::Create( NiApplication::ConvertMediaFilename( strTmp.FormatPt("%s/%s",
			strImagePath, arrImage->Element(i)->Element("name")->String() ) ) );
		if(spTex)
		{
			m_mapAllGPTex.insert( make_pair( arrImage->Element(i)->Element("prop_id")->Integer(), spTex ) );			
		}
	}
	*/
}


CPlayerRankMgr::enTurnBackEvent CPlayerRankMgr::GetTriggerTurnBackEvent(float fDist, unsigned int unRemote)
{
	if (fDist >= TURN_BACK_MIN_DIST && fDist < 0.0f) 
	{
		Play * pLocal = NxPlayMgr::GetHostPlay();
		Play * pRemote = NxPlayMgr::GetPlay(unRemote);

		if (pLocal && pRemote && pLocal->GetKartNIFRoot() && pRemote->GetKartNIFRoot()) 
		{
			NiTransform kLocalPose  = pLocal->GetKartNIFRoot()->GetWorldTransform();
			NiTransform kRemotePose = pRemote->GetKartNIFRoot()->GetWorldTransform();

			NiPoint3 kDistance = kLocalPose.m_Translate - kRemotePose.m_Translate;
			NiPoint3 kLocalRight;
			NiMatrix3 kRotate = kLocalPose.m_Rotate;

			NiMatrix3 mxY90;
			mxY90.MakeZRotation(-NI_PI / 2.f);
			mxY90 = mxY90.Inverse();
			kRotate = kRotate * mxY90;

			kRotate.GetCol(2, kLocalRight);


			if (kLocalRight.Dot(kDistance) < 0) 
			{
				return enLeft;
			}
			else
			{
				return enRight;
			}
		}		
	}

	return enNone;
}

void CPlayerRankMgr::TriggerTurnBackEvent(bool bLeft, unsigned int unRemote)
{
	LogText("**************Start BackEvent\n");

	Play * pSelf = NxPlayMgr::GetHostPlay();
	if (pSelf)
	{
		// Do Anim
		if (bLeft)
		{
			pSelf->SetCharaterAnimation("TurnLeft");
		}
		else
		{
			pSelf->SetCharaterAnimation("TurnRight");
		}
	}
	
	Play * p = NxPlayMgr::GetPlay(unRemote);
	if (p)
	{		
		NiCamera * pkCamera = p->GetCamera("front camera");
		assert(pkCamera);		
		EventViewerMgr::StartBackEvent(pkCamera);
	}
	m_bTurnBack = true;
}

void CPlayerRankMgr::StopTurnBackEvent()
{
	LogText("**************Try to Stop BackEvent\n");
	EventViewerMgr::CloseBackEvent();
	if(NxPlayMgr::GetHostPlay())
		NxPlayMgr::GetHostPlay()->PushPlayerAnimEvent(CAnimEventMgr::EAET_IDLE);

	m_bTurnBack = false;
}

void CPlayerRankMgr::UpdateCompensate()
{

	float fRaceDisTance =CLevelManager::Get()->GetLinks()->GetLapTotalLen()  * 
		CLevelManager::Get()->GetLinks()->GetTotalLaps();
	m_fCompensateValue = m_fCompensateDis / fRaceDisTance; 

	Play * pPlay = NxPlayMgr::GetHostPlay();
	if( pPlay )
	{
		CTencentCar * pCar = pPlay->GetKart()->GetVehicle();
		if ( pCar )
			pCar->ProcessCompensateValue(m_fCompensateValue);
	}
	;
}

void CPlayerRankMgr::Update(float fTime)
{
	int iSpeed = 400;

	for(int i = 0; i < m_iCurNumOfPlayers; i++)
	{
		if(!m_vPlayerRank[i].m_bIsFinish)
		{
			Play * pPlayer = NxPlayMgr::GetPlay(m_vPlayerRank[i].m_unUin);
			if (pPlayer)
			{
				m_vPlayerRank[i].m_fRestMiles = pPlayer->GetRestMileseage();	
			}
			else
			{
				m_vPlayerRank[i].m_fRestMiles = 99999999.0f;
			}
		}
		
	}


	sort(m_vPlayerRank.begin(), m_vPlayerRank.end());

	m_uiCurHeight = 150;
	

	m_fCompensateDis = 0.0f;

	for(int i = 0; i < m_iCurNumOfPlayers; i++)
	{
		m_apPRankItems[ m_vPlayerRank[i].m_iIndex ]->MoveTo(20, m_uiCurHeight);
		m_uiCurHeight +=m_vPlayerRank[i].m_uiItemHeight;

		if (m_apPRankItems[m_vPlayerRank[i].m_iIndex]->IsHost()) 
		{
			//本地玩家与前一玩家的距离
			if( i > 0)
			{
				m_nPreviousInterval = m_vPlayerRank[i].m_fRestMiles - m_vPlayerRank[i - 1].m_fRestMiles;
				m_fCompensateDis   = m_vPlayerRank[i].m_fRestMiles - m_vPlayerRank[0].m_fRestMiles;
			}
			else
			{
				m_nPreviousInterval = 0;
			}
		

			//本地玩家当前排名
			m_ushHostCurRank = i + 1;


			if (i < m_iCurNumOfPlayers - 1)
			{
				enTurnBackEvent enBackEvent = GetTriggerTurnBackEvent(m_vPlayerRank[i].m_fRestMiles - m_vPlayerRank[i + 1].m_fRestMiles, 
					m_vPlayerRank[i + 1].m_unUin);

				if (enNone != enBackEvent)
				{
					if (!m_bTurnBack)
						TriggerTurnBackEvent(enBackEvent == enLeft, m_vPlayerRank[i + 1].m_unUin);
				}
				else
				{
					if (m_bTurnBack)
						StopTurnBackEvent();
				}
			}
			else if (m_bTurnBack)
			{
				StopTurnBackEvent();
			}			
		}
	}

	for(int i = 0; i < m_iCurNumOfPlayers; i++)
	{	
		if(!m_apPRankItems[m_vPlayerRank[i].m_iIndex]->IsHost())
		{
			HRESULT hr = GlobalInterface::m_pQQTppp->GetP2pNetSpeed(m_vPlayerRank[i].m_unUin, &iSpeed);
			if(hr == E_FAIL )
			{
				iSpeed = HARD_NETWOEK_SPEED;
			}
			hr = GlobalInterface::m_pQQTppp->CanPass(m_vPlayerRank[i].m_unUin);
			if(hr == S_FALSE)
			{
				iSpeed = NETWORK_UNREACH;
			}
			m_vPlayerRank[i].m_nNetSpeed = iSpeed;
			m_apPRankItems[m_vPlayerRank[i].m_iIndex]->UpdateNetSitu(iSpeed);

			if(m_bIsTeamRace)
			{
				if(m_vPlayerRank[i].m_unTeamID == NxPlayMgr::GetHostPlay()->GetTeam())
				{
					if(!NxPlayMgr::GetPlay(m_vPlayerRank[i].m_unUin))
					{
						continue; 
					}
					GP_ID_LIST& listGPID = NxPlayMgr::GetPlay(m_vPlayerRank[i].m_unUin)->GetGPs();
					GP_ID_LIST_Iterator ID_It = listGPID.begin();

					int propIndex = 0;
					m_vPlayerRank[i].m_dwGPs[0] = 0;
					m_vPlayerRank[i].m_dwGPs[1] = 0;
					for(; ID_It != listGPID.end(); ++ID_It)
					{			
						m_vPlayerRank[i].m_dwGPs[propIndex] = (*ID_It).dwID;
						m_apPRankItems[m_vPlayerRank[i].m_iIndex]->ChangePropTex(propIndex, m_mapAllGPTex[(*ID_It).dwID]);
						++propIndex;
						if(propIndex > 1)
						{
							break;
						}
					}
					m_apPRankItems[m_vPlayerRank[i].m_iIndex]->SetPropNum(propIndex);
					
				}
			}
		}
	}
	UpdateCompensate();

	//提供数据给UI使用
	m_ri.nPlayNum = m_iCurNumOfPlayers;
	for ( int index = 0; index < m_iCurNumOfPlayers; index++ )
	{
		PLAY_RANKINFO& ri = m_ri.ri[index];
		CPlayerRankInfo& kRankInfo = m_vPlayerRank[index];
		ri.Uin = kRankInfo.m_unUin;
		ri.nColor = kRankInfo.m_nColor;
		ri.bFinish = kRankInfo.m_bIsFinish;
		ri.nNetSpeed = kRankInfo.m_nNetSpeed;
		ri.nTeamID = kRankInfo.m_unTeamID;
		ri.dwGPs[0] = kRankInfo.m_dwGPs[0];
		ri.dwGPs[1] = kRankInfo.m_dwGPs[1];
		ZeroMemory(ri.szNickName, MAXNICKNAMELEN + 1);
		strncpy(ri.szNickName, kRankInfo.m_cNickName, MAXNICKNAMELEN);
	}
}

void CPlayerRankMgr::Draw(NiRendererPtr spRenderer)
{
	
	/*
	for(int i = 0; i < m_iCurNumOfPlayers; i++)
		m_apPRankItems[i]->DrawItem(spRenderer);

	for(int i = 0; i < m_iCurNumOfPlayers; i++)
	{
		if ( m_apPRankItems[i]->IsHost() )
			m_apPRankItems[i]->DrawUI();
	}
	GlobalInterface::m_pDisplay->ClearScreen(0);
	for(int i = 0; i < m_iCurNumOfPlayers; i++)
	{
		if ( m_apPRankItems[i]->IsHost() )
			continue;
		m_apPRankItems[i]->DrawUI();
	}
	GlobalInterface::m_pDisplay->UpdateScreen();
	*/
}


unsigned short CPlayerRankMgr::GetHostCurRank()			//得到本地玩家当前排名
{
	return m_ushHostCurRank;
}

unsigned short CPlayerRankMgr::GetPlayerRank(unsigned int unUin)	//得到玩家当前排名
{
	for(int i = 0; i < m_iCurNumOfPlayers; i++)
	{
		if(m_vPlayerRank[i].m_unUin == unUin)
		{
			return i;
		}
	}
	return 0;
}


int CPlayerRankMgr::GetPreviousInterval()		//得到和前一玩家的距离
{
	return m_nPreviousInterval;
}

void CPlayerRankMgr::SetRaceType(bool bIsTeamRace)
{
	m_bIsTeamRace = bIsTeamRace;
}

void CPlayerRankMgr::SetPlayerFinish(DWORD dwUin)
{
	for(int i = 0; i < m_iCurNumOfPlayers; i++)
	{
		if ( dwUin == 0 && m_vPlayerRank[i].m_bisHost )
		{
			m_vPlayerRank[i].m_bIsFinish = true;
		}
		else if(m_vPlayerRank[i].m_unUin == dwUin && !m_vPlayerRank[i].m_bIsFinish)
		{
			++m_nFinishPlayerNum;
			m_vPlayerRank[i].m_fRestMiles = MinRestmiles + m_nFinishPlayerNum;	//固定玩家的剩余路程
			m_vPlayerRank[i].m_bIsFinish = true;			
		}
	}

}