#include "Top-KartPCH.h"
#include "AwardMgr.h"
#include "../network/msgheader.h"
#include "../Platform/GlobalInterface.h"
#include "network/msg.h"
#include "common.h"
#include "zlib.h"
#include "Network/Network_i.h"
#include "../Avatar/ItemDefine.h"
#include "../Avatar/AvatarMgr.h"




CAwardMgr::CAwardMgr()
{
	
}

CAwardMgr::~CAwardMgr()
{
	while(!m_qAward.empty())
	{
		m_qAward.pop();
	}	
}

bool CAwardMgr::IsAward()
{
	return (!m_qAward.empty());
}

void CAwardMgr::AddAward(STTaskAward kAward, bool bShowAtCarbarn /* = false */)
{
	m_qAward.push(kAward);
}

/*
STTaskAward CAwardMgr::PopAwardData()
{
	STTaskAward kTop = m_qAward.front();
	m_qAward.pop();
	return kTop;
}
*/
void CAwardMgr::FinishFirstAward()
{
	if(!m_qAward.empty())
	{
		STTaskAward kAward = m_qAward.front();
		m_qAward.pop();		
		CResponseLoginPara* pResponseLoginPara = GlobalInterface::m_pNetwork->GetGameInfo();
		pResponseLoginPara->m_stPlayerDBBaseInfo.m_unExperience = kAward.m_iTotalExp;
		pResponseLoginPara->m_stPlayerDBBaseInfo.m_iMoney = kAward.m_iTotalMoney;
		DWORD dwHostUin = NxPlayMgr::GetHostPlay()->GetPlayUin();
		if (!CAvatarMgr::Get())
		{
			return;
		}
		for(int i = 0; i < kAward.m_iAwardItemNum; ++i)
		{	
			STPlayerDBItemInfo kItemInfo = kAward.m_astAwardItemInfo[i];
			ItemStore* pItemSt = CAvatarMgr::Get()->GetItemDataMgr()->GetItemStore(dwHostUin,kItemInfo.m_iItemID);
			if(pItemSt)	//原来存在此道具
			{
				if(pItemSt->insInfo.m_iAvailPeriod != -1)	//修改道具有效期，判断原道具是否为无限使用
				{
					if(kItemInfo.m_iAvailPeriod == -1)
					{
						pItemSt->insInfo.m_iAvailPeriod = -1;
					}
					else
					{
						pItemSt->insInfo.m_iAvailPeriod += kItemInfo.m_iAvailPeriod;
					}					
				}
				pItemSt->insInfo.m_iItemNum += kItemInfo.m_iItemNum;		//修改数量
			}
			else
			{	
				//原来不存在此道具则直接修改
				CAvatarMgr::Get()->GetItemDataMgr()->AddItemStore(dwHostUin, &kAward.m_astAwardItemInfo[i], false);
			}
		}		
	}	
}