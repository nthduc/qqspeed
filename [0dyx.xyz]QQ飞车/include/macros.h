#pragma once

interface IEventSupport
{
	STDMETHOD(OnMessage)() PURE;
};

typedef void (IEventSupport::*MsgProc)(const void*);

typedef struct tagMSG_MAP_ITEM
{
	UINT	MsgID;
	MsgProc	Proc;
}MSG_MAP_ITEM;

#define DECLARE_MESSAGE_MAP_IQ	const MSG_MAP_ITEM* GetMessageMap();	\
								STDMETHODIMP OnMessage() { return S_OK; }

#define BEGIN_MESSAGE_MAP_IQ(x)	const MSG_MAP_ITEM* x::GetMessageMap()	{	\
	static const MSG_MAP_ITEM _Map[] = {

#define ON_COMMAND_IQ(MsgID, Proc)	{ (UINT)MsgID, (MsgProc)&Proc },

#define END_MESSAGE_MAP_IQ	{ 0, NULL } }; return _Map;	}

#define DISPATCH_MESSAGE_IQ(ID, Param)	\
	const MSG_MAP_ITEM* _Map = GetMessageMap();	\
	int i = 0;	\
	while(_Map[i].MsgID != 0)	\
	{	\
		if ( _Map[i].MsgID == ID )		\
		{	\
			MsgProc pfnProc = _Map[i].Proc;	\
			IEventSupport* pThis = (IEventSupport*)(int)this;\
			(pThis->*pfnProc)(Param);	\
		}	\
		i++;	\
	}

#include "iqnetmgr_i.h"
#include "iInputProducer.h"

inline HRESULT Register2NetMgr(IIQNetMgr* Net, IIQNetSink* Sink, UINT* Cookie)
{
	if ( !Net )
		return E_FAIL;
	return Net->GetEventListenerContainer()->RegisterListener(Sink, Cookie);
}

inline HRESULT Register2Input(IInputProducer* Input, IInputListener* Sink, UINT* Cookie)
{
	if ( !Input )
		return E_FAIL;
	return Input->RegisterListener(Sink, Cookie);
}