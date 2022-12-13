//////////////////////////////////////////////////////////////
//     FileName:    SwitchTex_Atom.h
//     Author:      Rayhuang
//     Date:        2007-6-20 9:55:54
//     Description: [�����ڿ��ص�ԭ�ӣ�startʱ��ʾĳһtex��stopʱ��ʾ�����tex]
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