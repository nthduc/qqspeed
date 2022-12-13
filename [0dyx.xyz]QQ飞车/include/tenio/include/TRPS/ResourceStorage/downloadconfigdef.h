// resourecestoragedef.h 
#pragma once
#include "TRPS/Downloader/downloaderdef.h"
#include "resourcestoragedef.h"

namespace Tenio
{
	// P2P config file name
#define    DLTYPE_FILE_DIR						_T("config\\Dltype.xml")
#define    DLPREDEFINED_FILE_DIR			    _T("config\\DlPredefined.xml")

   // dltype.xml �ֶ�
#define    DLTYPE_DLTYPE			                    _T("DlType")
#define    DLTYPE_DEFAULT			                    _T("Default")
#define    DLTYPE_HTTP									_T("Http")
#define    DLTYPE_SERVER			                    _T("server")
#define    DLTYPE_P2P									_T("P2P")
#define    DLTYPE_P2P_PARAM			                    _T("param")
#define    DLTYPE_PATH									_T("Path")
#define    DLTYPE_PATH_SERVERPATH			            _T("ServerPath")
#define    DLTYPE_PATH_LOCALPATH 		                _T("LocalPath ")
#define    DLTYPE_DEMAND			                    _T("demand")
#define    DLTYPE_DEMAND_LIST			                _T("list")
#define    DLTYPE_PARAM									_T("param")
#define    DLTYPE_PARAM_BANDWIDTH 						_T("BandWidth")
#define    DLTYPE_PARAM_TIMEOUT 						_T("TimeOut")

  // DlPredefined.xml�ֶ�
#define    DLPREDEFINED_PREDEFINED			            _T("Predefined")
#define    DLPREDEFINED_P2P			                    _T("P2P")
#define    DLPREDEFINED_P2PPARAM					    _T("p2pParam")
#define    DLPREDEFINED_P2PPARAM_COUNT			        _T("count")
#define    DLPREDEFINED_PEERSERVER						_T("PeerServer")
#define    DLPREDEFINED_PEERSERVER_TCP			        _T("tcp")
#define    DLPREDEFINED_PEERSERVER_UDP			        _T("udp")
#define    DLPREDEFINED_PEERSERVER_STUN		            _T("stun")
#define    DLPREDEFINED_IP								_T("ip")
#define    DLPREDEFINED_PORT							_T("port")
#define    DLPREDEFINED_HTTP			                _T("Http")
#define    DLPREDEFINED_SERVERLIST			            _T("ServerList")
#define    DLPREDEFINED_SERVERLIST_COUNT			    _T("count")
#define    DLPREDEFINED_SERVERLIST_SERVER			    _T("server")

#define    P2PFILE_EXTENSION							_T(".tpt")
#define    DOWNLOADANDSUBSTITUTE					    _T("DL2TEM")

#define  TASK_DELIMITER									_T('$')
#define  DEMAND_DELIMITER								_T(';')
#define  DEMANDTYPE_DELIMITER							_T(' ')
#define  COMMAND_KEYWORD_DOWNLOADTYPE					_T("DOWNLOADTYPE")
#define  COMMAND_KEYWORD_DOWNLOADFILENAME				_T("DOWNLOADFILE")

#define  SCRIPTFILEPATH			   _T("download//delayCommand.txt")
#define  DEMANDPARAM_SELF		   _T("THIS")
#define  DEMANDPARAM_ZIP_FOLDER	   _T("ZIP_FOLDER")
#define  DEMAND_KEYWORD_ATOMIC_BEGIN	   _T("ATOMIC{")
#define  DEMAND_KEYWORD_ATOMIC_END	       _T("}ATOMIC")
#define  DEMAND_KEYWORD_RESOURCE_BRGIN	   _T("RESOURCE{")
#define  DEMAND_KEYWORD_RESOURCE_END	   _T("}RESOURCE")

#define  DEMANDTYPE_MOVEFILE		   _T("MOVE")
#define  DEMANDTYPE_DELETEFILE		   _T("DELETE") 
#define  DEMANDTYPE_COPYFILE		   _T("COPY")
#define  DEMANDTYPE_REPLACEFILE		   _T("REPLACE")

#define  DEMANDTYPE_UNZIP		       _T("UNZIP")

#define  EXENAME_UNZIP		           _T("zip")

/**
 * @brief notice: ����·��(szSaveFilename)�͸���·��(szSubstituteFilename)Ϊ��ʱ��
 *				  Ҳ��Ϊ����Чֵ��
 *                ʵ��·����SetFileName()����Ŀɴ�·���ļ�������ʱ����
 *                ·��/����·��ƴ���γ�
 *                �����ʱ����·���͸���·����ͬʱ����ִ�и��ǲ���
 */
	typedef struct tagDownloadtypeParam
{
	TCHAR			szFilename[MAX_URL];
	
    // file which will be save (temporarily), for example : Download/
	TCHAR			szSaveFilename[MAX_PATH];	
	
	// file which will be replaced, for example : Tenio/
	TCHAR			szSubstituteFilename[MAX_PATH];	
	DWORD			dwBandWidth;					// net band width
	DWORD			dwTimeout;						// connect host timeout limit
	//DWORD			dwFlag;							// download flag(normar��continue...)
	IPPORTPACKETS	stIpPortPackets;				// ip&ports
	P2PPARAMLIST	stP2pParamList;					// p2p param
	TCHAR           szCommand[COMMAND_MAXLENGTH];   // demand list
}DOWNLOADTYPEPARAM, *PDOWNLOADTYPEPARAM;

};