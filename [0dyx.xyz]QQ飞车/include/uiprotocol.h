
#ifndef __UIPROTOCOL_H__
#define __UIPROTOCOL_H__

#include "gameui.h"
#include "ov_cproto.h"


//游戏发送给界面库的消息
//wparam :更新类型,lparam:附加参数
//wparam==0: player属性值变化
//wparam==1: 物品栏保管箱的钱，负重等数目变化 
//wparam==2: 商店买卖选定栏的数目变化(lparam=选定栏数目)
//wparam==3: 玩家交易，对方输入钱数的变化(lparam=钱数)
//wparam==4: 玩家交易，自己输入钱数的变化(lparam=钱数)
//wparam==5: 地图名字变化
#define GM_UPDATEVALUE    (WM_USER+100) 

#define GM_POPMSGBOX      (WM_USER+101) //wparam = pinfo,  lparam = msgbox_id
#define GM_UPDATEITEMINFO (WM_USER+102) //物品详细信息更新

//以下的打开游戏对话框命令，如果wparam == 0xffffffff(-1),  都是关闭此窗口
//否则用各自的参数打开
#define GM_OPENNPCDLG     (WM_USER+202) //打开和NPC的对话框   lparam = stDialogInfo *

#define GM_OPENSHOP       (WM_USER+203) //打开和npc或者player的买卖界面（商店）, wparam = 0(激活买)，1(激活卖), 2(只能买)，3(只能卖) lparam=多少个物品

#define GM_DEALREQUEST    (WM_USER+204) //交易请求来了，弹出是否同意的对话框, lparam = 请求发起人的名字

#define GM_OPENDEAL       (WM_USER+205) //开始交易，弹出交易的界面, lparam = 交易对方的名字（自己名字可通过GetSelfName得到）

#define GM_OPENNUMINPUT   (WM_USER+206) //打开数值输入界面, wparam = （低16位-最大数目， 高16位-缺省值）, lparam = 名字字符串

#define GM_OPENSTORE      (WM_USER+207) //打开保管仓库, wparam = 0(小箱子), 1(大箱子)

#define GM_OPENPLAYERMENU (WM_USER+208) //右键点击其他玩家，弹出下一步操作界面, 
                                        //wparam = 玩家名字(char *), lparam = 菜单出现位置(x=LOWWORD, y=HIWORD)

#define GM_DEALUPDATE     (WM_USER+209) //更新交易状态， wparam = 这些数值的位操作，自己同意1, 对方同意2, 自己成交4，对方成交8

#define GM_TEAMJOIN       (WM_USER+210) //加入队伍的请求, wparam = 请求人名字， lparam = 请求信息
#define GM_TEAMINVITE     (WM_USER+211) //邀请加入队伍，  wparam = 邀请队长的名字， lparam = 邀请信息
#define GM_TEAMUPDATE     (WM_USER+212) //队伍成员变化, wparam = (0,1)是否需要打开组队界面, lparam=(0,1)队员数目是否更新

#define GM_OPENSKILLWIN   (WM_USER+213) //打开技能界面, wparam=0:普通， 1:遗忘界面， (-1):关闭
#define GM_SKILLUPDATE    (WM_USER+214) //技能的变化，界面响应此消息刷新技能界面

#define GM_OPENEQUIPWIN   (WM_USER+215) //打开或者关闭(wparam==-1) 装备窗口
#define GM_OPENWAREWIN    (WM_USER+216) //打开或者关闭(wparam==-1) 背包窗口, wparam=0:消耗品， 1:收集品， 2:武器装备

#define GM_TASKUPDATE     (WM_USER+217) //任务更新, wparam = 第几个任务列表(1,2,3)

#define GM_OPENITEMMOD    (WM_USER+218) //弹出物品改变界面, wparam=0:合成， 1:精练， 2:镶嵌, 3:查询合成，4:查询精炼, lparam=listnum
#define GM_UPDATEITEMMOD  (WM_USER+219) //选择某一个要合成精炼的物品, wparam=材料数目,  lparam=价格

#define GM_MASTER_REQUEST (WM_USER+220) //拜师收徒请求   wparam=0:拜师, 1:收徒,  lparam = 请求人（将来的徒弟）名字
#define GM_MASTER_ACCEPT  (WM_USER+221) //对方接收拜师收徒请求,  wparam = bit0-0拜师:1收徒, bit1-0拒绝:1接收, lparam = 对方名字

#define GM_GOSSIPMSGUPDATE (WM_USER+250) //是否有私聊消息:  wparam = 多少条消息， 0表示没有消息
#define GM_GOSSIPTALK     (WM_USER+251) //私聊信息，自己说话,  wparam=好友名字,  lparam=信息
#define GM_GOSSIPREPLY    (WM_USER+252) //私聊信息，对方回答,  wparam=好友名字,  lparam=信息
#define GM_GMBUDDYUPDATE  (WM_USER+253) //游戏好友变动，上下线等等：wparam = 好友个数, lparam=bit0:是否隐身， bit1:是否显示等级
#define GM_GMBUDDYJOIN    (WM_USER+254) //有人请求加入游戏好友,  wparam = 请求人名字
#define GM_BEGINWHISPER   (WM_USER+255) //开始和某人密聊,   wparam = 对方名字
//{add by novaliu  qq message popup
#define GM_QQMSG		  GM_GOSSIPMSGUPDATE //(WM_USER+256) //QQ消息，属于GM_GOSSIPMSGUPDATE 
#define GM_QQBUDDYJOIN	  (WM_USER+257) //加QQ好友
#define GM_GOSSIPSYS      (WM_USER+258) //私聊系统消息
//}

#define GM_OPENTRANSPORT  (WM_USER+260) //打开传送列表 wparam = 传送点数目， (-1)表示关闭列表

#define GM_OPENMAILLIST   (WM_USER+261) //打开邮件列表界面 wparam = 邮件数目
#define GM_OPENMAIL       (WM_USER+262) //打开邮件详细内容的界面 wparam = 第几封邮件(0, 1, 2...)
#define GM_WRITEMAIL      (WM_USER+263) //打开写邮件的界面, wparam = 邮件类型(0, 1, 2, 3), lparam = char *[],  [0]发送对象(可能为null), [1]发送主题

