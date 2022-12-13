
#include "Top-KartPCH.h"


#include <NiAnimation.h>
#include <NiCollision.h>
#include <NiPhysX.h>
#include <NiMetricsLayer.h>
#include <NiApplicationMetrics.h>
#include <NiMetricsOutput.h>
#include "GameMain.h"
#include "InputManager.h"
#include "LevelManager.h"
#include "Scheduler.h"
#include "MommaTasks.h"
#include "HUDTasks.h"
#include "ShaderHelper.h"
#include "EventContainer.h"
#include "NxSceneMgr.h"
#include "framework/GameApp_i.h"
//#include "framework/gui_i.h"
#include "Platform/GlobalInterface.h"
#include "framework/gui_i.h"
#include "RenderBase.h"
#include "gpcontrol.h"
#include "physxtask.h"
#include "rmoperator.h"
#include "../libsrc/UIEngine/gbUIRenderer.h"
#include "./Avatar/AvatarMgr.h"
#include "./Avatar/ShopMgr.h"
#include "screenobjects.h"
#include "scene.h"
#include "Nifalloc.h"
#include "NifLoader.h"
#include "IDisplayD3D9/d3d9wrapper.h"
#include "./Task/TaskMgr.h"
#include "screendraw.h"
#include "nifop.h"
#include "Profiling.h"
#include "ProilingController.h"
#include "./IDisplayD3D9/FlashScreen.h"
#include "palyerlevel.h"
#include "./Task/AwardMgr.h"

#if defined(_XENON)
#include <NiXenonRenderer.h>
#elif defined(WIN32)
#include <NiDirectXVersion.h>
#include <NiDX9Renderer.h>
#endif


#ifdef ENABLE_NIWEBMETRICS
#include <NiWebMetricsInstanceOutput.h>
#endif

unsigned int GameMain::ms_uiScreenWidth = 1024;
unsigned int GameMain::ms_uiScreenHeight = 768;

float g_fXScale = 1.0f;
float g_fYScale = 1.0f;
float g_fOriginWidth = 1024;
float g_fOriginHeight= 768;
float g_fXUIScale = 1.0f;
float g_fYUIScale = 1.0f;

#if defined(WIN32)

#include <NiD3DShaderFactory.h>
#include <NiD3DShaderProgramFactory.h>

#include "common.h"
#include "soundmgr.h"
#include "./fmod/FMODSystem.h"

// Any shaders loaded in the NIFs are assumed to be version 1.1.
// Note: Alternatively, we could do a version check on a NIF-by-NIF basis.
#define MINIMUM_PIXELSHADER_VERSION    0x101
#define MINIMUM_VERTEXSHADER_VERSION   0x101

// We will trigger lib vs dll builds based on the NIMAIN_IMPORT define.
#if !defined(NIMAIN_IMPORT)
    // We need to define _LIB here so the headers expose the functions we
    // need to access.
    #define _LIB
        #include <NSBShaderLib.h>
        #include <NSFParserLib.h>
    #undef _LIB

    #pragma comment(lib, "NiBinaryShaderLibDX9")
    #pragma comment(lib, "NSBShaderLibDX9")
    #pragma comment(lib, "NSFParserLibDX9")
#endif  //#if !defined(NIMAIN_IMPORT)

#endif //#if defined(WIN32)

bool GameMain::m_bOnlostDevice = false;

//---------------------------------------------------------------------------
bool OnLostDevice(void* pvData)
{
	GameMain::m_bOnlostDevice = true;
	gbUIRenderer::GetRenderer()->OnUILostDevice(pvData);
	NxPlayMgr::OnAllPlayLostDevice(false);

	if ( GetMainApp()->GetFullscreen() )
	{
		if ( GlobalInterface::m_pApp )
		{
			HWND hWnd = NULL;
			GlobalInterface::m_pApp->GetMainWnd(&hWnd);
			DWORD dwStyle = GetWindowLong(hWnd, GWL_STYLE);
			dwStyle &= ~WS_VISIBLE;
			dwStyle &= ~WS_CAPTION;
			SetWindowLong(hWnd, GWL_STYLE, dwStyle);
		}
	}
	return true;
}

bool OnResetDevice(bool bBeforeReset, void* pvData)
{
	gbUIRenderer::GetRenderer()->OnUIResetDevice(bBeforeReset,pvData);
	if(!bBeforeReset)
	{
		GameMain::m_bOnlostDevice = false;
		NxPlayMgr::OnAllPlayLostDevice(true);
	}
	return true;
}

