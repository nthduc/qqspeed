////////////////////////////////////////////////
//     FileName:    gpdefine.h
//     Author:      Fish
//     Date:        2007-1-18 14:16:08
//     Description: ���߶���ͷ�ļ������˼·��ο�
//					<<�������������������.doc>>
////////////////////////////////////////////////

#ifndef _GPDEFINE_H_
#define _GPDEFINE_H_

#include "string"
#include "list"
using namespace std;

interface IGPTarget;
interface IGPAffect;
interface IGPTargetIterator;

//����ʹ������
typedef enum enmUseType
{
	EUT_Active,		//����ʹ����
	EUT_Passive,	//����ʹ����
}ENMUSETYPE;

//������������
typedef enum enmAffectType
{
	EAT_Attack		,	//�����͵���
	EAT_Protect		,	//�����͵���
	EAT_Generator	,	//���������͵���
	EAT_Cure		,	//����
}ENMAFFECTTYPE;

//��������Ŀ������
typedef enum enmTargetType
{
	ETT_Play,	//������
	ETT_Enemy,	//���е���
	ETT_FirstPlay,	//��һ��
	ETT_PrevPlay,	//ǰһ��
	ETT_PrevEnemy,	//ǰһ������
	ETT_Me,			//�Լ�
}ENMTARGETTYPE;

//���߹���ʱ��ѡ��Ŀ�귽ʽ
typedef enum enmSelectTargetType
{
	ESTT_NoSelect,		//����Ҫ��׼��ֱ��ʹ��
	ESTT_Select			//��Ҫ��׼����ʹ��
}ENMSELECTTARGETTYPE;

//�����͵��ߵķ�����ʽ
typedef enum enmProtectType
{
	EPT_AllAttack,		//�����й�������
	EPT_All,			//�����й����ͷ���������
	EPT_Specified		//�����ض�����
}ENMPROTECTTYPE;

//�����͵��ߵ����÷�Χ
typedef enum enmProtectTargetType
{
	EPTT_Me		= 100,			//�����Լ�
	EPTT_Team	= 101//������������
}ENMPROTECTTARGETTYPE;

//�˶�����
typedef enum enmMoveType
{
	EMT_None,		//���˶����̣�����������ߵ�Ĭ����Ϊ��
	EMT_Missile,	//����
	EMT_Flying,		//ˮ��Ӭ�켣
}ENMMOVETYPE;

//�����͵������е�����
typedef enum enmStaticProp
{
	ESP_None,
	ESP_Shelter,		//�ڵ�������ʹ�ã�
}ENMSTATICPROP;

//����߻���ģʽ
typedef enum enmBlendMode
{
	EBM_AddNext = 0,		//������������δ����������к�һ�����������ã���ֹͣ��ǰ���ã�����һ��������Ч�����õ���
	EBM_UseNext = 1,		//������������δ����������к�һ�����������ã�����ֹͣ��ǰ���ã�����һ��������Ч
	EBM_IgnoreNext = 2,		//������������δ������������һ����������
	EBM_ForceReplaceOld	= 3 //ǰһ������������δ����ʱ��������������ǿ�ƽ�����һ�����ߵ����ã���ʹǰһ��������EBM_IgnoreNext�ġ�
}ENMBLENDMODE;

//�ռ�λ��
typedef enum enmPositionType
{
	EPT_Relative,	//���λ��
	EPT_Absolute	//����λ��
}ENMPOSITIONTYPE;

//֧�ֵİ�Χ������
typedef enum enmBoundType
{
	EBT_Sphere,	
	EBT_Box,
}ENMBOUNDTYPE;

//����Ч�����࣬�����ÿ��Ч����Ӧ��һ��IGPAffectʵ�֡�
typedef enum enmAffectID
{
	EAI_None		= 0,	//��ռλ
	EAI_Lift		= 1,	//��̧�𣬲�������̧��ĸ߶�
	EAI_Magnet		= 2,	//����
	EAI_Stop		= 3,	//ֹͣ
	EAI_InvertLR	= 4,	//���ҷ�������ã�������TRUE�����ã���FALSE��ȡ����
	EAI_FreezeGP	= 5,	//������ʹ�õ���
	EAI_FreezeOp	= 6,	//�������κβ���
	EAI_No2			= 7,	//����
	EAI_Animation	= 8,	//���Ŷ���������������ID
	EAI_Effect		= 9,	//������effect file
	EAI_Sound		= 10,	//�����������ļ�
	EAI_Speed		= 11,	//�Ӽ���
	EAI_Jet			= 12,	//���
	EAI_Evil		= 13,	//ħ��������
	EAI_Generator	= 14,	//�����µ���
	EAI_No2Emitter	= 15,	//С�紥����
	EAI_MAX			= 15,
}ENMAFFECTID;

