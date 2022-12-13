#include "stdafx.h"
#include "Network/consts.h"
#include "Network/Network_i.h"
#include "httpdown.h"
#include "common.h"
#include "fileop.h"

CHttpDown::CHttpDown(const HTTPGETDATA* data)
{
	m_iFileSize         = 0;
	m_iDownloadSize     = 0;	
	m_iLastDownloadSize = 0;	
	m_iDownloadResult   = DOWNLOAD_PROGRESS;  /*首先将下载结果设置成正在下载*/
	m_pFile             = NULL;
	m_bConnected        = FALSE;
	m_iRecvIndex        = 0;
	m_pDestIP           = NULL;	
	m_bHaveClose        = FALSE;	
	m_bHaveCancelDown   = FALSE;
	m_bCanDelete        = FALSE;
	m_bAutoDel          = data->bAutoDel;
	m_iDownTimeout      = DOWN_TIMEOUTCOUNT;	
	m_iTimeoutCount		= 0;
}

CHttpDown::~CHttpDown()
{
	CancelDown();
}

void CHttpDown::CloseFile()
{
	if(NULL != m_pFile)
	{
		fclose(m_pFile);
		m_pFile = NULL;
	}
}

int CHttpDown::OpenTmpLocFile()
{
	/*如果文件已经打开，首先关闭*/
	if(NULL != m_pFile)
	{
		fclose(m_pFile);
	}
	
	/*以只读的方式打开文件*/
	m_pFile = fopen(m_szTempLocalFile, "rb");
	if(NULL == m_pFile)
	{
		/*如果文件不存在，以下载新文件的方式下载*/
		m_ulIsContinue = DOWNLOAD_NEW;
	}
	else
	{
		fseek(m_pFile, 0, SEEK_END);
		if(0 == ftell(m_pFile))
		{
			/*如果文件大小为0，以下载新文件的方式下载*/
			m_ulIsContinue = DOWNLOAD_NEW;
		}
		/*关闭文件句柄*/
		fclose(m_pFile);
		m_pFile = 0;
	}
	
    if(DOWNLOAD_NEW == m_ulIsContinue)
	{
		/*打开（以二进制方式）文件，如果文件有内容则被清空*/
		m_pFile = fopen(m_szTempLocalFile, "wb");
		if(NULL == m_pFile)
		{
			return ERR_CREATELOCALFILE;
		}
	}
	else
	{
		/*以追加写的方式（二进制）打开文件*/
		m_pFile = fopen(m_szTempLocalFile, "a+b");
		if(NULL == m_pFile)
		{
			return ERR_OPENLOCALFILE;
		}
		fseek(m_pFile, 0, SEEK_END);
		/*得到上次下载的长度*/
		m_iLastDownloadSize = ftell(m_pFile);
	}
    return OPEN_FILE_SUCCESS;
}

int CHttpDown::CloseSocket()
{
	if (TRUE == m_bHaveClose)
	{
		return 0;
	}
	m_bHaveClose = TRUE;
	return 0;
}

double CHttpDown::GetDownloadProgress()
{
	if(0 == m_iFileSize)
	{
		return 0;
	}
	double dRate;
	double dHaveDown = m_iDownloadSize + m_iLastDownloadSize;
	double dFilesize = m_iFileSize + m_iLastDownloadSize;
	dRate = dHaveDown/dFilesize;
	return dRate;
}

int CHttpDown::GetFileSize()
{
	if(0 == m_iFileSize)
		return -1;		//如果m_iFileSize=0 表示还没有取道文件大小的信息
	else
		return m_iFileSize + m_iLastDownloadSize;
}

int CHttpDown::GetDownloadSize()
{
    return m_iDownloadSize + m_iLastDownloadSize;
}

int CHttpDown::GetDownloadResult()
{
	return m_iDownloadResult;
}