NiApplication* NiApplication::Create()
{
	int nDisplayMode = 0;
	GlobalInterface::m_pSettings->GetIntValue("DisplayMode", &nDisplayMode);
	switch(nDisplayMode)
	{
	case 0:
		GameMain::ms_uiScreenWidth = 800;
		GameMain::ms_uiScreenHeight = 600;
		break;
	case 1:
		GameMain::ms_uiScreenWidth = 1024;
		GameMain::ms_uiScreenHeight = 768;
		break;
	default:
		GameMain::ms_uiScreenWidth = 800;
		GameMain::ms_uiScreenHeight = 600;
		break;
	}
#if defined(WIN32)
    SetMediaPath("DATA/WIN32/");
#endif // defined(WIN32) 
    return NiNew GameMain;
}
//---------------------------------------------------------------------------
GameMain::GameMain() : NiApplication(
    "GameMain", ms_uiScreenWidth, ms_uiScreenHeight), m_bDebug(true),
	m_kCustomCuller(&m_kVisible), m_bKeyF7(false), m_bKeyF8(false)
{
    // Set copy type for Clone operations
    NiObjectNET::SetDefaultCopyType(NiObjectNET::COPY_EXACT);
	NiTexture::SetMipmapByDefault(true);
	m_pkUIRender = NULL;
	// 添加人: wincoqin
	// 时间: 2007/1/19
	// 目的 : 限桢
	int nMaxFrame = 20;
	if (GlobalInterface::m_pSettings)
	{
		GlobalInterface::m_pSettings->GetIntValue("Framelimit", &nMaxFrame);
	}
	NiApplication::SetMaxFrameRate(nMaxFrame);
}
//---------------------------------------------------------------------------
bool GameMain::CreateRenderer()
{   
    const char* pcWorkingDir = ConvertMediaFilename("Shaders\\Generated");
    NiMaterial::SetDefaultWorkingDirectory(pcWorkingDir);

    unsigned int uiWidth = GetAppWindow()->GetWidth();
    unsigned int uiHeight = GetAppWindow()->GetHeight();


    unsigned int uiRefreshRate = NiDX9Renderer::REFRESHRATE_DEFAULT;
    unsigned int uiPresentationInterval 
        = NiDX9Renderer::PRESENT_INTERVAL_ONE;

/*
    if (ms_pkCommand)
    {
        if (m_bDebug)
            return NiApplication::CreateRenderer();

	
        int iTemp;
        if (ms_pkCommand->Integer("width", iTemp))
            uiWidth = (unsigned int)iTemp;
        else if (ms_pkCommand->Integer("w", iTemp))
            uiWidth = (unsigned int)iTemp;

        if (ms_pkCommand->Integer("height", iTemp))
            uiHeight = (unsigned int)iTemp;
        else if (ms_pkCommand->Integer("h", iTemp))
            uiHeight = (unsigned int)iTemp;

        if (ms_pkCommand->Integer("refresh", iTemp))
            uiRefreshRate = (unsigned int)iTemp;
		
        if (1) // ms_pkCommand->Boolean("novsync"))
        {
            uiPresentationInterval 
                = NiDX9Renderer::PRESENT_INTERVAL_IMMEDIATE;

        }
    }*/

	//	增加人:	wincoqin
	//  时间:	2007/02/01
	//  目的:	全屏和非全屏配置化	
	CComPtr<IGameSettings> spSettings;
	GlobalInterface::m_pApp->QueryInterface(IID_IGameSettings, (void**)&spSettings);

	BOOL bFullScreen = FALSE;
	if (spSettings)
		spSettings->GetIntValue("FullScreen", &bFullScreen);
	SetFullscreen(bFullScreen);

	g_fXUIScale = uiWidth/1024.f;
	g_fYUIScale = uiHeight/768.f;
	g_fXScale = g_fXUIScale;
	g_fYScale = g_fYUIScale;

	int nFbm = 0;
	if (spSettings)
		spSettings->GetIntValue("AntiAliasing", & nFbm);
	NiDX9Renderer::FramebufferMode eFbm = (NiDX9Renderer::FramebufferMode)nFbm;


	NiDX9Renderer* pDx9Renderer = NiDX9Renderer::Create(uiWidth, uiHeight,
		GetFullscreen() ? NiDX9Renderer::USE_FULLSCREEN :NiDX9Renderer::USE_MULTITHREADED,
		m_pkAppWindow->GetWindowReference(),
		m_pkAppWindow->GetWindowReference(), 
		D3DADAPTER_DEFAULT, 
		NiDX9Renderer::DEVDESC_PURE,
		NiDX9Renderer::FBFMT_UNKNOWN,
		NiDX9Renderer::DSFMT_UNKNOWN, 
		(NiDX9Renderer::PresentationInterval)uiPresentationInterval,
		NiDX9Renderer::SWAPEFFECT_DEFAULT,
		eFbm, 1,
		uiRefreshRate);
	pDx9Renderer->AddLostDeviceNotificationFunc((NiDX9Renderer::LOSTDEVICENOTIFYFUNC)OnLostDevice,NULL);
	pDx9Renderer->AddResetNotificationFunc((NiDX9Renderer::RESETNOTIFYFUNC)OnResetDevice,NULL);

	m_spRenderer = pDx9Renderer;

	if (!m_spRenderer)
	{
		NiOutputDebugString("GameMain::CreateRender -- "
			"Renderer Creation Failure");
		XError("致命错误，无法创建渲染设备！");
		return false;
	}

	BOOL bToneShader=TRUE;
	if ( spSettings )
		spSettings->GetIntValue("ToneShader", &bToneShader);

	int nThreads = 0;
	if ( spSettings )
		spSettings->GetIntValue( "ThreadCount",&nThreads );
	NxPlayMgr::SetThreads( nThreads );

	if (bToneShader)
	{
		m_pkShaderHelper = NiNew ShaderHelper();
		if (m_pkShaderHelper)
		{	//TODO:这里的目录写死了，需要改进
			char* apcProgramDirectories[2] = 
			{
				"data/win32/Shaders/DX9",
					"data/win32/Shaders/DX9"
			};

			char* apcShaderDirectories[2] =
			{
				"data/win32/Shaders",
					"data/win32/Shaders"
			};

			m_pkShaderHelper->SetupShaderSystem(apcProgramDirectories, 2,
				apcShaderDirectories, 2);
		}
	}



	return true;
}
//---------------------------------------------------------------------------
bool GameMain::CreateSceneMgr()
{

	if (!NxSceneMgr::Create())
	{
		return false;
	}

	/*
    if (!ScreenOverlay::Create())
	{
		return false;
	}
	*/

    Reset();

    // Not using the NiApplication scene
    assert(!m_spScene);

#if defined(NI_USE_MEMORY_MANAGEMENT)
//    NiMemManager::ms_bTrackerEnabled = true;
#endif

    return true;
}


