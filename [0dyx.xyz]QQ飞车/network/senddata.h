#ifndef __SENDDATA_H__
#define __SENDDATA_H__

#include "list"
using namespace std;

class CSendData
{
public:
	CSendData(const CSendData& r)
	{
		__construct(r.m_nLen, r.m_byData);	
	}

	CSendData(UINT nLen, const void* pData)
	{
		__construct(nLen, pData);
	}

	void __construct(UINT nLen, const void* pData)
	{
		m_nLen = nLen;
		m_byData = new BYTE[nLen];
		memcpy(m_byData, pData, sizeof(BYTE) * nLen);
	}

	~CSendData()
	{
		delete[] m_byData;
	}

	UINT m_nLen;
	BYTE* m_byData;
};

typedef list<CSendData>				SENDDATA_LIST;

#endif