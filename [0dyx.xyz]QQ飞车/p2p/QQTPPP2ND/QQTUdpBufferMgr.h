// QQTUdpBufferMgr.h: interface for the CQQTUdpBufferMgr class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_QQTUDPBUFFERMGR_H__6B69ED09_427D_463D_B7E3_5F7DFE30AECF__INCLUDED_)
#define AFX_QQTUDPBUFFERMGR_H__6B69ED09_427D_463D_B7E3_5F7DFE30AECF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "AfxTempl.h"

#define SMALL_BUFFER_SIZE			128
#define BIG_BUFFER_SIZE				640
#define SMALL_BUFFER_COUNT			2048
#define BIG_BUFFER_COUNT			512

#define MAX_P2P_DEST_PLAYER					8

class CQQTUdpBufferMgr  
{
public:
	enum _buffer_state
	{
		_buffer_state_unknown = 1,
		_buffer_state_need_resend,
		_buffer_state_arrived,
	};
	class IMsgBuffer
	{
	public:
		virtual int GetRightLevel() PURE;
		virtual BOOL GetBuffer(const byte *&pBuffer, int &iBufferLength) PURE;
		virtual short GetSequence(int iTag) PURE;		
		virtual byte GetBufferState(int iTag) PURE;
		virtual DWORD GetBufferTimeStamp(int iTag) PURE;
		virtual DWORD GetBufferFirstSendTime(int iTag) PURE;

		virtual void SetBufferSeq(int iTag, short sSequence) PURE;
		virtual void SetBufferState(int iTag, byte byState) PURE;
		virtual void SetBufferTimeStamp(int iTag, DWORD dwTimeStamp) PURE;
		virtual void SetBufferFirstSendTime(int iTag, DWORD dwTimeStamp) PURE;
		
		virtual ULONG AddRef() PURE;
		virtual ULONG Release() PURE;
	};
protected:
	template <size_t _size_t_buffer_size> class _msg_buffer : public IMsgBuffer
	{
	public:
		_msg_buffer();
		~_msg_buffer(){}

		void ClearRef();
		BOOL IsFreeBuffer();
		void SetBuffer(byte *pBuffer, int iBufferLength, int iBufferRightLevel);

		virtual void SetBufferSeq(int iTag, short sSequence);
		virtual void SetBufferState(int iTag, byte byState);
		virtual void SetBufferTimeStamp(int iTag, DWORD dwTimeStamp);
		virtual void SetBufferFirstSendTime(int iTag, DWORD dwTimeStamp);
	protected:
		virtual int GetRightLevel();
	
		virtual BOOL GetBuffer(const byte *&pBuffer, int &iBufferLength);
		virtual short GetSequence(int iTag);
		virtual byte GetBufferState(int iTag);
		virtual DWORD GetBufferTimeStamp(int iTag);
		virtual DWORD GetBufferFirstSendTime(int iTag);
		
		
		virtual ULONG AddRef();
		virtual ULONG Release();		
	private:
		byte m_byBuffer[_size_t_buffer_size];
		int m_iBufferLength;
		ULONG m_ulRef;
		short m_sArSequence[MAX_P2P_DEST_PLAYER];
		DWORD m_ArdwLastSendTime[MAX_P2P_DEST_PLAYER];
		DWORD m_arDwFirstSendTime[MAX_P2P_DEST_PLAYER];
		byte m_ArbyState[MAX_P2P_DEST_PLAYER];
		//Modify By Hyson, 2005.03.14 17:22
		//For 支持事件包重发
		//BOOL m_bIsGameData;
		int m_iBufferRightLevel;
		//End Add
	};
public:
	CQQTUdpBufferMgr();
	virtual ~CQQTUdpBufferMgr();
	void Initialize();
	IMsgBuffer* AddMsgBuffer(byte *pBuffer, int iBufferLength, int iBufferRightLevel);
	void SetLastBuffer(int iClientTag, IMsgBuffer *pMsgBuffer);
	IMsgBuffer* GetLastBuffer(int iClientTag);
private:
	void RollTagForward(int &iTag, int iSizeMax);
	_msg_buffer<SMALL_BUFFER_SIZE> m_arSmallBuffer[SMALL_BUFFER_COUNT];
	_msg_buffer<BIG_BUFFER_SIZE> m_arBigBuffer[BIG_BUFFER_COUNT];
	int m_iSmallBufferTag;
	int m_iBigBufferTag;

	//Add By Hyson, 2005.03.14 17:19
	//For 
	_msg_buffer<BIG_BUFFER_SIZE> m_arLastBuffer[MAX_P2P_DEST_PLAYER];
	//End Add	
};