short CHttpDown::DownloadFile(CString strUrl, CString strLocalFile, BOOL bIsContinue)
{
	CString str,strTempUrl;
	
	str = "http://";
	strTempUrl = strUrl.Right(strUrl.GetLength()-str.GetLength());  //去掉"http://"
	int iLen = strTempUrl.Find(":");
	if(iLen<0)
	{
		iLen = strTempUrl.Find("/");
		str = strTempUrl.Left(iLen);
		long lAddr = inet_addr(str);
		hostent* ent = gethostbyname(str);
		
		strTempUrl = strTempUrl.Right(strTempUrl.GetLength() - iLen );
		m_usServerPort = 80;
		strncpy(m_szRemoteFile,strTempUrl,sizeof(m_szRemoteFile));
	}
	else
	{
		str = strTempUrl.Left(iLen);
		strTempUrl = strTempUrl.Right(strTempUrl.GetLength()-iLen-1);
		iLen = strTempUrl.Find("/");
		str = strTempUrl.Left(iLen);
		m_usServerPort = (USHORT)(atoi(str));
		strTempUrl = strTempUrl.Right(strTempUrl.GetLength()-iLen);
		strncpy(m_szRemoteFile,strTempUrl,sizeof(m_szRemoteFile));
	}

//	str = inet_ntoa(*(struct in_addr*)&m_SocketInfo.m_ulDestAddr);
	strncpy(m_szServerIP, str, sizeof(m_szServerIP));  //210.10.30.254  这种IP信息
	strncpy(m_szLocalFile, strLocalFile, sizeof(m_szLocalFile));
	
	m_ulIsContinue = (ULONG)bIsContinue;
	
	CString strTmpPath = "DownTemp\\";
	CreateDirectory(strTmpPath , NULL);
	
	if (TRUE == m_bAutoDel)
	{	
		_snprintf(m_szTempLocalFile, MAX_PATH, "%sHttpDown%d.tmp", strTmpPath, m_dwNumber);
	}
	else
	{
		strncpy(m_szTempLocalFile, m_szLocalFile, MAX_PATH);
	}

	SHFILEINFO shInfo;
	
	//如果需要下载的文件已经存在 COPY过去
	if( (0 != ::SHGetFileInfo(m_szLocalFile, 0, &shInfo, sizeof(shInfo), SHGFI_EXETYPE)) 
		&& (0 != m_ulIsContinue) )
	{
		if (TRUE == m_bAutoDel)
		{
			::CopyFile(m_szLocalFile, m_szTempLocalFile, FALSE);
		}
	}

	Connect(m_szServerIP, m_usServerPort);
	if( GetLastError() != WSAEWOULDBLOCK )
	{	   
		m_iDownloadResult = ERR_CONNECTSERVER;
		return FALSE;
	}
	
	return 0;
}

void CHttpDown::SetDeleteDownFlag(BOOL bDeleteFlag)
{
    m_bCanDelete = bDeleteFlag;
}

BOOL CHttpDown::GetDeleteDownFlag()
{
    return m_bCanDelete;
}

BOOL CHttpDown::GetAutoDelFlag()
{
	return m_bAutoDel;
}

DWORD CHttpDown::GetDownIndex()
{
	return m_dwNumber;
}


BOOL CHttpDown::CheckTimeout()
{
	m_iTimeoutCount++;
	if ( m_iTimeoutCount > m_iDownTimeout)
	{
		LogText("CHttpDown::CheckTimeout 下载超时%d秒[IP:%s,Port:%d,File:%s]",CHECK_DOWNTIMEOUT * m_iDownTimeout, m_pDestIP, m_usServerPort, m_szLocalFile);
		return TRUE;
	}
	return FALSE;
}

