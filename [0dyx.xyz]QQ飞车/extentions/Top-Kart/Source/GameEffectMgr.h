#pragma once

#include "NiSystem.h"
#include "NiNode.h"
#include "NifAnimationInfo.h"
#include "NiParticleSystem.h"
#include "NiPSysBoxEmitter.h"
#include "singlet.h"

class IGameEffect;
class CSpeedLine;

#define MAKE_ID(nNum,nFlag)  ( (nNum << ET_MASK) | (nFlag) )

class IGameEffect : public NiMemObject
{
public:
	enum EffectType
	{
		///
		ET_SCREEN = 1,
		ET_ONLY   = 2,
		ET_LOOP   = 4,
		ET_REVERT = 8,
		ET_MASK   = 8,

		///
		ET_SNOW		=	MAKE_ID(1, ET_ONLY|ET_LOOP),
		ET_SPEED	=   MAKE_ID(2, ET_ONLY|ET_LOOP|ET_SCREEN),
		ET_ACCLINE	=	MAKE_ID(3, ET_ONLY|ET_LOOP),
		ET_CRASH	=	MAKE_ID(4, ET_ONLY),
		ET_BACKFIRE =	MAKE_ID(5, ET_ONLY|ET_REVERT),
		ET_CAIDAI   =   MAKE_ID(6, ET_ONLY|ET_LOOP|ET_SCREEN),
		ET_SMOKE	=	MAKE_ID(7, ET_ONLY|ET_LOOP),

		ET_STARTCOUNTDOWN = MAKE_ID(8,	ET_ONLY|ET_SCREEN),
		ET_OVERCOUNTDOWN  = MAKE_ID(9,	ET_ONLY|ET_SCREEN),
		ET_FINISHTEXT	  = MAKE_ID(10, ET_ONLY|ET_SCREEN),
		ET_UNFINISHTEXT   = MAKE_ID(11, ET_ONLY|ET_SCREEN),
		ET_FIRSTLAP		  = MAKE_ID(12, ET_ONLY|ET_SCREEN),
		ET_SECONDLAP	  = MAKE_ID(13, ET_ONLY|ET_SCREEN),
		ET_LASTLAP		  = MAKE_ID(14, ET_ONLY|ET_SCREEN),
		ET_WRONGWAY		  = MAKE_ID(15, ET_ONLY|ET_LOOP|ET_SCREEN),
		ET_ANGLEDRIFT	  = MAKE_ID(16, ET_LOOP),
		ET_ANGLEDRIFTCENTER = MAKE_ID(17, ET_LOOP),
		ET_STOP			  = MAKE_ID(18, ET_ONLY | ET_LOOP|ET_SCREEN),
		ET_PERFECT		  = MAKE_ID(19, ET_ONLY),
		ET_PRESSUP		  = MAKE_ID(20, ET_ONLY|ET_SCREEN),
		ET_PRESSDOWN	  = MAKE_ID(21, ET_ONLY|ET_SCREEN),
		ET_PRESSLEFT	  = MAKE_ID(22, ET_ONLY|ET_SCREEN),
		ET_PRESSRIGHT	  = MAKE_ID(23, ET_ONLY|ET_SCREEN),
		ET_TASKFINISH	  = MAKE_ID(24, ET_ONLY|ET_SCREEN),
		ET_DRIFT_STEP1	  = MAKE_ID(25, ET_ONLY|ET_SCREEN|ET_LOOP),
		ET_DRIFT_STEP2	  = MAKE_ID(26, ET_ONLY|ET_SCREEN|ET_LOOP),
		ET_DRIFT_STEP3	  = MAKE_ID(27, ET_ONLY|ET_SCREEN|ET_LOOP),
		ET_DRIFT_STEP4	  = MAKE_ID(28, ET_ONLY|ET_SCREEN|ET_LOOP),


		MAX_EFFECT_NUM = 30,
	};

	IGameEffect(EffectType eType) : m_eType(eType),m_bActive(false),m_spEffectNode(NULL),m_spCamera(NULL){};

	virtual ~IGameEffect(){};


	void Update(float fTime,NiPoint3* pPos);



	virtual void Start();
	virtual void Stop();

	void Destroy(){m_spEffectNode = NULL; m_spCamera = NULL;};


	bool IsActive(){return m_bActive;};
	EffectType GetType(){return m_eType;};
	int        GetIndex(){return m_eType >> ET_MASK;};
	CNifAnimationInfo GetInfo(){return m_sInfo;};
	void PostLoad(){if(m_spEffectNode)m_sInfo.CollectData(m_spEffectNode);};
	void PreLoad(){Destroy();};
	void UpdateActive();

	void Load(NiNodePtr spNode);

