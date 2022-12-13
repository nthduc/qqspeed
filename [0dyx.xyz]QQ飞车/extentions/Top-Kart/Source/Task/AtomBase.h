////////////////////////////////////////////////
//     FileName:    AtomBase.h
//     Author:      rayhuang
//     Date:        2007-5-15
//     Description: 表现原子（包括声音特效等）
////////////////////////////////////////////////


#pragma once 
#include "TaskDefine.h"
#include <list>
using namespace std;

class CAtomBase
{
public:
	CAtomBase();
	virtual ~CAtomBase();
	virtual void Start();
	virtual void Finish();
	virtual void Fail();
	virtual void Update(float fTime);
	virtual void SetParam(DWORD dwParam);
	virtual void SetOption(DWORD dwOption);
protected:
	virtual void _Stop();
protected:
	DWORD m_dwParam;
	DWORD m_dwOption;
};

typedef list<CAtomBase*> PAtomBase_list;
typedef list<CAtomBase*>::iterator PAtomBase_list_It;