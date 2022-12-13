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

//UDP����ͷ
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
	ULONG	m_unUin;							//Դ���uin
	UCHAR	m_cHasKeyChange;					//�а����仯û��
	UCHAR	m_cArrKey[MAX_KEY_NUM];				//����״̬�����а����仯ʱ�������ã�
	ULONG	m_unPlayAnimID;						//��ҵ�ǰ����Id
	ULONG	m_unKartAnimID;						//����ǰ����Id
	LONG	m_unLinearVelocity[3];				//��������������ٶ�
	LONG	m_unArrPosition[3];
	LONG	m_unArrMat[9];
	LONG	m_unOffSetZ;				
	LONG	m_unAngle;						//�������������̬��������ת�����λ�ã�
	LONG	m_nRestDistance;				// ��ʣ���پ�����ɱ���
	ULONG	m_unPackageIndex;					//�����
	ULONG	m_unTimeStamp;						//ʱ���
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


//��Ϣ��
class CMsgPara
{
};

//ͨ�������Ӧ����Ϣ
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
	UCHAR	m_bStatus;		//�Ƿ���Ϸ��
	LONG	m_iMapID;
	UCHAR 	m_bFlag;		//�Ƿ����
	CHAR	m_szRoomName[MAXROOMNAMELEN]; 
};


//������ݿ������Ϣ
struct STPlayerDBBaseInfo
{
	CHAR	m_szNickname[MAXQQKARTNICKNAMELEN];  	//��ҵ��ǳ�
	UCHAR	m_bGender;   		 				//����Ա�0��ʾŮ��1��ʾ��
	UCHAR	m_bCountry;	                                             //��ҹ���
	ULONG	m_unExperience;		       //����
	LONG 	m_iMoney;			       //��Ǯ
	ULONG	m_unWinNum;			//ʤ������
	ULONG	m_unTotalNum;			//��������
	ULONG	m_unHonor;                              //��ѫֵ
};

//���������Ϣ
struct STPlayerDBAttributeInfo
{
	USHORT m_nSpeed;               //�ٶ�
	USHORT m_nExplosion;         //����
	USHORT m_nGathering;         //����
	USHORT m_nSpout;                //����
	USHORT m_nMiniSpout;        //С��
	ULONG	m_unReserve;             //δ����
	UCHAR  m_bStrategy;            //�Զ��������
};

//���������Ϣ
struct STPlayerDBOtherInfo
{
	LONG m_iRightFlag;		//�Ƿ����
	LONG m_iAvailPeriod;	//����ʱ��	
};

//��ҵ�����Ϣ
struct STPlayerDBItemInfo
{
	LONG	m_iItemID;			//����ID 
	LONG	m_iItemNum;			//���߸���
	LONG	m_iAvailPeriod;		//������Ч�ڡ�
	UCHAR 	m_bStatus;			//����״̬��0δװ��,1װ����		
	ULONG	m_unObtainTime;		//���ʱ��	
	LONG	m_iOtherAttribute;	//�����������ֶΣ�
};

//��ҵı�������Ϣ��Ҫ����ͬһ�������������
struct STPlayerRoomVisibleInfo
{   
	ULONG	m_unUin;				//Uin
	ULONG	m_unIdentity;			//������                
	UCHAR 	m_bSeatID;         		//��λID
	UCHAR 	m_bTeamID;  			//�����
	USHORT 	m_nPlayerID;			//���ID
	USHORT  m_nRoleID;              //��ɫid���ݶ�
	USHORT 	m_nKartID;				//��ID
	UCHAR 	m_bStatus;             	//״̬ 
	STPlayerDBBaseInfo m_stPlayerDBBaseInfo;    //������Ϣ
	USHORT	m_nEquipedItemNum;
	STPlayerDBItemInfo m_stPlayerItemInfo[MAXITEMNUMPERPLAYER];	
};
//��Ϸ��Ϣ
struct STGameInfo
{
	STPlayerDBBaseInfo	m_stPlayerDBBaseInfo;					//������ݿ������Ϣ
	STPlayerDBAttributeInfo    m_stPlayerDBAttributeInfo;                                    //���������Ϣ
	USHORT		m_nItemNum;
	STPlayerDBItemInfo	m_stPlayerDBItemInfo[MAXITEMNUMPERPLAYER];
	STPlayerDBOtherInfo	m_stPlayerDBOtherInfo;						//������Ϣ
};

//���������Ϣ
struct STPlayerDBTaskInfo 
{
	LONG m_iTaskID;       //����ID
	SHORT m_shOptionID; //���ܷ�ʽ
	CHAR m_cStatus;      //����״̬��0δ��ɣ�1���
	UCHAR m_bPrevStatus;	//ǰ�������״̬
	LONG m_iProgressRate;    //��ǰ��ɻ��ռ������������ڱ�ʶ�������
	LONG m_iOther;        //���ã����ڱ�ʶ�������
	LONG m_iTakeTime;     //�������ʱ��

};

//�۸�
struct STPrice
{
	LONG m_iQBPrice;	//Q��
	LONG m_iQPPrice;	//Q��
	LONG m_iKBPrice;	//���
};

//����
struct STType
{
	SHORT m_shBase; //����
	SHORT m_shSub; //����
};

//����
struct STDescription
{
	CHAR m_szGeneralDesc[MAX_COMMODITY_DESCLEN]; //��������
	CHAR m_szAttributeDesc[MAX_COMMODITY_DESCLEN]; //��������
};


//�ۿ�
struct STRebate
{
	SHORT m_shMemeber; //��Ա
	SHORT m_shCommon; //��ͨ���
};

//����
struct STLimit
{
	LONG m_iLevelLimit;//�ȼ�����
	LONG m_iHonorLimit;//��������
	LONG m_iOtherLimit;
};

//������Ϣ
struct STItemInfo
{
	LONG m_iItemID;
	LONG m_shAvailPeriod;
	SHORT m_shItemNum;
};


//��Ʒ����
struct STCommodity
{
	LONG m_iID; //ID
	CHAR m_szName[MAX_COMMODITY_NAMELEN]; //����
	STType m_stType; //����
	STDescription m_stDescription; //����
	STRebate m_stRebate; //�ۿ�
	STLimit m_stLimit; //��������
	STPrice m_stPrice; //�۸�
	LONG m_iDateSaleLimit;//ÿ�������޶�
	LONG m_iClientVersion;//֧�ֵĿͻ��˰汾
	LONG m_iPriority; //���ȼ�
	LONG m_iSaleTag; //��ǩ���������Ƽ��ȣ�
	SHORT m_shItemCount;
	STItemInfo m_astItemInfo[MAX_ITEM_PERCOMMODITY];//��Ʒ�д���ĵ���
	LONG m_iDateSold; //�����������ĸ���
};


