#pragma once

#define MSGHEADLENGTH       (4*sizeof(short) + 2*sizeof(char) + sizeof(int))
#define MAXOPTLENGTH		127
#define MAXPACKAGESIZE		((short)0x7000)
#define MAXATTACHINFOLEN	2048
/*
以下定义所有消息类型
*/
#define Request         0
#define Response        1
#define Notify          2
#define Ack             3
#define Internal        4

/*
以下定义所有的FE
*/
#define FE_CLIENT            	0
#define FE_PLAYER           	1
#define FE_ROOM             	2
#define FE_GAMELOGIC          	3
#define FE_GAMESVRD           	4
#define FE_DBSVRD				5
#define FE_LOGSVRD				6
#define FE_DIRSVRD              7
#define FE_SHOPSVRD             8
#define FE_TEAM					9

enum enmGamemodeBaseIndex
{
	EGBI_BASEMODE_RANDOM = 0,
	EGBI_BASEMODE_RACE = 1,
	EGBI_BASEMODE_ITEM = 2,
	EGBI_BASEMODE_MAX
};


enum enmGamemodeSubIndex
{
	EGSI_SUBMODE_RANDOM = 0,
	EGSI_SUBMODE_SINGLE = 1,
	EGSI_SUBMODE_TEAM = 2,
	EGSI_SUBMODE_MAX
};

enum enReadyStateCmd
{
	State_Ready		=	0,	//准备
	State_UnReady	=	1,	//取消准备
};

enum enReadState
{
	State_UnReadied	=	0,	//非准备状态
	State_Readied	=	1,	//准备状态
};

enum enSeatStateCmd
{
	Seat_Close	=	0,	//关闭
	Seat_Open	=	1,	//打开
};

enum enSeatType
{
	Seat_Closed		= 0,  //关闭
	Seat_Free		= 1, // 空位
	Seat_Used		= 2   //有人
};

//默认为0 ，完胜为1
enum ENMWinType
{
	EWT_NORMAL = 0, //普通胜利
	EWT_PERFECT , //完胜
};

//道具准备状态
enum ENMITEMSTATUS
{
	EIS_UNEQUIPPED	=	0,          //未装备
	EIS_EQUIPPED	=	1,          //装备
};

enum EPropEffectResult
{
	EPER_ATTACKED = 0,//被击中
	EPER_OUTOFATTACK = 1,	//没被击中
	EPER_OUTOFATTACK_BECAUSEOFAVATAR =2,	//因为有avatar 所以没被击中
};


#define ST_PLAYER_INIT						(0x07)
#define ST_PLAYER_WAITFORGAMEINFO 			(0x08)
#define ST_PLAYER_INSINGLEROOM		   	    (0x09)  //在单人游戏房间, 玩家登陆时默认值
#define ST_PLAYER_INMULTIHALL               (15)  //在多人游戏大厅,登陆多人游戏但未进入MultiRoom 
#define ST_PLAYER_INMULTIROOM				(16)   //在多人游戏房间
#define ST_PLAYER_INMULTIGAME				(17)  //在多人游戏中
#define ST_PLAYER_INMULTIGAMEAWARD          (18)  //在多人游戏颁奖中
//others         //在单人游戏
#define ST_ROOM_IDLE				        0
#define ST_ROOM_WAITING			            1
#define ST_ROOM_INGAME				        2
#define ST_ROOM_AWARD                       3     //播放颁奖

#define MAXMODENUM    IDX_GM_MAX

///商品用
#define INTER_TIMEOUT	1	//内部超时消息
#define MAXRESULTSTRLENGTH	128 //结果字符串最大长度
#define MAX_ITEM_PERCOMMODITY 10 //商品中打包的最大道具数
#define MAX_COMMODITY_DESCLEN 128 //商品描述最大长度
#define	MAX_COMMODITY_NAMELEN 64 //商品名称最大长度
#define MAX_COMMODITY_NUM	500 //协议里的最大商品数

