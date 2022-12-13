#include "Top-KartPCH.h"
#include "./itemdatamgr.h"
#include "stringex.h"
//#include "../screenobjects.h"
#include "../changeColor.h"
#include "AavatarScript.h"
#include "AvatarMgr.h"

CItemDataMgr::CItemDataMgr(void)
{
}

CItemDataMgr::~CItemDataMgr(void)
{
	ITEMPLAYER_MAP::iterator it=m_maPlayerList.begin();
	for (;it!=m_maPlayerList.end();++it)
	{
		delete (it->second);
	}
}

bool CItemDataMgr::LoadData()
{
	bool bRes=false;
	CStringEx strTmp,strPath;
	CVarTable* pSub1st;
	UINT nNum=0,nCnt=0,nSub1st=0,nTmp=0;
	strPath="Avatar/";
	//����ҽӵ��б�
	strTmp.Format("%sitemlist.lua",strPath.c_str());
	CVarArray varHitchTable("HitchList");
	CVar::LoadVarsFromFile(&varHitchTable, NiApplication::ConvertMediaFilename(strTmp.c_str()));
	nNum = varHitchTable.GetNum();
	for (nCnt=0;nCnt<nNum;++nCnt)
	{//����ItemArray
		ItHitchUnit insHitch;
		pSub1st=(CVarTable*)varHitchTable.Element(nCnt);
		insHitch.nType=pSub1st->Element("typeid")->Integer();
		insHitch.strName=pSub1st->Element("name")->String();
		if (pSub1st->Element("defaultitemid"))
		{
			insHitch.nDefaultItem=pSub1st->Element("defaultitemid")->Integer();
		}
		m_maHitchList[insHitch.nType]=insHitch;
	}
	//����Itemlist
	strTmp.Format("%sitemlist.lua",strPath.c_str());
	CVarArray varItemTable("ItemList");
	CVar::LoadVarsFromFile(&varItemTable, NiApplication::ConvertMediaFilename(strTmp.c_str()));
	nNum = varItemTable.GetNum();
	for (nCnt=0;nCnt<nNum;++nCnt)
	{//����ItemArray
		ItemUnit insItem;
		pSub1st=(CVarTable*)varItemTable.Element(nCnt);
		_ATReadNumFromVar(&insItem.nId, pSub1st->Element("itemid"));
		_ATReadNumFromVar(&insItem.nGarageType, pSub1st->Element("garagetype"));
		_ATReadNumFromVar(&insItem.nSexType, pSub1st->Element("sextype"));
		_ATReadNumFromVar(&insItem.nHideRoleFoot, pSub1st->Element("hiderolefoot"));
		_ATReadStringFromVar(insItem.strName, pSub1st->Element("name"));
		_ATReadStringFromVar(insItem.strDesc, pSub1st->Element("description"));
		_ATReadNumFromVar(&insItem.nType, pSub1st->Element("itemtype"));
		_ATReadNumFromVar(&insItem.nResType, pSub1st->Element("restype"));
		_ATReadHitchFromVar(pSub1st, "hitchlist", insItem.liHitchList);
		_ATReadChgColorNameFromVar(pSub1st,"changecolorlist",&insItem);
		_ATReadHideObjNameFromVar(pSub1st,"hidemodellist",&insItem);
		_ATReadColorFromVar(pSub1st, "texcolor", insItem.insColor);
		_ATReadPropertyFromVar(pSub1st, "property", insItem.tagProperty);
		_ATReadStringFromVar(insItem.strModelName, pSub1st->Element("modelname"));
		m_maItemList[insItem.nId]=insItem;


		//insItem.nId=pSub1st->Element("itemid")->Integer();//
		//insItem.strName=pSub1st->Element("name")->String();
		//insItem.nType=pSub1st->Element("itemtype")->Integer();
		//insItem.nResType=pSub1st->Element("restype")->Integer();
		//pVarArrHitch=(CVarArray*)pSub1st->Element("hitchlist");
		//if (pVarArrHitch)
		//{//�йҽӵ��б�
		//	UINT nCntIns=0;
		//	UINT nHitchNum=pVarArrHitch->GetNum();
		//	_ASSERTE(nHitchNum);
		//	for (nCntIns=0;nCntIns<nHitchNum;++nCntIns)
		//	{
		//		pSub2nd=(CVarTable*)pVarArrHitch->Element(nCntIns);
		//		insItem.liHitchList.push_back(pSub2nd->Element("typeid")->Integer());
		//	}
		//}	
		//pSub2nd=(CVarTable*)pSub1st->Element("property");
		//if (pSub2nd)
		//{
		//	
		//	//���������㷨
		//	UINT nCntIns=0,nCntAlg=0;
		//	UINT nArgNum=pVarArrAttr->GetNum();
		//	_ASSERTE(nArgNum);
		//	for (nCntIns=0;nCntIns<nArgNum;++nCntIns)
		//	{
		//		pSub2nd=(CVarTable*)pVarArrAttr->Element(nCntIns);
		//		insItem.insAlgMgr.nTriOccation=pSub2nd->Element("triggeroccasion")->Integer();
		//		insItem.insAlgMgr.nTriParamNum=pSub2nd->Element("triggerparamnum")->Integer();
		//		_ASSERTE(insItem.insAlgMgr.nTriParamNum);
		//		for (nCntAlg=0;nCntAlg<insItem.insAlgMgr.nTriParamNum;++nCntAlg)
		//		{
		//			insItem.insAlgMgr.narrTriParam[nCntAlg]=
		//				pSub2nd->Element(strTmp.FormatPt("triggerparam_%d",nCntAlg+1))->Integer();
		//		}
		//		pVarArrAlg=(CVarArray*)pSub2nd->Element("attributealgorithm");
		//		insItem.insAlgMgr.nAlgNum=pVarArrAlg->GetNum();
		//		_ASSERTE(insItem.insAlgMgr.nAlgNum);
		//		UINT nCntGetArr=0;
		//		for (nCntAlg=0;nCntAlg<insItem.insAlgMgr.nAlgNum;++nCntAlg)
		//		{//��ȡ�㷨������
		//			pSub3rd=(CVarTable*)pVarArrAlg->Element(nCntAlg);
		//			insItem.insAlgMgr.arrAlg[nCntAlg].nId=
		//				pSub3rd->Element("algorithmid")->Integer();
		//			insItem.insAlgMgr.arrAlg[nCntAlg].nOptNum=
		//				pSub3rd->Element("optvaluenum")->Integer();
		//			_ASSERTE(insItem.insAlgMgr.arrAlg[nCntAlg].nOptNum);
		//			for (nCntGetArr=0;nCntGetArr<insItem.insAlgMgr.arrAlg[nCntAlg].nOptNum;++nCntGetArr)
		//			{
		//				insItem.insAlgMgr.arrAlg[nCntAlg].iarrOptVal[nCntGetArr]=
		//					pSub3rd->Element(strTmp.FormatPt("optvalue_%d",nCntGetArr+1))->Integer();
		//			}
		//		}
		//	}
		//}
		//m_maItemList[insItem.nId]=insItem;
	}
	return bRes;
}

