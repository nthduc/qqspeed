#pragma once

#include "../../../include/teniointerface.h"

namespace Tenio
{
	class IVersionUpdater : public IComponent, public IEventProducer
	{
	public:
		// lpParam����Բ�ͬ����Ŀ��������һ����
		// 1��ͨ����Ŀ���������а汾���ƣ�lpParam������Ŀ���еİ汾�����ļ���
		// 2��ͨ��AutoUpdateServer���а汾���ƣ�
		//    lpParam��������ģʽ+�Ƿ���������
		virtual BOOL UpdateClient(
			LPVOID lpParam, 
			BOOL bStartup,
			BOOL bOnline	// ���ߵȴ�
			) = 0;

		virtual BOOL UpdateModule(
			LPCTSTR lpszModuleGuid,
			BOOL bOnline	// ���ߵȴ�
			) = 0;
	};

	TENIO_MAKE_TYPEID_MAP(IVersionUpdater, COMPONENTID_TRPS_VERSIONUPDATER);

	class IVersionUpdaterEvent : public IEvent
	{
	public:
		// ��������ɹ������Client���ߵȴ��Ļ���
		virtual void OnUpdateSuccess() = 0;

		// ��������ɹ������Client���ߵȴ��Ļ���
		virtual void OnUpdateFail() = 0;

		// ���ڿ��÷�ǿ�������������û����Ա������������Client���ߵȴ��Ļ���
		virtual void OnIgnoreUpdate() = 0;

		// û�и��������������������°汾
		virtual void OnAlreadyNewest() = 0;

		virtual void OnNeedUpdate() = 0;
	};


};