//�����½��Ϸ
class CRequestLoginPara:public CMsgPara
{
	ULONG 	m_unUin;							//��ҵ�qq����
	ULONG 	m_unTime;
	ULONG 	m_unClientVersion;  				//�ͻ��˰汾��
	CHAR 	m_szQQNickname[MAXNICKNAMELEN];  	//��ҵ��ǳ�
	UCHAR 	m_bGender;   						//����Ա�0��ʾŮ��1��ʾ��
	UCHAR 	m_bIconID;  						//���QQͷ��ID
	ULONG 	m_unAttachIdentify;  				//�汾��Ϣ�������ɰ�
	ULONG	m_uiLoginType;                   //��½���ͣ���ENMLoginType
};

//�ظ���½��Ϸ
class CResponseLoginPara :public CMsgPara
{
	SHORT 		m_nResultID;                
	UCHAR 		m_bKeyGameDataLength;
	CHAR		m_szKeyGameData[MAXCSGAMEKEYLEN]; 
	ULONG 		m_unUin;
	ULONG 		m_unIdentity; 						 	//������
	USHORT 		m_nPlayerID;
	ULONG		m_uiLoginType;                 //��½���ͣ���ENMLoginType
	USHORT		m_nRoleID;                                    //��ɫid���ݶ�
	USHORT		m_nKartID; //��ID		
	ULONG 		m_unGameSvrdIdentity;  					//һЩgsvrd����Ϣ�����Ƿ���ѵ�
	STPlayerDBBaseInfo	m_stPlayerDBBaseInfo;					//��һ�����Ϣ
	STPlayerDBAttributeInfo     m_stPlayerDBAttributeInfo;                                 //���������Ϣ
	USHORT		m_nItemNum;
	STPlayerDBItemInfo	m_astPlayerDBItemInfo[MAXITEMNUMPERPLAYER];	
	SHORT		m_ushTaskNum; 
	STPlayerDBTaskInfo	m_astPlayerDBTaskInfo[MAXTASKNUMPERPLAYER];	//������Ϣ
	UCHAR 		m_bReasonLen;
	CHAR 	m_szReason[MAXATTACHCONTENTLEN];	
};

//����ǳ���Ϸ
class CRequestLogoutPara:public CMsgPara
{
	ULONG 	m_unUin;
	ULONG 	m_unTime;
};


//�ظ��ǳ���Ϸ
class CResponseLogoutPara:public CMsgPara
{
	SHORT	m_nResultID;						//0��ʾ�ɹ�������ʧ��
	UCHAR 	m_bReasonLen;
	CHAR 	m_szReason[MAXATTACHCONTENTLEN];	//ʧ��ԭ��˵��
};

//�������ע��
class CRequestRegisterPara :public CMsgPara
{
	ULONG	m_unUin;                      		//��ҵ�Uin
	ULONG	m_unTime;		       		//���ע��ʱ��	
	CHAR    m_szNickname[MAXQQKARTNICKNAMELEN];  	//��ҵ��ǳ�
	UCHAR 	m_bGender;   		 				//����Ա�0��ʾŮ��1��ʾ��
	LONG	m_nFaceID;                                  //�������
	LONG	m_nHairID;                                   //���ͷ��
	UCHAR   m_bDressIndex;                       //��װ����
	LONG 	m_nColorID;		       		//���������ɫID
	UCHAR  m_bCountry;	                                             //��ҹ���
};

//�ظ����ע��
class CResponseRegisterPara :public CMsgPara
{
	SHORT	m_nResultID; 				//0��ʾ�ɹ�������ʧ��
	ULONG 	m_unUin;					//ע����ҵ�uin
	CHAR	m_szNickname[MAXQQKARTNICKNAMELEN];  	//��ҵ��ǳ�
	UCHAR 	m_bGender;   		 				//����Ա�0��ʾŮ��1��ʾ��
	UCHAR	m_bCountry;	                                             //��ҹ���
	USHORT 	m_nAddItemNum;
	STPlayerDBItemInfo	m_stAddItemInfo[MAXREGISTERADDITEMNUM]; 
	USHORT	m_ushAddTaskNum;
	STPlayerDBTaskInfo	m_astAddTaskInfo[MAXREGISTERADDTASKNUM]; //ע��ʱ���ӵ�������Ϣ
	UCHAR 	m_bReasonLen;
	CHAR 	m_szReason[MAXATTACHCONTENTLEN]; 
};

//������ȡ�����б�
class CRequestGetRoomListPara:public CMsgPara
{
	ULONG	m_unUin;
	ULONG 	m_unTime;
	SHORT	m_nStartRoomNum;  	//ָ����ȡ��ʼλ��
	SHORT	m_nGetRoomNum; 		//����	
	SHORT	m_sGameModeBase;
	SHORT	m_sGameModeSub;
};



//�ظ���ȡ�����б�
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

//���󴴽�����
class CRequestCreateRoomPara:public CMsgPara
{
	ULONG 	m_unUin;
	ULONG 	m_unTime;
	SHORT	m_sGameModeBase; 
	SHORT	m_sGameModeSub; 
	CHAR 	m_szRoomName[MAXROOMNAMELEN]; 
	UCHAR	m_bSeatNum;
	UCHAR	m_bFlag; 		//�Ƿ���ܵ�
	CHAR 	m_szPassword[MAXROOMPASSWORDLEN]; 
};

//�ظ���������
class CResponseCreateRoomPara:public CMsgPara
{
	SHORT	m_nResultID;
	SHORT	m_nRoomID;  //������
	SHORT	m_sGameModeBase; 
	SHORT	m_sGameModeSub; 
	CHAR 	m_szRoomName[MAXROOMNAMELEN];
	UCHAR	m_bSeatNum;
	UCHAR	m_bFlag; 
	UCHAR 	m_bSeatID;  //������λ��
	UCHAR 	m_bTeamID;  //�����
	UCHAR 	m_bReasonLen;
	CHAR 	m_szReason[MAXATTACHCONTENTLEN];	
};

//������뷿��
class CRequestEnterRoomPara:public CMsgPara
{
	ULONG 	m_unUin;
	ULONG 	m_unTime;
	SHORT	m_nRoomID;
 	CHAR 	m_szPassword[MAXROOMPASSWORDLEN];	
};


//�ظ����뷿��
class CResponseEnterRoomPara:public CMsgPara
{
	SHORT	m_nResultID;
	SHORT	m_nRoomID;
	SHORT	m_sGameModeBase; 
	SHORT	m_sGameModeSub; 
	CHAR 	m_szRoomName[MAXROOMNAMELEN]; 
	UCHAR	m_bSeatNum;
	UCHAR	m_bFlag; 	
	UCHAR 	m_bSeatID;  //������λ��
	UCHAR   m_bTeamID;  //�����
    LONG	m_nMapID;               
    SHORT	m_nRoomOwnerID; 
	UCHAR 	m_bSeatStatus[MAXSEATNUMINROOM];         
    UCHAR 	m_bPlayerCount;         
	STPlayerRoomVisibleInfo 	m_stPlayerInfo[MAXSEATNUMINROOM];		
	UCHAR 	m_bReasonLen;
	CHAR 	m_szReason[MAXATTACHCONTENTLEN];
};


