#pragma once

#define MSGHEADLENGTH       (4*sizeof(short) + 2*sizeof(char) + sizeof(int))
#define MAXOPTLENGTH		127
#define MAXPACKAGESIZE		((short)0x7000)
#define MAXATTACHINFOLEN	2048
/*
���¶���������Ϣ����
*/
#define Request         0
#define Response        1
#define Notify          2
#define Ack             3
#define Internal        4

/*
���¶������е�FE
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
	State_Ready		=	0,	//׼��
	State_UnReady	=	1,	//ȡ��׼��
};

enum enReadState
{
	State_UnReadied	=	0,	//��׼��״̬
	State_Readied	=	1,	//׼��״̬
};

enum enSeatStateCmd
{
	Seat_Close	=	0,	//�ر�
	Seat_Open	=	1,	//��
};

enum enSeatType
{
	Seat_Closed		= 0,  //�ر�
	Seat_Free		= 1, // ��λ
	Seat_Used		= 2   //����
};

//Ĭ��Ϊ0 ����ʤΪ1
enum ENMWinType
{
	EWT_NORMAL = 0, //��ͨʤ��
	EWT_PERFECT , //��ʤ
};

//����׼��״̬
enum ENMITEMSTATUS
{
	EIS_UNEQUIPPED	=	0,          //δװ��
	EIS_EQUIPPED	=	1,          //װ��
};

enum EPropEffectResult
{
	EPER_ATTACKED = 0,//������
	EPER_OUTOFATTACK = 1,	//û������
	EPER_OUTOFATTACK_BECAUSEOFAVATAR =2,	//��Ϊ��avatar ����û������
};


#define ST_PLAYER_INIT						(0x07)
#define ST_PLAYER_WAITFORGAMEINFO 			(0x08)
#define ST_PLAYER_INSINGLEROOM		   	    (0x09)  //�ڵ�����Ϸ����, ��ҵ�½ʱĬ��ֵ
#define ST_PLAYER_INMULTIHALL               (15)  //�ڶ�����Ϸ����,��½������Ϸ��δ����MultiRoom 
#define ST_PLAYER_INMULTIROOM				(16)   //�ڶ�����Ϸ����
#define ST_PLAYER_INMULTIGAME				(17)  //�ڶ�����Ϸ��
#define ST_PLAYER_INMULTIGAMEAWARD          (18)  //�ڶ�����Ϸ�佱��
//others         //�ڵ�����Ϸ
#define ST_ROOM_IDLE				        0
#define ST_ROOM_WAITING			            1
#define ST_ROOM_INGAME				        2
#define ST_ROOM_AWARD                       3     //���Ű佱

#define MAXMODENUM    IDX_GM_MAX

///��Ʒ��
#define INTER_TIMEOUT	1	//�ڲ���ʱ��Ϣ
#define MAXRESULTSTRLENGTH	128 //����ַ�����󳤶�
#define MAX_ITEM_PERCOMMODITY 10 //��Ʒ�д������������
#define MAX_COMMODITY_DESCLEN 128 //��Ʒ������󳤶�
#define	MAX_COMMODITY_NAMELEN 64 //��Ʒ������󳤶�
#define MAX_COMMODITY_NUM	500 //Э����������Ʒ��

#define MAXITEMNUMONCEADD       20	//ÿ�����ӵ��ߵ�����������
//#define MAXATTACHINFOLEN	128     //���Ե���󳤶�

//�̵�֧������
enum ENMPayType
{
	EPT_PAY_QB = 0,	//Q��
	EPT_PAY_QP,		//Q��
	EPT_PAY_KB,		//���
};

/*
�û���Ϣ���
*/
#define MAXNICKNAMELEN          20
#define MAXATTACHCONTENTLEN    	200
#define MAXCLIENTGETROOMNUM    	60
#define MAXCSGAMEKEYLEN         32
#define MAXROOMNAMELEN         	22
#define MAXROOMPASSWORDLEN    	16
#define MAXSEATNUMINROOM       	8
#define MAXQQKARTNICKNAMELEN    17          //�ǳƵ���󳤶�

#define MAXREGISTERADDITEMNUM   10          //ע��ʱ���ӵ��ߵ��������

#define MAXITEMNUMPERPLAYER  	100			//ÿ�������������
#define MAXGIFTNUMPERPLAYER 	50			//ÿ��������������
#define	MAXGROUPNUMPERPLAYER	20			//ÿ����������ѷ�����
#define MAXFRIENDNUMPERPLAYER	200			//ÿ�������������
#define MAXFRIENDGROUPNAMELEN	20			//�����ѷ������Ƴ���
#define MAXFRIENDNUMPERGROUP    100         //ÿ��������������
#define MAXFRIENDNUMPERPLAEYR   100         //��������
#define MAXWORDSLEN             512         //���Ӻ������Գ���
#define MAXPROPBLANK			2

/*
Э�鶨��
*/
//0-499Ϊ�ͻ���������Ϣ
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
 //����������
#define ID_CMS_REQUESTASSIGNATTRIBUTE           131            
//����ϴ�� 
#define ID_CMS_REQUESTCLEARATTRIBUTE            132             
//����ע�������Ƿ���Ч
#define ID_CMS_REQUESTCHECKNAME                 133

#define ID_CMS_REQUESTGETTASKLIST				135		//��ȡ�����б�
#define ID_CMS_REQUESTTAKETASK					136     //�����������
#define ID_CMS_REQUESTDELETETASK				137     //����ɾ��(ע��)����
#define ID_CMS_REQUESTSTARTSINGLETASK			138     //����ʼ��������
#define ID_CMS_REQUESTREPORTCURRENTOPT          139		//�ϱ���������
#define ID_CMS_REQUESTSKIPTASK                  140		//������������

//500-999Ϊ������֪ͨ��Ϣ
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
#define ID_SMC_NOTIFYUPDATEMONEY				528  	//֪ͨ���¿��
#define ID_SMC_NOTIFYADDITEM					529		//֪ͨ���ӵ���

#define ID_SMC_NOTIFYSINGLEGAMEBEGIN			530     //֪ͨ������Ϸ��ʼ
#define ID_SMC_NOTIFYSINGLERACEBEGIN			531		//֪ͨ��������ʼ
#define ID_SMC_NOTIFYSINGLERACEFINISH			532     //֪ͨ������ɻ�ʱ
#define ID_SMC_NOTIFYSINGLERACEOVER             533		//֪ͨ����������
#define ID_SMC_NOTIFYSINGLEGAMEOVER				534     //֪ͨ������Ϸ����
#define ID_SMC_NOTIFYTASKAWARD                  537		//֪ͨ������

#define ID_SMC_NOTIFYKICKFROMSERVER				900
#define ID_SMC_NOTIFYUDPOK                      999

//�����
#define ID_CMS_NOTIFY_ANTI_BOT                  167

#define ID_CMS_REQUESTBUYCOMMODITY		20000	//������
#define ID_CMS_REQUESTGETCOMMODITYLIST	20001	//��������Ʒ�б�

//��½���Ͷ���ö��
enum ENMLoginType
{
	ELT_Single = 0,  //������Ϸ
	ELT_Multi = 1,  //������Ϸ
};

//��Ϸģʽ
enum enGameMode
{
	eItem_Single	= 0,
	eItem_Muti		= 1,
	eSpeed_Single	= 2,
	eSpeed_Muti		= 3,
};

enum ENMMapRandomType
{
	EMRT_RANDOM_ALL = 0,    //�������
};


#define MAXCHATCONTENTLEN           500
#define MAXAWORDSIZE				10 
#define MAP_HASH_CODE				32
#define MAXCHECKPOINTNUM			100
//#define MAXREGISTERADDITEMNUM		5		//ע��ʱ���ӵ��ߵ��������
#define	MAXCHANGEITEMSTATUSNUM		20		//һ���ܸı����״̬���������
#define MAXAWARDITEMNUM				3		//�������������������			
/*���¶���ͻ��˵�����״̬*/
#define ST_CLIENT_NOLINK	((BYTE)0x00) //δ����״̬
#define ST_CLIENT_ONLINE	((BYTE)0x01) //����״̬
#define ST_CLIENT_OFFLINE	((BYTE)0x02) //����״̬
#define ST_PLAYER_INIT						(0x07)
#define ST_PLAYER_WAITFORGAMEINFO 			(0x08)
#define ST_PLAYER_INSINGLEROOM		   	    (0x09)  //�ڵ�����Ϸ����, ��ҵ�½ʱĬ��ֵ

enum enTalkType
{
	Talk_Wisper = 0,  //����
	Talk_InRoom = 1, //����
	Talk_InGame = 2, //��Ϸ��
	Talk_Buglet = 3   //С����
};

#define		SEATCLOSED		0
#define		SEATOPENED		1
#define		READEINROOM		0
#define		NOTREADYINROOM 	1

