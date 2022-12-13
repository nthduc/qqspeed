struct CCSHead
{
	LONG	m_iTotalLength;
	SHORT	m_nVer;
	SHORT	m_nDialogID;
	LONG	m_iSeq;
	ULONG	m_iUin;
	UCHAR	m_byBodyFlag;
	UCHAR	m_byOptLength;
	UCHAR	m_abyOption[0];
};

class CMsgHead
{
	SHORT m_sMsgID;
	SHORT m_sMsgType;
	LONG  m_iMsgSeq;
	CHAR  m_bSrcFE;
	CHAR  m_bDstFE;
	SHORT m_sSrcID;
	SHORT m_sDstID;
};

//UDP传输头
class CTransferHead
{
	SHORT		m_nTotalLength;
	UCHAR 		m_cVer;
	LONG  		m_iSeq;
	SHORT		m_nDialogID;
	ULONG 		m_unUin;
	SHORT		m_nTransferCmd;
	SHORT		m_nCheckSum;
	UCHAR  		m_byOptLength;
	UCHAR  		m_abyOption[MAXOPTLENGTH];
};



struct STUdpPack
{
	ULONG MsgID;
};

class STUDPPlayMove : public STUdpPack
{
	ULONG	m_unUin;							//源玩家uin
	UCHAR	m_cHasKeyChange;					//有按键变化没有
	UCHAR	m_cArrKey[MAX_KEY_NUM];				//按键状态（在有按键变化时发挥作用）
	ULONG	m_unPlayAnimID;						//玩家当前动作Id
	ULONG	m_unKartAnimID;						//车当前动作Id
	LONG	m_unLinearVelocity[3];				//本地玩家赛车线速度
	LONG	m_unArrPosition[3];
	LONG	m_unArrMat[9];
	LONG	m_unOffSetZ;				
	LONG	m_unAngle;						//本地玩家赛车姿态（包括旋转矩阵和位置）
	LONG	m_nRestDistance;				// 还剩多少距离完成比赛
	ULONG	m_unPackageIndex;					//包序号
	ULONG	m_unTimeStamp;						//时间戳
	UCHAR	m_cisDrifting;
};

class STUDPPlayCollision : public STUdpPack
{
	ULONG	m_unSelfUin;
	ULONG   m_unOtherUin;
	LONG	m_nSelfLinearVel[3];
	LONG	m_nOtherLinearVel[3];
	LONG	m_nSelfPos[3];
	LONG    m_nOtherPos[3];
	ULONG	m_unTime;
	LONG	buf[50];
};


//消息体
class CMsgPara
{
};

//通用请求和应答消息
class CRequestPara : public CMsgPara
{
    ULONG m_unUin;
    ULONG m_unTime;
};

class CResponsePara : public CMsgPara
{
    SHORT	m_sResultID;
};

struct STRoomInfo
{
	SHORT	m_nRoomID;
	SHORT	m_sGameModeBase; 
	SHORT	m_sGameModeSub; 
	UCHAR	m_bCurrentPlayerNum;
	UCHAR	m_bTotalSeatNum;
	UCHAR	m_bStatus;		//是否游戏等
	LONG	m_iMapID;
	UCHAR 	m_bFlag;		//是否加密
	CHAR	m_szRoomName[MAXROOMNAMELEN]; 
};


//玩家数据库基本信息
struct STPlayerDBBaseInfo
{
	CHAR	m_szNickname[MAXQQKARTNICKNAMELEN];  	//玩家的昵称
	UCHAR	m_bGender;   		 				//玩家性别，0表示女，1表示男
	UCHAR	m_bCountry;	                                             //玩家国家
	ULONG	m_unExperience;		       //经验
	LONG 	m_iMoney;			       //金钱
	ULONG	m_unWinNum;			//胜利次数
	ULONG	m_unTotalNum;			//比赛总数
	ULONG	m_unHonor;                              //功勋值
};

//玩家属性信息
struct STPlayerDBAttributeInfo
{
	USHORT m_nSpeed;               //速度
	USHORT m_nExplosion;         //爆发
	USHORT m_nGathering;         //集气
	USHORT m_nSpout;                //喷射
	USHORT m_nMiniSpout;        //小喷
	ULONG	m_unReserve;             //未分配
	UCHAR  m_bStrategy;            //自动分配策略
};

//玩家其他信息
struct STPlayerDBOtherInfo
{
	LONG m_iRightFlag;		//是否禁言
	LONG m_iAvailPeriod;	//禁言时长	
};

//玩家道具信息
struct STPlayerDBItemInfo
{
	LONG	m_iItemID;			//道具ID 
	LONG	m_iItemNum;			//道具个数
	LONG	m_iAvailPeriod;		//道具有效期　
	UCHAR 	m_bStatus;			//道具状态（0未装备,1装备）		
	ULONG	m_unObtainTime;		//获得时间	
	LONG	m_iOtherAttribute;	//其他（保留字段）
};

//玩家的表现性信息，要报给同一房间内其他玩家
struct STPlayerRoomVisibleInfo
{   
	ULONG	m_unUin;				//Uin
	ULONG	m_unIdentity;			//玩家身份                
	UCHAR 	m_bSeatID;         		//座位ID
	UCHAR 	m_bTeamID;  			//队伍号
	USHORT 	m_nPlayerID;			//玩家ID
	USHORT  m_nRoleID;              //角色id，暂定
	USHORT 	m_nKartID;				//车ID
	UCHAR 	m_bStatus;             	//状态 
	STPlayerDBBaseInfo m_stPlayerDBBaseInfo;    //基础信息
	USHORT	m_nEquipedItemNum;
	STPlayerDBItemInfo m_stPlayerItemInfo[MAXITEMNUMPERPLAYER];	
};
//游戏信息
struct STGameInfo
{
	STPlayerDBBaseInfo	m_stPlayerDBBaseInfo;					//玩家数据库基本信息
	STPlayerDBAttributeInfo    m_stPlayerDBAttributeInfo;                                    //玩家属性信息
	USHORT		m_nItemNum;
	STPlayerDBItemInfo	m_stPlayerDBItemInfo[MAXITEMNUMPERPLAYER];
	STPlayerDBOtherInfo	m_stPlayerDBOtherInfo;						//其他信息
};

//玩家任务信息
struct STPlayerDBTaskInfo 
{
	LONG m_iTaskID;       //任务ID
	SHORT m_shOptionID; //接受方式
	CHAR m_cStatus;      //任务状态：0未完成，1完成
	UCHAR m_bPrevStatus;	//前置任务的状态
	LONG m_iProgressRate;    //当前完成或收集的数量，用于标识任务进度
	LONG m_iOther;        //备用，用于标识任务进度
	LONG m_iTakeTime;     //任务接受时间

};

//价格
struct STPrice
{
	LONG m_iQBPrice;	//Q币
	LONG m_iQPPrice;	//Q点
	LONG m_iKBPrice;	//酷币
};

//分类
struct STType
{
	SHORT m_shBase; //大类
	SHORT m_shSub; //子类
};

//描述
struct STDescription
{
	CHAR m_szGeneralDesc[MAX_COMMODITY_DESCLEN]; //总体描述
	CHAR m_szAttributeDesc[MAX_COMMODITY_DESCLEN]; //属性描述
};


//折扣
struct STRebate
{
	SHORT m_shMemeber; //会员
	SHORT m_shCommon; //普通玩家
};

//限制
struct STLimit
{
	LONG m_iLevelLimit;//等级限制
	LONG m_iHonorLimit;//荣誉限制
	LONG m_iOtherLimit;
};

