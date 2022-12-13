////////////////////////////////////////////////
//     FileName:    sc_uicallback.cpp
//     Author:      Fish
//     Date:        2006-10-11 15:33:20
//     Description: []
////////////////////////////////////////////////

#include "Top-KartPCH.h"
#include "sc.h"
#include "gameui.h"
#include "UIMiscMgr.h"
#include "GlobalInterface.h"
#include "datadef.h"
#include "msgdatacache.h"
#include "../Avatar/AvatarMgr.h"
#include "../Avatar/ItemDataMgr.h"
#include "../Avatar/ShopMgr.h"
#include "../NifAlloc.h"
#include "../IDisplayD3D9/FlashScreen.h"

extern float g_fXUIScale;
extern float g_fYUIScale;

void GetRefLink( char *paddr1, char *paddr2, const char *pstr );
void ExecRefLink( const char *addr1, const char *addr2, int cmdid );

UICALLBACK IsKeyChar(void *pdata ,IWgtData* pWgtData)
{
	stKeyChar *pRichData = (stKeyChar *)pdata;
	GlobalInterface::m_pUIMiscMgr->ParseKeyChar(pRichData);
}

UICALLBACK DrawKeyChar(void *pdata ,IWgtData* pWgtData)
{
	stDrawChar *p = (stDrawChar *)pdata;
	if( p->type == RICH_CHAR_ICON )
	{
		DWORD t = GetTickCount();
		int nframe = t / 300;
		GlobalInterface::m_pUIMiscMgr->DrawChatFace(p->index, nframe, p->rect.ox, p->rect.oy);
	}
	else if ( p->type == RICH_CHAR_FONT )
	{
		//g_pGameSystem->GetDisplay2D()->DrawText(p->rect.ox, p->rect.oy, p->)
	}
}

UICALLBACK ValidKeyChar( void *pdata ,IWgtData* pWgtData)
{
	stKeyChar *pRichData = (stKeyChar *)pdata;
	pRichData->cmdid = RICHCHAR_CMD_CLICK + RICHCHAR_CMD_RCLICK;

	//如果超级链接被点击过了或者不可用,则当前不可点击
	const char *pname = strchr( pRichData->key, '=' );
	const char *pend = strchr( pRichData->key, '>' );
	if( pname && pend )
	{
		char buffer[256];
		int len = pend - (pname+1);
		memcpy( buffer, pname+1, len );
		buffer[len] = 0;
	}
}

UICALLBACK HitKeyChar(void *pdata, IWgtData *pWgtData)
{
	stKeyChar *p = (stKeyChar *)pdata;

	if( p->cmdid != RICHCHAR_CMD_NONE)
	{
		char addr1[256], addr2[256];
		GetRefLink( addr1, addr2, p->key );
		ExecRefLink( addr1, addr2, p->cmdid );
	}

}

void GetRefLink( char *paddr1, char *paddr2, const char *pstr )
{
	bool bfirstaddr = true;
	int count = 0;
	const char *pcur = pstr+2;

	while( *pcur )
	{
		if( bfirstaddr )
		{
			if( *pcur == '=' )
			{
				bfirstaddr = false;
				paddr1[count] = 0;
				count = 0;
			}
			else
				paddr1[count++] = *pcur;
		}
		else
		{
			if( *pcur == '>' )
			{
				paddr2[count] = 0;
				return;
			}
			paddr2[count++] = *pcur;
		}
		pcur++;
	}
}

void ExecRefLink( const char *addr1, const char *addr2, int cmdid )
{

	if( strcmp(addr1, "http") == 0 )
	{
		ShellExecute(NULL,NULL,addr2,NULL,NULL,SW_SHOWMAXIMIZED);
	}
	else if( strcmp(addr1, "talkto") == 0 )
	{
		if( cmdid == RICHCHAR_CMD_CLICK && addr2)
		{
		}
	}
}

#include "../IDisplayD3D9/IDisplayD3D91.h"
extern IDisplay* g_pDisplay;
#include "../screenobjects.h"
#include "screendraw.h"
#include "../nifop.h"
#include "set"


