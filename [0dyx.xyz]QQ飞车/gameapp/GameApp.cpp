// GameApp.cpp : Defines the entry point for the application.
//
#include "stdafx.h"
#include "GameApp.h"
#include "GameAppImpl.h"
#include "common.h"
#include "stringex.h"
#include "fileop.h"
#include "shellapi.h"
#include "windows.h"
#include "Winbase.h"
#include "txtctrlr.h"
#include "filever.h"
#include "fileop.h"
#include "BugslayerUtil.h"
#include "assert.h"
#include "tenio/include/tenioapp.h"
using namespace Tenio;

#pragma comment(lib, "Imm32")
#define MAX_LOADSTRING 100


//-----------------------------------------------------------------------


typedef LONG (APIENTRY *LPSTARTPROC) (_EXCEPTION_POINTERS *ExceptionInfo );
LONG WINAPI MyUnhandledExceptionFilter ( EXCEPTION_POINTERS *ExceptionInfo );

class __cm_set_exception_handler
{
public:
	__cm_set_exception_handler()
	{

		LPCSTR szDllPath = "cmHelp.dll";
		char szModulePath[MAX_PATH] = "";
		if ( GetModulePath(NULL, szModulePath) )
		{
			SetCurrentDirectory(szModulePath);
		}

		OSVERSIONINFO osversion;
		memset(&osversion, 0, sizeof(OSVERSIONINFO));
		 osversion.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
		GetVersionEx(&osversion);
		if ( FileExists("BugslayerUtil.dll") && osversion.dwMajorVersion >= 5 )
		{
			::SetUnhandledExceptionFilter(MyUnhandledExceptionFilter);
			return;
		}

	}
	~__cm_set_exception_handler()
	{

	}
private:
	HINSTANCE m_hHelpDll;
};

__cm_set_exception_handler g_seh;



typedef BSUMDRET (_stdcall *CreateCurrentProcessCrashDump)( MINIDUMP_TYPE        eType      ,
														   char *               szFileName ,
														   DWORD                dwThread   ,
														   EXCEPTION_POINTERS * pExceptInfo );


LONG WINAPI MyUnhandledExceptionFilter ( EXCEPTION_POINTERS *ExceptionInfo )
{
#ifndef _DEBUG

	char szProduct[MAX_PATH] = "QQ飞车";

	char szVersion[MAX_PATH] = "";
	FormatVersion(szVersion, MAX_LOADSTRING, "GameApp.exe");
	CStringEx strTmp;

	HMODULE hModule = LoadLibrary("BugslayerUtil.dll");
	if ( hModule )
	{
		CreateCurrentProcessCrashDump pfnCreateCurrentProcessCrashDump = 
			(CreateCurrentProcessCrashDump)GetProcAddress(hModule, "CreateCurrentProcessCrashDumpA");

		if ( pfnCreateCurrentProcessCrashDump )
		{
			BSUMDRET eCCPMD ;
			strTmp.Format("%s%s.DMP", szProduct, szVersion);
			eCCPMD = pfnCreateCurrentProcessCrashDump( MiniDumpWithHandleData ,
				(char*)(strTmp.c_str()),//L"ExceptDumpW.DMP"     ,
				GetCurrentThreadId ( ) ,
				ExceptionInfo           );

			if( eDUMP_SUCCEEDED != eCCPMD )
			{
				strTmp.Format("%s%s.DMP", szProduct, szVersion);
				eCCPMD = pfnCreateCurrentProcessCrashDump ( MiniDumpWithHandleData ,
					(char*)(strTmp.c_str()),//L"ExceptDumpW.DMP"     ,
					0                      ,
					ExceptionInfo           );
			}
			strTmp.Format("程序出现异常，请把游戏目录下的'%s%s.dmp'提供给开发者", szProduct, szVersion);
			MessageBox( 0,strTmp.c_str() , szProduct, MB_OK);
		}
	}
#endif

	return ( EXCEPTION_EXECUTE_HANDLER ) ;
}




//-----------------------------------------------------------------------



// Global Variables:
HINSTANCE	hInst;								// current instance
TCHAR		szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR		szWindowClass[MAX_LOADSTRING];			// the main window class name
HWND		g_hWnd;
BOOL		g_bQuit = FALSE;
CComObject<CGameAppImpl>*	g_spGameApp = NULL;

CComModule _Module;

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

void PreExec();
void PostExec();
HRESULT RegistComponent(LPCSTR);
HRESULT UnRegistComponent(LPCSTR lpszComponentDll);