//道具信息
struct STItemInfo
{
	LONG m_iItemID;
	LONG m_shAvailPeriod;
	SHORT m_shItemNum;
};


//商品描述
struct STCommodity
{
	LONG m_iID; //ID
	CHAR m_szName[MAX_COMMODITY_NAMELEN]; //名称
	STType m_stType; //类型
	STDescription m_stDescription; //描述
	STRebate m_stRebate; //折扣
	STLimit m_stLimit; //购买限制
	STPrice m_stPrice; //价格
	LONG m_iDateSaleLimit;//每日销售限额
	LONG m_iClientVersion;//支持的客户端版本
	LONG m_iPriority; //优先级
	LONG m_iSaleTag; //标签（热卖，推荐等）
	SHORT m_shItemCount;
	STItemInfo m_astItemInfo[MAX_ITEM_PERCOMMODITY];//商品中打包的道具
	LONG m_iDateSold; //当天已卖出的个数
};


//请求登陆游戏
class CRequestLoginPara:public CMsgPara
{
	ULONG 	m_unUin;							//玩家的qq号码
	ULONG 	m_unTime;
	ULONG 	m_unClientVersion;  				//客户端版本号
	CHAR 	m_szQQNickname[MAXNICKNAMELEN];  	//玩家的昵称
	UCHAR 	m_bGender;   						//玩家性别，0表示女，1表示男
	UCHAR 	m_bIconID;  						//玩家QQ头像ID
	ULONG 	m_unAttachIdentify;  				//版本信息，如网吧版
	ULONG	m_uiLoginType;                   //登陆类型，见ENMLoginType
};

//回复登陆游戏
class CResponseLoginPara :public CMsgPara
{
	SHORT 		m_nResultID;                
	UCHAR 		m_bKeyGameDataLength;
	CHAR		m_szKeyGameData[MAXCSGAMEKEYLEN]; 
	ULONG 		m_unUin;
	ULONG 		m_unIdentity; 						 	//玩家身份
	USHORT 		m_nPlayerID;
	ULONG		m_uiLoginType;                 //登陆类型，见ENMLoginType
	USHORT		m_nRoleID;                                    //角色id，暂定
	USHORT		m_nKartID; //车ID		
	ULONG 		m_unGameSvrdIdentity;  					//一些gsvrd的信息，如是否免费等
	STPlayerDBBaseInfo	m_stPlayerDBBaseInfo;					//玩家基本信息
	STPlayerDBAttributeInfo     m_stPlayerDBAttributeInfo;                                 //玩家属性信息
	USHORT		m_nItemNum;
	STPlayerDBItemInfo	m_astPlayerDBItemInfo[MAXITEMNUMPERPLAYER];	
	SHORT		m_ushTaskNum; 
	STPlayerDBTaskInfo	m_astPlayerDBTaskInfo[MAXTASKNUMPERPLAYER];	//任务信息
	UCHAR 		m_bReasonLen;
	CHAR 	m_szReason[MAXATTACHCONTENTLEN];	
};

//请求登出游戏
class CRequestLogoutPara:public CMsgPara
{
	ULONG 	m_unUin;
	ULONG 	m_unTime;
};


//回复登出游戏
class CResponseLogoutPara:public CMsgPara
{
	SHORT	m_nResultID;						//0表示成功，其他失败
	UCHAR 	m_bReasonLen;
	CHAR 	m_szReason[MAXATTACHCONTENTLEN];	//失败原因说明
};

//玩家请求注册
class CRequestRegisterPara :public CMsgPara
{
	ULONG	m_unUin;                      		//玩家的Uin
	ULONG	m_unTime;		       		//玩家注册时间	
	CHAR    m_szNickname[MAXQQKARTNICKNAMELEN];  	//玩家的昵称
	UCHAR 	m_bGender;   		 				//玩家性别，0表示女，1表示男
	LONG	m_nFaceID;                                  //玩家脸型
	LONG	m_nHairID;                                   //玩家头发
	UCHAR   m_bDressIndex;                       //套装索引
	LONG 	m_nColorID;		       		//玩家喷漆颜色ID
	UCHAR  m_bCountry;	                                             //玩家国家
};

//回复玩家注册
class CResponseRegisterPara :public CMsgPara
{
	SHORT	m_nResultID; 				//0表示成功，其他失败
	ULONG 	m_unUin;					//注册玩家的uin
	CHAR	m_szNickname[MAXQQKARTNICKNAMELEN];  	//玩家的昵称
	UCHAR 	m_bGender;   		 				//玩家性别，0表示女，1表示男
	UCHAR	m_bCountry;	                                             //玩家国家
	USHORT 	m_nAddItemNum;
	STPlayerDBItemInfo	m_stAddItemInfo[MAXREGISTERADDITEMNUM]; 
	USHORT	m_ushAddTaskNum;
	STPlayerDBTaskInfo	m_astAddTaskInfo[MAXREGISTERADDTASKNUM]; //注册时增加的任务信息
	UCHAR 	m_bReasonLen;
	CHAR 	m_szReason[MAXATTACHCONTENTLEN]; 
};

//请求拉取房间列表
class CRequestGetRoomListPara:public CMsgPara
{
	ULONG	m_unUin;
	ULONG 	m_unTime;
	SHORT	m_nStartRoomNum;  	//指定拉取开始位置
	SHORT	m_nGetRoomNum; 		//个数	
	SHORT	m_sGameModeBase;
	SHORT	m_sGameModeSub;
};



//回复拉取房间列表
class CResponseGetRoomListPara:public CMsgPara
{
	SHORT		m_nResultID;
	SHORT		m_nCurTotalRoomNum;
	SHORT		m_nStartIndex;
	SHORT		m_nRoomNum;
	STRoomInfo 	m_stRooms[MAXCLIENTGETROOMNUM];
	UCHAR		m_bReasonLen;
	CHAR 		m_szReason[MAXATTACHCONTENTLEN];	
	SHORT		m_sGameModeBase;
	SHORT		m_sGameModeSub;
};

//请求创建房间
class CRequestCreateRoomPara:public CMsgPara
{
	ULONG 	m_unUin;
	ULONG 	m_unTime;
	SHORT	m_sGameModeBase; 
	SHORT	m_sGameModeSub; 
	CHAR 	m_szRoomName[MAXROOMNAMELEN]; 
	UCHAR	m_bSeatNum;
	UCHAR	m_bFlag; 		//是否加密等
	CHAR 	m_szPassword[MAXROOMPASSWORDLEN]; 
};

//回复创建房间
class CResponseCreateRoomPara:public CMsgPara
{
	SHORT	m_nResultID;
	SHORT	m_nRoomID;  //房间编号
	SHORT	m_sGameModeBase; 
	SHORT	m_sGameModeSub; 
	CHAR 	m_szRoomName[MAXROOMNAMELEN];
	UCHAR	m_bSeatNum;
	UCHAR	m_bFlag; 
	UCHAR 	m_bSeatID;  //房间座位号
	UCHAR 	m_bTeamID;  //队伍号
	UCHAR 	m_bReasonLen;
	CHAR 	m_szReason[MAXATTACHCONTENTLEN];	
};

//请求进入房间
class CRequestEnterRoomPara:public CMsgPara
{
	ULONG 	m_unUin;
	ULONG 	m_unTime;
	SHORT	m_nRoomID;
 	CHAR 	m_szPassword[MAXROOMPASSWORDLEN];	
};


