////////////////////////////////////////////////
//     FileName:    Esc_Atom.h
//     Author:      rayhuang
//     Date:        2007-6-13
//     Description: Esc请求服务器完成任务原子
////////////////////////////////////////////////


#pragma once 
#include "AtomBase.h"
#include "BnPressTime_Det.h"

class CEsc_Atom : public CAtomBase
{
public:
	CEsc_Atom();
	~CEsc_Atom();
	void Update(float fTime);
private:
	KEYCODE m_eKey;
	NiInputKeyboardPtr m_spKeyboard;
};