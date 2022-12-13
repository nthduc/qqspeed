// resourecestoragedef.h 
#pragma once
#include "TRPS/Downloader/downloaderdef.h"

namespace Tenio
{
	class IVersionControler;
	class IDownloadConfig;
	class ICommandInterpreter;

	#define MAX_FILE_NUMBER_PER_REQUEST						20
    #define INVALIDATETASKID						        -1
	#define SYN_GETRESOURCESUCCESS								-1
	#define DOWNLOADTASK_SUCCESS_SYN							0
//	#define DOWNLOADTASK_FAIL								    -1
	
	//#define DOWNLOADETASK_ASYN								0

	    //COMPRESSED FILE EXTENDED NAME 
   #define  COMPRESSED_FILE_EXTENDED_NAME	            _T(".zip")
    
	   //TEMPORARY DOWNLOADFILE EXTENDED NAME
   #define  TEMPORARY_DOWNLOADFILE_EXTENDED_NAME	    _T(".TEM")
	
	   //num of FolderCompress
   #define RESOURCESTORAGE_MAX_FOLDERCOMPRESS			4
	
	// 下载类型配置文件中命令行的最大字符数
	#define    COMMAND_MAXLENGTH								1024

	// 从版本描述文件中读取文件列表，并挨个下载时，出现同步下载错误
	#define ERROR_RESOURCECHECKER_SYNCDOWNLOADERROR						(0x00000001)

	// 从版本描述文件中读取文件列表，并挨个下载时，出现读取版本描述文件错误
	#define ERROR_RESOURCECHECKER_READFILEERROR						(0x00000002)

// 	typedef struct tagTorrentList
// 	{
// 		WORD			dwNumber;
// 		TCHAR           sz
// 	};
// 	typedef struct tagP2PParamList 
// 	{
// 	DWORD dwNumber;
// 	P2PPARAM stArrP2PParam[MAXNUM_P2PPARAM];
//	}P2PPARAMLIST, *PP2PPARAMLIST;   

	typedef struct tagResourceList
	{
		int   nFileCount;
		DWORD dwPriorityList[MAX_FILE_NUMBER_PER_REQUEST];
		TCHAR szDownloadTypeList[MAX_FILE_NUMBER_PER_REQUEST][MAX_PATH];
		TCHAR szFileNameList[MAX_FILE_NUMBER_PER_REQUEST][MAX_PATH];
	}
	RESOURCELIST, * PRESOURCELIST;

	typedef struct tagVersionControlerInfo
	{		
		TCHAR szDownloadType[MAX_PATH];
		IVersionControler * pVersionControler;
	}
	VERSIONCONTROLERINFO, * PVERSIONCONTROLERINFO;

	typedef struct tagVersionFilterInfo
	{		
		TCHAR szDownloadType[MAX_PATH];
		BOOL bValid;
	}
	VERSIONFILTERINFO, * PVERSIONFILTERINFO;

	typedef struct tagConfigInfo
	{		
		TCHAR szDownloadType[MAX_PATH];
		IDownloadConfig * pDownloadConfig;
	}
	CONFIGINFO, * PCONFIGINFO;

	typedef struct tagCommandInterpreterInfo
	{		
		TCHAR szCommandType[MAX_PATH];
		ICommandInterpreter* pCommandInterpreter;
	}
	COMMANDINTERPRETERINFO, * PCOMMANDINTERPRETERINFO;



	typedef struct tagDownloadTaskParam
	{
		int				nDownloadID;
		RESOURCELIST    stResourceList;
		DWORD			dwMilliSeconds;
	}DOWNLOADTASKPARAM, *PDOWNLOADTASKPARAM;

// 	typedef struct tagDownloadConfigParam
// 	{
// 		DWORD           dwBandWidth;
// 		IPPORTPACKETS   stIPPortPackets;
// 		P2PPARAMLIST    stP2PParamList;
// 		TCHAR           szServerPath[MAX_PATH];
// 		TCHAR           szLocalPath[MAX_PATH];
// 		TCHAR           szTemporaryPath[MAX_PATH];
// 		TCHAR           szSubstitutePath[MAX_PATH];
// 		TCHAR			szCommand[MAX_PATH];
//	}DOWNLOADCONFIGPARAM, *PDOWNLOADCONFIGPARAM;
};