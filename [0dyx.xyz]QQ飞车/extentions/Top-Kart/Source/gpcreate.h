////////////////////////////////////////////////
//     FileName:    gpcreate.h
//     Author:      Fish
//     Date:        2007-1-19 15:20:35
//     Description: ����ϵͳ�������󴴽��ļ�
////////////////////////////////////////////////

#ifndef _GPCREATE_H_
#define _GPCREATE_H_

#include "gpdefine.h"
#include "gptarget.h"
#include "gpaffectbase.h"
#include "gpcontrol.h"
#include "soundmgr.h"
#include "common.h"
#include "advtime.h"

class CGPDummyTarget : public CGPTargetBase
{
public:
	virtual DWORD	GetID()
	{
		return 0;
	}
	virtual DWORD	GetIndex()
	{
		return 0;
	}
	virtual DWORD	GetTeam()
	{
		return 0;
	}

};

class CGPTargetIterator : public IGPTargetIterator
{
public:
	virtual DWORD		GetTargetCount();
	virtual IGPTarget*	GetTarget(DWORD index);
	virtual IGPTarget*	GetHost();
	virtual IGPTarget*	GetDummy();

private:
	CGPDummyTarget	m_kDummyTarget;
};

class CGPDummyAffect : public CGPAffectBase
{
public:
};

/*
CGPAffect_FreezeGP:
��Ӧ��EAI_FreezeGP���μ�gpdefine.h�е�˵��
*/
class CGPAffect_FreezeGP : public CGPAffectBase
{
public:
	CGPAffect_FreezeGP():
	m_bToggle(TRUE)
	{
	}

	virtual void Start()
	{
		CGPAffectBase::Start();
		IGPTargetIterator* pIter = CGPControl::Get()->GetTargetIterator();
		if ( pIter && m_pDestTarget == pIter->GetHost() )
		{
			m_bToggle = m_pControl->ToggleOp(FALSE);
		}
	}

	virtual void Stop()
	{
		CGPAffectBase::Stop();
		if ( m_pControl )
		{
			m_pControl->ToggleOp(m_bToggle);
		}
	}

private:
	BOOL m_bToggle;
};

/*
CGPAffect_Animation:
��Ӧ��EAI_Animation���μ�gpdefine.h�е�˵��
*/
class CGPAffect_Animation : public CGPAffectBase
{
public:
	virtual void Start()
	{
		CGPAffectBase::Start();
		if ( m_pDestTarget )
		{
			BOOL bPlay = FALSE;
			if ( m_dwOption & EAO_AffectPlayInstead )
			{
				bPlay = TRUE;
			}
			XTrace("StartAnimation(%d)...at(%d)", (DWORD)m_fParam, GetHighResolutionTime());
			m_pDestTarget->PlayAnimation((DWORD)m_fParam, bPlay);
		}
	}

	virtual void Stop()
	{
		CGPAffectBase::Stop();
		if ( m_pDestTarget )
		{
			BOOL bPlay = FALSE;
			if ( m_dwOption & EAO_AffectPlayInstead )
			{
				bPlay = TRUE;
			}
			XTrace("StopAnimation(%d)...at(%d)", (DWORD)m_fParam, GetHighResolutionTime());
			m_pDestTarget->StopAnimation((DWORD)m_fParam, bPlay);
		}
	}

	virtual void Update(DWORD)
	{
		if ( m_pDestTarget )
		{
			BOOL bPlay = FALSE;
			if ( m_dwOption & EAO_AffectPlayInstead )
			{
				bPlay = TRUE;
			}
			m_pDestTarget->PlayAnimation((DWORD)m_fParam, bPlay);
		}
	}
};


/*
CGPAffect_Sound:
��Ӧ��EAI_Sound���μ�gpdefine.h�е�˵��
*/
class CGPAffect_Sound : public CGPAffectBase
{
public:
	CGPAffect_Sound():
	m_dwSound(0)
	{
	}