	void SetPos(NiPoint3* pPos)
	{
		if(pPos)
		{
			m_nPos = *pPos;
		};
	};



	virtual void Render(NiCamera* spCamera) = 0;
	virtual IGameEffect* Clone() = 0;
	virtual void CoreLoad(NiNodePtr spNode) = 0;
	virtual void CoreUpdate(float fTime,NiPoint3* pPos) = 0;
protected:
	EffectType m_eType;
	NiNodePtr  m_spEffectNode;
	NiCameraPtr m_spCamera;
	bool        m_bActive;
	CNifAnimationInfo  m_sInfo;
	float       m_fStartTime;
	NiPoint3    m_nPos;



};

inline int GET_EFFECT_INDEX(IGameEffect::EffectType eType)
{
	return  eType >> IGameEffect::ET_MASK;
}


class CGameEffectMgr : public CSingleT<CGameEffectMgr>
{
public:
	CGameEffectMgr();
	~CGameEffectMgr();
	void ActiveEffect(IGameEffect::EffectType eType,NiPoint3* pPos);
	void DeActiveEffect(IGameEffect::EffectType eType);

	void InitEffect();
	void DestroyEffect();

	void ResetEffect();

	void Update(float fTime);

	void Render(NiCamera* pkCamera);
	void ScreenRender(NiCamera* pkCamera);


private:

	IGameEffect* m_pkBaseParticles[IGameEffect::MAX_EFFECT_NUM];
	NiTPointerList<IGameEffect*> m_kActiveParticles;
	NiTPointerList<IGameEffect*> 
		m_kInActiveParticles[IGameEffect::MAX_EFFECT_NUM];
};



class ISnowEffect : public IGameEffect
{
public:
	ISnowEffect() : IGameEffect(ET_SNOW){m_bShow = true;m_fTime = 0.0f;};
	virtual ~ISnowEffect(){};

	virtual void Render(NiCamera* spCamera);
	virtual IGameEffect* Clone();
	virtual void CoreLoad(NiNodePtr spNode);
	virtual void CoreUpdate(float fTime,NiPoint3* pPos);


private:
	bool m_bShow;
	float m_fTime;
};


class ISpeedEffect : public IGameEffect
{
public:
	ISpeedEffect() : IGameEffect(ET_SPEED){m_bShow = true;};
	virtual ~ISpeedEffect();

	virtual void Render(NiCamera* spCamera);
	virtual IGameEffect* Clone();
	virtual void CoreLoad(NiNodePtr spNode);
	virtual void CoreUpdate(float fTime,NiPoint3* pPos);


private:
	bool m_bShow;
	CSpeedLine* m_pSpeedLine;
};

class ICrashEffect : public IGameEffect
{
public:
	ICrashEffect() : IGameEffect(ET_CRASH){};

	virtual ~ICrashEffect(){};

	virtual void Render(NiCamera* spCamera);
	virtual IGameEffect* Clone();
	virtual void CoreLoad(NiNodePtr spNode);
	virtual void CoreUpdate(float fTime,NiPoint3* pPos);

};


class IAccEffect : public IGameEffect
{
public:
	IAccEffect() : IGameEffect(ET_ACCLINE){};

	virtual ~IAccEffect(){};

	virtual void Render(NiCamera* spCamera);
	virtual IGameEffect* Clone();
	virtual void CoreLoad(NiNodePtr spNode);
	virtual void CoreUpdate(float fTime,NiPoint3* pPos);

	virtual void Start();
	virtual void Stop();

};


class IBackFireEffect : public IGameEffect
{
public:
	IBackFireEffect();

	virtual ~IBackFireEffect(){ m_spEffectNode2 = NULL; m_spRoot = NULL; m_spAmbientLight = NULL; m_spEffectNode3 = NULL; m_spEffectNode4 = NULL;};

	virtual void Render(NiCamera* spCamera);
	virtual IGameEffect* Clone();
	virtual void CoreLoad(NiNodePtr spNode);
	virtual void CoreUpdate(float fTime,NiPoint3* pPos);

	virtual void Start();

private:
	NiNodePtr m_spEffectNode2;
	NiNodePtr m_spEffectNode3;
	NiNodePtr m_spEffectNode4;

	NiNodePtr m_spRoot;
	NiAmbientLightPtr m_spAmbientLight;

};

class ICaiDaiEffect : public IGameEffect
{
public:
	ICaiDaiEffect();
	virtual ~ICaiDaiEffect();
	virtual void Render(NiCamera* spCamera);
	virtual IGameEffect * Clone();
	virtual void CoreLoad(NiNodePtr spNode);
	virtual void CoreUpdate(float fTime,NiPoint3* pPos);
};

