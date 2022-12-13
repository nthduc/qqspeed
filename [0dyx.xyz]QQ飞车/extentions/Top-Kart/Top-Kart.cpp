////////////////////////////////////////////////
//     FileName:    Top-Kart.cpp
//     Author:      Fish
//     Date:        2006-12-13 16:41:18
//     Description: []
////////////////////////////////////////////////

#include "Top-KartPCH.h"
#include "Top-Kart.h"
#include "vfilewrap.h"
#include "Source/Platform/GlobalInterface.h"
#include "Source/Platform/sc.h"
#include "Source/Platform/MsgDataCache.h"
#include "Source/Platform/ScriptEvent.h"
#include "Source/Platform/AntibotImpl.h"
#include "framework/gui_i.h"
#include "Source/MessageHouse.h"
#include "Source/GameMain.h"
#include "Source/GPNetInterCommn.h"
#include "Source/Avatar/AvatarNet.h"
#include "Source/Task/TaskNet.h"

extern IDisplay*	g_pDisplay;
NiApplication* NiInitializeWrap(HWND);
void NiMainLoopWrap(NiApplication* theApp);
void NiTerminateWrap(NiApplication* theApp);
// for determining if there are 'object leaks'
static unsigned int ms_uiInitialCount = 0;

DEFINE_FILESYSTEM_V

CTopKart::CTopKart():
m_pApp(NULL),
m_pNiApp(NULL),
m_pScriptEvent(NULL),
m_pDataCache(NULL),
m_pMessageHouse(NULL),
m_pGPNetIntercommn(NULL),
m_pAvatarNet(NULL),
m_pTaskNet(NULL),
m_pAntibot(NULL),
m_UIMiscMgr(&m_UIMiscCallback)
{
}

STDMETHODIMP CTopKart::Load(IGameApp* App)
{
	HRESULT hr = E_FAIL;
	KK_CHECK_POINTER(App);
	{
		m_pApp = App;
		hr = InitPlatform();
		KK_CHECK(hr);
		hr = InitEngine();
		KK_CHECK(hr);
		hr = InitObjects();
		KK_CHECK(hr);
	}
KK_EXIT:
	return hr;
}

STDMETHODIMP CTopKart::Active()
{
	return S_OK;
}

STDMETHODIMP CTopKart::DeActive()
{
	return S_OK;
}

STDMETHODIMP CTopKart::UnLoad()
{
	m_UIMiscMgr.UnInit();
	if ( GlobalInterface::m_pGUI )
	{
		GlobalInterface::m_pGUI->DestroyDevice();
		GlobalInterface::m_pGUI = NULL;
	}

	if ( m_pScriptEvent )
		m_pScriptEvent->AppUnInit();
	RELEASE(m_pScriptEvent);

	if ( m_pAntibot )
		m_pAntibot->AppUnInit();
	RELEASE(m_pAntibot);

	if ( m_pDataCache )
		m_pDataCache->AppUnInit();
	RELEASE(m_pDataCache);

	if ( m_pMessageHouse )
		m_pMessageHouse->AppUnInit();
	RELEASE(m_pMessageHouse);

	if ( m_pGPNetIntercommn )
		m_pGPNetIntercommn->AppUnInit();
	RELEASE(m_pGPNetIntercommn);

	if ( m_pAvatarNet )
		m_pAvatarNet->AppUnInit();
	RELEASE(m_pAvatarNet);

	if ( m_pTaskNet )
		m_pTaskNet->AppUnInit();
	RELEASE(m_pTaskNet);

	if ( m_pNiApp )
	{
		NiTerminateWrap(m_pNiApp);
		m_pNiApp = NULL;
	}
	return S_OK;
}

STDMETHODIMP CTopKart::GetName(LPCSTR* pName)
{
	*pName = "TopKart";
	return S_OK;
}

STDMETHODIMP CTopKart::Update(DWORD dwTime)
{
	if ( m_pNiApp )
		NiMainLoopWrap(m_pNiApp);
	return S_OK;
}

STDMETHODIMP CTopKart::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if ( m_pNiApp )
	{
		((GameMain*)m_pNiApp)->WndProc(hWnd, message, wParam, lParam);
	}
	return S_OK;
}

