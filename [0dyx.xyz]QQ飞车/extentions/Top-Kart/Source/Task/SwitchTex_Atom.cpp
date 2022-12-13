#include "Top-KartPCH.h"
#include "SwitchTex_Atom.h"


CSwitchTex_Atom::CSwitchTex_Atom():
m_bHasShowFirst(false),
m_pTex1(0),
m_pTex2(0)
{

}

CSwitchTex_Atom::~CSwitchTex_Atom()
{
	SAFE_DELETE(m_pTex1);
	SAFE_DELETE(m_pTex2);
}

void CSwitchTex_Atom::Start()
{
	if(!m_pTex1)
	{
		m_pTex1 = CTScreenTexMgr::Get()->CreateScreenTex(m_dwParam);
	}
	if(!m_pTex2)
	{
		m_pTex2 = CTScreenTexMgr::Get()->CreateScreenTex(m_dwOption);
	}
	if(!m_bHasShowFirst)
	{
		CALL(m_pTex1, SetActivity(true));
		CALL(m_pTex2, SetActivity(false));
		m_bHasShowFirst = true;
	}
}

void CSwitchTex_Atom::Finish()
{
	CALL(m_pTex1, SetActivity(false));
	CALL(m_pTex2, SetActivity(true));
}

void CSwitchTex_Atom::Fail()
{
	return;
}