void GameMain::LoadConfig()
{
	m_bDebug = GetCommand()->Boolean("debug") ? true : false;

	bool bHillBabies = GetCommand()->Boolean("hillbabies") ? true : false;

	//LevelManager::SetHillBabies(bHillBabies);
	RMOperator::SetUseShaders(true);

#if defined(WIN32)
	if (GetCommand()->Boolean("noshaders"))
		RMOperator::SetUseShaders(false);
#endif  //#if defined(WIN32)

#if defined(WIN32) && !defined(_XBOX)    
	if (!m_bDebug)
	{
		if (GetCommand()->Boolean("small"))
		{
			ms_uiScreenWidth = 800;
			ms_uiScreenHeight = 600;
		}
		else if (GetCommand()->Boolean("large"))
		{ 
			ms_uiScreenWidth = 1280;
			ms_uiScreenHeight = 1024;
		}
		else 
		{ 
			ms_uiScreenWidth = 1024;
			ms_uiScreenHeight = 768;
		}
	}
#endif  //#if defined(WIN32) && !defined(_XBOX)
}
bool GameMain::Initialize()
{
	Statistics::Create();

    m_pkShaderHelper=NULL;
	//LoadConfig
	LoadConfig();

    // Create the scheduler 
    if (!Scheduler::Create())
	{
		return false;
	}

	// Create the Renderer
    if (!CreateRenderer())
	{
		return false;
	}

    // Create the input manager
    if (!InputManager::Create())
	{
		return false;
	}

	if(!EventContainer::Create())
	{
		return false;
	}

	CFMODSystem::Create();
	CSoundMgr::Create();
	CRenderMgr::Create();
	//CGameEffectMgr::Create();
 	CScreenObjectsMgr::Create();
	CNifAlloc::Create();
	CNifLoader::Create();
	FlashScreenManager::Create();
	CAwardMgr::Create();

	NiAlphaAccumulator* pkSorter = NiNew NiAlphaAccumulator;
	//pkSorter->SetSortByClosestPoint(true);
	NiRenderer::GetRenderer()->SetSorter(pkSorter);

	m_pkUIRender = CreateUIRenderer(GetWindowReference());

	//m_pkUIRender->CreateFont("SystemFont", NiApplication::ConvertMediaFilename("font/STZHONGS.TTF"),48, false,false,false, false, 1 );
	m_pkUIRender->CreateFont("SystemFont", NiApplication::ConvertMediaFilename("font/STZHONGS.TTF"),44, true,false,false, false, 1,3, 0xFF000000);

	MenuManager::Create(&m_kScreenElements, &m_kScreenTextures);
	//创建带描边不带阴影的字体
/*	virtual void CreateFont( const char* pFontName, const char* pFontFilePath, int nFontHeight, bool bAnsiAliased, 
		bool bUnderline, bool bBold, bool bItalic, int nRowSpacing ,
		unsigned int nEdgeWidth, DWORD EdgeColor ) =0;*/
    // Bring up Loading screen.
    DisplaySplash("Load.tga");

    // Get the physics started and set some parameters
    NiPhysXManager* pkManager = NiPhysXManager::GetPhysXManager();
    if (!pkManager->Initialize())
	{
		NiMessageBox("Create PhysXManager Failed!!!", "ERROR", NULL);
		return false;
	}

    if (!CreateSceneMgr())
	{
		return false;
	}

	
	//NxSceneMgr::EnterSceneLevel(0);

    // Start the Scheduler
    Scheduler::Get()->Run();

#if defined(OUTPUT_METRICS)
#ifdef _XENON
    NIMETRICS_ADDMODULE(NiNew NiXMLMetricsOutput(10,"D:\\Metrics.xml"));
#else
    NIMETRICS_ADDMODULE(NiNew NiXMLMetricsOutput(10,"Metrics.xml"));
#endif

#ifdef ENABLE_NIWEBMETRICS
    NiWebMetricsInstanceOutput* pkWM = NiNew NiWebMetricsInstanceOutput(
#ifdef _XENON
        "D:\\MetricsFile.txt", "Xenon",
#else
        "MetricsFile.txt", "PC",
#endif
         "GameMain", "GameMain", NULL, 
        0, false, NULL);

#ifdef _XENON
    pkWM->SetUploadOnClose(false);
#else
    pkWM->SetUploadOnClose(true);
#endif
    NiMetricsLayer::AddOutputModule(pkWM);
#endif
    NIMETRICS_ADDMODULE(NiNew NiVisualTrackerOutput(10));
#endif

	NiSourceTexturePtr pkTexture = NiSourceTexture::Create(
		NiApplication::ConvertMediaFilename("ScreenOverLay/panic_mode_meter.dds"));
	if(!pkTexture)
	{
		NiOutputDebugString("Error:ScreenOverlay::Create()Failed\n");
		return false;
	}
	m_spNDLLogo = NiNew NiScreenTexture(pkTexture);
	m_spNDLLogo->AddNewScreenRect(0, 0, 1, 1, 0, 0);
	
    CreateVisualTrackers();
	///载入Avatar设置数据
	CAvatarMgr::Create();
	///载入商店数据
	CShopMgr::Create();
	//创建任务控制器
	CTaskMgr::Create();
	if(CTaskMgr::Get())
	{
		CTaskMgr::Get()->Init();
	}
	m_spLogo = CScreenDraw::CreateTextureObject(NiApplication::ConvertMediaFilename(""));

	if ( !NxPlayMgr::Init() ) 
		return false;

    return true;
}
//---------------------------------------------------------------------------
bool GameMain::CreateVisualTrackers()
{
    m_fVTPerformanceMax = 100.0f;
    if (!NiApplication::CreateVisualTrackers())
        return false;

    // Add trackers
    for (unsigned int ui = 0; ui < m_kTrackers.GetSize(); ui++)
    {
        NiVisualTracker* pkTracker = m_kTrackers.GetAt(ui);
        assert(pkTracker != NULL);

        if (!strcmp(pkTracker->GetName(), "Time (ms)"))
        {
            pkTracker->AddGraph(NiNew GenericFloatUpdate(0.001f, 
                &m_fAITime), "Phys", NiColor(1.0f, 0.0f, 1.0f),
                100, 0.1f, true);

            pkTracker->AddGraph(NiNew GenericFloatUpdate(0.001f, 
                &m_fAnimTime), "Anim", NiColor(0.0f, 0.0f, 0.0f),
                100, 0.1f, true);
        }


        if (!strcmp(pkTracker->GetName(), "Performance"))
        {
            // Visible Array is not used in the standard fashion, so remove the
            // VisibleArrayUpdate graph and replace with a GenericFloatUpdate
            // graph that will be set manually
            pkTracker->RemoveGraph("Vis Objects");

            //pkTracker->AddGraph(NiNew GenericFloatUpdate(10.0f, 
            //    &m_fVisableObjects),  "VisObjects(10)", 
            //    NiColor(0.0f, 0.0f, 0.0f), 100, 0.1f, true);
			Statistics::SetTracker(pkTracker);

			int id;
			id = pkTracker->AddGraph(NiNew GenericUnsignedIntUpdate(1000.0f, Statistics::GetNum(Statistics::GT_SCENE)),
				"Scene", NiColor(1.0f, 1.0f, 0.0f), 100, 0.1f, true);
			Statistics::SetGraphID(Statistics::GT_SCENE, id);

			id = pkTracker->AddGraph(NiNew GenericUnsignedIntUpdate(1000.0f, Statistics::GetNum(Statistics::GT_CAR)),
				"Car", NiColor(1.0f, 0.0f, 0.0f), 100, 0.1f, true);
			Statistics::SetGraphID(Statistics::GT_CAR, id);

			id = pkTracker->AddGraph(NiNew GenericUnsignedIntUpdate(1000.0f, Statistics::GetNum(Statistics::GT_MINIMAP)),
				"minimap", NiColor(1.0f, 0.0f, 0.0f), 100, 0.1f, true);
			Statistics::SetGraphID(Statistics::GT_MINIMAP, id);


			id = pkTracker->AddGraph(NiNew GenericUnsignedIntUpdate(1000.0f, Statistics::GetNum(Statistics::GT_TOTAL)),
				"Total", NiColor(0.0f, 1.0f, 0.0f), 100, 0.1f, true);
			Statistics::SetGraphID(Statistics::GT_TOTAL, id);
        }


#ifdef NI_MEMORY_DEBUGGER
        NiMemTracker *pkMemTracker = NiMemTracker::Get();
        if (!strcmp(pkTracker->GetName(), "Memory (kb)"))
        {
            m_kTrackers.RemoveAt(ui);

            NiRect<float> kWindowRect3;
            kWindowRect3.m_left   = 0.05f;
            kWindowRect3.m_right  = 0.95f;
            kWindowRect3.m_top    = 0.7f;
            kWindowRect3.m_bottom = 0.95f;
            
            m_fVTMemoryMax = 200;
            
            pkTracker = NiNew NiVisualTracker(m_fVTMemoryMax ,0 , kWindowRect3,
                "Memory (Mb)", true, 3);
            pkTracker->SetShow(false);
            

            pkTracker->AddGraph(NiNew MemHighWaterMarkUpdate(
                1048576.0f, pkMemTracker), "High Watermark",
                NiColor(1.0f, 0.0f, 0.0f), 100, 0.1f, true);

            pkTracker->AddGraph(NiNew MemCurrentUpdate(
                1048576.0f, pkMemTracker), "Current",
                NiColor(0.0f, 1.0f, 0.0f), 100, 0.1f, true);

            pkTracker->AddGraph(NiNew MemCurrentAllocCountUpdate(
                10000.0f, pkMemTracker), "NumAllocs(10e3)",
                NiColor(0.0f, 0.0f, 1.0f), 100, 0.1f, true);
   
            m_kTrackers.AddFirstEmpty(pkTracker);
        }
#endif
    }
    return true;
}
//---------------------------------------------------------------------------
void GameMain::Terminate()
{
	Statistics::Release();
	CPlayerLevel::Destroy();
	CFMODSystem::Destroy();
	CSoundMgr::Destroy();
	CRenderMgr::Destroy();

	MenuManager::Destroy();
    Scheduler::Get()->Stop();
    
	CScreenObjectsMgr::Destroy();
	Scheduler::Destroy();
    //ScreenOverlay::Destroy();
	//CGameEffectMgr::Destroy();
	CKartScene::Destroy();
	CLevelManager::Destroy();
   	NxSceneMgr::Destory();
	InputManager::Destroy();
	EventContainer::Destroy();
	NxPlayMgr::Destory();
	CNifAlloc::Destroy();
	CNifLoader::Destroy();
	CTaskMgr::Destroy();
	FlashScreenManager::Destroy();
	CAwardMgr::Destroy();
	SAFE_RELEASE(m_pkUIRender);
	CALL(m_spNDLLogo, SetTexture(NULL));
    m_spNDLLogo = 0;
    m_spGamebryoLogo = 0;
    m_spDemoScreen = 0;
	m_spLogo = 0;

    NiPhysXManager::GetPhysXManager()->Shutdown(); 

    NiDelete m_pkFrameRate;
    m_pkFrameRate = 0;

    if(m_spRenderer)
    {
        m_spRenderer->SetSorter(NULL);
    }
	
	//shader释放
	NiDelete m_pkShaderHelper;
	//释放Avatar资源
	CAvatarMgr::Destroy();
	//释放Shop资源
	CShopMgr::Destroy();
    NiApplication::Terminate();
    ShowPointer();
}

