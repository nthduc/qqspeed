#include "Top-KartPCH.h"
#include "gpevilctrl.h"
#include "gpcontrol.h"
#include "gpsettings.h"
#include <NiD3DShaderFactory.h>

CGPEvilCtrl::CGPEvilCtrl(void):
m_spOldShader(0)
{
	NiD3DShaderFactory* pkFactory = 
		(NiD3DShaderFactory*)NiShaderFactory::GetInstance();
	m_spGlow=pkFactory->RetrieveShader("ToonShadingWithOutlineGlow", 0);
}

CGPEvilCtrl::~CGPEvilCtrl(void)
{
	m_spGlow=0;
	m_spOldShader=0;
}

void CGPEvilCtrl::Start()
{
	/*
	NiNode* pNode=m_pSrcTarget->GetNode();
	NiShaderPtr m_spOldShader=((NiGeometry*)pNode)->GetShader();
	((NiGeometry*)pNode)->SetShader(m_spGlow);
	*/
	NiGeometry* pkGeom = NiSmartPointerCast(NiGeometry,
		m_pSrcTarget->GetNode()->GetObjectByName("hair"));
	_ASSERTE(pkGeom);
	NiShaderPtr m_spOldShader=pkGeom->GetShader();
	pkGeom->SetShader(m_spGlow);
	CGPAffectBase::Start();
}

void CGPEvilCtrl::Stop()
{
	/*
	NiNode* pNode=m_pSrcTarget->GetNode();
	((NiGeometry*)pNode)->SetShader(m_spOldShader);
	*/
	NiGeometry* pkGeom = NiSmartPointerCast(NiGeometry,
		m_pSrcTarget->GetNode()->GetObjectByName("hair"));
	pkGeom->SetShader(m_spOldShader);
	CGPAffectBase::Stop();
}

void CGPEvilCtrl::Update(DWORD dwLifeAcc)
{
	CGPAffectBase::Update(dwLifeAcc);
}