#include "Top-KartPCH.h"
#include "BaseItem.h"
#include "CameraController.h"
#include "EventContainer.h"
#include "PhysKart\AccelTask.h"
CItemMgr * CItemMgr:: ms_pItemMgr = 0;


CAccelItem::CAccelItem()
{
	m_spItemFigure = NiSourceTexture::Create(NiApplication::ConvertMediaFilename("ItemFigure\\accel.tga"));
}

CAccelItem::~CAccelItem()
{
	m_spItemFigure = 0;
}


NiSourceTexturePtr CAccelItem::GetItemFigure()
{
	return m_spItemFigure;
}

void CAccelItem::Use()
{
	AccelTask * pTask = NiNew AccelTask(5.0f, 3.0f);
	Scheduler::Get()->Schedule(pTask);
//	EventContainer::Get()->PushAnimateEvent(EventContainer::ANIM_SPPEDUP);
	if(NxPlayMgr::GetHostPlay())
		NxPlayMgr::GetHostPlay()->PushPlayerAnimEvent(CAnimEventMgr::EAET_SPPEDUP);
	NxSceneMgr::Get()->GetCameraController()->SetCameraParamType(CameraController::CPT_SPEEDUP);
}

void CAccelItem::BeUsed(void * pData)
{

}


CItemMgr::CItemMgr()
{/*
	NiSourceTexturePtr pTex = NiSourceTexture::Create(
				NiApplication::ConvertMediaFilename("ItemFigure\\columnbase.tga"));

	NiTexturingPropertyPtr  pkTexPro = NiNew NiTexturingProperty();
	NiAlphaPropertyPtr			pkAlphaPro = NiNew NiAlphaProperty();

	pkTexPro->SetBaseTexture(pTex);
	pkTexPro->SetApplyMode(NiTexturingProperty::APPLY_REPLACE);
	pkTexPro->SetBaseClampMode(NiTexturingProperty::CLAMP_S_CLAMP_T);

	pkAlphaPro->SetAlphaBlending(true);
	pkAlphaPro->SetSrcBlendMode(NiAlphaProperty::ALPHA_SRCALPHA);
	pkAlphaPro->SetDestBlendMode(NiAlphaProperty::ALPHA_INVSRCALPHA);


	m_spItemColumnBase = NiNew NiScreenElements(NiNew NiScreenElementsData(false, false, 1));
	m_spItemColumnBase->Insert(4);
	

	m_spItemColumnBase->SetRectangle(0, 0.02, 0.03,(float)pTex->GetWidth()/GameMain::GetScreenWidth(), 
					(float)pTex->GetHeight()/GameMain::GetScreenHeight());

	m_spItemColumnBase->SetTextures(0, 0, 0, 0, 1, 1);

	m_spItemColumnBase->AttachProperty(pkTexPro);
	m_spItemColumnBase->AttachProperty(pkAlphaPro);
	
	m_spItemColumnBase->UpdateBound();
	m_spItemColumnBase->UpdateProperties();
	m_spItemColumnBase->UpdateEffects();
	m_spItemColumnBase->Update(0.0f);

	for(int i = 0; i < MAX_OWNITEM_NUM; i++)
	{
		NiTexturingPropertyPtr  pTexPro = NiNew NiTexturingProperty();
		NiAlphaPropertyPtr		pAlphaPro = NiNew NiAlphaProperty();

		pTexPro->SetApplyMode(NiTexturingProperty::APPLY_REPLACE);
		pTexPro->SetBaseClampMode(NiTexturingProperty::CLAMP_S_CLAMP_T);

		pAlphaPro->SetAlphaBlending(true);
		pAlphaPro->SetSrcBlendMode(NiAlphaProperty::ALPHA_SRCALPHA);
		pAlphaPro->SetDestBlendMode(NiAlphaProperty::ALPHA_INVSRCALPHA);


		m_aspItemScnElt[i] = NiNew NiScreenElements(NiNew NiScreenElementsData(false, false, 1));
		m_aspItemScnElt[i]->Insert(4);

		m_aspItemScnElt[i]->SetTextures(0, 0, 0, 0, 1, 1);

		m_aspItemScnElt[i]->AttachProperty(pTexPro);
		m_aspItemScnElt[i]->AttachProperty(pAlphaPro);

		m_aspItemScnElt[i]->UpdateProperties();
		m_aspItemScnElt[i]->UpdateEffects();
		m_aspItemScnElt[i]->Update(0.0f);

	}

	m_aspItemScnElt[0]->SetRectangle(0, 0.023, 0.032, 
					56.0/GameMain::GetScreenWidth(), 57.0/GameMain::GetScreenHeight());
	m_aspItemScnElt[0]->UpdateBound();

	m_aspItemScnElt[1]->SetRectangle(0, 0.023 + 62.0/GameMain::GetScreenWidth(), 0.0335, 
					70.0/GameMain::GetScreenWidth(), 70.0/GameMain::GetScreenHeight());
	m_aspItemScnElt[1]->UpdateBound();

	m_uiCurOwnItemsNum = 0;
*/
}