#define MAXITEMNUMONCEADD       20	//每次增加道具的最大种类个数
//#define MAXATTACHINFOLEN	128     //留言的最大长度

//商店支付类型
enum ENMPayType
{
	EPT_PAY_QB = 0,	//Q币
	EPT_PAY_QP,		//Q点
	EPT_PAY_KB,		//酷币
};

/*
用户信息相关
*/
#define MAXNICKNAMELEN          20
#define MAXATTACHCONTENTLEN    	200
#define MAXCLIENTGETROOMNUM    	60
#define MAXCSGAMEKEYLEN         32
#define MAXROOMNAMELEN         	22
#define MAXROOMPASSWORDLEN    	16
#define MAXSEATNUMINROOM       	8
#define MAXQQKARTNICKNAMELEN    17          //昵称的最大长度

#define MAXREGISTERADDITEMNUM   10          //注册时增加道具的最大数量

#define MAXITEMNUMPERPLAYER  	100			//每个玩家最多道具数
#define MAXGIFTNUMPERPLAYER 	50			//每个玩家最多礼物数
#define	MAXGROUPNUMPERPLAYER	20			//每个玩家最多好友分组数
#define MAXFRIENDNUMPERPLAYER	200			//每个玩家最多好友数
#define MAXFRIENDGROUPNAMELEN	20			//最大好友分组名称长度
#define MAXFRIENDNUMPERGROUP    100         //每个分组最大好友数
#define MAXFRIENDNUMPERPLAEYR   100         //最大好友数
#define MAXWORDSLEN             512         //增加好友留言长度
#define MAXPROPBLANK			2

/*
协议定义
*/
//0-499为客户端请求消息
#define ID_CMS_REQUESTLOGIN                  	100
#define ID_CMS_REQUESTLOGOUT                 	101
#define ID_CMS_REQUESTGETROOMLIST           	102
#define ID_CMS_REQUESTCREATEROOM           		103
#define ID_CMS_REQUESTENTERROOM             	104
#define ID_CMS_REQUESTLEAVEROOM            		105
#define ID_CMS_REQUESTQUICKENTERROOM         	106
#define ID_CMS_REQUESTCHANGESEATSTATE       	107
#define ID_CMS_REQUESTKICKFROMROOM              108
#define ID_CMS_REQUESTCHANGEREADYSTATE    		109
#define ID_CMS_REQUESTSTARTGAME                 110
#define ID_CMS_REQUESTTALK                      111
#define ID_CMS_REQUESTHELLO                     112
#define ID_CMS_REQUESTUDPOK                     499

#define ID_CMS_REQUESTGETFRIENDLIST             113
#define ID_CMS_REQUESTADDFRIEND                 114
#define ID_CMS_REQUESTDELETEFRIEND              115
#define ID_CMS_REQUESTADDFRIENDGROUP            116
#define ID_CMS_REQUESTDELETEFRIENDGROUP         117
#define ID_CMS_REQUESTMOVEFRIENDTOSOMEGROUP     118
#define ID_CMS_REQUESTDIRFT						119
#define ID_CMS_REQUESTPREPAREREADY 				120
#define ID_CMS_REQUESTCHANGEMAP                 121
#define ID_CMS_REQUESTFINDPLAYERBYQQ            122
#define ID_CMS_REQUESTREPORTCURRENTINFO			123
#define ID_CMS_REQUESTGETPROP                   124
#define ID_CMS_REQUESTUSEPROP                   125
#define ID_CMS_REQUESTPROPEFFECTRESULT          126
#define ID_CMS_REQUESTCHANGEAIMSTATE            127
#define ID_CMS_REQUESTCHANGETEAM                128
#define ID_CMS_REQUESTREGISTER         			129  
#define ID_CMS_REQUESTCHANGEITEMSTATUS			130
 //请求分配点数