bool GameMain::MeasureTime()
{
	// start performance measurements
	if (m_fLastTime == -1.0f)
	{
		m_fLastTime = NiGetCurrentTimeInSec();
		m_fAccumTime = 0.0f;
		m_iClicks = 0;
	}

	// measure time
	float fCurrentTime1 = NiGetCurrentTimeInSec();
	//m_fCurrentTime = NiGetCurrentTimeInSec();
	//float fDeltaTime = m_fCurrentTime - m_fLastTime;
	float fDeltaTime = fCurrentTime1 - m_fLastTime;

	// NVPerfHUD support!
	if (m_bNVPerfHUD && fDeltaTime == 0.0f)
		return true;

	if (fDeltaTime < 0.0f)
		fDeltaTime = 0.0f;

	// frame rate limiter
	if (fDeltaTime < (m_fMinFramePeriod))
	{
		Sleep(((m_fMinFramePeriod) - fDeltaTime) * 1000.f);
		//return false;
	}

	m_fCurrentTime = NiGetCurrentTimeInSec();
	fDeltaTime = m_fCurrentTime - m_fLastTime;
	if (fDeltaTime < 0.0f)
		fDeltaTime = 0.0f;
	m_fAccumTime += fDeltaTime;
	m_fFrameTime = m_fAccumTime - m_fLastFrame;
	m_fLastFrame = m_fAccumTime;
	m_fLastTime = m_fCurrentTime;


	return true;
}