CItemMgr::~CItemMgr()
{
	m_spItemColumnBase = 0;
	for(int i =0; i < MAX_OWNITEM_NUM; i++)
		m_aspItemScnElt[i] = 0;
	
	while(!m_pOwnItemsQueue.empty())
	{
		SAFE_NIDELETE(m_pOwnItemsQueue.front());
		m_pOwnItemsQueue.pop();
	}

	while(!m_pBeUsedItemsQueue.empty())
	{
		SAFE_NIDELETE(m_pBeUsedItemsQueue.front());
		m_pBeUsedItemsQueue.pop();
	}

}

bool CItemMgr::Create()
{
	if(ms_pItemMgr)
		return true;
	ms_pItemMgr = NiNew CItemMgr();
	if(!ms_pItemMgr)
		return false;

	NiSourceTexturePtr pTex = NiSourceTexture::Create(
		NiApplication::ConvertMediaFilename("ItemFigure\\columnbase.tga"));

	NiTexturingPropertyPtr  pkTexPro = NiNew NiTexturingProperty();
	NiAlphaPropertyPtr			pkAlphaPro = NiNew NiAlphaProperty();

	pkTexPro->SetBaseTexture(pTex);
	pkTexPro->SetApplyMode(NiTexturingProperty::APPLY_REPLACE);
	pkTexPro->SetBaseClampMode(NiTexturingProperty::CLAMP_S_CLAMP_T);

	pkAlphaPro->SetAlphaBlending(true);
	pkAlphaPro->SetSrcBlendMode(NiAlphaProperty::ALPHA_SRCALPHA);
	pkAlphaPro->SetDestBlendMode(NiAlphaProperty::ALPHA_INVSRCALPHA);


	ms_pItemMgr->m_spItemColumnBase = NiNew NiScreenElements(NiNew NiScreenElementsData(false, false, 1));
	ms_pItemMgr->m_spItemColumnBase->Insert(4);


	ms_pItemMgr->m_spItemColumnBase->SetRectangle(0, 0.03, 0.04,(float)pTex->GetWidth()/GameMain::GetScreenWidth(), 
		(float)pTex->GetHeight()/GameMain::GetScreenHeight());

	ms_pItemMgr->m_spItemColumnBase->SetTextures(0, 0, 0, 0, 1, 1);

	ms_pItemMgr->m_spItemColumnBase->AttachProperty(pkTexPro);
	ms_pItemMgr->m_spItemColumnBase->AttachProperty(pkAlphaPro);

	ms_pItemMgr->m_spItemColumnBase->UpdateBound();
	ms_pItemMgr->m_spItemColumnBase->UpdateProperties();
	ms_pItemMgr->m_spItemColumnBase->UpdateEffects();
	ms_pItemMgr->m_spItemColumnBase->Update(0.0f);

	for(int i = 0; i < MAX_OWNITEM_NUM; i++)
	{
		NiTexturingPropertyPtr  pTexPro = NiNew NiTexturingProperty();
		NiAlphaPropertyPtr		pAlphaPro = NiNew NiAlphaProperty();

		pTexPro->SetApplyMode(NiTexturingProperty::APPLY_REPLACE);
		pTexPro->SetBaseClampMode(NiTexturingProperty::CLAMP_S_CLAMP_T);

		pAlphaPro->SetAlphaBlending(true);
		pAlphaPro->SetSrcBlendMode(NiAlphaProperty::ALPHA_SRCALPHA);
		pAlphaPro->SetDestBlendMode(NiAlphaProperty::ALPHA_INVSRCALPHA);


		ms_pItemMgr->m_aspItemScnElt[i] = NiNew NiScreenElements(NiNew NiScreenElementsData(false, false, 1));
		if(!ms_pItemMgr->m_aspItemScnElt[i])
			return false;

		ms_pItemMgr->m_aspItemScnElt[i]->Insert(4);

		ms_pItemMgr->m_aspItemScnElt[i]->SetTextures(0, 0, 0, 0, 1, 1);

		ms_pItemMgr->m_aspItemScnElt[i]->AttachProperty(pTexPro);
		ms_pItemMgr->m_aspItemScnElt[i]->AttachProperty(pAlphaPro);

		ms_pItemMgr->m_aspItemScnElt[i]->UpdateProperties();
		ms_pItemMgr->m_aspItemScnElt[i]->UpdateEffects();
		ms_pItemMgr->m_aspItemScnElt[i]->Update(0.0f);

	}

	ms_pItemMgr->m_aspItemScnElt[0]->SetRectangle(0, 0.039, 0.062, 
		54.0/GameMain::GetScreenWidth(), 40.0/GameMain::GetScreenHeight());
	ms_pItemMgr->m_aspItemScnElt[0]->UpdateBound();

	ms_pItemMgr->m_aspItemScnElt[1]->SetRectangle(0, 0.03 + 88.0/GameMain::GetScreenWidth(), 0.067, 
		69.0/GameMain::GetScreenWidth(), 51.0/GameMain::GetScreenHeight());
	ms_pItemMgr->m_aspItemScnElt[1]->UpdateBound();

	ms_pItemMgr->m_uiCurOwnItemsNum = 0;

	ms_pItemMgr->m_bIsShowColumn = true;

	return true;

}

