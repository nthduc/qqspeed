/******************************************************************************
   ��Ȩ���� (C), 2001-2011, ��Ѹ�����ϵͳ���޹�˾

/******************************************************************************
  �ļ���          : httpdown.h
  �汾��          : 1.0
  ����            : �߷�
  ��������        : 2003-5-26
  ����޸�        : Fishhong ������QQ�����������
  ��������        : ʵ��HTTP�����ع���
  �����б�        : 
  �޸���ʷ        : 
  1 ����          : 2003-5-26
    ����          : �߷�
    �޸�����      : ����
*******************************************************************************/
#ifndef _DOWNLOAD_H_
#define _DOWNLOAD_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/*�������ļ�*/
#define DOWNLOAD_NEW        0
/*�ϵ�����*/
#define DOWNLOAD_CONTINUE   1
#define IP_LENGTH           20
#define MAX_BUFFER_LEN      1024
/*�������и��ļ�*/
#define REPLY_OK_1          "HTTP/1.0 200"
#define REPLY_OK_2          "HTTP/1.1 200"
/*�ϵ�����*/
#define REPLY_OK_3          "HTTP/1.0 206"
#define REPLY_OK_4          "HTTP/1.1 206"

/*��ʵ�ļ���С*/
#define FILE_LEN_STRING     "Content-Length:"

/*���سɹ�*/
#define DOWNLOAD_SUCCESS    0
/*����ʧ��*/
#define DOWNLOAD_FAILED     1
/*���ؽ�����*/
#define DOWNLOAD_PROGRESS   2
/*���ӷ�����ʧ��*/
#define ERR_CONNECTSERVER   3
/*�򿪱����ļ��ɹ�*/
#define OPEN_FILE_SUCCESS   4
/*���������ļ�ʧ��*/
#define ERR_CREATELOCALFILE 5
/*�򿪱����ļ�ʧ��*/
#define ERR_OPENLOCALFILE   6
/*HTTP������û�д����ص��ļ�*/
#define ERR_REFILENOTEXIST  7
/*����socketʧ��*/
#define ERR_SOCKET          8
/*û������������INDEX����Ϣ*/
#define ERR_NO_THIS_INDEX   9

#define THREAD_NOT_START    0 /*�߳���δ����*/
#define THREAD_RUNNING      1 /*�߳���������*/
#define THREAD_EXIT         2 /*�߳��˳�*/

#define RECV_DATA_OVER			0
#define HEAD_DATA_TYPE			1
#define FILE_DATA_TYPE			2

/*��ʱ���ã��ͻ��������ʱ����û���յ�IP��������������*/
#define DOWNLOAD_TIMEOUT            60
#define MAX_RECV_LEN 50 * 1024

typedef struct tagHTTPGETDATA
{
	unsigned char chUrl[ 512 ];
	unsigned char chLocalFile[ 256 ];
	BOOL bIsContinue;
	BOOL bAutoDel;
}HTTPGETDATA;

class CHttpDown : public CAsyncSocket
{
public:
	#define DOWN_TIMEOUTCOUNT        30
	#define CHECK_DOWNTIMEOUT        2
public:

	DWORD          m_dwNumber;
	BOOL           m_bAutoDel;	//������Ϻ��Ƿ��Զ�ɾ��
	short          m_shNowRcvDataType;
	int            m_iRecvIndex;
	char           *m_pDestIP;
	BOOL           m_bHaveClose;
	BOOL           m_bHaveCancelDown;
	BOOL           m_bCanDelete;
	int            m_iDownTimeout;	

	virtual void OnConnect(int nErrorCode);
	virtual void OnReceive(int nErrorCode);		
	void SetDownloadResult(int iResult);	
	/*���캯��*/
	CHttpDown(const HTTPGETDATA* data);
	/*�鹹����*/
    virtual ~CHttpDown();
	/*�ṩ��������Ľӿ�*/
	short DownloadFile(CString strUrl,CString strLocalFile,BOOL bIsContinue = FALSE);
	/*������ؽ��
	�������ṩ�Ľӿڷ���ֵ�����ǣ�
	
	���ظ��ֶα�ʾ�������ڽ���	
	DOWNLOAD_PROGRESS   ���ؽ�����
	
	���������ֶα�ʾ�����Ѿ�����
	DOWNLOAD_SUCCESS    ���سɹ�
	DOWNLOAD_FAILED     ����ʧ��
	ERR_CONNECTSERVER   �������ط�����ʧ��	
	ERR_OPENLOCALFILE   �򿪱����ļ�ʧ�ܣ������ļ��Ѿ��򿪵��µ�ʧ��
	ERR_CREATELOCALFILE ���ɱ����ļ�ʧ��
	ERR_REFILENOTEXIST  ������ļ��ڷ������ϲ�����	
	ERR_SOCKET          ����Socketʧ��		
	*/
    int     GetDownloadResult();
	/*����ļ���С*/
    int     GetFileSize();
	/*�����Ѿ����صĴ�С*/
    int     GetDownloadSize();
	/*�������صĽ��̣��ٷֱȣ�*/
    double  GetDownloadProgress();
	/*�ر�socket*/
    int     CloseSocket();
	//ȡ������
	BOOL	CancelDown();
	BOOL    CheckTimeout();
	void    SetDeleteDownFlag(BOOL bDeleteFlag);
	BOOL    GetDeleteDownFlag();
	DWORD   GetDownIndex();
	BOOL    GetAutoDelFlag();

private:	
	int     OpenTmpLocFile();  //����ʱ�����ļ�    
	void    CloseFile();       /*�رմ򿪵��ļ�*/	

public:
	/*�����ļ���*/
	char                m_szLocalFile[MAX_PATH];
	/*Զ���ļ���*/
	char                m_szRemoteFile[MAX_PATH];
	//��ʱ�ļ���
	char                m_szTempLocalFile[MAX_PATH];
private:
	/*������IP*/
    char                m_szServerIP[IP_LENGTH];
	/*�Ƿ�ϵ�����*/
    ULONG               m_ulIsContinue;
	/*������Port*/
    USHORT              m_usServerPort;
	/*�����صĴ�С����һ�����ļ����ȣ�*/
    int                 m_iFileSize;
	/*�Ѿ����صĴ�С*/
    int                 m_iDownloadSize;
    /*�����ļ����*/
    FILE               *m_pFile;
	/*��ʵ������*/
    struct sockaddr_in  m_sockServer;
	/*�ϴ����صĴ�С�����ڶϵ�������*/
    int                 m_iLastDownloadSize;
    char                m_szBuffer[MAX_RECV_LEN];
    char                m_szErrBuf[MAX_PATH];
	/*���ؽ��*/
    int                 m_iDownloadResult;
	//�Ƿ���Server���������� 
	BOOL m_bConnected;		

	//��ʱ����
	int m_iTimeoutCount;
};

#endif /* __DOWNGAME_H__ */