//---------------------------------------------------------------------------
void GameMain::OnIdle()
{
	Statistics::BeginCount();

    // MeasureTime returns false if the frame rate is over the pre-set limit
    if (!MeasureTime())
        return;
	
    ResetFrameTimings();	


	BeginUpdate();
	UpdateFrame();
	EndUpdate();

	BeginFrame();
	RenderFrame();
	EndFrame();

	DisplayFrame();

	UpdateVisualTrackers();
	UpdateMetrics();

    m_iClicks++;

	Statistics::EndCount();

}
extern int g_pFrameRate;
void GameMain::UpdateFrame()
{
	// Update the input system
//	UpdateInput();
	// Handle user input
//	ProcessInput();
	//PROFILE(__FUNCTION__);
	ChangeDeviceSetting();
	if (m_bFrameRateEnabled && m_pkFrameRate)
	{
		m_pkFrameRate->TakeSample( );
		m_pkFrameRate->Update( );
	}

	m_fFrameRate = 1.0f / GetFrameTime();
	
	g_pFrameRate = (int)(m_fFrameRate + 0.5f);

	//USE_AUTOCSEXT(CNifLoader::Get()->m_xAutoCS);
	
	Scheduler::Get()->ExecuteFrame();
	CALL(CTaskMgr::Get(), Update(0));
	float fAccumTime = GetAccumTime();
	MenuManager::Get()->UpdateMenu(fAccumTime);
	CLevelManager * pkLevelMgr = CLevelManager::Get();
	if ( pkLevelMgr )
	{		
		pkLevelMgr->Update(fAccumTime);
	}
	else
	{
		float fFrameTime = Scheduler::Get()->GetFrameLength();
		float fTime = Scheduler::Get()->GetClock().GetUnScaledTime();
		NxPlayMgr::UpdateAllPlay(fTime, fFrameTime, true);
		CALL(CKartScene::Get(), Update(fAccumTime));
	}
	CALL(FlashScreenManager::Get(), UpdateFrame());		
	
	CALL(CNifAlloc::Get(), Update(fAccumTime));
	CALL(CNifLoader::Get(), Update(fAccumTime));
	CALL(GlobalInterface::m_pGUI,Update(1000));
	if (CSoundMgr::Get())
		CSoundMgr::Get()->Update(0);	
}