void CItemMgr::Destroy()
{
	SAFE_NIDELETE(ms_pItemMgr);
}

CItemMgr * CItemMgr::Get()
{
	return ms_pItemMgr;
}

void CItemMgr::PopItemFigure()
{
	if(m_uiCurOwnItemsNum == MAX_OWNITEM_NUM)
	{
		NiTexturingPropertyPtr pTexPro1 = NiDynamicCast(NiTexturingProperty, 
									m_aspItemScnElt[0]->GetProperty(NiProperty::TEXTURING));
		NiTexturingPropertyPtr pTexPro2 = NiDynamicCast(NiTexturingProperty, 
									m_aspItemScnElt[1]->GetProperty(NiProperty::TEXTURING));
		pTexPro2->SetBaseTexture(pTexPro1->GetBaseTexture());
		pTexPro1->SetBaseTexture(NULL);

		m_aspItemScnElt[0]->UpdateProperties();
		m_aspItemScnElt[0]->UpdateEffects();
		m_aspItemScnElt[0]->Update(0);	

		m_aspItemScnElt[1]->UpdateProperties();
		m_aspItemScnElt[1]->UpdateEffects();
		m_aspItemScnElt[1]->Update(0);	
	}
	else if(m_uiCurOwnItemsNum == 1)
	{
		NiTexturingPropertyPtr pTexPro2 = NiDynamicCast(NiTexturingProperty, 
			m_aspItemScnElt[1]->GetProperty(NiProperty::TEXTURING));
		pTexPro2->SetBaseTexture(NULL);

		m_aspItemScnElt[1]->UpdateProperties();
		m_aspItemScnElt[1]->UpdateEffects();
		m_aspItemScnElt[1]->Update(0);	
	}
}

void CItemMgr::PusnItemFigure(NiSourceTexturePtr pkTex)
{
	if(m_uiCurOwnItemsNum == 0)
	{
		NiTexturingPropertyPtr pTexPro1 = NiDynamicCast(NiTexturingProperty, 
			m_aspItemScnElt[1]->GetProperty(NiProperty::TEXTURING));
		pTexPro1->SetBaseTexture(pkTex);

		m_aspItemScnElt[1]->UpdateProperties();
		m_aspItemScnElt[1]->UpdateEffects();
		m_aspItemScnElt[1]->Update(0);	
	}
	else if(m_uiCurOwnItemsNum == 1)
	{

		NiTexturingPropertyPtr pTexPro1 = NiDynamicCast(NiTexturingProperty, 
			m_aspItemScnElt[0]->GetProperty(NiProperty::TEXTURING));
		pTexPro1->SetBaseTexture(pkTex);

		m_aspItemScnElt[0]->UpdateProperties();
		m_aspItemScnElt[0]->UpdateEffects();
		m_aspItemScnElt[0]->Update(0);	

	}
}

void CItemMgr::ShowItemsColumn(NiRendererPtr pkRenderer)
{
	if(!m_bIsShowColumn)
	{
		return;
	}
	m_spItemColumnBase->Draw(pkRenderer);
	switch(m_uiCurOwnItemsNum)
	{
	case 1:
		m_aspItemScnElt[1]->Draw(pkRenderer);
		break;
	case 2:
		m_aspItemScnElt[0]->Draw(pkRenderer);
		m_aspItemScnElt[1]->Draw(pkRenderer);
		break;
	}

}

void CItemMgr::PushOwnItem(enItemType type, void * pData)
{
	if(m_uiCurOwnItemsNum >= MAX_OWNITEM_NUM)
		return;

	CBaseItem * pItem;
	switch(type)
	{
	case enAccel:
		pItem = NiNew CAccelItem();
		PusnItemFigure(pItem->GetItemFigure());
		m_pOwnItemsQueue.push(pItem);
		m_uiCurOwnItemsNum++;
		break;
	}
}

void CItemMgr::UseItem()
{
	if(!m_pOwnItemsQueue.empty())
	{
		PopItemFigure();
		m_pOwnItemsQueue.front()->Use();		
		SAFE_NIDELETE(m_pOwnItemsQueue.front());
		m_pOwnItemsQueue.pop();
		m_uiCurOwnItemsNum--;
	}
}

void CItemMgr::ClearItems()
{

	m_uiCurOwnItemsNum = 0;

	while(!m_pOwnItemsQueue.empty())
	{
		SAFE_NIDELETE(m_pOwnItemsQueue.front());
		m_pOwnItemsQueue.pop();
	}

	while(!m_pBeUsedItemsQueue.empty())
	{
		SAFE_NIDELETE(m_pBeUsedItemsQueue.front());
		m_pBeUsedItemsQueue.pop();
	}
}

void CItemMgr::TurnOnColumnShow()
{
	m_bIsShowColumn = true;
}


void CItemMgr::TurnOffColumnShow()
{
	m_bIsShowColumn = false;
	ClearItems();
}