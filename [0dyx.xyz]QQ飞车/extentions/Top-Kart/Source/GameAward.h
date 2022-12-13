#ifndef GameAward_H
#define GameAward_H

#include "singlet.h"

class CGameAward : public CSingleT<CGameAward>
{
public:

	CGameAward():m_spPlatform(0),m_fStartTime(0),m_bLoaded(false),m_bShowTeam(false){}	
	~CGameAward();

	void Draw();
	bool Init(CNotifyRaceOverPara* pData);

private:
	void ShowArena();

private:
	CNotifyRaceOverPara m_kRaceOverData;
	NiNodePtr	m_spPlatform;
	float		m_fStartTime;
	bool		m_bLoaded;
	bool		m_bShowTeam;
};

#endif