//回复进入房间
class CResponseEnterRoomPara:public CMsgPara
{
	SHORT	m_nResultID;
	SHORT	m_nRoomID;
	SHORT	m_sGameModeBase; 
	SHORT	m_sGameModeSub; 
	CHAR 	m_szRoomName[MAXROOMNAMELEN]; 
	UCHAR	m_bSeatNum;
	UCHAR	m_bFlag; 	
	UCHAR 	m_bSeatID;  //房间座位号
	UCHAR   m_bTeamID;  //队伍号
    LONG	m_nMapID;               
    SHORT	m_nRoomOwnerID; 
	UCHAR 	m_bSeatStatus[MAXSEATNUMINROOM];         
    UCHAR 	m_bPlayerCount;         
	STPlayerRoomVisibleInfo 	m_stPlayerInfo[MAXSEATNUMINROOM];		
	UCHAR 	m_bReasonLen;
	CHAR 	m_szReason[MAXATTACHCONTENTLEN];
};


//通知进入房间
class CNotifyEnterRoomPara:public CMsgPara
{
	SHORT			m_nRoomID;
	STPlayerRoomVisibleInfo	m_stNewPlayerInfo; 
};

//请求离开房间
class CRequestLeaveRoomPara:public CMsgPara
{
	ULONG 	m_unUin;
	ULONG 	m_unTime;
};

//回复离开房间
class CResponseLeaveRoomPara:public CMsgPara
{
	SHORT	m_nResultID;
	UCHAR	m_bReasonLen;
	CHAR 	m_szReason[MAXATTACHCONTENTLEN];
};

//通知离开房间
class CNotifyLeaveRoomPara:public CMsgPara
{
	SHORT	m_nRoomID;
	ULONG 	m_unUin;
	UCHAR 	m_bSeatID; 
	CHAR 	m_cIsChangeRoomOwner; //0 不更换 1 更换
	SHORT	m_nNewRoomOwnerID; 
};

class CRequestTransferUDPOKPara : public CMsgPara
{
	ULONG	m_unUin;                
	ULONG	m_unTime;               
	SHORT	m_nDstDlg;              
	ULONG	m_unDstUin;             
	SHORT	m_nInfoLength;          
	CHAR	m_szInfo[32];   
};

class CResponseTransferUDPOKPara : public CMsgPara
{
	SHORT m_nResultID;
};

class CNotifyUDPOKPara : public CMsgPara
{
	ULONG	m_unUin;                
	ULONG	m_unTime;               
	SHORT	m_nSrcDlg;              
	ULONG	m_unSrcUin;             
	SHORT	m_nInfoLength;          
	CHAR	m_szInfo[32];     
};

//请求快速加入
class CRequestQuickEnterRoomPara : public CMsgPara
{
	ULONG	m_unUin;		//qq号
	ULONG	m_unTime;		//时间
	SHORT	m_sGameModeBase; 
	SHORT	m_sGameModeSub; 
};


//回复快速加入
class CResponseQuickEnterRoomPara : public CMsgPara
{
	SHORT 	m_nResultID;
	SHORT 	m_nRoomID;
	SHORT	m_sGameModeBase; 
	SHORT	m_sGameModeSub; 
	CHAR 	m_szRoomName[MAXROOMNAMELEN]; 
	UCHAR	m_bSeatNum;
	UCHAR	m_bFlag; 	
	UCHAR 	m_bSeatID;  //房间座位号
	UCHAR   m_bTeamID;  //队伍号
	LONG 	m_nMapID;               
	SHORT	m_nRoomOwnerID; 
	UCHAR 	m_bSeatStatus[MAXSEATNUMINROOM];         
	UCHAR 	m_bPlayerCount;         
	STPlayerRoomVisibleInfo 	m_stPlayerInfo[MAXSEATNUMINROOM];		
	UCHAR 	m_bReasonLen;
	CHAR 	m_szReason[MAXATTACHCONTENTLEN];
};

//请求改变座位状态
class CRequestChangeSeatStatePara:public CMsgPara
{
	ULONG 	m_unUin;
	ULONG	m_unTime;   
	SHORT 	m_nRoomID;		//房间号
	UCHAR	m_bSeatID;		//要改变的座位号
	UCHAR	m_bSeatState;	//座位改变后的状态（0表示关闭，1表示打开）
};


//回复改变座位状态
class CResponseChangeSeatStatePara:public CMsgPara
{
	SHORT 	m_nResultID;
	UCHAR	m_bSeatID;		//被改变的座位号
	UCHAR	m_bSeatState;	//座位改变后的状态
	UCHAR	m_bReasonLen;
	CHAR	m_szReason[MAXATTACHCONTENTLEN];
};

//通知改变座位状态
class CNotifyChangeSeatStatePara:public CMsgPara
{
	UCHAR 	m_bSeatID;		//被改变的座位号
	UCHAR	m_bSeatState;	//座位改变后的状态
};

//请求踢人
class CRequestKickFromRoomPara:public CMsgPara
{
	ULONG 	m_unUin;
	ULONG	m_unTime;   
	ULONG	m_unDstUin;		//被踢玩家的Uin
};

//回复踢人
class CResponseKickFromRoomPara:public CMsgPara
{
	SHORT 	m_nResultID;
	ULONG 	m_unDstUin;		//被踢者Uin
	UCHAR	m_bReasonLen;
	CHAR	m_szReason[MAXATTACHCONTENTLEN];

};

//通知踢人
class CNotifyKickFromRoomPara:public CMsgPara
{
	SHORT 	m_nRoomID;
	ULONG 	m_unDstUin;		//被踢玩家的Uin
};


//请求改变准备状态
class CRequestChangeReadyStatePara:public CMsgPara
{
	ULONG 	m_unUin;
	ULONG 	m_unTime;
	UCHAR	m_bReadyState;	//准备的状态（0表示准备，1表示取消准备）
};

//回复改变准备状态
class CResponseChangeReadyStatePara:public CMsgPara
{
	SHORT 	m_nResultID;
	UCHAR	m_bReadyState;
	UCHAR	m_bReasonLen;
	CHAR	m_szReason[MAXATTACHCONTENTLEN];
};

//通知改变准备状态
class CNotifyChangeReadyStatePara:public CMsgPara
{
	ULONG 	m_unUin;	
	UCHAR	m_bReadyState;
};

//聊天 m_bDstFE=FE_PLAYER
class CRequestTalkPara : public CMsgPara
{
	ULONG 	m_unUin;                
	ULONG 	m_unTime;               
	UCHAR 	m_bType;			//密语/房间/游戏中/小喇叭等
	ULONG 	m_unDestPlayerUin;  //私聊
	SHORT 	m_nContentLength;       
	CHAR 	m_szContent[MAXCHATCONTENTLEN]; 
};

class CResponseTalkPara : public CMsgPara
{
	SHORT	m_nResultID;
	UCHAR 	m_bType;
	ULONG 	m_unDestPlayerUin;
	UCHAR 	m_bReasonLen;
	CHAR 	m_szReason[MAXATTACHCONTENTLEN];
};

//通知聊天
class CNotifyTalkPara : public CMsgPara
{
	ULONG	m_unSrcUin;
	CHAR	m_szSrcNickName[MAXNICKNAMELEN];
	UCHAR	m_bGender;    
	UCHAR	m_bType;			//密语/房间/游戏中/小喇叭等
	ULONG	m_unDestPlayerUin;  //私聊
	SHORT	m_nContentLength;       
	CHAR	m_szContent[MAXCHATCONTENTLEN];
};

//心跳  m_bDstFE=FE_PLAYER
class CRequestHelloPara : public CMsgPara
{
	ULONG	m_unUin;
	ULONG	m_unTime;
	UCHAR	m_bInfoLength;
	CHAR	m_abyInfo[MAXATTACHCONTENTLEN];
};

