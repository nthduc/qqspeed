////////////////////////////////////////////////
//     FileName:    idgen.h
//     Author:      Fish
//     Date:        2007-2-2 10:19:22
//     Description: []
////////////////////////////////////////////////

#ifndef _IDGEN_H_
#define _IDGEN_H_

#include "list"
using namespace std;

typedef unsigned long DWORD;
#define IDGEN_BASEID	1
#define INVALIDID		((DWORD)0)

class CIDGen
{
public:
	CIDGen():
	m_dwAllocIndex(IDGEN_BASEID)
	{
	}

	DWORD	Alloc()
	{
		if ( m_listUnused.size() > 0 )
		{
			DWORD dwID = *m_listUnused.begin();
			m_listUnused.pop_front();
			return dwID;
		}
		else
		{
			return m_dwAllocIndex++;
		}
	}

	void	Free(DWORD dwID)
	{
		m_listUnused.push_back(dwID);
	}

	void Reset()
	{
		m_listUnused.clear();
		m_dwAllocIndex = IDGEN_BASEID;
	}

	typedef list<DWORD> ID_LIST;
	typedef ID_LIST::iterator ID_LIST_Iterator;

private:
	ID_LIST	m_listUnused;
	DWORD	m_dwAllocIndex;
};
#endif