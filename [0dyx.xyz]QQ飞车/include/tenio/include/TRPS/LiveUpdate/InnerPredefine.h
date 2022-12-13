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
	
	//  LiveUpdate版办
#define LIVEUPDATE_VERSION			1
	
	// “TRPS之版本升级系统”的版本
#define TRPS_UPDATE_CENTER_VERSION_1	1
#define TRPS_UPDATE_CENTER_VERSION_2	2

	// 更新程序的映象名字，例如“update.exe”
#define LIVEUPDATE_PATH				_T("liveupdater\\")
#define LIVEUPDATE_IMAGE_NAME_CFG	_T("LiveUpdatePath.ini")
#define LIVEUPDATE_IMAGE_NAME		_T("liveupdater\\liveupdate.exe")
#define UPDATEFILE_PATH				_T("updatefile\\")
#define LIVEUPDATE_CONFIG_FILE		_T("liveupdate.cfg")

#define DOWNLOAD_DEFAULT_PORT	8080 // Todo: make sure this value

	//////////////////////////////////////////////////////////////////////////
	/// Error Code
#define TRPS_ERROR_RETRIEVE_FAILURE			10001	// 获取更新信息失败
#define TRPS_ERROR_RETRIEVE_NOUPDATE		10002	// 没有更新，无需更新

#define TRPS_ERROR_PROMPTCANCEL				10003

#define TRPS_ERROR_UPDATESERVER_CLOSE		10004	// 更新服务器连接关闭
#define TRPS_ERROR_UPDATESERVER_TIMEOUT		10005	// 等待更新服务器回应超时
#define TRPS_ERROR_UPDATESERVER_NOTCONNECT	10006	// 更新服务器连接失败

#define TRPS_ERROR_ADVISEMODE_USERCANCLE	10007	// 推荐升级模式，用户取消升级，下次不再提示
	
	//////////////////////////////////////////////////////////////////////////
	///

#define NSIS_ERR          0xfffffff0
#define NSIS_PROGRESS     0xfffffff1

	typedef struct tagClientBaseVersion
	{
		char	cType;				// 版本类型，可用扩展
		DWORD	dwClientVersion;	// Client版本号
		WORD	wClientPatch;		// Client SP 号
		DWORD	dwClientKB;			// Client KB 号
		DWORD	dwUpdateID;			// Client的更新ID
		WORD	wBitmapLen;			// pbyBitmapBuffer 的长度
		BYTE*	pbyBitmapBuffer;	// pbyBitmapBuffer 每一位代表一个模块，看Client已经存在哪些模块
		char	cReserver;			// 扩展用
	}CLIENTBASEVERSION;
	
	typedef struct tagQueryUpdateResult
	{
		char	cResult;			// 结果，1要升级，0不需要
		DWORD	dwUpdateID;			// Client的更新ID
		WORD	wBufferLen;			// pbyBuffer的长度
		BYTE *	pbyBuffer;			// Buffer
		char	cReserver;			// 保留扩展
	}QUERYUPDATERESULT;
	
	//////////////////////////////////////////////////////////////////////////
	///
	// 普通模块的版本信息（组成列表往Server报）
	typedef struct tagNormalModuleVersion
	{
		int		nBitmapID;	// 在BITMAP 中的索引
		DWORD	dwVersion;	
	}NORMALMODULEVERSION;

#define MAX_REQUEST_BUFFER_LEN		10240		// 10K
#define MAX_REQUEST_MODULE_COUNT	200			// 防止请求buffer下溢

#define REQUEST_UPDATESERVER_TIMEOUT		10000  // 等待更新服务器的响应，10秒超时