//֪ͨ���뷿��
class CNotifyEnterRoomPara:public CMsgPara
{
	SHORT			m_nRoomID;
	STPlayerRoomVisibleInfo	m_stNewPlayerInfo; 
};

//�����뿪����
class CRequestLeaveRoomPara:public CMsgPara
{
	ULONG 	m_unUin;
	ULONG 	m_unTime;
};

//�ظ��뿪����
class CResponseLeaveRoomPara:public CMsgPara
{
	SHORT	m_nResultID;
	UCHAR	m_bReasonLen;
	CHAR 	m_szReason[MAXATTACHCONTENTLEN];
};

//֪ͨ�뿪����
class CNotifyLeaveRoomPara:public CMsgPara
{
	SHORT	m_nRoomID;
	ULONG 	m_unUin;
	UCHAR 	m_bSeatID; 
	CHAR 	m_cIsChangeRoomOwner; //0 ������ 1 ����
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

//������ټ���
class CRequestQuickEnterRoomPara : public CMsgPara
{
	ULONG	m_unUin;		//qq��
	ULONG	m_unTime;		//ʱ��
	SHORT	m_sGameModeBase; 
	SHORT	m_sGameModeSub; 
};


//�ظ����ټ���
class CResponseQuickEnterRoomPara : public CMsgPara
{
	SHORT 	m_nResultID;
	SHORT 	m_nRoomID;
	SHORT	m_sGameModeBase; 
	SHORT	m_sGameModeSub; 
	CHAR 	m_szRoomName[MAXROOMNAMELEN]; 
	UCHAR	m_bSeatNum;
	UCHAR	m_bFlag; 	
	UCHAR 	m_bSeatID;  //������λ��
	UCHAR   m_bTeamID;  //�����
	LONG 	m_nMapID;               
	SHORT	m_nRoomOwnerID; 
	UCHAR 	m_bSeatStatus[MAXSEATNUMINROOM];         
	UCHAR 	m_bPlayerCount;         
	STPlayerRoomVisibleInfo 	m_stPlayerInfo[MAXSEATNUMINROOM];		
	UCHAR 	m_bReasonLen;
	CHAR 	m_szReason[MAXATTACHCONTENTLEN];
};

//����ı���λ״̬
class CRequestChangeSeatStatePara:public CMsgPara
{
	ULONG 	m_unUin;
	ULONG	m_unTime;   
	SHORT 	m_nRoomID;		//�����
	UCHAR	m_bSeatID;		//Ҫ�ı����λ��
	UCHAR	m_bSeatState;	//��λ�ı���״̬��0��ʾ�رգ�1��ʾ�򿪣�
};


//�ظ��ı���λ״̬
class CResponseChangeSeatStatePara:public CMsgPara
{
	SHORT 	m_nResultID;
	UCHAR	m_bSeatID;		//���ı����λ��
	UCHAR	m_bSeatState;	//��λ�ı���״̬
	UCHAR	m_bReasonLen;
	CHAR	m_szReason[MAXATTACHCONTENTLEN];
};

//֪ͨ�ı���λ״̬
class CNotifyChangeSeatStatePara:public CMsgPara
{
	UCHAR 	m_bSeatID;		//���ı����λ��
	UCHAR	m_bSeatState;	//��λ�ı���״̬
};

//��������
class CRequestKickFromRoomPara:public CMsgPara
{
	ULONG 	m_unUin;
	ULONG	m_unTime;   
	ULONG	m_unDstUin;		//������ҵ�Uin
};

//�ظ�����
class CResponseKickFromRoomPara:public CMsgPara
{
	SHORT 	m_nResultID;
	ULONG 	m_unDstUin;		//������Uin
	UCHAR	m_bReasonLen;
	CHAR	m_szReason[MAXATTACHCONTENTLEN];

};

//֪ͨ����
class CNotifyKickFromRoomPara:public CMsgPara
{
	SHORT 	m_nRoomID;
	ULONG 	m_unDstUin;		//������ҵ�Uin
};


//����ı�׼��״̬
class CRequestChangeReadyStatePara:public CMsgPara
{
	ULONG 	m_unUin;
	ULONG 	m_unTime;
	UCHAR	m_bReadyState;	//׼����״̬��0��ʾ׼����1��ʾȡ��׼����
};

//�ظ��ı�׼��״̬
class CResponseChangeReadyStatePara:public CMsgPara
{
	SHORT 	m_nResultID;
	UCHAR	m_bReadyState;
	UCHAR	m_bReasonLen;
	CHAR	m_szReason[MAXATTACHCONTENTLEN];
};

//֪ͨ�ı�׼��״̬
class CNotifyChangeReadyStatePara:public CMsgPara
{
	ULONG 	m_unUin;	
	UCHAR	m_bReadyState;
};

//���� m_bDstFE=FE_PLAYER
class CRequestTalkPara : public CMsgPara
{
	ULONG 	m_unUin;                
	ULONG 	m_unTime;               
	UCHAR 	m_bType;			//����/����/��Ϸ��/С���ȵ�
	ULONG 	m_unDestPlayerUin;  //˽��
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

//֪ͨ����
class CNotifyTalkPara : public CMsgPara
{
	ULONG	m_unSrcUin;
	CHAR	m_szSrcNickName[MAXNICKNAMELEN];
	UCHAR	m_bGender;    
	UCHAR	m_bType;			//����/����/��Ϸ��/С���ȵ�
	ULONG	m_unDestPlayerUin;  //˽��
	SHORT	m_nContentLength;       
	CHAR	m_szContent[MAXCHATCONTENTLEN];
};

//����  m_bDstFE=FE_PLAYER
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
	CHAR	m_szReason[MAXATTACHCONTENTLEN]; //ʧ��ԭ��˵��
};

//������
class CNotifyTestNetSpeedPara : public CMsgPara
{
	ULONG	m_unStartSec;	//��ʼ��
	ULONG	m_unUSec;		//��ʼ΢��
};

//m_bDstFE=FE_PLAYER
class CAckTestNetSpeedPara : public CMsgPara
{
	ULONG	m_unStartSec;	//��ʼ��
	ULONG	m_unUSec;		//��ʼ΢��
};

class CNotifyKickFromServerPara: public CMsgPara
{
	SHORT	m_nReasonID;     
	ULONG	m_unSrcUin;
	UCHAR	m_bReasonLen; 
	CHAR	m_szReason[MAXATTACHCONTENTLEN];
};


//����������������������������ϵͳ��������������������������
//���ѷ�����Ϣ
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

//������ȡ�����б�
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

//�������Ӻ���
//�������ӳɹ���, ���Ѿ�λ��Ĭ�Ϸ�����
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

//����ɾ������
class CRequestDeleteFriendPara : public CMsgPara
{
	ULONG	m_unUin; 
	ULONG	m_unTime;
	ULONG	m_unFriendUin;
	SHORT	m_nFriendGroupID;      //����ָ�������ID
};

class CResponseDeleteFriendPara : public CMsgPara
{
	SHORT	m_nResultID;
	ULONG	m_unUin;
	ULONG	m_unFriendUin;
	SHORT	m_nFriendGroupID;
	UCHAR	m_bReasonLen;
	CHAR	m_szReason[MAXATTACHCONTENTLEN]; //ԭ��˵��
};


//�������Ӻ��ѷ���
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
	CHAR	m_szReason[MAXATTACHCONTENTLEN]; //ԭ��˵��
};

