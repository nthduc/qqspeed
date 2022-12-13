#pragma once

#include "tchar.h"
#include "wtypes.h"
#include "assert.h"
#include "../../tstring.h"
#include <map>
#include <list>
#include <VECTOR>
using namespace std;
#include "../../globalerrorcode.h"
namespace Tenio
{
	//////////////////////////////////////////////////////////////////////////
	
	//  LiveUpdate���
#define LIVEUPDATE_VERSION			1
	
	// ��TRPS֮�汾����ϵͳ���İ汾
#define TRPS_UPDATE_CENTER_VERSION_1	1
#define TRPS_UPDATE_CENTER_VERSION_2	2

	// ���³����ӳ�����֣����硰update.exe��
#define LIVEUPDATE_PATH				_T("liveupdater\\")
#define LIVEUPDATE_IMAGE_NAME_CFG	_T("LiveUpdatePath.ini")
#define LIVEUPDATE_IMAGE_NAME		_T("liveupdater\\liveupdate.exe")
#define UPDATEFILE_PATH				_T("updatefile\\")
#define LIVEUPDATE_CONFIG_FILE		_T("liveupdate.cfg")

#define DOWNLOAD_DEFAULT_PORT	8080 // Todo: make sure this value

	//////////////////////////////////////////////////////////////////////////
	/// Error Code
#define TRPS_ERROR_RETRIEVE_FAILURE			10001	// ��ȡ������Ϣʧ��
#define TRPS_ERROR_RETRIEVE_NOUPDATE		10002	// û�и��£��������

#define TRPS_ERROR_PROMPTCANCEL				10003

#define TRPS_ERROR_UPDATESERVER_CLOSE		10004	// ���·��������ӹر�
#define TRPS_ERROR_UPDATESERVER_TIMEOUT		10005	// �ȴ����·�������Ӧ��ʱ
#define TRPS_ERROR_UPDATESERVER_NOTCONNECT	10006	// ���·���������ʧ��

#define TRPS_ERROR_ADVISEMODE_USERCANCLE	10007	// �Ƽ�����ģʽ���û�ȡ���������´β�����ʾ
	
	//////////////////////////////////////////////////////////////////////////
	///

#define NSIS_ERR          0xfffffff0
#define NSIS_PROGRESS     0xfffffff1

	typedef struct tagClientBaseVersion
	{
		char	cType;				// �汾���ͣ�������չ
		DWORD	dwClientVersion;	// Client�汾��
		WORD	wClientPatch;		// Client SP ��
		DWORD	dwClientKB;			// Client KB ��
		DWORD	dwUpdateID;			// Client�ĸ���ID
		WORD	wBitmapLen;			// pbyBitmapBuffer �ĳ���
		BYTE*	pbyBitmapBuffer;	// pbyBitmapBuffer ÿһλ����һ��ģ�飬��Client�Ѿ�������Щģ��
		char	cReserver;			// ��չ��
	}CLIENTBASEVERSION;
	
	typedef struct tagQueryUpdateResult
	{
		char	cResult;			// �����1Ҫ������0����Ҫ
		DWORD	dwUpdateID;			// Client�ĸ���ID
		WORD	wBufferLen;			// pbyBuffer�ĳ���
		BYTE *	pbyBuffer;			// Buffer
		char	cReserver;			// ������չ
	}QUERYUPDATERESULT;
	
	//////////////////////////////////////////////////////////////////////////
	///
	// ��ͨģ��İ汾��Ϣ������б���Server����
	typedef struct tagNormalModuleVersion
	{
		int		nBitmapID;	// ��BITMAP �е�����
		DWORD	dwVersion;	
	}NORMALMODULEVERSION;

#define MAX_REQUEST_BUFFER_LEN		10240		// 10K
#define MAX_REQUEST_MODULE_COUNT	200			// ��ֹ����buffer����

#define REQUEST_UPDATESERVER_TIMEOUT		10000  // �ȴ����·���������Ӧ��10�볬ʱ

// �汾�����Ĵ�����
#define TYPE_MODULE_QUERYUPDATE		_T("QueryModuleUpdate") // �ͻ�������
#define TYPE_UPDATE_QUERYUPDATE		_T("QueryUpdateUpdate")	// LiveUpdater������


#define HTTP_REQUEST_HEADER_PATTERN	 TEXT(\
	"GET *** HTTP 1.1 \r\n"\
	"VERSION:%d\r\n"\
	"QUERY_MODE:%d\r\n"\
	"UPDATE_MODULELIST_ONLY:%d\r\n"\
	"HASH_CODE:%s\r\n"\
	"KEY:%s\r\n"\
	"CLIENTIDENTIFY:%s\r\n"\
	"MAIN_MODULE_GUID:%s\r\n"\
	"MAIN_MODULE_VERSION:%d\r\n"\
	"MAIN_MODULE_SPNUM:%d\r\n"\
	"MAIN_MODULE_KBNUM:%d\r\n")