void CustomDrawObject(NiNode* pkNode, LPCSTR lpszCamera, int x, int y, int w, int h)
{
	IDisplayD3D9* pDisplay = (IDisplayD3D9*)g_pDisplay;		
	NiCamera* pkCamera = CNifOp::FindCameraByName(pkNode, lpszCamera);
	CScreenDraw::Draw(pkNode, pkCamera, x, y, w, h, CScreenDraw::SCREENDRAW_ALIGN_HEIGHT);	
	NiNode* pkFakeNode = GetMainApp()->m_spLogo;
	if ( pkFakeNode )
	{
		CScreenDraw::Draw(pkFakeNode, CNifOp::FindCamera(pkFakeNode), 0, 0, 10, 10);
	}
	pDisplay->BeginScreen();
}

void CustomDrawObject(int x, int y, int w, int h, NiNode* pkNode, NiCamera * pkCamera)
{
	IDisplayD3D9* pDisplay = (IDisplayD3D9*)g_pDisplay;	
	CScreenDraw::Draw(pkNode, pkCamera, x, y, w, h, CScreenDraw::SCREENDRAW_ALIGN_HEIGHT);	
	NiNode* pkFakeNode = GetMainApp()->m_spLogo;
	if ( pkFakeNode )
	{
		CScreenDraw::Draw(pkFakeNode, CNifOp::FindCamera(pkFakeNode), 0, 0, 10, 10);
	}
	pDisplay->BeginScreen();	
}

void CustomDrawFlash(int x, int y, int w, int h, FlashScreenTexture * pkTexture)
{
	IDisplayD3D9* pDisplay = (IDisplayD3D9*)g_pDisplay;	
	pkTexture->SetPosition(x, y, w, h);
	pkTexture->Draw(NiRenderer::GetRenderer());
	pDisplay->BeginScreen();
}

void DrawItem(void* pData, IWgtData*)
{
	stDrawItem* di = (stDrawItem*)pData;
	int type = di->hosttype;
	int index = di->hostindex;

	int x = di->rect.ox * g_fXUIScale;
	int y = di->rect.oy * g_fYUIScale;
	int w = di->rect.width * g_fXUIScale;
	int h = di->rect.height * g_fYUIScale;

	if ( type == hosttype_play )
	{
		if ( index < 0 )
		{
			NiNode* pkNode = NxPlayMgr::GetHostPlay()->GetNode();			
			CustomDrawObject(pkNode, "selfshow camera", x, y, w, h);
		}
		else
		{	//index=uin
			{
				CMsgDataCache* pCache = GlobalInterface::m_pDataCache;
				assert(pCache);
				RoomDesc& refRoomDesc = pCache->m_RoomDesc;
				UINT uin = index;
				if ( uin != 0 )
				{
					Play* p = NxPlayMgr::GetPlay(uin);
					NiCamera * pkCamera = CKartScene::Get()->GetSceneCamera("selfshow");
					if ( p )
					{	
						const NiFrustum & kOldFrustum = pkCamera->GetViewFrustum();
						NiNode* pkNode = p->GetNode();
						float hy = 0.55f;
						float hx = hy * 0.75f;						
						NiFrustum frustum = NiFrustum(-hx, hx, hy, -hy, kOldFrustum.m_fNear, kOldFrustum.m_fFar, false);
						pkCamera->SetViewFrustum(frustum);
						//CustomDrawObject(pkNode, "full camera", x, y, w, h);
						CustomDrawObject(x, y, w, h, pkNode, pkCamera);
					}
				}
			}
		}
	}
	else if ( type == hosttype_avatar )
	{
		int nItemID = index;
		if (nItemID)
		{
			ItemStore* pStore= CAvatarMgr::Get()->GetItemDataMgr()->GetItemStore(NxPlayMgr::GetHostPlay()->GetPlayUin(),nItemID);
			if(pStore &&  pStore->GetItemNode() && CNifOp::FindCameraByName(pStore->GetItemNode(), "shopshow"))
			{
				//pShopItem= CShopMgr::Get()->GetShopItemById(7);
				//NiNode* pkNode = NxPlayMgr::GetHostPlay()->GetKartNIFRoot();			
				//CustomDrawObject(pkNode, "selfshow camera", x, y, w, h);
				CustomDrawObject(pStore->GetItemNode(), "shopshow", x, y, w, h);
			}
		}

	}
	else if ( type == hosttype_nif )
	{
		//NiNode* pkObject = CNifAlloc::Get()->GetObject(index);		
		NiNode* pkObject = (NiNode*)index;
		if ( pkObject )
		{
			CustomDrawObject(pkObject, NULL, x, y, w, h);
		}
	}
	else if ( type == hosttype_scene)
	{
		if (CKartScene::Get())
		{
			NiNode * pkObject = CKartScene::Get()->GetScene();
			NiCamera * pkCamera = CKartScene::Get()->GetCamera();
			if (pkObject && pkCamera)
				CustomDrawObject(x, y, w, h, pkObject, pkCamera);
		}
	}
	else if ( type == hosttype_roomself )
	{
		if (CKartScene::Get())
		{
			NiNode * pkObject = CKartScene::Get()->GetScene();
			NiCamera * pkCamera = CKartScene::Get()->GetCamera();
			if (pkObject && pkCamera)
				CustomDrawObject(0, 0, 1024 * g_fXUIScale, 768 * g_fYUIScale, pkObject, pkCamera);
		}
	}
	else if ( type == hosttype_gameplay )
	{
		Play* pHostPlay = NxPlayMgr::GetHostPlay();
		if ( pHostPlay )
		{
			NiNode* pkNodeKart = pHostPlay->GetKartNIFRoot();
			NiNode* pkNode = pHostPlay->GetCharacterNIFRoot();
			CustomDrawObject(x, y, w, h, pkNode, CNifOp::FindCameraByName(pkNodeKart, "head camera"));
		}
	}
	else if ( type == hosttype_shopitem )
	{
		int nItemID = index;
		ShopItem* pShopItem= CShopMgr::Get()->GetShopItemById(nItemID);
		if(pShopItem && pShopItem->GetSelfShowNode() &&
			CNifOp::FindCameraByName(pShopItem->GetSelfShowNode(), "shopshow"))
		{
			//pShopItem= CShopMgr::Get()->GetShopItemById(7);
			//NiNode* pkNode = NxPlayMgr::GetHostPlay()->GetKartNIFRoot();			
			//CustomDrawObject(pkNode, "selfshow camera", x, y, w, h);
			CustomDrawObject(pShopItem->GetSelfShowNode(), "shopshow", x, y, w, h);
		}

		/*
		int nItemID = index;
		ShopItem* pShopItem= CShopMgr::Get()->GetShopItemById(nItemID);


		if(pShopItem && pShopItem->GetSelfShowNode())
		{
		//NiCamera* pkCamera = CNifOp::FindCameraByName(NxPlayMgr::GetHostPlay()->GetKartNIFRoot(), "selfshow camera");
		NiCamera * pkCamera = CKartScene::Get()->GetCamera();
		if (pkCamera)
		{
		CustomDrawObject(x, y, w, h, pShopItem->GetSelfShowNode(), pkCamera);
		}
		}
		*/
		//CustomDrawObject(pShopItem->GetSelfShowNode(), NULL, x, y, w, h);
	}
}

