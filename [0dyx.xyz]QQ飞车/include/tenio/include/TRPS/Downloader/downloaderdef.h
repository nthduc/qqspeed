#ifndef __DOWNLOADERDEF_I_H
#define __DOWNLOADERDEF_I_H

#define CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include "NetModule\ProxyDef.h"

namespace Tenio
{
#ifndef __NET_tagIP_PORT_PACKETS__
#define __NET_tagIP_PORT_PACKETS__

	// max number of IPPORT
#define MAX_IPPORT									64

// Ip and port information
typedef struct tagIP_PORT_INFO
{
	TCHAR	szHost[MAX_PATH];						// host name or ip(www.sohu.com,127.0.0.1)
	WORD	wPort;									// port
}IPPORTINFO, *PIPPORTINFO;

// Ip and port packets information
typedef struct tagIP_PORT_PACKETS
{
	WORD			dwNumber;						// number of packets
	IPPORTINFO		ipPortInfo[MAX_IPPORT];			// packets buffer
}IPPORTPACKETS, *PIPPORTPACKETS;

#endif

    //  下载超时时长(毫秒)
#define  MAX_TIME_OUT			200000

#define  MAX_BANDWIDTH			500

#define  MAXNUM_DOWNLOADTHREAD	4

#define  INVALID_TASKID        -1	
#define  SYNSUCCESS_TASKID     0
    // p2p windows class name
#define DOWNLOAD_P2P_SINGLE_THREAD_WINDOWS_CLASSNAME  _T("DownloadSingleThradP2PWindows")

    // p2pCenter windows class name
#define DOWNLOAD_P2PCENTER_WINDOWS_CLASSNAME  _T("P2PCENTERWindows")

#define DOWNLOAD_THREAD_TIMEOUT						1000

#define DOWNLOAD_P2PCENTERTHREAD_TIMEOUT			4000

#define DOWNLOAD_SPEEDDETECT_TIMEOUT				1

    // p2p download dll module name
#define NEW_DOWN_RECORD_CFG_DIR					      _T("config\\dynamic\\NewDownRecord.cfg")

    // p2p download dll module name
#define VQQ_P2P_MODLE_NAME							  _T("Tenio/vqqsdl.dll")

    // Http Response Code
#define DOWNLOAD_HTTP_RESPONSE_SUCCESS				200
#define DOWNLOAD_HTTP_RESPONSE_PART					206
#define DOWNLOAD_HTTP_RESPONSE_SIZECHANGED			416
#define DOWNLOAD_HTTP_RESPONSE_MOVEDTEMPORARILY		302

    //Download Center Flag
#define DOWNLOADCENTER_SERVICE_QQPET				0

    // HTTP windows class name
#define DOWNLOAD_HTTPPROTOCOL_WINDOWS_CLASSNAME		_T("DOWNLOADHTTPPROTWINDOWS")

#define DOWNLOAD_IPGROUP_WINDOWS_CLASSNAME		    _T("IPGROUPHTTPPROTWINDOWS")

#define DOWNLOAD_HTTPDOWNLOAD_WINDOWS_CLASSNAME		_T("HTTPDOWNLOADWINDOWS")

#define WM_DOWNLOAD_CREATETASKSUCCESS				(0x3333 + 0x01)
#define WM_DOWNLOAD_CREATETASKSFAIL					(0x3333 + 0x02)
#define WM_DOWNLOAD_STOPTASK						(0x3333 + 0x03)
#define WM_DOWNLOAD_DELETETASK						(0x3333 + 0x04)
#define WM_DOWNLOAD_SETP2PDOWNLOADSPEED				(0x3333 + 0x05)
#define WM_DOWNLOAD_ADJUSTP2PUPLOADSPEED			(0x3333 + 0x06)
#define WM_DOWNLOAD_ADJUSTP2PHTTPSPEED				(0x3333 + 0x07)
#define WM_DOWNLOAD_ONGETFILESIZE					(0x3333 + 0x08)
#define WM_DOWNLOAD_ONDOWNLOADSUCCESS				(0x3333 + 0x09)
#define WM_DOWNLOAD_ONDOWNLOADERROR					(0x3333 + 0x0a)
#define WM_DOWNLOAD_ONPROGRESSCHANGED				(0x3333 + 0x0b)
#define WM_DOWNLOAD_ONUPDATESUCCESS					(0x3333 + 0x0c)
#define WM_DOWNLOAD_ONUPDATEFAIL					(0x3333 + 0x0d)
#define WM_DOWNLOAD_SETBANDWIDTH					(0x3333 + 0x0e)
#define WM_DOWNLOAD_CANCELTASK					    (0x3333 + 0x0f)
#define WM_DOWNLOAD_ADDVERSIONCONTROLER				(0x3333 + 0x10)
#define WM_DOWNLOAD_REMOVEVERSIONCONTROLER			(0x3333 + 0x11)
#define WM_DOWNLOAD_SETVERSIONFILTER				(0x3333 + 0x12)
#define WM_DOWNLOAD_ADDCONFIG						(0x3333 + 0x13)
#define WM_DOWNLOAD_REMOVECONFIG					(0x3333 + 0x14)
#define WM_DOWNLOAD_ADDCOMMANDINTERPRETER			(0x3333 + 0x15)
#define WM_DOWNLOAD_REMOVECOMMANDINTERPRETER		(0x3333 + 0x16)
#define WM_DOWNLOAD_SETDEFAULTPROXY					(0x3333 + 0x17)
#define WM_DOWNLOAD_SETTOTALBANDWIDTH				(0x3333 + 0x18)
#define WM_DOWNLOAD_CHECKUPDATE						(0x3333 + 0x19)
#define WM_DOWNLOAD_SETPRIORITY						(0x3333 + 0x1a)

