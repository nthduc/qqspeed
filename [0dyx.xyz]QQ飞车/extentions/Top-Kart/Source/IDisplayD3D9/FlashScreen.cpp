/********************************************************************
	作者:	　wincoqin
	时间:	  2007/06/13
    文件名:   FlashScreen.cpp
	说明：     []
*********************************************************************/
// precompile header
//#include "Top-KartPCH.h"
// libfile
// userfile
#include "NiApplication.h"
#include "FlashScreen.h"
#include "NiDynamicTexture.h"
#ifndef SAFE_DELETE
#define SAFE_DELETE(x) if (x){delete x; x = NULL;}
#endif


FlashScreen * FlashScreen::CreateFlashScreen(NiTexturePtr spTexture)
{
	if (!spTexture)
		return NULL;

	FlashScreen * pkFlashScreen = new FlashScreen;
	
	if (!pkFlashScreen || !pkFlashScreen->Initialize(spTexture))
	{
		SAFE_DELETE(pkFlashScreen);
	}

	return pkFlashScreen;
}

FlashScreen * FlashScreen::CreateFlashScreen(const int w, const int h)
{
	FlashScreen * pkFlashScreen = new FlashScreen;

	if (!pkFlashScreen || !pkFlashScreen->Initialize(w, h))
	{
		SAFE_DELETE(pkFlashScreen);
	}

	return pkFlashScreen;
}

FlashScreen::FlashScreen()
: m_spTexture(NULL)
{

}

FlashScreen::~FlashScreen()
{

}

NiTexture * FlashScreen::GetTexture()
{
	return m_spTexture;
}

bool FlashScreen::Initialize(NiTexturePtr spTexture)
{
	bool bRet = true;
	m_spTexture = spTexture;
	
	m_pkFlashRender = new FlashRenderer;
	assert(m_pkFlashRender);

	bRet &= m_pkFlashRender->Initialize(m_spTexture->GetWidth(),m_spTexture->GetHeight());
	m_pkFlashRender->SetSourceTexture(m_spTexture);

	return bRet;
}

bool FlashScreen::Initialize(const int w, const int h)
{
	bool bRet = true;

	m_spTexture = NiDynamicTexture::Create(w, h);

	m_pkFlashRender = new FlashRenderer;
	assert(m_pkFlashRender);

	NiDynamicTexture::FormatPrefs FP;
	NiDynamicTexture* pkTT = NiDynamicTexture::Create(m_spTexture->GetWidth(),m_spTexture->GetHeight(),FP);


	bRet &= m_pkFlashRender->Initialize(m_spTexture->GetWidth(),m_spTexture->GetHeight());
	m_pkFlashRender->SetSourceTexture(m_spTexture);

	return bRet;
}

FlashRenderer * FlashScreen::GetFlashRender()
{
	return m_pkFlashRender;
}

void FlashScreen::UpdateFrame()
{
	if (m_pkFlashRender)
		m_pkFlashRender->UpdateFrame();
}

void FlashScreen::Draw(NiRenderer * pkRenderer)
{
}

FlashScreenTexture * FlashScreenTexture::CreateFlashScreen(unsigned int unWidth, unsigned int unHeight)
{
	FlashScreenTexture * pkFlashScreen = new FlashScreenTexture;

	if (!pkFlashScreen || !pkFlashScreen->Initialize(unWidth, unHeight))
	{
		SAFE_DELETE(pkFlashScreen);
	}

	return pkFlashScreen;
}

FlashScreenTexture::FlashScreenTexture()
: m_unScreenRectID(0)
, m_spScreenTexture(NULL)
{

}

FlashScreenTexture::~FlashScreenTexture()
{
	m_spScreenTexture = NULL;
}

bool FlashScreenTexture::Initialize(unsigned int unWidth, unsigned int unHeight)
{
	NiDynamicTexture::FormatPrefs FP;
	NiDynamicTexture* pkTT = NiDynamicTexture::Create(unWidth, unHeight,FP);
	m_spScreenTexture = NiNew NiScreenTexture(pkTT);

	NiTexturingPropertyPtr spProp = m_spScreenTexture->GetTexturingProperty();

	bool bRet = FlashScreen::Initialize(spProp->GetBaseTexture());	
	m_unScreenRectID = m_spScreenTexture->AddNewScreenRect(0, 0, unWidth, unHeight, 0, 0);

	return bRet;
}