class CResponseHelloPara : public CMsgPara
{
	SHORT	m_nResultID; 
	UCHAR	m_bReasonLen;
	CHAR	m_szReason[MAXATTACHCONTENTLEN]; //失败原因说明
};

//测网速
class CNotifyTestNetSpeedPara : public CMsgPara
{
	ULONG	m_unStartSec;	//开始秒
	ULONG	m_unUSec;		//开始微秒
};

//m_bDstFE=FE_PLAYER
class CAckTestNetSpeedPara : public CMsgPara
{
	ULONG	m_unStartSec;	//开始秒
	ULONG	m_unUSec;		//开始微秒
};

class CNotifyKickFromServerPara: public CMsgPara
{
	SHORT	m_nReasonID;     
	ULONG	m_unSrcUin;
	UCHAR	m_bReasonLen; 
	CHAR	m_szReason[MAXATTACHCONTENTLEN];
};


//－－－－－－－－－－－－好友系统－－－－－－－－－－－－－
//好友分组信息
struct STFriendGroupInfo
{
	CHAR	m_szFriendGroupName[MAXFRIENDGROUPNAMELEN];
	SHORT	m_nFriendGroupID;
	SHORT	m_nCurrentFriendNum;
	ULONG	m_unFriendUin[MAXFRIENDNUMPERGROUP];
};


//
struct STPlayerInfoForFriend
{
	CHAR	m_szNickName[MAXNICKNAMELEN];
};

//请求拉取好友列表
class CRequestGetFriendListPara : public CMsgPara
{
	ULONG	m_unUin;
	ULONG	m_unTime;
};

class CResponseGetFriendListPara : public CMsgPara
{
	SHORT	m_nResultID;            
	ULONG	m_unUin;                
	SHORT	m_nMaxFriendNum;        
	SHORT	m_nTotalFriendNum;
	SHORT	m_nFriendGroupNum;
	STFriendGroupInfo m_astFriendGroupInfo[MAXGROUPNUMPERPLAYER];
	UCHAR	m_bReasonLen;
	CHAR	m_szReason[MAXATTACHCONTENTLEN];
};

//请求增加好友
//好友增加成功后, 好友均位于默认分组中
class CRequestAddFriendPara : public CMsgPara
{
	ULONG	m_unUin;
	ULONG	m_unTime;
	ULONG	m_unDstUin;
	SHORT	m_nWordLength;
	CHAR	m_szWords[MAXWORDSLEN];
};

class CResponseAddFriendPara : public CMsgPara
{
	SHORT	m_nResultID;
	ULONG	m_unUin;
	ULONG	m_unDstUin;
	UCHAR	m_bReasonLen;
	CHAR	m_szReason[MAXATTACHCONTENTLEN];
};

//请求删除好友
class CRequestDeleteFriendPara : public CMsgPara
{
	ULONG	m_unUin; 
	ULONG	m_unTime;
	ULONG	m_unFriendUin;
	SHORT	m_nFriendGroupID;      //必须指明邋分组ID
};

class CResponseDeleteFriendPara : public CMsgPara
{
	SHORT	m_nResultID;
	ULONG	m_unUin;
	ULONG	m_unFriendUin;
	SHORT	m_nFriendGroupID;
	UCHAR	m_bReasonLen;
	CHAR	m_szReason[MAXATTACHCONTENTLEN]; //原因说明
};


//请求增加好友分组
class CRequestAddFriendGroupPara : public CMsgPara
{
	ULONG	m_unUin;
	ULONG	m_unTime;
	UCHAR	m_bGroupNameLen;
	CHAR	m_szGroupName[MAXFRIENDGROUPNAMELEN];
};

class CResponseAddFriendGroupPara : public CMsgPara
{
	SHORT	m_nResultID;
	ULONG	m_unUin;
	UCHAR	m_bGroupNameLen;
	CHAR	m_szGroupName[MAXFRIENDGROUPNAMELEN];
	SHORT	m_nFriendGroupID;
	UCHAR	m_bReasonLen;
	CHAR	m_szReason[MAXATTACHCONTENTLEN]; //原因说明
};

//请求删除好友分组
class CRequestDeleteFriendGroupPara : public CMsgPara
{
	ULONG	m_unUin;
	ULONG	m_unTime;
	SHORT	m_nFriendGroupID;
};

class CResponseDeleteFriendGroupPara : public CMsgPara
{
	SHORT	m_nResultID;
	ULONG	m_unUin;
	SHORT	m_nFriendGroupID;
	UCHAR	m_bReasonLen;
	CHAR	m_szReason[MAXATTACHCONTENTLEN]; //原因说明
};

//请求移动好友至某分组
class CRequestMoveFriendToSomeGroupPara : public CMsgPara
{
	ULONG	m_unUin;
	ULONG	m_unTime;
	ULONG	m_unFriendUin;
	SHORT	m_nSrcGroupID;
	SHORT	m_nDstGroupID;
};

class CResponseMoveFriendToSomeGroupPara : public CMsgPara
{
	SHORT	m_nResultID;
	ULONG	m_unUin;
	ULONG	m_unFriendUin;
	SHORT	m_nSrcGroupID;
	SHORT	m_nDstGroupID;
	UCHAR	m_bReasonLen;
	CHAR	m_szReason[MAXATTACHCONTENTLEN]; //原因说明
};

//Gsvrd通知某玩家被其他玩家加为好友(可以:接受/拒绝)
class CNotifyBeAddedAsFriendPara:public CMsgPara
{
	ULONG	m_unSrcUin;			//原Uin　　(加好友的发起人)
	ULONG	m_unDstUin;			//目的 Uin	(被加的人)
	STPlayerInfoForFriend m_stSrcPlayerInfo;
	SHORT	m_nWordLength;				//加好友时的留言长度
	CHAR	m_szWords[MAXWORDSLEN];	//留言

};

//被加为好友的人应答Gsvrd 选择结果
class CAckBeAddedAsFriendPara:public CMsgPara
{
	SHORT	m_nResultID;				
	ULONG	m_unSrcUin;			//(被加的人)
	ULONG	m_unDstUin;			//(加好友的发起人)
	SHORT	m_nWordLength;				//留言长度
	CHAR	m_szWords[MAXWORDSLEN];	//留言

};

//Gsvrd通知发起加好友消息的玩家
class CNotifyAddFriendResultPara:public CMsgPara
{
	SHORT   m_nResultID;				
	ULONG	m_unSrcUin;			//(加好友的发起人)
	ULONG	m_unDstUin;			//(被加的人)
	SHORT	m_nWordLength;				//留言长度
	CHAR	m_szWords[MAXWORDSLEN];	//留言
};

class CRequestFindPlayerByQQPara 
{
	ULONG	m_unUin;
	ULONG	m_unTime;
	ULONG	m_unDstUin;
};

struct STFindPlayerInfo
{
	ULONG 	m_unUin;				//Uin
	ULONG 	m_unIdentity;			//玩家身份                
	USHORT 	m_nPlayerID;			//玩家ID
	STPlayerDBBaseInfo m_stPlayerDBBaseInfo;    //基础信息
	USHORT	m_nEquipedItemNum; 
	STPlayerDBItemInfo m_stPlayerItemInfo[MAXITEMNUMPERPLAYER];	//已装备的道具信息
};