void GameMain::RenderFrame()
{	
	//PROFILE(__FUNCTION__);
	if(m_bOnlostDevice)
		return;
	//USE_AUTOCSEXT(CNifLoader::Get()->m_xAutoCS);

	CScene * pkActiveScene = GetActiveScene();
	BeginRender();
	{
		PROFILE("RenderFrame::PreRender");
		CALL(pkActiveScene, CacheRender());
		CALL(CScreenObjectsMgr::Get(), CacheRender());
		NxPlayMgr::RenderAllPlayName();
	}
	m_spRenderer->SetBackgroundColor(NiColor(0.6f, 0.6f, 0.75f));
	m_spRenderer->BeginUsingDefaultRenderTargetGroup(NiRenderer::CLEAR_ALL);
	{
		PROFILE("RenderFrame::RenderScene");
		CALL(pkActiveScene, Render());		
		if ( !CLevelManager::Get() )
		{
			CScreenDraw::Draw(m_spLogo, CNifOp::FindCamera(m_spLogo), 0, 0, 10, 10);
		}
	}
	//draw ui
	{
		PROFILE("RenderFrame::OtherUI");

		RenderScreenItems();
		RenderVisualTrackers();
		
		CALL(pkActiveScene, Draw());
		CALL(m_spNDLLogo, Draw(m_spRenderer));				
	}
	
	CALL(GlobalInterface::m_pGUI, RenderUI());		
	
	if (NxSceneMgr::Get() && NxSceneMgr::Get()->GetProfilingController())
		NxSceneMgr::Get()->GetProfilingController()->ProfileDraw();
	m_spRenderer->EndUsingRenderTargetGroup();
	EndRender();
}