HRESULT CTopKart::InitPlatform()
{
	HRESULT hr = E_FAIL;
	KK_CHECK_POINTER(m_pApp);
	{
		GlobalInterface::m_pApp = m_pApp;
		CComPtr<IFileSystem_V> spVFS;
		hr = m_pApp->GetService(IID_IFileSystem_V, (void**)&spVFS);
		KK_CHECK(hr);
		vfinit(spVFS);

		CComPtr<INetwork> spNetwork;
		hr = m_pApp->GetService(IID_INetwork, (void**)&spNetwork);
		KK_CHECK(hr);
		GlobalInterface::m_pNetwork = spNetwork;

		CComPtr<IQQTppp> spQQTppp;
		hr = m_pApp->GetService(IID_IQQTppp, (void**)&spQQTppp);
		KK_CHECK(hr);
		GlobalInterface::m_pQQTppp = spQQTppp;

		CComPtr<IGameSettings> spSettings;
		hr = m_pApp->QueryInterface(IID_IGameSettings, (void**)&spSettings);
		KK_CHECK(hr);
		GlobalInterface::m_pSettings = spSettings;
	}
KK_EXIT:
	return hr;
}

HRESULT CTopKart::InitObjects()
{
	HRESULT hr = E_FAIL;
	KK_CHECK_POINTER(m_pApp);
	{
		CComPtr<IGUI> spGUI;
		hr = m_pApp->GetService(IID_IGUI, (void**)&spGUI);
		KK_CHECK(hr);
		GlobalInterface::m_pGUI = spGUI;
		hr = spGUI->CreateDevice();
		KK_CHECK(hr);
		GlobalInterface::m_pScriptState = GlobalInterface::m_pGUI->GetScriptState();
		GlobalInterface::m_pDisplay = GlobalInterface::m_pGUI->GetDisplay();
		RegisterScriptFuncs(GlobalInterface::m_pScriptState);

		m_UIMiscMgr.Init(GlobalInterface::m_pDisplay);
		GlobalInterface::m_pUIMiscMgr = &m_UIMiscMgr;

		hr = spGUI->LoadUI("res/uires/uires.lua");
		GlobalInterface::m_pGUI->EnterUILayout("entry");

		CComObject<CMsgDataCache>::CreateInstance(&m_pDataCache);
		hr = m_pDataCache->AppInit(m_pApp);
		KK_CHECK(hr);
		m_pDataCache->AddRef();
		GlobalInterface::m_pDataCache = m_pDataCache;

		//lua ui刷新时序与avatar网络消息有矛盾，调到最后注册 pighuang 20070328
		//CComObject<CScriptEvent>::CreateInstance(&m_pScriptEvent);
		//hr = m_pScriptEvent->AppInit(m_pApp);
		//KK_CHECK(hr);
		//m_pScriptEvent->AddRef();

		CComObject<CAntibotImpl>::CreateInstance(&m_pAntibot);
		hr = m_pAntibot->AppInit(m_pApp);
		KK_CHECK(hr);
		m_pAntibot->AddRef();

		CComObject<CMessageHouse>::CreateInstance(&m_pMessageHouse);
		hr = m_pMessageHouse->AppInit(m_pApp);
		KK_CHECK(hr);
		m_pMessageHouse->AddRef();

		CComObject<CGPNetIntercommn>::CreateInstance(&m_pGPNetIntercommn);
		hr = m_pGPNetIntercommn->AppInit(m_pApp);
		KK_CHECK(hr);
		m_pGPNetIntercommn->AddRef();

		CComObject<CAvatarNet>::CreateInstance(&m_pAvatarNet);
		hr = m_pAvatarNet->AppInit(m_pApp);
		KK_CHECK(hr);
		m_pAvatarNet->AddRef();

		CComObject<CTaskNet>::CreateInstance(&m_pTaskNet);
		hr = m_pTaskNet->AppInit(m_pApp);
		KK_CHECK(hr);
		m_pTaskNet->AddRef();

		CComObject<CScriptEvent>::CreateInstance(&m_pScriptEvent);
		hr = m_pScriptEvent->AppInit(m_pApp);
		KK_CHECK(hr);
		m_pScriptEvent->AddRef();		
	}
KK_EXIT:
	return hr;
}

HRESULT CTopKart::InitEngine()
{
	HRESULT hr = E_FAIL;
	KK_CHECK_POINTER(m_pApp);
	{
		HWND hWnd;
		hr = m_pApp->GetMainWnd(&hWnd);
		KK_CHECK(hr);

		m_pNiApp = NiInitializeWrap(hWnd);
		hr = (m_pNiApp == NULL ? E_FAIL : S_OK);
	}
KK_EXIT:
	return hr;
}

