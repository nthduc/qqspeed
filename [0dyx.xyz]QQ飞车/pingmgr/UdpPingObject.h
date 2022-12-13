#if !defined(AFX_UPDPINGOJBECT_H__AB44DE29_FE48_42B7_815A_04332177AFF0__INCLUDED_)
#define AFX_UPDPINGOJBECT_H__AB44DE29_FE48_42B7_815A_04332177AFF0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// UpdPingOjbect.h : header file
//

#include <afxsock.h>
#include "IPingObject.h"

#define UDPPING_CMD		0x01

#pragma pack(push)
#pragma pack(1)

typedef struct __UDPPING_DATA{
	DWORD dwCmd;
	USHORT ushSeq;
	DWORD dwTimeStamp;
	DWORD dwFillData;		//用于验证的信息
}UDPPING_DATA;


/////////////////////////////////////////////////////////////////////////////
// CUpdPingOjbect command target

class CUdpPingObject : public CAsyncSocket, public IPingObject
{
// Attributes
public:

// Operations
public:
	CUdpPingObject();
	virtual ~CUdpPingObject();

// Overrides
public:
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUpdPingOjbect)
	//}}AFX_VIRTUAL

	// Generated message map functions
	//{{AFX_MSG(CUpdPingOjbect)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	virtual void Ping(unsigned int ulSvrIP, unsigned short usSvrPort, int nPingCount, int nStartTime, int nLimitTime);

	virtual void CancelPing();

	virtual void GetPingResult(int &nTTL, float &fLossRate, int &nAverageRTT);
	
	void StartPings();

	virtual void OnReceive(int nErrorCode);
	
	virtual BOOL CanDelete();

	virtual void CheckCanDelete() {}

	int m_nPingCount;
	int m_nReplyCount;

	int m_nTotalTTL;
	int m_nTotalRTT;
	
	int m_cResult;
	int m_nSeq;
	
// Implementation
protected:
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_UPDPINGOJBECT_H__AB44DE29_FE48_42B7_815A_04332177AFF0__INCLUDED_)
