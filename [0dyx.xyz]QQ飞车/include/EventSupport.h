#pragma once 

#include "objbase.h"


interface IEventSupport
{
	STDMETHOD(OnMessage)() PURE;
};


typedef void (IEventSupport::*MsgProc)(void*);

typedef struct tagMSG_MAP_ITEM
{
	UINT MsgID;
	MsgProc Proc;
} MSG_MAP_ITEM;


#define DECLARE_MESSAGE_MAP_QQKART const MSG_MAP_ITEM * GetMessageMap(); \
			STDMETHODIMP OnMessage() {return S_OK;}

#define BEGIN_MESSAGE_MAP_QQKART(x) const MSG_MAP_ITEM* x::GetMessageMap() { \
	static const MSG_MAP_ITEM _Map[] = {

#define ON_COMMAND_QQKART(MsgID,Proc) { (UINT)MsgID, (MsgProc)&Proc },

#define END_MESSAGE_MAP_QQKART {0,NULL} } ; return _Map; }

#define DISPATCH_MESSAGE_QQKART(ID, Param) \
	const MSG_MAP_ITEM * _Map = GetMessageMap();	\
	int i = 0;		\
	while(_Map[i].MsgID != 0)	\
	{	\
		if( _Map[i].MsgID == ID)	\
		{	\
			MsgProc pfnProc = _Map[i].Proc;		\
			IEventSupport * pThis = (IEventSupport*)(int)this;	\
			(pThis->*pfnProc)(Param);	\
		}	\
		i++;	\
	} 

