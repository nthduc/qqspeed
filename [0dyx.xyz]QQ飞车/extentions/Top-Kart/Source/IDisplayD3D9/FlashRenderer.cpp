#include "Top-KartPCH.h"
#include "IShockwaveFlash.h"
#include "FlashRenderer.h"
//#include "IShockwaveFlash.c"
#include "mycom.h"
#pragma comment(lib, "oleaut32.lib")
extern CComModule _Module;
//CComModule _Module;

//---------------------------------------------------------------------------
FlashRenderer::FlashRenderer()
{
	m_spTexture=0;
	m_pkUnknown = NULL;
	m_pkShockwareFlash = NULL;	
	m_pkViewObject=NULL;
	m_pcBuffer = NULL;
	m_lLastFrame = -1;
}
//---------------------------------------------------------------------------

FlashRenderer::~FlashRenderer()
{
	if (m_pkViewObject)
		m_pkViewObject->Release();

	if (m_pkShockwareFlash)
		m_pkShockwareFlash->Release();

	if (m_pkUnknown)
		m_pkUnknown->Release();

}

HRESULT RegistComponent(LPCSTR lpszComponentDll)
{
	HMODULE hModule = LoadLibrary(lpszComponentDll);
	if ( !hModule )
	{
		XError("注册组件失败,模块%s无法加载(GetLastError=%d)", lpszComponentDll, GetLastError());
		return E_FAIL;
	}
	typedef HRESULT (STDAPICALLTYPE *DllRegisterServer)(void);
	DllRegisterServer pfnDllRegisterServer = (DllRegisterServer)GetProcAddress(hModule, "DllRegisterServer");
	if ( !pfnDllRegisterServer )
	{
		XError("注册组件失败,模块%s无法找到注册入口点", lpszComponentDll);
		return E_NOTIMPL;
	}
	HRESULT hr = pfnDllRegisterServer();
	if ( FAILED(hr) )
	{
		XError("注册组件失败,模块%s的DllRegisterServer返回错误,hr=0x%X", lpszComponentDll, hr);
		return E_FAIL;
	}
	FreeLibrary(hModule);
	return hr;
}
//---------------------------------------------------------------------------
bool FlashRenderer::Initialize(int iWidth, int iHeight)
{
	IShockwaveFlash* pkShockwaveFlash;	
	::OleInitialize(NULL);
	
	::CoCreateInstance(CLSID_ShockwaveFlash,NULL,CLSCTX_INPROC_SERVER,IID_IShockwaveFlash,(void **)&pkShockwaveFlash);
	//CreateObjectFromFile("Flash9b.ocx", CLSID_ShockwaveFlash, IID_IShockwaveFlash, (void**)&pkShockwaveFlash);
	if (pkShockwaveFlash == NULL)
	{
		HRESULT hResult = RegistComponent("Flash9b.ocx");
		if (S_OK == hResult)
		{
			::CoCreateInstance(CLSID_ShockwaveFlash,NULL,CLSCTX_INPROC_SERVER,IID_IShockwaveFlash,(void **)&pkShockwaveFlash);
		}
		else
		{
			return false;
		}
	}
	if (pkShockwaveFlash == NULL)
		return false;
	
	long lVersion;
	pkShockwaveFlash->FlashVersion(&lVersion);
	if (lVersion < 0x60000)
	{
		pkShockwaveFlash->Release();
		return false;
	}
	pkShockwaveFlash->Release();
	
	::AtlAxWinInit();

	RECT rc = {0, 0, iWidth, iHeight};
	m_kWindow.Create(GetMainApp()->GetWindowReference(), &rc, flash_clsid_c, 0);
	::AdjustWindowRect(&rc, m_kWindow.GetWindowLong(GWL_STYLE), FALSE);
	m_kWindow.SetWindowPos(NULL, 0, 0, rc.right - rc.left,rc.bottom - rc.top, 0);

	m_kWindow.QueryControl(&m_pkUnknown);
	if (!m_pkUnknown)
		return false;

	m_pkUnknown->QueryInterface(IID_IShockwaveFlash,(void **)&m_pkShockwareFlash);
	if (!m_pkShockwareFlash)
		return false;	

	m_pkShockwareFlash->QueryInterface(IID_IViewObjectEx,(void **)&m_pkViewObject);
	if (!m_pkViewObject)
		return false;

	m_kRectl.left = 0;
	m_kRectl.right = iWidth;
	m_kRectl.top = 0;
	m_kRectl.bottom = iHeight;



	return true;
}
//---------------------------------------------------------------------------
bool FlashRenderer::LoadMovie(const char* pcFilename)
{
	if (!m_pkShockwareFlash)
		return false;

	HRESULT hr;

	unsigned short usFilename[_MAX_PATH];
	::MultiByteToWideChar(CP_ACP, 0, pcFilename, -1, usFilename, _MAX_PATH);	

	hr = m_pkShockwareFlash->LoadMovie(0, usFilename);
	//unsigned int framenum = GetFrameNum();
	//bool bLoop = GetLoop();
	hr = m_pkShockwareFlash->Play();
	
	SetLoop(false);
    return true;
}
//---------------------------------------------------------------------------
void FlashRenderer::Play()
{
	m_pkShockwareFlash->Play();
}