// 版本升级的大类型
#define TYPE_MODULE_QUERYUPDATE		_T("QueryModuleUpdate") // 客户端升级
#define TYPE_UPDATE_QUERYUPDATE		_T("QueryUpdateUpdate")	// LiveUpdater自升级


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
	
				   
	// 查询版本升级信息的头部
	typedef struct tagRequestUpdateInfoHeader
	{
		int		nVersion;				// 协议版本号，现为1
		int		nQueryMode;				// 查询模式：0 正常更新查询、1 封版更新查询、2 单模块更新查询
		char	cUpdateModuleListOnly;	// 针对（nQueryMode=0），
										//		1表示只更新列表中的模块和插件（版本升级除外）
										//		0表示下载所有插件，包括未安装的新插件	

		tstring	strHashCode;			// 标识主机的Hash值
		tstring	strKey;					// 验证Key信息，Server用来校验
		tstring	strCltIdentify;			// 用于标志客户端的ID

		tstring	strMainModuleGUID;		// 主模块的唯一标志
		DWORD	dwMainModuleVersion;	// 主模块的版本
		WORD	wMainModulePatch;		// 主模块的SP版本
		DWORD	dwMainModuleKB;			// 主模块的KB版本

	}REQUESTUPDATEINFOHEADER;

	// 针对 REQUESTUPDATEINFOHEADER::nQueryMode
	typedef enum enmQueryMode
	{
		EQM_NORMAL_UPDATE = 0,	// 0 正常更新查询
		EQM_CLOSE_VERSION = 1,  // 1 封版更新查询
		EQM_SINGLE_MODULE = 2   // 2 单模块更新查询
	}ENMQUERYMODE;

	// 针对 REQUESTUPDATEINFOHEADER::cUpdateModuleListOnly，当（nQueryMode=0）
	enum enmUpdateModuleFlag 
	{
		EUMF_ONLY_LIST = 1,  	//	1 表示只更新列表中的模块和插件（版本升级除外）
		EUMF_ALL_MODULE = 0		//	0 表示下载所有插件，包括未安装的新插件	
	};
	
	//////////////////////////////////////////////////////////////////////////
	// 普通升级，查询版本升级信息
	typedef struct tagRequestNormalUpdateInfo
	{
		REQUESTUPDATEINFOHEADER stHeader;
		
		tstring  strLocalModuleBitmap;	// 客户端已经安装的模块或者插件位图信息
		tstring	strUpdateModuleBitmap;	// 需要更新的模块或者插件位图信息
		vector<NORMALMODULEVERSION> arrayNormalModuleVersion;// 需要更新普通模块的版本信息列表	

	}REQUESTNORMALUPDATEINFO;
	
	//////////////////////////////////////////////////////////////////////////
	// 单模块升级，查询版本升级信息
	typedef struct tagRequestModuleUpdateInfo
	{
		REQUESTUPDATEINFOHEADER stHeader;
		
		tstring  strModuleGUID;			// 模块的唯一标志
		DWORD	dwModuleVersion;		// 模块的版本信息

	}REQUESTMODULEUPDATEINFO;
	
	//////////////////////////////////////////////////////////////////////////

	typedef struct tagUpdateInfoResult
	{
		DWORD	dwCount;				// 可用更新的数目，0表示没有更新
		char	cUpdateFlag;			// 更新的标志：0默认、1版本优先、2补丁优先
		tstring strLiveUpdateSelfUpdateInfo;	//LiveUpdate自更新信息
		tstring	strUpdateInfo;			// Server下发的版本升级详细描述信息
	}UPDATEINFORESULT;
	
	//////////////////////////////////////////////////////////////////////////
	/// liveupdate自升级，查询版本升级信息
	typedef struct tagRequestLiveUpdateInfo
	{
		DWORD	dwLocalVersion;			// LiveUpdate的本地版本
	}REQUESTLIVEUPDATEINFO;
	
	typedef struct tagLiveUpdateInfoResult
	{
		int		nResult;				// 1需要更新，1不需要
		DWORD	dwUpdateVersion;		// 新版本信息
		tstring	strUpdateURL;			// 升级的地址信息
		tstring	strUpdateHash;			// Hash值
	}LIVEUPDATEINFORESULT;

	//////////////////////////////////////////////////////////////////////////
	/// 安装包信息

	typedef struct tagUpdatePackageInfo
	{
		// -->安装包基本信息<--
		tstring		strPackageGuid;		// 安装包的GUID（32位可视化字符）
		tstring	    strPackageName;		// 安装包的名称
		DWORD		dwPackageVersion;	// 主版本号 （如果sp/kb升级，不用写这项）
		WORD		wPackagePatch;		// SP号 （如果SP升级，必填）
		DWORD		dwPackageKB;		// KB号 （如果KB升级，必填）
		tstring		strPackageDesc;		// 安装包的信息描述 （HTML格式）
		int			nPackageType;		// 升级包类型：版本升级、KB升级、SP升级、普通模块升级……
		
		// -->安装包的文件信息<--
		DWORD	    dwPackageFileSize;	// 安装包zip文件的大小（单位：字节）
		tstring		strPackageFileName;	// 本地保存文件名;
		tstring		strPackageFileHash;	// zip包校验值;
		
		// -->安装包的模块信息<--
		tstring		strModuleTypeGuid;	// 安装的模块类型名
		DWORD		dwModuleBitmapID;	// 安装的模块Bitmap ID
		
		// -->安装包的升级策略<--
		DWORD		dwPackageSelectFlag;// 高四位表示用户是否有选择权，低四位表示是否默认选上
		DWORD		dwUpdateStrategy;	// 安装包的选择安装方式：强制升级（完全无提示）、……

		// -->安装包的下载参数信息<--
		int			nDownloadType;		// 下载方式，1为HTTP方式，2为P2P方式，3为两者混合
		tstring		strPackageURL;		// 下载URL;
		tstring		strEuraUrl;			//　安装包的eura的URL
		tstring		strEuraHash;		// eura文件的校验值
		tstring		strPeerUDPIP;		// P2P 相关参数 ↓<<
		USHORT		ushPeerUDPPort;
		tstring		strPeerTCPIP;
		USHORT		ushPeerTCPPort;
		tstring		strStunIP;
		USHORT		ushStunPort;
		tstring     strTorrentURL;
		DWORD 		dwHttpSpeedLimit;
		DWORD		dwHttpConnectionLimit;
		DWORD		dwlTargetShareRate; // P2P 相关参数 ↑>>

		// -->安装包的依赖关系<--
		tstring		strDependencyList;  // 安装包依赖列表

	}UPDATEPACKAGEINFO;

	//	<option value="0">手动升级</option>
	//	<option value="1">推荐升级(下载无提示)</option>
	//	<option value="2">推荐升级(下载有提示)</option>
	//	<option value="3">强制升级(下载无提示)</option>
	//	<option value="4">强制升级(下载有提示)</option>
	//  <option value="5">强制升级(无任何提示)</option>
	typedef enum enmUpdateStrategy
	{
		EUS_MANUAL					= 0,
		EUS_ADVISE_NOTIPDOWNLOAD	= 1,
		EUS_ADVISE_TIPDOWNLOAD		= 2,
		EUS_FORCE_NOTIPDOWNLOAD		= 3,
		EUS_FORCE_TIPDOWNLOAD		= 4,
		EUS_FORCE_TIPNOTHING		= 5 
	}ENMUPDATESTRATEGY;

	//	<option value="1">版本升级</option>
	//	<option value="2">补丁升级</option>
	//	<option value="3">SP升级</option>
	typedef enum enmPackageType
	{
		EPT_CLIENTVERSION	= 0,
		EPT_CLIENTKB		= 200000,
		EPT_CLIENTSP		= 100000,
		EPT_SUBMODULE_MIN	= 100,
		EPT_SUBMODULE_MAX	= 10000
	}ENMPACKAGETYPE;

	//////////////////////////////////////////////////////////////////////////
	