//add by lincoln [2005:12:10] [14:02]
//{{{去掉旧consign
/*
#define GM_OPENCONSIGNCONFIG (WM_USER+265) //打开寄售物品的输入价格窗口
#define GM_OPENCONSIGN    (WM_USER+266) //打开寄售窗口, wparam=0: 寄售, 1:购买, 2:状态, lparam=0, 1, 2,3: 切换到第几个窗口
#define GM_UPDATECONSIGN  (WM_USER+267) //更新寄售状态窗口, wparam = 0:道具,1:武器,2:防具,3:自己物品,  lparam=物品个数
#define GM_UPDATECONSIGNLIST (WM_USER+268) //更新等待确认的寄售或者购买物品列表, wparam = 物品个数
*/
//}}}

//add by lincoln [2005:12:10] [14:36]
//{{{点卡寄售
#define GM_OPENCONSIGN    (WM_USER+266) //打开点卡寄售窗口, wparam=0: 寄售实体卡, 1:寄售绑定卡, 2:购买（点卡交易平台）
//}}}

#define GM_OPENPLAYERSHOP (WM_USER+280)  //打开玩家自己的商店, wparam==0:出售物品,wparam=1:购买物品,wparam=2:(lparam=0:我的出售商店,lparam=1:我的收购商店)
#define GM_OPENSHOPITEM   (WM_USER+281)  //打开商品细节的窗口, wparam = id|(最大数目<<16),  lparam = 物品名字(char *)
#define GM_UPDATEPLAYERSHOP (WM_USER+282) //更新玩家自己的商店，wparam=0:准备卖物品, 1:准备收购物品, 2:本人商店物品列表(需要消除按钮) lparam=物品个数

#define GM_NEWBIETIPS     (WM_USER+283)  //新手提示, 1:有消息， 0:无消息
#define GM_SYSMSGUPDATE   (WM_USER+284)  //系统提示, 多少条消息， 0:无消息
#define GM_MAILMSGUPDATE  (WM_USER+285)  //邮件提示，1:新邮件到来, 0:无新邮件

#define GM_TARGETSTATE    (WM_USER+290)  //选择目标变化, 可能是npc, 怪物等，wparam = pselect_actor
#define GM_CHANGEFIGURE   (WM_USER+291)  //改变发型发色, wparam = 0:发色， 1:发型,  lparam = 选项数目
#define GM_OPENSKILLLEARN (WM_USER+292)  //打开技能学习界面, wparam=职业(0:生活技能)
#define GM_SKILLLEARNUPDATE (WM_USER+293) //技能学习界面的更新

#define GM_OPENFAMILYLIST (WM_USER+300)     //打开家族列表界面
#define GM_FAMILYJOIN     (WM_USER+301)     //通知 某人申请加入本家族
#define GM_FAMILYINVITE   (WM_USER+302)     //通知 某家族邀请当前玩家
#define GM_FAMILYALLY   (WM_USER+303)       //通知 有其他家族邀请结盟
#define GM_FAMILYDISHOSTILE (WM_USER+304)   //通知 有其他家族申请取消敌对
#define GM_OPENCREATEFAMILY (WM_USER+305)   //通知 打开创建家族面板
#define GM_UPDATEFAMILYDETAILINFO (WM_USER+306)   //通知 本家族的信息需要更新
#define GM_OPENFMMANAGE (WM_USER+307) // 通知 打开关闭家族管理面板
#define GM_UPDATEMASTERPRENTICE (WM_USER+308) // 通知打开师徒列表,师傅登记列表等

#define GM_FAMSTASHAUTH (WM_USER+309) // 通知 申请使用家族仓库的授权
#define GM_OPENFAMSTASH (WM_USER+310) //wparam=0: 输入密码 (输入验证身份密码)
                                      //wparam=1: 首次设置密码
                                      //wparam=2: 修改密码
                                      //wparam=3: 打开购买列表,num
                                      //wparam=4: 输入密码 (同意授权)


#define GM_OPENAUTHWIN  (WM_USER+315) // 打开图片验证窗口,  wparam=-1(关闭)
#define GM_OPENREPAIRITEM (WM_USER+316) // 打开修理窗口, wparam=-1(关闭)

#define GM_OPENINPUTMONEY (WM_USER+317) // 打开输入钱的对话框

#define GM_OPENEQUIPOTHER (WM_USER+318) // 打开其查看它玩家的装备窗口

#define GM_UPDATETASKLIST (WM_USER+319) // 更新任务称号列表

#define GM_OPENWORKTIP (WM_USER+320)    // 打开打工选择菜单
                                        // wparam = 1 ; 表示显示  lparam = 1 ; 表示钓鱼 lparam = 2 ; 表示挖矿
                                        // wparam = 0 ; 表示关闭

#define GM_ENTERMAP   (WM_USER+400)     //进入新的地图 wparam=0:第一次进入地图， 1:切换地图

//add by lincoln [2005:12:6] [19:57]
//{{{公告界面
#define GM_OPENADVERTISE   (WM_USER+410) //打开公告界面
//}}}


//add by lincoln [2005:12:12] [10:41]
//{{{财产安全锁
#define GM_OPENSECULOCK   (WM_USER+411) //打开财产安全锁界面
//}}}


//add by lincoln [2005:12:12] [10:41]
//{{{充值提示
#define GM_OPENOWEOPT   (WM_USER+412) //打开充值提示界面
//}}}

//end 打开游戏对话框命令

#define GM_TEAMCREATE	(WM_USER+480)
#define GM_MEMBERUPDATE (WM_USER+481)
#define GM_REGIST		(WM_USER+482)
#define GM_TEAMDESTORY  (WM_USER+483)
#define GM_TEAMLEAVE	(WM_USER+484)
#define GM_TEAMREADY	(WM_USER+485)
#define GM_PLAYERUPDATE  (WM_USER+486)
#define GM_TEAMKICK		(WM_USER+487)
#define GM_SHOWBROKER	(WM_USER+488)

//msgbox包含(提示信息,提示图标,关闭按钮,确定按钮,取消按钮)
//其中,确定(确定,接受,成交...)
//其中,取消(取消,拒绝,放弃...)
//UI根据下面的枚举变量,在脚本中确定msgbox的显示风格.
enum UIMsgId
{
	//下面是UI自己使用的消息id
	//UI_MSGID_DEFAULT=0;在gameui.h中已经定义
	//因此，client定义的msgid必须从1开始

	UI_MSGID_YESNO			= 1,//带一个YES按钮，一个NO按钮(是，否)
	UI_MSGID_OKCANCEL		= 2,//带一个OK按钮，一个CANCEL按钮(确定，取消)
	UI_MSGID_AGREERESIST	= 3,//带一个AGREE按钮，一个RESIST按钮（同意，拒绝）

