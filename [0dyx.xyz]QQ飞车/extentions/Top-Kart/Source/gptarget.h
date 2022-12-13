////////////////////////////////////////////////
//     FileName:    gptarget.h
//     Author:      Fish
//     Date:        2007-1-19 15:26:47
//     Description: []
////////////////////////////////////////////////

#ifndef _GPTARGET_H_
#define _GPTARGET_H_

#include "gpdefine.h"

class CGPTargetBase : public IGPTarget
{
public:
	CGPTargetBase();
	virtual ~CGPTargetBase();

	virtual DWORD	GetType();
	virtual DWORD	GetID();
	virtual DWORD	GetIndex();
	virtual DWORD	GetTeam();
	virtual NiPoint3	GetPos();
	virtual void	SetPos(const NiPoint3&);
	virtual NiPoint3	GetPhysXPos();
	virtual void		SetPhysXPos(const NiPoint3&);
	virtual NiPoint3	GetVelocity(BOOL bLocal);
	virtual void		ZeroVelocity();
	virtual NiPoint3	GetForward();
	virtual NiTransform GetTransform();
	virtual NiNode*	GetNode();
	virtual BOOL	Attach(const char* effect, const char* node);
	virtual BOOL	Detach(const char* node);
	virtual BOOL	Attach(NiAVObject*);
	virtual BOOL	Detach(NiAVObject*);
	virtual BOOL	PlayAnimation(DWORD AnimID, BOOL bPlay);
	virtual BOOL	StopAnimation(DWORD AnimID, BOOL bPlay);
	virtual void	AddForce(float force[3], BOOL bLocal);
	virtual void	ControlSpeed(float fRatio);
	virtual BOOL	ToggleSim(BOOL);

	virtual GP_ID_LIST&	GetGPs();
	virtual void		AddGP(DWORD id, DWORD index);
	virtual void		RemoveGP(DWORD id);

	virtual GP_DATA_LIST&	GetAffectGPs();
	virtual void			AddAffectGP(LPGP_DATA pData);
	virtual void			RemoveAffectGP(DWORD index);
	virtual void			ClearAffectGPs();

	virtual BOOL	SetAimed(BOOL);
	virtual BOOL	GetAimed();

	virtual BOOL	SetInvalidTarget(BOOL);
	virtual BOOL	GetInvalidTarget();

protected:
	void _DestroyAffectGP(LPGP_DATA);

protected:
	GP_ID_LIST		m_listGPs;
	GP_DATA_LIST	m_listAffectGPs;
	BOOL			m_bAimed;
	BOOL			m_bInvalidTarget;
};

#endif