void CHttpDown::OnConnect(int nErrorCode)
{	
	int  iSendLen;	
	int  iRet;

	if ( nErrorCode != 0)
	{
		m_iDownloadResult = ERR_CONNECTSERVER;
		return;
	}
	
	m_bConnected   = TRUE;	
	
	iRet = OpenTmpLocFile();
	if(OPEN_FILE_SUCCESS != iRet) /*如果创建本地文件失败，则下载失败*/
	{		
		m_iDownloadResult = iRet;
		CloseSocket();
		CloseFile();	
		return;
	}

	if(DOWNLOAD_NEW == m_ulIsContinue) /*构造HTTP命令*/
	{
		_snprintf(m_szBuffer, MAX_RECV_LEN, "GET %s HTTP/1.0\r\n"
											"Accept: */*\r\n"
											"Accept-Language: zh-cn\r\n"
											"Accept-Encoding: gzip, deflate\r\n"
											"User-Agent: QQGame\r\n"
											"Host: %s:%d\r\n"
											"Connection: Keep-Alive\r\n\r\n",
											m_szRemoteFile,
											m_szServerIP,
											m_usServerPort);
	}
	else
	{
		_snprintf(m_szBuffer, MAX_RECV_LEN, 
							"GET %s HTTP/1.1\r\n"
						    "Host: %s\r\n"
							"Accept: */*\r\n"
							"User-Agent: QQTang\r\n"
							"Pragma: no-cache\r\n"
							"Cache-Control: no-cache\r\n"
							"Connection: keep-alive\r\n"
							"Range: bytes=%d-\r\n\r\n" ,
							m_szRemoteFile,
							m_szServerIP,
							m_iLastDownloadSize);
	}

	iSendLen = strlen(m_szBuffer);
	LogText("CHttpDown::OnConnect: 发送http命令(长度:%d): \n %s", iSendLen, m_szBuffer);
	
	/*发送HTTP下载请求*/		
	Send((LPBYTE)m_szBuffer, iSendLen);
	if ( GetLastError() != WSAEWOULDBLOCK )
	{
		CloseSocket();
		CloseFile();	
		m_iDownloadResult = ERR_CONNECTSERVER;
		return;
	}
	m_shNowRcvDataType = HEAD_DATA_TYPE;		//等待接受头信息

	return;
}