void FlashRenderer::Forward()
{
	m_pkShockwareFlash->Forward();
}

void FlashRenderer::Rewind()
{
	m_pkShockwareFlash->Rewind();
}

unsigned int FlashRenderer::GetFrameNum()
{
	long framenum;
	m_pkShockwareFlash->get_FrameNum(&framenum);

	return framenum;
}

void FlashRenderer::Pause()
{
	VARIANT_BOOL bPlay;
	m_pkShockwareFlash->get_Playing(&bPlay);
	if (bPlay == -1)
		bPlay = 0;
	else
		bPlay = -1;
	m_pkShockwareFlash->put_Playing(bPlay);
}

bool FlashRenderer::IsPlaying()
{
	VARIANT_BOOL bPlay = FALSE;
	m_pkShockwareFlash->IsPlaying(&bPlay);
	return bPlay == 0;
}

void FlashRenderer::Stop()
{
	m_pkShockwareFlash->Stop();
}

void FlashRenderer::StopPlay()
{
	m_pkShockwareFlash->StopPlay();
}

void FlashRenderer::Back()
{
	m_pkShockwareFlash->Back();
}

void FlashRenderer::GotoFrame(const unsigned int unFrame)
{
	m_pkShockwareFlash->GotoFrame(unFrame);
}

void FlashRenderer::SetLoop(bool bLoop)
{
	VARIANT_BOOL bPlay;
	if (bLoop)
		bPlay = 0;
	else
		bPlay = -1;
	
	m_pkShockwareFlash->put_Loop(bPlay);
}

bool FlashRenderer::GetLoop()
{
	VARIANT_BOOL bPlay;

	m_pkShockwareFlash->get_Loop(&bPlay);

	return bPlay == 0;
}

//---------------------------------------------------------------------------
bool FlashRenderer::UpdateFrame()
{
	if (!m_pkShockwareFlash)
		return false;

	long lCurrFrame;

	VARIANT_BOOL bPlay;
	m_pkShockwareFlash->IsPlaying(&bPlay);

	//if (bPlay != 1)
	//	return false;

	m_pkShockwareFlash->CurrentFrame(&lCurrFrame);

	if (m_lLastFrame != lCurrFrame)
	{

		m_lLastFrame = lCurrFrame;

		NiTexture::RendererData* pRenderData = m_spTexture->GetRendererData();

		NiDX9TextureData* pDX9RenderData = static_cast<NiDX9TextureData*>(pRenderData);

		if(pDX9RenderData)
		{

			HRESULT hr;

			D3DBaseTexturePtr pBaseTexture =  pDX9RenderData->GetD3DTexture();

			LPDIRECT3DTEXTURE9  pTexture9 = (LPDIRECT3DTEXTURE9) pBaseTexture;
			LPDIRECT3DSURFACE9  pSurface9;

			hr = pTexture9->GetSurfaceLevel(0,&pSurface9);

			if(FAILED(hr))
			{
				return false;
			}

			HDC hDC;

			hr = pSurface9->GetDC(&hDC);


			if(FAILED(hr))
			{
				pSurface9->Release();
				pBaseTexture = NULL;
				return false;
			}


			
			m_pkViewObject->Draw(DVASPECT_CONTENT, -1, NULL, NULL, NULL, 
				hDC, &m_kRectl, &m_kRectl, NULL, NULL);



			pSurface9->ReleaseDC(hDC);
			pSurface9->Release();
			pBaseTexture = NULL;

		
			return true;
		}
	}

	return false;
}