//����ɾ�����ѷ���
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
	CHAR	m_szReason[MAXATTACHCONTENTLEN]; //ԭ��˵��
};

//�����ƶ�������ĳ����
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
	CHAR	m_szReason[MAXATTACHCONTENTLEN]; //ԭ��˵��
};

//Gsvrd֪ͨĳ��ұ�������Ҽ�Ϊ����(����:����/�ܾ�)
class CNotifyBeAddedAsFriendPara:public CMsgPara
{
	ULONG	m_unSrcUin;			//ԭUin����(�Ӻ��ѵķ�����)
	ULONG	m_unDstUin;			//Ŀ�� Uin	(���ӵ���)
	STPlayerInfoForFriend m_stSrcPlayerInfo;
	SHORT	m_nWordLength;				//�Ӻ���ʱ�����Գ���
	CHAR	m_szWords[MAXWORDSLEN];	//����

};

//����Ϊ���ѵ���Ӧ��Gsvrd ѡ����
class CAckBeAddedAsFriendPara:public CMsgPara
{
	SHORT	m_nResultID;				
	ULONG	m_unSrcUin;			//(���ӵ���)
	ULONG	m_unDstUin;			//(�Ӻ��ѵķ�����)
	SHORT	m_nWordLength;				//���Գ���
	CHAR	m_szWords[MAXWORDSLEN];	//����

};

//Gsvrd֪ͨ����Ӻ�����Ϣ�����
class CNotifyAddFriendResultPara:public CMsgPara
{
	SHORT   m_nResultID;				
	ULONG	m_unSrcUin;			//(�Ӻ��ѵķ�����)
	ULONG	m_unDstUin;			//(���ӵ���)
	SHORT	m_nWordLength;				//���Գ���
	CHAR	m_szWords[MAXWORDSLEN];	//����
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
	ULONG 	m_unIdentity;			//������                
	USHORT 	m_nPlayerID;			//���ID
	STPlayerDBBaseInfo m_stPlayerDBBaseInfo;    //������Ϣ
	USHORT	m_nEquipedItemNum; 
	STPlayerDBItemInfo m_stPlayerItemInfo[MAXITEMNUMPERPLAYER];	//��װ���ĵ�����Ϣ
};

//struct STFindPlayerInfo
//{
//	ULONG	m_unUin;                    //Uin
//	ULONG	m_unIdentity;              //������                
//	CHAR	m_szQQNickname[MAXNICKNAMELEN];     
//	USHORT	m_nPlayerID;               //���ID
//	UCHAR	m_bGender;              //�Ա�
//	UCHAR	m_bLevel;                   //���յȼ�
//	ULONG	m_unTotalExp;            //�ܾ���
//	ULONG	m_unWinNum;                  //ʤ������
//	ULONG	m_unFirstNum;            //��һ������
//	ULONG	m_unTotalNum;                 //��������   
//	USHORT	m_nEquipedItemNum;  
//	STPlayerDBItemInfo m_stPlayerItemInfo[MAXITEMNUMPERPLAYER];     //��װ���ĵ�����Ϣ
//};

struct STPositionInfo
{
	USHORT	m_nStatus;
	LONG	m_iServerID;           //������λ��
	STRoomInfo m_stRoomInfo;
};


class CResponseFindPlayerByQQPara
{
	SHORT	m_nResultID;
	STFindPlayerInfo m_stPlayerInfo;
	UCHAR	m_bIsOnLine;       //�Ƿ����ߣ�δ����0x00/����0x01/����0x02
	STPositionInfo m_stPositionInfo;
	UCHAR	m_bReasonLen;
	CHAR	m_szReason[MAXATTACHCONTENTLEN];
};


//�����
struct NOTIFY_ANTI_BOT
{
	ULONG m_dwUin;                
	SHORT m_nBufferLen;           
	CHAR m_szBuffer[32000];      
};

//�����㿪ʼ��Ϸ
class CRequestStartGamePara
{
	ULONG	m_unUin;	//����Uin
	ULONG	m_unTime;				
};

class CResponseStartGamePara
{
	SHORT	m_nResultID;
	UCHAR	m_bReasonLen;
	CHAR	m_szReason[MAXATTACHCONTENTLEN];
};

//.�������ֱ�֪ͨ������ÿ�������Ϸ��ʼ
class CNotifyGameBeginPara
{
	ULONG	m_unNormalLoadTime;	//���ͻ���Ԥ������������ʱ��(10s�� 
	LONG	m_nMapID;			//��ͼID
	UCHAR	m_szMapHash[MAP_HASH_CODE]; //��ͼHash
	ULONG	m_unGameID;		
	ULONG	m_unSeed;	
	CHAR	m_bCurrentPlayerNum1; //��ǰ�����ڵ������
	ULONG	m_anRaceTrackOrders[MAXSEATNUMINROOM];	//�����ܵ������uin
	SHORT   m_nLap;
	ULONG	m_unPropUpdateInterval;//����ˢ�µ�ʱ����(����)
};

class CNotifyGameOverPara
{
	SHORT m_nResultID;
};

//�ͻ���֪ͨ�����������
class CRequestPrepareReadyPara
{
	ULONG	m_unUin;	
	ULONG	m_unTime;	
};

//������֪ͨ�ȴ�δ������ɵ���ң���ʱ��ʼ
class CNotifyWaitUnReadyPlayersPara
{
	ULONG	m_unWaitLoadTime; //�ȴ�ʱ�� ��20s��
};

//������֪ͨ������ʼ
class CNotifyRaceBeginPara
{
	ULONG	m_unCountDownTime; //������ʼǰ�ĵ���ʱʱ�䣨3�룩
};

//�ͻ���֪ͨ��ɱ��� ������������������
class CRequestPlayerFinishRacePara
{
	ULONG	m_unUin;	
	ULONG	m_unTime;
	ULONG 	m_unFinUin;//����ߵ�uin (�п��ܲ���˭���˭�ϱ�) 
	ULONG 	m_unFinResult;//����߳ɼ����ͻ���ͳ�Ƶ����ʱ�䣬���������ο���
};