void CHttpDown::OnReceive(int nErrorCode)
{
	if(NULL == m_pFile || nErrorCode != 0 )
	{
		return;
	}

	char chRecvBuf[NORMAL_DATA_LEN];
	int iRecvBytes = NORMAL_DATA_LEN;
	if ( Receive(chRecvBuf, iRecvBytes) <= 0 )
		return;

	int  iWriteSize;
	char *m_pFileLenPos;		
	int  iIndex = 0;
	char *pTemp;
	int  iPackHeadLen = 0;
	int  iContentOffset = 0;
		
	if(HEAD_DATA_TYPE == m_shNowRcvDataType)	////等待接收头信息
	{
		memset(m_szBuffer + m_iRecvIndex, 0, sizeof(m_szBuffer) - m_iRecvIndex);
    	memcpy(m_szBuffer + m_iRecvIndex , chRecvBuf, iRecvBytes);

		char* pTemp1 = strstr(m_szBuffer, "\n\n");
		char* pTemp2 = strstr(m_szBuffer, "\r\n\r\n");
		if((NULL != pTemp1) && (NULL != pTemp2))
		{
			if(pTemp1 < pTemp2)
			{
				pTemp = pTemp1;
				iContentOffset = 2;
			}
			else
			{
				pTemp = pTemp2;
				iContentOffset = 4;
			}
		}
		else if(NULL != pTemp1)
		{
			pTemp = pTemp1;
			iContentOffset = 2;
		}
		else if(NULL != pTemp2)
		{
			pTemp = pTemp2;
			iContentOffset = 4;
		}
		else
		{
			pTemp = NULL;
		}
		
		if (NULL == pTemp)
		{
			m_iRecvIndex = m_iRecvIndex + iRecvBytes;

			LogText("CHttpDown::OnReceive 从%s接收到http下载头部信息,信息错误1", m_pDestIP);
			return;
		}

		LogText("CHttpDown::OnReceive %s回应的HTTP消息为 \n%s", m_pDestIP, &m_szBuffer[0]);

		m_shNowRcvDataType = FILE_DATA_TYPE;

		if(	0 != strncmp(m_szBuffer, REPLY_OK_1, strlen(REPLY_OK_1))
			&& 0 != strncmp(m_szBuffer, REPLY_OK_2, strlen(REPLY_OK_2))
			&& 0 != strncmp(m_szBuffer, REPLY_OK_3, strlen(REPLY_OK_3))
			&& 0 != strncmp(m_szBuffer, REPLY_OK_4, strlen(REPLY_OK_4)) )
		{
			CloseFile();
			m_iDownloadResult = ERR_REFILENOTEXIST;

			LogText("CHttpDown::OnReceive 回应的消息是\n%s!", m_szBuffer);
			return ;
		}
		m_pFileLenPos = strstr(m_szBuffer, FILE_LEN_STRING);
		if(NULL == m_pFileLenPos)
		{
			CloseFile();
			m_iDownloadResult = ERR_REFILENOTEXIST;
			LogText("CHttpDown::OnReceive 回应的消息是\n%s!", m_szBuffer);
			return ;
		}
		/*获得待下载的文件大小（如果是断点续传则是文件后部分的大小）*/
		m_iFileSize = atoi(m_pFileLenPos + strlen(FILE_LEN_STRING) + 1);
		
		if(0 == m_iFileSize)
		{
			/*如果文件大小为0，说明本地文件和远程文件一样大，无需下载*/
			CloseFile();
			m_iDownloadResult = DOWNLOAD_SUCCESS;
			return ;
		}
		
		iPackHeadLen = (pTemp + iContentOffset) - m_szBuffer;
		iRecvBytes   = iRecvBytes - iPackHeadLen;
		if(0 < iRecvBytes)
		{
			iWriteSize = fwrite(pTemp + iContentOffset, iRecvBytes, 1, m_pFile);			
			if(iWriteSize != 1)
			{
				/*写入硬盘失败*/
				CloseFile();
				m_iDownloadResult = DOWNLOAD_FAILED;
				LogText("CHttpDown::OnReceive 从%s接收到http下载头部信息,写入硬盘的时候失败", m_pDestIP , m_iFileSize);
				return ;
			}
			else
			{
				iWriteSize = iRecvBytes;
			}
		}
		m_iDownloadSize = iRecvBytes;	
		
		if(m_iDownloadSize == m_iFileSize)
		{
			/*接收文件成功*/
			CloseFile();
			if (TRUE == m_bAutoDel)
			{
				::CopyFile(m_szTempLocalFile , m_szLocalFile, FALSE);
			}
			m_iDownloadResult = DOWNLOAD_SUCCESS;		
			LogText("CHttpDown::OnReceive 从%s接收到http下载头部信息,直接接收数据成功", m_pDestIP , m_iFileSize);
			return ;
		}
		
	}
	else if(FILE_DATA_TYPE == m_shNowRcvDataType)
	{
		m_iDownloadSize = m_iDownloadSize + iRecvBytes;

		iWriteSize = fwrite(chRecvBuf, iRecvBytes, 1, m_pFile);
		
		if(iWriteSize != 1)
		{
			/*写入硬盘失败*/
			CloseFile();
			m_iDownloadResult = DOWNLOAD_FAILED;
			LogText("CHttpDown::OnReceive 从%s接收到http下载数据,写入硬盘的时候失败", m_pDestIP );
			return ;
		}
		else
		{
			iWriteSize = iRecvBytes;
		}
		
		if(m_iDownloadSize == m_iFileSize)
		{
			/*接收文件成功*/
			CloseFile();
			if (TRUE == m_bAutoDel)
			{
				::CopyFile(m_szTempLocalFile , m_szLocalFile, FALSE);
			}
			m_iDownloadResult = DOWNLOAD_SUCCESS;					
			LogText("CHttpDown::OnReceive 从%s接收文件成功,总长度为%d", m_pDestIP , m_iDownloadSize);
		}
		else if(m_iDownloadSize > m_iFileSize)
		{
			/*本次接收文件失败*/
			CloseFile();
			m_iDownloadResult = DOWNLOAD_FAILED;
			LogText("CHttpDown::OnReceive 从%s接收到http下载数据,出现了异常", m_pDestIP );
		}		
	}
}

void CHttpDown::SetDownloadResult(int iResult)
{
	m_iDownloadResult = iResult;
}

BOOL CHttpDown::CancelDown()
{
	if (TRUE == m_bHaveCancelDown)
	{
		return TRUE;
	}
	m_bHaveCancelDown = TRUE;
	CloseFile();
	CloseSocket();
	if(TRUE == FileExists(m_szTempLocalFile) && TRUE == m_bAutoDel)
	{
		try
		{
			CFile::Remove(m_szTempLocalFile);  //删除COPY的临时文件
			return TRUE;
		}
		catch(CFileException *e)
		{
			e->Delete();
			return FALSE;
		}
	}	
	if (DOWNLOAD_PROGRESS == m_iDownloadResult)
	{
		m_iDownloadResult = DOWNLOAD_FAILED;
	}
	
	return TRUE;
}
