//////////////////////////////////////////////////////////////
//     FileName:    AwardMgr.h
//     Author:      Rayhuang
//     Date:        2007-6-26
//     Description: [Ω±¿¯π‹¿Ì]
/////////////////////////////////////////////////////////////

#pragma once 
#include "singlet.h"
#include <queue>
using namespace std;


class CAwardMgr : public CSingleT<CAwardMgr>
{
public:
	CAwardMgr();
	~CAwardMgr();
	void AddAward(STTaskAward kAward, bool bShowAtCarbarn = false);
	bool IsAward();
	//STTaskAward PopAwardData();
	void FinishFirstAward();
private:
	queue<STTaskAward> m_qAward;
};

