#pragma once

#include <windows.h>

#define NS_USING_ANTIBOT using namespace AntiBot;

namespace AntiBot {
	
#define SC_AB_SEND_ANTIBOT		1	//���Ͷ�̬������ͻ���
#define SC_AB_RAND_CHECK		2	//�����ʱ֪ͨ���
#define CS_AB_INIT_INFO			3	//�ͻ�����Ҫ�ĳ�ʼ��Ϣ
#define CS_AB_VERIFY_RESULT		4	//�ͻ����յ�ʱ��󷵻���֤����������
	
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
	int nPassive;				//1 �ͻ�������; 0 ���������
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

//! Э�����ݰ��ṹ��ʹ��CABMsgData���б����

typedef struct tagABData	//��̬�����cp������Э��,allan����
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

typedef struct tagRandParam		//���������check������
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

/*! ʹ��˵����
	ʹ�÷���̳�INotifySink�ӿڣ�sdkͨ���ص��ýӿ�ʵ�ַ����������ʹ�÷�
	ͨ��ʹ�÷���Ҫ������ͨ��socket�ϱ������

	��Ҫע�⣺
	sdkʹ�õ����߳�ִ�м�鹤�����ص����ڹ����߳���ִ�У�����ص���ֻ������copy������
	socket���͵ȹ����ŵ����߳�ִ��
*/


/*! ��Ϸ�ͻ�����Ҫʵ�ֵĻص��ӿڣ���̬����ִ���ǵ������̣߳������ڴ˻ص�
	��Ϸ�ͻ��˵ĺ������ڴ˿���ִ���ϱ���ȡ��ȥ���ͻ�����Ϣ�Ĺ���
*/
class INotifySink
{
public:
	/*!	ȡ�ý������Ҫ�ص�ʹ�÷�����Ϣ�Ѽ����ϱ�����
		\param pbyResult ������CC_PACKET���ֽ���
		\param dwSize �����Ľ������
	*/
	virtual int OnCheckResult(BYTE *pbyResult, DWORD dwSize) = 0;

	DWORD GetSvrId() {return m_dwSvrId;}
	void SetSvrId(DWORD dwSvrId) {m_dwSvrId = dwSvrId;}
protected:
	DWORD m_dwSvrId;
};

//! ���ݷ���˱���Ҫ�����շ�����ʱ���ö�Ӧ�������а������
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
	��̬����ִ�б���SDK�ӿ�
*/
class IAntiBot
{
public:
	IAntiBot() {}
	virtual ~IAntiBot() {}
	
	/*! ���ü�����ص�������svr��Sink�ӿ�
		ÿ����һ����������Addһ��sink�ӿڣ��˹�����Ҫ���QQGame�����½��̨mainsvr
		��Ҫ��fo�ȵ�½һ̨svr��Ҳ��ҪAddһ�Σ��ر�ǰDel�ýӿ�
	*/
	virtual int AddNotifySink(INotifySink *pSink) = 0;
	virtual int DelNotifySink(INotifySink *pSink, bool bFreeMem) = 0;

	//! ������Ϸ�ͻ��˰汾��
	virtual void SetGameVer(DWORD dwGameVer) = 0;

	/*! ������Ч��̬���벢��ѹ����øú���
		\param pbyRCData ��̬����buffer�ĵ�ַ
		\param dwSize ��̬����buffer�ĳ���
		\param pbyVer �汾��(16�ֽ�)
		\param nRCId ���к�
	*/
	virtual int OnRcvRCFinish(BYTE *pbyRCData, DWORD dwSize, BYTE *pbyVer, int nRCId) = 0;

	/*! ������Чcheckpoint�������ݺ����
		\param pbyCPData ����buffer��ַ
		\param dwSize ����buffer�ĳ���
		\param pbyVer �汾��(16�ֽ�)
		\param nRCId ���к�
	*/
	virtual int OnRcvCPFinish(BYTE *pbyCPData, DWORD dwSize, BYTE *pbyVer, int nRCId) = 0;

	/*! server��ʱ֪ͨcheck�ǵ��ã�ִ�ж�̬������
		\param dwSvrId ��½��svr�ı��
		\param pbyRand ����·��������
	*/
	virtual int OnRandCheck(DWORD dwSvrId, BYTE *pbyRand) = 0;
	
	/*! ��Ϸ������ȡ��api��ַ�Ͷ�̬�������ַ��ͨ��Э�鷢��server��
		\param ppbyInitInfo �ڲ������ڴ棬ע��ʹ�ú���Ҫ�ͷ�
		\param dwSize ��һ������buffer�ĳ��ȣ���λΪ�ֽ�
	*/
	virtual void GetInitInfo(PREV_DATA *pstPrevData) = 0;

	//! �رձ��ؼ�ⶨʱ��
	virtual void StopLocalCheck(bool bFreeRes) = 0;

	//! ����ͷ�AntiBot�ڴ�
	virtual void Release() = 0;
};

//! ����AntiBot�ӿڵ�һʵ�������صĶ�����Ҫʹ��Release()�ͷ�
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