	UI_MSGID_AFFIRM			= 4,//带一个AFFIRM按钮(确定)
	UI_MSGID_CLOSE			= 5,//带一个CLOSE按钮(关闭)
	UI_MSGID_MINICLOSE		= 6,//带一个右上角小CLOSE按钮(关闭)
	UI_MSGID_SENTENCE		= 7,//带一行文本
	
	//100下面是Client使用的消息id
	UI_MSGID_LOGIN			= 101,//登陆失败(提示信息,提示图标,关闭按钮)
	UI_MSGID_NETERROR       = 102,//游戏中网络连接错误
	
	UI_MSGID_SKILLREVIVE    = 103,//角色死亡, 使用技能复活
	UI_MSGID_ITEMREVIVE     = 104,//角色死亡, 使用道具复活
	UI_MSGID_REVIVE         = 105,//角色死亡, 复活

	UI_MSGID_ENDWORK        = 106,//是否要结束打工

	UI_MSGID_QUIT			= 107,//是否退出

	UI_MSGID_CONN           = 108,//正在连接, 是否取消(只有一个cancel按钮，省略号的点数从小到大循环播放)
    UI_MSGID_CONN_NOBTN     = 109,//正在进入中...(无按钮，省略号的点数从小到大循环播放)
    UI_MSGID_CANCEL         = 110,//只有一个cancel按钮
	
	UI_MSGID_BIND		    = 111,//装备绑定
	UI_MSGID_DROPLOST		= 112,//丢弃消失提示

	UI_MSGID_MONEYALERT		= 113,//邮件金钱超限提示
	
	//{{{ add by novaliu, 20060224
	UI_MSGID_UPDATE         = 114, //询问玩家是否需要升级
	//}}}

	//add by lincoln [2006:2:27] [15:41]
	//{{{采集操作
	UI_MSGID_COLLECT		= 115,
	//}}}
};


enum UIMsgSubId
{
    UI_MSGSUBID_DEFAULT     = 0,    // 默认
    UI_MSGSUBID_CONNPSW     = 1,    // 正在连接密码服务器
    UI_MSGSUBID_CHECKPSW    = 2,    // 正在验证帐号密码
    UI_MSGSUBID_CONNDIR     = 3,    // 正在连接目录服务器
    UI_MSGSUBID_READDIR     = 4,    // 正在读取服务器目录
    UI_MSGSUBID_CONNZONE    = 5,    // 正在连接游戏服务器
    UI_MSGSUBID_LOGINZONE   = 6,    // 正在登陆游戏服务器
    UI_MSGSUBID_READROLES   = 7,    // 正在读取角色列表
    UI_MSGSUBID_ENTERWORLD  = 8,    // 正在进入游戏事件(无取消按钮)
    UI_MSGSUBID_CREATEROLE  = 9,    // 正在创建角色
    UI_MSGSUBID_CONNPSWERR  = 10,   // 连接密码服务器错误
    UI_MSGSUBID_PSWERR      = 11,   // 用户密码错误
    UI_MSGSUBID_CONNDIRERR  = 12,   // 连接dir服务器错误
    UI_MSGSUBID_READDIRERR  = 13,   // 读取dir服务器错误
};

enum UICustomState
{	
	
	UI_CUSTOM_STATE_DEFAULT		= 0,
	UI_CUSTOM_STATE_LARGE		= 1,
		
};

enum UICustomStyle
{	
	
	UI_CUSTOM_STYLE_DEFAULT		= 0,
		
};

enum UISpriteState
{	
	
	UI_SPRITE_STATE_NORMAL		= 0,//绘制静止人物
	UI_SPRITE_STATE_IDLE		= 1,//绘制静止人物，随机给出小动作
	UI_SPRITE_STATE_ACTIVE		= 2,//绘制奔跑
	UI_SPRITE_STATE_DISABLE		= 3,//绘制变灰的静止人物
	UI_SPRITE_STATE_INVISIBLE	= 4,//不绘制
		
};


enum UISpriteDirect
{	
	UI_SPRITE_DIRECT_FRONT		= 0,
	UI_SPRITE_DIRECT_BACK		= 1,
	UI_SPRITE_DIRECT_RIGHT		= 2,
	UI_SPRITE_DIRECT_LEFT		= 3,
	
};

	
enum UIItemType
{

	//UI_ITEM_NONE = -1,//defined in gameui.h

	UI_ITEM_ITEM = 0,
	UI_ITEM_WEAPON,
	UI_ITEM_ARMOR,
	UI_ITEM_MEDICINE,
	UI_ITEM_WARE,	
	UI_ITEM_SPECIAL,

};


//供客户端查询的全局变量的标志
enum UIGlobalQuery
{
	GlobalQuery_None				=0,
	GlobalQuery_MailListOpen		=1,
	
	//add by lincoln [2005:12:10] [14:02]
	//{{{去掉旧consign
	//GlobalQuery_ConsignListOpen     =2,
	//}}}

	GlobalQuery_ItemModOpen         =3,
	GlobalQuery_WriteMail           =4,
	GlobalQuery_PlayerDeal          =5,
	GlobalQuery_Msgbox				=6,
	GlobalQuery_PlayerShop          =7,
	GlobalQuery_WriteSmallPack      =8,
	GlobalQuery_WriteBigPack        =9,
	GlobalQuery_Probe				=10,
    GlobalQuery_RegMPList		    =11,
	GlobalQuery_FAMILYList		    =12,
	GlobalQuery_AccelBar		    =13,
	GlobalQuery_SketchMap		    =14,
	GlobalQuery_Gossip			    =15,
	GlobalQuery_OtherEquip		    =16,

	GlobalQuery_End
};

enum UIQueryType
{

	//(gameui.h)
	//#define UI_QUERY_TYPE_NONE	-1
	UI_QUERY_TYPE_AUTH	=0,
	UI_QUERY_TYPE_VOICE	=1,

};

enum UIDragType
{	
	
