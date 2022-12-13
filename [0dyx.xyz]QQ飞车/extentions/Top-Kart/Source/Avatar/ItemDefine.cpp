#include "Top-KartPCH.h"
#include "./itemdefine.h"
#include "./AvatarMgr.h"
#include "./ItemDataMgr.h"

tagItHitchUnit::tagItHitchUnit()
{
	nDefaultItem=0;
}

tagItHitchUnit::~tagItHitchUnit()
{
}

//tagItAlgUnit::tagItAlgUnit()
//{
//}
//
//tagItAlgUnit::~tagItAlgUnit()
//{
//}
//
//tagItAlgMgr::tagItAlgMgr()
//{
//	memset(this,0,sizeof(tagItAlgMgr));
//}
//
//tagItAlgMgr::~tagItAlgMgr()
//{
//}

tagItemUnit::tagItemUnit()
{
	//pkBaseNode=NULL;
	//pkBaseTexture=NULL;
//	insAlgMgr.nTriOccation=0;
//	nAlgNum=1;
	nRoleBindNum=0;
	nCarBindNum=0;
	nBlockSlotNum=0;
	nRepellRoleNum=0;
	nSexType=0;
	nHideRoleFoot=0;
	//dwScreenObject=0;
}

tagItemUnit::~tagItemUnit()
{
	//pkBaseNode=0;
	//pkBaseTexture=0;
}

tagItemStore::tagItemStore()
{
	insInfo.m_iItemID=0;
	insInfo.m_bStatus=0;
	insInfo.m_iItemNum=0;
	insInfo.m_iAvailPeriod=0;
	pkNode=NULL;
}

tagItemStore::~tagItemStore()
{
	pkNode=0;
}

void tagItemStore::CopyItemStore(tagItemStore* pStoreSrc)
{
	memcpy(&insInfo,&pStoreSrc->insInfo,sizeof(STPlayerDBItemInfo));
	pkNode=pStoreSrc->pkNode;
}

NiNodePtr tagItemStore::GetItemNode(void)
{
	if(!pkNode)
	{
		ItemUnit* pItem=CAvatarMgr::Get()->GetItemInfoById(insInfo.m_iItemID);
		CAvatarMgr::Get()->GetItemDataMgr()->GetItemNodeClone(pItem,pkNode);
	}
	return pkNode;
}

tagItTypeObj::tagItTypeObj()
{

}

tagItTypeObj::~tagItTypeObj()
{

}

tagGarageTypeObj::tagGarageTypeObj()
{

}

tagGarageTypeObj::~tagGarageTypeObj()
{

}

tagItPlayerUint::tagItPlayerUint()
{

}

tagItPlayerUint::~tagItPlayerUint()
{

}

tagQueryPlayerAvatarInfo::tagQueryPlayerAvatarInfo()
{
	m_unUin_In=0;
	m_cQueryMode_In=0;
	m_nItemNum_InOut=0;
}

tagQueryPlayerAvatarInfo::~tagQueryPlayerAvatarInfo()
{

}