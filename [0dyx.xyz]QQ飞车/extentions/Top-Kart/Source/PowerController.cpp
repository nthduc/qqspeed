#include "Top-KartPCH.h"
#include "PowerController.h"
#include "BlinkItem.h"
#include "BarItem.h"
#include "variant.h"
#include "GPNetInterCommn.h"


CSinglePower::CSinglePower():
m_unCurPower(0),
m_unPrePower(0),
m_unMaxPower(0),
m_unCompensatePower(0),
m_unAnimPower(0),
m_bIsAnim(false),
m_bIsCrash(false),
m_fKartCoef(80)
{
	m_pBarItem = NiNew CBarItem;
	m_pBlinkItem = NiNew CBlinkItem;
}

CSinglePower::~CSinglePower()
{
	SAFE_DELETE(m_pBarItem);
	SAFE_DELETE(m_pBlinkItem);
}

void CSinglePower::AccumulateSucs()
{
	if(m_unCurPower - m_unPrePower > 0)
	{
		CGPNetIntercommn::RequestDrift(m_unCurPower - m_unPrePower);
	}

	m_unPrePower = m_unCurPower;

	if(m_unCurPower >= m_unMaxPower)
	{
		m_unCurPower = 0;
		m_unPrePower = 0;
		m_pBlinkItem->StartAnim();
		m_bIsAnim = true;
	}
}

void CSinglePower::AccumulateFail()
{
	m_unCurPower = m_unPrePower;

	m_bIsCrash = true;
	/*m_fPreSpeed = NxPlayMgr::GetHostPlay()->GetKart()->GetVelocity();
	m_fCurSpeed = m_fPreSpeed;*/

	m_pBarItem->SetCurUnits(m_unCurPower);
}

void CSinglePower::AccumulatePower(unsigned int unPower)
{
	if (unPower)
	{
		m_fPreSpeed = m_fCurSpeed;
	}

	m_unCurPower += unPower;

	if(m_unAnimPower >= m_unMaxPower)		//此处加判断用于播放满气后进度条递减的动画
	{
		m_pBarItem->SetCurUnits(m_unCurPower);
	}
	
}


void CSinglePower::SetCurPower(unsigned int unPower)
{
	m_unCurPower = unPower;

	if(m_unAnimPower >= m_unMaxPower)		//此处加判断用于播放满气后进度条递减的动画
	{
		m_pBarItem->SetCurUnits(m_unCurPower);
	}
	
}

void CSinglePower::Reset()
{
	m_unCurPower = 0;
	m_unPrePower = 0;
	m_pBarItem->SetCurUnits(0);
}

void CSinglePower::SetKartCoef(float fCoef)
{
	m_fKartCoef = fCoef;
}

void CSinglePower::Update(float fTime)
{
	if(NxPlayMgr::GetHostPlay()->GetKart()->IsAccumulate() && !m_bIsCrash)	//此处加m_bIsCrash，因为当碰撞发生时，IsAccumulate()
	{																		//在当前帧仍然为true，在下一帧才为false
		m_fCurSpeed = NxPlayMgr::GetHostPlay()->GetKart()->GetVelocity();
		if(m_fCurSpeed >= m_fPreSpeed)
		{
			AccumulatePower(m_unCompensatePower);
			
		}
		else
		{
			AccumulatePower((m_fPreSpeed - m_fCurSpeed) * m_fKartCoef);
		}		
	}
	else
	{
		m_fPreSpeed = NxPlayMgr::GetHostPlay()->GetKart()->GetVelocity();
		m_bIsCrash = false;
		AccumulateSucs();		
	}

	if(m_bIsAnim)
	{	
		if(m_unAnimPower > 50)
		{
			m_unAnimPower -= 50;
			m_pBarItem->SetCurUnits(m_unAnimPower);
		}
		else
		{
			m_unAnimPower = m_unMaxPower;
			m_bIsAnim = false;
			m_pBarItem->SetCurUnits(0);
		}	
	}
	
}
 

