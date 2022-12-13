/********************************************************************
	作者:	　wincoqin
	时间:	  2007/04/13
    文件名:   BaseTerrainEffect.h
	说明:     []
*********************************************************************/
#pragma once

// libfile


// userfile
#include "ITerrainEffect.h"

// predefine



class CBaseTerrainPropEffect
	: public ITerrainPropEffect
{
public:
	CBaseTerrainPropEffect(const int nTerrainEffectType)
		: m_nTerrainEffectType(nTerrainEffectType)
		, m_bIsFinished(false)
		, m_bStart(false)
	{
	}
	virtual ~CBaseTerrainPropEffect(void)
	{

	}

	virtual void Update(float fDeltaTime)
	{

	}
	
	virtual void StartTerrainEffect()
	{
		m_bStart = true;
	}
	virtual void StopTerrainEffect()
	{
		m_bStart = false;
	}

	virtual bool IsPropEffectFinished()
	{
		return m_bIsFinished;
	}

	virtual const int GetTerrainPropEffectType()
	{
		return m_nTerrainEffectType;
	}

	static ITerrainPropEffect * CreateTerrainPropEffect(int nTerrainPropEffectType, 
		TerrainPropEffect * pTerrainPropEffect);

protected:
	bool	  m_bStart;
	bool	  m_bIsFinished;
	const int m_nTerrainEffectType;		
};

class CTerrainSoundEffect
	: public CBaseTerrainPropEffect
{
public:
	CTerrainSoundEffect();

	virtual bool InitPropEffect(TerrainPropEffect * pTerrainPropEffect);
	virtual void Update(float fDeltaTime);
	virtual void StartTerrainEffect();
	virtual void StopTerrainEffect();	

protected:
	int m_nSoundID;
	int m_nCycleType;
};

class CTerrainAnimationEffect
	: public CBaseTerrainPropEffect
{
public:
	CTerrainAnimationEffect();

	virtual bool InitPropEffect(TerrainPropEffect * pTerrainPropEffect);
	virtual void Update(float fDeltaTime);
	virtual void StartTerrainEffect();
	virtual void StopTerrainEffect();	

protected:
	int m_nAnimationPos;
	NiFixedString m_kAnimation;
	int m_nCycleType;
	NiAVObjectPtr m_spEffect;
};

class CGPJectAffect;
class CGPNo2Affect;

class CTerrainAccelEffect
	: public CBaseTerrainPropEffect
{
public:
	CTerrainAccelEffect();

	virtual bool InitPropEffect(TerrainPropEffect * pTerrainPropEffect);
	virtual void Update(float fDeltaTime);
	virtual void StartTerrainEffect();
	virtual void StopTerrainEffect();	

protected:
	float m_fXForce;
	float m_fForceTime;
	float m_fStartTime;

	CGPJectAffect * m_pJetAffect;
	CGPNo2Affect  * m_pNo2Affect;
};

class CTerrainJumpEffect
	: public CBaseTerrainPropEffect
{
public:
	CTerrainJumpEffect();
	
	virtual bool InitPropEffect(TerrainPropEffect * pTerrainPropEffect);
protected:
	NxVec3 m_kDeltaVelocity;
};

class CTerrainTransformEffect
	: public CBaseTerrainPropEffect
{
public:
	CTerrainTransformEffect();
	
	virtual bool InitPropEffect(TerrainPropEffect * pTerrainPropEffect);
	virtual void Update(float fDeltaTime);
	virtual void StartTerrainEffect();
	virtual void StopTerrainEffect();	

protected:
	NiFixedString m_kTransformAimedName;
	NiFixedString m_kAnimationName;
	float		  m_fAnimationTime;
};

class CTerrainAccelSpeedEffect 
	: public CBaseTerrainPropEffect
{
public:
	CTerrainAccelSpeedEffect();

	virtual void Update(float fDeltaTime);
	virtual bool InitPropEffect(TerrainPropEffect * pTerrainPropEffect);
	virtual void StartTerrainEffect();
	virtual void StopTerrainEffect();	

private:
	float			m_fOldGravityValue;
	bool			m_bIsDriftModifyGravity;
	float			m_fModifyRate;
};
