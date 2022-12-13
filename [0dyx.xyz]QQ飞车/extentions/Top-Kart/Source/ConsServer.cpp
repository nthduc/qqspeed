// ConsServer.cpp: implementation of the CConsServer class.
//
//////////////////////////////////////////////////////////////////////
#include "Top-KartPCH.h"
#include "ConsServer.h"
#include "stdio.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#define MAX_BUFFER 4096

static DWORD WINAPI LoadMainFunc( void *pdata )
{
	static char buffer[MAX_BUFFER];
	static int  len;

	CConsServer* pServer = (CConsServer*)pdata;

	for(;;)
	{

		if(pServer->m_nQuit)
		{
			return 0;
		}

		pServer->Listen();

		if(pServer->m_stSendSocket != INVALID_SOCKET)
		{
			len = recv(pServer->m_stSendSocket,buffer,MAX_BUFFER,0);

			if(len > 0)
			{
				buffer[len] = '\0';
				if(pServer->m_recvfunc)
				{
					//接收到
					(*pServer->m_recvfunc)(buffer,len);

					pServer->SendOutMsg();
					pServer->SendErrMsg();

				}	
			}
			else
			{
				int err = WSAGetLastError();
					
				WaitForSingleObject(pServer->m_hMutex,INFINITE);
				
				pServer->CloseHandle();
				if(pServer->m_nQuit == false)
				{
					pServer->CoreInit();

				}
				ReleaseMutex(pServer->m_hMutex);
			}
		} 
	}
};

CConsServer::CConsServer(OnRecvType recvfunc)
{
	m_bConnected = false;
	m_stSocket = INVALID_SOCKET;
	m_bstart = false;
	m_stConnectAddrLen = sizeof(m_stConnectAddr);
	m_stSendSocket = INVALID_SOCKET;
    m_recvfunc = recvfunc;

	m_hMutex = CreateMutex( NULL, FALSE, NULL );

	astderrbuf = astderrbuf_pre + 1;
    astdoutbuf = astdoutbuf_pre + 1;

	astdoutbuf_pre[0] = 'o';
	astderrbuf_pre[0] = 'e';

	//重新定位输入，输出流
	setvbuf(stderr, astderrbuf,_IOFBF ,ERRBUF_LENGTH );
	setvbuf(stdout, astdoutbuf,_IOFBF ,OUTBUF_LENGTH );
	ResetBufferErr();
	ResetBufferOut();

	m_bSaveFile = true;
}

CConsServer::~CConsServer()
{
	close();
	::CloseHandle( m_hMutex );
}



void CConsServer::Init()
{
	CoreInit();

	DWORD threadid = 0;
	m_hThread = CreateThread( NULL, 0, LoadMainFunc, this, CREATE_SUSPENDED , &threadid );
	//SetThreadPriority( m_hThread, THREAD_PRIORITY_IDLE );
	//SetThreadPriority( m_hThread, THREAD_PRIORITY_LOWEST );
	SetThreadPriority( m_hThread, THREAD_PRIORITY_BELOW_NORMAL );

	ResumeThread(m_hThread);
}

void CConsServer::Listen()
{
	if(!m_bstart)
	{
		return;
	}

	if(!m_bConnected)
	{
		int rt = listen(m_stSocket,1);
		
		m_stSendSocket = accept(m_stSocket,(sockaddr*)&m_stConnectAddr,&m_stConnectAddrLen);

		if(m_stSendSocket != INVALID_SOCKET)
		{
			m_bConnected = true;
		}
		else
		{
			int err = WSAGetLastError();
		}
	}
}

void CConsServer::close()
{
	WaitForSingleObject(m_hMutex,INFINITE);
	m_nQuit = true;
	CloseHandle();
	ReleaseMutex(m_hMutex);

	if(m_hThread != NULL)
	{
		if( WaitForSingleObject(m_hThread, 2000) == WAIT_TIMEOUT)
		{
			TerminateThread( m_hThread, 1 );
		}
		else 
		{
			::CloseHandle( m_hThread );
		}
		m_hThread = NULL;
	}
}

bool CConsServer::Send(void *buffer, int len)
{
	if(m_stSendSocket == INVALID_SOCKET)
	{
		return false;
	}

	int rt = send(m_stSendSocket,(char*)buffer,len,0);

	if(rt != 0)
	{
		int err = WSAGetLastError();
	}
	return true;
}

void CConsServer::CloseHandle()
{
	if(m_stSendSocket != INVALID_SOCKET)
	{
		closesocket(m_stSendSocket);
		m_stSendSocket = INVALID_SOCKET;
	}

	if(m_stSocket != INVALID_SOCKET)
	{
		closesocket(m_stSocket);
		m_stSocket = INVALID_SOCKET;
	}
}
 

void CConsServer::CoreInit()
{
	m_stSocket = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);

	struct sockaddr_in tcpaddr;
	tcpaddr.sin_family = AF_INET;
	tcpaddr.sin_port = htons(CConsSocket::GetLocalPort());
	tcpaddr.sin_addr.s_addr = inet_addr(CConsSocket::GetLocalIP());

	//unsigned long ul = 1;
	//绑定
	int rt = bind(m_stSocket,(const sockaddr*)&tcpaddr,sizeof(tcpaddr));
	//ioctlsocket(m_stSocket,FIONBIO,&ul);

	if(rt != 0)
	int err = WSAGetLastError();

	m_bstart = true;
	m_nQuit = false;
	m_bConnected = false;
}

void CConsServer::OutPutDugInfo(char *info)
{
	if(m_recvfunc)
	{
		(*m_recvfunc)(info,strlen(info));
	}
}

void CConsServer::ResetBufferErr()
{
	memset(astderrbuf,0,ERRBUF_LENGTH);
}
void CConsServer::ResetBufferOut()
{
	memset(astdoutbuf,0,OUTBUF_LENGTH);
}
void CConsServer::SendErrMsg()
{
	if(astderrbuf[0]!=0)
	{
		int len = strlen(astderrbuf);

		if(len > 0)
		{
			Send(astderrbuf_pre,len + 1);
			
			if(m_bSaveFile)
			{
				fflush(stderr);
			}

			memset(astderrbuf,0,ERRBUF_LENGTH);
		}
	}
}

void CConsServer::SendOutMsg()
{
	if(astdoutbuf[0]!=0)
	{
		int len = strlen(astdoutbuf);

		if(len > 0)
		{
			Send(astdoutbuf_pre,len + 1);
			if(m_bSaveFile)
			{
				fflush(stdout);
			}
			memset(astdoutbuf,0,OUTBUF_LENGTH);
		}
    }
}

void  CConsServer::SaveErr()
{
	if (astderrbuf[0]!=0)
	{
		fflush(stderr);
		memset((char *)&astderrbuf,0,ERRBUF_LENGTH);
	}
}
void  CConsServer::SaveOut()
{
	if(astdoutbuf[0]!=0)
	{
		fflush(stdout);
		memset((char *)&astdoutbuf,0,OUTBUF_LENGTH);
	}
}