#define ID_CMS_REQUESTASSIGNATTRIBUTE           131            
//请求洗点 
#define ID_CMS_REQUESTCLEARATTRIBUTE            132             
//请求注册名字是否有效
#define ID_CMS_REQUESTCHECKNAME                 133

#define ID_CMS_REQUESTGETTASKLIST				135		//拉取任务列表
#define ID_CMS_REQUESTTAKETASK					136     //请求接受任务
#define ID_CMS_REQUESTDELETETASK				137     //请求删除(注销)任务
#define ID_CMS_REQUESTSTARTSINGLETASK			138     //请求开始单局任务
#define ID_CMS_REQUESTREPORTCURRENTOPT          139		//上报操作序列
#define ID_CMS_REQUESTSKIPTASK                  140		//请求跳过任务

//500-999为服务器通知消息
#define ID_SMC_NOTIFYENTERROOM              	500
#define ID_SMC_NOTIFYLEAVEROOM              	501
#define ID_SMC_NOTIFYCHANGESEATSTATE     	 	502
#define ID_SMC_NOTIFYKICKFROMROOM              	503
#define ID_SMC_NOTIFYCHANGEREADYSTATE    		504
#define ID_SMC_NOTIFYTALK                       505
#define ID_SMC_NOTIFYTESTNETSPEED               506
#define ID_SMC_NOTIFYBEADDEDASFRIEND            507
#define ID_SMC_NOTIFYADDFRIENDRESULT            508
#define ID_SMC_NOTIFYGAMEBEGIN					509
#define ID_SMC_NOTIFYWAITUNREADYPLAYERS			510
#define ID_SMC_NOTIFYRACEBEGIN					511
#define ID_SMC_NOTIFYCOUNTDOWN 					512

#define ID_SMC_NOTIFYRACESHOW					515

#define ID_SMC_NOTIFYRACEOVER					513
#define ID_SMC_NOTIFYGAMEOVER					514

#define ID_SMC_NOTIFYGETPROP                    516
#define ID_SMC_NOTIFYUSEPROP                    517
#define ID_SMC_NOTIFYPROPEFFECTRESULT           518
#define ID_SMC_NOTIFYCHANGEAIMSTATE             519
#define ID_SMC_NOTIFYCHANGEMAP                  520
#define ID_SMC_NOTIFYDRIFT						521

#define ID_SMC_NOTIFYPLAYERFINISHRACE			522
#define ID_SMC_NOTIFYCHANGETEAM                 523
#define ID_SMC_NOTIFYCHANGEROOMOWNER			524
#define ID_SMC_NOTIFYREPLACEPROP				525
#define ID_SMC_NOTIFYADDPROP					526
#define ID_SMC_NOTIFYPLAYERCHANGEITEMSTATUS		527
#define ID_SMC_NOTIFYUPDATEMONEY				528  	//通知更新酷币
#define ID_SMC_NOTIFYADDITEM					529		//通知增加道具

#define ID_SMC_NOTIFYSINGLEGAMEBEGIN			530     //通知单人游戏开始
#define ID_SMC_NOTIFYSINGLERACEBEGIN			531		//通知单人赛开始
#define ID_SMC_NOTIFYSINGLERACEFINISH			532     //通知单人完成或超时
#define ID_SMC_NOTIFYSINGLERACEOVER             533		//通知个人赛结束
#define ID_SMC_NOTIFYSINGLEGAMEOVER				534     //通知个人游戏结束
#define ID_SMC_NOTIFYTASKAWARD                  537		//通知任务奖励

#define ID_SMC_NOTIFYKICKFROMSERVER				900
#define ID_SMC_NOTIFYUDPOK                      999

//反外挂
#define ID_CMS_NOTIFY_ANTI_BOT                  167

#define ID_CMS_REQUESTBUYCOMMODITY		20000	//请求购买
#define ID_CMS_REQUESTGETCOMMODITYLIST	20001	//请求拉商品列表