#define NORMALUPDATE_BITMAPLIST_PATTERN	TEXT(\
	"LOCAL_MODULE_BITMAPLIST:%s\r\n"\
	"UPDATE_MODULE_BITMAPLIST:%s\r\n")
							  
#define NORMALUPDATE_MOULE_PATTERN TEXT("SUB_MODULE%d_VERSION:%d\r\n")

#define SINGLEUPDATE_MODULE_PATTERN TEXT(\
	"SUB_MODULE_GUID:%s\r\n"\
	"SUB_MODULE_VERSION:%d\r\n")

#define LIVEUPDATER_SELFUPDATE_PATTERN TEXT(\
	"LIVEUPDATER_VERSION:%d\r\n")
	
				   
	// ��ѯ�汾������Ϣ��ͷ��
	typedef struct tagRequestUpdateInfoHeader
	{
		int		nVersion;				// Э��汾�ţ���Ϊ1
		int		nQueryMode;				// ��ѯģʽ��0 �������²�ѯ��1 �����²�ѯ��2 ��ģ����²�ѯ
		char	cUpdateModuleListOnly;	// ��ԣ�nQueryMode=0����
										//		1��ʾֻ�����б��е�ģ��Ͳ�����汾�������⣩
										//		0��ʾ�������в��������δ��װ���²��	

		tstring	strHashCode;			// ��ʶ������Hashֵ
		tstring	strKey;					// ��֤Key��Ϣ��Server����У��
		tstring	strCltIdentify;			// ���ڱ�־�ͻ��˵�ID

		tstring	strMainModuleGUID;		// ��ģ���Ψһ��־
		DWORD	dwMainModuleVersion;	// ��ģ��İ汾
		WORD	wMainModulePatch;		// ��ģ���SP�汾
		DWORD	dwMainModuleKB;			// ��ģ���KB�汾

	}REQUESTUPDATEINFOHEADER;

	// ��� REQUESTUPDATEINFOHEADER::nQueryMode
	typedef enum enmQueryMode
	{
		EQM_NORMAL_UPDATE = 0,	// 0 �������²�ѯ
		EQM_CLOSE_VERSION = 1,  // 1 �����²�ѯ
		EQM_SINGLE_MODULE = 2   // 2 ��ģ����²�ѯ
	}ENMQUERYMODE;

	// ��� REQUESTUPDATEINFOHEADER::cUpdateModuleListOnly������nQueryMode=0��
	enum enmUpdateModuleFlag 
	{
		EUMF_ONLY_LIST = 1,  	//	1 ��ʾֻ�����б��е�ģ��Ͳ�����汾�������⣩
		EUMF_ALL_MODULE = 0		//	0 ��ʾ�������в��������δ��װ���²��	
	};
	
	//////////////////////////////////////////////////////////////////////////
	// ��ͨ��������ѯ�汾������Ϣ
	typedef struct tagRequestNormalUpdateInfo
	{
		REQUESTUPDATEINFOHEADER stHeader;
		
		tstring  strLocalModuleBitmap;	// �ͻ����Ѿ���װ��ģ����߲��λͼ��Ϣ
		tstring	strUpdateModuleBitmap;	// ��Ҫ���µ�ģ����߲��λͼ��Ϣ
		vector<NORMALMODULEVERSION> arrayNormalModuleVersion;// ��Ҫ������ͨģ��İ汾��Ϣ�б�	

	}REQUESTNORMALUPDATEINFO;
	
	//////////////////////////////////////////////////////////////////////////
	// ��ģ����������ѯ�汾������Ϣ
	typedef struct tagRequestModuleUpdateInfo
	{
		REQUESTUPDATEINFOHEADER stHeader;
		
		tstring  strModuleGUID;			// ģ���Ψһ��־
		DWORD	dwModuleVersion;		// ģ��İ汾��Ϣ

	}REQUESTMODULEUPDATEINFO;
	
	//////////////////////////////////////////////////////////////////////////

	typedef struct tagUpdateInfoResult
	{
		DWORD	dwCount;				// ���ø��µ���Ŀ��0��ʾû�и���
		char	cUpdateFlag;			// ���µı�־��0Ĭ�ϡ�1�汾���ȡ�2��������
		tstring strLiveUpdateSelfUpdateInfo;	//LiveUpdate�Ը�����Ϣ
		tstring	strUpdateInfo;			// Server�·��İ汾������ϸ������Ϣ
	}UPDATEINFORESULT;
	
	//////////////////////////////////////////////////////////////////////////
	/// liveupdate����������ѯ�汾������Ϣ
	typedef struct tagRequestLiveUpdateInfo
	{
		DWORD	dwLocalVersion;			// LiveUpdate�ı��ذ汾
	}REQUESTLIVEUPDATEINFO;
	
	typedef struct tagLiveUpdateInfoResult
	{
		int		nResult;				// 1��Ҫ���£�1����Ҫ
		DWORD	dwUpdateVersion;		// �°汾��Ϣ
		tstring	strUpdateURL;			// �����ĵ�ַ��Ϣ
		tstring	strUpdateHash;			// Hashֵ
	}LIVEUPDATEINFORESULT;

	//////////////////////////////////////////////////////////////////////////
	/// ��װ����Ϣ

	typedef struct tagUpdatePackageInfo
	{
		// -->��װ��������Ϣ<--
		tstring		strPackageGuid;		// ��װ����GUID��32λ���ӻ��ַ���
		tstring	    strPackageName;		// ��װ��������
		DWORD		dwPackageVersion;	// ���汾�� �����sp/kb����������д���
		WORD		wPackagePatch;		// SP�� �����SP���������
		DWORD		dwPackageKB;		// KB�� �����KB���������
		tstring		strPackageDesc;		// ��װ������Ϣ���� ��HTML��ʽ��
		int			nPackageType;		// ���������ͣ��汾������KB������SP��������ͨģ����������
		
		// -->��װ�����ļ���Ϣ<--
		DWORD	    dwPackageFileSize;	// ��װ��zip�ļ��Ĵ�С����λ���ֽڣ�
		tstring		strPackageFileName;	// ���ر����ļ���;
		tstring		strPackageFileHash;	// zip��У��ֵ;
		
		// -->��װ����ģ����Ϣ<--
		tstring		strModuleTypeGuid;	// ��װ��ģ��������
		DWORD		dwModuleBitmapID;	// ��װ��ģ��Bitmap ID
		
		// -->��װ������������<--
		DWORD		dwPackageSelectFlag;// ����λ��ʾ�û��Ƿ���ѡ��Ȩ������λ��ʾ�Ƿ�Ĭ��ѡ��
		DWORD		dwUpdateStrategy;	// ��װ����ѡ��װ��ʽ��ǿ����������ȫ����ʾ��������

		// -->��װ�������ز�����Ϣ<--
		int			nDownloadType;		// ���ط�ʽ��1ΪHTTP��ʽ��2ΪP2P��ʽ��3Ϊ���߻��
		tstring		strPackageURL;		// ����URL;
		tstring		strEuraUrl;			//����װ����eura��URL
		tstring		strEuraHash;		// eura�ļ���У��ֵ
		tstring		strPeerUDPIP;		// P2P ��ز��� ��<<
		USHORT		ushPeerUDPPort;
		tstring		strPeerTCPIP;
		USHORT		ushPeerTCPPort;
		tstring		strStunIP;
		USHORT		ushStunPort;
		tstring     strTorrentURL;
		DWORD 		dwHttpSpeedLimit;
		DWORD		dwHttpConnectionLimit;
		DWORD		dwlTargetShareRate; // P2P ��ز��� ��>>

		// -->��װ����������ϵ<--
		tstring		strDependencyList;  // ��װ�������б�

	}UPDATEPACKAGEINFO;

	//	<option value="0">�ֶ�����</option>
	//	<option value="1">�Ƽ�����(��������ʾ)</option>
	//	<option value="2">�Ƽ�����(��������ʾ)</option>
	//	<option value="3">ǿ������(��������ʾ)</option>
	//	<option value="4">ǿ������(��������ʾ)</option>
	//  <option value="5">ǿ������(���κ���ʾ)</option>
	typedef enum enmUpdateStrategy
	{
		EUS_MANUAL					= 0,
		EUS_ADVISE_NOTIPDOWNLOAD	= 1,
		EUS_ADVISE_TIPDOWNLOAD		= 2,
		EUS_FORCE_NOTIPDOWNLOAD		= 3,
		EUS_FORCE_TIPDOWNLOAD		= 4,
		EUS_FORCE_TIPNOTHING		= 5 
	}ENMUPDATESTRATEGY;

	//	<option value="1">�汾����</option>
	//	<option value="2">��������</option>
	//	<option value="3">SP����</option>
	typedef enum enmPackageType
	{
		EPT_CLIENTVERSION	= 0,
		EPT_CLIENTKB		= 200000,
		EPT_CLIENTSP		= 100000,
		EPT_SUBMODULE_MIN	= 100,
		EPT_SUBMODULE_MAX	= 10000
	}ENMPACKAGETYPE;

	//////////////////////////////////////////////////////////////////////////
	

