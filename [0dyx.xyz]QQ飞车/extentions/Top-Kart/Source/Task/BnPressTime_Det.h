////////////////////////////////////////////////
//     FileName:    BnPressTime_Det.h
//     Author:      rayhuang
//     Date:        2007-5-25
//     Description: ����ʱ������
////////////////////////////////////////////////
#pragma once 
#include "DetectorBase.h"
#include <list>
using namespace std;

typedef NiInputKeyboard::KeyCode KEYCODE;

class CBnPressTime_Det : public CDetectorBase
{
public:
	CBnPressTime_Det();
	virtual ~CBnPressTime_Det();
	void Start();
	void Succeed();
	void Fail();
	void Update(float fTime);
	bool LoadConfig(CVarTable* pTable);
private:
	list<KEYCODE> m_listKey;
	DWORD m_dwStartTime;
	DWORD m_dwLife;
	bool m_bFirstPress;
};