	//(gameui.h)
	#define UI_DRAG_TYPE_NONE	-1
    UI_DRAG_TYPE_ITEM = 0,
	UI_DRAG_TYPE_SHOP = 1,
	UI_DRAG_TYPE_DRESSROOM_ITEM = 2,
	UI_DRAG_TYPE_DRESSROOM_TEMPLATE = 3,
	UI_DRAG_TYPE_GAMELOGIC = 4,
/*	UI_DRAG_TYPE_SHOP			= 0,//"快捷"
	UI_DRAG_TYPE_WARE_MEDICINE	= 1,//"物品栏中"药品"表单
	UI_DRAG_TYPE_WARE_WARE		= 2,//"物品栏中"物品"表单
	UI_DRAG_TYPE_WARE_EQUIP		= 3,//"物品栏中"装备"表单
	UI_DRAG_TYPE_WARE_SPECIAL   = 4,//"物品栏中"特殊"表单
	UI_DRAG_TYPE_EQUIPED		= 5,//"已经装备好的装备---套装开始"

	UI_DRAG_TYPE_EQUIPOTHER		= 6,//"其他玩家的装备"

	UI_DRAG_TYPE_STORE			= 7,//"道具仓库"

	UI_DRAG_TYPE_BUYLIST       = 8,//和npc（商店）---买进物品列表
	UI_DRAG_TYPE_SELBUYLIST    = 9,//和npc（商店）---买进的选择的物品列表
//	UI_DRAG_TYPE_SELLLIST       = 10, //和npc（商店）---卖出物品列表
//	UI_DRAG_TYPE_SELSELLLIST    = 11, //和npc（商店）---卖出的选择的物品列表
	UI_DRAG_TYPE_DEAL_MINE      = 12,//和玩家交易的自己这方的物品列表
	UI_DRAG_TYPE_DEAL_OTHER     = 13,//和玩家交易的别人提供的物品列表
	UI_DRAG_TYPE_SYN_ITEMS      = 14,//物品合成的原料物品
	UI_DRAG_TYPE_SYN_LIST       = 15,//物品可以合成出来的列表
	UI_DRAG_TYPE_ITEMHOLE       = 16,//物品镶嵌
	//UI_DRAG_TYPE_MAILITEM       = 17, //邮件附件里面的物品列表
	
	//add by lincoln [2005:12:10] [14:02]
	//{{{去掉旧consign
	/*
	UI_DRAG_TYPE_CONSIGN_ITEM   = 18, //寄售物品列表--道具
	UI_DRAG_TYPE_CONSIGN_WEAPON = 19, //寄售物品列表--武器
	UI_DRAG_TYPE_CONSIGN_ARMOR  = 20, //寄售物品列表--防具
	UI_DRAG_TYPE_CONSIGN_MINE   = 21, //寄售物品列表--本人寄售的所有物品
	UI_DRAG_TYPE_CONSIGN_LIST   = 22, //寄售物品选择后等待确定的list

	//}}}

	//add by lincoln [2005:12:11] [11:50]
	//{{{点卡寄售
	UI_DRAG_TYPE_CONSIGN_ACCCARD = 18, //绑定卡寄售
	UI_DRAG_TYPE_CONSIGN_LIST	 = 19, //点卡交易平台
	//}}}
	

	UI_DRAG_TYPE_SHOP           = 23, //玩家商店物品列表
	UI_DRAG_TYPE_PETS           = 24, //玩家宠物列表
	UI_DRAG_TYPE_SMALLPACK_RECV = 25, //收到小包裹的物品列表
	UI_DRAG_TYPE_SMALLPACK_WRITE = 26,//发送小包裹的物品列表 
	UI_DRAG_TYPE_BIGPACK_RECV = 27,   //收到大包裹的物品列表
	UI_DRAG_TYPE_BIGPACK_WRITE = 28,  //发送大包裹的物品列表

	//add by jension [2006:01:06] [14:13]
	//{{{宠物唤醒
	UI_DRAG_TYPE_PETS_WAKE		= 29,
	//}}}
	
	UI_MAX_PACKTYPE,

	UI_DRAG_TYPE_SKILL_PET      = 99,   //宠物技能
	UI_DRAG_TYPE_SKILL_MAIN     = 100, //职业技能
	UI_DRAG_TYPE_SKILL_OTHER    = 101, //附属技能
	UI_DRAG_TYPE_SKILL_LIFE     = 102, //生活技能
	UI_DRAG_TYPE_OPENTASK       = 103, //未完成任务
	UI_DRAG_TYPE_WORLDTASK      = 104, //完成的世界任务
	UI_DRAG_TYPE_STORYTASK      = 105, //完成的故事任务
	UI_DRAG_TYPE_TASKHINT       = 106, //任务的hint列表
	UI_DRAG_TYPE_GMBUDDY        = 107, //游戏好友
	UI_DRAG_TYPE_QQBUDDY        = 108, //qq好友
	UI_DRAG_TYPE_TRANSPORT      = 109, //传送点列表
	UI_DRAG_TYPE_MAILLIST       = 110, //邮件列表
	UI_DRAG_TYPE_IDOBJ          = 111, //以id来区分的物品
	UI_DRAG_TYPE_DLGNPCPIC      = 112, //对话框npc半身像
	UI_DRAG_TYPE_SCREEN			= 113, //屏幕
	UI_DRAG_TYPE_TEAM           = 114, //组队队员
	UI_DRAG_TYPE_MAILREJECT     = 115, //邮件拒收列表
	UI_DRAG_TYPE_HELP           = 116, //游戏帮助树状信息
	UI_DRAG_TYPE_STORE2			= 117,//"道具仓库第二页"
	UI_DRAG_TYPE_FIGURE			= 118, //发色选择列表
	UI_DRAG_TYPE_SKILLLEARN_MAIN  = 119, //技能学习-职业技能
	UI_DRAG_TYPE_SKILLLEARN_OTHER = 120, //技能学习-附属技能
	UI_DRAG_TYPE_SKILLLEARN_LIFE  = 121, //技能学习-生活技能
	UI_DRAG_TYPE_SKETCHMAP        = 122, //小地图

	UI_DRAG_TYPE_CURPETSKILL      = 124, //当前宠物技能

    UI_DRAG_TYPE_EMBLEMFAMILYLIST = 125, //家族徽章 列表 ID

	UI_DRAG_TYPE_QQGROUP          = 126,  //qq群

	//add+lincoln
	//[2005:7:5] [10:52]
	UI_DRAG_TYPE_GARAGE           = 127,  //仓库列表
	UI_DRAG_TYPE_MASTERREG        = 128,  //师父登记列表
	UI_DRAG_TYPE_MASTERLIST       = 129,  //师父同门列表
	UI_DRAG_TYPE_PRENTICELIST     = 130,  //徒弟列表

    UI_DRAG_TYPE_STORE3			  = 132,//"道具仓库第三页"
    UI_DRAG_TYPE_STORE4			  = 133,//"道具仓库第四页"
    UI_DRAG_TYPE_STORE5			  = 134,//"道具仓库第五页"

    UI_DRAG_TYPE_FAMMEMLIST       = 135,//家族成员列表（在交友面板上的）
	
	UI_DRAG_TYPE_FILTERLIST       = 136,//屏蔽玩家列表（在交友面板上的）
	*/
};


