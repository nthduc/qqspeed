////////////////////////////////////////////////
//     FileName:    BlinkTex_Atom.h
//     Author:      rayhuang
//     Date:        2007-5-23
//     Description: ÉÁË¸µÄÌùÍ¼Ô­×Ó
////////////////////////////////////////////////

#pragma once 
#include "AtomBase.h"
#include "ScreenTexMgr.h"
class CBlinkTex_Atom : public CAtomBase
{
public:
	CBlinkTex_Atom();
	virtual ~CBlinkTex_Atom();
	void Start();
	void Finish();
	void Fail();
	void Update(float fTime);
protected:
	void _Stop();
private:
	CScreenTex* m_pScreenTex;
	DWORD m_dwBlinkTime;
	bool m_bIsShow;
};