void CSinglePower::Draw(NiRendererPtr spRenderer)
{
	m_pBlinkItem->Draw(spRenderer);
	m_pBarItem->Draw(spRenderer);
	
}


void CSinglePower::SetActivity(bool bActive)
{
	Reset();
	CRenderBase2D::SetActivity(bActive);
}

void CSinglePower::LoadConfig()
{
	CVarTable vtSingleBar("Single_bar");
	CVar::LoadVarsFromFile(& vtSingleBar, "Config/power_bar.lua");

	CVarArray* varrTex = (CVarArray*) vtSingleBar.Element("bar_path");
	NiSourceTexturePtr spTex = 0;
	unsigned int unTexNum = varrTex->GetNum();
	for(int i = 0; i < unTexNum; ++i)
	{
		spTex = NiSourceTexture::Create(NiApplication::ConvertMediaFilename(varrTex->Element(i)->String()));
		m_pBarItem->SetBarTex(i, spTex);
	}

	varrTex = (CVarArray*) vtSingleBar.Element("blink_path");
	unTexNum = varrTex->GetNum();
	for(int i = 0; i < unTexNum; ++i)
	{
		spTex = NiSourceTexture::Create(NiApplication::ConvertMediaFilename(varrTex->Element(i)->String()));
		m_pBlinkItem->SetTex(i, spTex);
	}


	m_pBarItem->SetWidth(vtSingleBar.Element("Width")->Float());
	m_pBarItem->SetHeight(vtSingleBar.Element("Height")->Float());	
	m_pBarItem->SetBarStartLen(vtSingleBar.Element("BarStartLen")->Float());
	m_pBarItem->SetBarLenPerUnit(vtSingleBar.Element("BarLenPerUnit")->Float());
	m_pBarItem->SetFullUnits(vtSingleBar.Element("MaxUnits")->Integer());
	m_pBarItem->MoveTo(vtSingleBar.Element("Left")->Float(), vtSingleBar.Element("Top")->Float());
	
	
	m_pBlinkItem->SetWidth(vtSingleBar.Element("Width")->Float());
	m_pBlinkItem->SetHeight(vtSingleBar.Element("Height")->Float());
	m_pBlinkItem->MoveTo(vtSingleBar.Element("Left")->Float(), vtSingleBar.Element("Top")->Float());
	
	m_unMaxPower = vtSingleBar.Element("MaxUnits")->Integer();
	m_unCompensatePower = vtSingleBar.Element("CompensatePower")->Integer();
	m_unAnimPower = m_unMaxPower;
}					


CTeamPower::CTeamPower():
m_unCurPower(0),
m_unAnimPower(0),
m_bIsAnim(false)
{
	m_pBarItem = NiNew CBarItem;
	m_pBlinkItem = NiNew CBlinkItem;
}

CTeamPower::~CTeamPower()
{
	SAFE_DELETE(m_pBarItem);
	SAFE_DELETE(m_pBlinkItem);
}

void CTeamPower::Update(float fTime)
{
	if(m_bIsAnim)
	{
		if(m_unAnimPower > 50)
		{
			m_unAnimPower -= 50;
			m_pBarItem->SetCurUnits(m_unAnimPower);
		}
		else
		{
			m_unAnimPower = m_unMaxPower;
			m_bIsAnim = false;
			m_pBarItem->SetCurUnits(0);
		}
	}
}

void CTeamPower::Reset()
{
	m_unCurPower = 0;
	m_pBarItem->SetCurUnits(0);
}


void CTeamPower::SetActivity(bool bActive)
{
	Reset();
	CRenderBase2D::SetActivity(bActive);
}


void CTeamPower::AccumulatePower(unsigned int unPower)
{
	m_unCurPower += unPower;
	if(m_unCurPower >= m_unMaxPower)
	{
		m_pBlinkItem->StartAnim();
		m_bIsAnim = true;
		m_unCurPower = 0;
		m_pBarItem->SetCurUnits(m_unMaxPower);
	}
	else
	{
		m_pBarItem->SetCurUnits(m_unCurPower);
	}
	
}