//add+lincoln
//[2004:11:9] [21:30]
enum
{

	SCRIPT_FUNC_ON_UPDATEVALUE		=SYSTEM_SCIRPT_NUM,	
	SCRIPT_FUNC_ON_SHOWDIALOG		,
	SCRIPT_FUNC_ON_TEAMUPDATE		,
	SCRIPT_FUNC_ON_MEMBERUPDATE		,
	SCRIPT_FUNC_ON_SHOWSELFICON		,


	SCRIPT_FUNC_ON_TALKINFO			,
	SCRIPT_FUNC_UPDATEITEMINFO		,
	SCRIPT_FUNC_GLOBALQUERY			,

	SCRIPT_FUNC_ON_OPENSHOP			,
	SCRIPT_FUNC_ON_OPENNUMINPUT		,
	SCRIPT_FUNC_ON_OPENPLAYERMENU	,
	SCRIPT_FUNC_ON_OPENSTORE		,

	SCRIPT_FUNC_ON_OPENDEAL			,
	SCRIPT_FUNC_ON_DEALUPDATE		,




	SCRIPT_FUNC_ON_OPENSKILLWIN		,
	SCRIPT_FUNC_ON_SKILLUPDATE		,

	SCRIPT_FUNC_ON_OPENWAREWIN		,
	SCRIPT_FUNC_ON_OPENEQUIPWIN		,

	SCRIPT_FUNC_ON_TASKUPDATE		,
	
    SCRIPT_FUNC_OPENITEMMOD			,   //(int 类型,   int 列表数目)
	SCRIPT_FUNC_UPDATEITEMMOD       ,   //(int 类型，  int 材料孔洞数目，  int 价格)
	SCRIPT_FUNC_OWEUPDATE			,

	SCRIPT_FUNC_GOSSIPMSGUPDATE		,
	SCRIPT_FUNC_GOSSIPREPLY         ,
	SCRIPT_FUNC_GMBUDDYUPDATE		,
	SCRIPT_FUNC_BEGINWHISPER        ,

	SCRIPT_FUNC_OPENTRANSPORT		,

	SCRIPT_FUNC_OPENMAILLIST		,
	SCRIPT_FUNC_OPENMAIL		    ,
	SCRIPT_FUNC_WRITEMAIL		    ,

	//add by lincoln [2005:12:10] [14:02]
	//{{{去掉旧consign
	/*
	SCRIPT_FUNC_OPENCONSIGNCONFIG   ,
	SCRIPT_FUNC_OPENCONSIGN			,
	SCRIPT_FUNC_UPDATECONSIGN		,
	SCRIPT_FUNC_UPDATECONSIGNLIST   ,
	*/
	//}}}


	//add by lincoln [2005:12:10] [14:36]
	//{{{点卡寄售
	SCRIPT_FUNC_OPENCONSIGN			,
	//}}}

	SCRIPT_FUNC_OPENPLAYERSHOP		,
	SCRIPT_FUNC_OPENSHOPITEM		,		
	SCRIPT_FUNC_UPDATEPLAYERSHOP	,

	SCRIPT_FUNC_NEWBIETIPS          ,
	SCRIPT_FUNC_SYSMSGUPDATE        ,
	SCRIPT_FUNC_MAILMSGUPDATE       ,
	SCRIPT_FUNC_TARGETSTATE         ,
	SCRIPT_FUNC_CHANGEFIGURE        ,
	SCRIPT_FUNC_OPENSKILLLEARN      ,
	SCRIPT_FUNC_SKILLLEARNUPDATE    ,
    
    SCRIPT_FUNC_OPENFAMILYLIST      ,  // 打开家族列表界面
    SCRIPT_FUNC_OPENCREATEFAMILY    ,  // 打开创建家族面板
    SCRIPT_FUNC_UPDATEFMDETAILINFO  ,  // 本家族的信息需要更新

	SCRIPT_FUNC_UPDATEMASTERPRENTICE,  //wparam=1:打开师父登记列表界面,num;
									   //wparam=2:更新师徒面板列表,nummaster,numprentice,mode(1:open, 2:update)
									   //wparam=3:打开师父登记时输入介绍的界面

    SCRIPT_FUNC_OPENFAMSTASH,          //wparam=0: 输入密码
                                       //wparam=1: 首次设置密码
                                       //wparam=2: 修改密码
                                       //wparam=3: 打开购买列表,num个数

	SCRIPT_FUNC_OPENAUTHWIN,           

	SCRIPT_FUNC_OPENREPAIRITEM,

    SCRIPT_FUNC_OPENINPUTMONEY,         // 打开输入钱的对话框
    
    SCRIPT_FUNC_OPENEQUIPOTHER,         // 打开其查看它玩家的装备窗口

    SCRIPT_FUNC_UPDATETASKLIST,         // 更新任务称号列表

    SCRIPT_FUNC_OPENWORKTIP,            // 打开打工选择菜单
                                        // wparam = 1 ; 表示显示  lparam = 1 ; 表示钓鱼 lparam = 2 ; 表示挖矿
                                        // wparam = 0 ; 表示关闭

	SCRIPT_FUNC_ENTERMAP,               //进入地图, wparam=0: 第一次， =1:切换地图

	SCRIPT_FUNC_OPENADVERTISE,          //打开公告界面

	SCRIPT_FUNC_OPENSECULOCK,          //打开财产安全锁界面--(bshow,nmode,pcaption,pinfo),nmode==0:设定,nmode==1:修改,nmode==2:输入

	SCRIPT_FUNC_OPENOWEOPT,            //打开充值提示界面--bshow
	
	//add by jension [2006:01:06] [15:56]
	//{{{宠物唤醒
	SCRIPT_FUNC_OPENWAKEPETDLG,			//打开宠物唤醒界面
	//}}}

};


/**
 * 聊天的信息
 */