//////////////////////////////////////////////////////////////////////////
// implementation of template <size_t _size_t_buffer_size> class _msg_buffer : public IMsgBuffer
template <size_t _size_t_buffer_size>
CQQTUdpBufferMgr::_msg_buffer<_size_t_buffer_size>::_msg_buffer() : m_ulRef(0), m_iBufferLength(0){}
template <size_t _size_t_buffer_size>
void CQQTUdpBufferMgr::_msg_buffer<_size_t_buffer_size>::ClearRef()
{
	m_ulRef = 0;
	m_iBufferLength = 0;
}
template <size_t _size_t_buffer_size>
BOOL CQQTUdpBufferMgr::_msg_buffer<_size_t_buffer_size>::IsFreeBuffer()
{
	return(0 == m_ulRef) ? TRUE : FALSE;
}
template <size_t _size_t_buffer_size>
void CQQTUdpBufferMgr::_msg_buffer<_size_t_buffer_size>::SetBuffer(byte *pBuffer, int iBufferLength, int iBufferRightLevel)
{
	ASSERT(0 == m_ulRef);
	ASSERT(iBufferLength > 0 && iBufferLength <= _size_t_buffer_size);
	ASSERT(pBuffer);
	::memcpy(m_byBuffer, pBuffer, (m_iBufferLength = iBufferLength));
	m_ulRef = 1;
	m_iBufferRightLevel = iBufferRightLevel;
}
template <size_t _size_t_buffer_size>
BOOL CQQTUdpBufferMgr::_msg_buffer<_size_t_buffer_size>::GetBuffer(const byte *&pBuffer, int& iBufferLength)
{
	if(m_ulRef)
	{
		pBuffer = m_byBuffer;
		iBufferLength = m_iBufferLength;
		return TRUE;
	}
	pBuffer = NULL;
	iBufferLength = 0;
	return FALSE;	
}
template <size_t _size_t_buffer_size>
ULONG CQQTUdpBufferMgr::_msg_buffer<_size_t_buffer_size>::AddRef()
{
	return ++m_ulRef;
}
template <size_t _size_t_buffer_size>
ULONG CQQTUdpBufferMgr::_msg_buffer<_size_t_buffer_size>::Release()
{
	return --m_ulRef;
}
template <size_t _size_t_buffer_size>
void CQQTUdpBufferMgr::_msg_buffer<_size_t_buffer_size>::SetBufferSeq(int iTag, short sSequence)
{
	if(iTag >= 0 && iTag < MAX_P2P_DEST_PLAYER)
	{
		m_sArSequence[iTag] = sSequence;
		return;
	}
	ASSERT(FALSE);
}
template <size_t _size_t_buffer_size>
short CQQTUdpBufferMgr::_msg_buffer<_size_t_buffer_size>::GetSequence(int iTag)
{
	if(iTag >= 0 && iTag < MAX_P2P_DEST_PLAYER)
	{
		return m_sArSequence[iTag];
	}
	ASSERT(FALSE);
	return 0;
}
template <size_t _size_t_buffer_size>
int CQQTUdpBufferMgr::_msg_buffer<_size_t_buffer_size>::GetRightLevel()
{
	return m_iBufferRightLevel;
}
template <size_t _size_t_buffer_size>
void CQQTUdpBufferMgr::_msg_buffer<_size_t_buffer_size>::SetBufferState(int iTag, byte byState)
{
	if(iTag >= 0 && iTag < MAX_P2P_DEST_PLAYER)
	{
		m_ArbyState[iTag] = byState;
	}
	else
	{
		ASSERT(FALSE);
	}
}
template <size_t _size_t_buffer_size>
void CQQTUdpBufferMgr::_msg_buffer<_size_t_buffer_size>::SetBufferTimeStamp(int iTag, DWORD dwTimeStamp)
{
	if(iTag >= 0 && iTag < MAX_P2P_DEST_PLAYER)
	{
		m_ArdwLastSendTime[iTag] = dwTimeStamp;
	}
	else
	{
		ASSERT(FALSE);
	}
}
template <size_t _size_t_buffer_size>
void CQQTUdpBufferMgr::_msg_buffer<_size_t_buffer_size>::SetBufferFirstSendTime(int iTag, DWORD dwTimeStamp)
{
	if(iTag >= 0 && iTag < MAX_P2P_DEST_PLAYER)
	{
		m_arDwFirstSendTime[iTag] = dwTimeStamp;
	}
	else
	{
		ASSERT(FALSE);
	}
}
template <size_t _size_t_buffer_size>
DWORD CQQTUdpBufferMgr::_msg_buffer<_size_t_buffer_size>::GetBufferFirstSendTime(int iTag)
{
	if(iTag >= 0 && iTag < MAX_P2P_DEST_PLAYER)
	{
		return m_ArdwLastSendTime[iTag];
	}
	ASSERT(FALSE);
	return 0;
}
template <size_t _size_t_buffer_size>
byte CQQTUdpBufferMgr::_msg_buffer<_size_t_buffer_size>::GetBufferState(int iTag)
{
	if(iTag >= 0 && iTag < MAX_P2P_DEST_PLAYER)
	{
		return m_ArbyState[iTag];
	}
	ASSERT(FALSE);
	return 0;
}
template <size_t _size_t_buffer_size>
DWORD CQQTUdpBufferMgr::_msg_buffer<_size_t_buffer_size>::GetBufferTimeStamp(int iTag)
{
	if(iTag >= 0 && iTag < MAX_P2P_DEST_PLAYER)
	{
		return m_ArdwLastSendTime[iTag];
	}
	ASSERT(FALSE);
	return 0;
}
#endif // !defined(AFX_QQTUDPBUFFERMGR_H__6B69ED09_427D_463D_B7E3_5F7DFE30AECF__INCLUDED_)
