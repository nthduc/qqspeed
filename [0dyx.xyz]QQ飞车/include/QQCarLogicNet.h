#pragma once




class IQQCarLogicNet
{
public:
	virtual void OnNetMessage( UINT MsgID, void * pBody ) = 0;
};

class IQQCarLogicEvent
{
public:
	virtual void OnEvent( UINT EventID, void * pBody ) = 0;
};