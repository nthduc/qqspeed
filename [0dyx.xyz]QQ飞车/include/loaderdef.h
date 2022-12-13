#ifndef __LOADDEF_H__
#define __LOADDEF_H__

#define PAGE_SIZE	1024
#define KART_MAP	"KartMap_Shared"

#pragma pack(1)
typedef struct tagSharedData
{
	HWND	hWnd;
	DWORD	dwUin;
	BYTE	byPwd[20];
	DWORD	dwWorldID;
}SharedData;

typedef enum tagLoadCopyDataType
{
	EDT_NotifyLoginResult	=	1,
}LoadCopyDataType;

#define MAX_MESSAGE_LEN	100
typedef struct tagNotifyLoginResult
{
	HWND	hWnd;
	DWORD	dwEntity;
	INT		nLoginResult;
	CHAR	strErrorMessage[MAX_MESSAGE_LEN];
}NotifyLoginResult;
#pragma pack()

#endif