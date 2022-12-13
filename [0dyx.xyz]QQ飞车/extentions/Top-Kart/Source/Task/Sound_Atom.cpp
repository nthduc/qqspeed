#include "Top-KartPCH.h"
#include "Sound_Atom.h"
#include "../soundmgr.h"

CSound_Atom::CSound_Atom():
m_dwSound(0)
{

}

CSound_Atom::~CSound_Atom()
{

}

void CSound_Atom::Start()
{
	m_dwSound = CSoundMgr::Get()->PlaySound(m_dwParam);
}

void CSound_Atom::_Stop()
{
	if(m_dwSound)
	{
		CSoundMgr::Get()->StopSound(m_dwSound);
	}
}