//登陆类型定义枚举
enum ENMLoginType
{
	ELT_Single = 0,  //单人游戏
	ELT_Multi = 1,  //多人游戏
};

//游戏模式
enum enGameMode
{
	eItem_Single	= 0,
	eItem_Muti		= 1,
	eSpeed_Single	= 2,
	eSpeed_Muti		= 3,
};

enum ENMMapRandomType
{
	EMRT_RANDOM_ALL = 0,    //所有随机
};


#define MAXCHATCONTENTLEN           500
#define MAXAWORDSIZE				10 
#define MAP_HASH_CODE				32
#define MAXCHECKPOINTNUM			100
//#define MAXREGISTERADDITEMNUM		5		//注册时增加道具的最大数量
#define	MAXCHANGEITEMSTATUSNUM		20		//一次能改变道具状态的最大数量
#define MAXAWARDITEMNUM				3		//完成任务奖励的最大道具数			
/*以下定义客户端的连接状态*/
#define ST_CLIENT_NOLINK	((BYTE)0x00) //未连接状态
#define ST_CLIENT_ONLINE	((BYTE)0x01) //在线状态
#define ST_CLIENT_OFFLINE	((BYTE)0x02) //断线状态
#define ST_PLAYER_INIT						(0x07)
#define ST_PLAYER_WAITFORGAMEINFO 			(0x08)
#define ST_PLAYER_INSINGLEROOM		   	    (0x09)  //在单人游戏房间, 玩家登陆时默认值

enum enTalkType
{
	Talk_Wisper = 0,  //密聊
	Talk_InRoom = 1, //房间
	Talk_InGame = 2, //游戏中
	Talk_Buglet = 3   //小喇叭
};

#define		SEATCLOSED		0
#define		SEATOPENED		1
#define		READEINROOM		0
#define		NOTREADYINROOM 	1

/*
以下定义结果编号
*/
typedef enum
{
	Succeed                 		=   0,
	Err_InvalidDestFE         	 	=   1,  //非法的目的实体
	Err_InvalidSrcFE        	   	=   2,  //非法的源实体
	Err_LowVersion            	 	=   3,  //版本过低
	Err_MultiLogin             		=   4,  //重复登录
	Err_ServerFull             		=   5,  //表示server的容量已满，需要玩家换地方
	Err_WaitPlayerInfoTimeOut  		=   6,  //取得玩家资料超时，请重试
	Err_GetPlayerInfoFailed    		=   7,  //取得玩加资料失败
	Err_WaitRegisterTimeOut  		=   8,  //注册超时，请重试
	Err_RegisterFailed    			=   9,  //注册失败
	Err_WaitSpecialTitlesTimeOut  	=   10, //取得玩家特殊称号超时，请重试
	Err_PointLessThanLowLimit  		=   11,  //积分不足
	Err_ForbidLogin            		=   12,  //禁止登陆
	Err_InvalidServerID        		=   13, //无效的服务器编号
	Err_InvalidRoomID         	 	=   14, //无效的房间编号
	Err_InvalidGameLogicID     		=   15, //无效的游戏编号
	Err_InvalidPlayerID        		=   16, //无效的玩家编号
	Err_InvalidUserIdef        		=   17, //玩家身份不满足进入当前服务器条件
	Err_PlayerNotOnLine        		=   18, //玩家不在线
	Err_PlayerNotOnServer         	=   19, //不在当前服务器
	Err_PlayerNotExist         		=   20, //玩家不存在
	Err_AddFriendOffline       		=   21, //加好友时对方不在线
	Err_AddFriendUpperLimit    		=   22, //好友数量已达上限
	Err_AddFriendRepeat        		=   23, //加好友时目的方已经在好友中
	Err_NoLimitKickGM          		=   24, //没有权限踢GM
	Err_InvalidAction         		=   25, //错误操作
	Err_PlayerNotInTeam  			=   26, //玩家不属于队伍
	Err_PlayerNotCaptain 			=   27, //玩家不是队长
	Err_TeamLeaveGame  				=   28, //队伍离开游戏
	Err_GetFriendListTimeOut 		=   29, //获取玩家好友列表超时
	Err_ListDeleteFriendTimeOut 	=   30, //玩家删除好友超时
	Err_InvalidUin    				=   31, //qq号码错误
	Err_RoomInGame                  =   32,//房间正在游戏中
	Err_NotUsedRoom                 =   33, //没有这个房间
	Err_InvalidRoomKey              =   34, //房间密码不正确
	Err_RoomFull                    =   35, //房间已满
	Err_IsNewPlayer              	=   1000, //新玩家需要注册
	Err_ResouceLack            		=   1001, //服务器资源不足
	Err_TimeOut                		=   1002, //操作超时
	Err_System                 		=   1003, //系统错误
	Err_Unknow                 		=   1004, //未知错误
} EResult;

