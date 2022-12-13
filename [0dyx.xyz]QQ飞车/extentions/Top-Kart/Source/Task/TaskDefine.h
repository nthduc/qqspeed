////////////////////////////////////////////////
//     FileName:    TaskDefine.h
//     Author:      rayhuang
//     Date:        2007-4-24
//     Description: �����������
////////////////////////////////////////////////
#pragma once
#include "variant.h"
#include <list>
#include <string>
using namespace std;

//ԭ������
typedef enum enmAtomType
{
	EAMT_DialoggBox,	//�Ի���
	EAMT_ScreenTex,		//��Ļ��ͼ
	EAMT_BlinkTex,		//��ҫ����ͼ
	EAMT_Effect,		//��Ч
	EAMT_Anim,			//����
	EAMT_Sound,			//����
	EAMT_ScreenEff,		//��Ļ��Ч����CGameEffectMgt������
	EAMT_LockKey,		//������
	EAMT_CDTimerShow,	//��ʾ����ʱʱ��
	EAMT_KeyBlink,		//������˸
	EAMT_StartNo2,		//����С��
	EAMT_ScreenFont,	//��Ļ����
	EAMT_Esc,			//Esc����������������������
	EAMT_SwitchTex,		//�����ڿ��ص�ԭ�ӣ�startʱ��ʾĳһtex��stopʱ��ʾ�����tex
}ENMATOMTYPE;


//���������
typedef enum enmDetectorType
{
	EDT_Timer,			//��ʱ��
	EDT_BNPress,		//�������
	EDT_BNPressTime,	//����ʱ����
	EDT_ExecuteCount,	//ִ�д������
	EDT_CDTimer,		//����ʱ
	EDT_Speed,			//����ٶ��Ƿ�ﵽĳһֵ
	EDT_Drift_Speed,	//����Ƿ�Ư�Ʋ����ٶ���ĳһֵ֮��
	EDT_TurnCar_Drift,	//����Ƿ�ذᳵͷ���ҵ�ʱ����Ư��״̬
	EDT_SmallNo2,		//���С���Ƿ��ڼ���״̬
}ENMDETECTORTYPE;

//�����״̬
typedef enum enmDetectorState
{
	EDS_Prepair,
	EDS_Executing,
	EDS_Success,
	EDS_Fail,
}ENMDETECTORSTATE;


//����Ľ���״̬
typedef enum enmTaskAccessState
{	
	ETAS_Available,			//����ģ��ѽӵģ�
	ETAS_Unavailable,		//������ģ�δ�ӵģ�
}ENMTASTACCESSSTATE;

//��������״̬
typedef enum enmTaskAchieveState
{
	ETST_Finish,		//���
	ETST_Unfinish,		//δ���
}ENMTASKACHIEVESTATE;

//������Ϣ
typedef struct tagTaskInfo
{
	DWORD dwID;				//����id
	DWORD dwAccessState;	//����Ŀɽ���״̬
	DWORD dwAchieveState;	//��������״̬
}TASKINFO, *PTASKINFO;
typedef list<TASKINFO> TASKINFO_LIST;
typedef list<TASKINFO>::iterator TASKINFO_LIST_IT;


typedef struct tagTaskPackCfg
{
	DWORD		dwPackID;	//�����id
	list<DWORD> listTask;	//�����б�
}TASKPACKCFG, *PTASKPACKCFG;

typedef list<TASKPACKCFG> TASKPACKCFG_LIST;
typedef list<TASKPACKCFG>::iterator TASKPACKCFG_LIST_IT;

//ԭ������
typedef struct tagAtomCfg
{
	DWORD dwType;		//ԭ������
	DWORD dwParam;		//ԭ�ӵ�id
	DWORD dwOption;		//�������
}ATOMCFG;

typedef list<ATOMCFG> ATOMCFG_LIST;
typedef list<ATOMCFG>::iterator ATOMCFG_LIST_IT;

typedef struct tagAtomLevelCfg
{
	DWORD			dwDetectorID;	//�����id
	ATOMCFG_LIST	listAtom;		//��ͬһ�׶ε�ԭ���б�
}ATOMLEVELCFG, *PATOMLEVELCFG;

typedef list<ATOMLEVELCFG> ATOMLEVELCFG_LIST;
typedef list<ATOMLEVELCFG>::iterator ATOMLEVELCFG_LIST_IT;

//����ִ�е�����
typedef enum enmProcessType
{
	EPT_NeverBack,		//������ͷ��
	EPT_Circle,			//ʧ�ܿɻ�ͷ������	
}ENMPROCESSTYPE;


typedef struct tagSubTaskCfg
{
	DWORD				dwValidation;			//����������ϱ����������ļ�����
	DWORD				dwProcessType;			//������ִ�е�����
	ATOMCFG_LIST		listSubTaskPeriodAtom;	//�������ڼ䶼���ڵ�ԭ��
	ATOMLEVELCFG_LIST	listAtomLevel;			//ԭ�Ӳ��б�
}SUBTASKCFG, *PSUBTASKCFG;

typedef list<SUBTASKCFG> SUBTASKCFG_LIST;
typedef list<SUBTASKCFG>::iterator SUBTASKCFG_LIST_IT;


typedef struct tagSwitchTable
{

	bool bAccePlaneOff;			//�ٶȼƿ���
	bool bCountDownTimeOff;		//����ʱ����
	bool bEagleMapOff;			//С��ͼ����
	bool bElapseTimeOff;		//����ʱ�俪��	
	bool bItemColumnOff;		//����������
	bool bPlayerRankOff;		//�����б���	
	bool bSinglePowerCtrlOff;	//���������ƿ���
	bool bTeamPowerCtrlOff;		//���������ƿ���
	bool bResetOff;				//��λ���أ�������ʾ��
}SWITCHTABLE;

typedef list<DWORD> PREKEY_LIST;		//��������ʼ��������Կ�ף��б�
typedef list<DWORD>::iterator PREKEY_LIST_IT;


typedef struct tagTaskConfig
{
	DWORD			dwID;				//����id
	DWORD			dwMapID;			//����id
	string			strDesc;			//��������
	PREKEY_LIST		listPreKey;			//��������ʼ��������Կ�ף��б�
	SWITCHTABLE		kSwitchTable;		//������صĿ��ر�
	ATOMCFG_LIST	listTaskPeriodAtom;	//�����ڼ䶼���ڵ�ԭ��
	SUBTASKCFG_LIST listSubTask;		//�������б�
}TASKCONFIG, *PTASKCONFIG;

typedef list<TASKCONFIG> TASKCONFIG_LIST;
typedef list<TASKCONFIG>::iterator TASKCONFIG_LIST_IT;