////////////////////////////////////////////////
//FileName:    GPMissileCtrl.h
//Author:      PigHuang
//Date:        2007-01-29
//Description: �������߶���������
////////////////////////////////////////////////

#pragma once
#include "gpdefine.h"
///����ģ��nif��ԭʼָ��������ת(������)
const	NiPoint3	conMissileNifDirection=NiPoint3(1.0f,0.0f,0.0f);
///����ģ����Effect���Id��Ŀǰ��ֱ���趨��idֵ
const UINT conMissileModleEffectId=14;
///����β��ģ����Effect���Id��Ŀǰ��ֱ���趨��idֵ
const UINT conMissileSpraySmokeEffectId=30;

///�������߶���������
class CGPMissileCtrl	:	public IGPMovementControl
{
private:
	///�����������в���
	typedef	struct tagHelixParam
	{
		float	m_fHelixRadius;		///�������������˶��뾶
		float	m_fCircleCount;		///�ܵķ���Ȧ��
		tagHelixParam::tagHelixParam();
	}HelixParam;
public:
	CGPMissileCtrl(void);
	virtual ~CGPMissileCtrl(void);
	//ʵ���ڽӿڵĶ��庯��
	virtual void SetTarget(IGPTarget* pSrc, IGPTarget* pDest);
	virtual void SetPos(NiPoint3& kSrcPos, NiPoint3& kDestPos);
	virtual void SetParam(NiNodePtr pkMissile,DWORD dwLife);
	virtual void Update(DWORD dwLifeAcc);
	virtual void Start();
	virtual void Stop();
	virtual BOOL IsPlaying();

protected:
	NiPoint3	GetSrcTargetPos();
	NiPoint3	GetDestTargetPos();

protected:
	NiPoint3	m_kSrcPos;
	NiPoint3	m_kDestPos;

	bool	m_bActive;		///��ǰ������ʾ��update�б�־
	NiPoint3	m_kStartVect;	///��������������
	NiPoint3	m_kStartDir;	///��������ʱ�ķ���ָ��
	NiPoint3	m_kLastVec;	///��һ��updateʱ����������
	NiPoint3	m_kLastDir;	///��һ��updateʱ�����ķ���ָ��
	IGPTarget*	m_pSrcTarget;	///���䵼���Ķ���
	IGPTarget*	m_pDestTarget;	///�����������Ķ���
	NiNodePtr	m_pkMissile;	///����ģ��
	HelixParam	m_stHelixParam;	///�������в���
	DWORD		m_dwWholeLife;		///���߶�������������(ms)
	DWORD		m_dwLastUpdateTime;	///���һ��updateʱ��(ָ�ۼ�ʱ��,<=m_dwWholeLife)
	NiNodePtr	m_pkSmoke;	///������β��������ģ��
	NiAmbientLightPtr m_spAmbientLight;
	NiPSysBoxEmitterPtr	m_spSmokeEmitter;
	NiTimeControllerPtr	m_spSmokeEmitCtr;
	float m_fInitEmitSpeed;
	float m_fCurEmitSpeed;
	float m_fInitEmitFrequence;
};