//������֪ͨĳplayer��ɱ���
class CNotifyPlayerFinishRacePara 
{
	ULONG m_unUin;
};


//������֪ͨ�����������ǰ����ʱ
class CNotifyCountDownPara
{

	ULONG	m_unCountDownTime;//��������ǰ�ĵ���ʱʱ�䣨10s��
	//�п������������ɱ�����p2p��Ϣ��û�������ң���ʱ����Ҳ�֪��˭�ǹھ��͵�	��ʱ�ˣ����Է�����֪ͨ����ʱ��ͬʱ˳��֪ͨ˭�ǹھ���
	ULONG	m_unUin;//�ھ�Uin
};

//�����ɼ�
struct STRaceScore
{

	ULONG	m_unUin;
	ULONG	m_unFinTime; //��ɱ�����ʱ�䣨����ʾδ��ɣ�
	LONG	m_nTP;						//����Ӿ�����ʹ��
	LONG	m_nMoneyInc;					//�������
	LONG	 m_nTotalMoney;				//������������������
	LONG	m_nExpInc;					//��������
	LONG	m_nTotalExp;					//�������������������	
	LONG	m_nAttrResInc;                                     //δ�������Ե�������������Ϊ����˵������
	LONG	m_nTotalAttrRes;                             //��δ�������Ե�(�������)

	// ����
	//������߽�������"С�Խ���"��
	//û��װ���������ʱm_bAwardNumΪ0��
	//��Ϊ���ֵ��߽���Ч�����ܵ��ӣ�����Ҫ������չ������bitȡ�����⽱��λ��
	//���⽱���������Ϊsizeof(SHORT) * 8 * MAXAWORDSIZE 
	
	CHAR		m_bAwardNum;
	LONG		m_nSpecialAward[MAXAWORDSIZE]; 
};

//������֪ͨ��������,����佱����
class CNotifyRaceOverPara
{
	//��ǰ�����,����;���ߵ���Ҳ���ͳ�ƣ���������ʱ����<=��Ϸ��ʼʱ������ 
	CHAR		m_bCurrentPlayerNum;	  
	//����ҵı����ɼ�,��index�Ѿ��ź��򣬿ͻ��˰����˳����ʾ�ɼ�,��ʾ�佱̨����		��վλ����ʾ�佱̨���������ʱ��վλ��
	//���������������±�0-2��ʾǰ3��
	//���������ʤ������������е�������С���������磺1,4,6,7
	STRaceScore	m_astRaceScore[MAXSEATNUMINROOM];
	CHAR		m_bWinTeamID;	//ʤ��ID����һ���ǹھ����ڶ�Ϊʤ�ӣ�,�������в�������
	LONG		m_iWinType;// ʤ������,��:��ʤ
	ULONG		m_unReceiveHonorTime;
};

class CNotifyRaceShowPara
{
	ULONG		m_unRaceShowTime;			// �յ��� �����ʾʤ����ʧ�ܵı��飬��ʱӦ��ͣ������� ԭ������ RaceOver��ͣ�������
};

class CRequestReportCurrentInfoPara
{
	ULONG	m_unUin;
	ULONG	m_unTime;
	LONG	m_iCurrentStatus;  //���߼��٣�Ư�Ƽ��٣�����(Ԥ��)
	LONG	m_iCurrenRate[3];   //��ά�ٶ� ȡ��
	LONG	m_iCurrentPosition[3]; //��ά����  ȡ��
	UCHAR	m_bPassedCheckPointNum;	//�������ļ���
	USHORT	m_nPassedCheckPointIDs[MAXCHECKPOINTNUM];  //�����id
};


//�ͻ��������ϱ�Ư�ƽ��(����)
class CRequestReportDriftPara
{	
	ULONG	m_unUin;
	ULONG	m_unTime;
	LONG	m_aiSpeedOfDriftStart[3];	//Ư�ƿ�ʼʱ���ٶ�
	LONG	m_aiSpeedOfDriftStop[3];	//Ư�ƽ���ʱ���ٶ�
	LONG	m_aiPositonOfDirftStart[3];	//Ư�ƿ�ʼ������
	LONG	m_aiPositionOfDriftStop[3];	//Ư�ƽ���������
	
	
	//Ư�Ʊ�־λ��Ԥ����,����ͻ���ÿ��Ư�ƶ��ϱ��Ļ���
	//�������������ʾƯ�ƽ�����	ԭ��������������
	//������ײ����������, �����������ǲ��ӵ����ġ�
	

	LONG	m_iDriftFlag;
	LONG	m_iAccelerateFuelInc;	//����Ư�Ƶĵ���ֵ���������������������Աȣ�
};

//�������ظ�Ư��(����)
class CResponseReportDriftPara
{
	SHORT	m_sResultID;
	SHORT	m_sPropID;		//���ٿ��� ID
	LONG	m_iPropIndex;
	LONG	m_iCurrentAccelerateFuel;	//��ǰ����ֵ(����)
	UCHAR	m_bReasonLen; 
	CHAR	m_szReason[MAXATTACHCONTENTLEN];
};






//��õ��ߣ�prop��
class CRequestGetPropPara
{
	ULONG m_unUin;
	ULONG m_unTime;
	SHORT m_sPropPositionNO;			 //����λ�ñ��
	UCHAR m_bCurrentRank;				//��ǰ����
	LONG m_aiCurrentPosition[3];        //��ҵ�ǰλ�ã���ά����( *1000 ȡ��)
	LONG m_iPreviousInterval;           //��ǰһ��ҵľ���
};

//��õ��ߣ�prop��
class CResponseGetPropPara
{
	SHORT m_nResultID;
	ULONG m_unUin;
	SHORT m_sPropID;			//��õ���ID
	LONG  m_iPropIndex;			//���������������������·�
};

//֪ͨ������һ�õ���
class CNotifyGetPropPara
{
	ULONG m_unUin;
	SHORT m_sPropID;              //��õ���ID
	LONG  m_iPropIndex;			//���������������������·�
	SHORT m_sPropPositionNO;     //����λ�ñ��
	
}; 

//ʹ�õ��ߣ�prop��
class CRequestUsePropPara
{
	ULONG m_unUin;
	ULONG m_unTime;
	SHORT m_sPropID;				//����ID
	ULONG m_unDstUin;				//Ŀ�����Uin��
									//����������ĵ���m_unDstUin=m_unUin
									//��ָ������Ŀ��ĵ���m_unDstUin=0
	LONG m_aiCurrentPosition[3];	//��ҵ�ǰλ��
	LONG m_iPropIndex;				//��������������������
	SHORT m_sNewID;					//����ת������id��0��ʾûת��
};
//���������ص�����Ϣ
class CResponseUsePropPara
{
	SHORT m_nResultID;
	ULONG m_unUin;
	SHORT m_sPropID;              //����ID
	ULONG m_unDstUin;
	LONG  m_aiPropPosition[3];  //�����㽶Ƥ�ȵ�λ��
	LONG  m_iPropIndex;        //��������������������
};

