
#include "StdAfx.h"
#include "QQTHelper.h"
#include "AtlConv.h"
#include "WinSock2.h"
#include "QQTHelper.h"

HINSTANCE CreateObjectFromFile(const char* szPath, REFCLSID factoryid, REFCLSID interfaceid, LPVOID *pInterface)
{
	USES_CONVERSION;
	if(NULL == pInterface || NULL == szPath)
	{
		ASSERT(FALSE);
		return NULL;
	}
	typedef HRESULT (__stdcall *GETCLASS_PROC)(REFCLSID, REFIID, LPVOID*);
	*pInterface = NULL;
	HRESULT hr = S_FALSE;
	HINSTANCE hInstance = ::CoLoadLibrary(T2OLE(szPath), FALSE);
	GETCLASS_PROC lpGetClssProc = (GETCLASS_PROC)::GetProcAddress(hInstance, "DllGetClassObject"); 
	if(lpGetClssProc)
	{
		IClassFactory *pfactory = NULL;
		hr = lpGetClssProc(factoryid, IID_IClassFactory, (void**)&pfactory);
		if(pfactory)
		{
			hr = pfactory->CreateInstance(NULL, interfaceid, pInterface);
			pfactory->Release();
		}
	}
	return hInstance;
}

// 将'/' 改为 '\\'
void WinPathString(char *szPath)
{
	if (NULL == szPath)
	{
		return;
	}

	for(int i = 0; i < strlen(szPath); ++i)
	{
		if ('/' == szPath[i])
		{
			szPath[i] = '\\';
		}
	}
}

void WinPathString(CString &strPath)
{
	int nLen = strPath.GetLength();
	char *buff = strPath.GetBuffer(0);
	
	for (int i = 0; i < nLen; ++i)
	{
		if ('/' == buff[i])
		{
			buff[i] = '\\';
		}
	}
	strPath.ReleaseBuffer();
}

void NetPathString(char *szPath)
{
	if (NULL == szPath)
	{
		return;
	}

	for (int i = 0; i < strlen(szPath); ++i)
	{
		if ('\\' == szPath[i])
		{
			szPath[i] = '/';
		}
	}
}

void NetPathString(CString &strPath)
{
	int nLen = strPath.GetLength();
	char *buff = strPath.GetBuffer(0);
	
	for (int i = 0; i < nLen; ++i)
	{
		if ('\\' == buff[i])
		{
			buff[i] = '/';
		}
	}
	strPath.ReleaseBuffer();
}

CString GetRootPath()
{
	static CString strRoot = _T("");
	if(strRoot.IsEmpty())
	{
		strRoot = GetRootPathFromHandle(NULL);
	}
	return strRoot;
	
}

CString GetRootNetPath()
{
	static CString strRoot = _T("");
	if(strRoot.IsEmpty())
	{
		strRoot = GetRootPathFromHandle(NULL);
	}

	NetPathString(strRoot);
	return strRoot;
}



CString GetRootPathFromHandle(HINSTANCE hInstance)
{
	CString strRoot;
	
	GetModuleFileName(hInstance, strRoot.GetBuffer(MAX_PATH),MAX_PATH);
	strRoot.ReleaseBuffer();
	strRoot = strRoot.Left(strRoot.ReverseFind('\\') + 1);
	
	return strRoot;
}

BOOL CheckFileExist(CString strFileName)
{
	BOOL             bFind = FALSE;
	WIN32_FIND_DATA  findFileData;	                 
	HANDLE           hFind = FindFirstFile(strFileName, &findFileData);
	if(hFind != INVALID_HANDLE_VALUE)
	{   
		bFind = TRUE;
	}
	FindClose(hFind);
	
	return bFind;
}