enum
{
	CHAT_GENERAL = 0,   //公共，一般信息
	CHAT_WHISPER,       //密语
	CHAT_TEAM,          //队伍
	CHAT_CLAN,          //工会
	CHAT_BUDDY,         //好友
	CHAT_SYSTEM,        //系统
	CHAT_MASTER,        //师徒
};

enum
{
	UI_RICHFLAG_ICONNICKNAME=1,
};


struct stDialogInfo
{
	enum
	{
		MAX_INPUT = 4,
	};
	struct stOption
	{
		bool enddlg;  //此选项结束对话
		char name[CS_MAX_OPT_LEN];
	};
	struct stInput
	{
		int inputtype; //0: 数字，   1: 字符串
		char varname[CS_MAX_VAR_LEN];
		char showname[CS_MAX_SHOW_LEN];
		int  minvalue, maxvalue;
	};

	int type; //0: 只有一句话, 1: 一句话+选项,  2: 一句话+若干输入值
	char sentence[CS_MAX_STR_LEN];
	
	stOption options[CS_MAX_OPTS];
	int noption;

	stInput inputs[MAX_INPUT];
	int ninput;
};


enum
{

	CUSTOM_BUFFER_NONE			=  -1,
	CUSTOM_BUFFER_SKETCHMAP		=	0,
	CUSTOM_BUFFER_CHARACTER		=	1,
	CUSTOM_BUFFER_HEAD          =   2, //自己或者交易对象的头像
	CUSTOM_BUFFER_TEAMHEAD      =   3, //队友的头像
	CUSTOM_BUFFER_CHARSTATE     =   4, //角色状态，宠物等图标
	CUSTOM_BUFFER_MAINHEAD      =   5, //自己左上角的头像
	CUSTOM_BUFFER_QQHEAD        =   6, //QQ好友的头像
	CUSTOM_BUFFER_FIGURE        =   7, //改变形象的预览图
	CUSTOM_BUFFER_HAIRHEAD      =   8, //发型发色的头像
	CUSTOM_BUFFER_MONEY         =   9, //金钱
	CUSTOM_BUFFER_CLOUD         =   10,//片头
	CUSTOM_BUFFER_LOGO          =   11,//动画
	CUSTOM_BUFFER_PET           =   12, //宠物
	CUSTOM_BUFFER_DIAGRAM       =   13, //缩略图
    CUSTOM_BUFFER_EMBLEM        =   14, //家族徽章
	CUSTOM_BUFFER_PETHEAD       =   15, //宠物头像
	CUSTOM_BUFFER_PROBE	        =   16, //防范外挂图片
    CUSTOM_BUFFER_TIMER	        =   17, //防范外挂中选择时的倒计时
	CUSTOM_BUFFER_OTHERSTATE    =   18, //其他玩家的角色状态形象
	CUSTOM_BUFFER_STAMINA       =   19, //耐力，念力
	CUSTOM_BUFFER_PINGDELAY     =   20, //网络ping值
	CUSTOM_BUFFER_LOGINAUTH     =   21, //登录验证码
};



struct stUICharacter
{
	//0-选人界面的character，1-创建角色的选择图,  2-创建角色的preview character
	int style;

	int race;
	int gender;
	int hair;
	int head;
	int suitid;

	//普通状态（静止），空闲（随机小动作），激活状态（运动），无效状态（变灰，静止），不可见状态（不绘制）
	//normal,active,diable,invisible
	//参考UISpriteState定义
	int state;
	

	//朝向(0-front,1-back,2-right,3-left)
	//参考UISpriteDirect定义
	int direct;


	stUICharacter()
	{
		style=0;
		race=0;
		gender=0;
		hair=0;
		head=0;
		suitid=0;

		state=0;
		direct=0;

	}


	
};

struct stDrawSketchMap
{
	stRect rect;
	int  scalelevel;  //将缩放等级分为[0, 100],  0表示地图缩小到最小，100表示地图放大到最大
	bool bdrawtips;   //是否画所有的鼠标提示
};

struct stDrawMainHead
{
	stRect rect;
};

struct stDrawFigure
{
	int index;
};

struct stDrawHairHead
{
	int gender;
	int hair;   //发色(0..n)
	int head;   //发型(0..n)
};

struct stDrawMoney
{
	int money;
	int style; //0:center,  1:left
};

struct stDrawTeamHead
{
	int index;
};

struct stDrawDiagram
{
	int mapid;
};



/*
目前游戏发给UI模块的消息规则:
1.只在OnkeyDown中的:
//VK_RETURN, VK_TAB, VK_ESCAPE , VK_BACK, 
//VK_HOME, VK_END, VK_DELETE, VK_INSERT
//VK_LEFT, VK_RIGHT,  VK_UP, VK_DOWN
//其他


//2.只在OnChar中的:
//没有

//3.在OnChar,OnKeyDown中都有的:
//VK_SPACE,   '1',  'A',  'B',............

所以在onChar只是处理往buffer里面加字符， 在OnKeyDown里面来改变buffer
*/