//֪ͨ��m_unUin���������,ʹ�õ���
class CNotifyUsePropPara
{
	ULONG m_unUin;
	SHORT m_sPropID;              //����ID
	ULONG m_unDstUin;
	LONG  m_aiPropPosition[3];  //�����㽶Ƥ�ȵ�λ�ã�����ҵ�ǰλ��
	LONG  m_iPropIndex;        //���������������������·�
	SHORT m_sNewID;				//����ת������id��0��ʾûת��
};

//�������ý���ϱ�
class CRequestPropEffectResultPara
{
	ULONG	m_unUin;
	ULONG	m_unTime;
	SHORT	m_sPropID;
	LONG	m_iPropIndex;				//��������������������
	SHORT	m_nEffectResult;			//���ý��,0��ʾ�����У�1��ʾû������, 2��ʾ���������� avtarЧ��
	LONG	m_iItemID;				//���m_nEffectResult = 2�Ļ�����ֵ���������õ�avatar ID
};

//֪ͨ��m_unUin��������ҵ������ý��
class CNotifyPropEffectResultPara
{
	ULONG m_unUin;
	SHORT m_sPropID;
	LONG  m_iPropIndex;        //��������������������
};

//��׼״̬
class CRequestChangeAimStatePara 
{
	ULONG m_unUin;
	ULONG m_unTime;
	ULONG m_unDstUin;
	UCHAR m_bAimState;        //��׼״̬��0ȡ����׼��1��׼
};

//֪ͨm_unDstUin���,��׼״̬
class CNotifyChangeAimStatePara 
{
	ULONG m_unUin;
	ULONG m_unDstUin;
	UCHAR m_bAimState;        //��׼״̬��0ȡ����׼��1��׼
};

//1�����������ͼ
class CRequestChangeMapPara
{
	ULONG	m_unUin;
	ULONG	m_unTime;
	LONG	m_iNewMapID; //�µ�ͼid
};

class CResponseChangeMapPara
{
	SHORT	m_sResultID; //���
	LONG	m_iNewMapID; //�µ�ͼid
	UCHAR	m_bReasonLen;   //ԭ�򳤶ȣ�ʧ��ʱ��
	CHAR	m_szReason[MAXATTACHCONTENTLEN];  //ԭ��
};

class CNotifyChangeMapPara
{
	ULONG	m_unUin;  //������ͼ��Һ���
	LONG	m_iNewMapID; //�µ�ͼid
}; 


//����ı����
class CRequestChangeTeamPara
{
	ULONG	m_unUin;                
	ULONG	m_unTime;               
	UCHAR	m_bNewTeamID;   //Ҫ�ı�ɵĶ���ID        
};

//�ظ��ı����
class CResponseChangeTeamPara
{
	SHORT	m_nResultID;   //����������Ľ��
	UCHAR	m_bNewTeamID;  //�µĶ���ID
	UCHAR	m_bReasonLen;
	CHAR	m_szReason[MAXATTACHCONTENTLEN];
};

//֪ͨ�����ͻ������˸ı����
class CNotifyPlayerChangeTeamPara
{
	ULONG	m_unUin;        //����ı��������Uin
	UCHAR	m_bNewTeamID;   //�ı����µĶ���ID    
};

//֪ͨ�����ı�
class CNotifyChangeRoomOwnerPara
{
	SHORT	m_sRoomID;
	SHORT   m_nNewRoomOwnerID;          //�·���playerID
};

//�����滻��Ϣ
struct STReplacePropInfo
{
	LONG	m_iReplaceFlag;//�滻��־λ,0-���滻;1--�滻
	LONG	m_iPropIndex;
	SHORT	m_sPropID;
};

//��ҵ����滻��Ϣ
struct STRacerReplacePropInfo
{
	ULONG	m_unUin;
	STReplacePropInfo m_astReplaceProp[MAXPROPBLANK];
};

//֪ͨ�滻����
class CNotifyReplacePropPara 
{
	SHORT m_sPlayerNum;
	STRacerReplacePropInfo m_astRacerReplacePropInfo[MAXSEATNUMINROOM];
	LONG	m_iItemID;//�������Ϊavatar ����ĵ����滻,��Avatar�� ID...����,Ĭ��0 
};

//֪ͨ��ӵ���(������ɷ��������������·��ģ����ǿͻ��˼񵽵�)
class CNotifyAddPropPara
{
	ULONG	m_unUin;
	SHORT	m_sPropID;
	LONG	m_iPropIdx;
	LONG	m_iItemID;//�������Ϊavatar ����ĵ������,��Avatar�� ID...����,Ĭ��0 
};

class CNotifyReportDriftPara
{
	LONG m_iTeamAccelerateFule;//���鵪��ֵ
};

struct STItemStatus
{
	LONG    m_iItemID;		//����ID              
	UCHAR 	m_bNewStatus;   //����״̬�������ENMITEMSTATUS
};

class CRequestChangeItemStatus
{
	ULONG	m_unUin;           //qq����
	ULONG	m_unTime;          //ʱ��
	SHORT m_sItemNum;                  //Ҫ�ı�ĵ�������
	STItemStatus m_stItemStatus[MAXCHANGEITEMSTATUSNUM]; //Ҫ�ı�ĵ���
};

class CResponseChangeItemStatus :public CMsgPara
{
	SHORT 	m_nResultID;               //���ؽ����0�ɹ�������ʧ�ܣ�����ԭ����m_szReason��
	SHORT	m_sItemNum;                //�ı�ĵ����������ɹ�ʱʹ��
	STItemStatus m_stItemStatus[MAXCHANGEITEMSTATUSNUM];  //�ı�ĵ��ߣ��ɹ�ʱʹ��
	UCHAR	m_bReasonLen;
	CHAR  m_szReason[MAXATTACHCONTENTLEN];    //ԭ��ʧ��ʱʹ��
};

class CNotifyChangeItemStatusPara
{
	ULONG m_unUin;    //�ı����״̬�����
	SHORT m_sItemNum;
	STPlayerDBItemInfo m_stItemStatus[MAXCHANGEITEMSTATUSNUM]; 
};

//---------------------------------------ע�����Э��------------------------------------
//����������Ե���
class CRequestAssignAttributePara :public CMsgPara
{
	ULONG	m_unUin;           //qq����
	ULONG	m_unTime;          //ʱ��
	STPlayerDBAttributeInfo m_stPlayerDBAttributeInfo;        //�����ĵ����ֲ����������Ե��޸�
};
//��Ӧ�������
class CResponseAssignAttributePara :public CMsgPara
{
	SHORT 	m_nResultID;               //���ؽ����0�ɹ�������ʧ�ܣ�����ԭ����m_szReason��
	STPlayerDBAttributeInfo m_stPlayerDBAttributeInfo;        //��ǰ�ĵ����ֲ����ɹ�ʱʹ��
	UCHAR	m_bReasonLen;
	CHAR	m_szReason[MAXATTACHCONTENTLEN]; 
};