CString GetLocalIP(DWORD* pIP /*= NULL*/)
{
	CString s = "";
	//char sHostname[256];
	hostent *lpHost;
	SOCKADDR_IN sLocalSockAddr;

	char sHostname[256];
	CString strTempHostName;

	if(gethostname(sHostname,256))
	{
		if (pIP)
			*pIP = 0;
		return s;
	}
	
	strTempHostName = sHostname;
	
	lpHost = gethostbyname(strTempHostName);
	
	if(!lpHost)
	{
		if (pIP)
			*pIP = 0;
		return s;
	}

	for(int i = 0; lpHost!= NULL && lpHost->h_addr_list[i]!= NULL; i++ );
	if(i > 1)
	{
		for(i = 0; lpHost!= NULL && lpHost->h_addr_list[i]!= NULL; i++ )
		{
			SOCKADDR_IN sLocalSockAddr;
			sLocalSockAddr.sin_addr.s_addr = *((unsigned long *)lpHost->h_addr_list[i]);
			
			s = inet_ntoa(sLocalSockAddr.sin_addr);
			
			if(s == "0.0.0.0" || s.Left(3) == "169")
			{
				//排除 0.0.0.0 或 169.x.x.x
				continue;
			}
			
			if (pIP)
				*pIP = sLocalSockAddr.sin_addr.S_un.S_addr;
			break;
		}
	}
	else
	{
		//just one Ip:
		sLocalSockAddr.sin_addr.s_addr = *((unsigned long *)lpHost->h_addr);
		s = inet_ntoa(sLocalSockAddr.sin_addr);
		
		if (pIP)
			*pIP = sLocalSockAddr.sin_addr.S_un.S_addr;
	}

	return s;
}

CString GetWord(CString str,CString Key,int num)
{
	if(str.IsEmpty())
		return _T("");
	long length=Key.GetLength();
	int i=1;
	while(i<num)
	{
		while(str.Left(length)!=Key)
		{
			if(str.GetLength()==1)
				return _T(""); 
			str=str.Right(str.GetLength()-1);
		}
		str=str.Right(str.GetLength()-length);
        i++;
	}
	if(str.Find(Key)>0)
		str=str.Left(str.Find(Key));
	return str;
}

////////////////////////////////////////////
//取得exe/dll版本号
//	return type : DWORD	
////////////////////////////////////////////
DWORD GetInnerInfo( CString ObjectPath )
{
	UINT nSizeOfInfo = ::GetFileVersionInfoSize((char*)(LPCTSTR)ObjectPath, NULL);
	CString str;
	if(0 == nSizeOfInfo)
	{
		return (DWORD)((int)-1);
	}
	char *pInfobuf = new char[nSizeOfInfo + 2];
	VS_FIXEDFILEINFO *pFileInfo = NULL;
	UINT nSize;

	::GetFileVersionInfo((char*)(LPCTSTR)ObjectPath, NULL, nSizeOfInfo, pInfobuf);
	::VerQueryValue(pInfobuf ,TEXT("\\") , (LPVOID*)&pFileInfo, &nSize);
	
	
	DWORD innerVer[5];

	innerVer[0] = pFileInfo->dwFileVersionMS >> 16;
	innerVer[1] = pFileInfo->dwFileVersionMS & 0xffff;
	innerVer[2] = ( pFileInfo->dwFileVersionLS >> 16 ) / 100;
	innerVer[3] = ( pFileInfo->dwFileVersionLS >> 16 ) - innerVer[2] * 100;
	innerVer[4] = pFileInfo->dwFileVersionLS & 0xffff;

	delete []pInfobuf;
	return	  innerVer[0] * 10000000
			+ innerVer[1] * 100000
			+ innerVer[2] * 10000
			+ innerVer[3] * 100
			+ innerVer[4] ;
}

////////////////////////////////////////////
//取得exe/dll版本号
//	return type : CString
////////////////////////////////////////////
CString GetOuterInfo( CString ObjectPath )
{
	DWORD innerVer_compact = GetInnerInfo( ObjectPath );
	DWORD innerVer[5];
	
	innerVer[0] = innerVer_compact / 10000000;
	innerVer_compact -= innerVer[0] * 10000000;
	innerVer[1] = innerVer_compact / 100000;
	innerVer_compact -= innerVer[1] * 100000;
	innerVer[2] = innerVer_compact / 10000;
	innerVer_compact -= innerVer[2] * 10000;
	innerVer[3] = innerVer_compact / 100;
	innerVer_compact -= innerVer[3] * 100;
	innerVer[4] = innerVer_compact;
	
	CString str;
	CString tempstr;

	// main version
	str.Format( "%d." , innerVer[0] );

	// sub version
	if( innerVer[1] < 10 ) tempstr += '0';
	str += tempstr;
	tempstr.Empty();
	tempstr.Format( "%d " , innerVer[1] );
	str += tempstr;
	
	// alpha - beta - formal
	switch( innerVer[2] ) {
	case 0:
		str += "Alpha";
		break;
	case 1:
		str += "Beta";
		break;
	case 2:
		str += "正式版";
		break;
	default:
		str = "";
		return str;
	}
	
	// sub-ver of alpha - beta - formal and build number
	tempstr.Empty();
	tempstr.Format( "%d Build%d" , innerVer[3] , innerVer[4] );
	str += tempstr;
	
	return str;
}