// LiveUpdater ��������������
#define STARTUPTYPE_NORMALUPDATE		1
#define STARTUPTYPE_CONTINUEUPDATE		2
#define STARTUPTYPE_SWITCHVERSION		3

	
	// ��ѡ��װ��������������ݽ����Ľṹ
//	typedef struct tagSelectPackageInfo
//	{
//		tstring strPackageGuid;
//		tstring strPackageName;
//		tstring strPackageDesc;
//		DWORD	dwPackageFileSize;
//		DWORD   dwPackageSelectFlag;
//		list<SELECTPACKAGEINFO> listDependency;
//	}SELECTPACKAGEINFO;


	//////////////////////////////////////////////////////////////////////////
	// ÿ��������������

	// Step1��
	typedef struct tagPromptStepData
	{
		DWORD dwUpdateStrategy;
		list<UPDATEPACKAGEINFO>* plistUpdatePackage;
	}PROMPTSTEPDATA;

	// Step2��
	typedef PROMPTSTEPDATA		SELECTSTEPDATA;

	// Step3:
	typedef SELECTSTEPDATA		DOWNLOADSTEPDATA;

	// Step4:
	typedef DOWNLOADSTEPDATA	UPDATECLIENTSTEPDATA;

	//////////////////////////////////////////////////////////////////////////
	// Error Manager Related

	// ���ظ��°���״̬
	typedef enum enmDownloadState
	{
		EDS_NODOWNLOAD		= 0,		// δ����
		EDS_DOWNLOADED		= 1,		// �������
		EDS_DOWNLOADING		= 2,		// ������
		EDS_DONLOADHUNG		= 3,		// ���ع���
		EDS_DOWNLOADFAIL	= 4			// ����ʧ��
//		EDS_MD5CHECKFAILED	= 5			// MD5У��ʧ��
	}ENMDOWNLOADSTATE;

	//TODO:��ΪWorkFlowErrorType��UpdateInfoQureyErrorType
	// ��������	<��Ϊ�궨����ʽ>
