#include "Top-KartPCH.h"

#include ".\Suspension.h"
#include "DriftCenter.h"


Suspension::Suspension(CTencentCar* pCar)
{
	for (int i = 0; i < 4; i++)
		m_afWheelOffset[i] = 0.0f;

	InitStims(pCar);	
}

Suspension::~Suspension()
{
	
}

void Suspension::InitStims(CTencentCar* pCar)
{
	m_pDriftStim = new DriftStimulator(pCar);
}

void Suspension::AddStimulator(IStimulator* pStim)
{
	
}

void Suspension::Update(float dt, const CTencentCar::KEYINFO& keyinfo)
{
	m_pDriftStim->Update(dt, keyinfo);


	UpdateWheelOffset(dt);
}

void Suspension::UpdateWheelOffset(float dt)
{
	static float fWidthOfCar = 0.2f;

	static float fUpMax = 0.015f;
	static float fDownMax = -0.03f;

	float fAngle = GetRotX();

	m_afWheelOffset[0] = tan(fAngle)* fWidthOfCar;

	if (m_afWheelOffset[0]> fUpMax)
		m_afWheelOffset[0] = fUpMax;
	else if (m_afWheelOffset[0] < fDownMax)
		m_afWheelOffset[0] = fDownMax;
	m_afWheelOffset[3] = m_afWheelOffset[0];
	
	m_afWheelOffset[1] = -tan(fAngle)* fWidthOfCar;

	if (m_afWheelOffset[1]> fUpMax)
		m_afWheelOffset[1] = fUpMax;
	else if (m_afWheelOffset[1] < fDownMax)
		m_afWheelOffset[1] = fDownMax;
	m_afWheelOffset[2] = m_afWheelOffset[1];
}

float Suspension::GetRotX()
{
	return m_pDriftStim->GetRotAngleX();
}

float Suspension::GetRotY()
{
	return m_pDriftStim->GetRotAngleY();
}





//-------------DriftStimulator----------------
DriftStimulator::DriftStimulator(CTencentCar* pCar):
MAXROT_X(0.21799),
MAXROT_Y(0.21799),
m_bDriftPre(false),
m_bDriftCur(false)
{
	m_pTCar = pCar;
	m_fVelUp = 0.0f;
	m_fVelDown = 0.0f;
	m_fVelPost = 0.0f;
	m_fRotX = 0.0f;
	m_fRotY = 0.0f;
	m_state = DS_NONE;
	m_fVelUp = NxMath::degToRad(30.0f);
	m_fVelDown = NxMath::degToRad(80.0f);
	m_fVelPost = NxMath::degToRad(50.0f);
	m_fDirection = 0.0f;
	m_fDirectionPre = 0.0f;



}

DriftStimulator::~DriftStimulator()
{
	
}

void DriftStimulator::Update(float dt, const CTencentCar::KEYINFO& keyinfo)
{
	m_bDriftCur = m_pTCar->GetDriftCenter()->IsDrifting();

	UpdateState(dt, keyinfo);
	UpdateAngle(dt);

	m_bDriftPre = m_bDriftCur;
}
void DriftStimulator::UpdateState(float dt, const CTencentCar::KEYINFO& keyinfo)
{
	CDriftCenter* pDrift = m_pTCar->GetDriftCenter();


	if (m_bDriftCur)
	{
		m_fDirection = (m_pTCar->GetCarHeadAngle() > 0.0f)? 1.0f:-1.0f;

		//if (keyinfo.bLeft && !keyinfo.bRight)
		//{
		//	m_fDirection = 1.0f;
		//}
		//else if (!keyinfo.bLeft && keyinfo.bRight)
		//{
		//	m_fDirection = -1.0f;
		//}

		//漂移过程开始的第一种情况：从无到有
		if (!m_bDriftPre)
		{
			m_state = DS_UP;
			//m_fDirection = (m_pTCar->GetCarHeadAngle() > 0.0f)? 1.0f:-1.0f;
			//m_fDirectionPre = m_fDirection;
		}

	}
	else
	{
		if (m_bDriftPre)//drift end
		{
			//根据当前的状况决定下降的速度、幅度. how?
			m_state = DS_DOWN;

			float fCoeff = NxMath::abs(m_pTCar->GetWecXLocal()) / 14.0f + 0.65f;
			if (fCoeff > 0.75f)
				fCoeff = 0.75f;

			m_fTargetAngleDown = -NxMath::sign(m_fRotX)* NxMath::abs(m_fRotX)* fCoeff;

		}

		//if ()
	}



}
void DriftStimulator::UpdateAngle(float dt)
{
	if (m_state == DS_UP)
	{
		
		if (m_fDirection > 0.0f)
		{
			if (m_fRotX < MAXROT_X)
			{
				m_fRotX += m_fVelUp* dt * m_fDirection;
			}
		}
		else
		{
			if (m_fRotX > (-MAXROT_X))
			{
				m_fRotX += m_fVelUp* dt * m_fDirection;
			}
		}
		if (abs(m_fRotX) < MAXROT_X)
		{
			m_fRotX += m_fVelUp* dt * m_fDirection;
		}

		LogText("HEAD ANGLE: %f\n", m_pTCar->GetCarHeadAngle());

	}
	else if (m_state == DS_DOWN)
	{
		if (m_fDirection > 0.0f)
		{
			m_fRotX += m_fVelDown*dt * (-m_fDirection);
			if (m_fRotX < m_fTargetAngleDown)//已经下降到底
			{
				m_fRotX = m_fTargetAngleDown;
				m_state = DS_POST;
			}
		}
		else
		{
			m_fRotX += m_fVelDown*dt * (-m_fDirection);
			if (m_fRotX > m_fTargetAngleDown)//已经下降到底
			{
				m_fRotX = m_fTargetAngleDown;
				m_state = DS_POST;
			}
		}
	}
	else if (m_state == DS_POST)
	{
		if (m_fDirection > 0.0f)
		{
			m_fRotX += m_fVelPost* dt* m_fDirection;
			if (m_fRotX > 0.0f)
			{
				m_fRotX = 0.0f;
				m_state = DS_NONE;
			}
		}
		else
		{
			m_fRotX += m_fVelPost* dt* m_fDirection;
			if (m_fRotX < 0.0f)
			{
				m_fRotX = 0.0f;
				m_state = DS_NONE;
			}
		}
	}

}
//-----------------------------