//����ϴ��
class CRequestClearAttributePara :public CMsgPara
{
	ULONG	m_unUin;           //qq����
	ULONG	m_unTime;          //ʱ��
};
//��Ӧϴ��
class CResponseClearAttributePara :public CMsgPara
{
	SHORT 	m_nResultID;               //���ؽ����0�ɹ�������ʧ�ܣ�����ԭ����m_szReason��
	STPlayerDBAttributeInfo m_stPlayerDBAttributeInfo;        //��ǰ�ĵ����ֲ����ɹ�ʱʹ��
	UCHAR	m_bReasonLen;
	CHAR	m_szReason[MAXATTACHCONTENTLEN]; 
};

//������������Ч��
class CRequestCheckNamePara :public CMsgPara
{
	ULONG	m_unUin;           //qq����
	ULONG	m_unTime;          //ʱ��
	CHAR  m_szNickname[MAXQQKARTNICKNAMELEN];  	//��ҵ��ǳ�
};
//���ؼ����
class CResponseCheckNamePara :public CMsgPara
{
	SHORT 	m_nResultID;               //���ؽ����0�ɹ�������ʧ�ܣ�����ԭ����m_szReason��
	CHAR	m_szNickname[MAXQQKARTNICKNAMELEN];  	//��ҵ��ǳ�
	UCHAR	m_bReasonLen;
	CHAR	m_szReason[MAXATTACHCONTENTLEN]; 
};

//
// ����Ʒ
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
// ����Ʒ�б�
//
class CRequestGetCommodityListPara :public CMsgPara
{
	ULONG m_unUin;
	ULONG m_unTime;
};

class CResponseGetCommodityListPara :public CMsgPara
{
	SHORT m_shResultID;
	CHAR m_cEndFlag; //��Ʒ�б�ᳬ��һ�����ݰ��ĳ���,����Ҫ��ɼ����������ͣ��˱�־Ϊ0��ʾ��δ������1��ʾ������
	SHORT m_shCommodityNum;
	STCommodity m_astCommodity[MAX_COMMODITY_NUM];
	USHORT m_ushReasonLen;
	CHAR m_szReason[MAX_REASON_LEN];
};

//GameSvrd֪ͨclient���ӵ���
class CNotifyClientAddItemPara :public CMsgPara
{
	ULONG	m_unUin;                               
	ULONG	m_unSrcUin; 	//0��ʶ�����ͣ���> 0 ���ʾΪ�������uin
	SHORT	m_sItemNum;
	STPlayerDBItemInfo	m_stItemInfo[MAXITEMNUMONCEADD];	
	SHORT	m_sAttachLen;
	UCHAR	m_szAttachInfo[MAXATTACHINFOLEN];
};


//dirЭ��///////////////////////////////////////////////////
//���ط���������
struct STDownloadServerInfo
{
	ULONG m_uiIPAddr;             //ip��ַ
	USHORT m_ushPort;            //�˿ں�
	USHORT m_ushLocationID;       //IDC id
};
//Ŀ¼��������Ϣ
struct  STDirServerInfo
{
	LONG m_iServerID;                    //id��
	USHORT m_ushLocationID;      //IDC id
	ULONG m_uiServerIP;           //ip��ַ
	USHORT m_aushServerPort[MAX_DIR_LISTEN_PORT_NUM];  //�����˿ں�
};
//��������
struct STWorldBaseInfo
{
	UCHAR m_byWorldID;         //id��
	UCHAR m_byNameLen;    //���ֳ���    
	CHAR m_szName[MAX_WORLD_NAME_LEN];  //����
	UCHAR m_byDirNum;     //Ŀ¼����������
	STDirServerInfo m_astDirServerInfo[MAX_WORLD_DIR_SERVER_NUM]; //dir������
};
//������������Ϣ
struct  STServerBaseInfo
{
	LONG m_iServerID;                     //id��
	USHORT m_ushLocationID;        //IDC id
	ULONG m_uiServerIP;              //ip��ַ
	USHORT m_ushServerPort;         //�˿ں�
};
//��Ϸ��������Ϣ
struct  STGameServerInfo
{
	LONG m_iServerID;                     //server id��
	ULONG m_uiServerIP;             //ip��ַ
	USHORT m_ushTcpPort;           //tcp�˿ں�
	USHORT m_ushUdpPort;           //udp�˿ں�
	USHORT m_ushMaxPlayerNum;     //���ɳ�������
	USHORT m_ushCurrentPlayerNum;   //��ǰ��������
};

//ͨ����Ϸ��������Ϣ
struct  STCommGameServerInfo
{
	STGameServerInfo m_stServerInfo;
	USHORT m_ushLocationID;        //IDC id
};

//idc��Ϣ
struct STIDCServerInfo
{
	USHORT m_ushLocationID;  //IDC id
	UCHAR m_byGameServerNum;   //��Ϸ����������
	STGameServerInfo m_astGameServerInfo[MAX_IDC_GANE_SERVER_NUM];  //��Ϸ��Ϣ
};
//Ƶ����Ϣ
struct STChannelInfo
{
	UCHAR m_byChannelID;  //Ƶ��id��
	UCHAR m_byIDCNum;   //IDC����
	STIDCServerInfo m_astIDCServerInfo[MAX_IDC_SERVER_NUM];  //idc��Ϣ
};
//������Ϣ
struct STCountryInfo
{
	UCHAR m_byCountryID; //����id��
	UCHAR m_byChannelNum;   //Ƶ������
	STChannelInfo m_astChannelInfo[MAX_COUNTRY_CHANNEL_NUM];  //Ƶ����Ϣ
};

//Client ��ȡworld��Ϣ
class CRequestGetWorldInfoPara
{
	ULONG m_uiUin;                
	ULONG m_uiTime;               
	ULONG m_uiClientVersion;       //�ͻ��˰汾�ţ����ڿ�������
	ULONG m_uiMasterVersion;      //���汾�ţ�����ǰ̨����
	ULONG m_uiSlaveVersion;       //�ΰ汾�ţ����ں�̨����
	UCHAR m_byWorldID;        //��Ҫ��ȡ������
	UCHAR m_szFileHash[HASH_CODE_LENGTH]; //������У����
};