	// max url length(TCHAR not byte)
#define MAX_URL										4096

	// max number of IPPORT
//#define MAX_IPPORT									64

	// max num of p2pParam in P2PParamList
#define MAXNUM_P2PPARAM								1

	// default Timeout for p2p 
#define DOWNLOAD_P2P_TIMEOUT						100000

	// default Timeout for Http 
#define DOWNLOAD_HTTP_TIMEOUT						100000

	    //Download Center Flag
#define MAX_FILE_NUMBER_PER_REQUEST					20
   
	// 临时下载文件后缀
#define  TEMP_DOWNLOADFILENAME_SUFFIX   _T(".tdl")

	// 覆盖文件时备份文件后缀
#define  TEMP_BACKUPFILENAME_SUFFIX   _T(".bkp")

#define  IPGROUPDOWNLOADER			  _T("IPGROUPDOWNLOADER")
#define  HTTPDOWNLOADER			      _T("HTTPDOWNLOADER")
#define  P2PDOWNLOADER				  _T("P2PDOWNLOADER")

// Download Protocol Type
#define DOWNLOAD_PROTOCOL_TYPE_BASE					0x00
#define DOWNLOAD_PROTOCOL_TYPE_HTTP					0x01
#define DOWNLOAD_PROTOCOL_TYPE_P2P					0x02
#define DOWNLOAD_PROTOCOL_TYPE_FTP					0x03
#define DOWNLOAD_PROTOCOL_TYPE_UDP					0x04
#define DOWNLOAD_PROTOCOL_TYPE_UNKNOW				0xFF
		
    // p2p mode param
typedef struct tagP2P_PARAM
{
	TCHAR	szTorrentURL[MAX_URL];					// torrent文件URL地址,如"http://61.152.88.236/QQTangB2B12.tpt"
	TCHAR	szTCPServerIP[32];						// TCP模式的P2P服务器IP地址
	WORD	wTCPServerPort;							// TCP模式的P2P服务器端口
	TCHAR	szUDPServerIP[32];						// UDP模式的P2P服务器IP地址
	WORD	wUDPServerPort;							// UDP模式的P2P服务器端口
	TCHAR	szSTUNServerIP[32];						// STUN模式的P2P服务器IP地址
	WORD	wSTUNServerPort;						// STUN模式的P2P服务器端口
	DWORD	dwDelay;								// 下载完成后还提供多久的上传服务(单位:毫秒),0表示不延时
}P2PPARAM, *PP2PPARAM;

// url array(for redirect)
typedef struct tagUrlArray
{
	WORD wNumber;
	TCHAR szarrUrlArray[16][1024];
}URLARRAY;

// Ip and port information
// typedef struct tagIP_PORT_INFO
// {
// 	TCHAR	szHost[MAX_PATH];						// host name or ip(www.sohu.com,127.0.0.1)
// 	WORD	wPort;									// port
//}IPPORTINFO, *PIPPORTINFO;

