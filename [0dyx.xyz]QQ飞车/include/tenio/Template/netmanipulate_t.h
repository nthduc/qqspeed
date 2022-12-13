#ifndef __NETMANIPULATE_T_H__
#define __NETMANIPULATE_T_H__

//=============================================================================
// Include
//=============================================================================

//#include <Winsock2.h>
#include <atlbase.h>

//#pragma comment(lib, "ws2_32.lib") 

#define NETMANIPULATE_IPSTRINGDEFAULTLENGTH	512

//=============================================================================
// Template
//=============================================================================
namespace Tenio
{
	
	template<class TSubclass>
		class TNetManipulate
	{
public:
	
	//-----------------------------------------------------------------------------
	//  把 ulIP 转为字符串形式, 类似 inet_ntoa. 注意, 返回的字符串指针指向函数静态
	//	变量, 不能把返回的指针保存起来. 因为下次使用时可能其中内容已经改变
	//-----------------------------------------------------------------------------
	
	LPCTSTR IP2String(ULONG ulIP)
	{
		USES_CONVERSION;
		
		static TCHAR s_szIPString[NETMANIPULATE_IPSTRINGDEFAULTLENGTH] = {0};
		
		static LPTSTR s_pCurrent = NULL;
		
		if(s_pCurrent == NULL)
		{
			s_pCurrent = s_szIPString;
		}
		
		s_szIPString[NETMANIPULATE_IPSTRINGDEFAULTLENGTH - 1] = TEXT('\0');
		
		LPCTSTR lpszIP = A2T(inet_ntoa(*(in_addr*)&ulIP));
		
		int nThisTimeStringLength = _tcslen(lpszIP);
		
		LPCTSTR pEndPosition = s_szIPString + NETMANIPULATE_IPSTRINGDEFAULTLENGTH - 1;
		
		// 如果此字符串超过了缓冲区末端, 则把指针设置为缓冲区开始处
		if(s_pCurrent + nThisTimeStringLength > pEndPosition)
		{
			s_pCurrent = s_szIPString;
		}
		
		// 计算从 s_pCurrent 到缓冲区结尾之间有多少容量
		int nCapability = 0;
		nCapability = NETMANIPULATE_IPSTRINGDEFAULTLENGTH - (s_pCurrent - s_szIPString) - 1;
		_tcsncpy(s_pCurrent, lpszIP, nCapability);
		
		LPCTSTR lpszResult = s_pCurrent;
		
		s_pCurrent += _tcslen(s_pCurrent);
		
		s_pCurrent[0] = TEXT('\0');
		
		++s_pCurrent;
		
		if(s_pCurrent >= pEndPosition)
		{
			s_pCurrent = s_szIPString;
		}
		
		return lpszResult;
	}
	
	//-----------------------------------------------------------------------------
	//  把字符串格式的IP转为网络字节序的数字
	//-----------------------------------------------------------------------------
	
	ULONG String2IP(LPCTSTR lpszIP)
	{
		USES_CONVERSION;
		
		return inet_addr(T2A((LPTSTR)lpszIP));
	}
	
	BOOL IsValidIP(ULONG ulIP)
	{
		return (ulIP != 0 && ulIP != INADDR_NONE);
	}
	};
	
};
#endif