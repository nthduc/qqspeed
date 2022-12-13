#pragma once

#include "../../../include/teniointerface.h"

namespace Tenio
{
	class IVersionUpdater : public IComponent, public IEventProducer
	{
	public:
		// lpParam，针对不同的项目，参数不一样。
		// 1、通过项目服务器进行版本控制，lpParam会是项目特有的版本描述文件名
		// 2、通过AutoUpdateServer进行版本控制，
		//    lpParam会是升级模式+是否启动升级
		virtual BOOL UpdateClient(
			LPVOID lpParam, 
			BOOL bStartup,
			BOOL bOnline	// 在线等待
			) = 0;

		virtual BOOL UpdateModule(
			LPCTSTR lpszModuleGuid,
			BOOL bOnline	// 在线等待
			) = 0;
	};

	TENIO_MAKE_TYPEID_MAP(IVersionUpdater, COMPONENTID_TRPS_VERSIONUPDATER);

	class IVersionUpdaterEvent : public IEvent
	{
	public:
		// 更新请求成功（如果Client在线等待的话）
		virtual void OnUpdateSuccess() = 0;

		// 更新请求成功（如果Client在线等待的话）
		virtual void OnUpdateFail() = 0;

		// 存在可用非强制升级，但是用户忽略本次升级（如果Client在线等待的话）
		virtual void OnIgnoreUpdate() = 0;

		// 没有更新升级，本地已是最新版本
		virtual void OnAlreadyNewest() = 0;

		virtual void OnNeedUpdate() = 0;
	};


};