ItemUnit* CItemDataMgr::GetItemById(UINT nId)
{
	ItemUnit* pItem=NULL;
	if(m_maItemList.find(nId)!=m_maItemList.end())
	{
		pItem=&m_maItemList[nId];
		LoadItemRes(pItem);
	}
	_ASSERTE(pItem);
	return pItem;
}

ItemStore* CItemDataMgr::GetItemStore(ULONG dwUin,UINT nItemId)
{
	ItemUnit* pItem=GetItemById(nItemId);
	ItTypeObj* pObj=GetTypeObj(dwUin,pItem->nType);
	ItemStore* pStore=GetObjItStore(pObj,nItemId);
	return pStore;	
}

ItemStore* CItemDataMgr::GetCurrEquipStore(ULONG dwUin,UINT nType)
{
	ItemStore* pStore=NULL;
	ItTypeObj* pObj=GetTypeObj(dwUin,nType);
	if (pObj && !pObj->maItemStore.empty())
	{
		ITEMSTORE_MAP::iterator it=pObj->maItemStore.begin();
		for (;it!=pObj->maItemStore.end();++it)
		{
			if (EIS_EQUIPPED==it->second.insInfo.m_bStatus)
			{//�ҵ�
				pStore=&(it->second);
				break;
			}
		}
	}
	return pStore;
}