//struct STFindPlayerInfo
//{
//	ULONG	m_unUin;                    //Uin
//	ULONG	m_unIdentity;              //玩家身份                
//	CHAR	m_szQQNickname[MAXNICKNAMELEN];     
//	USHORT	m_nPlayerID;               //玩家ID
//	UCHAR	m_bGender;              //性别
//	UCHAR	m_bLevel;                   //驾照等级
//	ULONG	m_unTotalExp;            //总经验
//	ULONG	m_unWinNum;                  //胜利次数
//	ULONG	m_unFirstNum;            //第一名次数
//	ULONG	m_unTotalNum;                 //比赛总数   
//	USHORT	m_nEquipedItemNum;  
//	STPlayerDBItemInfo m_stPlayerItemInfo[MAXITEMNUMPERPLAYER];     //已装备的道具信息
//};

struct STPositionInfo
{
	USHORT	m_nStatus;
	LONG	m_iServerID;           //服务器位置
	STRoomInfo m_stRoomInfo;
};


class CResponseFindPlayerByQQPara
{
	SHORT	m_nResultID;
	STFindPlayerInfo m_stPlayerInfo;
	UCHAR	m_bIsOnLine;       //是否在线，未连接0x00/在线0x01/短线0x02
	STPositionInfo m_stPositionInfo;
	UCHAR	m_bReasonLen;
	CHAR	m_szReason[MAXATTACHCONTENTLEN];
};


//反外挂
struct NOTIFY_ANTI_BOT
{
	ULONG m_dwUin;                
	SHORT m_nBufferLen;           
	CHAR m_szBuffer[32000];      
};

//房主点开始游戏
class CRequestStartGamePara
{
	ULONG	m_unUin;	//房主Uin
	ULONG	m_unTime;				
};

class CResponseStartGamePara
{
	SHORT	m_nResultID;
	UCHAR	m_bReasonLen;
	CHAR	m_szReason[MAXATTACHCONTENTLEN];
};

//.服务器分别通知房间内每个玩家游戏开始
class CNotifyGameBeginPara
{
	ULONG	m_unNormalLoadTime;	//给客户端预留的正常载入时间(10s） 
	LONG	m_nMapID;			//地图ID
	UCHAR	m_szMapHash[MAP_HASH_CODE]; //地图Hash
	ULONG	m_unGameID;		
	ULONG	m_unSeed;	
	CHAR	m_bCurrentPlayerNum1; //当前房间内的玩家数
	ULONG	m_anRaceTrackOrders[MAXSEATNUMINROOM];	//各个跑道的玩家uin
	SHORT   m_nLap;
	ULONG	m_unPropUpdateInterval;//道具刷新的时间间隔(毫秒)
};

class CNotifyGameOverPara
{
	SHORT m_nResultID;
};

//客户端通知场景载入完成
class CRequestPrepareReadyPara
{
	ULONG	m_unUin;	
	ULONG	m_unTime;	
};

//服务器通知等待未载入完成的玩家，计时开始
class CNotifyWaitUnReadyPlayersPara
{
	ULONG	m_unWaitLoadTime; //等待时间 （20s）
};

//服务器通知比赛开始
class CNotifyRaceBeginPara
{
	ULONG	m_unCountDownTime; //比赛开始前的倒计时时间（3秒）
};

//客户端通知完成比赛 （待定。。。。。）
class CRequestPlayerFinishRacePara
{
	ULONG	m_unUin;	
	ULONG	m_unTime;
	ULONG 	m_unFinUin;//完成者的uin (有可能不是谁完成谁上报) 
	ULONG 	m_unFinResult;//完成者成绩（客户端统计的完成时间，供服务器参考）
};

//服务器通知某player完成比赛
class CNotifyPlayerFinishRacePara 
{
	ULONG m_unUin;
};


//服务器通知比赛进入结束前倒计时
class CNotifyCountDownPara
{

	ULONG	m_unCountDownTime;//比赛结束前的倒计时时间（10s）
	//有可能其他玩家完成比赛的p2p消息还没到达该玩家，此时该玩家不知道谁是冠军就倒	计时了，所以服务器通知倒计时的同时顺便通知谁是冠军。
	ULONG	m_unUin;//冠军Uin
};

//比赛成绩
struct STRaceScore
{

	ULONG	m_unUin;
	ULONG	m_unFinTime; //完成比赛的时间（０表示未完成）
	LONG	m_nTP;						//仅组队竞速赛使用
	LONG	m_nMoneyInc;					//金币增量
	LONG	 m_nTotalMoney;				//金币总量（保险起见）
	LONG	m_nExpInc;					//经验增量
	LONG	m_nTotalExp;					//经验总量（保险起见）	
	LONG	m_nAttrResInc;                                     //未分配属性点增量，如果此项不为０则说明升级
	LONG	m_nTotalAttrRes;                             //总未分配属性点(保险起见)

	// 待定
	//特殊道具奖励，如"小乖奖励"：
	//没有装备特殊道具时m_bAwardNum为0，
	//因为各种道具奖励效果可能叠加，所以要把数组展开，按bit取得特殊奖励位。
	//特殊奖励种类最多为sizeof(SHORT) * 8 * MAXAWORDSIZE 
	
	CHAR		m_bAwardNum;
	LONG		m_nSpecialAward[MAXAWORDSIZE]; 
};

//服务器通知比赛结束,进入颁奖典礼
class CNotifyRaceOverPara
{
	//当前玩家数,对中途掉线的玩家不做统计，比赛结束时人数<=游戏开始时的人数 
	CHAR		m_bCurrentPlayerNum;	  
	//各玩家的比赛成绩,按index已经排好序，客户端按这个顺序显示成绩,显示颁奖台上玩		家站位，显示颁奖台下玩家升级时的站位。
	//单人赛：按数组下标0-2显示前3名
	//组队赛：按胜队玩家在数组中的索引从小到大排序，如：1,4,6,7
	STRaceScore	m_astRaceScore[MAXSEATNUMINROOM];
	CHAR		m_bWinTeamID;	//胜队ID（不一定是冠军所在队为胜队）,单人赛中不起作用
	LONG		m_iWinType;// 胜利类型,如:完胜
	ULONG		m_unReceiveHonorTime;
};

class CNotifyRaceShowPara
{
	ULONG		m_unRaceShowTime;			// 收到后 玩家显示胜利或失败的表情，此时应该停掉物理的 原来是在 RaceOver中停掉物理的
};

class CRequestReportCurrentInfoPara
{
	ULONG	m_unUin;
	ULONG	m_unTime;
	LONG	m_iCurrentStatus;  //道具加速，漂移加速，其他(预留)
	LONG	m_iCurrenRate[3];   //三维速度 取整
	LONG	m_iCurrentPosition[3]; //三维坐标  取整
	UCHAR	m_bPassedCheckPointNum;	//所经过的检查点
	USHORT	m_nPassedCheckPointIDs[MAXCHECKPOINTNUM];  //检查点的id
};


//客户端请求上报漂移结果(待定)
class CRequestReportDriftPara
{	
	ULONG	m_unUin;
	ULONG	m_unTime;
	LONG	m_aiSpeedOfDriftStart[3];	//漂移开始时的速度
	LONG	m_aiSpeedOfDriftStop[3];	//漂移结束时的速度
	LONG	m_aiPositonOfDirftStart[3];	//漂移开始的坐标
	LONG	m_aiPositionOfDriftStop[3];	//漂移结束的坐标
	
	
	//漂移标志位（预留）,如果客户端每次漂移都上报的话，
	//这个可以用来标示漂移结束的	原因，是正常结束，
	//还是碰撞到东西结束, 非正常结束是不加氮气的。
	

	LONG	m_iDriftFlag;
	LONG	m_iAccelerateFuelInc;	//本次漂移的氮气值的增量（服务器用来做对比）
};