CScene*	GameMain::GetActiveScene()
{
	if ( CLevelManager::Get() )
	{
		return CLevelManager::Get();
	}
	return CKartScene::Get();
}

static bool g_bF8 = false;

bool GameMain::ChangeDeviceSetting()
{
	bool bFullScreen = GetFullscreen();
	NiDX9Renderer * pkDX9Render = NiDynamicCast(NiDX9Renderer, NiRenderer::GetRenderer());
	NiAppWindow * pkAppWindow = GetMainApp()->GetAppWindow();
	HWND hWnd = GetMainApp()->GetWindowReference();
	DWORD dwStyle = pkAppWindow->GetWindowStyle();

	if (m_bKeyF8)
	{
		m_bKeyF8 = false;

		g_bF8 = !g_bF8;
		

		int w = 1024;
		int h = 768;
		
		if (!g_bF8)
		{
			w = 1024;
			h = 768;
		}
		else
		{
			w = 800;
			h = 600;
		}

		//D3D9::m_Width = w;
		//D3D9::m_Height =h;

		g_fXScale = (float(w))/g_fOriginWidth;
		g_fYScale = (float(h))/g_fOriginHeight;

		pkAppWindow->SetWidth(w);
		pkAppWindow->SetHeight(h);

		//pkDX9Render->LockRenderer();
		
		NiDX9Renderer::RecreateStatus enRecreateStatus = pkDX9Render->Recreate(w, h);

		if (enRecreateStatus == NiDX9Renderer::RECREATESTATUS_FAILED)
		{
			return false;
		}
		else if(enRecreateStatus == NiDX9Renderer::RECREATESTATUS_OK)
		{
			g_fXUIScale = ((float)w)/1024.f;
			g_fYUIScale = ((float)h)/768.f;
		}

		RECT rcOrigin;

		GetWindowRect(hWnd, &rcOrigin);
		int left = rcOrigin.left;
		int top = rcOrigin.top;
		RECT rcWindow = { 0, 0, w, h };
		AdjustWindowRect(&rcWindow, dwStyle, FALSE);
		//
		MoveWindow(hWnd, left, top, rcWindow.right - rcWindow.left, rcWindow.bottom - rcWindow.top, TRUE);
		//pkDX9Render->UnlockRenderer();
	}
	if (m_bKeyF7)
	{
			
		//g_bF8 = !g_bF8;		

		if (!bFullScreen)
			dwStyle |= WS_VISIBLE;
		else
			dwStyle |= WS_CAPTION;

		SetWindowLong(hWnd, GWL_STYLE, dwStyle);
		pkAppWindow->SetWindowStyle(dwStyle);


		unsigned int uiRefreshRate = NiDX9Renderer::REFRESHRATE_DEFAULT;
		unsigned int uiPresentationInterval 
			= NiDX9Renderer::PRESENT_INTERVAL_ONE;

		SetFullscreen(!bFullScreen);

		
		pkDX9Render->LockRenderer();


		NiDX9Renderer::RecreateStatus enRecreateStatus = pkDX9Render->Recreate(pkAppWindow->GetWidth(), pkAppWindow->GetHeight(),
			GetFullscreen() ? NiDX9Renderer::USE_FULLSCREEN :NiDX9Renderer::USE_NOFLAGS,
			pkAppWindow->GetWindowReference(),				
			NiDX9Renderer::FBFMT_UNKNOWN,
			NiDX9Renderer::DSFMT_UNKNOWN, 
			(NiDX9Renderer::PresentationInterval)uiPresentationInterval,
			NiDX9Renderer::SWAPEFFECT_DEFAULT,
			NiDX9Renderer::FBMODE_DEFAULT, 1,
			uiRefreshRate);				


		if (enRecreateStatus == NiDX9Renderer::RECREATESTATUS_FAILED)
		{
			return false;
		}
		else if(enRecreateStatus == NiDX9Renderer::RECREATESTATUS_OK)
		{
			g_fXUIScale = pkAppWindow->GetWidth()/1024.f;
			g_fYUIScale = pkAppWindow->GetHeight()/768.f;
		}
		//g_fOriginWidth = pkAppWindow->GetWidth();
		//g_fOriginHeight= pkAppWindow->GetHeight();

		g_fXScale = (float(pkAppWindow->GetWidth()))/g_fOriginWidth;
		g_fYScale = (float(pkAppWindow->GetHeight()))/g_fOriginHeight;

		if (bFullScreen)
		{
			//g_fXScale *= g_fXScale;
			//g_fYScale *= g_fYScale;
			//D3D9::m_Width = pkAppWindow->GetWidth();
			//D3D9::m_Height =pkAppWindow->GetHeight();


			RECT rcOrigin;

			GetWindowRect(hWnd, &rcOrigin);
			int left = rcOrigin.left;
			int top = rcOrigin.top;
			RECT rcWindow = { 0, 0, pkAppWindow->GetWidth(), pkAppWindow->GetHeight() };
			AdjustWindowRect(&rcWindow, dwStyle, FALSE);
			//
			MoveWindow(hWnd, left, top, rcWindow.right - rcWindow.left, rcWindow.bottom - rcWindow.top, TRUE);
		}
		m_bKeyF7 = false;
		pkDX9Render->UnlockRenderer();
	}

	return true;
}