ItemStore* CItemDataMgr::AddItemStore(ULONG dwUin,STPlayerDBItemInfo* pDbItem,bool bAddBackup)
{
	ItemUnit* pItem=GetItemById(pDbItem->m_iItemID);
	ItTypeObj* pObj=GetTypeObj(dwUin,pItem->nType);
	ItemStore* pStore=GetObjItStore(pObj,pItem->nId);
	ItPlayerUint* pPlayer=GetPlayerByUin(dwUin);
	if (!pStore)
	{//�½�
		ItemStore insStore;
		insStore.insInfo=*pDbItem;
		pObj->maItemStore[pItem->nId]=insStore;
		pStore=&pObj->maItemStore[pItem->nId];
		GarageTypeObj* pGaObj=NULL;
		pGaObj=GetGarageTypeObj(dwUin,pItem->nGarageType);
		pGaObj->maStorePt[pItem->nId]=pStore;
		if (pStore->insInfo.m_bStatus)
		{//��ʱ��Ҫװ��
		}
		XTrace("�����ҵ���ʵ��Id=%u ״̬=%u ������=%u",pDbItem->m_iItemID,pDbItem->m_bStatus,	pDbItem->m_iItemNum);
	}
	else
	{//����Ѿ��������״̬����װ��״̬��
		pStore->insInfo.m_iAvailPeriod=pDbItem->m_iAvailPeriod;
		pStore->insInfo.m_iItemNum=pDbItem->m_iItemNum;
		pStore->insInfo.m_iOtherAttribute=pDbItem->m_iOtherAttribute;
		pStore->insInfo.m_unObtainTime=pDbItem->m_unObtainTime;
		XTrace("��ӣ����£���ҵ���ʵ��Id=%u ������=%u",pDbItem->m_iItemID,pDbItem->m_iItemNum);
	}
	//
	if(bAddBackup && !pPlayer->maBackupInfo.empty())
	{//Ҫ�������ƷҲ���뱸�������Ա�ָ�
		if(pPlayer->maBackupInfo.find(pDbItem->m_iItemID)!=pPlayer->maBackupInfo.end())
		{
			pStore=&pPlayer->maBackupInfo[pDbItem->m_iItemID];
			pStore->insInfo.m_iAvailPeriod += pDbItem->m_iAvailPeriod;
			pStore->insInfo.m_iItemNum += pDbItem->m_iItemNum;
		}
		else
		{//�¼���
			ItemStore insStore;
			insStore.insInfo=*pDbItem;
			pPlayer->maBackupInfo[pDbItem->m_iItemID]=insStore;
		}
	}
	return pStore;	
}

ItemStore* CItemDataMgr::GetObjItStore(ItTypeObj* pObj,UINT nId)
{
	ItemStore* pStore=NULL;
	bool bLoadRes=false;
	if(pObj->maItemStore.find(nId)!=pObj->maItemStore.end())
	{
		pStore=&pObj->maItemStore[nId];
	}
	//_ASSERTE(pStore);
	return pStore;
}

ItPlayerUint* CItemDataMgr::GetPlayerByUin(ULONG dwUin)
{
	ItPlayerUint* pPlayer=NULL;
	if (m_maPlayerList.find(dwUin)!=m_maPlayerList.end())
	{//�������
		pPlayer=m_maPlayerList[dwUin];
	}
	else
	{//��û�����
		pPlayer=new ItPlayerUint;
		pPlayer->nlUin=dwUin;
		m_maPlayerList[dwUin]=pPlayer;
	}
	return pPlayer;
}

ItTypeObj* CItemDataMgr::GetTypeObj(ULONG dwUin,UINT nTypeId)
{
	ItPlayerUint* pPlayer=NULL;
	ItTypeObj* pObj=NULL;
	ItTypeObj insObj;
	insObj.nType=nTypeId;
	pPlayer=GetPlayerByUin(dwUin);
	if (pPlayer->maHObjList.find(nTypeId)==pPlayer->maHObjList.end())
	{
		pPlayer->maHObjList[nTypeId]=insObj;
		pObj=&pPlayer->maHObjList[nTypeId];
	}
	else
	{
		pObj=&pPlayer->maHObjList[nTypeId];
	}
	//
	_ASSERTE(pObj);
	return pObj;
}

const char* CItemDataMgr::GetTypeName(UINT nType)
{
	const char* cpRes=NULL;
	if (m_maHitchList.find(nType)==m_maHitchList.end())
	{
		return cpRes;
	}
	//
	cpRes=m_maHitchList[nType].strName.c_str();
	_ASSERTE(cpRes);
	return cpRes;
}

void CItemDataMgr::GetNifFilePath(UINT nType,CStringEx& strName)
{
	UINT nTypeTmp=(nType & 0xff00);
	if (EAIBT_ROLE==nTypeTmp)
	{
		strName="model/one";
	}
	else if(EAIBT_CAR==nTypeTmp)
	{
		strName="model/car";
	}
	else if(EAIBT_HAIR==nTypeTmp)
	{
		strName="model/hair";
	}
	else
	{
		strName=GetTypeName(nType);
	}
}