class IGameFinishEffect : public IGameEffect
{
public:
	IGameFinishEffect();
	virtual ~IGameFinishEffect();
	virtual void Render(NiCamera * spCamera);
	virtual IGameEffect * Clone();
	virtual void CoreLoad(NiNodePtr spNode);
	virtual void CoreUpdate(float fTime,NiPoint3* pPos);
};

class IGamePerfectEffect : public IGameEffect
{
public:
	IGamePerfectEffect();
	virtual ~IGamePerfectEffect();
	virtual void Render(NiCamera * spCamera);
	virtual IGameEffect * Clone();
	virtual void CoreLoad(NiNodePtr spNode);
	virtual void CoreUpdate(float fTime,NiPoint3* pPos);
};

class IGameUnFinishEffect : public IGameEffect
{
public:
	IGameUnFinishEffect();
	virtual ~IGameUnFinishEffect();
	virtual void Render(NiCamera * spCamera);
	virtual IGameEffect * Clone();
	virtual void CoreLoad(NiNodePtr spNode);
	virtual void CoreUpdate(float fTime,NiPoint3* pPos);
};

class IGameStartCountdownEffect: public IGameEffect
{
public:
	IGameStartCountdownEffect();
	virtual ~IGameStartCountdownEffect();
	virtual void Render(NiCamera* spCamera);
	virtual IGameEffect * Clone();
	virtual void CoreLoad(NiNodePtr spNode);
	virtual void CoreUpdate(float fTime,NiPoint3* pPos);
};

class IFirstLapEffect: public IGameEffect
{
public:
	IFirstLapEffect();
	virtual ~IFirstLapEffect();
	virtual void Render(NiCamera* spCamera);
	virtual IGameEffect * Clone();
	virtual void CoreLoad(NiNodePtr spNode);
	virtual void CoreUpdate(float fTime,NiPoint3* pPos);
};

class ISecondLapEffect: public IGameEffect
{
public:
	ISecondLapEffect();
	virtual ~ISecondLapEffect();
	virtual void Render(NiCamera* spCamera);
	virtual IGameEffect * Clone();
	virtual void CoreLoad(NiNodePtr spNode);
	virtual void CoreUpdate(float fTime,NiPoint3* pPos);
};

class ILastLapEffect: public IGameEffect
{
public:
	ILastLapEffect();
	virtual ~ILastLapEffect();
	virtual void Render(NiCamera* spCamera);
	virtual IGameEffect * Clone();
	virtual void CoreLoad(NiNodePtr spNode);
	virtual void CoreUpdate(float fTime,NiPoint3* pPos);
};
class IWrongWayEffect: public IGameEffect
{
public:
	IWrongWayEffect();
	virtual ~IWrongWayEffect();
	virtual void Render(NiCamera* spCamera);
	virtual IGameEffect * Clone();
	virtual void CoreLoad(NiNodePtr spNode);
	virtual void CoreUpdate(float fTime,NiPoint3* pPos);
};
class IGameOverCountdownEffect: public IGameEffect
{
public:
	IGameOverCountdownEffect();
	virtual ~IGameOverCountdownEffect();
	virtual void Render(NiCamera* spCamera);
	virtual IGameEffect * Clone();
	virtual void CoreLoad(NiNodePtr spNode);
	virtual void CoreUpdate(float fTime,NiPoint3* pPos);
protected:
	bool m_bStarting;
	DWORD m_nSoundTimes;

};

class ISmokeEffect :public IGameEffect
{
public:
	ISmokeEffect();
	virtual ~ISmokeEffect();
	virtual void Render(NiCamera* spCamera);
	virtual IGameEffect* Clone();
	virtual void CoreLoad(NiNodePtr spNode);
	virtual void CoreUpdate(float fTime,NiPoint3* pPos);

	virtual void Start();
private:
	NiNodePtr m_spSmokeEffNode2;

	NiNodePtr m_spRoot;
	NiAmbientLightPtr m_spAmbientLight;

	NiPSysBoxEmitterPtr m_spSmokeEmitter1;
	NiPSysBoxEmitterPtr m_spSmokeEmitter2;

	NiTimeControllerPtr m_spSmokeEmitCtr1;
	NiTimeControllerPtr m_spSmokeEmitCtr2;

	float m_fInitEmitSpeed;
	float m_fCurEmitSpeed;
	float m_fInitEmitFrequence;
};



class CAngleDrifting : public IGameEffect
{
public:
	CAngleDrifting();
	virtual ~CAngleDrifting();

