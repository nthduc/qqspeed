/******************************************************************************
   版权所有 (C), 2001-2011, 腾迅计算机系统有限公司

/******************************************************************************
  文件名          : httpdown.h
  版本号          : 1.0
  作者            : 高峰
  生成日期        : 2003-5-26
  最近修改        : Fishhong 把它从QQ堂中整理出来
  功能描述        : 实现HTTP的下载功能
  函数列表        : 
  修改历史        : 
  1 日期          : 2003-5-26
    作者          : 高峰
    修改内容      : 生成
*******************************************************************************/
#ifndef _DOWNLOAD_H_
#define _DOWNLOAD_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/*下载新文件*/
#define DOWNLOAD_NEW        0
/*断点续传*/
#define DOWNLOAD_CONTINUE   1
#define IP_LENGTH           20
#define MAX_BUFFER_LEN      1024
/*服务器有该文件*/
#define REPLY_OK_1          "HTTP/1.0 200"
#define REPLY_OK_2          "HTTP/1.1 200"
/*断点续传*/
#define REPLY_OK_3          "HTTP/1.0 206"
#define REPLY_OK_4          "HTTP/1.1 206"

/*标实文件大小*/
#define FILE_LEN_STRING     "Content-Length:"

/*下载成功*/
#define DOWNLOAD_SUCCESS    0
/*下载失败*/
#define DOWNLOAD_FAILED     1
/*下载进行中*/
#define DOWNLOAD_PROGRESS   2
/*连接服务器失败*/
#define ERR_CONNECTSERVER   3
/*打开本地文件成功*/
#define OPEN_FILE_SUCCESS   4
/*建立本地文件失败*/
#define ERR_CREATELOCALFILE 5
/*打开本地文件失败*/
#define ERR_OPENLOCALFILE   6
/*HTTP服务器没有待下载的文件*/
#define ERR_REFILENOTEXIST  7
/*创建socket失败*/
#define ERR_SOCKET          8
/*没有所请求下载INDEX的信息*/
#define ERR_NO_THIS_INDEX   9

#define THREAD_NOT_START    0 /*线程尚未启动*/
#define THREAD_RUNNING      1 /*线程正在运行*/
#define THREAD_EXIT         2 /*线程退出*/

#define RECV_DATA_OVER			0
#define HEAD_DATA_TYPE			1
#define FILE_DATA_TYPE			2

/*超时设置，客户端在这段时间内没有收到IP包将会重新下载*/
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
	BOOL           m_bAutoDel;	//下载完毕后是否自动删除
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
	/*构造函数*/
	CHttpDown(const HTTPGETDATA* data);
	/*虚构函数*/
    virtual ~CHttpDown();
	/*提供给其它类的接口*/
	short DownloadFile(CString strUrl,CString strLocalFile,BOOL bIsContinue = FALSE);
	/*获得下载结果
	给大厅提供的接口返回值可能是：
	
	返回该字段表示下载正在进行	
	DOWNLOAD_PROGRESS   下载进行中
	
	返回以下字段表示下载已经结束
	DOWNLOAD_SUCCESS    下载成功
	DOWNLOAD_FAILED     下载失败
	ERR_CONNECTSERVER   连接下载服务器失败	
	ERR_OPENLOCALFILE   打开本地文件失败，本地文件已经打开导致的失败
	ERR_CREATELOCALFILE 生成本地文件失败
	ERR_REFILENOTEXIST  请求的文件在服务器上不存在	
	ERR_SOCKET          创建Socket失败		
	*/
    int     GetDownloadResult();
	/*获得文件大小*/
    int     GetFileSize();
	/*或者已经下载的大小*/
    int     GetDownloadSize();
	/*或者下载的进程（百分比）*/
    double  GetDownloadProgress();
	/*关闭socket*/
    int     CloseSocket();
	//取消下载
	BOOL	CancelDown();
	BOOL    CheckTimeout();
	void    SetDeleteDownFlag(BOOL bDeleteFlag);
	BOOL    GetDeleteDownFlag();
	DWORD   GetDownIndex();
	BOOL    GetAutoDelFlag();

private:	
	int     OpenTmpLocFile();  //打开临时下载文件    
	void    CloseFile();       /*关闭打开的文件*/	

public:
	/*本地文件名*/
	char                m_szLocalFile[MAX_PATH];
	/*远程文件名*/
	char                m_szRemoteFile[MAX_PATH];
	//临时文件名
	char                m_szTempLocalFile[MAX_PATH];
private:
	/*服务器IP*/
    char                m_szServerIP[IP_LENGTH];
	/*是否断点续传*/
    ULONG               m_ulIsContinue;
	/*服务器Port*/
    USHORT              m_usServerPort;
	/*待下载的大小（不一定是文件长度）*/
    int                 m_iFileSize;
	/*已经下载的大小*/
    int                 m_iDownloadSize;
    /*本地文件句柄*/
    FILE               *m_pFile;
	/*标实服务器*/
    struct sockaddr_in  m_sockServer;
	/*上次下载的大小（用于断点续传）*/
    int                 m_iLastDownloadSize;
    char                m_szBuffer[MAX_RECV_LEN];
    char                m_szErrBuf[MAX_PATH];
	/*下载结果*/
    int                 m_iDownloadResult;
	//是否还与Server保持着联接 
	BOOL m_bConnected;		

	//超时次数
	int m_iTimeoutCount;
};

#endif /* __DOWNGAME_H__ */
