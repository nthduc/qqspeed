////////////////////////////////////////////////
//     FileName:    gpdefine.h
//     Author:      Fish
//     Date:        2007-1-18 14:16:08
//     Description: 道具定义头文件，设计思路请参考
//					<<卡丁车道具赛总体设计.doc>>
////////////////////////////////////////////////

#ifndef _GPDEFINE_H_
#define _GPDEFINE_H_

#include "string"
#include "list"
using namespace std;

interface IGPTarget;
interface IGPAffect;
interface IGPTargetIterator;

//道具使用类型
typedef enum enmUseType
{
	EUT_Active,		//主动使用型
	EUT_Passive,	//被动使用型
}ENMUSETYPE;

//道具作用类型
typedef enum enmAffectType
{
	EAT_Attack		,	//攻击型道具
	EAT_Protect		,	//防护型道具
	EAT_Generator	,	//道具生成型道具
	EAT_Cure		,	//治疗
}ENMAFFECTTYPE;

//道具作用目标类型
typedef enum enmTargetType
{
	ETT_Play,	//所有人
	ETT_Enemy,	//所有敌人
	ETT_FirstPlay,	//第一名
	ETT_PrevPlay,	//前一名
	ETT_PrevEnemy,	//前一名敌人
	ETT_Me,			//自己
}ENMTARGETTYPE;

//道具攻击时的选择目标方式
typedef enum enmSelectTargetType
{
	ESTT_NoSelect,		//不需要瞄准，直接使用
	ESTT_Select			//需要瞄准才能使用
}ENMSELECTTARGETTYPE;

//防护型道具的防护方式
typedef enum enmProtectType
{
	EPT_AllAttack,		//对所有攻击免疫
	EPT_All,			//对所有攻击和防护都免疫
	EPT_Specified		//免疫特定道具
}ENMPROTECTTYPE;

//防护型道具的作用范围
typedef enum enmProtectTargetType
{
	EPTT_Me		= 100,			//保护自己
	EPTT_Team	= 101//保护整个队伍
}ENMPROTECTTARGETTYPE;

//运动类型
typedef enum enmMoveType
{
	EMT_None,		//无运动过程（绝大多数道具的默认行为）
	EMT_Missile,	//导弹
	EMT_Flying,		//水苍蝇轨迹
}ENMMOVETYPE;

//被动型道具特有的属性
typedef enum enmStaticProp
{
	ESP_None,
	ESP_Shelter,		//遮挡（迷雾使用）
}ENMSTATICPROP;

//多道具互斥模式
typedef enum enmBlendMode
{
	EBM_AddNext = 0,		//本道具作用尚未结束，如果有后一个道具来作用，不停止当前作用，让下一个道具生效，作用叠加
	EBM_UseNext = 1,		//本道具作用尚未结束，如果有后一个道具来作用，马上停止当前作用，让下一个道具生效
	EBM_IgnoreNext = 2,		//本道具作用尚未结束，忽略下一个道具作用
	EBM_ForceReplaceOld	= 3 //前一个道具作用尚未结束时，本道具来到，强制结束上一个道具的作用，即使前一个道具是EBM_IgnoreNext的。
}ENMBLENDMODE;

//空间位置
typedef enum enmPositionType
{
	EPT_Relative,	//相对位置
	EPT_Absolute	//绝对位置
}ENMPOSITIONTYPE;

//支持的包围盒类型
typedef enum enmBoundType
{
	EBT_Sphere,	
	EBT_Box,
}ENMBOUNDTYPE;

//作用效果分类，这里的每个效果对应于一个IGPAffect实现。
typedef enum enmAffectID
{
	EAI_None		= 0,	//空占位
	EAI_Lift		= 1,	//被抬起，参数：被抬起的高度
	EAI_Magnet		= 2,	//磁铁
	EAI_Stop		= 3,	//停止
	EAI_InvertLR	= 4,	//左右方向键反置，参数：TRUE（设置），FALSE（取消）
	EAI_FreezeGP	= 5,	//不允许使用道具
	EAI_FreezeOp	= 6,	//不运行任何操作
	EAI_No2			= 7,	//氮气
	EAI_Animation	= 8,	//播放动作，参数：动作ID
	EAI_Effect		= 9,	//参数：effect file
	EAI_Sound		= 10,	//参数：声音文件
	EAI_Speed		= 11,	//加减速
	EAI_Jet			= 12,	//喷火
	EAI_Evil		= 13,	//魔王发光体
	EAI_Generator	= 14,	//生成新道具
	EAI_No2Emitter	= 15,	//小喷触发器
	EAI_MAX			= 15,
}ENMAFFECTID;