//---------------------------------------------------------------------------
NiApplication* NiInitializeWrap(HWND hWnd)
{
	LPSTR cmdline = GetCommandLine();
	// for determining if there are 'object leaks'
	//unsigned int uiInitialCount = NiRefObject::GetTotalObjectCount();

	ms_uiInitialCount = NiRefObject::GetTotalObjectCount();

	NiInit();

	// If cmdline.txt file exists in the work directory, read it and 
	// set it as the command line
	NiFile* pkCmdlineFile = NULL;
	if (!cmdline || strlen(cmdline) == 0)
	{
		pkCmdlineFile = NiNew NiFile("./cmdline.txt", NiFile::READ_ONLY);
	}

	if (pkCmdlineFile && *pkCmdlineFile)
	{
		// artificial 1024-character limit
		char* pcCmdline = NiAlloc(char, 1024);
		int iLength = 0;
		char c;
		// Read until limit is reached or end of file or end of string
		while (iLength < 1023) 
		{
			if (!(pkCmdlineFile->Read(&c, 1) || c == '\0'))
				break;

			// Factor out newlines and carriage returns
			// Replace with ' ' in case newline seperates valid parameters
			if (c == '\n' || c == '\r')
				c = ' ';
			pcCmdline[iLength++] = c;
		}
		pcCmdline[iLength] = '\0';

		// let the application know about the command line
		NiApplication::SetCommandLine(pcCmdline);

		NiFree(pcCmdline);
	}
	else
	{
		// let the application know about the command line
		NiApplication::SetCommandLine(cmdline);
	}

	NiDelete pkCmdlineFile;

	// create the application
	NiApplication* pkTheApp = NiApplication::Create();
	if (!pkTheApp)
	{
		NiMessageBox("Unable to create application", 
			NiAppWindow::GetWindowClassName());
		NiShutdown();    
		return NULL;
	}

	HINSTANCE hInst = _Module.GetModuleInstance();
	// Set reference
	NiApplication::SetInstanceReference(hInst);

	// process the command line for renderer selection
	NiApplication::CommandSelectRenderer();

	DWORD dwStyle = GetWindowLong(hWnd, GWL_STYLE);
	NiAppWindow* pAppWindow = pkTheApp->GetAppWindow();
	pAppWindow->SetWindowReference(hWnd);
	pAppWindow->SetRenderWindowReference(hWnd);
	
	int nScreenX = GetSystemMetrics(SM_CXSCREEN);
	int nScreenY = GetSystemMetrics(SM_CYSCREEN);
	if ( !pkTheApp->GetFullscreen() &&
		(nScreenX < pAppWindow->GetWidth() ||
		nScreenY < pAppWindow->GetHeight()) )
	{
		if ( IDNO == MessageBox(hWnd, "你的屏幕分辨率太低，游戏将以全屏方式运行，确定吗？\n\n选择'取消'结束游戏。", 
			"警告", MB_YESNO | MB_ICONEXCLAMATION) )
		{
			exit(0);
		}
		pkTheApp->SetFullscreen(true);
	}
	if ( nScreenX == pAppWindow->GetWidth() &&
		nScreenY == pAppWindow->GetHeight() )
	{
		dwStyle &= ~WS_CAPTION;
	}
	if ( pkTheApp->GetFullscreen() )
	{
		dwStyle &= ~WS_VISIBLE;
	}
	SetWindowLong(hWnd, GWL_STYLE, dwStyle);
	pAppWindow->SetWindowStyle(dwStyle);

	int w = pAppWindow->GetWidth();
	int h = pAppWindow->GetHeight();
	RECT rcWindow = { 0, 0, w, h };
	AdjustWindowRect(&rcWindow, dwStyle, FALSE);
	MoveWindow(hWnd, 0, 0, rcWindow.right - rcWindow.left, rcWindow.bottom - rcWindow.top, TRUE);

	// allow the application to initialize before starting the message pump
	if (!pkTheApp->Initialize())
	{
		return NULL;
	}

	return pkTheApp;
}

void NiMainLoopWrap(NiApplication* theApp)
{
	theApp->OnIdle();
}

void NiTerminateWrap(NiApplication* theApp)
{
	theApp->Terminate();

	NiDelete theApp;

	NiShutdown();    

	// for determining if there are 'object leaks'
	
	unsigned int uiFinalCount = NiRefObject::GetTotalObjectCount();
	char acMsg[256];
	NiSprintf(acMsg, 256,
		"\nGamebryo NiRefObject counts:  initial = %u, final = %u. ", 
		ms_uiInitialCount, uiFinalCount);
	OutputDebugString(acMsg);
	if (uiFinalCount > ms_uiInitialCount)
	{
		unsigned int uiDiff = uiFinalCount - ms_uiInitialCount;
		NiSprintf(acMsg, 256, "Application is leaking %u objects\n\n", uiDiff);
		OutputDebugString(acMsg);
	}
	else
	{
		OutputDebugString("Application has no object leaks.\n\n");
	}
	
}