/*	typedef enum enmErrorType
	{
		//====���ش���====
		EET_PACKAGEMD5CHECKFAILED_DEPENDED	= 16,	// ��װ��MD5У��ʧ��,������
		EET_PACKAGEMD5CHECKFAILED			= 15,	//û�б�����
		EET_DOWNLOADSERVERLINKFAILD			= 11,		// ��Դ����������ʧ��
		EET_DOWNLOADDISKERROR				= 14,
		
		//====��װ����====
		EET_PACKAGEINSTALL_ERROR			= 20,
		EET_UPDATEDISKERROR					= 21
	}ENMERRORTYPE;
*/
	// ���������
	typedef enum enmErrorHandlePolicy
	{
		EEHP_ABORTALLSTEPS			= 0,		//ǿ����ֹ����step
		EEHP_BEGINNEXTSTEP			= 1,		//ǿ����ֹ��ǰstep,������һ��step
		EEHP_RETRYCURRENTPACKAGE	= 2,		//ǿ�����Ե�ǰ��package
		EEHP_BEGINNEXTPACKAGE		= 3,			//ǿ����ֹ��ǰpackage,������һ��package	
		EEHP_IGNORE					= 4,		// �����������
		EEHP_CONTINUE				= 5			// ��������Ĵ���
	}ENMERRORHANDLEPOLICY;

	// ��װ���°���״̬
	typedef enum enmInstallState
	{
		EUCS_NOINSTALL		= 0,		// δ��װ
		EUCS_INSTALLED		= 1,		// ��װ���
		EUCS_INSTALLING		= 2,		// ��װ��
		EUCS_INSTALLFAIL	= 3			// ��װʧ��
	}ENMINSTALLSTATE;

	// liveUpdate һ�θ��¹��̵Ľ��
	typedef enum enmLiveUpdateResult
	{		
		ELUR_FAIL			= 0,		// ����ʧ��
		ELUR_SUCCESS		= 1,		// ���³ɹ�
		ELUR_UPDATING		= 2,
		ELUR_GIVEUP			= 3			// ��������
	}ENMLIVEUPDATERESULT;


	//////////////////////////////////////////////////////////////////////////
	// Not Standard Update Related

