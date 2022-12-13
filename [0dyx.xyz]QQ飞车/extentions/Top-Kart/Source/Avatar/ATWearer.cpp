#include "Top-KartPCH.h"
#include "ATWearer.h"

CATWearer::CATWearer()
{

}

CATWearer::~CATWearer()
{
	list<CAvatarBase*>::iterator at_it = m_listAvatar.begin();
	for(; at_it != m_listAvatar.end(); ++at_it)
	{
		SAFE_DELETE(*at_it);
	}
	m_listAvatar.clear();
}

VOID CATWearer::Update(float fTime)
{
	list<CAvatarBase*>::const_iterator at_it = m_listAvatar.begin();
	for(; at_it != m_listAvatar.end(); ++at_it)
	{
		(*at_it)->Update(fTime);
	}
}

VOID CATWearer::OnAttackByGP(GPINFO& tagGPInfo)
{
	DWORD gpID = tagGPInfo.dwID;
	DWORD gpLife = tagGPInfo.dwLife;
	list<CAvatarBase*>::const_iterator at_it = m_listAvatar.begin();
	for(; at_it != m_listAvatar.end(); ++at_it)
	{
		gpID = (*at_it)->GPTransform(tagGPInfo.dwID);
		if(gpID != tagGPInfo.dwID)
		{
			tagGPInfo.sResult = 2;
			tagGPInfo.dwItemID = (*at_it)->GetID();
			break;
		}
	}
	for(at_it = m_listAvatar.begin(); at_it != m_listAvatar.end(); ++at_it)
	{
		gpLife = (*at_it)->GPLifeTransfrom(tagGPInfo.dwID, tagGPInfo.dwLife);
		if(gpLife != tagGPInfo.dwLife)
		{
			tagGPInfo.sResult = 2;
			tagGPInfo.dwItemID = (*at_it)->GetID();
			break;
		}
	}
	tagGPInfo.dwID = gpID;
	tagGPInfo.dwLife = gpLife;	
}

DWORD CATWearer::AnimTranform(CONST DWORD& dwID)
{

	DWORD dwAnimID;
	list<CAvatarBase*>::const_iterator at_it = m_listAvatar.begin();
	for(; at_it != m_listAvatar.end(); ++at_it)
	{
		dwAnimID = (*at_it)->AnimTransform(dwID);
		if(dwAnimID != dwID)
		{
			return dwAnimID;
		}
	}
	return dwID;
}

PHYSICPROPERTY CATWearer::GetPhysicPropt()
{
	PHYSICPROPERTY tagPhyPropt;
	tagPhyPropt.dwSpeed		= 0;
	tagPhyPropt.dwAccumNo2	= 0;
	tagPhyPropt.dwErupt		= 0;
	tagPhyPropt.dwBigNo2	= 0;
	tagPhyPropt.dwMiniNo2	= 0;

	list<CAvatarBase*>::const_iterator at_it = m_listAvatar.begin();
	for(; at_it != m_listAvatar.end(); ++at_it)
	{
		tagPhyPropt = tagPhyPropt + (*at_it)->GetPhysicPropt();
	}
	return tagPhyPropt;
}

BOOL CATWearer::HaveAT(DWORD dwID)
{
	list<CAvatarBase*>::const_iterator at_it = m_listAvatar.begin();
	for(; at_it != m_listAvatar.end(); ++at_it)
	{
		if((*at_it)->GetID() == dwID)
		{
			return TRUE;
		}
	}
	return FALSE;
}

VOID CATWearer::AddAvatar(CAvatarBase* pAvatar)
{
	pAvatar->SetWearer(this);
	m_listAvatar.push_back(pAvatar);
}

VOID CATWearer::AddAvatar(const ItemUnit& tagUnit)
{
	CAvatarBase* pAvatar = 0;
	switch(tagUnit.nId)
	{
	default:
		pAvatar = new CAvatarBase;
		pAvatar->Create(tagUnit.nId, tagUnit.tagProperty);
	}
	AddAvatar(pAvatar);
}

VOID CATWearer::RemoveAvatar(DWORD dwID)
{
	list<CAvatarBase*>::iterator at_it = m_listAvatar.begin();
	for(; at_it != m_listAvatar.end(); ++at_it)
	{
		if(dwID == (*at_it)->GetID())
		{
			SAFE_DELETE(*at_it);
			m_listAvatar.erase(at_it);
			break;
		}
	}
}