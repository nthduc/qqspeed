////////////////////////////////////////////////
//     FileName:    ScreenFont_Atom.h
//     Author:      rayhuang
//     Date:        2007-6-13
//     Description: ÆÁÄ»ÎÄ×ÖÔ­×Ó
////////////////////////////////////////////////

#pragma once 
#include "AtomBase.h"
#include "ScreenFontMgr.h"
class CScreenFont_Atom : public CAtomBase
{
public:
	CScreenFont_Atom();
	~CScreenFont_Atom();
	void Start();
protected:
	void _Stop();

private:
	CScreenFont* m_pFont;
};