typedef enum enmSpecialEffect
{
	ESE_None,		
	ESE_Effect,		//特效
	ESE_Sound,		//声音
	ESE_Animation	//动作
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
	ECUC_None,		//使用不受任何限制
	ECUC_OneExists	= 1,	//已经有一个道具正在作用的时候
	ECUC_NoAccel	= 2,	//不在加速的时候
}ENMCANNOTUSECONDITION;

typedef enum enumAffectOption
{
	EAO_None,
	EAO_AffectMeInstead = 1,		//不作用给别人，反而作用于自己
	EAO_AffectPlayInstead = 2,		//不作用给车，作用给人
	EAO_FaceToPeer	= 4,			//朝向别人（磁铁特效）
	EAO_AttachToWorld	= 8,		//特效绑定到世界中（默认是人身上）
}ENUMAFFECTOPTION;

//特效定义
typedef struct tagGP_SPECIAL_EFFECT
{
	DWORD	dwType;		//enmSpecialEffect
	DWORD	dwParam;	//参数
}GP_SPECIAL_EFFECT, *PGP_SPECIAL_EFFECT, *LPGP_SPECIAL_EFFECT;

//监视相机定义
typedef struct tagGP_WATCH_CAMERA
{
	string	strCameraName;
}GP_WATCH_CAMERA, *PGP_WATCH_CAMERA, *LPGP_WATCH_CAMERA;

//位置定义，用于定义新道具的产生位置
typedef struct tagGP_POSITION
{
	DWORD	dwType;		//enmPositionType
	float	x;
	float	y;
	float	z;
}GP_POSITION, *PGP_POSITION, *LPGP_POSITION;

//包围体定义
typedef struct tagGP_BOUND
{
	DWORD	dwType;	// box, sphere
	float	x;
	float	y;
	float	z;
}GP_BOUND, *PGP_BOUND, *LPGP_BOUND;

//作用属性集定义
typedef struct tagGP_AFFECT
{
	DWORD	dwID;			//enmAffectID
	float	fParam;
	DWORD	dwOption;
	DWORD	dwStartTime;	//开始等待时间
	DWORD	dwLifeTime;		//作用时间
}GP_AFFECT, *PGP_AFFECT, *LPGP_AFFECT;