unsigned int GetLiveUpdateVersionInfo(DWORD dwInnerVersion)
{
	DWORD innerVer_compact = dwInnerVersion;
	DWORD innerVer[5];
	
	innerVer[0] = innerVer_compact / 10000000;
	innerVer_compact -= innerVer[0] * 10000000;
	innerVer[1] = innerVer_compact / 100000;
	innerVer_compact -= innerVer[1] * 100000;
	innerVer[2] = innerVer_compact / 10000;
	innerVer_compact -= innerVer[2] * 10000;
	innerVer[3] = innerVer_compact / 100;
	innerVer_compact -= innerVer[3] * 100;
	innerVer[4] = innerVer_compact;

	return unsigned int(innerVer[2] << 16) | unsigned int (innerVer[3] << 8) | unsigned int (innerVer[4]);
}

unsigned int GetLiveUpdateVersionInfo(CString ObjectPath)
{
	DWORD innerVer_compact = GetInnerInfo( ObjectPath );
	DWORD innerVer[5];
	
	innerVer[0] = innerVer_compact / 10000000;
	innerVer_compact -= innerVer[0] * 10000000;
	innerVer[1] = innerVer_compact / 100000;
	innerVer_compact -= innerVer[1] * 100000;
	innerVer[2] = innerVer_compact / 10000;
	innerVer_compact -= innerVer[2] * 10000;
	innerVer[3] = innerVer_compact / 100;
	innerVer_compact -= innerVer[3] * 100;
	innerVer[4] = innerVer_compact;

	return unsigned int(innerVer[2] << 16) | unsigned int (innerVer[3] << 8) | unsigned int (innerVer[4]);
}

unsigned short GetUploadVersionInfo(CString ObjectPath)
{
	DWORD innerVer_compact = GetInnerInfo( ObjectPath );
	DWORD innerVer[5];
	
	innerVer[0] = innerVer_compact / 10000000;
	innerVer_compact -= innerVer[0] * 10000000;
	innerVer[1] = innerVer_compact / 100000;
	innerVer_compact -= innerVer[1] * 100000;
	innerVer[2] = innerVer_compact / 10000;
	innerVer_compact -= innerVer[2] * 10000;
	innerVer[3] = innerVer_compact / 100;
	innerVer_compact -= innerVer[3] * 100;
	innerVer[4] = innerVer_compact;

	return unsigned short(innerVer[3] << 8) | unsigned short(innerVer[4]);
}
BOOL Buffer2String(const byte *pInBuffer, int iSizeofBuffer, char *pOutszMsg, int iSizeOfszMsg, int& iOutStringLength)
{
	iOutStringLength = 0;
	if(NULL == pInBuffer || iSizeofBuffer < 1 || NULL == pOutszMsg || iSizeOfszMsg <= 2 * iSizeofBuffer)
	{
		return FALSE;
	}
	iOutStringLength = 2 * iSizeofBuffer;
	pOutszMsg[iOutStringLength] = '\0';
	for(int i = 0; i < iSizeofBuffer; i ++)
	{
		pOutszMsg[i * 2] = (pInBuffer[i] >> 4) + 'a';
		pOutszMsg[i * 2 + 1] = (pInBuffer[i] & 0xf) + 'a';
	}
	return TRUE;
}
BOOL String2Buffer(const char *pInszMsg, byte *pOutBuffer, int iBufferSize, int &iOutBufferSize)
{
	iOutBufferSize = 0;
	int iMsgLen = ::strlen(pInszMsg);
	ASSERT(0 == iMsgLen % 2);
	if(iBufferSize < iMsgLen / 2)
	{
		return FALSE;
	}
	iOutBufferSize = iMsgLen / 2;
	for(int i = 0; i < iOutBufferSize; i ++)
	{
		ASSERT(pInszMsg[2 * i] >= '0' && pInszMsg[2 * i] < 'a' + 16);
		ASSERT(pInszMsg[2 * i + 1] >= '0' && pInszMsg[2 * i + 1] < 'a' + 16);
		pOutBuffer[i] = byte(((pInszMsg[2 * i] - 'a') << 4 ) | (pInszMsg[2 * i + 1] - 'a'));
	}
	return TRUE;
}

BOOL IsFileExist(CString strFileName)
{
	WIN32_FIND_DATA findFileData;	                 
	HANDLE hFind = FindFirstFile(strFileName, &findFileData);
	BOOL bFind = (hFind == INVALID_HANDLE_VALUE) ? FALSE : TRUE;	
	FindClose(hFind);	
	return bFind;
}