typedef enum enmSpecialEffect
{
	ESE_None,		
	ESE_Effect,		//��Ч
	ESE_Sound,		//����
	ESE_Animation	//����
}ENMSPECIALEFFECT;

typedef enum enmAffectState
{
	EAS_Idle,
	EAS_Playing,
	EAS_Die
}ENMAFFECTSTATE;

typedef enum enmLifeState
{
	ELS_Throw,
	ELS_Trigger,
	ELS_Disbled,
	ELS_Die,
}ENMLIFESTATE;

typedef enum enmCannotUseCondition
{
	ECUC_None,		//ʹ�ò����κ�����
	ECUC_OneExists	= 1,	//�Ѿ���һ�������������õ�ʱ��
	ECUC_NoAccel	= 2,	//���ڼ��ٵ�ʱ��
}ENMCANNOTUSECONDITION;

typedef enum enumAffectOption
{
	EAO_None,
	EAO_AffectMeInstead = 1,		//�����ø����ˣ������������Լ�
	EAO_AffectPlayInstead = 2,		//�����ø��������ø���
	EAO_FaceToPeer	= 4,			//������ˣ�������Ч��
	EAO_AttachToWorld	= 8,		//��Ч�󶨵������У�Ĭ���������ϣ�
}ENUMAFFECTOPTION;

//��Ч����
typedef struct tagGP_SPECIAL_EFFECT
{
	DWORD	dwType;		//enmSpecialEffect
	DWORD	dwParam;	//����
}GP_SPECIAL_EFFECT, *PGP_SPECIAL_EFFECT, *LPGP_SPECIAL_EFFECT;

//�����������
typedef struct tagGP_WATCH_CAMERA
{
	string	strCameraName;
}GP_WATCH_CAMERA, *PGP_WATCH_CAMERA, *LPGP_WATCH_CAMERA;

//λ�ö��壬���ڶ����µ��ߵĲ���λ��
typedef struct tagGP_POSITION
{
	DWORD	dwType;		//enmPositionType
	float	x;
	float	y;
	float	z;
}GP_POSITION, *PGP_POSITION, *LPGP_POSITION;

//��Χ�嶨��
typedef struct tagGP_BOUND
{
	DWORD	dwType;	// box, sphere
	float	x;
	float	y;
	float	z;
}GP_BOUND, *PGP_BOUND, *LPGP_BOUND;

//�������Լ�����
typedef struct tagGP_AFFECT
{
	DWORD	dwID;			//enmAffectID
	float	fParam;
	DWORD	dwOption;
	DWORD	dwStartTime;	//��ʼ�ȴ�ʱ��
	DWORD	dwLifeTime;		//����ʱ��
}GP_AFFECT, *PGP_AFFECT, *LPGP_AFFECT;

typedef struct tagGP_AFFECTDATA
{
	LPGP_AFFECT		pAffectConfig;	//����һ��ָ�룬�������
	IGPAffect*		pAffect;		//ʵ�ֽӿ�
	DWORD			dwState;		//enmAffectState
}GP_AFFECTDATA, *PGP_AFFECTDATA, *LPGP_AFFECTDATA;

typedef struct tagGP_ID
{
	tagGP_ID()
	{
		dwID = 0;
		dwIndex = 0;
		dwOriginID = 0;
	}
	tagGP_ID(DWORD id, DWORD index, DWORD originid)
	{
		dwID = id;
		dwIndex = index;
		dwOriginID = originid;
	}
	DWORD dwID;
	DWORD dwIndex;
	DWORD dwOriginID;	//��Avatar���µĵ���ת����A��Bʹ�õ���P�����B���ض�Avatar��A���ת�����ʣ��ó�ת����Ľ������Q����B������ֶ�������¼ԭʼ����P
}GP_ID, *PGP_ID, *LPGP_ID;

typedef list<GP_AFFECTDATA>	GP_AFFECTDATA_LIST;
typedef GP_AFFECTDATA_LIST::iterator GP_AFFECTDATA_LIST_Iterator;

typedef list<GP_SPECIAL_EFFECT>	GP_SPECIAL_EFFECT_LIST;
typedef GP_SPECIAL_EFFECT_LIST::iterator GP_SPECIAL_EFFECT_LIST_Iterator;

typedef list<GP_ID>			GP_ID_LIST;
typedef GP_ID_LIST::iterator GP_ID_LIST_Iterator;

typedef list<GP_AFFECT>		GP_AFFECT_LIST;
typedef GP_AFFECT_LIST::iterator GP_AFFECT_LIST_Iterator;

typedef list<IGPTarget*>	GP_TARGET_LIST;
typedef GP_TARGET_LIST::iterator GP_TARGET_LIST_Iterator;


