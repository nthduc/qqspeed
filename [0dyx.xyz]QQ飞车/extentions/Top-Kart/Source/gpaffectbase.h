////////////////////////////////////////////////
//     FileName:    gpaffectbase.h
//     Author:      Fish
//     Date:        2007-1-26 14:48:34
//     Description: 实现IGPAffect的基类，其他的IGPAffect建议从这个类派生
////////////////////////////////////////////////

#ifndef _GPAFFECTBASE_H_
#define _GPAFFECTBASE_H_

#include "gpdefine.h"

class CGPControl;
class CGPAffectBase : public IGPAffect
{
public:
	CGPAffectBase():
    m_pControl(NULL),
	m_pSrcTarget(NULL),
	m_pDestTarget(NULL),
	m_fParam(0.0),
	m_dwOption(0),
	m_bPlaying(FALSE),
	m_dwLifeTime(0)
	{
	}

	~CGPAffectBase()
	{
	}

	//IGPAffect
	virtual void Init(CGPControl* pControl, GP_ID kGPID)
	{
		m_pControl = pControl;
		m_kGPID = kGPID;
	}

	virtual void SetTarget(IGPTarget* pTargetSrc, IGPTarget* pTargetDest)
	{
		m_pSrcTarget = pTargetSrc;
		m_pDestTarget = pTargetDest;
	}

	virtual void SetLifeTime(DWORD dwTime)
	{
		m_dwLifeTime = dwTime;
	}

	virtual void SetParam(float fParam)
	{
		m_fParam = fParam;
	}

	virtual void SetOption(DWORD dwOption)
	{
		m_dwOption = dwOption;
	}

	virtual void Start()
	{
		m_bPlaying = TRUE;
	}

	virtual void Stop()
	{
		m_bPlaying = FALSE;
	}

	virtual BOOL IsPlaying()
	{
		return m_bPlaying;
	}

	virtual void Update(DWORD)
	{
	}

	virtual BOOL CanUnload()
	{
		return FALSE;
	}

public:
	CGPControl*	m_pControl;
	GP_ID		m_kGPID;
	DWORD		m_dwLifeTime;
	IGPTarget*	m_pSrcTarget;
	IGPTarget*	m_pDestTarget;
	float		m_fParam;
	DWORD		m_dwOption;
	BOOL		m_bPlaying;
};
#endif