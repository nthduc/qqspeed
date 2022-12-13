////////////////////////////////////////////////
//     FileName:    gpcontrol.h
//     Author:      Fish
//     Date:        2007-1-18 14:37:27
//     Description: ���������߼�ʵ���ļ�
////////////////////////////////////////////////

#ifndef _GPCONTROL_H_
#define _GPCONTROL_H_

#include "gpdefine.h"
#include "GamePropCollisionReport.h"
#include "gpaffectbase.h"
#include "singlet.h"

class CGPAimer;
class CGPControl;
class CGPColumnMgr;

/*
CGPControl:
����ʹ���ܿ��߼���������
1��ʹ�õ��ߴ���
2�����������ڹ���
3���������û�����������
4�����������ڱ��ִ���
*/
class CGPControl:
	public CSingleT<CGPControl>,
	public CGamePropCollisionReport
{
public:
	CGPControl();
	~CGPControl();

	//�ر�����
	BOOL AppInit();
	BOOL AppUnInit();

	//����
	void Update(DWORD);
	
	//��ӦCtrl��������
	void OnCtrlPress();

	//��ӦCtrl�����ɿ�
	void OnCtrlRelease();

	//OnAimFinished�����������CGPAimer�ڽ�����׼��ʱ��ص�
	void OnAimFinished(IGPTarget*);	
	
	//OnTargetAimed����ĳ��Ŀ�걻��׼��ʱ����ã�bSet������ʾ����׼����ȡ����׼
	void OnTargetAimed(DWORD dwDestID, BOOL bSet);

	//OnUseGP��CNotifyUsePropPara��Ϣ���˵����
	void OnUseGP(GP_ID id, DWORD dwSrcID, DWORD dwDestID = 0, bool bSelfDefineTarget = false,IGPTarget * pTarget = NULL );

	//OnTriggerByGP:CNotifyPropEffectResultPara���˵����
	void OnTriggerByGP(GP_ID id, DWORD dwDestID);

	//SetAimed����Ŀ�괦�ڱ���׼״̬
	void SetAimed(DWORD dwDestID, BOOL bSet);

	//ToggleAffect:�򿪻�ر����ÿ��أ�������ÿ��ر��رգ�����Ч��������ֹ
	BOOL ToggleAffect(BOOL bToggle);

	//ToggleOp���򿪻�رղ������أ�����������ر��رգ����в���������ֹ
	BOOL ToggleOp(BOOL);

	//OnGotten���õ��˵��ߣ��������Ქ�ųԵ�����Ч
	void OnGotten(DWORD id, DWORD dwSrcID);

	//�ӳ����ߣ��������Ქ����Ч֮��
	void OnThrow(DWORD id, DWORD dwSrcID, BOOL bThrow = TRUE);

	//���߻�����Ŀ�꣬�������Ქ����Ч֮��
	void OnTrigger(LPGP_DATA pData);

	//���߱��ų��ˣ���������ɱ��
	void OnDisabled(LPGP_DATA pData);

	//���������ڽ����ˣ���������ɱ��
	void OnDie(LPGP_DATA pData);

	//�����͵��߷�����һ�ι������������Ქ����Ч֮��
	void OnProtect(DWORD id, DWORD dwSrcID);

	//�����͵������Ƴɹ���������Ч
	void OnCure(DWORD id, DWORD dwSrcID);

	//��Ӧϵͳ��Ϣ
	STDMETHOD(WndProc)(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	//FindConfig: �ҵ���Ӧ���ߵ�������Ϣ
	LPGP_CONFIG FindConfig(DWORD id);

	//���������͵ĵ�����Ҫ���
	BOOL		ProcessGeneratorGP(GP_ID id, IGPTarget* pSrcTarget);	

	//�����µ���
	BOOL	GenerateNewGP(GP_ID id, DWORD dwNewID, IGPTarget*);

	//GetTargetIterator:
	IGPTargetIterator* GetTargetIterator();

	//GetTargetByID
	IGPTarget*	GetTargetByID(DWORD id);	//ͨ��TargetIDȡ��Target

	//CGamePropCollisionReport
	virtual int OnTrigger(LPVOID lpData,LPVOID lpHitObjData);

	//Draw
	void Draw(NiCamera*);

	//������Դ
	void Start();
	void End();

private:
	BOOL _LoadConfig();
	void _UnloadConfig();
	BOOL _InitObjects();
	void _UnInitObjects();

	BOOL	_CanUse(IGPTarget*, DWORD id);
	BOOL	_CanStop(IGPTarget*, DWORD id);
	BOOL	_IsTrueCondition(IGPTarget*, DWORD id, DWORD dwCondition);
	BOOL	_ApplyGP(GP_ID id, IGPTarget* pSrcTarget, IGPTarget* pTarget = NULL, DWORD* pdwIDConv = NULL);
	void	_AddAffectGP(IGPTarget*, GP_ID id, DWORD dwSrcID);
	void	_UpdateTarget(IGPTarget* pTarget);
	void	_UpdateOrphans();
	void	_RemoveOrphans();
	void	_RemoveOrphan(LPGP_DATA);
	void	_UpdateBlendState(IGPTarget* pTarget, GP_DATA_LIST&, GP_DATA_LIST_Iterator&);	//ͨ����������������״̬
	void	_UpdateGPMove(LPGP_DATA, DWORD dwTime);
	void	_PlaySpecialEffect(IGPTarget*, GP_SPECIAL_EFFECT_LIST&);
	DWORD	_GetAffectsTotalTime(GP_AFFECT_LIST&);	//������ʱ��
	IGPTarget*	_GetTargetByIndex(DWORD dwIndex);//ͨ������ȡ��Target
	DWORD		_GetIndexInTeam(IGPTarget*);	//ȡ�����Լ������ڵ�����
	GPTARGET_LIST	_FetchTargetList(DWORD id, IGPTarget*);	//������Ŀ�꼯��
	ENMLIFESTATE	_EnsureLifeState(LPGP_DATA, ENMLIFESTATE);
	GP_DATA_LIST_Iterator	_FindOrphan(GP_ID);

	friend class CGPAimer;

private:
	GP_CONFIG_LIST		m_ConfigList;
	GP_DATA_LIST		m_listOrphanGPs;
	GP_ID				m_idCurrentHold;
	CGPAimer*			m_pAimer;
	IGPTargetIterator*	m_pTargetIter;
	CGPColumnMgr*		m_pColumnMgr;
	BOOL				m_bAffectEnabled;
	BOOL				m_bOpEnabled;
	BOOL				m_bEnd;
};

#define AIM_MAX_EFFECTS	3
class RotateController;
class ScaleController;

/*
CGPAimer:
��׼��
ʵ�ַ�ʽ������
1����׼��ԭ���ڳ���λ�ÿ�ǰ��һ���̶�λ�ã���ͨ���ű����ã���
2����׼�����ǵ�ǰ����ĳ���
3���жϴ���׼��ԭ���������׼�����ϵ�Ŀ��ľ���ͽǶȣ���ΪԶ���롢
	�о���ͽ����롣
4������ж��Ŀ���ڽ������ڣ���ÿ��Ŀ��һ����׼ʱ�䣬������׼
*/
class CGPAimer
{
public:
	typedef enum enumShootScope
	{
		ESS_None	= -1,
		ESS_Far		= 0,
		ESS_Mid		= 1,
		ESS_Near	= 2,
	}ENUMSHOOTSCOPE;

	CGPAimer();
	~CGPAimer();

	BOOL Init(CGPControl*);
	BOOL UnInit();

	void Update(DWORD);
	IGPTarget*	GetTarget();

	void OnCtrlPress();
	void OnCtrlRelease();

	void SetAimed(DWORD dwDestID, BOOL bSet = TRUE);
	void ShowAim(ENUMSHOOTSCOPE, IGPTarget* pTarget = NULL, BOOL bShow = TRUE);

private:
	void _StartAim();
	void _EndAim();
	void _PrepareEffect();
	NiPoint3 _GetAimLocalOriginPos(IGPTarget*);
	NiPoint3 _GetAimWorldOriginPos(IGPTarget*);
	NiPoint3 _GetAimWorldDirection(IGPTarget*);
	NiPoint3 _GetAimedLocalOriginPos(IGPTarget*);
	NiPoint3 _GetAimedWorldOriginPos(IGPTarget*);
	ENUMSHOOTSCOPE _JudgeTargetScope(NiPoint3 kOrigin, NiPoint3 kDir, NiPoint3 kTarget);

private:
	BOOL		m_bAimming;		//�Ƿ�������׼
	BOOL		m_bAimShow;		//�Ƿ�����ʾ��׼��ͷ
	ENUMSHOOTSCOPE	m_ShootScope;
	IGPTarget*	m_pTarget;		//��ǰ��׼Ŀ��
	DWORD		m_dwTargetTime;	//��ǰĿ���Ѿ�����׼�˶��ʱ���ˣ����룩
	CGPControl*	m_pControl;		
	RotateController*	m_pkRotateController;
	ScaleController*	m_pkScaleController;
	ScaleController*	m_pkMeScaleController;
	DWORD		m_dwLastTime;
	IGPShelterIterator*	m_pShelterIter;
	NiBillboardNodePtr	m_spBillboard1;	//��׼��ͷ��Ч����Ҫ���κ�ʱ���ž�ͷ����
	NiBillboardNodePtr	m_spBillboard2; //������������Ҽ��ϱ���׼��ͷ��
	NiSourceTexturePtr	m_pTextures[AIM_MAX_EFFECTS]; //��׼��ͷ�л��õ���ͼ����ɫ����ɫ����ɫ��
};

#endif