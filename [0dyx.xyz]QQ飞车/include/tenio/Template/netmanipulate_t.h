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
	//  �� ulIP תΪ�ַ�����ʽ, ���� inet_ntoa. ע��, ���ص��ַ���ָ��ָ������̬
	//	����, ���ܰѷ��ص�ָ�뱣������. ��Ϊ�´�ʹ��ʱ�������������Ѿ��ı�
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
		
		// ������ַ��������˻�����ĩ��, ���ָ������Ϊ��������ʼ��
		if(s_pCurrent + nThisTimeStringLength > pEndPosition)
		{
			s_pCurrent = s_szIPString;
		}
		
		// ����� s_pCurrent ����������β֮���ж�������
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
	//  ���ַ�����ʽ��IPתΪ�����ֽ��������
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