//服务器回复漂移(待定)
class CResponseReportDriftPara
{
	SHORT	m_sResultID;
	SHORT	m_sPropID;		//加速卡的 ID
	LONG	m_iPropIndex;
	LONG	m_iCurrentAccelerateFuel;	//当前氮气值(保留)
	UCHAR	m_bReasonLen; 
	CHAR	m_szReason[MAXATTACHCONTENTLEN];
};






//获得道具（prop）
class CRequestGetPropPara
{
	ULONG m_unUin;
	ULONG m_unTime;
	SHORT m_sPropPositionNO;			 //道具位置编号
	UCHAR m_bCurrentRank;				//当前名次
	LONG m_aiCurrentPosition[3];        //玩家当前位置，三维坐标( *1000 取整)
	LONG m_iPreviousInterval;           //与前一玩家的距离
};

//获得道具（prop）
class CResponseGetPropPara
{
	SHORT m_nResultID;
	ULONG m_unUin;
	SHORT m_sPropID;			//获得道具ID
	LONG  m_iPropIndex;			//道具索引，服务器生成下发
};

//通知所有玩家获得道具
class CNotifyGetPropPara
{
	ULONG m_unUin;
	SHORT m_sPropID;              //获得道具ID
	LONG  m_iPropIndex;			//道具索引，服务器生成下发
	SHORT m_sPropPositionNO;     //道具位置编号
	
}; 

//使用道具（prop）
class CRequestUsePropPara
{
	ULONG m_unUin;
	ULONG m_unTime;
	SHORT m_sPropID;				//道具ID
	ULONG m_unDstUin;				//目标玩家Uin，
									//作用于自身的道具m_unDstUin=m_unUin
									//不指定攻击目标的道具m_unDstUin=0
	LONG m_aiCurrentPosition[3];	//玩家当前位置
	LONG m_iPropIndex;				//道具索引，服务器生成
	SHORT m_sNewID;					//道具转换成新id，0表示没转换
};
//服务器返回道具信息
class CResponseUsePropPara
{
	SHORT m_nResultID;
	ULONG m_unUin;
	SHORT m_sPropID;              //道具ID
	ULONG m_unDstUin;
	LONG  m_aiPropPosition[3];  //放置香蕉皮等的位置
	LONG  m_iPropIndex;        //道具索引，服务器生成
};

//通知除m_unUin外所有玩家,使用道具
class CNotifyUsePropPara
{
	ULONG m_unUin;
	SHORT m_sPropID;              //道具ID
	ULONG m_unDstUin;
	LONG  m_aiPropPosition[3];  //放置香蕉皮等的位置，即玩家当前位置
	LONG  m_iPropIndex;        //道具索引，服务器生成下发
	SHORT m_sNewID;				//道具转换成新id，0表示没转换
};

//道具作用结果上报
class CRequestPropEffectResultPara
{
	ULONG	m_unUin;
	ULONG	m_unTime;
	SHORT	m_sPropID;
	LONG	m_iPropIndex;				//道具索引，服务器生成
	SHORT	m_nEffectResult;			//作用结果,0表示被击中，1表示没被击中, 2表示产生了其他 avtar效果
	LONG	m_iItemID;				//如果m_nEffectResult = 2的话，该值就是起作用的avatar ID
};

//通知除m_unUin外所有玩家道具作用结果
class CNotifyPropEffectResultPara
{
	ULONG m_unUin;
	SHORT m_sPropID;
	LONG  m_iPropIndex;        //道具索引，服务器生成
};

//瞄准状态
class CRequestChangeAimStatePara 
{
	ULONG m_unUin;
	ULONG m_unTime;
	ULONG m_unDstUin;
	UCHAR m_bAimState;        //瞄准状态，0取消瞄准，1瞄准
};

//通知m_unDstUin玩家,瞄准状态
class CNotifyChangeAimStatePara 
{
	ULONG m_unUin;
	ULONG m_unDstUin;
	UCHAR m_bAimState;        //瞄准状态，0取消瞄准，1瞄准
};

//1．请求更换地图
class CRequestChangeMapPara
{
	ULONG	m_unUin;
	ULONG	m_unTime;
	LONG	m_iNewMapID; //新地图id
};

class CResponseChangeMapPara
{
	SHORT	m_sResultID; //结果
	LONG	m_iNewMapID; //新地图id
	UCHAR	m_bReasonLen;   //原因长度，失败时用
	CHAR	m_szReason[MAXATTACHCONTENTLEN];  //原因
};

class CNotifyChangeMapPara
{
	ULONG	m_unUin;  //更换地图玩家号码
	LONG	m_iNewMapID; //新地图id
}; 


//请求改变队伍
class CRequestChangeTeamPara
{
	ULONG	m_unUin;                
	ULONG	m_unTime;               
	UCHAR	m_bNewTeamID;   //要改变成的队伍ID        
};

//回复改变队伍
class CResponseChangeTeamPara
{
	SHORT	m_nResultID;   //服务器处理的结果
	UCHAR	m_bNewTeamID;  //新的队伍ID
	UCHAR	m_bReasonLen;
	CHAR	m_szReason[MAXATTACHCONTENTLEN];
};

//通知其他客户端有人改变队伍
class CNotifyPlayerChangeTeamPara
{
	ULONG	m_unUin;        //请求改变队伍的玩家Uin
	UCHAR	m_bNewTeamID;   //改变后的新的队伍ID    
};

//通知房主改变
class CNotifyChangeRoomOwnerPara
{
	SHORT	m_sRoomID;
	SHORT   m_nNewRoomOwnerID;          //新房主playerID
};

//道具替换信息
struct STReplacePropInfo
{
	LONG	m_iReplaceFlag;//替换标志位,0-不替换;1--替换
	LONG	m_iPropIndex;
	SHORT	m_sPropID;
};

//玩家道具替换信息
struct STRacerReplacePropInfo
{
	ULONG	m_unUin;
	STReplacePropInfo m_astReplaceProp[MAXPROPBLANK];
};

//通知替换道具
class CNotifyReplacePropPara 
{
	SHORT m_sPlayerNum;
	STRacerReplacePropInfo m_astRacerReplacePropInfo[MAXSEATNUMINROOM];
	LONG	m_iItemID;//如果是因为avatar 引起的道具替换,填Avatar的 ID...否则,默认0 
};

//通知添加道具(这个是由服务器主动生成下发的，不是客户端捡到的)
class CNotifyAddPropPara
{
	ULONG	m_unUin;
	SHORT	m_sPropID;
	LONG	m_iPropIdx;
	LONG	m_iItemID;//如果是因为avatar 引起的道具添加,填Avatar的 ID...否则,默认0 
};

class CNotifyReportDriftPara
{
	LONG m_iTeamAccelerateFule;//队伍氮气值
};

struct STItemStatus
{
	LONG    m_iItemID;		//道具ID              
	UCHAR 	m_bNewStatus;   //道具状态，定义见ENMITEMSTATUS
};

class CRequestChangeItemStatus
{
	ULONG	m_unUin;           //qq号码
	ULONG	m_unTime;          //时间
	SHORT m_sItemNum;                  //要改变的道具数量
	STItemStatus m_stItemStatus[MAXCHANGEITEMSTATUSNUM]; //要改变的道具
};

class CResponseChangeItemStatus :public CMsgPara
{
	SHORT 	m_nResultID;               //返回结果，0成功，其他失败，具体原因在m_szReason中
	SHORT	m_sItemNum;                //改变的道具数量，成功时使用
	STItemStatus m_stItemStatus[MAXCHANGEITEMSTATUSNUM];  //改变的道具，成功时使用
	UCHAR	m_bReasonLen;
	CHAR  m_szReason[MAXATTACHCONTENTLEN];    //原因，失败时使用
};

