////////////////////////////////////////////////
//     FileName:    AtomLevel.h
//     Author:      rayhuang
//     Date:        2007-5-15
//     Description: 任务中某一阶段的表现，控制一个或多个原子
////////////////////////////////////////////////
#pragma once
#include "TaskDefine.h"
#include "AtomBase.h"
#include "DetectorBase.h"
#include "Observer.h"
#include <list>
using namespace std;

class CAtomLevel : public CObserver, public CSource
{
	friend class CSubTask;
public:
	CAtomLevel();
	~CAtomLevel();
	bool Init(const PATOMLEVELCFG& pAtomLCfg);
	void Start();
	void Update(float fTime);
	void SetState(DWORD dwState);
	DWORD GetState();
	void UpdateMessage();
//	void SetAtom(CAtomBase* pAtom);
//	void SetDetector(CDetectorBase* pDetector);

private:
	void _Finish();
	void _Fail();
	void _AllAtomFinish();
	void _AllAtomFail();

private:
	list<CAtomBase*> m_listAtom;
	CDetectorBase* m_pDetector;
	PATOMLEVELCFG m_pAtomLevelCfg;
	DWORD m_dwState;
};