void __cdecl _mypurecall()
{
	XTrace("_purecall error");
	_asm
	{
		int 3;
	}
}

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
 	// TODO: Place code here.
	{
		_set_purecall_handler(_mypurecall);

		atexit(PostExec);

		char szModulePath[MAX_PATH] = "";
		if ( GetModulePath(NULL, szModulePath) )
		{
			SetCurrentDirectory(szModulePath);
		}
		char szProductName[MAX_LOADSTRING] = "";
		if ( 0 < GetVersionStringInfo("GameApp.exe", "ProductName", -1, -1, szProductName, MAX_LOADSTRING) )
		{
			char szVer[MAX_LOADSTRING] = "";
			FormatVersion(szVer, MAX_LOADSTRING, "GameApp.exe");
			_sntprintf(szTitle, MAX_LOADSTRING, "%s%s", szProductName, szVer);
		}
	}
	PreExec();
	SetCurrentDirectory("../");

	MSG msg;
	HACCEL hAccelTable;
	HRESULT hr = S_OK;

	// Initialize global strings
	if ( strlen(szTitle) == 0 )
		LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_GAMEAPP, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow)) 
	{
		return FALSE;
	}
	hAccelTable = LoadAccelerators(hInstance, (LPCTSTR)IDC_GAMEAPP);

	CoInitialize(NULL);
	{
		CComObject<CGameAppImpl>::CreateInstance(&g_spGameApp);
		g_spGameApp->AddRef();
		BOOL bInit = g_spGameApp->Init(g_hWnd);
		KK_CHECK_BOOL(bInit);
		// Main message loop:
		PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE);
		while (WM_QUIT != msg.message) 
		{
			if (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE)) 
			{
				if(GetMessage(&msg, NULL, 0, 0)) 
				{
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}
			}
			else 
			{
				g_spGameApp->Update();
				// 修改人： wincoqin
				// 时间: 2007/1/22
				// 目的：这个地方不需要限桢
				//Sleep(1);
			}
		}
		g_bQuit = TRUE;
		g_spGameApp->Release();
	}
KK_EXIT:
	TenUninitialize();
	CoUninitialize();
	return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
//    This function and its usage are only necessary if you want this code
//    to be compatible with Win32 systems prior to the 'RegisterClassEx'
//    function that was added to Windows 95. It is important to call this function
//    so that the application will get 'well formed' small icons associated
//    with it.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX); 

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= (WNDPROC)WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, (LPCTSTR)IDI_GAMEAPP);
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= (LPCTSTR)IDC_GAMEAPP;
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, (LPCTSTR)IDI_SMALL);

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HANDLE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable
   
   DWORD		dwExStyle;				// Window Extended Style
   DWORD		dwStyle;				// Window Style

   dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;			// Window Extended Style
   dwStyle = WS_OVERLAPPEDWINDOW;							// Windows Style
   dwStyle &= ~WS_MAXIMIZEBOX;			//没有最大化按钮
   //dwStyle &= ~WS_THICKFRAME;			//不能改变大小

   HWND hWnd = CreateWindowEx(dwExStyle, szWindowClass, szTitle, 
	   dwStyle | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, 0, 0, 
	   0,
	   0,
	   NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   g_hWnd = hWnd;
   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, unsigned, WORD, LONG)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	

	if ( g_spGameApp && !g_bQuit )
	{
		HRESULT hr = g_spGameApp->WndProc(hWnd, message, wParam, lParam);
		if ( hr != S_OK )
			return 0;
	}
	switch (message) 
	{
	case WM_COMMAND:
		wmId    = LOWORD(wParam); 
		wmEvent = HIWORD(wParam); 
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, (LPCTSTR)IDD_ABOUTBOX, hWnd, (DLGPROC)About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code here...
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_ERASEBKGND:
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Message handler for about box.
LRESULT CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG:
		return TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) 
		{
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}
		break;
	}
	return FALSE;
}

void PreExec()
{
	if ( FileExists("regist") )
	{
		CtxtCtrlr txtCtrl;
		if ( txtCtrl.Open("regist") )
		{
			while( txtCtrl.Next() )
			{
				CtxtRecordset* rs = txtCtrl.GetRecordset();
				if ( rs && rs->GetFieldCount() > 0 )
				{
					LPCSTR lpszField = rs->GetField(0);
					HRESULT hr = RegistComponent(lpszField);
					if ( FAILED(hr) )
					{
						LogText("Regist Component '%s' failed\n", lpszField);
						exit(0);
					}
					else
					{
						LogText("Regist Component '%s' succeeded\n", lpszField);
					}
				}
			}
		}
	}
}

void PostExec()
{
	/*
	char szFilePath[MAX_PATH] = "";
	FullName("regist", szFilePath);
	if ( FileExists(szFilePath) )
	{
		CtxtCtrlr txtCtrl;
		if ( txtCtrl.Open(szFilePath) )
		{
			while( txtCtrl.Next() )
			{
				CtxtRecordset* rs = txtCtrl.GetRecordset();
				if ( rs && rs->GetFieldCount() > 0 )
				{
					LPCSTR lpszField = rs->GetField(0);
					HRESULT hr = UnRegistComponent(lpszField);
					if ( FAILED(hr) )
					{
						LogText("UnRegist Component '%s' failed\n", lpszField);
						exit(0);
					}
					else
					{
						LogText("UnRegist Component '%s' succeeded\n", lpszField);
					}
				}
			}
		}
	}*/
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


HRESULT UnRegistComponent(LPCSTR lpszComponentDll)
{
	HMODULE hModule = LoadLibrary(lpszComponentDll);
	if ( !hModule )
	{
		XError("反注册组件失败,模块%s无法加载(GetLastError=%d)", lpszComponentDll, GetLastError());
		return E_FAIL;
	}
	typedef HRESULT (STDAPICALLTYPE *DllRegisterServer)(void);
	DllRegisterServer pfnDllRegisterServer = (DllRegisterServer)GetProcAddress(hModule, "DllUnregisterServer");
	if ( !pfnDllRegisterServer )
	{
		XError("反注册组件失败,模块%s无法找到注册入口点", lpszComponentDll);
		return E_NOTIMPL;
	}
	HRESULT hr = pfnDllRegisterServer();
	if ( FAILED(hr) )
	{
		XError("反注册组件失败,模块%s的DllRegisterServer返回错误,hr=0x%X", lpszComponentDll, hr);
		return E_FAIL;
	}
	FreeLibrary(hModule);
	return hr;
}