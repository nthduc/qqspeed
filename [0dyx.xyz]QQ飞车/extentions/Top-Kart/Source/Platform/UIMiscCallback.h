////////////////////////////////////////////////
//     FileName:    UIMiscCallback.h
//     Author:      Fish
//     Date:        2006-12-28 10:59:36
//     Description: []
////////////////////////////////////////////////

#pragma once

#include "UIMiscMgr.h"
#include "network/network_i.h"
#include "GlobalInterface.h"
#include "../NxPlayMgr.h"

class CKartUIMiscCallback : public CUIMiscCallback
{
public:
	virtual void PlayerDoAction(UINT Uin, int ActionSeq)
	{
		Play* p = NxPlayMgr::GetHostPlay();
		if ( Uin == 0 || Uin == 0xffffffff || Uin == p->GetPlayUin() )
		{
			if(ActionSeq > 2000)
			{
				p->SetKartAnimation(ActionSeq);
			}
			else
			{
				p->SetCharaterAnimation(ActionSeq);
			}
			
		}
		else
		{
			p = NxPlayMgr::GetPlay(Uin);
			if ( p )
			{
				p->SetCharaterAnimation(ActionSeq);
			}
		}
	}
	virtual LPCSTR GetPlayerName()
	{
		assert(GlobalInterface::m_pNetwork);
		QQUSERDATA* pQQData = (QQUSERDATA*)GlobalInterface::m_pNetwork->GetQQUserData();
		if ( pQQData )
		{
			return pQQData->szNickName;
		}
		return NULL;
	}
	virtual void SetCtrlTextColor(INT index, DWORD c)
	{
		assert(GlobalInterface::m_pDisplay);
		GlobalInterface::m_pDisplay->SetCtrlTextColor(index, c);
	}
};
