#include "Top-KartPCH.h"
#include ".\itemupdatemgr.h"
#include "AvatarMgr.h"
#include "ItemDataMgr.h"

CItemUpdateMgr::CItemUpdateMgr(void)
{
}

CItemUpdateMgr::~CItemUpdateMgr(void)
{
}

void CItemUpdateMgr::Update(float fDeltaTime)
{
}

void CItemUpdateMgr::AddAvatar(DWORD dwUin, DWORD dwATID)
{
	/*ItemUnit* tagItem = CAvatarMgr::Get()->GetItemDataMgr()->GetItemById(dwATID);
	AddAvatar((CATWearer*)NxPlayMgr::GetPlay(dwUin), *tagItem);*/

}

void CItemUpdateMgr::AddAvatar(CATWearer* pWearer,ItemUnit tagUnit)
{
	/*CAvatarBase* pAvatar = 0;
	switch(tagUnit.nId)
	{
	default:
		pAvatar = new CAvatarBase;
		pAvatar->Create(tagUnit.nId, tagUnit.tagProperty);
	}
	pWearer->AddAvatar(pAvatar);*/
}