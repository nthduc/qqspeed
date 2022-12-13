#ifndef _QQT_HELPER_
#define _QQT_HELPER_

#include "wtypes.h"
#include "afx.h"
#include "AsyncGetHostByNameWnd.h"
//#include "ICore.h"


#define __pre_compile_i2a(x) #x
#define _PRECOMPILE_I2A(x) __pre_compile_i2a(x)
#define _compile_message_file_line __FILE__ "("_PRECOMPILE_I2A(__LINE__)") : "

inline BOOL IsEqualGuid(REFGUID g1,REFGUID g2)
{
	return (((PLONG) &g1)[0] == ((PLONG) &g2)[0] &&
	((PLONG) &g1)[1] == ((PLONG) &g2)[1] &&
	((PLONG) &g1)[2] == ((PLONG) &g2)[2] &&
	((PLONG) &g1)[3] == ((PLONG) &g2)[3]);
}
inline int GetQQMainVer() { return 9; } //QQ主版本号
inline int GetQQSubVer() {return 10; } //QQ次版本号

HINSTANCE CreateObjectFromFile(const char* szPath, REFCLSID factoryid, REFCLSID interfaceid, LPVOID *pInterface);
CString GetRootPathFromHandle(HINSTANCE hInstance);
BOOL CheckFileExist(CString strFileName);
CString GetRootPath();    
CString GetRootNetPath(); // 将GetRootPath() 的'\\' 转成'/'
CString GetLocalIP(DWORD *pIP = NULL);
CString GetWord(CString str,CString Key,int num); //取得以Key分隔的第num个字段 num为1基
DWORD GetInnerInfo(CString ObjectPath);//取得exe/dll版本号
CString GetOuterInfo(CString ObjectPath);//取得exe/dll版本号
BOOL Buffer2String(const byte *pInBuffer, int iSizeofBuffer, char *pOutszMsg, int iSizeOfszMsg, int& iOutStringLength);
BOOL String2Buffer(const char *pInszMsg, byte *pOutBuffer, int iBufferSize, int &iOutBufferSize);
BOOL IsFileExist(CString strFileName);//文件是否存在

void WinPathString(char *szPath); // 将路径中的'/'字符 改为 '\\'
void WinPathString(CString &strPath); 

void NetPathString(char *szPath); // 将路径中的'\\'字符 改为 '/'
void NetPathString(CString &strPath);

unsigned short GetUploadVersionInfo(CString ObjectPath);
unsigned int GetLiveUpdateVersionInfo(CString ObjectPath);
unsigned int GetLiveUpdateVersionInfo(DWORD dwInnerVersion);
//////////////////////////////////////////////////////////////////////////
// for get tick count
//DWORD GetQQTTickCount(IQQTCore *pCore);
//////////////////////////////////////////////////////////////////////////


#define RUN_LIST(xxxlist) {for(POSITION pos = xxxlist.GetHeadPosition(); pos; xxxlist.GetNext(pos)){
#define END_RUN_LIST }}
//#pragma message(_compile_message_file_line "log is opened!!!")
//#define _ENABLE_QQT_LOG_
#ifdef _ENABLE_QQT_LOG_
void __qqt_set_log_path(const char *szPath);
void __qqt_log_string(BOOL bWriteTime, const char *szKeyName, const char *pFormat, ...);
void __qqt_log_bin(BOOL bWriteTime, const char *szKeyName, int nLen, const char *pBuf);
void __qqt_debug_win_log(char* pFormat, ...);
#define QQT_SET_LOG_PATH		__qqt_set_log_path
#define LOGSTRING				__qqt_log_string
#define LOGBIN					__qqt_log_bin
#define DBWLOG					__qqt_debug_win_log
#else
#define QQT_SET_LOG_PATH		/##/
#define LOGSTRING				/##/
#define LOGBIN					/##/
#define DBWLOG					/##/
#endif

/*
#define GETMODULE(pCore, IModule, ptr) \
	IModule *ptr = NULL; \
	if(pCore) \
	{ \
		pCore->GetMudule(&IID_##IModule, (IUnknown**)&ptr); \
		ASSERT(ptr); \
	} \
	else \
	{ \
		ASSERT(FALSE); \
	}

#define GETMODULE_NOASSERT(pCore, IModule, ptr) \
	IModule *ptr = NULL; \
	if(pCore) \
	{ \
		pCore->GetMudule(&IID_##IModule, (IUnknown**)&ptr); \
	}
	
*/

#pragma comment( lib, "QQTHelper.lib" )

template <class T> class CQQTWiseComPtr
{
public:
	CQQTWiseComPtr(T *p = NULL)
		{if(ptr = p) ptr->AddRef();}
	~CQQTWiseComPtr()
		{if(ptr) ptr->Release();}

	T** operator& ()
		{return &ptr;}
	T* operator = (T *p)
		{if(ptr == p) return ptr; if(ptr) ptr->Release(); if(ptr = p) ptr->AddRef(); return ptr;}
	T* operator ->()
		{return ptr;}
	operator BOOL()
		{return ptr ? TRUE : FALSE;}
	operator !()							{return ptr ? FALSE : TRUE;}
	operator T*()
		{return ptr;}
private:
	T *ptr;
};

template <class T, size_t nSize = 1> class CQQTBufferHelper
{
public:
	CQQTBufferHelper()			 {if(1 == nSize) ptr = new T; else ptr = new T[nSize]; ASSERT(ptr);}
	~CQQTBufferHelper()			{if(ptr) {if(1 == nSize) delete ptr; else delete []ptr;}}
	T* operator->()						{ASSERT(ptr); return ptr;}
	operator void*()					{ASSERT(ptr); return ptr;}
	operator T*()							{ASSERT(ptr); return ptr;}
	T& operator[](int i)				{ASSERT(i >= 0 && i < nSize && ptr); return ptr[i];}
	operator BOOL()					{return ptr ? TRUE : FALSE;}
	operator !()							{return ptr ? FALSE : TRUE;}
	void detach()						{ptr = NULL;}
private:
	T *ptr;
};


#pragma comment( lib, "version.lib" )
#pragma comment(lib, "Ws2_32.lib")

#endif