/**
处理这些界面时候需要调用的脚本接口

SetInputNumber( int number );    //设置移动物品等需要输入的数值

int GetMoney( [int number] );    //得到 [包裹] [如果有参数，就是第几个仓库] 的钱数 

int GetMaxWeight();              //得到最大负重

int GetPackWeight( [int itype] );//得到 [包裹] [如果有参数，就是UI_DragType] 的重量---显示负重

int GetWeightAfterShop();        //得到预计的买卖以后负重

int GetTotalPrice();             //得到 [买卖选定列表] 的总的价格

CloseStore();                    //关闭保管仓库---按仓库界面关闭按钮

UseItem( int type, int slot );   //使用某种类型，某个栏位的物品

SaveMoney( boolean bsave, int number ); //bsave决定存钱到保管箱，还是取钱

ToggleNpcShop( int mode );       //切换商店模式， mode = 0:关闭，  1：购买模式， 2：卖出模式

ConfirmShopSale();               //确认进行当前模式的商店买卖

CancelShopSale();                //取消当前模式的商店买卖

ClearNpcShop();                  //清空商店选择列表

CancelNpcShopItem( int i )       //取消商店选择列表中的第几个(1...n)

DealRequest( char *pother );     //向别的玩家提交易请求

AgreeDeal( boolean agree );      //当别人的交易请求到达，是否同意进行交易

CancelDeal();                    //取消交易

LockDealItem( money );           //锁定或者解锁将要交易的物品，金钱

CompleteDeal();                  //点成交，达成交易

InputDealMoney( int number );    //输入交易的金钱数目

TeamCreate( char *pteamname, [char *pother, const char *pmsg] )  //创建队伍 [可能邀请别的玩家共同创建]
TeamInvite( char *pother, char *pmsg ); //向别的玩家提出加入队伍邀请
TeamAcceptInvite( boolean accept ) //接受加入队伍的邀请
TeamJoin( char *pother, char *pmsg );  //向别的玩家要求加入他的队伍
TeamAcceptJoin( bool accept );   //接受某人的加入队伍请求
TeamKickout( char *pname );      //踢掉名字为pname的成员
TeamLeave();                     //离开队伍
TeamDismiss();                   //解散队伍
TeamFollow();                    //跟随队长行动
TeamGetInfo( [char *pname] );    //返回自己[或者某玩家]Table( name, nmember, isleader ), 这个isleader表示自己是否队长
TeamGetMember( int i );          //返回Table( name, hp, maxhp, isleader, isonline )
AddNeighborTeam( char *pbuf, char *pname ) //把人名加到一组名字里面去， 返回加入后的名字

SkillCanForget();                //是否可以遗忘技能
SkillForget( int type, int i );  //遗忘某项技能, type=0:主技能， 1:附属技能,  i= (1...n)
SkillGetStatus( int type, int i );//返回一个table( blearned--是否已经学会, bupgrade--能否升级, busable--能否使用, bstudy--是否可以学习, name--名字，level--等级描述char* )
SkillGetNumber( int type );      //得到当前学会技能的数目
SkillGetPoints();                //得到当前技能点数
SkillUpgrade( int type, int i ); //升级技能
SkillLearn( int type, int i );   //习得技能

SwitchSuit( int i );             //切换到第几个套装

TaskRequireList();               //获取任务列表
TaskCancel( int i );             //取消未完成任务里面的第几个
TaskGetNumber( int type );       //得到某种任务列表里面任务的个数
TaskGetInfo( int type, int i );  //得到某种(1-未完成,2-世界任务,3-故事任务)任务列表里面, 某个任务的信息
                                 //返回Table( title, desc, nhint, hints ),  其中hints是一个Table, hints[1]到hints[nhint]
								   都是String类型的信息
TaskGetHint( int type, int itask, int ihint ); //得到某种任务列表的某个任务的某个hint, String类型
TaskFindByName( int type, char *pstr );   //寻找包含此字符串的任务, 返回task_index

Chat( char *pdest, char *pinfo );    //pdest = "公共", "队伍", "公会", "好友" 或者玩家名字
t, num = GetChatGoal();             //t[i=1...n] 是第几个聊天对象的名字, num 多少个对象
bool IsChatEnable( int i );         //得到某种chat信息是否接收
EnableChat( int i, bool enable );  //enable or disable 接收某种chat信息
neatmsg, sender = GetNeatChatMsg( const char *pmsg ) //分解聊天消息

-------合成过程-----------------------
1. 打开合成精炼镶嵌界面
SCRIPT_FUNC_OPENITEMMOD( type, 列表数目 ）
2. ItemModSelect( int i )            //选择合成精炼的第几项(1...n),  镶嵌不需要调用
3. SCRIPT_FUNC_UPDATEITEMMOD( 材料或者孔洞数目, 价格 )
4. ItemModConfirm( bool b )          //确认或者取消合成精炼镶嵌
-------合成过程-----------------------
ItemModSelect( int i )            //选择合成精炼的第几项(1...n), 镶嵌不需要调用
ItemModConfirm( bool b )          //确认或者取消合成精炼镶嵌

MasterRequest( char *pname )      //提出拜师请求
ApprRequest( char *pname )        //收对方为徒的请求
AgreeMasterRequest( bool b )      //是否接收拜师请求
AgreeApprRequest( bool b )        //是否接受收徒请求
OweMaster()                       //对师傅尽情感恩
MasterKickout( char *pname )      //踢出某个徒弟
MasterBetray( char *pname )       //叛师
GetMasterName( char *pname )      //得到名字为pname的那个人的师傅名字

PopQQMsg()                        //点击QQ信息， 弹出一条待处理的信息
GetNumQQBuddy()                   //得到QQ好友个数
t = GetQQBuddy( int i, bool isindex=true ) //得到第几个(1...n)QQ好友， 
			返回table( char *name, int uin, bool bonline, int age, char *gender, char *nation, char *province, char *city )
QQBuddyAccept( bool b )           //接受QQ好友加入请求
QQBuddyReqAdd( char *pname )      //请求加某人为好友
QQBuddyTalk( int uin, char *pmsg )  //和qq_uin好友聊天，将向UI发GM_QQBUDDYTALK消息
QQBuddyDel( int uin )               //删除QQ好友: uin
QQChangeStatus( int status )       //改变qq在线状态:  1-离开, 2-离线，3-上线,  4-隐身
QQBuddyRequireInfo( int uin )     //需要qq好友信息

GetNumGameBuddy()                 //得到游戏好友个数
t = GetGameBuddy( int i )         //得到第几个(1...n)游戏好友， 
		返回table( char *name, bool bonline, bool bmaster, bool bmale, level, prof:职业， type:类型, title:称号 )
num,names,flags=GmBuddyGetAll(plisttxt); //返回好友个数，和好友名字, 是否在plisttxt中的数组

GmBuddyReqAdd( char *pname )      //请求加某人为好友
GmBuddyReqRemove( char *pname )   //请求从好友里面删除某人
AcceptGmBuddyReq( bool b )        //是否接受对方的加入好友请求
GmBuddySetFlag( bool bhide, bool bshowlevel ) //设置自己是否在好友名单里面隐身和显示等级
bhide, bshowlevel = GmBuddyGetFlag() //得到自己是否在好友名单里面隐身和显示等级
ToggleMailReject( int i )         //改变第几个游戏好友的邮件拒收选项(1...n)
ConfirmMailReject( bool b )       //true--确认所有对游戏好友的邮件拒收选项, false--直接关闭取消
MailReject( int i, bool b )       //设置第几个游戏好友的邮件拒收选项(1...n)

name, enable, price, desc = GetTransport( int i ) //得到第几个(1....n)传送点的信息
TransportTo( int i )              //传送到第几个(1....n)传送点

WriteMail( int type, char *pto, char *ptitle )  //写某种类型的email: 0--普通， 1--小包裹， 2--大包裹
SendMail( char *pto, char *ptitle, char *pbody, int money=0 ) //发送邮件
ReceiveMail( int i )              //收第几封邮件(1.....n)
GetMailDetail( int i )            //得到第几封邮件(1.....n)的内容，返回: char *pfrom, char *ptitle, char *pbody, int money, int type, int nitem
DeleteMail( int i )               //删除邮件(1.....n)
GetMailFlag()
SetMailFlag()

int GetNumWorldServer()               //得到多少个worldserver
char *GetWorldServer( int i )         //得到第几个(1...n)worldserver的名字
SelectWorldServer( int i )            //选择第几个(1...n)作为当前server
int GetNumZoneServer()                //得到多少个zoneserver
char *GetZoneServer( int i )          //得到第几个(1...n)zoneserver的名字
int GetServerBusy( int i )            //得到第几个(1...n)zoneserver的忙碌状态, 1 （优秀）  2（良好）  3（繁忙）   4（爆满）
SelectZoneServer( int i )             //选择第几个(1...n)作为当前server
ConnectDirServer()                    //连接dir server, client引擎到选择world, zone界面
ConnectZoneServer()                   //连接选择好的zoneserver, 当连接成功，client引擎将调用GameUIGotoScreen("entry")进入登陆界面

ConsignItem( int price, int count, int discount ) //寄售物品, count:玩家输入数目，可能超出范围,  discount:折扣，<=0表示无折扣
ConfirmConsign( bool b )               //确认寄售
BuyConsign( int type, int i )         //买某类型的第几个物品(1...n)
CancelConsign( int i )                //取消第几个物品寄售(1...n)
FindMyConsign()                       //自己寄售的物品
FindConsign( char *pitemname, int minlevel, int maxlevel, int minprice, int maxprice, char *powner ) //复合查询, 无意义的值表示不考虑此类型
name, price = ConsignItemInfo( int type, int i ) //买某类型的第几个物品(1...n)的名称，价格信息
int GetNumConsign( int type )         //得到某类型的寄售物品总数
EndConsign()                          //结束寄售活动，关闭寄售状态栏，购买物品时候需要调用
ClearConsign()                        //清除寄售物品

AddShopItem( int price, int number )  //在商店中增加一个物品
DelShopItem( int i )                  //删除第几个物品(1...n)
int GetNumShopItem()                      //得到商店设置中的物品个数
name, price = ShopItemInfo( int i )   //商店设置中第几个物品的信息
CreateShop( char *pshopname )         //选择好物品开一个商店，物品在UI_DRAG_TYPE_SHOP里面
EndShop();                            //关闭商店

SetPlayerTitle( int type )            //设置称号系统的选项: 1--职业称号, 2--任务称号, 3--师徒称号, 4--工会称号
int GetPlayerTitle()                  //得到称号系统的设置: ...

char *PopNewbieTips()                 //弹出一条tip, 如果没有了， 返回空字符串
char *PopSystemMsg()                  //弹出一条系统消息, 如果没有了， 返回空字符串

ncolor, array = GetCtrlTextColor()    //得到控制字符对应的颜色表,  ncolor=多少个颜色，array[ncolor]=颜色值(int)
bool CanMakeMaster(const char* playername);//能否拜玩家playername为师父
bool CanMakePrentice(const char* playername);//能否收玩家playername为徒弟
//返回-1, 不显示菜单
//0:可拜为师父
//1:可收为徒弟
//2:可以叛师
//3:可以逐出师门
int GetMasterMenuType( const char *playername);
//返回-1, 不显示菜单
//返回0，显示“邀请组队”
//返回1，显示“请求入队”
//返回2，显示“邀请入队”
//返回3，显示“踢出队伍”
//返回4, 显示 "跟随队长"
int       GetTeamMenuType(const char* playername);
bool IsQQBuddy(const char* player);
bool IsGMBuddy(const char* player);
bool CanOweMaster(const char *player);  //是否能对师傅感恩

GetCharInitParam( int type[1,2,3])     //得到创建角色时候的参数, 返回table{ STR, DEX, VIT, AGI, INT, LUK, HP, SP }
FollowTeamLeader()                     //开始跟随队长
FollowPlayer( const char *pname )      //开始跟随某个玩家
GetCurMapName()                        //得到当前地图名字
IsPlayerLive()                         //当前玩家角色是否活着
GetPlayerProf()                        //得到当前玩家的职业
IsFollowOther()                        //是否跟随别人
IsHoldPhone()                          //是否持麦

num,names,bselectflags=GetNeighborList(pcurlisttxt);
bselectflags[i]:name[i]是否已经添加在pcurlisttxt中
pcurlisttxt形如：delta;lincoln;qujianbiao

ChangePlayerFigure(int i)              //改变发型发色
GetActorTypeByName(char *pname)        //根据名字得到actor类型
name, price = GetShopItemInfo( int dragtype, int i ) //得到商店item的信息,  i = (1....n)
GetHelpText()
GetLastUserName()
GetNumMail()
DeleteAllMail()
PushNewbieTips( int id )               //加入新手提示
ChangePlayerRace( int race )           //创建角色界面改变角色类型（敏捷，力量等）

GetMainPetInfo()                       //得到当前宠物的信息,类似于GetMainPlayerInfo, 返回table{name=, CURHP=, ...}
SetPetNickName( const char *pname )    //设置当前宠物昵称
bool HaveMapDiagram()                  //返回当前地图是否有缩略图
*/

struct UITeamInfo
{
	char szName[32];
	UINT TeamID;
	int TeamMode; //1 个人 2 多人
};

struct UIMemberInfo
{
	char szName[32];
	UINT Uin;
	UINT PlayerID;
};
struct UIPlayerInfo
{
	char szName[32];
	UINT Uin;
	UINT PlayerID;
};
extern stDialogInfo g_DlgInfo;
extern std::vector<UITeamInfo> g_vecCurTeamlist;
extern std::vector<UIMemberInfo> g_vecMemberList;
extern std::vector<UIPlayerInfo> g_vecPlayerList;
extern UITeamInfo * g_MyTeam;

extern float GameUIGlobalQuery(int type,float defaultvalue=0.0f,void *pdata=0);
//{ modify by novaliu , 20050915
//extern void  GameUITalkInfo( int type, const char *pinfo, const char *pfrom, const char *pto );
extern void GameUITalkInfo ( int channel, int mode, const char* pinfo, const char*pfrom, const char* pto );
//}
extern int   GameUIOnWinMessage( UINT msg, WPARAM wparam, LPARAM lparam);


#endif