/*
P2P协议部分
*/
#define ID_TCP_HELP_P2P_SEND_MSG 101
#define ID_TCP_HELP_P2P_RECV_MSG 102

//任务系统部分
#define MAXREGISTERADDTASKNUM	5         //注册时增加的最大任务数
#define MAXTASKNUMPERPLAYER     20		  //每个玩家拥有的最大任务数

/*-------------------------------------------------------------------
Dirsvrd 与Client 间消息部分
------------------------------------------------------------------*/									//在玩家看来Location是第一层目录，Gsvrd是第二层目录
#define HASH_CODE_LENGTH                   32           //hash码长度
#define MAX_DOWNSVR_NUMBER              10         //最大下载服务器数
#define MAX_DIRECTION_LENGTH              128         //下载根目录长度
#define MAX_FILE_NAME_LENGTH               128       //下载文件名长度
#define MAX_WORLD_NUM                      10        //最大world数目
#define MAX_WORLD_NAME_LEN                 16        //最大world名字长度
#define MAX_DIR_LISTEN_PORT_NUM               3        //dir最大监听端口数量
#define MAX_WORLD_DIR_SERVER_NUM       4    //world中目录服务器最大个数
#define MAX_SHOP_SERVER_NUM              5       //最大商店服务器个数
#define MAX_BORDER_SERVER_NUM           20      //最大边境服务器个数
#define MAX_REASON_LEN                    200      //最大原因长度
#define MAX_NETADMIN_SERVER_NUM         4       //最大网管服务器个数
#define MAX_MATCH_SERVER_NUM             6      //最大比赛服务器个数
#define MAX_COUNTRY_NUM                  5      //最大国家个数
#define MAX_COUNTRY_CHANNEL_NUM       10      //国家内的最大频道数
#define MAX_IDC_SERVER_NUM               10      //最大idc个数
#define MAX_IDC_GANE_SERVER_NUM         20     //IDC中游戏服务器最大个数


#define ID_CMD_REQGETHALLINFO         90     //Client  拉取服务器列表


/*---------------------------------------------------------------------
Client 与　Client 间的消息部分
----------------------------------------------------------------------*/

#define MAX_KEY_NUM			16
#define UDP_MSG_PLAYMOVE   20000
#define UDP_MSG_COLLSION   20001

//定义向Client返回的结果编号
enum ENMDirLoginResult
{
	EDLR_Succeed = 0,
	EDLR_Err_IdentityLimit  = 1, //未取得体验资格
	EDLR_Err_VersionLimit  = 2 , //版本过低
	EDLR_Err_VersionRecommend = 3, //版本升级建议
	EDLR_Err_FileCracked  = 4,  // 文件损坏
	EDLR_Err_WorldIDError = 5, //请求的worldid 不存在
};

#pragma  warning(disable:4200)
#pragma  warning(disable:4244)
#pragma pack(push,1)
#include "code.h"
#pragma pack(pop)