#define MAX_PACKAGE_COUNT		50
	
	typedef struct tagPackageCoreInfo
	{
		DWORD		dwPackageVersion;	// �������汾��
		TCHAR	    szPackageName[MAX_PATH];		// ������������
		TCHAR		szPackageFileName[MAX_PATH];	// ���ر����ļ���;
		TCHAR		szPackageFileHash[33];	// δѹ��������У��ֵ;
		DWORD		dwUpdateStrategy;	// ��װ����ѡ��װ��ʽ��ǿ����������ȫ����ʾ��������
		int			nDownloadType;		// ���ط�ʽ��1ΪHTTP��ʽ��2ΪP2P��ʽ��3Ϊ���߻��
		TCHAR		szPackageDir[MAX_PATH];		// ����Dir;		
		DWORD	    dwPackageFileSize;	// ��װ��zip�ļ��Ĵ�С����λ���ֽڣ�
		
		TCHAR		szHttpIP[MAX_PATH];			//Http��ز��� ��<<
		USHORT		ushHttpPort;				//Http��ز��� ��>>
		
		TCHAR		szPeerUDPIP[MAX_PATH];		// P2P ��ز��� ��<<
		USHORT		ushPeerUDPPort;
		TCHAR		szPeerTCPIP[MAX_PATH];
		USHORT		ushPeerTCPPort;
		TCHAR		szStunIP[MAX_PATH];
		USHORT		ushStunPort;
		TCHAR		szTorrentURL[MAX_PATH];
		DWORD 		dwHttpSpeedLimit;
		DWORD		dwHttpConnectionLimit;
		DWORD		dwlTargetShareRate; // P2P ��ز��� ��>>

		BOOL		bIsSelfUpdate;	//�Ƿ���Liveupdate��������
	} PACKAGECOREINFO;
	
	typedef struct tagPackageCoreInfoGroup
	{
		int nPackageNum;	//������ʵ�ʸ���
		PACKAGECOREINFO stPackageCoreInfo[MAX_PACKAGE_COUNT];
	} PACKAGECOREINFOGROUP;


	//////////////////////////////////////////////////////////////////////////
	// IPC Related 	
#define MAX_WMIPC_BUFFER		1024
	typedef struct tagLiveUpdateIPCData 
	{
		int nCommandID;
		DWORD dwSize;
		BYTE DataBuffer[MAX_WMIPC_BUFFER];
	}LIVEUPDATEIPCDATA;

#define L2C_UPDATESUCCESS		1
#define L2C_UPDATEFAIL			2
#define L2C_IGNOREUPDATE		3
#define L2C_ALREADYNEWEST		4
#define L2C_NEEDUPDATE			5
	//////////////////////////////////////////////////////////////////////////
	
};