typedef struct tagGP_MOVECTRL
{
	DWORD dwMoveType;
	DWORD dwStartTime;
	DWORD dwLifeTime;
	DWORD dwMoveNode;
	GP_POSITION	stSrcPosition;
	GP_POSITION	stDestPosition;
}GP_MOVECTRL, *PGP_MOVECTRL, *LPGP_MOVECTRL;

//���߶��壬�������ļ���Ӧ��ÿһ�������һ��ʵ��
typedef struct tagGP_CONFIG
{
	//��ݡ����ͱ�־
	DWORD		dwID;
	DWORD		dwParentID;
	string		strName;
	DWORD		dwCategory;
	DWORD		dwUseType;		//enmUseType
	DWORD		dwAffectType;	//enmAffectType
	DWORD		dwBlendMode;	//enmBlendMode
	DWORD		dwCannotUseCondition;	//enmCannotUseCondition
	DWORD		dwStopCondition;		//enmCannotUseCondition

	//�����ڱ���
	GP_SPECIAL_EFFECT_LIST	listOriginEffect;
	GP_SPECIAL_EFFECT_LIST	listEattenEffect;
	GP_SPECIAL_EFFECT_LIST	listThrowEffect;
	GP_SPECIAL_EFFECT_LIST	listTriggerEffect;
	GP_SPECIAL_EFFECT_LIST	listProtectEffect;
	GP_SPECIAL_EFFECT_LIST	listCureEffect;

	//�˶���ʽ
	GP_MOVECTRL	stMoveCtrl;

	//�����͵�����Ҫһ��ģ�ͣ���ˮ�ݡ��㽶��
	DWORD		dwStayNode;		//
	DWORD		dwStayTime;

	//�����͵�������
	DWORD		dwTargetType;	//enmTargetType
	DWORD		dwSelectTargetType;	//enmSelectTargetType
	BOOL		bAffectSelf;
	BOOL		bIgnoreProtect;
	
	//�����͵�������
	DWORD		dwProtectType;		//enmProtectType
	DWORD		dwProtectCount;		//0��ʾ���޴Σ���������n��ʾn��
	DWORD		dwProtectIndex;		//enmProtectTypeΪEPT_Specifiedʱ��ʾ����ID
	DWORD		dwProtectTarget;	//enmProtectTargetType

	//����ʹ�õ�������
	GP_BOUND	stBound;
	GP_POSITION	stPosition;
	DWORD		dwPassCount;	//ʹ�ô���
	DWORD		dwStaticProp;	//enmStaticProp

	//���Ƶ�������
	DWORD		dwCureID;

	//���������͵�������
	DWORD		dwNewID;

	//ʱ�����
	DWORD		dwReadyTime;	//�ӿ�ʼʹ�õ����е�ʱ����������
	DWORD		dwBirthTime;
	DWORD		dwTriggerTime;	//���еĳ���ʱ�䣨ˮ��Ӭ���������ʱ��ſ�ʼ����

	//ͼ����Ϣ
	string		strIconTex;
	DWORD		dwCameraID;
	//���������б�
	GP_AFFECT_LIST	listAffect;

}GP_CONFIG, *PGP_CONFIG, *LPGP_CONFIG;

typedef list<GP_CONFIG>		GP_CONFIG_LIST;
typedef GP_CONFIG_LIST::iterator GP_CONFIG_LIST_Iterator;

interface IGPMovementControl;
//��������ʱ����
typedef struct tagGP_DATA
{
	DWORD		dwID;
	DWORD		dwIndex;
	DWORD		dwOriginID;
	DWORD		dwSrcID;		//����ʹ����
	DWORD		dwDestID;		//���߱�������
	DWORD		dwUseTime;		//ʹ�õ�ʱ�䣬����
	DWORD		dwUseCount;		//�Ѿ�����/�����Ĵ���
	DWORD		dwLifeState;	//enmLifeState
	DWORD		dwData;
	LPGP_CONFIG	pConfig;
	IGPMovementControl*	pMoveCtrl;
	GP_AFFECTDATA_LIST	listAffect;	//��Ӧ�ڵ��������б�
}GP_DATA, *PGP_DATA, *LPGP_DATA;

typedef list<GP_DATA>		GP_DATA_LIST;
typedef GP_DATA_LIST::iterator GP_DATA_LIST_Iterator;

/*
*	��������Ŀ�����
*	Ŀǰ�ȼ���Play��Kart������Play��ʵ��
*/
interface IGPTarget
{
	//�����Ϣ
	virtual DWORD	GetType() = 0;
	virtual DWORD	GetID() = 0;
	virtual DWORD	GetIndex() = 0;
	virtual DWORD	GetTeam() = 0;

	//λ����Ϣ
	virtual NiPoint3	GetPos() = 0;
	virtual void		SetPos(const NiPoint3&) = 0;
	virtual NiPoint3	GetPhysXPos() = 0;
	virtual void		SetPhysXPos(const NiPoint3&) = 0;
	virtual NiPoint3	GetVelocity(BOOL bLocal = FALSE) = 0;
	virtual void		ZeroVelocity() = 0;
	virtual NiPoint3	GetForward() = 0;
	virtual NiTransform GetTransform() = 0;
	virtual NiNode*		GetNode() = 0;

