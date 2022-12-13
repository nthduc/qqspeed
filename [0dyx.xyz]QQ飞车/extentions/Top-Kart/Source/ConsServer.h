// ConsServer.h: interface for the CConsServer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CONSSERVER_H__F5968564_6AF2_4E1A_81CB_F09627D8CCBB__INCLUDED_)
#define AFX_CONSSERVER_H__F5968564_6AF2_4E1A_81CB_F09627D8CCBB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ConsSocket.h"

#define ERRBUF_LENGTH 1000
#define OUTBUF_LENGTH 20000

class CConsServer  
{
public:
	void OutPutDugInfo(char *info);
	void CoreInit();

	void CloseHandle();
	bool Send(void* buffer,int len);
	void close();
	void Listen();
	void Init();
	CConsServer(OnRecvType recvfunc);
	virtual ~CConsServer();
 
	SOCKET m_stSocket;
	bool   m_bConnected;
	sockaddr_in m_stConnectAddr;
	int      m_stConnectAddrLen;
	bool     m_bstart;
	SOCKET m_stSendSocket;


	HANDLE		m_hMutex; 
	HANDLE		m_hThread;
	bool		m_nQuit;

	OnRecvType  m_recvfunc;


	char  astderrbuf_pre[ERRBUF_LENGTH + 1];
	char  astdoutbuf_pre[OUTBUF_LENGTH + 1];

	char*  astderrbuf;
	char*  astdoutbuf;

	void  ResetBufferErr();
	void  ResetBufferOut();
	void  SendErrMsg();
	void  SendOutMsg();
	void  SaveErr();
	void  SaveOut();

	bool   m_bSaveFile;

	bool EnableSaveFile(bool bSave){m_bSaveFile = bSave;};
	bool IsEnableSaveFile(){return m_bSaveFile;};
};

#endif // !defined(AFX_CONSSERVER_H__F5968564_6AF2_4E1A_81CB_F09627D8CCBB__INCLUDED_)
