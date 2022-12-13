////////////////////////////////////////////////
//     FileName:    script.h
//     Author:      Fish
//     Date:        2006-10-11 10:46:58
//     Description: []
////////////////////////////////////////////////

#pragma once
#define UICALLBACK	void

struct	IWgtData;
struct	lua_State;
class	CScriptState;

enum 
{
	hosttype_play	= 1,
	hosttype_avatar	= 2,
	hosttype_nif	= 3,
	hosttype_scene  = 4,
	hosttype_shopitem = 5,
	hosttype_roomself = 6,
	hosttype_gameplay = 7,
};

enum
{
	customBufferdrawtype_effect = 1,	 
	customBufferdrawtype_flash  = 2,
};

enum
{
	restype_nif = 1,
};

struct stCustomBufferDrawEffect
{
	stCustomBufferDrawEffect()
		: m_bHasLoaded(false), m_bHasAnimation(false)
		, m_unResourceID(0) 
	{
	};
	~stCustomBufferDrawEffect()
	{		
		m_unResourceID = 0;
	}
	bool		m_bHasLoaded;
	bool		m_bHasAnimation;	//banimation;
	NiTimeController::CycleType m_enCycleType;//cycletype;
	unsigned int m_unResourceID;	
	unsigned int m_unResType;				// restype
	//NiNodePtr	m_spEffectNode;
	std::string m_strEffectName;			//resname
};

void RegisterScriptFuncs(CScriptState* pState);
int SC_GetDirInfo(lua_State *ls);
int SC_GetLastUserName(lua_State *ls);
int SC_Login( lua_State *ls );
int SC_Logout(lua_State* ls);
int SC_CancelLogin(lua_State *ls);
int SC_RegistUser(lua_State *ls);
int SC_LoginGame(lua_State* ls);
int SC_GetGameVar(lua_State *ls);
int SC_SetGameVar(lua_State *ls);
int SC_GetRecentUsers(lua_State *ls);
int SC_GetChatActionsDesc(lua_State *ls);
int SC_GotoUIScreen(lua_State *ls);
int SC_GetDisplayAttr(lua_State *ls);
int SC_ActiveExt(lua_State *ls);
int SC_GetRoomList(lua_State* ls);
int SC_CreateRoom(lua_State* ls);
int SC_ChangeMap(lua_State* ls);
int SC_ChangeTeam(lua_State* ls);
int SC_GetMapDesc(lua_State* ls);
int SC_QuickEnterRoom(lua_State* ls);
int SC_LeaveRoom(lua_State* ls);
int SC_GetMyInfo(lua_State* ls);
int SC_GetMyRoomInfo(lua_State* ls);
int SC_GetRoomPlayerInfo(lua_State* ls);
int SC_UinToSeat(lua_State* ls);
int SC_PIDToSeat(lua_State* ls);
int SC_EnterRoom(lua_State* ls);
int SC_SeatToIndex(lua_State* ls);
int SC_IndexToSeat(lua_State* ls);
int SC_ToggleSeatState(lua_State* ls);
int SC_ToggleReady(lua_State* ls);
int SC_ExitApp(lua_State* ls);
int SC_StartGame(lua_State* ls);
int SC_Talk(lua_State* ls);
int SC_TalkFilter(lua_State* ls);
int SC_Kick(lua_State* ls);
int SC_EnterUILayout(lua_State* ls);
int SC_AddBubble(lua_State* ls);
int SC_ClearBubble(lua_State* ls);
int SC_DoString(lua_State *ls);
int SC_LogText(lua_State *ls);
int SC_PlaySound(lua_State *ls);
int SC_StopSound(lua_State *ls);
int SC_FindPlayer(lua_State *ls);
int SC_Test(lua_State* ls);
int SC_AllocNif(lua_State* ls);
int SC_FreeNif(lua_State* ls);
int SC_NifAddRef(lua_State* ls);
int SC_NifRelease(lua_State* ls);
int SC_TexAddRef(lua_State* ls);
int SC_TexRelease(lua_State* ls);
int SC_GetGameRankInfo(lua_State * ls);
int SC_GetRaceTime(lua_State * ls);
int SC_GetLaps(lua_State * ls);
int SC_GetSpeed(lua_State * ls);

///播放mp3音乐，输入3个参数:
//1.音乐名称(相对于data\win32\sound\下的路径)
//2.播放状态 1=播放，0=停止
//3.循环标志 1=循环，0=停止
int SC_PlayMusic(lua_State * ls);
//把车子转到对于初始矩阵一个指定角度的位置
int SC_SelfShowFixedTurn(lua_State * ls);
///获取指定玩家所有Avatar的Item列表
int SC_GetAvatarAllList(lua_State* ls);
///改变指定玩家所有Avatar的Item状态
int SC_ChangeAvatarStatus(lua_State* ls);
///获取指定商品主分类的商品个数
int SC_GetShopCatItemNum(lua_State* ls);
///获取指定商品分类的分页商品真实商品Id
int SC_GetShopCatItemId(lua_State* ls);
///商品列表单个模型旋转显示
int SC_ShopSelfShowTurn(lua_State* ls);
///购买商品
int SC_ShopBuyItem(lua_State* ls);
///判断某个id的商品购买状态，看内部函数返回值
int SC_GetShopBuyState(lua_State* ls);
///转备指定商品
int SC_EquipShopItem(lua_State* ls);
///恢复原有装备状态
int SC_RestoreShopEquip(lua_State* ls);
///备份原有装备状态
int SC_BackupShopEquip(lua_State* ls);
///恢复原有装备状态
int SC_CleanShopEquipInfo(lua_State* ls);
///获取玩家拥有的按车库分类的avtar数量
int SC_GetAvatarTypeCount(lua_State* ls);
///获取玩家指定Avatar的Item信息
//在该车库分类中顺序排序的索引号，从0开始
int SC_GetAvatarInfoByType(lua_State* ls);
//////////////////////////////////////////////////////////////////////////
UICALLBACK IsKeyChar(void *pdata ,IWgtData* pWgtData=NULL);
UICALLBACK DrawKeyChar(void *pdata ,IWgtData* pWgtData=NULL);
UICALLBACK ValidKeyChar( void *pdata ,IWgtData* pWgtData=NULL);
UICALLBACK HitKeyChar(void *pdata, IWgtData *pWgtData=NULL);
UICALLBACK PlaySound(void *pdata ,IWgtData* pWgtData=NULL);
UICALLBACK DrawItem(void*, IWgtData*);
UICALLBACK UIBufferDecrypt(void *pdata, IWgtData *pWgtData=NULL);
UICALLBACK UIBufferEncrypt(void *pdata, IWgtData *pWgtData=NULL);
UICALLBACK UIParseCustomBuffer(void * pData, IWgtData * pWgtData=NULL);
UICALLBACK UIDrawCustomBuffer(void * pData, IWgtData * pWgtData=NULL);
UICALLBACK UICustomBufferSelfShow(void * pData, IWgtData * pWgtData = NULL);
UICALLBACK UICustomBufferDeinit(void * pData, IWgtData * pWgtData = NULL);
UICALLBACK UIGetCustomBuffer(void * pData, IWgtData * pWgtData = NULL);
UICALLBACK UISetCustomBuffer(void * pData, IWgtData * pWgtData = NULL);
//////////////////////////////////////////////////////////////////////////
int IndexToSeat(int index);
bool SetKartParam(int dwPID, float fParam, float fLifeTime);