#include "Top-KartPCH.h"
#include "ScreenFontMgr.h"
#include "AtomProducer.h"
#include "ScreenTex_Atom.h"
#include "BlinkTex_Atom.h"
#include "LockKey_Atom.h"
#include "ScreenEff_Atom.h"
#include "CDTimerShow_Atom.h"
#include "KeyBlink_Atom.h"
#include "StartNo2_Atom.h"
#include "ScreenFont_Atom.h"
#include "Esc_Atom.h"
#include "Sound_Atom.h"
#include "SwitchTex_Atom.h"


CAtomProducer::CAtomProducer()
{
//	m_pScreenTexMgr = new CTScreenTexMgr;
	CTScreenTexMgr::Create();
	CTScreenTexMgr::Get()->Init();
	CScreenFontMgr::Create();
	CScreenFontMgr::Get()->Init();
}


CAtomProducer::~CAtomProducer()
{
//	SAFE_DELETE(m_pScreenTexMgr);
	CTScreenTexMgr::Destroy();
	CScreenFontMgr::Destroy();
}

CAtomBase* CAtomProducer::CreateAtom(const ATOMCFG* pAtomCfg)
{
	CAtomBase* pAtom = NULL;
	switch(pAtomCfg->dwType)
	{
	case EAMT_ScreenTex:
		{
			pAtom = new CScreenTex_Atom;
			pAtom->SetParam(pAtomCfg->dwParam);
		}
		break;
	case EAMT_BlinkTex:
		{
			pAtom = new CBlinkTex_Atom;
			pAtom->SetParam(pAtomCfg->dwParam);
			pAtom->SetOption(pAtomCfg->dwOption);
		}
		break;
	case EAMT_ScreenEff:
		{
			pAtom = new CScreenEff_Atom;
			pAtom->SetParam(pAtomCfg->dwParam);
		}
		break;
	case EAMT_Sound:
		{
			pAtom = new CSound_Atom;
			pAtom->SetParam(pAtomCfg->dwParam);
		}
		break;
	case EAMT_LockKey:
		{
			pAtom = new CLockKey_Atom;
		}
		break;
	case EAMT_CDTimerShow:
		{
			pAtom = new CCDTimerShow_Atom;
			pAtom->SetParam(pAtomCfg->dwParam);
			pAtom->SetOption(pAtomCfg->dwOption);
		}
		break;
	case EAMT_KeyBlink:
		{
			pAtom = new CKeyBlink_Atom;
			pAtom->SetParam(pAtomCfg->dwParam);
			pAtom->SetOption(pAtomCfg->dwOption);
		}
		break;
	case EAMT_StartNo2:
		{
			pAtom = new CStartNo2_Atom;
			pAtom->SetParam(pAtomCfg->dwParam);
		}
		break;
	case EAMT_ScreenFont:
		{
			pAtom = new CScreenFont_Atom;
			pAtom->SetParam(pAtomCfg->dwParam);
		}
		break;
	case EAMT_Esc:
		{
			pAtom = new CEsc_Atom;
			pAtom->SetParam(pAtomCfg->dwParam);
		}
		break;
	case EAMT_SwitchTex:
		{
			pAtom = new CSwitchTex_Atom;
			pAtom->SetParam(pAtomCfg->dwParam);
			pAtom->SetOption(pAtomCfg->dwOption);
		}
		break;
	default:
		break;
	}
	return pAtom;
}