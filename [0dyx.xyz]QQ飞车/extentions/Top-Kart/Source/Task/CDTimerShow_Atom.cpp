#include "Top-KartPCH.h"
#include "CDTimerShow_Atom.h"


CCDTimerShow_Atom::CCDTimerShow_Atom():
m_spTime(0),
m_dwEndTime(0)
{

}

CCDTimerShow_Atom::~CCDTimerShow_Atom()
{
	m_spTime = 0;
	SAFE_DELETE(m_pTimerBase);
}

void CCDTimerShow_Atom::SetParam(DWORD dwParam)
{
	CAtomBase::SetParam(dwParam);
	NiFontPtr spFont = NiFont::Create(NiRenderer::GetRenderer(),
		NiApplication::ConvertMediaFilename("ScreenOverLay/HUD.nff"));
	m_spTime = NiNew NiString2D(
		spFont,
		NiFontString::COLORED,
		32, 
		"", 
		NiColorA(1.0f, 1.0f, 1.0f, 1.0f),
		430, 
		60 );
	m_spTime->sprintf("%d : 00", m_dwParam/1000);
	
}

void CCDTimerShow_Atom::Start()
{
	CAtomBase::Start();
	m_pTimerBase = CTScreenTexMgr::Get()->CreateScreenTex(m_dwOption);
	m_pTimerBase->SetActivity(true);

	m_dwEndTime = GetTickCount() + m_dwParam;	
	SetActivity(true);
	
}

void CCDTimerShow_Atom::_Stop()
{
	SetActivity(false);
	m_pTimerBase->SetActivity(false);
}

void CCDTimerShow_Atom::Update(float fTime)
{
	if(m_dwEndTime)
	{
		m_dwOption = m_dwEndTime - GetTickCount();
		if(m_dwOption > 0)
		{
			DWORD dwSec = m_dwOption/1000;
			DWORD dwMinSec = (m_dwOption%1000)/10;
			if(dwSec < 10)
			{
				m_spTime->SetColor(NiColorA(1.0f, 0.0f, 0.0f, 1.0f));
				if (dwMinSec < 10)
				{
					m_spTime->sprintf("0%d : 0%d", dwSec, dwMinSec);
				}
				else
				{
					m_spTime->sprintf("0%d : %d", dwSec, dwMinSec);
				}
			}
			else
			{
				if (dwMinSec < 10)
				{
					m_spTime->sprintf("%d : 0%d", dwSec, dwMinSec);
				}
				else
				{
					m_spTime->sprintf("%d : %d", dwSec, dwMinSec);
				}
			}
		}
		else
		{
			m_spTime->sprintf("00 : 00");
		}
	}

}

void CCDTimerShow_Atom::Draw(NiRendererPtr spRenderer)
{
	m_spTime->Draw(spRenderer);
}