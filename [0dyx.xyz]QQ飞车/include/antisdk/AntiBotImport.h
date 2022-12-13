#pragma once

#include <windows.h>

#define NS_USING_ANTIBOT using namespace AntiBot;

namespace AntiBot {
	
#define SC_AB_SEND_ANTIBOT		1	//发送动态代码给客户端
#define SC_AB_RAND_CHECK		2	//随机或定时通知检测
#define CS_AB_INIT_INFO			3	//客户端需要的初始信息
#define CS_AB_VERIFY_RESULT		4	//客户端收到时间后返回验证包给服务器
	
#define MD5_HASH_LEN	16			

#pragma pack(push, 1)
#define MAX_PROC_NAME	255	
typedef struct tagScanResult
{
	DWORD dwGameId;					
	DWORD dwBotId;					
	short shProcLen;				
	char szProcName[MAX_PROC_NAME];
} RC_SCAN_RESULT;

typedef struct tagCCPacket
{
	BYTE byStamp[MD5_HASH_LEN];	
	BYTE byModule[MD5_HASH_LEN];
	BYTE byScan[MD5_HASH_LEN];	
	RC_SCAN_RESULT stScan;
	int nPassive;				//1 客户端主动; 0 服务端主动
} CC_PACKET;

typedef struct tagPrevData
{
	BYTE byVer[MD5_HASH_LEN];	
	DWORD dwModuleHandle;	
	DWORD dwProcAddress;		
	DWORD dwBase;			
	int nOsVer;					
	DWORD dwClientVer;			
	int nRCID;
} PREV_DATA;

//! 协议数据包结构，使用CABMsgData进行编解码

typedef struct tagABData	//动态代码或cp的数据协议,allan定义
{
	short shProtoId;
	BYTE byVer[MD5_HASH_LEN];
	int nHasData;
	int nHasRCData;
	int nHasCPData;
	int nRCID;
	short shDataSize;
	BYTE *pbyData;
} AB_DATA;

typedef struct tagRandParam		//服务端主动check的数据
{
	short shProtoId;
	BYTE byRand[MD5_HASH_LEN];
} AB_RAND_PARAM;

typedef struct tagInitInfo
{
	short shProtoId;
	PREV_DATA stPrevData;
} AB_INIT_INFO;

typedef struct tagVerifyResult
{
	short shProtoId;
	DWORD dwUin;
	CC_PACKET stPacket;
} AB_VERIFY_RESULT;

#pragma pack(pop)

/*! 使用说明：
	使用方需继承INotifySink接口，sdk通过回调该接口实现反馈检查结果给使用方
	通常使用方需要做的是通过socket上报检查结果

	需要注意：
	sdk使用单独线程执行检查工作，回调是在工作线程内执行，建议回调内只把数据copy出来，
	socket发送等工作放到主线程执行
*/


/*! 游戏客户端需要实现的回调接口，因动态代码执行是单独的线程，所以在此回调
	游戏客户端的函数，在此可以执行上报和取得去他客户端信息的工作
*/
class INotifySink
{
public:
	/*!	取得结果后需要回调使用方的信息搜集或上报函数
		\param pbyResult 反馈的CC_PACKET的字节流
		\param dwSize 反馈的结果长度
	*/
	virtual int OnCheckResult(BYTE *pbyResult, DWORD dwSize) = 0;

	DWORD GetSvrId() {return m_dwSvrId;}
	void SetSvrId(DWORD dwSvrId) {m_dwSvrId = dwSvrId;}
protected:
	DWORD m_dwSvrId;
};

//! 根据服务端编码要求在收发数据时调用对应函数进行包编解码
class CABDataMsg
{
public:
	CABDataMsg();
	virtual ~CABDataMsg();
		
	short DecodeProtolId(BYTE *pbyBuf);

	void DecodeDataMsg(BYTE *pbyBuf, AB_DATA *pstABData);
	void DecodeRandParam(BYTE *pbyBuf, AB_RAND_PARAM *pstRandParam);
	void EncodeInitInfo(AB_INIT_INFO *pstInitInfo, BYTE *pbyInitInfo);
	DWORD EncodeVerifyResult(AB_VERIFY_RESULT *pstResult, BYTE *pbyResult);
	
private:
	DWORD EncodeCCPacket(CC_PACKET stPacket, BYTE *pbyCCPacket);
	DWORD EncodeScanResult(RC_SCAN_RESULT stScanResult, BYTE *pbyScanResult);		
};

/*! 
	动态代码执行本地SDK接口
*/
class IAntiBot
{
public:
	IAntiBot() {}
	virtual ~IAntiBot() {}
	
	/*! 设置检查结果回调反馈给svr的Sink接口
		每增加一个服务器需Add一个sink接口，此功能主要针对QQGame类需登陆多台mainsvr
		需要，fo等登陆一台svr的也需要Add一次，关闭前Del该接口
	*/
	virtual int AddNotifySink(INotifySink *pSink) = 0;
	virtual int DelNotifySink(INotifySink *pSink, bool bFreeMem) = 0;

	//! 设置游戏客户端版本号
	virtual void SetGameVer(DWORD dwGameVer) = 0;

	/*! 接收有效动态代码并解压后调用该函数
		\param pbyRCData 动态代码buffer的地址
		\param dwSize 动态代码buffer的长度
		\param pbyVer 版本号(16字节)
		\param nRCId 序列号
	*/
	virtual int OnRcvRCFinish(BYTE *pbyRCData, DWORD dwSize, BYTE *pbyVer, int nRCId) = 0;

	/*! 接收有效checkpoint检查点数据后调用
		\param pbyCPData 检查点buffer地址
		\param dwSize 检查点buffer的长度
		\param pbyVer 版本号(16字节)
		\param nRCId 序列号
	*/
	virtual int OnRcvCPFinish(BYTE *pbyCPData, DWORD dwSize, BYTE *pbyVer, int nRCId) = 0;

	/*! server定时通知check是调用，执行动态代码检测
		\param dwSvrId 登陆的svr的编号
		\param pbyRand 随机下发的随机串
	*/
	virtual int OnRandCheck(DWORD dwSvrId, BYTE *pbyRand) = 0;
	
	/*! 游戏启动后取得api地址和动态代码基地址，通过协议发回server端
		\param ppbyInitInfo 内部分配内存，注意使用后需要释放
		\param dwSize 第一个参数buffer的长度，单位为字节
	*/
	virtual void GetInitInfo(PREV_DATA *pstPrevData) = 0;

	//! 关闭本地检测定时器
	virtual void StopLocalCheck(bool bFreeRes) = 0;

	//! 最后释放AntiBot内存
	virtual void Release() = 0;
};

//! 创建AntiBot接口单一实例，返回的对象需要使用Release()释放
IAntiBot* CreateAntiBot();

#ifndef NBS_ONE_LIB
#ifdef INJECT_CODE

#ifdef _DEBUG
#pragma comment(lib, "AntiBotSdkjd.lib")
#else
#pragma comment(lib, "AntiBotSdkj.lib")
#endif

#else

#ifdef _DEBUG
#pragma comment(lib, "AntiBotSdkd.lib")
#else
#pragma comment(lib, "AntiBotSdk.lib")
#endif

#endif
#endif//NBS_ONE_LIB

}//end namespace antibot