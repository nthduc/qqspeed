//////////////////////////////////////////////////////////////
//     FileName:    SwitchTex_Atom.h
//     Author:      Rayhuang
//     Date:        2007-6-20 9:55:54
//     Description: [类似于开关的原子，start时显示某一tex，stop时显示另外的tex]
/////////////////////////////////////////////////////////////

#pragma once 
#include "AtomBase.h"
#include "ScreenTexMgr.h"

class CSwitchTex_Atom : public CAtomBase
{
public:
	CSwitchTex_Atom();
	~CSwitchTex_Atom();
	void Start();
	void Finish();
	void Fail();
private:
	bool m_bHasShowFirst;
	CScreenTex* m_pTex1;
	CScreenTex* m_pTex2;
};