void CItemDataMgr::GetItemNodeClone(ItemUnit* pItem,NiNodePtr& pClone)
{
	_ASSERTE(!pClone);
	CStringEx strName,strPath;
	GetNifFilePath(pItem->nType,strName);
	strPath.Format("Avatar/%s/%u/%s.nif",GetTypeName(pItem->nType),
		pItem->nId,strName.c_str());
	NiStream kStream;
	if (!kStream.Load(NiApplication::ConvertMediaFilename(strPath.c_str())))
	{
		_ASSERTE(0);
		return;
	}
	NiNode* pkNode=NiDynamicCast(NiNode, kStream.GetObjectAt(0));
	pClone = NiDynamicCast(NiNode,pkNode->Clone());
	pClone->UpdateNodeBound();
	pClone->UpdateEffects();
	pClone->UpdateProperties();
	pClone->Update(0.0f);
	//������������Ҫ�������������ģ�ͣ���������Ʒ��װ��UI�б�������Ⱦ�޷���ʾ������ͷ��
	if (!pItem->strModelName.empty() && pClone->GetObjectByName(pItem->strModelName.c_str()))
	{
		pClone->GetObjectByName(pItem->strModelName.c_str())->Update(0.0f);
	}

	//NiTimeController::StartAnimations(pClone);

	//������Դ���ʹ���
	switch(pItem->nResType)
	{
	case EIRES_SIG_NIF:
		{//�󶨶�̬ģ��
			break;
		}
		///////////////////////
	case EIRES_SIG_TEX:
		{//������ͼ
			NiSourceTexturePtr	pTexClone;
			GetItemTexClone(pItem,pTexClone);
			NiTriBasedGeom* pkShape = NiDynamicCast(NiTriBasedGeom, 
				pClone->GetObjectByName(pItem->strModelName.c_str()));
			_ASSERTE(pkShape);
			NiTexturingProperty* m_spOriTexProp = 
				(NiTexturingProperty*) pkShape->GetProperty(NiProperty::TEXTURING);
			m_spOriTexProp->SetBaseTexture(pTexClone);
			break;
		}
		///////////////////////
	case EIRES_SIG_COLOR_TEX:
		{//��ɫ��ʱû��ж�صĸ��ֻ����������ɫ�滻
			UINT nCnt=0;
			NiGeometry* pColoredGeometry = NiDynamicCast(NiGeometry, 
					pClone->GetObjectByName(pItem->strModelName.c_str()));
			changeColor(pColoredGeometry,0,0,&pItem->insColor);
			break;
		}
		///////////////////////
	case EIRES_MIX_SIG_SKINNIF:
		{
		}
		break;
	default:
		_ASSERTE(0);
	}		
}

void CItemDataMgr::GetItemTexClone(ItemUnit* pItem,NiSourceTexturePtr& pTexClone)
{
	_ASSERTE(!pTexClone);
	CStringEx strPath;
	strPath.Format("Avatar/%s/%u/%s.dds",GetTypeName(pItem->nType),
		pItem->nId,GetTypeName(pItem->nType));
	pTexClone = 
		NiSourceTexture::Create(NiApplication::ConvertMediaFilename(strPath.c_str()));
	_ASSERTE(pTexClone);
}

bool CItemDataMgr::LoadItemRes(ItemUnit* pItem)
{
	_ASSERTE(pItem);
	bool bRes=true;
	//todo:��ɫ�ͳ�����Դ���������ر���
	CStringEx strName;
	GetNifFilePath(pItem->nType,strName);
	//������Դ���ʹ���
	switch(pItem->nResType)
	{
	case EIRES_SIG_NIF:
		{//����ģ��
			/*
			if (pItem->pkNode)
			{//������
				break;
			}
			CStringEx strPath;
			strPath.Format("Avatar/%s/%u/%s.nif",GetTypeName(pItem->nType),
				pItem->nId,strName.c_str());
			NiStream kStream;
			if (!kStream.Load(NiApplication::ConvertMediaFilename(strPath.c_str())))
			{
				bRes=false;
				break;
			}
			_ASSERTE(!pItem->pkNode);
			NiNode* pkNode=NiDynamicCast(NiNode, kStream.GetObjectAt(0));
			pItem->pkNode = NiDynamicCast(NiNode,pkNode->Clone());
			pItem->pkNode->UpdateEffects();
			pItem->pkNode->UpdateProperties();
			pItem->pkNode->Update(0.0f);
			NiTimeController::StartAnimations(pItem->pkNode);
			*/
			/*
			if(!pItem->dwScreenObject)
			{
				NiNodePtr pClone;
				GetItemNodeClone(pItem,pClone);
				pItem->dwScreenObject = CScreenObjectsMgr::Get()->AddScreenObject(pClone, NULL, false);
			}
			*/
			break;
		}
		///////////////////////
	case EIRES_SIG_TEX:
		{//������ͼ
			/*(
			if (pItem->pkBaseTexture)
			{//������
				break;
			}
			CStringEx strPath2nd;
			strPath2nd.Format("Avatar/%s/%u/%s.dds",GetTypeName(pItem->nType),
				pItem->nId,GetTypeName(pItem->nType));
			pItem->pkBaseTexture = 
				NiSourceTexture::Create(NiApplication::ConvertMediaFilename(strPath2nd.c_str()));
			if(!pItem->pkBaseTexture)
			{
				bRes=false;
				break;
			}
			*/
			break;
		}
		///////////////////////
	case EIRES_SIG_COLOR_TEX:
		{
			break;
		}
		///////////////////////
	case EIRES_MIX_SIG_SKINNIF:
		{
			break;
		}
	default:
		{
			_ASSERTE(0);
		}
	}
	//
	_ASSERTE(bRes);
	return bRes;
}