	//��Ч����
	virtual BOOL	Attach(const char* effect, const char* node = NULL) = 0;
	virtual BOOL	Detach(const char* node) = 0;
	virtual BOOL	Attach(NiAVObject*) = 0;
	virtual BOOL	Detach(NiAVObject*) = 0;
	virtual BOOL	PlayAnimation(DWORD AnimID, BOOL bPlay = FALSE) = 0;
	virtual BOOL	StopAnimation(DWORD AnimID, BOOL bPlay = FALSE) = 0;

	//������
	//ʩ������������
	virtual void	AddForce(float force[3], BOOL bLocal = FALSE) = 0;
	//�����ٶȣ�fRatio�Ǳ���
	virtual void	ControlSpeed(float fRatio) = 0;

	//�л�����ģ��״̬���ڿղ���������
	virtual BOOL	ToggleSim(BOOL) = 0;

	//ӵ�еĵ���
	virtual GP_ID_LIST&	GetGPs() = 0;
	virtual void		AddGP(DWORD id, DWORD index) = 0;
	virtual void		RemoveGP(DWORD id) = 0;

	//�����õĵ���
	virtual GP_DATA_LIST&	GetAffectGPs() = 0;
	virtual void			AddAffectGP(LPGP_DATA) = 0;
	virtual void			RemoveAffectGP(DWORD id) = 0;
	virtual void			ClearAffectGPs() = 0;

	virtual BOOL	SetAimed(BOOL) = 0;
	virtual BOOL	GetAimed()	= 0;

	virtual BOOL	SetInvalidTarget(BOOL) = 0;
	virtual BOOL	GetInvalidTarget() = 0;
};

typedef list<IGPTarget*>	GPTARGET_LIST;
typedef GPTARGET_LIST::iterator GPTARGET_LIST_Iterator;

/*
*	��������Ŀ���������
*	ö�����п����õ�Ŀ�����
*/
interface IGPTargetIterator
{
	virtual DWORD		GetTargetCount() = 0;
	virtual IGPTarget*	GetTarget(DWORD index) = 0;
	virtual IGPTarget*	GetHost() = 0;	//ȡ�ñ������
	virtual IGPTarget*	GetDummy() = 0;
};

BOOL CreateGPTargetIterator(IGPTargetIterator** ppv);
void DestroyGPTargetIterator(IGPTargetIterator* pv);

class CGPControl;
/*
*	��������ӿ�
*	ÿ�����߶�Ӧһϵ��Ч����������ͨ��ʱ��ͻ����ϵ��������ЩЧ��
*	��ʱ��ʼ���������������ڻ��ʱ�����
*/
interface IGPAffect
{
	virtual void Init(CGPControl* pControl, GP_ID kGPID) = 0;
	virtual void SetTarget(IGPTarget* pTargetSrc, IGPTarget* pTargetDest) = 0;
	virtual void SetLifeTime(DWORD dwTime) = 0;
	virtual void SetParam(float) = 0;
	virtual void SetOption(DWORD dwOption) = 0;
	virtual void Start() = 0;
	virtual void Stop() = 0;
	virtual BOOL IsPlaying() = 0;
	virtual void Update(DWORD) = 0;
	virtual BOOL CanUnload() = 0;
};

BOOL CreateGPAffect(ENMAFFECTID, IGPAffect** ppv);
void DestroyGPAffect(ENMAFFECTID, IGPAffect* pv);

/*
*	�˶��켣���ƣ�����...��
*/
interface IGPMovementControl
{
	virtual void SetTarget(IGPTarget* pSrc, IGPTarget* pDest) = 0;
	virtual void SetPos(NiPoint3& kSrcPos, NiPoint3& kDestPos) = 0;
	virtual void SetParam(NiNodePtr, DWORD dwLife) = 0;	
	virtual void Update(DWORD) = 0;
	virtual void Start() = 0;
	virtual void Stop() = 0;
	virtual BOOL IsPlaying() = 0;
};

BOOL CreateGPMovementControl(ENMMOVETYPE, IGPMovementControl**);
void DestroyGPMovementControl(ENMMOVETYPE, IGPMovementControl*);

/*
*	ö���ϰ��������������ֹ��׼��
*/
interface IGPShelterIterator
{
	virtual DWORD	GetShelterCount();
	virtual void	GetShelterBoundingBox(NxVec3& vecMin, NxVec3& vecMax);
};
BOOL CreateGPShelterIterator(IGPShelterIterator**);
void DestroyGPShelterIterator(IGPShelterIterator*);

#endif	//_GPDEFINE_H_