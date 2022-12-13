#define MAX_NICK_LEN					255
#define MAX_TIP_LEN						100
#define MAX_MSG_LEN						700

#define MAX_PROFILE_LEN					2000
#define MAX_PASSWORD_LEN				32

#define MAX_PREVIEW_MSG_LEN				18

// 好友数目上限:  500 好友上限 20 群上限
#define MAX_BUDDYS_NUM					520 
#define MAX_GROUP_BUDDYS                200

#define MAX_GROUP_NUMBER				3
#define CONST_MAX_USERINFO_SIZE			256





#ifdef GAMEQQ_EXPORTS
#define GAMEQQ_API __declspec(dllexport)
#else
#define GAMEQQ_API __declspec(dllimport)
#endif

GAMEQQ_API void GameQQLogin(HWND hWnd, const char * szUIN, const char * szPassword, const BOOL bHideLogin);
GAMEQQ_API void GameQQOnTimer(void);
GAMEQQ_API void GameQQSendMsg(const DWORD dwUIN, const char * szMsg, BOOL bGroupMsg);
GAMEQQ_API void GameQQChangeStatus(const int nStatus);
GAMEQQ_API void GameQQGetProfile(const DWORD dwUIN);
GAMEQQ_API void GameQQAuthBuddy(const DWORD dwUIN, const BOOL bAccept);
GAMEQQ_API void GameQQAddBuddy(const DWORD dwUIN, const char * pszReason);
GAMEQQ_API void GameQQChangeUINLogin(const char * pszUIN, const char * pszPassword, const BOOL bHideLogin);
GAMEQQ_API void GameQQRefrashFriendList();
GAMEQQ_API void GameQQGetFriendGroup();
GAMEQQ_API void GameQQDelBuddy(const DWORD dwUIN);
GAMEQQ_API void GameQQLogOut();


#define WM_GAMEQQ_STOP_TIMER			WM_USER+9
#define WM_GAMEQQ_START_TIMER			WM_USER+10
#define WM_GAMEQQ_RECV_BUDDY_MSG		WM_USER+11
#define WM_GAMEQQ_RECV_GROUP_MSG		WM_USER+12
#define WM_GAMEQQ_BUDDY_ONLINE			WM_USER+13
#define WM_GAMEQQ_RECV_SYS_MSG			WM_USER+14
#define WM_GAMEQQ_NEED_AUTH_MSG			WM_USER+15
#define WM_GAMEQQ_NONEED_AUTH_MSG		WM_USER+16
#define WM_GAMEQQ_CHANGE_STATUS			WM_USER+17
#define WM_GAMEQQ_BUDDY_LIST			WM_USER+18
#define WM_GAMEQQ_GET_PROFILE			WM_USER+19
#define WM_GAMEQQ_SEND_MSG_TIMEOUT		WM_USER+20
#define WM_GAMEQQ_LOGIN_INFO_ERROR		WM_USER+21
#define WM_GAMEQQ_DEL_BUDDY				WM_USER+22
#define WM_GAMEQQ_GROUP_BUDDYLIST       WM_USER+23



// 在线状态
enum enumUserStat
{
	enumOnline =	10,
	enumOffline =	20,
	enumAway =	30,
	enumInvisible = 40,

	enumLoging =	50,
	enumFail =		60,
	enumPasswordErr = 70,
	enumTimeout =	80,
};



// 聊天消息
struct SMsgDesc
{
	int			nMsgType;
	char		szName[MAX_NICK_LEN+1];
	DWORD		dwUIN;
	char		szMsgTxt[MAX_MSG_LEN+1];
	int			nLen;

	WORD		wFace;
	COLORREF	dwTextColor;
	COLORREF	dwBackColor;

	int			bAgreeAddBuddy;
	//DWORD		dwGroupUIN;

	DWORD		dwUINInGroup;
	char		szUINInGroupName[MAX_NICK_LEN+1];
	WORD		wUINInGroupFace;
};


// Server保存最近的几条聊天纪录
/*
struct SServerCacheMsgDesc
{
	struct SMsgDesc msg[MAX_SERVER_CACHE_MSG_NUM];
	int nNum;
};
*/

// 好友简单信息
struct SBuddySimpleInfoDesc
{
	char			szName[MAX_NICK_LEN+1];
	char			szNote[256];
	DWORD			dwUIN;
	DWORD           dwGroupUIN;
	BOOL            bGroup;
	enumUserStat	enumStatus;
	WORD			wFace;
	QBYTE           chFlag;
	char			szGroupName[32];

}; 

// 好友列表
struct SBuddysListDesc
{
	struct SBuddySimpleInfoDesc BuddySimpleInfo[MAX_BUDDYS_NUM];
	int nNum;
};

struct SGroupBuddysListDesc
{
	struct SBuddySimpleInfoDesc BuddySimpleInfo[MAX_GROUP_BUDDYS];
	int nNum;
	DWORD dwGroupUIN;
};

typedef struct {
	char		m_szName[CONST_MAX_USERINFO_SIZE];
	DWORD		m_dwUIN;
	
	char		m_szAge[4];
	char		m_szGender[4];
	char		m_wFace;
	char		m_szCountry[CONST_MAX_USERINFO_SIZE];
	char		m_szProvince[CONST_MAX_USERINFO_SIZE];
	char		m_szCity[CONST_MAX_USERINFO_SIZE];
	
} BasicInfo;
typedef struct{
	char		m_szEmail[CONST_MAX_USERINFO_SIZE];
	char		m_szAddress[CONST_MAX_USERINFO_SIZE];
	char		m_szPostCode[CONST_MAX_USERINFO_SIZE];
	char		m_szTelNum[CONST_MAX_USERINFO_SIZE];
	BYTE		m_chPublic;
} ContactInfo;
typedef struct{
	char		m_szRealName[CONST_MAX_USERINFO_SIZE];
	char		m_szSchool[CONST_MAX_USERINFO_SIZE];
	char		m_szOccupation[CONST_MAX_USERINFO_SIZE];
	char		m_szHomePage[CONST_MAX_USERINFO_SIZE];
	char		m_chBirthDay;
	
	char		m_chBloodType;
	char		m_chConstellation;
	char		m_szIntroduce[CONST_MAX_USERINFO_SIZE];
	
} DetailInfo;
typedef struct{
	BasicInfo	m_basinInfo;
	ContactInfo m_contactInfo;
	DetailInfo	m_detailInfo;
} UserDetailInfo;