void UIBufferEncrypt(void *pdata, IWgtData *pWgtData)

{

	stUIEncrypt *pEncrypt = (stUIEncrypt *)pdata;

	if ( pEncrypt->nInBufLen <= 0 )

	{

		pEncrypt->nOutBufLen = 0;

		return;

	}

	assert(GlobalInterface::m_pNetwork);
	LPQQUSERDATA pData = (LPQQUSERDATA)GlobalInterface::m_pNetwork->GetQQUserData();
	BYTE byKey[KEY_LEN] = {};
	BYTE* pbyKey = byKey;
	if ( pData )
		pbyKey = pData->byKey;

	GlobalInterface::m_pNetwork->oi_symmetry_encrypt2(pEncrypt->pInBuf, pEncrypt->nInBufLen, 

		pbyKey, 

		pEncrypt->pOutBuf, &pEncrypt->nOutBufLen);

}

void UIBufferDecrypt(void *pdata, IWgtData *pWgtData)

{

	stUIEncrypt *pDecrypt = (stUIEncrypt *)pdata;

	if ( pDecrypt->nInBufLen <= 0 )

	{

		pDecrypt->nOutBufLen = 0;

		pDecrypt->pOutBuf[pDecrypt->nOutBufLen] = 0;

		return;

	}



	int nOutLen = pDecrypt->nOutBufLen;



	assert(GlobalInterface::m_pNetwork);
	LPQQUSERDATA pData = (LPQQUSERDATA)GlobalInterface::m_pNetwork->GetQQUserData();
	BYTE byKey[KEY_LEN] = {};
	BYTE* pbyKey = byKey;
	if ( pData )
		pbyKey = pData->byKey;

	GlobalInterface::m_pNetwork->oi_symmetry_decrypt2(pDecrypt->pInBuf, pDecrypt->nInBufLen, 

		pbyKey, 

		pDecrypt->pOutBuf, &pDecrypt->nOutBufLen);



	if ( pDecrypt->nOutBufLen < nOutLen )

	{

		pDecrypt->pOutBuf[pDecrypt->nOutBufLen] = 0;

	}

}