class CNotifyChangeItemStatusPara
{
	ULONG m_unUin;    //改变道具状态的玩家
	SHORT m_sItemNum;
	STPlayerDBItemInfo m_stItemStatus[MAXCHANGEITEMSTATUSNUM]; 
};

//---------------------------------------注册相关协议------------------------------------
//请求分配属性点数
class CRequestAssignAttributePara :public CMsgPara
{
	ULONG	m_unUin;           //qq号码
	ULONG	m_unTime;          //时间
	STPlayerDBAttributeInfo m_stPlayerDBAttributeInfo;        //分配后的点数分布，包括策略的修改
};
//回应分配点数
class CResponseAssignAttributePara :public CMsgPara
{
	SHORT 	m_nResultID;               //返回结果，0成功，其他失败，具体原因在m_szReason中
	STPlayerDBAttributeInfo m_stPlayerDBAttributeInfo;        //当前的点数分布，成功时使用
	UCHAR	m_bReasonLen;
	CHAR	m_szReason[MAXATTACHCONTENTLEN]; 
};

//请求洗点
class CRequestClearAttributePara :public CMsgPara
{
	ULONG	m_unUin;           //qq号码
	ULONG	m_unTime;          //时间
};
//回应洗点
class CResponseClearAttributePara :public CMsgPara
{
	SHORT 	m_nResultID;               //返回结果，0成功，其他失败，具体原因在m_szReason中
	STPlayerDBAttributeInfo m_stPlayerDBAttributeInfo;        //当前的点数分布，成功时使用
	UCHAR	m_bReasonLen;
	CHAR	m_szReason[MAXATTACHCONTENTLEN]; 
};

//请求检查名字有效性
class CRequestCheckNamePara :public CMsgPara
{
	ULONG	m_unUin;           //qq号码
	ULONG	m_unTime;          //时间
	CHAR  m_szNickname[MAXQQKARTNICKNAMELEN];  	//玩家的昵称
};
//返回检查结果
class CResponseCheckNamePara :public CMsgPara
{
	SHORT 	m_nResultID;               //返回结果，0成功，其他失败，具体原因在m_szReason中
	CHAR	m_szNickname[MAXQQKARTNICKNAMELEN];  	//玩家的昵称
	UCHAR	m_bReasonLen;
	CHAR	m_szReason[MAXATTACHCONTENTLEN]; 
};

//
// 买商品
//
class CRequestBuyCommodityPara :public CMsgPara
{
	ULONG m_unUin;
	ULONG m_unTime;
	LONG m_iReqBuySeq;
	LONG m_iCommodityID;
	LONG m_iPrice;
	LONG m_iPayType;
	ULONG m_unDstUin;
	SHORT m_nAttachInfoLen;
	UCHAR m_abyAttachInfo[MAXATTACHINFOLEN];
};


class CResponseBuyCommodityPara :public CMsgPara
{
	SHORT m_nResultID; 
	ULONG m_uiUin;
	LONG m_iReqBuySeq;
	LONG m_iCommodityID;
	LONG m_iPayType;
	LONG m_iPrice;
	ULONG m_uiDstUin;
	USHORT m_ushReasonLen;
	CHAR m_szReason[MAX_REASON_LEN];
};

//
// 拉商品列表
//
class CRequestGetCommodityListPara :public CMsgPara
{
	ULONG m_unUin;
	ULONG m_unTime;
};

class CResponseGetCommodityListPara :public CMsgPara
{
	SHORT m_shResultID;
	CHAR m_cEndFlag; //商品列表会超出一个数据包的长度,所以要拆成几个包来发送，此标志为0表示还未结束；1表示结束。
	SHORT m_shCommodityNum;
	STCommodity m_astCommodity[MAX_COMMODITY_NUM];
	USHORT m_ushReasonLen;
	CHAR m_szReason[MAX_REASON_LEN];
};

//GameSvrd通知client增加道具
class CNotifyClientAddItemPara :public CMsgPara
{
	ULONG	m_unUin;                               
	ULONG	m_unSrcUin; 	//0标识非赠送，若> 0 则表示为赠送玩家uin
	SHORT	m_sItemNum;
	STPlayerDBItemInfo	m_stItemInfo[MAXITEMNUMONCEADD];	
	SHORT	m_sAttachLen;
	UCHAR	m_szAttachInfo[MAXATTACHINFOLEN];
};


//dir协议///////////////////////////////////////////////////
//下载服务器配置
struct STDownloadServerInfo
{
	ULONG m_uiIPAddr;             //ip地址
	USHORT m_ushPort;            //端口号
	USHORT m_ushLocationID;       //IDC id
};
//目录服务器信息
struct  STDirServerInfo
{
	LONG m_iServerID;                    //id号
	USHORT m_ushLocationID;      //IDC id
	ULONG m_uiServerIP;           //ip地址
	USHORT m_aushServerPort[MAX_DIR_LISTEN_PORT_NUM];  //监听端口号
};
//区域配置
struct STWorldBaseInfo
{
	UCHAR m_byWorldID;         //id号
	UCHAR m_byNameLen;    //名字长度    
	CHAR m_szName[MAX_WORLD_NAME_LEN];  //名字
	UCHAR m_byDirNum;     //目录服务器个数
	STDirServerInfo m_astDirServerInfo[MAX_WORLD_DIR_SERVER_NUM]; //dir服务器
};
//服务器基本信息
struct  STServerBaseInfo
{
	LONG m_iServerID;                     //id号
	USHORT m_ushLocationID;        //IDC id
	ULONG m_uiServerIP;              //ip地址
	USHORT m_ushServerPort;         //端口号
};
//游戏服务器信息
struct  STGameServerInfo
{
	LONG m_iServerID;                     //server id号
	ULONG m_uiServerIP;             //ip地址
	USHORT m_ushTcpPort;           //tcp端口号
	USHORT m_ushUdpPort;           //udp端口号
	USHORT m_ushMaxPlayerNum;     //最大可承载人数
	USHORT m_ushCurrentPlayerNum;   //当前在线人数
};

//通用游戏服务器信息
struct  STCommGameServerInfo
{
	STGameServerInfo m_stServerInfo;
	USHORT m_ushLocationID;        //IDC id
};

//idc信息
struct STIDCServerInfo
{
	USHORT m_ushLocationID;  //IDC id
	UCHAR m_byGameServerNum;   //游戏服务器个数
	STGameServerInfo m_astGameServerInfo[MAX_IDC_GANE_SERVER_NUM];  //游戏信息
};
//频道信息
struct STChannelInfo
{
	UCHAR m_byChannelID;  //频道id号
	UCHAR m_byIDCNum;   //IDC个数
	STIDCServerInfo m_astIDCServerInfo[MAX_IDC_SERVER_NUM];  //idc信息
};
//国家信息
struct STCountryInfo
{
	UCHAR m_byCountryID; //国家id号
	UCHAR m_byChannelNum;   //频道个数
	STChannelInfo m_astChannelInfo[MAX_COUNTRY_CHANNEL_NUM];  //频道信息
};

//Client 拉取world信息
class CRequestGetWorldInfoPara
{
	ULONG m_uiUin;                
	ULONG m_uiTime;               
	ULONG m_uiClientVersion;       //客户端版本号，用于控制升级
	ULONG m_uiMasterVersion;      //主版本号，用于前台下载
	ULONG m_uiSlaveVersion;       //次版本号，用于后台下载
	UCHAR m_byWorldID;        //所要拉取的区域
	UCHAR m_szFileHash[HASH_CODE_LENGTH]; //完整性校验码
};

