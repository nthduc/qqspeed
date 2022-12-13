
// downloadenginedef.h 
#pragma once

#include "TRPS/ResourceStorage/resourcestoragedef.h"
#include "NetModule/ProxyDef.h"

namespace Tenio
{

		// 配置文件中命令的最大长度
#define    COMMAND_MAXLENGTH								1024

#define  MAX_FILE_NUMBER_IN_CHECKUPDATE		                100

	// MAXNUMOFPARALLELTASK
#define  MAXNUM_PARALLELDOWNLOADINGTASK		                4

	// MAXNUMOFPARALLELTASK
#define  MAXNUM_PARALLELUPDATETASK							4

#define  CHECKUPDATE_ASYN				 1
#define  CHECKUPDATE_SUCCESS			 0
#define  CHECKUPDATE_FAIL				-1
#define  EXECUTE_ERROR_UNKNOWN	        -2

	// 每次请求下载同一个文件时，任务前进的距离
#define  ADVANCE_DISTANCE				 1

	typedef struct tagVersionResourceList
	{
		int   nFileCount;
		int   arrOfUpdateVersion[MAX_FILE_NUMBER_PER_REQUEST];
		DWORD dwPriorityList[MAX_FILE_NUMBER_PER_REQUEST];
		TCHAR szDownloadTypeList[MAX_FILE_NUMBER_PER_REQUEST][MAX_PATH];
		TCHAR szFileNameList[MAX_FILE_NUMBER_PER_REQUEST][MAX_PATH];
	}VERSIONRESOURCELIST, *PVERSIONRESOURCELIST;

// 	// Download add task param, for IProcessMessagerObject
// 	typedef struct tagDOWNLOAD_ADD_PARAM
// 	{
// 	//int				nEventID;					// event ID for callback pointer
// 	//int				nIndex;						// task index
// 	int				nVersion;					    // file version
// 	SOCK5PROXYPARAM sock5ProxyParam;				// sock5 proxy param
// 	//TCHAR			szUsername[MAX_PATH];			// username
// 	//TCHAR			szPassword[MAX_PATH];			// password
// 	//WORD			wProtocolType;					// protocol type
// 	TCHAR			szFilename[MAX_URL];			// file which will be downloaded
// 	TCHAR			szSaveFilename[MAX_PATH];		// file which will be save
// 	DWORD			dwBandWidth;					// net band width
// 	DWORD			dwTimeout;						// connect host timeout limit
// 	//DWORD			dwFlag;							// download flag(normar、continue...)
// 	IPPORTPACKETS	stIpPortPackets;				// ip&ports
// 	P2PPARAMLIST	stP2PParamList;					// p2p param,when wProtocolType = p2p
// 	}DOWNLOADADDPARAM, *PDOWNLOADADDPARAM;
	
	typedef struct tagUpdateFileList
	{
		int   nFileCount;
		TCHAR szDownloadTypeList[MAX_FILE_NUMBER_IN_CHECKUPDATE][MAX_PATH];
		TCHAR szFileNameList[MAX_FILE_NUMBER_IN_CHECKUPDATE][MAX_PATH];
	}
	UPDATEFILELIST, * PUPDATEFILELIST;


	typedef struct tagCommandList
	{
		int   nFileCount;
		DWORD dwPriorityList[MAX_FILE_NUMBER_PER_REQUEST];
		TCHAR szDownloadTypeList[MAX_FILE_NUMBER_PER_REQUEST][MAX_PATH];
		TCHAR szCommandList[MAX_FILE_NUMBER_PER_REQUEST][COMMAND_MAXLENGTH];
	}
	COMMANDLIST, * PCOMMANDLIST;

		typedef struct tagUpdateTaskInfo
	{
		int   nFileCount;
		//BOOL  bAtomic;
		DWORD dwPriorityList[MAX_FILE_NUMBER_PER_REQUEST];
		TCHAR szDownloadTypeList[MAX_FILE_NUMBER_PER_REQUEST][MAX_PATH];
		TCHAR szDestFileList[MAX_FILE_NUMBER_PER_REQUEST][MAX_PATH];
		TCHAR szSourceFileList[MAX_FILE_NUMBER_PER_REQUEST][MAX_PATH];
		TCHAR szCommandList[MAX_FILE_NUMBER_PER_REQUEST][COMMAND_MAXLENGTH];
	}
	UPDATETASKINFO, * PUPDATETASKINFO;

	// CheckUpdate执行的命令脚本信息
// 	typedef struct tagScriptUpdateTaskInfo
// 	{
// 		int   nScriptID;
// 		int	  nResult;
// 	}
// 	SCRIPTUPDATETASKINFO, * PSCRIPTUPDATETASKINFO;
}