UICALLBACK UIParseCustomBuffer(void * pData, IWgtData * pWgtData)
{
	CVarTable * pVar = (CVarTable*) pData;

	assert(pVar);
	assert(pWgtData);

	if (pWgtData && pVar)
	{
		stCustomBuffer & kCustomBuffer = pWgtData->GetCustomBuffer();

		int nType = kCustomBuffer.GetType();

		if (nType == customBufferdrawtype_effect)
		{
			stCustomBufferDrawEffect * pDrawEffect = (stCustomBufferDrawEffect *)kCustomBuffer.AllocBuffer(sizeof(stCustomBufferDrawEffect));
			
			pDrawEffect->m_bHasAnimation = (pVar->Element("banimation")->Integer() == 1);
			pDrawEffect->m_enCycleType   = (NiTimeController::CycleType)(pVar->Element("cycletype")->Integer());
			pDrawEffect->m_strEffectName = (pVar->Element("resname")->String());
			pDrawEffect->m_unResType     = (pVar->Element("restype")->Integer());
			pDrawEffect->m_unResourceID  = CNifAlloc::Get()->Alloc(pDrawEffect->m_strEffectName.c_str());
			if (pDrawEffect->m_unResourceID)
				pDrawEffect->m_bHasLoaded    = true;
		}
		else if (nType == customBufferdrawtype_flash)
		{
			stCustomBufferDrawEffect * pDrawEffect = (stCustomBufferDrawEffect *)kCustomBuffer.AllocBuffer(sizeof(stCustomBufferDrawEffect));
			pDrawEffect->m_bHasAnimation = (pVar->Element("banimation")->Integer() == 1);
			pDrawEffect->m_enCycleType   = (NiTimeController::CycleType)(pVar->Element("cycletype")->Integer());
			pDrawEffect->m_strEffectName = (pVar->Element("resname")->String());
			pDrawEffect->m_unResType     = (pVar->Element("restype")->Integer());

			//stRect  rect = pWgtData->GetAbsoluteClientRect();
			
			//pDrawEffect->m_unResourceID = FlashScreenManager::Get()->CreateFlashScreen(pDrawEffect->m_strEffectName.c_str(), rect.ox, rect.oy, rect.width, rect.height);

			//if (pDrawEffect->m_unResourceID)
			//	pDrawEffect->m_bHasLoaded    = true;
		}
		
	}
}

UICALLBACK UICustomBufferSelfShow(void * pData, IWgtData * pWgtData /* = NULL */)
{
	stCustomBuffer * pCustomBuffer = (stCustomBuffer*)pData;

	assert(pCustomBuffer);

	if (pCustomBuffer)
	{
		int nType = pCustomBuffer->GetType();
		if (nType == customBufferdrawtype_effect)
		{
			stCustomBufferDrawEffect * pDrawEffect = (stCustomBufferDrawEffect *)pCustomBuffer->GetBuffer();
			if (pDrawEffect->m_bHasLoaded)
			{
				NiNode * pkNode = CNifAlloc::Get()->GetObject(pDrawEffect->m_unResourceID);
				assert(pkNode);

				if (pkNode && pDrawEffect->m_bHasAnimation)
				{
					float fTime = GetMainApp()->GetAccumTime();
					NiTimeController::StartAnimations(pkNode, fTime);
					pkNode->Update(fTime);
				}
			}
		}
		else if (nType == customBufferdrawtype_flash)
		{

		}
	}
}

UICALLBACK UICustomBufferDeinit(void * pData, IWgtData * pWgtData /* = NULL */)
{
	stCustomBuffer * pCustomBuffer = (stCustomBuffer*)pData;

	assert(pCustomBuffer);

	if (pCustomBuffer)
	{
		int nType = pCustomBuffer->GetType();
		if (nType == customBufferdrawtype_effect)
		{
			stCustomBufferDrawEffect * pDrawEffect = (stCustomBufferDrawEffect *)pCustomBuffer->GetBuffer();
			if (pDrawEffect->m_bHasLoaded)
			{
				CNifAlloc::Get()->Free(pDrawEffect->m_unResourceID);
				pDrawEffect->m_unResourceID = 0;
				pDrawEffect->m_bHasLoaded = false;
			}
		}
		else if (nType == customBufferdrawtype_flash)
		{

		}
	}
}

