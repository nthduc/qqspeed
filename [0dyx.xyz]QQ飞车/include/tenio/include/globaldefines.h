#ifndef __GLOBALDEFINES_H__
#define __GLOBALDEFINES_H__

namespace Tenio
{
	
#define MAX_CHAR_COUNT_OF_ROOM_NAME		1024	// 房间名字的最大字符个数
#define MD5HASHEDLEN					16
#define MAX_URL_LENGTH					1024	
#define MAX_URLCAPTION_LENGTH           64
#define MAX_SERVER_NAME					64
#define MAX_HOST_LENGTH					64
#define MAX_IPSTRING_LENGTH				32
#define MAX_INFO_LENGTH					256
	
#define DIR_DYNAMIC_CONFIG_FILE		TEXT("Config\\Dynamic\\DynaCfg.ini")
#define VERSION_CONFIG_FILE		    TEXT("Config\\Original\\Version.ini")
#define URLS_CONFIG_FILE			TEXT("Config\\Original\\URLs.ini")
#define DIRSERVER_CONFIG_FILE		TEXT("Config\\Original\\DirSvrs.ini")
#define LOGIN_CONFIG_FILE			TEXT("Config\\Original\\Login.ini")
#define DEBUG_CONFIG_FILE			TEXT("Config\\Original\\Debug.ini")
#define GAMESINFODATA_FILE			TEXT("Config\\Dynamic\\GameInfo.dat")
#define SERVERLISTDATA_FILE			TEXT("Config\\Dynamic\\SvrInfo.dat")
#define DYNAMIC_INFO_FILE			TEXT("Config\\Dynamic\\Info.ini")
	
};
#endif