//返回world信息
class CResponseGetWorldInfoPara
{
	USHORT m_ushResultID;         //结果ENMDirLoginResult
	ULONG m_uiLastClientVersion;   //最新客户端版本号
	ULONG m_uiLastMasterVersion; //最新主版本号
	ULONG m_uiLastSlaveVersion;  //最新次版本号
	UCHAR m_byDownloadServerNum;     
	STDownloadServerInfo m_astDownloadServerInfo[MAX_DOWNSVR_NUMBER];     //下载服务器配置
	UCHAR m_byRootDirectionLen;      
	CHAR m_szRootDirection[MAX_DIRECTION_LENGTH]; //下载根路径
	ULONG	m_uiMasterFullVersion;	//下载服务器最新的前台更新完整包版本号
	ULONG	m_uiSlaveFullVersion;	//下载服务器最新的后台更新完整包版本号
	//UCHAR m_byMasterFileNameLen;
	//CHAR  m_szMasterFileName[MAX_FILE_NAME_LENGTH]; //主版本下载文件名
	//UCHAR m_bySlaveFileNameLen;
	//CHAR  m_szSlaveFileName[MAX_FILE_NAME_LENGTH];  //次版本下载文件名
	USHORT m_ushP2POption;           //p2p配置，0表示下载完成以后，需要关闭p2p下载服务器。此项可能扩展
	UCHAR m_byWorldCount;
	STWorldBaseInfo m_astWorldBaseInfo[MAX_WORLD_NUM]; //全部world信息
	UCHAR m_byCountryCount;         
	STCountryInfo m_astCountryInfo[MAX_COUNTRY_NUM];    //国家信息
	UCHAR m_byShopServerCount;         
	STServerBaseInfo m_astShopServerInfo[MAX_SHOP_SERVER_NUM]; //商店服务器信息
	UCHAR m_byBorderServerCount;         
	STCommGameServerInfo m_astBorderServerInfo[MAX_BORDER_SERVER_NUM];          //边境服务器信息
	UCHAR m_byMatchServerCount;         
	STCommGameServerInfo m_astMatchServerInfo[MAX_MATCH_SERVER_NUM];          //比赛服务器信息
	UCHAR m_bNetAdminServerCount;         
	STServerBaseInfo m_astNetAdminServerInfo[MAX_NETADMIN_SERVER_NUM];          //网管服务器信息
	USHORT m_ushReasonLen;
	CHAR m_szReason[MAX_REASON_LEN];      //保留
};


//任务状态/进度信息
struct STTaskStatusInfo
{
	LONG m_iTaskID;
	CHAR m_cStatus;			//任务状态：0未完成，1完成 
	LONG m_iProgressRate;    //当前完成或收集的数量，用于标识任务进度
	LONG m_iOther;			//备用，用于标识任务进度
};

//任务奖励信息
struct STTaskAward
{
	LONG m_iAwardMoney;			//金币奖励
	LONG m_iTotalMoney;			//金币总数
	LONG m_iAwardExp;           //经验奖励
	LONG m_iTotalExp;           //经验总数
	LONG m_iAwardItemNum;
	STPlayerDBItemInfo m_astAwardItemInfo[MAXAWARDITEMNUM];   //道具奖励
};

//任务前置任务状态
struct STTaskPrevStatusInfo
{
	LONG m_iTaskID;
	UCHAR m_bPrevStatus;    //前置任务状态
};


class CNotifySingleGameBeginPara
{
	ULONG m_uiNormalLoadTime;			//给客户端预留的正常载入时间(10s）
	LONG m_iTaskID;						// 任务id
	LONG m_iMapID;						//地图ID
	UCHAR m_szMapHash[MAP_HASH_CODE];   //地图Hash
	ULONG m_uiGameID;		
	ULONG m_uiSeed;
	SHORT m_shTotalMapRound;			//圈数
	ULONG m_uiTimeLimit;				//时间限制
};

class CNotifySingleRaceBeginPara
{
	ULONG m_uiCountDownTime; //比赛开始前的倒计时时间（3秒）
};

class CNotifySingleRaceFinishPara
{
	USHORT m_shResultID;      //0表示完成, 其他表示超时
};

class CNotifySingleRaceOverPara
{
	ULONG m_uiUin;
	STTaskStatusInfo m_stTaskStatusInfo;
	ULONG m_uiFinTime;					//使用时间
	STTaskAward m_stTaskAward;
};

class CNotifySingleGameOverPara
{
	SHORT m_shResultID;   //结果
};

class CRequestStartSingleTaskPara
{
	ULONG m_uiUin;
	ULONG m_uiTime;      //时间
	LONG m_iTaskID;      //任务ID
	LONG m_iMapID;       //地图ID
};

class CResponseStartSingleTaskPara
{
	SHORT m_shResultID;
	ULONG m_uiUin;
	LONG m_iTaskID;       //任务ID
	LONG m_iMapID;
	UCHAR m_bReasonLen;
	CHAR m_szReason[MAXATTACHCONTENTLEN];
};

class CRequestReportCurrentOptPara
{
	ULONG m_uiUin;
	ULONG m_uiTime;      //时间
	LONG m_iOptID;       //任务ID
};

class CRequestGetTaskListPara
{
	ULONG m_uiUin;
	ULONG m_uiTime;     //时间
};

class CResponseGetTaskListPara
{
	SHORT m_shResultID;
	ULONG m_uiUin;
	SHORT m_shTaskNum;
	STPlayerDBTaskInfo m_astPlayerDBTaskInfo[MAXTASKNUMPERPLAYER]; //玩家任务列表
	UCHAR m_bReasonLen;
	CHAR m_szReason[MAXATTACHCONTENTLEN];
};


class CRequestTakeTaskPara
{
	ULONG m_uiUin;
	ULONG m_uiTime;			//时间
	LONG m_iTaskID;         //任务ID
	SHORT m_shTakeOptionID; //主动接受任务的选项ID
};

class CResponseTakeTaskPara
{
	SHORT m_shResultID;
	ULONG m_uiUin;
	STPlayerDBTaskInfo m_stPlayerDBTaskInfo;
	UCHAR m_bReasonLen;
	CHAR m_szReason[MAXATTACHCONTENTLEN];
};

class CRequestDeleteTaskPara
{
	ULONG m_uiUin;
	ULONG m_uiTime;          //时间
	LONG m_iTaskID;          //任务ID
};

class CResponseDeleteTaskPara
{
	SHORT m_shResultID;
	ULONG m_uiUin;
	LONG m_iTaskID;           //任务ID 
	UCHAR m_bReasonLen;
	CHAR m_szReason[MAXATTACHCONTENTLEN];
};

class CRequestSkipTaskPara
{
	ULONG m_uiUin;
	ULONG m_uiTime;          //时间
	LONG m_iTaskID;          //任务ID
};

class CResponseSkipTaskPara
{
	SHORT m_shResultID;
	ULONG m_uiUin;
	LONG m_iTaskID;                  //任务ID 
	STTaskAward m_stTaskAward;
	UCHAR m_bReasonLen;
	CHAR m_szReason[MAXATTACHCONTENTLEN];
};

class CNotifyClientUpdateMoneyPara 
{
	ULONG m_uiUin;
	LONG m_iCurrentMoney;   //更新后的酷币数量
};

class CNotifyTaskAwardPara
{
	ULONG m_uiUin;
	LONG m_iTaskID;
	STTaskAward m_stTaskAward;
};