/*
���¶��������
*/
typedef enum
{
	Succeed                 		=   0,
	Err_InvalidDestFE         	 	=   1,  //�Ƿ���Ŀ��ʵ��
	Err_InvalidSrcFE        	   	=   2,  //�Ƿ���Դʵ��
	Err_LowVersion            	 	=   3,  //�汾����
	Err_MultiLogin             		=   4,  //�ظ���¼
	Err_ServerFull             		=   5,  //��ʾserver��������������Ҫ��һ��ط�
	Err_WaitPlayerInfoTimeOut  		=   6,  //ȡ��������ϳ�ʱ��������
	Err_GetPlayerInfoFailed    		=   7,  //ȡ���������ʧ��
	Err_WaitRegisterTimeOut  		=   8,  //ע�ᳬʱ��������
	Err_RegisterFailed    			=   9,  //ע��ʧ��
	Err_WaitSpecialTitlesTimeOut  	=   10, //ȡ���������ƺų�ʱ��������
	Err_PointLessThanLowLimit  		=   11,  //���ֲ���
	Err_ForbidLogin            		=   12,  //��ֹ��½
	Err_InvalidServerID        		=   13, //��Ч�ķ��������
	Err_InvalidRoomID         	 	=   14, //��Ч�ķ�����
	Err_InvalidGameLogicID     		=   15, //��Ч����Ϸ���
	Err_InvalidPlayerID        		=   16, //��Ч����ұ��
	Err_InvalidUserIdef        		=   17, //�����ݲ�������뵱ǰ����������
	Err_PlayerNotOnLine        		=   18, //��Ҳ�����
	Err_PlayerNotOnServer         	=   19, //���ڵ�ǰ������
	Err_PlayerNotExist         		=   20, //��Ҳ�����
	Err_AddFriendOffline       		=   21, //�Ӻ���ʱ�Է�������
	Err_AddFriendUpperLimit    		=   22, //���������Ѵ�����
	Err_AddFriendRepeat        		=   23, //�Ӻ���ʱĿ�ķ��Ѿ��ں�����
	Err_NoLimitKickGM          		=   24, //û��Ȩ����GM
	Err_InvalidAction         		=   25, //�������
	Err_PlayerNotInTeam  			=   26, //��Ҳ����ڶ���
	Err_PlayerNotCaptain 			=   27, //��Ҳ��Ƕӳ�
	Err_TeamLeaveGame  				=   28, //�����뿪��Ϸ
	Err_GetFriendListTimeOut 		=   29, //��ȡ��Һ����б�ʱ
	Err_ListDeleteFriendTimeOut 	=   30, //���ɾ�����ѳ�ʱ
	Err_InvalidUin    				=   31, //qq�������
	Err_RoomInGame                  =   32,//����������Ϸ��
	Err_NotUsedRoom                 =   33, //û���������
	Err_InvalidRoomKey              =   34, //�������벻��ȷ
	Err_RoomFull                    =   35, //��������
	Err_IsNewPlayer              	=   1000, //�������Ҫע��
	Err_ResouceLack            		=   1001, //��������Դ����
	Err_TimeOut                		=   1002, //������ʱ
	Err_System                 		=   1003, //ϵͳ����
	Err_Unknow                 		=   1004, //δ֪����
} EResult;

/*
P2PЭ�鲿��
*/
#define ID_TCP_HELP_P2P_SEND_MSG 101
#define ID_TCP_HELP_P2P_RECV_MSG 102

//����ϵͳ����
#define MAXREGISTERADDTASKNUM	5         //ע��ʱ���ӵ����������
#define MAXTASKNUMPERPLAYER     20		  //ÿ�����ӵ�е����������

/*-------------------------------------------------------------------
Dirsvrd ��Client ����Ϣ����
------------------------------------------------------------------*/									//����ҿ���Location�ǵ�һ��Ŀ¼��Gsvrd�ǵڶ���Ŀ¼
#define HASH_CODE_LENGTH                   32           //hash�볤��
#define MAX_DOWNSVR_NUMBER              10         //������ط�������
#define MAX_DIRECTION_LENGTH              128         //���ظ�Ŀ¼����
#define MAX_FILE_NAME_LENGTH               128       //�����ļ�������
#define MAX_WORLD_NUM                      10        //���world��Ŀ
#define MAX_WORLD_NAME_LEN                 16        //���world���ֳ���
#define MAX_DIR_LISTEN_PORT_NUM               3        //dir�������˿�����
#define MAX_WORLD_DIR_SERVER_NUM       4    //world��Ŀ¼������������
#define MAX_SHOP_SERVER_NUM              5       //����̵����������
#define MAX_BORDER_SERVER_NUM           20      //���߾�����������
#define MAX_REASON_LEN                    200      //���ԭ�򳤶�
#define MAX_NETADMIN_SERVER_NUM         4       //������ܷ���������
#define MAX_MATCH_SERVER_NUM             6      //����������������
#define MAX_COUNTRY_NUM                  5      //�����Ҹ���
#define MAX_COUNTRY_CHANNEL_NUM       10      //�����ڵ����Ƶ����
#define MAX_IDC_SERVER_NUM               10      //���idc����
#define MAX_IDC_GANE_SERVER_NUM         20     //IDC����Ϸ������������


#define ID_CMD_REQGETHALLINFO         90     //Client  ��ȡ�������б�


/*---------------------------------------------------------------------
Client �롡Client �����Ϣ����
----------------------------------------------------------------------*/

#define MAX_KEY_NUM			16
#define UDP_MSG_PLAYMOVE   20000
#define UDP_MSG_COLLSION   20001

//������Client���صĽ�����
enum ENMDirLoginResult
{
	EDLR_Succeed = 0,
	EDLR_Err_IdentityLimit  = 1, //δȡ�������ʸ�
	EDLR_Err_VersionLimit  = 2 , //�汾����
	EDLR_Err_VersionRecommend = 3, //�汾��������
	EDLR_Err_FileCracked  = 4,  // �ļ���
	EDLR_Err_WorldIDError = 5, //�����worldid ������
};

#pragma  warning(disable:4200)
#pragma  warning(disable:4244)
#pragma pack(push,1)
#include "code.h"
#pragma pack(pop)