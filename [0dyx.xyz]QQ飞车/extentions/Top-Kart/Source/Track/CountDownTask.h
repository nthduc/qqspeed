#pragma once
#include "../Task.h"
#include "NiNode.h"
#include "NiStream.h"
#include "NiApplication.h"
#include "../Play.h"
/*
class CountDownTask 
	: public Task
{
public:
	CountDownTask();
	virtual ~CountDownTask();
	virtual void Execute(unsigned int uiFrameCount, float fTime);

public:
	void SetParam(int nMode,float fTime);

	enum
	{
		CDM_BEGIN,
		CDM_END,
	};

protected:
    int m_nExcutedTimes;
	int m_nExcutedFrame;
	NiNodePtr m_spCountDown;
	int m_nMode;
	float m_fTime;
	int m_nTime;
};

*/

//class CountDownTask 
//	: public Task
//{
//public:
//	CountDownTask(unsigned int uiLife);
//	virtual ~CountDownTask();
//	virtual void Execute(unsigned int uiFrameCount, float fTime);
//protected:
//	int m_nExcutedTimes;
//	NiNodePtr m_spCountDown;
//	int m_nStartTime;
//	int m_nLife;
//	float m_fStartTime;
//	float m_fEndTime;
//};

//临时类，等有了timer去掉
class CCountDownTask : public CSingleT<CCountDownTask> 
{
public:
	CCountDownTask();
	~CCountDownTask();
	void Update(float fTime);
private:
	DWORD m_dwStartTime;
	bool m_bIsActive;
	NiNodePtr m_spCountDown;
	bool m_bArrStep[5];
};