// LiveUpdater 程序启动的类型
#define STARTUPTYPE_NORMALUPDATE		1
#define STARTUPTYPE_CONTINUEUPDATE		2
#define STARTUPTYPE_SWITCHVERSION		3

	
	// 和选择安装包组件，进行数据交互的结构
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
	// 每个步骤的输入参数

	// Step1：
	typedef struct tagPromptStepData
	{
		DWORD dwUpdateStrategy;
		list<UPDATEPACKAGEINFO>* plistUpdatePackage;
	}PROMPTSTEPDATA;

	// Step2：
	typedef PROMPTSTEPDATA		SELECTSTEPDATA;

	// Step3:
	typedef SELECTSTEPDATA		DOWNLOADSTEPDATA;

	// Step4:
	typedef DOWNLOADSTEPDATA	UPDATECLIENTSTEPDATA;

	//////////////////////////////////////////////////////////////////////////
	// Error Manager Related

	// 下载更新包的状态
	typedef enum enmDownloadState
	{
		EDS_NODOWNLOAD		= 0,		// 未下载
		EDS_DOWNLOADED		= 1,		// 下载完成
		EDS_DOWNLOADING		= 2,		// 下载中
		EDS_DONLOADHUNG		= 3,		// 下载挂起
		EDS_DOWNLOADFAIL	= 4			// 下载失败
//		EDS_MD5CHECKFAILED	= 5			// MD5校验失败
	}ENMDOWNLOADSTATE;

	//TODO:分为WorkFlowErrorType和UpdateInfoQureyErrorType
	// 错误类型	<改为宏定义形式>
