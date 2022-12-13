#pragma once
#include "NiMemObject.h"
#include "NiNode.h"


#define KART_STATE_WAITSTART      1
#define KART_STATE_WAITOVER		  2
#define KART_STATE_NORMALRUN      3
#define KART_STATE_STOP			  4


#define KART_EXCEPTSTATE_NONE     0
#define KART_EXCEPTSTATE_WRONGWAY 1
#define KART_EXCEPTSTATE_RESET    2
#define KART_EXCEPTSTATE_CRASH    3
#define KART_EXCEPTSTATE_OUT      4


#define KART_ITEMSTATUS_NONE	  0
#define KART_ITEMSTATUS_ACCEL	  1	

#define RACE_FINISH_NORMAL		  0
#define RACE_FINISH_TIMEOUT       1
#define RACE_FINISH_GM			  2  //GM ǿ�ƽ�����Ϸ

#define MAX_LAPS 10

#include "CheckEditor.h"

enum RaceFinishType
{
	enTimeOut		= 0,
	enNormalFinish	= 1,
	enGMTerminate	= 2,
};


struct KartWrongWay
{
	
};




class CKartStatusManager
	: public NiMemObject
{
	CKartStatusManager();
	virtual ~CKartStatusManager();

	bool Init();
public: // Create, Destroy And Get
	enum enMark
	{
		enLap2,
		enLap3,
		enLastLap,
		enMarkWin4,
		enMarkLose,
		enGameOver6,
		enMarkNum,
	};
	static bool Create();
	static void Destroy();
	static CKartStatusManager * Get();

	static bool GetResetState();
	static void EnableReset(bool bEnable);

public: // Method
	void ClearStatus();
	virtual void OnTriggerRaceWait(float fWaitTime);
	virtual void OnTriggerRaceCountDown();		// ����ʱ
	virtual void OnTriggerRaceStart();			// ��ʼ
	virtual void OnTriggerRevertRunDir();		// ����ת��
	virtual void OnTriggerLapsUp();				// ����Ȧ��
	virtual void OnTriggerKnockItemEvent(DWORD dwItemID, void * ItemData);	// ײ�ϵ���
	virtual void OnTriggerBeUsedItemEvent(DWORD dwItemID, void * ItemData);  // ��ʹ�õ���
	virtual void OnTriggerExceptionEvent(DWORD dwExceptionCode, void * ExceptionData); // �����쳣
	virtual void OnTriggerRaceFinish(DWORD dwFinishType, float fTime);			       // ��Ϸ�������Լ���ɱ���, ʱ�䵽	
	virtual void OnTriggerShowWrongWay(bool bOn = true);					// ��ʾ/�ر�Wrong way������
	virtual void ShowHint(enMark enHint, bool bShow);

	static NiPoint3 GetRightDir(CCheckPoint * pChkPt, RunDirection dir);

protected:
	void Reset(bool bAuto = false);
	

	static CKartStatusManager * ms_KartStatusMgr;

	DWORD	m_dwKartStatus;
	DWORD   m_dwKartExceptionStatus;
	DWORD   m_dwKartItemStatus;

	NiNodePtr m_aspMarks[enMarkNum];

	DWORD   m_dwCurLap;
	float   m_afLapTime[MAX_LAPS];
	float   m_fStartWaitTime;
	float   m_fCurTime;
	float	m_fFinishWaitTime;
	DWORD   m_dwWarningTime;

	bool    m_bHasShowWrongHint;
	bool	m_bHasFinishRace;
	bool	m_bEnable;
};