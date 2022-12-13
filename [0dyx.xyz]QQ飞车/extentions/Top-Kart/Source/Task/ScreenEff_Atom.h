////////////////////////////////////////////////
//     FileName:    ScreenEff_Atom.h
//     Author:      rayhuang
//     Date:        2007-5-28
//     Description: 屏幕特效原子
////////////////////////////////////////////////
#pragma once 
#include "AtomBase.h"

class CScreenEff_Atom : public CAtomBase
{
public:
	CScreenEff_Atom();
	~CScreenEff_Atom();
	void Start();
	void Finish();
	void Fail();
	void Update(float fTime);
protected:
	void _Stop();
};