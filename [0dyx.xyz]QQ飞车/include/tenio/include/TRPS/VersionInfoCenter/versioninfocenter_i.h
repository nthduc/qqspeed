
#pragma once

#include "../../teniointerface.h"
#include "../../tstring.h"
#include <vector>
#include "../LiveUpdate/InnerPredefine.h"
using namespace std;

namespace Tenio
{
	class IVersionInfoCenter;

	TENIO_MAKE_TYPEID_MAP(IVersionInfoCenter, COMPONENTID_VERSIONINFOCENTER);
	
	class IVersionInfoCenter : public IComponent
	{
	public:
		virtual BOOL GetClientVersionInfo(
			tstring& strMainModuleGUID,
			DWORD& dwMainModuleVersion,
			WORD& wMainModulePatch,
			DWORD& dwMainModuleKB,
			tstring& strVersionName
			) = 0;
		
		virtual BOOL GetModuleVersionInfo(
			tstring& strModuleGUID,
			int& nBitmapID,	// 在BITMAP 中的索引
			DWORD& dwVersion,
			tstring& strVersionName	
			) = 0;
		
		virtual	BOOL GetAllModuleVersion(
			vector<NORMALMODULEVERSION>& arrayVersion
			) = 0;		
		
		virtual BOOL SetClientVersionInfo(
			tstring& strMainModuleGUID,
			DWORD dwMainModuleVersion,
			WORD wMainModulePatch,
			DWORD dwMainModuleKB,
			tstring& strVersionName
			) = 0;
		
		virtual BOOL SetClientSP(WORD wMainModulePatch) = 0;
		
		virtual BOOL SetClientKB(DWORD dwMainModuleKB) = 0;

		virtual BOOL SetModuleVersionInfo(
			tstring& strModuleGUID,
			int nBitmapID,	// 在BITMAP 中的索引
			DWORD dwVersion,
			tstring& strVersionName	
			) = 0;
	};
};
