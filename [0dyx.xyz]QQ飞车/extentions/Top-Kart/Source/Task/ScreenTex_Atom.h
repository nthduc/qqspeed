////////////////////////////////////////////////
//     FileName:    ScreenTex_Atom.h
//     Author:      rayhuang
//     Date:        2007-5-23
//     Description: ÆÁÄ»ÌùÍ¼Ô­×Ó
////////////////////////////////////////////////
#pragma once 
#include "AtomBase.h"
#include "ScreenTexMgr.h"

class CScreenTex_Atom : public CAtomBase
{
public:
	CScreenTex_Atom();
	~CScreenTex_Atom();
	void Start();
	void Finish();
	void Fail();
	void Update(float fTime);
protected:
	void _Stop();
private:
	CScreenTex* m_pScreenTex;
};