////////////////////////////////////////////////
//     FileName:    KeyBlink_Atom.h
//     Author:      rayhuang
//     Date:        2007-6-8
//     Description: ¼üÎ»ÉÁË¸Ô­×Ó
////////////////////////////////////////////////

#pragma once 
#include "AtomBase.h"
#include "ScreenTexMgr.h"
class CKeyBlink_Atom : public CAtomBase
{
public:
	CKeyBlink_Atom();
	~CKeyBlink_Atom();
	void Start();
	void Update(float fTime);
protected:
	void _Stop();

private:
	CScreenTex* m_pScreenTexUp;
	CScreenTex* m_pScreenTexDown;
	DWORD m_dwBlinkTime;
	bool m_bIsShowUP;
};