bool CItemDataMgr::BindItem(Play* pPlay,ItemStore* pStore,bool bEquip)
{
	bool bRes=true;
	if (!pPlay)
	{//��ʱPlayδ����
		return bRes;
	}
	//
	if (bEquip)
	{
		XTrace("Avtar����[װ��]=%u",pStore->insInfo.m_iItemID);
	}
	else
	{
		XTrace("Avtar����[ж ��]=%u",pStore->insInfo.m_iItemID);
	}
	ItemUnit* pItem=GetItemById(pStore->insInfo.m_iItemID);
	UINT unBaseType = pItem->nType & 0xff00;
	if(unBaseType== EAIBT_ROLE || unBaseType== EAIBT_CAR/* ||	unBaseType== EAIBT_HAIR*/)
	{
		pPlay->ChangeItem(pItem->nId);
	}
	//else if (unBaseType== EAIBT_HAIR)
	//{
	//	XTrace("Avatarϵͳ��ʱ����ͷ������װ��");
	//}
	else
	{
		ItemUnit* pHitchItem=NULL;//�ҽӵ�ʵ��Item�������ɫ��
		NiNode* pNode=NULL;
		NiNode* pkHint=NULL;
		switch((pItem->nType & 0xff00))
		{//�������ͻ�ȡ�ҽ�ʵ��ģ��
		case EAIBT_ROLERELEVANT:
		case EAIBT_HAIR:
			{
				pHitchItem=GetItemById(pPlay->GetPlayInfo().m_nRoleID);
				pNode=pPlay->GetCharacterNIFRoot();
				break;
			}
		case EAIBT_CARRELEVANT:
			{
				pHitchItem=GetItemById(pPlay->GetPlayInfo().m_nKartID);
				pNode=pPlay->GetKartNIFRoot();
				break;
			}
		}
		//
		if (pNode)
		{
			if(bEquip)
			{
				pPlay->AddAvatar(*pItem);
			}
			else
			{
				pPlay->RemoveAvatar(pItem->nId);
			}
			//������Դ���ʹ���
			switch(pItem->nResType)
			{
			case EIRES_SIG_NIF:
				{//�󶨶�̬ģ��
					if (bEquip)
					{//װ��
						if(!pStore->pkNode)
						{
							GetItemNodeClone(pItem,pStore->pkNode);
						}
						//
						if(pStore->pkNode)
						{
							pPlay->AttachDummy(GetTypeName(pItem->nType),pStore->pkNode);
						}
						else
						{
							XTrace("EIRES_SIG_NIF����ģ�Ͷ�ȡ����");
						}
					}
					else
					{
						if (pStore->pkNode)
						{
							pPlay->DetachDummy(GetTypeName(pItem->nType),pStore->pkNode);
							pStore->pkNode=0;	///ж��
						}
					}

					break;
				}
				///////////////////////
			case EIRES_SIG_TEX:
				{//������ͼ
					if (bEquip)
					{//װ��
						NiSourceTexturePtr	pTexClone;
						GetItemTexClone(pItem,pTexClone);
						NiTriBasedGeom* pkShape = NiDynamicCast(NiTriBasedGeom, 
							pNode->GetObjectByName(GetTypeName(pItem->nType)));
						_ASSERTE(pkShape);
						NiTexturingProperty* m_spOriTexProp = 
							(NiTexturingProperty*) pkShape->GetProperty(NiProperty::TEXTURING);
						if (pItem->strOriTexName.empty() && m_spOriTexProp)
						{//��¼ԭʼ��ͼ��Ϣ
							NiSourceTexture* pTexture=NiDynamicCast(NiSourceTexture, m_spOriTexProp->GetBaseTexture());
							_ASSERTE(pTexture);
							pItem->strOriTexName=pTexture->GetPlatformSpecificFilename();
						}	
						m_spOriTexProp->SetBaseTexture(pTexClone);
					}
					else
					{//��ԭԭ������ͼ
						if (!pItem->strOriTexName.empty())
						{
							NiTriBasedGeom* pkShape = NiDynamicCast(NiTriBasedGeom, 
								pNode->GetObjectByName(GetTypeName(pItem->nType)));
							NiSourceTexturePtr pTexOri = 
								NiSourceTexture::Create(pItem->strOriTexName.c_str());
							NiTexturingProperty* m_spOriTexProp = 
								(NiTexturingProperty*) pkShape->GetProperty(NiProperty::TEXTURING);
							m_spOriTexProp->SetBaseTexture(pTexOri);
						}

					}
					break;
				}
				///////////////////////
			case EIRES_SIG_COLOR_TEX:
				{//��ɫ��ʱû��ж�صĸ��ֻ����������ɫ�滻
					_ASSERTE(pHitchItem);
					if (bEquip)
					{//װ��
						/*
						UINT nCnt=0;
						for (nCnt=0;nCnt<pHitchItem->deChgColorName.size();++nCnt)
						{
							if (!pNode->GetObjectByName(pHitchItem->deChgColorName[nCnt].c_str()))
							{
								XTrace("�Ҳ�����ɫģ��");
								continue;
							}
							NiGeometry* pColoredGeometry = NiDynamicCast(NiGeometry, 
								pNode->GetObjectByName(pHitchItem->deChgColorName[nCnt].c_str()));
							changeColor(pColoredGeometry,0,0,&pItem->insColor);
						}
						*/
						pPlay->ChangeTeam();
					}
					break;
				}
			//////////////////
			case	EIRES_MIX_SIG_SKINNIF:
				{
					if (bEquip)
					{//װ��
						if(!pStore->pkNode)
						{
							GetItemNodeClone(pItem,pStore->pkNode);
						}
						//�������
						if (pStore->pkNode)
						{
							UINT nCnt=0;
							for (nCnt=0;nCnt<pItem->deChgColorName.size();++nCnt)
							{
								ChangeShapeWithSkin(pNode,pStore->pkNode,pItem->deChgColorName[nCnt].c_str());
							}
							pStore->pkNode=0;
							//ԭģ�������ѱ�detach����Ҫ���»�ȡ
							GetItemNodeClone(pItem,pStore->pkNode);
						}
						else
						{
							XTrace("EIRES_MIX_SIG_SKINNIF����ģ�Ͷ�ȡ����");
						}
					}
					else
					{
						if (pStore->pkNode)
						{
							//pPlay->DetachDummy(GetTypeName(pItem->nType),pStore->pkNode);
							//pStore->pkNode=0;	///ж��
							//��������
							//pStore->pkNode->SetAppCulled(true);
							pStore->pkNode=0;
						}
					}
				}
				break;
			default:
				_ASSERTE(0);
			}					
		}
	}
	return bRes;
}

