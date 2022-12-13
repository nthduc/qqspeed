////////////////////////////////////////////////
//     FileName:    TaskDefine.h
//     Author:      rayhuang
//     Date:        2007-4-24
//     Description: 任务参数定义
////////////////////////////////////////////////
#pragma once
#include "variant.h"
#include <list>
#include <string>
using namespace std;

//原子类型
typedef enum enmAtomType
{
	EAMT_DialoggBox,	//对话框
	EAMT_ScreenTex,		//屏幕贴图
	EAMT_BlinkTex,		//闪耀的贴图
	EAMT_Effect,		//特效
	EAMT_Anim,			//动画
	EAMT_Sound,			//声音
	EAMT_ScreenEff,		//屏幕特效（由CGameEffectMgt激发）
	EAMT_LockKey,		//锁键盘
	EAMT_CDTimerShow,	//显示倒计时时间
	EAMT_KeyBlink,		//键盘闪烁
	EAMT_StartNo2,		//激活小喷
	EAMT_ScreenFont,	//屏幕字体
	EAMT_Esc,			//Esc按键，请求服务器完成任务
	EAMT_SwitchTex,		//类似于开关的原子，start时显示某一tex，stop时显示另外的tex
}ENMATOMTYPE;


//监测器类型
typedef enum enmDetectorType
{
	EDT_Timer,			//计时器
	EDT_BNPress,		//按键检测
	EDT_BNPressTime,	//按键时间检测
	EDT_ExecuteCount,	//执行次数检测
	EDT_CDTimer,		//倒计时
	EDT_Speed,			//检测速度是否达到某一值
	EDT_Drift_Speed,	//检测是否漂移并且速度在某一值之上
	EDT_TurnCar_Drift,	//检测是否回搬车头并且当时处于漂移状态
	EDT_SmallNo2,		//检测小喷是否处于激发状态
}ENMDETECTORTYPE;

//检测器状态
typedef enum enmDetectorState
{
	EDS_Prepair,
	EDS_Executing,
	EDS_Success,
	EDS_Fail,
}ENMDETECTORSTATE;


//任务的接受状态
typedef enum enmTaskAccessState
{	
	ETAS_Available,			//可玩的（已接的）
	ETAS_Unavailable,		//不可玩的（未接的）
}ENMTASTACCESSSTATE;

//任务的完成状态
typedef enum enmTaskAchieveState
{
	ETST_Finish,		//完成
	ETST_Unfinish,		//未完成
}ENMTASKACHIEVESTATE;

//任务信息
typedef struct tagTaskInfo
{
	DWORD dwID;				//任务id
	DWORD dwAccessState;	//任务的可接受状态
	DWORD dwAchieveState;	//任务的完成状态
}TASKINFO, *PTASKINFO;
typedef list<TASKINFO> TASKINFO_LIST;
typedef list<TASKINFO>::iterator TASKINFO_LIST_IT;


typedef struct tagTaskPackCfg
{
	DWORD		dwPackID;	//任务包id
	list<DWORD> listTask;	//任务列表
}TASKPACKCFG, *PTASKPACKCFG;

typedef list<TASKPACKCFG> TASKPACKCFG_LIST;
typedef list<TASKPACKCFG>::iterator TASKPACKCFG_LIST_IT;

//原子配置
typedef struct tagAtomCfg
{
	DWORD dwType;		//原子类型
	DWORD dwParam;		//原子的id
	DWORD dwOption;		//额外参数
}ATOMCFG;

typedef list<ATOMCFG> ATOMCFG_LIST;
typedef list<ATOMCFG>::iterator ATOMCFG_LIST_IT;

typedef struct tagAtomLevelCfg
{
	DWORD			dwDetectorID;	//检测器id
	ATOMCFG_LIST	listAtom;		//在同一阶段的原子列表
}ATOMLEVELCFG, *PATOMLEVELCFG;

typedef list<ATOMLEVELCFG> ATOMLEVELCFG_LIST;
typedef list<ATOMLEVELCFG>::iterator ATOMLEVELCFG_LIST_IT;

//任务执行的类型
typedef enum enmProcessType
{
	EPT_NeverBack,		//永不回头的
	EPT_Circle,			//失败可回头重做的	
}ENMPROCESSTYPE;


typedef struct tagSubTaskCfg
{
	DWORD				dwValidation;			//子任务完成上报给服务器的检验码
	DWORD				dwProcessType;			//子任务执行的类型
	ATOMCFG_LIST		listSubTaskPeriodAtom;	//子任务期间都存在的原子
	ATOMLEVELCFG_LIST	listAtomLevel;			//原子层列表
}SUBTASKCFG, *PSUBTASKCFG;

typedef list<SUBTASKCFG> SUBTASKCFG_LIST;
typedef list<SUBTASKCFG>::iterator SUBTASKCFG_LIST_IT;


typedef struct tagSwitchTable
{

	bool bAccePlaneOff;			//速度计开关
	bool bCountDownTimeOff;		//倒计时开关
	bool bEagleMapOff;			//小地图开关
	bool bElapseTimeOff;		//比赛时间开关	
	bool bItemColumnOff;		//道具栏开关
	bool bPlayerRankOff;		//排名列表开关	
	bool bSinglePowerCtrlOff;	//氮气条控制开关
	bool bTeamPowerCtrlOff;		//氮气条控制开关
	bool bResetOff;				//复位开关（逆行提示）
}SWITCHTABLE;

typedef list<DWORD> PREKEY_LIST;		//任务任务开始的条件（钥匙）列表
typedef list<DWORD>::iterator PREKEY_LIST_IT;


typedef struct tagTaskConfig
{
	DWORD			dwID;				//任务id
	DWORD			dwMapID;			//场景id
	string			strDesc;			//任务描叙
	PREKEY_LIST		listPreKey;			//任务任务开始的条件（钥匙）列表
	SWITCHTABLE		kSwitchTable;		//任务相关的开关表
	ATOMCFG_LIST	listTaskPeriodAtom;	//任务期间都存在的原子
	SUBTASKCFG_LIST listSubTask;		//子任务列表
}TASKCONFIG, *PTASKCONFIG;

typedef list<TASKCONFIG> TASKCONFIG_LIST;
typedef list<TASKCONFIG>::iterator TASKCONFIG_LIST_IT;