UICALLBACK UIDrawCustomBuffer(void * pData, IWgtData * pWgtData)
{
	stCustomBuffer * pCustomBuffer = (stCustomBuffer*)pData;

	assert(pCustomBuffer);

	if (pCustomBuffer)
	{
		int nType = pCustomBuffer->GetType();
		stCustomBufferDrawEffect * pDrawEffect = (stCustomBufferDrawEffect *)pCustomBuffer->GetBuffer();
		if (nType == customBufferdrawtype_effect)
		{			
			if (pDrawEffect->m_bHasLoaded)
			{
				NiNode * pkNode = CNifAlloc::Get()->GetObject(pDrawEffect->m_unResourceID);
				//assert(pkNode);

				if (pkNode && pDrawEffect->m_bHasAnimation)
				{					
					stRect rect = pWgtData->GetAbsoluteClientRect();
					float fCoef = 3.0f;
					CustomDrawObject(pkNode, NULL, 0, rect.oy-rect.height*(fCoef-1.0)/2.f, 1024, rect.height*fCoef);
				}
			}
			else
			{// Load Res
				pDrawEffect->m_unResourceID  = CNifAlloc::Get()->Alloc(pDrawEffect->m_strEffectName.c_str());
				if (pDrawEffect->m_unResourceID)
					pDrawEffect->m_bHasLoaded    = true;
			}
		}
		else if (nType == customBufferdrawtype_flash)
		{
			if (pDrawEffect->m_bHasLoaded)
			{
				stRect rect = pWgtData->GetAbsoluteClientRect();

				FlashScreenTexture * pkFlash = FlashScreenManager::Get()->GetFlashScreen(pDrawEffect->m_unResourceID);
				if (!pkFlash)
				{
					pDrawEffect->m_bHasLoaded = false;
					return;
				}

				CustomDrawFlash(rect.ox, rect.oy, rect.width, rect.height, pkFlash);
			}
			else
			{
				stRect rect = pWgtData->GetAbsoluteClientRect();
				TCHAR strCurDrt[500];
				int nLen = ::GetCurrentDirectory(500,strCurDrt);
				if( strCurDrt[nLen]!='\\' )
				{
					strCurDrt[nLen++] = '\\';
					strCurDrt[nLen] = '\0';
				}
				CStringEx str;
				str.Format("%s%s", strCurDrt, NiApplication::ConvertMediaFilename(pDrawEffect->m_strEffectName.c_str()));
				pDrawEffect->m_unResourceID = FlashScreenManager::Get()->CreateFlashScreen(str.c_str(), rect.ox*g_fXUIScale, rect.oy*g_fYUIScale, rect.width*g_fXUIScale, rect.height*g_fYUIScale);

				if (pDrawEffect->m_unResourceID)
					pDrawEffect->m_bHasLoaded    = true;
			}
		}
	}
}

UICALLBACK UIGetCustomBuffer(void * pData, IWgtData * pWgtData)
{	
	lua_State * ls = (lua_State *) pData;
	stCustomBuffer & rCustomBuffer = pWgtData->GetCustomBuffer();
	stCustomBufferDrawEffect * pDrawEffect = (stCustomBufferDrawEffect*) rCustomBuffer.GetBuffer();
	assert(ls);

	if (ls)
	{
		int nType = rCustomBuffer.GetType();
			
		if (nType == customBufferdrawtype_flash || nType == customBufferdrawtype_effect)
		{
			  lua_pushnumber(ls, pDrawEffect->m_unResourceID);
		}
	}
}

// unfinished
UICALLBACK UISetCustomBuffer(void * pData, IWgtData * pWgtData)
{
	lua_State * ls = (lua_State *) pData;
	stCustomBuffer & rCustomBuffer = pWgtData->GetCustomBuffer();
	stCustomBufferDrawEffect * pDrawEffect = (stCustomBufferDrawEffect*) rCustomBuffer.GetBuffer();
	assert(ls);

	if (ls && pDrawEffect)
	{
		int nType = rCustomBuffer.GetType();

		if (nType == customBufferdrawtype_flash || nType == customBufferdrawtype_effect)
		{
			
		}
	}
}