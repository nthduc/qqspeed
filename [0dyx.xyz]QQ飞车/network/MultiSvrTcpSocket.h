#ifndef __MULTISVRTCPSOCKET_H__
#define __MULTISVRTCPSOCKET_H__

#include "Network/Network_i.h"
#include "typedef.h"
#include "common.h"
#include "senddata.h"
#include "Timer.h"
#include "TimerMgr.h"
#include "list"
using namespace std;

interface ISocketSinkProxy;
typedef list<SOCKET_CREATE_INFO>	SOCKET_CREATE_INFO_LIST;

class CMultiSvrTcpSocket : public CAsyncSocket, public CTimerEventBase
{
public:
	CMultiSvrTcpSocket();
	~CMultiSvrTcpSocket();

	void Create(int nHeadLenBytes = sizeof(int));
	bool Connect();
	void Close();
	void AddSocketInfo(SOCKET_CREATE_INFO*);
	void ClearSocketInfoList();
	SOCKET_CREATE_INFO_LIST& GetSocketInfoList();
	void AddSocketInfoList(SOCKET_CREATE_INFO_LIST&);
	void SetSocketInfoList(SOCKET_CREATE_INFO_LIST&);
	void Reset();
	bool SendData(const void* lpBuf, int nBufLen, int nFlags = 0);
	void SetSinkProxy(ISocketSinkProxy* pSink);
	SOCKET_CREATE_INFO*	GetSocketInfo();
	bool GetResponse(){ return m_bResponse; }

private:
	//CAsyncSocket
	virtual void OnReceive(int nErrorCode);
	virtual void OnConnect(int nErrorCode);
	virtual void OnClose(int nErrorCode);
	virtual void OnSend(int nErrorCode);

private:
	//CTimer
	virtual void OnTimer(DWORD dwHandle);

protected: 
	//overidables
	virtual void OnMyReceive(BYTE* Data, int DataLen){}
	virtual void OnMyConnect(int nErrorCode){}
	virtual void OnMyClose(int nErrorCode){}
	virtual void OnMySend(int nErrorCode){}

	//子类需要继承这个以便登陆使用
	virtual bool Login() { return false; }
	virtual ServerEntity GetEntity() = 0;

private:
	void ReceiveData();
	bool SocketInfoMoveNext();
	
private:
	SENDDATA_LIST			m_DataBuffer;
	SOCKET_CREATE_INFO_LIST	m_listSocketInfo;
	int				m_nCurSvrIndex;
	int				m_nCurPortIdx;
	ConnectState	m_nConnectState;
	int				m_nHeadLenBytes;
	int				m_nLastLeftSize;
	bool			m_bFirstSend;
	bool			m_bResponse;

protected:
	ISocketSinkProxy* m_pSinkProxy;

};
#endif