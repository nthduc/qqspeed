// ConsSocket.h: interface for the CConsSocket class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CONSSOCKET_H__1522315C_B23D_4653_A164_BFB32EBB11C7__INCLUDED_)
#define AFX_CONSSOCKET_H__1522315C_B23D_4653_A164_BFB32EBB11C7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//#include "stdafx.h"
#include "Winsock.h"
#pragma comment(lib,"Ws2_32.lib")

class CConsSocket  
{
public:
	static WSADATA* GetWsaData();
	static void ShutDown();
	static int GetLocalPort();
	static const char* GetLocalIP();
	static void StartUp();
	CConsSocket();
	virtual ~CConsSocket();

private:
	static WSADATA m_stWsaData;
	static char*    m_scLocalIP;
	static int     m_nPort;
};

typedef void(*OnRecvType)(char* buffer,int len);

#endif // !defined(AFX_CONSSOCKET_H__1522315C_B23D_4653_A164_BFB32EBB11C7__INCLUDED_)