void FlashScreenTexture::Draw(NiRenderer * pkRenderer)
{
	m_spScreenTexture->Draw(pkRenderer);
}

void FlashScreenTexture::GetPosInfo(int & x, int & y, int & w, int & h)
{
	NiScreenTexture::ScreenRect& rect = m_spScreenTexture->GetScreenRect(m_unScreenRectID);
	x = rect.m_sPixLeft;
	y = rect.m_sPixTop;
	w = rect.m_usPixWidth;
	h = rect.m_usPixHeight;
}

void FlashScreenTexture::SetPosition(int x, int y, int w, int h)
{
	NiScreenTexture::ScreenRect& rect = m_spScreenTexture->GetScreenRect(m_unScreenRectID);

	rect.m_sPixLeft = x;
	rect.m_sPixTop = y;
	rect.m_usPixWidth = w;
	rect.m_usPixHeight = h;
	rect.m_usTexLeft = 0;
	rect.m_usTexTop = 0;

	m_spScreenTexture->MarkAsChanged(NiScreenTexture::VERTEX_MASK);

	//m_spScreenTexture->RemoveScreenRect(m_unScreenRectID);
	//m_unScreenRectID = m_spScreenTexture->AddNewScreenRect(x, y, w, h, 0, 0);
}

FlashScreenManager::~FlashScreenManager()
{
	FlashScreenTexIter iter    = m_kFlashScreenTexMap.begin();
	FlashScreenTexIter iterEnd = m_kFlashScreenTexMap.end();

	for (; iter != iterEnd; ++iter)
	{
		SAFE_DELETE(iter->second);
	}

	m_kFlashScreenTexMap.clear();
}

unsigned int FlashScreenManager::CreateFlashScreen(const char * path, const int x, const int y, const int w, const int h)
{
	DWORD id = m_kIDGen.Alloc();
	
	FlashScreenTexture * pkFlashScreen = FlashScreenTexture::CreateFlashScreen(w, h);
	if (pkFlashScreen)
	{
		pkFlashScreen->SetPosition(x, y, w, h);
		FlashRenderer * pkRender = pkFlashScreen->GetFlashRender();
		bool bSuccess = pkRender->LoadMovie(path);
		assert(bSuccess);
		m_kFlashScreenTexMap[id] = pkFlashScreen;
		return id;
	}

	m_kIDGen.Free(id);
	return 0;
}

FlashScreenTexture * FlashScreenManager::GetFlashScreen(const int unID)
{
	FlashScreenTexIter iter = m_kFlashScreenTexMap.find(unID);

	if (iter != m_kFlashScreenTexMap.end())
	{
		return iter->second;
	}

	return NULL;
}

void  FlashScreenManager::ReleaseFlashScreen(unsigned int & unID)
{
	FlashScreenTexIter iter = m_kFlashScreenTexMap.find(unID);

	if (iter != m_kFlashScreenTexMap.end())
	{
		if (iter->second)
		{
			FlashRenderer * pkRender = iter->second->GetFlashRender();
			if (pkRender)
			{
				pkRender->Stop();
			}
		}
		SAFE_DELETE(iter->second);

		m_kFlashScreenTexMap.erase(iter);
	}	
	if (unID != 0)
		m_kIDGen.Free(unID);
	unID = 0;
}

void FlashScreenManager::ReleaseAllFlash()
{
	FlashScreenTexIter iter = m_kFlashScreenTexMap.begin();
	FlashScreenTexIter iterEnd = m_kFlashScreenTexMap.end();
	for (; iter != iterEnd; ++iter)
	{
		if (iter->second)
		{
			FlashRenderer * pkRender = iter->second->GetFlashRender();
			if (pkRender)
			{
				pkRender->Stop();
			}
		}
		SAFE_DELETE(iter->second);		
	}

	m_kFlashScreenTexMap.clear();
	m_kIDGen.Reset();
}

void FlashScreenManager::UpdateFrame()
{
	FlashScreenTexIter iter = m_kFlashScreenTexMap.begin();
	FlashScreenTexIter iterEnd = m_kFlashScreenTexMap.end();
	for (; iter != iterEnd; ++iter)
	{
		iter->second->UpdateFrame();
	}
}