#include "Top-KartPCH.h"
#include "DataCollecter.h"
#include "NxSceneMgr.h"
#include "NO2Controller.h"
#include <iomanip>


CDataCollecter::CDataCollecter():
m_bIsUpdate(false),
m_pTarget(0),
m_spKeyboard(0)
{
	Clear();
	m_Fout.open("kart.txt");
}

CDataCollecter::~CDataCollecter()
{
	m_spKeyboard = 0;
	m_Fout.close();
}


void CDataCollecter::SetTarget(Play* pPlay)
{
	m_pTarget = pPlay;
}


void CDataCollecter::SetKeyboard(NiInputKeyboardPtr spKeyboard)
{
	m_spKeyboard = spKeyboard;
}


void CDataCollecter::SetOutPutFile(char* strFilePath)
{
	m_Fout.close();
	m_Fout.clear();
	m_Fout.open(strFilePath);

}

void CDataCollecter::Start()
{
	m_bIsUpdate = true;

	m_Fout<<"------------------------------------新的数据段---------------------------------"<<endl;
	m_Fout<<"时间"<<"\t"<<"车头角"<<"\t"<<"速度"<<"\t"<<"转弯半径"<<"\t"<<"当前赛道坡度"<<"\t"<<"按下Up"<<"\t"<<"按下Left"<<"\t"
		<<"按下Right"<<"\t"<<"按下Down"<<"\t"<<"按下Shift"<<"\t"<<"处于漂移状态"<<"\t"<<"满足漂移状态"<<endl;
}



void CDataCollecter::Stop()
{
	m_bIsUpdate = false;
	m_Fout<<endl<<endl;
}

void CDataCollecter::Clear()
{
	m_bIsUp = 0;
	m_bIsLeft = 0;
	m_bIsRight = 0;
	m_bIsDown = 0;
	m_bIsShift = 0;
	m_bIsDrift = 0;
	m_bIsNo2 = 0;

	m_fTime = 0;
	m_fSpeed = 0;
	m_fKartAngle = 0;
	m_fRotRadius = 0;
	m_fGradient = 0;	
}

void CDataCollecter::Update(float fTime)
{
	if(m_bIsUpdate && m_pTarget && m_spKeyboard)
	{
		Clear();
		m_fTime = fTime;
		m_fSpeed = m_pTarget->GetKart()->GetVelocity();
		m_fKartAngle = m_pTarget->GetKart()->GetTurnAngle();
	//	m_fRotRadius = m_pTarget->GetKart();
	//	m_fGradient = 
		if(m_pTarget->GetKart()->IsDrifting())
		{
			m_bIsDrift = 1;
		}
		if(CNO2Controller::Get())
		{
			if(CNO2Controller::Get()->GetActive())
			{
				m_bIsNo2 = 1;
			}
		}

		if(m_spKeyboard->KeyIsDown(NiInputKeyboard::KEY_UP)||m_spKeyboard->KeyIsDown(NiInputKeyboard::KEY_W))
		{
			m_bIsUp = 1;
		}
		if(m_spKeyboard->KeyIsDown(NiInputKeyboard::KEY_LEFT)||m_spKeyboard->KeyIsDown(NiInputKeyboard::KEY_A))
		{
			m_bIsLeft = 1;
		}
		if(m_spKeyboard->KeyIsDown(NiInputKeyboard::KEY_RIGHT)||m_spKeyboard->KeyIsDown(NiInputKeyboard::KEY_D))
		{
			m_bIsRight = 1;
		}
		if(m_spKeyboard->KeyIsDown(NiInputKeyboard::KEY_DOWN)||m_spKeyboard->KeyIsDown(NiInputKeyboard::KEY_S))
		{
			m_bIsDown = 1;
		}
		if(m_spKeyboard->KeyIsDown(NiInputKeyboard::KEY_LSHIFT)||m_spKeyboard->KeyIsDown(NiInputKeyboard::KEY_RSHIFT))
		{
			m_bIsShift = 1;
		}

		//输出到文本
		m_Fout<<fixed<<setprecision(2)<<m_fTime<<"\t"<<m_fSpeed<<"\t"<<m_fKartAngle<<"\t"<<m_fRotRadius<<"\t""\t"<<m_fGradient<<"\t""\t"
			<<m_bIsUp<<"\t"<<m_bIsLeft<<"\t""\t"<<m_bIsRight<<"\t""\t"<<m_bIsDown<<"\t""\t"<<m_bIsShift<<"\t""\t"
			<<m_bIsDrift<<"\t""\t"<<m_bIsNo2<<endl;
	}
}


