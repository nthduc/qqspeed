////////////////////////////////////////////////
//     FileName:    BNPress_Det.h
//     Author:      rayhuang
//     Date:        2007-5-23
//     Description: °´¼ü¼ì²âÆ÷
////////////////////////////////////////////////


#pragma once 
#include "DetectorBase.h"
#include "NiInputKeyboard.h"

class CBNPress_Det : public CDetectorBase
{
public:
	CBNPress_Det();
	virtual ~CBNPress_Det();
	void Start();
	void Succeed();
	void Fail();
	void Update(float fTime);
	bool LoadConfig(CVarTable* pTable);
private:
	list<NiInputKeyboard::KeyCode> m_listKey;
	DWORD m_dwOp;
	NiInputKeyboardPtr m_spKeyboard;
};