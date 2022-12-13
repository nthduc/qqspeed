////////////////////////////////////////////////
//     FileName:    gpcontrol.h
//     Author:      Fish
//     Date:        2007-1-18 14:37:27
//     Description: 道具整体逻辑实现文件
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
道具使用总控逻辑，包括：
1、使用道具触发
2、道具生命期管理
3、道具作用互斥条件处理
4、道具生命期表现处理
*/
class CGPControl:
	public CSingleT<CGPControl>,
	public CGamePropCollisionReport
{
public:
	CGPControl();
	~CGPControl();

	//必备函数
	BOOL AppInit();
	BOOL AppUnInit();

	//更新
	void Update(DWORD);
	
	//响应Ctrl键被按下
	void OnCtrlPress();

	//相应Ctrl键被松开
	void OnCtrlRelease();

	//OnAimFinished：这个函数给CGPAimer在结束瞄准的时候回调
	void OnAimFinished(IGPTarget*);	
	
	//OnTargetAimed：在某个目标被瞄准的时候调用，bSet用来表示是瞄准还是取消瞄准
	void OnTargetAimed(DWORD dwDestID, BOOL bSet);

	//OnUseGP：CNotifyUsePropPara消息来了调这个
	void OnUseGP(GP_ID id, DWORD dwSrcID, DWORD dwDestID = 0, bool bSelfDefineTarget = false,IGPTarget * pTarget = NULL );

	//OnTriggerByGP:CNotifyPropEffectResultPara来了调这个
	void OnTriggerByGP(GP_ID id, DWORD dwDestID);

	//SetAimed：让目标处于被瞄准状态
	void SetAimed(DWORD dwDestID, BOOL bSet);

	//ToggleAffect:打开或关闭作用开关，如果作用开关被关闭，所有效果将被禁止
	BOOL ToggleAffect(BOOL bToggle);

	//ToggleOp：打开或关闭操作开关，如果操作开关被关闭，所有操作将被禁止
	BOOL ToggleOp(BOOL);

	//OnGotten：得到了道具，给个机会播放吃到的特效
	void OnGotten(DWORD id, DWORD dwSrcID);

	//扔出道具，给个机会播放特效之类
	void OnThrow(DWORD id, DWORD dwSrcID, BOOL bThrow = TRUE);

	//道具击中了目标，给个机会播放特效之类
	void OnTrigger(LPGP_DATA pData);

	//道具被排斥了，给个机会干别的
	void OnDisabled(LPGP_DATA pData);

	//道具生命期结束了，给个机会干别的
	void OnDie(LPGP_DATA pData);

	//防御型道具防御了一次攻击，给个机会播放特效之类
	void OnProtect(DWORD id, DWORD dwSrcID);

	//治疗型道具治疗成功，播放特效
	void OnCure(DWORD id, DWORD dwSrcID);

	//响应系统消息
	STDMETHOD(WndProc)(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	//FindConfig: 找到对应道具的配置信息
	LPGP_CONFIG FindConfig(DWORD id);

	//道具生成型的道具需要这个
	BOOL		ProcessGeneratorGP(GP_ID id, IGPTarget* pSrcTarget);	

	//生成新道具
	BOOL	GenerateNewGP(GP_ID id, DWORD dwNewID, IGPTarget*);

	//GetTargetIterator:
	IGPTargetIterator* GetTargetIterator();

	//GetTargetByID
	IGPTarget*	GetTargetByID(DWORD id);	//通过TargetID取得Target

	//CGamePropCollisionReport
	virtual int OnTrigger(LPVOID lpData,LPVOID lpHitObjData);

	//Draw
	void Draw(NiCamera*);

	//重置资源
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
	void	_UpdateBlendState(IGPTarget* pTarget, GP_DATA_LIST&, GP_DATA_LIST_Iterator&);	//通过互斥条件来设置状态
	void	_UpdateGPMove(LPGP_DATA, DWORD dwTime);
	void	_PlaySpecialEffect(IGPTarget*, GP_SPECIAL_EFFECT_LIST&);
	DWORD	_GetAffectsTotalTime(GP_AFFECT_LIST&);	//作用总时间
	IGPTarget*	_GetTargetByIndex(DWORD dwIndex);//通过名次取到Target
	DWORD		_GetIndexInTeam(IGPTarget*);	//取得在自己队伍内的名次
	GPTARGET_LIST	_FetchTargetList(DWORD id, IGPTarget*);	//搜索到目标集合
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
瞄准境
实现方式概述：
1、瞄准镜原点在车身位置靠前的一个固定位置（可通过脚本配置）。
2、瞄准方向是当前相机的朝向。
3、判断从瞄准境原点出发沿瞄准方向上的目标的距离和角度，分为远距离、
	中距离和近距离。
4、如果有多个目标在近距离内，给每个目标一个瞄准时间，依次瞄准
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
	BOOL		m_bAimming;		//是否正在瞄准
	BOOL		m_bAimShow;		//是否在显示瞄准镜头
	ENUMSHOOTSCOPE	m_ShootScope;
	IGPTarget*	m_pTarget;		//当前瞄准目标
	DWORD		m_dwTargetTime;	//当前目标已经被瞄准了多久时间了（毫秒）
	CGPControl*	m_pControl;		
	RotateController*	m_pkRotateController;
	ScaleController*	m_pkScaleController;
	ScaleController*	m_pkMeScaleController;
	DWORD		m_dwLastTime;
	IGPShelterIterator*	m_pShelterIter;
	NiBillboardNodePtr	m_spBillboard1;	//瞄准镜头特效（需要在任何时候朝着镜头方向）
	NiBillboardNodePtr	m_spBillboard2; //用来给其他玩家加上被瞄准镜头的
	NiSourceTexturePtr	m_pTextures[AIM_MAX_EFFECTS]; //瞄准镜头切换用的贴图（绿色、黄色、红色）
};

#endif