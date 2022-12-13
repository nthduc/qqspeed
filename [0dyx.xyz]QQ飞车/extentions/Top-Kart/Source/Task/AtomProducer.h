////////////////////////////////////////////////
//     FileName:    AtomProducer.h
//     Author:      rayhuang
//     Date:        2007-5-21
//     Description: 任务原子的产生器
////////////////////////////////////////////////

#pragma once 
#include "singlet.h"
#include "TaskDefine.h"
#include "AtomBase.h"
#include "ScreenTexMgr.h"



class CAtomProducer : public CSingleT<CAtomProducer>
{
public:
	CAtomProducer();
	~CAtomProducer();
	CAtomBase* CreateAtom(const ATOMCFG* pAtomCfg);
private:
//	CTScreenTexMgr* m_pScreenTexMgr;
};