void CItemDataMgr::ShowRolePart(Play* pPlay,bool bShowFlag)
{
	NiNodePtr pNode=pPlay->GetCharacterNIFRoot();
	ItemUnit* pItem=GetItemById(pPlay->GetPlayInfo().m_nRoleID);
	UINT nCnt=0;
	for (nCnt=0;nCnt<pItem->deHideObjName.size();++nCnt)
	{
		if (!pNode->GetObjectByName(pItem->deHideObjName[nCnt].c_str()))
		{
			XTrace("�Ҳ���������ģ��");
			continue;
		}
		NiGeometry* pShowGeometry = NiDynamicCast(NiGeometry, 
			pNode->GetObjectByName(pItem->deHideObjName[nCnt].c_str()));
		pShowGeometry->SetAppCulled(!bShowFlag);
	}
}

UINT CItemDataMgr::GetItemOnHitch(ItPlayerUint* pPlayUn,UINT nType)
{
	UINT nRes=0;
	UINT unBaseType=nType & 0xff00;
	if(unBaseType== EAIBT_ROLE || unBaseType== EAIBT_CAR ||	unBaseType== EAIBT_HAIR)
	{//���ڽ�ɫ��������ͷ��ʹ��ͳһװ��λ
		nType=unBaseType;
	}
	//
	if (pPlayUn->maItemOnHitch.find(nType)!=pPlayUn->maItemOnHitch.end())
	{//found
		nRes=pPlayUn->maItemOnHitch[nType];
	}
	return nRes;
}

