#include "Top-KartPCH.h"
#include "ScreenEff_Atom.h"
#include "../GameEffectMgr.h"

CScreenEff_Atom::CScreenEff_Atom()
{

}

CScreenEff_Atom::~CScreenEff_Atom()
{

}

void CScreenEff_Atom::Start()
{
	CALL(CGameEffectMgr::Get(), ActiveEffect((IGameEffect::EffectType)m_dwParam, 0));
}

void CScreenEff_Atom::Finish()
{
	_Stop();
}

void CScreenEff_Atom::Fail()
{
	_Stop();
}

void CScreenEff_Atom::_Stop()
{
	CALL(CGameEffectMgr::Get(), DeActiveEffect((IGameEffect::EffectType)m_dwParam));
}

void CScreenEff_Atom::Update(float fTime)
{
	CAtomBase::Update(fTime);
}