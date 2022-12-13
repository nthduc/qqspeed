////////////////////////////////////////////////
//FileName:    GPFlyingCtrl.h
//Author:      PigHuang
//Date:        2007-02-09
//Description: ���ը�����߶���������
////////////////////////////////////////////////

#pragma once
#include "gpdefine.h"
///���ը��ģ��nif��ԭʼָ��������ת(������)
const	NiPoint3	conFlyingNifDirection=NiPoint3(1.0f,0.0f,0.0f);
///���ը��ģ����Effect���Id��Ŀǰ��ֱ���趨��idֵ
const UINT conFlyingModleEffectId=12;
///todo:���ը��β��ģ����Effect���Id��!!!!Ŀǰ��ֱ��ʹ�õ���β�̵�idֵ
const UINT conFlyingSpraySmokeEffectId=30;

//��Ӱ��Ч
class Swoosh;

///���ը�����߶���������
class CGPFlyingCtrl	:	public IGPMovementControl
{
private:
	///���ը��ת�۷��в���
	typedef	struct tagHelixParam
	{
		float	m_fHelixMaxRadius;		///���ը������ƽ���˶�������ư뾶
		float	m_fHelixMinRadius;		///���ը������ƽ���˶���С���ư뾶
		float	m_fHelixSideMaxAngle;		///���ը������ƽ�浥����ת��󻡶�
		float	m_fHelixSideMinAngle;		///���ը������ƽ�浥����ת��С����
		float	m_fTurnMaxCount;		///�ܵ�ת�۷��е�������ƴ���
		float	m_fTurnMinCount;		///�ܵ�ת�۷��е���С���ƴ���
		float	m_fTurnCount;		///�ܵ�ת�۷��еĴ���,Startʱȷ��
		float	m_fNowTurnCount;	///��ǰ������ת�۷��еĴ���,��ʼ����ʱ��0.0f
		float	m_fLastTurnCount;	///��ǰ������ת�۷��еĴ���,��ʼ����ʱ��0.0f
		float	m_fLastTurnAngle;	///��ǰת�۷���Ŀ����������ת����
		NiPoint3	m_kNowDestVec;	///��ǰ���ը��ȷ���ı���ת�۷��е���������
		NiPoint3	m_kLastTurnVec;	///��һ��updateʱ���ը��ȷ���ı���ת�۷��е���������
		tagHelixParam::tagHelixParam();
	}HelixParam;
public:
	CGPFlyingCtrl(void);
	virtual ~CGPFlyingCtrl(void);
	//ʵ���ڽӿڵĶ��庯��
	virtual void SetTarget(IGPTarget* pSrc, IGPTarget* pDest);
	virtual void SetPos(NiPoint3& kSrcPos, NiPoint3& kDestPos);
	virtual void SetParam(NiNodePtr pkFlying,DWORD dwLife);
	virtual void Update(DWORD dwLifeAcc);
	virtual void Start();
	virtual void Stop();
	virtual BOOL IsPlaying();
protected:
	bool	m_bActive;		///��ǰ���ը����ʾ��update�б�־
	NiPoint3	m_kStartVect;	///���ը������������
	NiPoint3	m_kLastVec;	///��һ��updateʱ���ը��������
	IGPTarget*	m_pSrcTarget;	///������ը���Ķ���
	IGPTarget*	m_pDestTarget;	///�����ը�������Ķ���
	NiNodePtr	m_pkFlying;	///���ը��ģ��
	HelixParam	m_stHelixParam;	///�������в���
	DWORD		m_dwWholeLife;		///���߶�������������(ms)
	DWORD		m_dwLastUpdateTime;	///���һ��updateʱ��(ָ�ۼ�ʱ��,<=m_dwWholeLife)
	NiNodePtr	m_pkSmoke;	///���ը����β��������ģ��
	//�����Ӱ��Ч
	Swoosh* m_pkSwoosh;
};