UINT CItemDataMgr::GetDefaultOnHitch(UINT nType)
{
	UINT nRes=0;
	if (m_maHitchList.find(nType)!=m_maHitchList.end())
	{//found
		nRes=m_maHitchList[nType].nDefaultItem;
	}
	return nRes;
}

void CItemDataMgr::SetItemOnHitch(ItPlayerUint* pPlayUn,UINT nType,UINT nItemId)
{
	UINT unBaseType=nType & 0xff00;
	if(unBaseType== EAIBT_ROLE || unBaseType== EAIBT_CAR ||	unBaseType== EAIBT_HAIR)
	{//���ڽ�ɫ��������ͷ��ʹ��ͳһװ��λ
		nType=unBaseType;
	}
	pPlayUn->maItemOnHitch[nType]=nItemId;
}

void CItemDataMgr::BackupUserInfo(UINT dwUin)
{
	ItPlayerUint* pPlayer=NULL;
	ItTypeObj* pObj=NULL;
	ItemStore insStore;
	pPlayer=GetPlayerByUin(dwUin);
	if(!pPlayer->maBackupInfo.empty())
	{
		CleanUserInfo(dwUin);
	}
	ITEMOBJ_MAP::iterator itType=pPlayer->maHObjList.begin();
	ITEMSTORE_MAP::iterator itStore;
	for (;itType!=pPlayer->maHObjList.end();++itType)
	{//ö�������ͷ����Item��
		pObj=&itType->second;
		for (itStore=pObj->maItemStore.begin();itStore!=pObj->maItemStore.end();++itStore)
		{
			insStore.CopyItemStore(&itStore->second);
			pPlayer->maBackupInfo[insStore.insInfo.m_iItemID]=insStore;
		}
	}
}

void CItemDataMgr::RestoreUserInfo(UINT dwUin)
{
	ItPlayerUint* pPlayer=NULL;
	ItTypeObj* pObj=NULL;
	ItemUnit* pUnit=NULL;
	ItemStore* pStore=NULL,*pStNew=NULL;
	pPlayer=GetPlayerByUin(dwUin);
	_ASSERTE(!pPlayer->maBackupInfo.empty());
	ITEMSTORE_MAP::iterator itStore;
	//�����ԭ������
	CleanMainAvatarData(dwUin);
	//
	itStore=pPlayer->maBackupInfo.begin();
	for (;itStore!=pPlayer->maBackupInfo.end();++itStore)
	{//�ѱ�����Ϣ���뵽����б���
		pStore=&itStore->second;
		pUnit=GetItemById(pStore->insInfo.m_iItemID);
		pObj=GetTypeObj(dwUin,pUnit->nType);
		pStNew=GetItemStore(dwUin,pStore->insInfo.m_iItemID);
		if (!pStNew)
		{//�½�
			AddItemStore(dwUin,&pStore->insInfo,false);
		}
		else
		{
			pStNew->CopyItemStore(pStore);
		}
		
	}
}

bool CItemDataMgr::CleanUserInfo(UINT dwUin)
{
	bool bRes=false;
	ItPlayerUint* pPlayer=GetPlayerByUin(dwUin);
	if (!pPlayer->maBackupInfo.empty())
	{
		pPlayer->maBackupInfo.clear();
		bRes=true;
	}
	return bRes;
}

bool CItemDataMgr::CleanAllBind(void)
{
	ItPlayerUint* pPlayer=NULL;
	ItTypeObj* pObj=NULL;
	Play* pPlay=NxPlayMgr::GetHostPlay();
	pPlayer=GetPlayerByUin(pPlay->GetPlayUin());
	bool bRes=false;
	CRequestChangeItemStatus insQu;
	memset(&insQu,0,sizeof(CRequestChangeItemStatus));
	insQu.m_unUin=pPlay->GetPlayUin();
	insQu.m_unTime=::GetTickCount();
	if (pPlayer->maBackupInfo.empty())
	{
		bRes=false;
	}
	else
	{
		ItemStore insStore;
		ItemStore* pStore;
		ITEMOBJ_MAP::iterator itType=pPlayer->maHObjList.begin();
		ITEMSTORE_MAP::iterator itStore;
		for (;itType!=pPlayer->maHObjList.end();++itType)
		{//ö�������ͷ����Item��
			pObj=&itType->second;
			for (itStore=pObj->maItemStore.begin();itStore!=pObj->maItemStore.end();++itStore)
			{
				pStore=&itStore->second;
				if (pStore->insInfo.m_bStatus>0)
				{
					insQu.m_stItemStatus[insQu.m_sItemNum].m_iItemID=pStore->insInfo.m_iItemID;
					insQu.m_stItemStatus[insQu.m_sItemNum].m_bNewStatus=0;
					insQu.m_sItemNum++;
					//BindItem(pPlay,pStore,false);
				}
			}
		}
		//��ʼж��
		if (insQu.m_sItemNum)
		{
			CAvatarMgr::Get()->ChangeItemStatus(&insQu,true);
		}
		bRes=true;
	}

	return bRes;
}

