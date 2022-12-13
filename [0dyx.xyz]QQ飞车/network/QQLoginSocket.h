#pragma once

#include "TcpSocket.h"
#include "MultiSvrTcpSocket.h"
#include "Network/QQCrypt.h"
#include "Network/QQMsgStu.h"

typedef struct tagQQLoginInfo
{
	UINT	uin;
	BYTE	byPwd[PWD_LEN];
	INT		nHashType;
}QQLoginInfo;

class CQQLoginSocket : //public CTcpSocket
	public CMultiSvrTcpSocket
{
public: //Íâ²¿º¯Êý
	CQQLoginSocket()
	{
		memset(&m_QQUserData, 0, sizeof(QQUSERDATA));
	}

	virtual ~CQQLoginSocket(){}

	bool Init(const QQLoginInfo* LoginInfo);

	QQUSERDATA* GetUserData()
	{
		if ( GetResponse() )
			return &m_QQUserData;
		return NULL;
	}

	// CMultiSvrTcpSocket
	virtual bool Login();
	virtual ServerEntity GetEntity() { return eQQServer; }
	virtual void OnMyConnect(int nErrorCode);
	virtual void OnMyReceive(BYTE *Data, int DataLen);
	virtual void OnMyClose(int nErrorCode);
	virtual void OnMySend(int nErrorCode);

private:
	QQLoginInfo	m_LoginInfo;
	QQUSERDATA	m_QQUserData;
};