void GameMain::OnSize(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	bool bFullScreen = GetFullscreen();
	if (!bFullScreen)
	{
		if (wParam == VK_F8 && lParam == 0x00420001)
		{
			m_bKeyF8 = true;			
		}
		if (WM_SIZE == message)
		{
			int w = LOWORD(lParam); 
			int h = HIWORD(lParam);

			D3D9::m_Width = w;
			D3D9::m_Height =h;

			g_fXScale = (float(w))/g_fOriginWidth;
			g_fYScale = (float(h))/g_fOriginHeight;

			NiAppWindow * pkAppWindow = GetMainApp()->GetAppWindow();
			pkAppWindow->SetWidth(w);
			pkAppWindow->SetHeight(h);

			DWORD dwStyle = pkAppWindow->GetWindowStyle();

			RECT rcOrigin;

			GetWindowRect(hWnd, &rcOrigin);
			int left = rcOrigin.left;
			int top = rcOrigin.top;
			RECT rcWindow = { 0, 0, w, h };
			AdjustWindowRect(&rcWindow, dwStyle, FALSE);
			//
			MoveWindow(hWnd, left, top, rcWindow.right - rcWindow.left, rcWindow.bottom - rcWindow.top, TRUE);
		}
	}
	
	if (wParam == VK_F7 && lParam == 0x00410001)
	{
		m_bKeyF7 = true;		
	}
	
	
}

STDMETHODIMP GameMain::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	OnSize(hWnd, message, wParam, lParam);
	if (CGPControl::Get())
		CGPControl::Get()->WndProc(hWnd, message, wParam, lParam);
	return S_OK;
}
//---------------------------------------------------------------------------
void GameMain::ResetFrameTimings()
{
    m_fUpdateTime = 0.0f;
    m_fCullTime = 0.0f;
    m_fRenderTime = 0.0f;
    m_fAITime = 0.0f;
    m_fAnimTime = 0.0f;
    m_fVisableObjects = 0.0f;
}
//---------------------------------------------------------------------------
void GameMain::Reset()
{
    /* */
}
//---------------------------------------------------------------------------
void GameMain::DisplaySplash(const char* pcName)
{	
    NiTexture::FormatPrefs kPrefs;
    kPrefs.m_eMipMapped = NiTexture::FormatPrefs::NO;

    NiSourceTexture* pkSourceTexture = 
        NiSourceTexture::Create(ConvertMediaFilename(pcName), kPrefs);

    NiScreenTexturePtr spLoadGame = NiNew NiScreenTexture(pkSourceTexture);

    if (ms_uiScreenWidth <= pkSourceTexture->GetWidth())
    {
        spLoadGame->AddNewScreenRect(0, 0, ms_uiScreenWidth, ms_uiScreenHeight,
            0, 0);
    }
    else
    {
        spLoadGame->AddNewScreenRect(0, 0, ms_uiScreenWidth, ms_uiScreenHeight,
            pkSourceTexture->GetHeight(), pkSourceTexture->GetWidth());
        spLoadGame->AddNewScreenRect(
            (ms_uiScreenHeight - pkSourceTexture->GetHeight()) >> 1, 
            (ms_uiScreenWidth - pkSourceTexture->GetWidth()) >> 1,
            ms_uiScreenWidth, ms_uiScreenHeight, 0, 0);
    }

    m_spRenderer->BeginFrame();
    m_spRenderer->BeginUsingDefaultRenderTargetGroup(NiRenderer::CLEAR_ALL);
    m_spRenderer->SetScreenSpaceCameraData();

    spLoadGame->Draw(m_spRenderer);
    
	m_spRenderer->EndUsingRenderTargetGroup();
    m_spRenderer->EndFrame();
    m_spRenderer->DisplayFrame();
}
//---------------------------------------------------------------------------
