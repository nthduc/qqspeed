#pragma once

#include "teniointerface.h"
#include "InnerPredefine.h"

namespace Tenio
{
	class IUpdateServerManager;
	class IUpdateServerEvent;
	
	TENIO_MAKE_TYPEID_MAP(IUpdateServerManager, COMPONENTID_UPDATESERVERMANAGER);
	
	class IUpdateServerManager : public IComponent, public IEventProducer
	{
	public:
		virtual BOOL Initialize() = 0;
		virtual BOOL QueryUpdate(CLIENTBASEVERSION& stClientBaseVersion) = 0;
		
		virtual BOOL RequestNormalUpdateInfo(REQUESTNORMALUPDATEINFO& stRequest) = 0;
		virtual BOOL RequestModuleUpdateInfo(REQUESTMODULEUPDATEINFO& stRequest) = 0;
		virtual BOOL RequestLiveUpdateInfo(REQUESTLIVEUPDATEINFO& stRequest) = 0;
	};
	
	class IUpdateServerEvent : public IEvent
	{
	public:
		virtual void OnQueryUpdate(QUERYUPDATERESULT* pstQueryUpdateResult) = 0;
		virtual void OnUpdateInfoResult(UPDATEINFORESULT* pstResult) = 0;			
		virtual void OnLiveUpdateInfoResult(LIVEUPDATEINFORESULT* pstResult) = 0;
	};
};