//����world��Ϣ
class CResponseGetWorldInfoPara
{
	USHORT m_ushResultID;         //���ENMDirLoginResult
	ULONG m_uiLastClientVersion;   //���¿ͻ��˰汾��
	ULONG m_uiLastMasterVersion; //�������汾��
	ULONG m_uiLastSlaveVersion;  //���´ΰ汾��
	UCHAR m_byDownloadServerNum;     
	STDownloadServerInfo m_astDownloadServerInfo[MAX_DOWNSVR_NUMBER];     //���ط���������
	UCHAR m_byRootDirectionLen;      
	CHAR m_szRootDirection[MAX_DIRECTION_LENGTH]; //���ظ�·��
	ULONG	m_uiMasterFullVersion;	//���ط��������µ�ǰ̨�����������汾��
	ULONG	m_uiSlaveFullVersion;	//���ط��������µĺ�̨�����������汾��
	//UCHAR m_byMasterFileNameLen;
	//CHAR  m_szMasterFileName[MAX_FILE_NAME_LENGTH]; //���汾�����ļ���
	//UCHAR m_bySlaveFileNameLen;
	//CHAR  m_szSlaveFileName[MAX_FILE_NAME_LENGTH];  //�ΰ汾�����ļ���
	USHORT m_ushP2POption;           //p2p���ã�0��ʾ��������Ժ���Ҫ�ر�p2p���ط����������������չ
	UCHAR m_byWorldCount;
	STWorldBaseInfo m_astWorldBaseInfo[MAX_WORLD_NUM]; //ȫ��world��Ϣ
	UCHAR m_byCountryCount;         
	STCountryInfo m_astCountryInfo[MAX_COUNTRY_NUM];    //������Ϣ
	UCHAR m_byShopServerCount;         
	STServerBaseInfo m_astShopServerInfo[MAX_SHOP_SERVER_NUM]; //�̵��������Ϣ
	UCHAR m_byBorderServerCount;         
	STCommGameServerInfo m_astBorderServerInfo[MAX_BORDER_SERVER_NUM];          //�߾���������Ϣ
	UCHAR m_byMatchServerCount;         
	STCommGameServerInfo m_astMatchServerInfo[MAX_MATCH_SERVER_NUM];          //������������Ϣ
	UCHAR m_bNetAdminServerCount;         
	STServerBaseInfo m_astNetAdminServerInfo[MAX_NETADMIN_SERVER_NUM];          //���ܷ�������Ϣ
	USHORT m_ushReasonLen;
	CHAR m_szReason[MAX_REASON_LEN];      //����
};


//����״̬/������Ϣ
struct STTaskStatusInfo
{
	LONG m_iTaskID;
	CHAR m_cStatus;			//����״̬��0δ��ɣ�1��� 
	LONG m_iProgressRate;    //��ǰ��ɻ��ռ������������ڱ�ʶ�������
	LONG m_iOther;			//���ã����ڱ�ʶ�������
};

//��������Ϣ
struct STTaskAward
{
	LONG m_iAwardMoney;			//��ҽ���
	LONG m_iTotalMoney;			//�������
	LONG m_iAwardExp;           //���齱��
	LONG m_iTotalExp;           //��������
	LONG m_iAwardItemNum;
	STPlayerDBItemInfo m_astAwardItemInfo[MAXAWARDITEMNUM];   //���߽���
};

//����ǰ������״̬
struct STTaskPrevStatusInfo
{
	LONG m_iTaskID;
	UCHAR m_bPrevStatus;    //ǰ������״̬
};


class CNotifySingleGameBeginPara
{
	ULONG m_uiNormalLoadTime;			//���ͻ���Ԥ������������ʱ��(10s��
	LONG m_iTaskID;						// ����id
	LONG m_iMapID;						//��ͼID
	UCHAR m_szMapHash[MAP_HASH_CODE];   //��ͼHash
	ULONG m_uiGameID;		
	ULONG m_uiSeed;
	SHORT m_shTotalMapRound;			//Ȧ��
	ULONG m_uiTimeLimit;				//ʱ������
};

class CNotifySingleRaceBeginPara
{
	ULONG m_uiCountDownTime; //������ʼǰ�ĵ���ʱʱ�䣨3�룩
};

class CNotifySingleRaceFinishPara
{
	USHORT m_shResultID;      //0��ʾ���, ������ʾ��ʱ
};

class CNotifySingleRaceOverPara
{
	ULONG m_uiUin;
	STTaskStatusInfo m_stTaskStatusInfo;
	ULONG m_uiFinTime;					//ʹ��ʱ��
	STTaskAward m_stTaskAward;
};

class CNotifySingleGameOverPara
{
	SHORT m_shResultID;   //���
};

class CRequestStartSingleTaskPara
{
	ULONG m_uiUin;
	ULONG m_uiTime;      //ʱ��
	LONG m_iTaskID;      //����ID
	LONG m_iMapID;       //��ͼID
};

class CResponseStartSingleTaskPara
{
	SHORT m_shResultID;
	ULONG m_uiUin;
	LONG m_iTaskID;       //����ID
	LONG m_iMapID;
	UCHAR m_bReasonLen;
	CHAR m_szReason[MAXATTACHCONTENTLEN];
};

class CRequestReportCurrentOptPara
{
	ULONG m_uiUin;
	ULONG m_uiTime;      //ʱ��
	LONG m_iOptID;       //����ID
};

class CRequestGetTaskListPara
{
	ULONG m_uiUin;
	ULONG m_uiTime;     //ʱ��
};

class CResponseGetTaskListPara
{
	SHORT m_shResultID;
	ULONG m_uiUin;
	SHORT m_shTaskNum;
	STPlayerDBTaskInfo m_astPlayerDBTaskInfo[MAXTASKNUMPERPLAYER]; //��������б�
	UCHAR m_bReasonLen;
	CHAR m_szReason[MAXATTACHCONTENTLEN];
};


class CRequestTakeTaskPara
{
	ULONG m_uiUin;
	ULONG m_uiTime;			//ʱ��
	LONG m_iTaskID;         //����ID
	SHORT m_shTakeOptionID; //�������������ѡ��ID
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
	ULONG m_uiTime;          //ʱ��
	LONG m_iTaskID;          //����ID
};

class CResponseDeleteTaskPara
{
	SHORT m_shResultID;
	ULONG m_uiUin;
	LONG m_iTaskID;           //����ID 
	UCHAR m_bReasonLen;
	CHAR m_szReason[MAXATTACHCONTENTLEN];
};

class CRequestSkipTaskPara
{
	ULONG m_uiUin;
	ULONG m_uiTime;          //ʱ��
	LONG m_iTaskID;          //����ID
};

class CResponseSkipTaskPara
{
	SHORT m_shResultID;
	ULONG m_uiUin;
	LONG m_iTaskID;                  //����ID 
	STTaskAward m_stTaskAward;
	UCHAR m_bReasonLen;
	CHAR m_szReason[MAXATTACHCONTENTLEN];
};

class CNotifyClientUpdateMoneyPara 
{
	ULONG m_uiUin;
	LONG m_iCurrentMoney;   //���º�Ŀ������
};

class CNotifyTaskAwardPara
{
	ULONG m_uiUin;
	LONG m_iTaskID;
	STTaskAward m_stTaskAward;
};