	virtual void Start()
	{
		CGPAffectBase::Start();
		IGPTargetIterator* pIter = CGPControl::Get()->GetTargetIterator();
		IGPTarget* pTarget = m_pDestTarget;
		if ( m_dwOption & EAO_AffectMeInstead )
		{
			pTarget=  m_pSrcTarget;
		}
		if ( pIter && pTarget == pIter->GetHost() )
		{
			m_dwSound = CSoundMgr::Get()->PlaySound((DWORD)m_fParam);
		}
	}

	virtual void Stop()
	{
		CGPAffectBase::Stop();
		if ( m_dwSound )
		{
			CSoundMgr::Get()->StopSound(m_dwSound);
		}
	}

private:
	DWORD	m_dwSound;
};

#include "no2controller.h"
/*
CGPAffect_No2Emitter:
��Ӧ��EAI_No2Emitter���μ�gpdefine.h�е�˵��
*/
class CGPAffect_No2Emitter : public CGPAffectBase
{
public:
	  virtual void Start()
	  {
		  CGPAffectBase::Start();
		  IGPTargetIterator* pIter = CGPControl::Get()->GetTargetIterator();
		  IGPTarget* pTarget = m_pDestTarget;
		  if ( pIter && pTarget == pIter->GetHost() )
		  {
			  if ( CNO2Controller::Get() )
			  {
					CNO2Controller::Get()->SetTerminateTime(m_fParam);
					CNO2Controller::Get()->ActivateNO2();
			  }
		  }
	  }

	  virtual void Stop()
	  {
		  CGPAffectBase::Stop();
		  if ( CNO2Controller::Get() )
		  {
				CNO2Controller::Get()->DeactivateNO2();
		  }
	  }
};

/*
CGPAffect_Speed:
��Ӧ��EAI_Speed���μ�gpdefine.h�е�˵��
*/
class CGPAffect_Speed : public CGPAffectBase
{
public:
	virtual void Start()
	{
		CGPAffectBase::Start();
		IGPTargetIterator* pIter = CGPControl::Get()->GetTargetIterator();
		//����Ǽ���ЧӦ�������ӻ���Ħ����ʵ��
		if ( pIter && m_pDestTarget == pIter->GetHost() && m_fParam < 1.0 )
		{
			m_pDestTarget->ControlSpeed(m_fParam);
		}
	}
	virtual void Stop()
	{
		CGPAffectBase::Stop();
		IGPTargetIterator* pIter = CGPControl::Get()->GetTargetIterator();
		if ( pIter && m_pDestTarget == pIter->GetHost() && m_fParam < 1.0 )
		{
			m_pDestTarget->ControlSpeed(1.0 / m_fParam);
		}
	}
	
	virtual void Update(DWORD)
	{
		IGPTargetIterator* pIter = CGPControl::Get()->GetTargetIterator();
		//����Ǽ���Ч�����ü�����ʵ��
		if ( pIter && m_pDestTarget == pIter->GetHost() && m_fParam > 1.0 )
		{
			NiPoint3 kVel = m_pDestTarget->GetVelocity(TRUE);
			//�����û���ٶȾͲ�Ҫ����ĥ����
			if ( abs(kVel.x) > 0.8 )
			{
				float fForceX = m_fParam;
				if ( kVel.x < 0 )
				{	//�����ˣ������ķ���Ҳȡ��
					fForceX *= (-1); 
				}
				float fForce[3] = { fForceX, 0, 0 };
				m_pDestTarget->AddForce(fForce, TRUE);
			}
		}
	}
};

/*
CGPAffect_Generator:
��Ӧ��EAI_Generator���μ�gpdefine.h�е�˵��
*/
class CGPAffect_Generator : public CGPAffectBase
{
public:
	virtual void Start()
	{
		m_pControl->GenerateNewGP(m_kGPID, (DWORD)m_fParam, m_pSrcTarget);
	}
};
#endif