typedef struct tagGP_AFFECTDATA
{
	LPGP_AFFECT		pAffectConfig;	//保留一份指针，方便操作
	IGPAffect*		pAffect;		//实现接口
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
	DWORD dwOriginID;	//有Avatar导致的道具转换，A对B使用道具P，如果B有特定Avatar，A算出转换几率，得出转换后的结果道具Q发给B，这个字段用来记录原始道具P
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

//道具定义，跟配置文件对应，每一类道具有一个实例
typedef struct tagGP_CONFIG
{
	//身份、类型标志
	DWORD		dwID;
	DWORD		dwParentID;
	string		strName;
	DWORD		dwCategory;
	DWORD		dwUseType;		//enmUseType
	DWORD		dwAffectType;	//enmAffectType
	DWORD		dwBlendMode;	//enmBlendMode
	DWORD		dwCannotUseCondition;	//enmCannotUseCondition
	DWORD		dwStopCondition;		//enmCannotUseCondition

	//生命期表现
	GP_SPECIAL_EFFECT_LIST	listOriginEffect;
	GP_SPECIAL_EFFECT_LIST	listEattenEffect;
	GP_SPECIAL_EFFECT_LIST	listThrowEffect;
	GP_SPECIAL_EFFECT_LIST	listTriggerEffect;
	GP_SPECIAL_EFFECT_LIST	listProtectEffect;
	GP_SPECIAL_EFFECT_LIST	listCureEffect;

	//运动方式
	GP_MOVECTRL	stMoveCtrl;

	//被动型道具需要一个模型（大水泡、香蕉）
	DWORD		dwStayNode;		//
	DWORD		dwStayTime;

	//攻击型道具属性
	DWORD		dwTargetType;	//enmTargetType
	DWORD		dwSelectTargetType;	//enmSelectTargetType
	BOOL		bAffectSelf;
	BOOL		bIgnoreProtect;
	
	//防护型道具属性
	DWORD		dwProtectType;		//enmProtectType
	DWORD		dwProtectCount;		//0表示无限次，其他数字n表示n次
	DWORD		dwProtectIndex;		//enmProtectType为EPT_Specified时表示道具ID
	DWORD		dwProtectTarget;	//enmProtectTargetType

	//被动使用道具属性
	GP_BOUND	stBound;
	GP_POSITION	stPosition;
	DWORD		dwPassCount;	//使用次数
	DWORD		dwStaticProp;	//enmStaticProp

	//治疗道具属性
	DWORD		dwCureID;

	//道具生成型道具属性
	DWORD		dwNewID;

	//时间控制
	DWORD		dwReadyTime;	//从开始使用到击中的时间间隔，毫秒
	DWORD		dwBirthTime;
	DWORD		dwTriggerTime;	//击中的持续时间（水苍蝇）做了这个时间才开始作用

	//图标信息
	string		strIconTex;
	DWORD		dwCameraID;
	//道具作用列表
	GP_AFFECT_LIST	listAffect;

}GP_CONFIG, *PGP_CONFIG, *LPGP_CONFIG;

typedef list<GP_CONFIG>		GP_CONFIG_LIST;
typedef GP_CONFIG_LIST::iterator GP_CONFIG_LIST_Iterator;

interface IGPMovementControl;
//道具运行时数据
typedef struct tagGP_DATA
{
	DWORD		dwID;
	DWORD		dwIndex;
	DWORD		dwOriginID;
	DWORD		dwSrcID;		//道具使用者
	DWORD		dwDestID;		//道具被作用者
	DWORD		dwUseTime;		//使用的时间，毫秒
	DWORD		dwUseCount;		//已经攻击/防御的次数
	DWORD		dwLifeState;	//enmLifeState
	DWORD		dwData;
	LPGP_CONFIG	pConfig;
	IGPMovementControl*	pMoveCtrl;
	GP_AFFECTDATA_LIST	listAffect;	//对应于道具作用列表
}GP_DATA, *PGP_DATA, *LPGP_DATA;

typedef list<GP_DATA>		GP_DATA_LIST;
typedef GP_DATA_LIST::iterator GP_DATA_LIST_Iterator;

/*
*	道具作用目标抽象：
*	目前等价于Play或Kart，可在Play里实现
*/
interface IGPTarget
{
	//身份信息
	virtual DWORD	GetType() = 0;
	virtual DWORD	GetID() = 0;
	virtual DWORD	GetIndex() = 0;
	virtual DWORD	GetTeam() = 0;

	//位置信息
	virtual NiPoint3	GetPos() = 0;
	virtual void		SetPos(const NiPoint3&) = 0;
	virtual NiPoint3	GetPhysXPos() = 0;
	virtual void		SetPhysXPos(const NiPoint3&) = 0;
	virtual NiPoint3	GetVelocity(BOOL bLocal = FALSE) = 0;
	virtual void		ZeroVelocity() = 0;
	virtual NiPoint3	GetForward() = 0;
	virtual NiTransform GetTransform() = 0;
	virtual NiNode*		GetNode() = 0;

	//特效功能
	virtual BOOL	Attach(const char* effect, const char* node = NULL) = 0;
	virtual BOOL	Detach(const char* node) = 0;
	virtual BOOL	Attach(NiAVObject*) = 0;
	virtual BOOL	Detach(NiAVObject*) = 0;
	virtual BOOL	PlayAnimation(DWORD AnimID, BOOL bPlay = FALSE) = 0;
	virtual BOOL	StopAnimation(DWORD AnimID, BOOL bPlay = FALSE) = 0;

	//物理功能
	//施加力（磁铁）
	virtual void	AddForce(float force[3], BOOL bLocal = FALSE) = 0;
	//控制速度，fRatio是比率
	virtual void	ControlSpeed(float fRatio) = 0;

	//切换物理模拟状态（腾空不受外力）
	virtual BOOL	ToggleSim(BOOL) = 0;

	//拥有的道具
	virtual GP_ID_LIST&	GetGPs() = 0;
	virtual void		AddGP(DWORD id, DWORD index) = 0;
	virtual void		RemoveGP(DWORD id) = 0;

	//起作用的道具
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
*	道具作用目标迭代器：
*	枚举所有可作用的目标对象
*/
interface IGPTargetIterator
{
	virtual DWORD		GetTargetCount() = 0;
	virtual IGPTarget*	GetTarget(DWORD index) = 0;
	virtual IGPTarget*	GetHost() = 0;	//取得本地玩家
	virtual IGPTarget*	GetDummy() = 0;
};

BOOL CreateGPTargetIterator(IGPTargetIterator** ppv);
void DestroyGPTargetIterator(IGPTargetIterator* pv);

class CGPControl;
/*
*	单属性体接口
*	每个道具对应一系列效果，控制器通过时间和互斥关系来控制这些效果
*	何时开始、合适灭亡，并在活动的时候更新
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
*	运动轨迹控制（导弹...）
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
*	枚举障碍物（迷雾），用来禁止瞄准器
*/
interface IGPShelterIterator
{
	virtual DWORD	GetShelterCount();
	virtual void	GetShelterBoundingBox(NxVec3& vecMin, NxVec3& vecMax);
};
BOOL CreateGPShelterIterator(IGPShelterIterator**);
void DestroyGPShelterIterator(IGPShelterIterator*);

#endif	//_GPDEFINE_H_