	virtual void Render( NiCamera * spCamera);
	virtual IGameEffect * Clone();
	virtual void CoreLoad(NiNodePtr spNode);
	virtual void CoreUpdate( float fTime, NiPoint3* pPos);
	virtual void Start();
protected:
	float m_fLiveingTime;
	float m_fDriftingStartTime;


};
class CAngleDriftingCenter : public IGameEffect
{
public:
	CAngleDriftingCenter();
	virtual ~CAngleDriftingCenter();
	virtual void Render( NiCamera * spCamera);
	virtual IGameEffect * Clone();
	virtual void CoreLoad(NiNodePtr spNode);
	virtual void CoreUpdate( float fTime, NiPoint3* pPos);
	virtual void Start();
protected:
	float m_fLiveingTime;
	float m_fDriftingStartTime;
};

//	增加人:	wincoqin
//  时间:	2007/02/06
//  目的:	禁止牌
class CStopShow : public IGameEffect
{
public:
	CStopShow();
	virtual ~CStopShow();
	virtual void Render(NiCamera * spCamera);
	virtual IGameEffect * Clone();
	virtual void CoreLoad(NiNodePtr spNode);
	virtual void CoreUpdate(float fTime,NiPoint3* pPos);
};
//---------------------------------------------------------------
//按前键特效
class CPressUP_ET : public IGameEffect
{
public:
	CPressUP_ET();
	virtual ~CPressUP_ET();
	virtual void Render(NiCamera * spCamera);
	virtual IGameEffect * Clone();
	virtual void CoreLoad(NiNodePtr spNode);
	virtual void CoreUpdate(float fTime,NiPoint3* pPos);
};
//---------------------------------------------------------------
//按后键特效
class CPressDown_ET : public IGameEffect
{
public:
	CPressDown_ET();
	virtual ~CPressDown_ET();
	virtual void Render(NiCamera * spCamera);
	virtual IGameEffect * Clone();
	virtual void CoreLoad(NiNodePtr spNode);
	virtual void CoreUpdate(float fTime,NiPoint3* pPos);
};
//---------------------------------------------------------------
//按左键特效
class CPressLeft_ET : public IGameEffect
{
public:
	CPressLeft_ET();
	virtual ~CPressLeft_ET();
	virtual void Render(NiCamera * spCamera);
	virtual IGameEffect * Clone();
	virtual void CoreLoad(NiNodePtr spNode);
	virtual void CoreUpdate(float fTime,NiPoint3* pPos);
};
//---------------------------------------------------------------
//按右键特效
class CPressRight_ET : public IGameEffect
{
public:
	CPressRight_ET();
	virtual ~CPressRight_ET();
	virtual void Render(NiCamera * spCamera);
	virtual IGameEffect * Clone();
	virtual void CoreLoad(NiNodePtr spNode);
	virtual void CoreUpdate(float fTime,NiPoint3* pPos);
};

//Task w完成特效
class CTaskFinish_ET : public IGameEffect
{
public:
	CTaskFinish_ET();
	virtual ~CTaskFinish_ET();
	virtual void Render(NiCamera * spCamera);
	virtual IGameEffect * Clone();
	virtual void CoreLoad(NiNodePtr spNode);
	virtual void CoreUpdate(float fTime,NiPoint3* pPos);
};

//---------------------------------------------------------------
//漂移任务step1特效
class CDriftTask_Step1 : public IGameEffect
{
public:
	CDriftTask_Step1();
	virtual ~CDriftTask_Step1();
	virtual void Render(NiCamera * spCamera);
	virtual IGameEffect * Clone();
	virtual void CoreLoad(NiNodePtr spNode);
	virtual void CoreUpdate(float fTime,NiPoint3* pPos);
};

//---------------------------------------------------------------
//漂移任务step2特效
class CDriftTask_Step2 : public IGameEffect
{
public:
	CDriftTask_Step2();
	virtual ~CDriftTask_Step2();
	virtual void Render(NiCamera * spCamera);
	virtual IGameEffect * Clone();
	virtual void CoreLoad(NiNodePtr spNode);
	virtual void CoreUpdate(float fTime,NiPoint3* pPos);
};


//---------------------------------------------------------------
//漂移任务step3特效
class CDriftTask_Step3 : public IGameEffect
{
public:
	CDriftTask_Step3();
	virtual ~CDriftTask_Step3();
	virtual void Render(NiCamera * spCamera);
	virtual IGameEffect * Clone();
	virtual void CoreLoad(NiNodePtr spNode);
	virtual void CoreUpdate(float fTime,NiPoint3* pPos);
};


//---------------------------------------------------------------
//漂移任务step4特效
class CDriftTask_Step4 : public IGameEffect
{
public:
	CDriftTask_Step4();
	virtual ~CDriftTask_Step4();
	virtual void Render(NiCamera * spCamera);
	virtual IGameEffect * Clone();
	virtual void CoreLoad(NiNodePtr spNode);
	virtual void CoreUpdate(float fTime,NiPoint3* pPos);
};