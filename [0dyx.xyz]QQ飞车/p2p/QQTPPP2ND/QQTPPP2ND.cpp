// QQTPPP2ND.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "QQTPPP2ND.h"
#include "DownloadLog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//
//	Note!
//
//		If this DLL is dynamically linked against the MFC
//		DLLs, any functions exported from this DLL which
//		call into MFC must have the AFX_MANAGE_STATE macro
//		added at the very beginning of the function.
//
//		For example:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// normal function body here
//		}
//
//		It is very important that this macro appear in each
//		function, prior to any calls into MFC.  This means that
//		it must appear as the first statement within the 
//		function, even before any object variable declarations
//		as their constructors may generate calls into the MFC
//		DLL.
//
//		Please see MFC Technical Notes 33 and 58 for additional
//		details.
//

/////////////////////////////////////////////////////////////////////////////
// CQQTPPP2NDApp

BEGIN_MESSAGE_MAP(CQQTPPP2NDApp, CWinApp)
	//{{AFX_MSG_MAP(CQQTPPP2NDApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CQQTPPP2NDApp construction

CQQTPPP2NDApp::CQQTPPP2NDApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CQQTPPP2NDApp object

CQQTPPP2NDApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CQQTPPP2NDApp initialization]

int g_outputstat = 0;
int g_statInterval = 0;

int s_sndlasttime = 0;

int s_sndlastpacketSize = 0;
int s_sndpacketSize = 0;

int s_sndlastPacketCount = 0;
int s_sndpacketCount = 0;

int s_revlasttime = 0;

int s_revlastpacketSize = 0;
int s_revpacketSize = 0;

int s_revlastPacketCount = 0;
int s_revpacketCount = 0;
 

void RecvDataLog(int buflen)
{
	if (0 == g_outputstat) return;

	s_revpacketCount++;
	s_revpacketSize += buflen;

	if (s_revlasttime == 0)
	{
		s_revlasttime = GetTickCount();
	}
	else if ((int)GetTickCount() - s_revlasttime >= g_statInterval)
	{
		float time = (GetTickCount() - s_revlasttime) / 1000.0f;
		int size = s_revpacketSize - s_revlastpacketSize;
		int count = s_revpacketCount - s_revlastPacketCount;

		DLOG_WARNING("recv per second : size = %d, count = %d", int(size / time), int(count / time));

		s_revlasttime = GetTickCount();
		s_revlastpacketSize = s_revpacketSize;
		s_revlastPacketCount = s_revpacketCount;
	}
}

void SendDataLog(int buflen)
{
	if (0 == g_outputstat) return;
	
	s_sndpacketCount++;
	s_sndpacketSize += buflen;
	
	if (s_sndlasttime == 0)
	{
		s_sndlasttime = GetTickCount();
	}
	else if ((int)GetTickCount() - s_sndlasttime >= g_statInterval)
	{
		float time = (GetTickCount() - s_sndlasttime) / 1000.0f;
		int size = s_sndpacketSize - s_sndlastpacketSize;
		int count = s_sndpacketCount - s_sndlastPacketCount;
		
		DLOG_WARNING("send per second : size = %d, count = %d", int(size / time), int(count / time));
		
		s_sndlasttime = GetTickCount();
		s_sndlastpacketSize = s_sndpacketSize;
		s_sndlastPacketCount = s_sndpacketCount;
	}
}

BOOL CQQTPPP2NDApp::InitInstance()
{
	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}

	CString strFileName = GetRootPath() + "netstatic.ini";

	g_statInterval = GetPrivateProfileInt("public", "p2pinterval", 5000, strFileName);
	g_outputstat = GetPrivateProfileInt("public", "p2poutput", 0, strFileName);

	// Register all OLE server (factories) as running.  This enables the
	//  OLE libraries to create objects from other applications.
	COleObjectFactory::RegisterAll();
	
	#ifdef _ENABLE_QQT_LOG_
	char szPath[MAX_PATH];
	_snprintf(szPath, MAX_PATH, "%s%s", ::GetRootPath(), "log\\QQTp2p\\");
	QQT_SET_LOG_PATH(szPath);
	#endif

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// Special entry points required for inproc servers

STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID* ppv)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return AfxDllGetClassObject(rclsid, riid, ppv);
}

STDAPI DllCanUnloadNow(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return AfxDllCanUnloadNow();
}

// by exporting DllRegisterServer, you can use regsvr.exe
STDAPI DllRegisterServer(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	COleObjectFactory::UpdateRegistryAll();
	return S_OK;
}