void CItemDataMgr::CleanMainAvatarData(UINT dwUin)
{
	ItPlayerUint* pPlayer=GetPlayerByUin(dwUin);
	ItTypeObj* pObj=NULL;
	ITEMOBJ_MAP::iterator itType=pPlayer->maHObjList.begin();
	ITEMSTORE_MAP::iterator itStore;
	//�����ԭ������
	for (;itType!=pPlayer->maHObjList.end();++itType)
	{//ö�������ͷ����Item��
		pObj=&itType->second;
		pObj->maItemStore.clear();
	}
	pPlayer->maHObjList.clear();
	pPlayer->maItemOnHitch.clear();
	pPlayer->maGarageList.clear();
}

GarageTypeObj* CItemDataMgr::GetGarageTypeObj(UINT dwUin,UINT nGarageType)
{
	ItPlayerUint* pPlayer=GetPlayerByUin(dwUin);
	GarageTypeObj* pGaType=NULL;
	if (pPlayer->maGarageList.find(nGarageType)!=pPlayer->maGarageList.end())
	{
		pGaType=&pPlayer->maGarageList[nGarageType];
	} 
	else
	{//�½�
		GarageTypeObj insObj;
		insObj.nType=nGarageType;
		pPlayer->maGarageList[nGarageType]=insObj;
		pGaType=&pPlayer->maGarageList[nGarageType];
	}
	return pGaType;
}

bool CItemDataMgr::ChangeShapeWithSkin(NiNode* pOriNode,NiNode* pNewNode,const char* cpShapeName)
{
	NiGeometryPtr pkOriObj=NiDynamicCast(NiGeometry,pOriNode->GetObjectByName(cpShapeName));
	//pNewNode�ں���attach��ԭģ��ʧЧ
	NiGeometryPtr pkNewObj=NiDynamicCast(NiGeometry, pNewNode->GetObjectByName(cpShapeName));
	
	if (!pkOriObj || !pkNewObj)
	{
		_ASSERTE(0);
		XTrace("ChangeShapeWithSkin ģ�����ݴ���");
		return false;
	}
	//
	NiSkinInstancePtr pkOriSkin=pkOriObj->GetSkinInstance();
	NiAVObject* pkParentNode=NiDynamicCast(NiAVObject,pOriNode);
	NiSkinInstancePtr pkNewSkin=pkNewObj->GetSkinInstance();
	NiAVObjectPtr pkSkinRoot=pkNewSkin->GetRootParent();
	//���ù���root
	pkNewSkin->SetRootParent(pkOriSkin->GetRootParent());
	//������ģ�͵Ĺ���������һ��ԭ��ģ�����ҳ����������Ƶ���ģ����
	UINT nNumBones=pkNewSkin->GetSkinData()->GetBoneCount();
	NiAVObject* const* pBodyBones = pkNewSkin->GetBones();
	for(UINT nCnt=0; nCnt< nNumBones;++nCnt)
	{
		const char* cBoneName = pBodyBones[nCnt]->GetName();
		//XTrace("Bone=%s",cBoneName);
		NiAVObjectPtr pBoneObject = pkParentNode->GetObjectByName(cBoneName);
		if(pBoneObject)
		{
			//XTrace("Set Bone=%s",cBoneName);
			pkNewSkin->SetBone(nCnt,pBoneObject);
		}
		else
		{
			_ASSERTE(0);
		}
	}
	pkNewObj->UpdateNodeBound();
	//�滻ԭ��ģ��
	pkOriObj->GetParent()->DetachChild(pkOriObj);
	pOriNode->AttachChild(pkNewObj);

	//pOriNode->UpdateNodeBound();
	pOriNode->UpdateEffects();
	pOriNode->UpdateProperties();
	float fUnScaledTime = GetMainApp()->GetCurrentTime();	
	pOriNode->UpdateControllers(fUnScaledTime);
	pOriNode->Update(fUnScaledTime);
	return true;
}