    // Ip and port packets information
// typedef struct tagIP_PORT_PACKETS
// {
// 	WORD			dwNumber;						// number of packets
// 	IPPORTINFO		ipPortInfo[MAX_IPPORT];			// packets buffer
// }IPPORTPACKETS, *PIPPORTPACKETS;

// p2p参数列表
typedef struct tagP2PParamList 
{
	DWORD dwNumber;
	P2PPARAM stArrP2PParam[MAXNUM_P2PPARAM];
}P2PPARAMLIST, *PP2PPARAMLIST; 

	// Download add task param, for IProcessMessagerObject
	typedef struct tagDOWNLOAD_ADD_PARAM
	{
	//int				nEventID;					// event ID for callback pointer
	//int				nIndex;						// task index
	int				nVersion;					    // file version
	SOCK5PROXYPARAM sock5ProxyParam;				// sock5 proxy param
	//TCHAR			szUsername[MAX_PATH];			// username
	//TCHAR			szPassword[MAX_PATH];			// password
	//WORD			wProtocolType;					// protocol type
	TCHAR			szFilename[MAX_URL];			// file which will be downloaded
	TCHAR			szSaveFilename[MAX_PATH];		// file which will be save
	DWORD			dwBandWidth;					// net band width
	DWORD			dwTimeout;						// connect host timeout limit
	//DWORD			dwFlag;							// download flag(normar、continue...)
	IPPORTPACKETS	stIpPortPackets;				// ip&ports
	P2PPARAMLIST	stP2PParamList;					// p2p param,when wProtocolType = p2p
	}DOWNLOADADDPARAM, *PDOWNLOADADDPARAM;

// Download add task param, for IProcessMessagerObject
typedef struct tagDOWNLOAD_PARAM
{   
	SOCK5PROXYPARAM sock5ProxyParam;				// sock5 proxy param
	IPPORTINFO	    ipPortInfo;					    // ip&ports
	int             nVersion;                       // version
	TCHAR			szUsername[MAX_PATH];			// username
	TCHAR			szPassword[MAX_PATH];			// password
	TCHAR			szFilename[MAX_URL];			// file which will be downloaded
	TCHAR			szSaveFilename[MAX_PATH];		// file which will be save
	DWORD			dwBandWidth;					// net band width
	DWORD			dwTimeout;						// connect host timeout limit
	DWORD           dwVelocity;
}DOWNLOADPARAM, *PDOWNLOADPARAM;

// Download add task param, for IProcessMessagerObject
typedef struct tagDOWNLOAD_P2P_PARAM
{
    TCHAR			szURL[MAX_PATH];                        //URL of file to be downloaded
	TCHAR			szSaveFilename[MAX_PATH];
	int			    nBandWidth;					// net band width
	DWORD			dwTimeout;						// connect host timeout limit
	P2PPARAMLIST	stP2PParamList;						// p2p param,when wProtocolType = p2p
}DOWNLOADP2PPARAM, *PDOWNLOADP2PPARAM;


	// 缓存信息
	typedef struct tagCacheInfo 
	{
	TCHAR szFileName[MAX_PATH];
	int   nVersion;
	DWORD dwFileSize;
	DWORD dwLastModifiedTime;
	}CACHEINFO, *PCACHEINFO; 

	typedef struct tagFileList
 	{
 		int   nFileCount;
 		TCHAR szFileNameList[MAX_FILE_NUMBER_PER_REQUEST][MAX_PATH];
 	}
 	FILELIST, * PFILELIST;

	typedef struct tagP2PTaskInfo
 	{
 		DOWNLOADP2PPARAM   stP2PParam;
		HWND  hNotifyWnd;
 	}
 	P2PTASKINFO, *PP2PTASKINFO;

// 	typedef struct tagResourceList
// 	{
// 		int   nFileCount;
// 		DWORD dwPriorityList[MAX_FILE_NUMBER_PER_REQUEST];
// 		TCHAR szDownloadTypeList[MAX_FILE_NUMBER_PER_REQUEST][MAX_PATH];
// 		TCHAR szFileNameList[MAX_FILE_NUMBER_PER_REQUEST][MAX_PATH];
// 	}
// 	RESOURCELIST, * PRESOURCELIST;

};

#endif	// __DOWNLOADERDEF_I_H