#pragma  once


#include "teniointerface.h"
#include "InnerPredefine.h"

namespace Tenio
{
	// ���桢����������·��İ汾������Ϣ
	
	class IUpdateInfoStorage : public IComponent
	{
	public:
		virtual BOOL SaveUpdateInfo(tstring& strUpdateInfo, tstring& strUpdateInfoFile) = 0;
		virtual BOOL LoadUpdateInfo(tstring& strUpdateInfoFile) = 0;
		
		virtual BOOL GetUpdatePackageInfo(
			tstring& strPackageGUID, 
			UPDATEPACKAGEINFO& stUpdatePackageInfo
			) = 0;

		virtual BOOL VerifyUpdateInfoFile(tstring& strUpdateInfoFile) = 0;
		
		virtual int GetPackageCount() = 0;
		
		// Search all Package Infomation
		virtual BOOL GetFirstPackageInfo(UPDATEPACKAGEINFO& stUpdatePackageInfo) = 0;
		virtual BOOL GetNextPackageInfo(UPDATEPACKAGEINFO& stUpdatePackageInfo) = 0;
	};

	TENIO_MAKE_TYPEID_MAP(IUpdateInfoStorage, COMPONENTID_UPDATEINFOSTORAGE);
	
};