/*	typedef enum enmErrorType
	{
		//====下载错误====
		EET_PACKAGEMD5CHECKFAILED_DEPENDED	= 16,	// 安装包MD5校验失败,被依赖
		EET_PACKAGEMD5CHECKFAILED			= 15,	//没有被依赖
		EET_DOWNLOADSERVERLINKFAILD			= 11,		// 资源服务器连接失败
		EET_DOWNLOADDISKERROR				= 14,
		
		//====安装错误====
		EET_PACKAGEINSTALL_ERROR			= 20,
		EET_UPDATEDISKERROR					= 21
	}ENMERRORTYPE;
*/
	// 错误处理策略
	typedef enum enmErrorHandlePolicy
	{
		EEHP_ABORTALLSTEPS			= 0,		//强制终止所有step
		EEHP_BEGINNEXTSTEP			= 1,		//强制终止当前step,处理下一个step
		EEHP_RETRYCURRENTPACKAGE	= 2,		//强制重试当前的package
		EEHP_BEGINNEXTPACKAGE		= 3,			//强制终止当前package,处理下一个package	
		EEHP_IGNORE					= 4,		// 忽略这个错误
		EEHP_CONTINUE				= 5			// 继续后面的处理
	}ENMERRORHANDLEPOLICY;

	// 安装更新包的状态
	typedef enum enmInstallState
	{
		EUCS_NOINSTALL		= 0,		// 未安装
		EUCS_INSTALLED		= 1,		// 安装完成
		EUCS_INSTALLING		= 2,		// 安装中
		EUCS_INSTALLFAIL	= 3			// 安装失败
	}ENMINSTALLSTATE;

	// liveUpdate 一次更新过程的结果
	typedef enum enmLiveUpdateResult
	{		
		ELUR_FAIL			= 0,		// 更新失败
		ELUR_SUCCESS		= 1,		// 更新成功
		ELUR_UPDATING		= 2,
		ELUR_GIVEUP			= 3			// 放弃更新
	}ENMLIVEUPDATERESULT;


	//////////////////////////////////////////////////////////////////////////
	// Not Standard Update Related

#define MAX_PACKAGE_COUNT		50
	
	typedef struct tagPackageCoreInfo
	{
		DWORD		dwPackageVersion;	// 升级包版本号
		TCHAR	    szPackageName[MAX_PATH];		// 升级包的名称
		TCHAR		szPackageFileName[MAX_PATH];	// 本地保存文件名;
		TCHAR		szPackageFileHash[33];	// 未压缩升级包校验值;
		DWORD		dwUpdateStrategy;	// 安装包的选择安装方式：强制升级（完全无提示）、……
		int			nDownloadType;		// 下载方式，1为HTTP方式，2为P2P方式，3为两者混合
		TCHAR		szPackageDir[MAX_PATH];		// 下载Dir;		
		DWORD	    dwPackageFileSize;	// 安装包zip文件的大小（单位：字节）
		
		TCHAR		szHttpIP[MAX_PATH];			//Http相关参数 ↓<<
		USHORT		ushHttpPort;				//Http相关参数 ↑>>
		
		TCHAR		szPeerUDPIP[MAX_PATH];		// P2P 相关参数 ↓<<
		USHORT		ushPeerUDPPort;
		TCHAR		szPeerTCPIP[MAX_PATH];
		USHORT		ushPeerTCPPort;
		TCHAR		szStunIP[MAX_PATH];
		USHORT		ushStunPort;
		TCHAR		szTorrentURL[MAX_PATH];
		DWORD 		dwHttpSpeedLimit;
		DWORD		dwHttpConnectionLimit;
		DWORD		dwlTargetShareRate; // P2P 相关参数 ↑>>

		BOOL		bIsSelfUpdate;	//是否是Liveupdate自升级包
	} PACKAGECOREINFO;
	
	typedef struct tagPackageCoreInfoGroup
	{
		int nPackageNum;	//升级包实际个数
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