void CTeamPower::SetPower(unsigned int unPower)
{
	m_unCurPower = unPower;

	if(m_unCurPower >= m_unMaxPower - 1)
	{
		m_pBlinkItem->StartAnim();
		m_bIsAnim = true;
		m_unCurPower = 0;
		m_pBarItem->SetCurUnits(m_unMaxPower);
	}
	else
	{
		m_pBarItem->SetCurUnits(m_unCurPower);
	}
}

void CTeamPower::Draw(NiRendererPtr spRenderer)
{
	m_pBlinkItem->Draw(spRenderer);
	m_pBarItem->Draw(spRenderer);
	
}


void CTeamPower::LoadConfig()
{
	CVarTable vtTeamBar("Team_bar");
 	CVar::LoadVarsFromFile(& vtTeamBar, "Config/power_bar.lua");

	CVarArray* varrTex = (CVarArray*) vtTeamBar.Element("bar_path");
	NiSourceTexturePtr spTex = 0;
	unsigned int unTexNum = varrTex->GetNum();
	for(int i = 0; i < unTexNum; ++i)
	{
		spTex = NiSourceTexture::Create(NiApplication::ConvertMediaFilename(varrTex->Element(i)->String()));
		m_pBarItem->SetBarTex(i, spTex);
	}

	varrTex = (CVarArray*) vtTeamBar.Element("blink_path");
	unTexNum = varrTex->GetNum();
	for(int i = 0; i < unTexNum; ++i)
	{
		spTex = NiSourceTexture::Create(NiApplication::ConvertMediaFilename(varrTex->Element(i)->String()));
		m_pBlinkItem->SetTex(i, spTex);
	}
	
	m_pBarItem->SetWidth(vtTeamBar.Element("Width")->Float());
	m_pBarItem->SetHeight(vtTeamBar.Element("Height")->Float());	
	m_pBarItem->SetBarStartLen(vtTeamBar.Element("BarStartLen")->Float());
	m_pBarItem->SetBarLenPerUnit(vtTeamBar.Element("BarLenPerUnit")->Float());
	m_pBarItem->SetFullUnits(vtTeamBar.Element("MaxUnits")->Integer());
	m_pBarItem->MoveTo(vtTeamBar.Element("Left")->Float(), vtTeamBar.Element("Top")->Float());
	
	
	m_pBlinkItem->SetWidth(vtTeamBar.Element("Width")->Float());
	m_pBlinkItem->SetHeight(vtTeamBar.Element("Height")->Float());	
	m_pBlinkItem->MoveTo(vtTeamBar.Element("Left")->Float(), vtTeamBar.Element("Top")->Float());
	

	m_unMaxPower = vtTeamBar.Element("MaxUnits")->Integer();
}



CPowerController::CPowerController()
{
	m_pSinglePower = NiNew CSinglePower;
	
	if(m_pSinglePower)
	{
		m_pSinglePower->LoadConfig();
	}

	m_pTeamPower = NiNew  CTeamPower;

	if(m_pTeamPower)
	{
		m_pTeamPower->LoadConfig();
	}

	
}

CPowerController::~CPowerController()
{
	SAFE_DELETE(m_pSinglePower);
	SAFE_DELETE(m_pTeamPower);
}

void CPowerController::StartSgPower()
{
	m_pSinglePower->SetActivity(true);
}

void CPowerController::StartTeamPower()
{
	m_pTeamPower->SetActivity(true);
}

void CPowerController::TerminateSgPower()
{
	m_pSinglePower->SetActivity(false);
}


void CPowerController::TerminateTeamPower()
{
	m_pTeamPower->SetActivity(false);
}

void CPowerController::StopAllPowerCtrl()
{
	TerminateSgPower();
	TerminateTeamPower();
}

CSinglePower* CPowerController::GetSinglePower()
{
	return m_pSinglePower;
}

CTeamPower* CPowerController::GetTeamPower()
{
	return m_pTeamPower;
}
