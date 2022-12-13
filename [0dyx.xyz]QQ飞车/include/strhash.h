#ifndef __STRHASH_H__
#define __STRHASH_H__

#include "idgen.h"
#include "singlet.h"
#include "map"
#include "string"
using namespace std;

class CStrHash : public CSingleT<CStrHash>
{
	typedef map<string, DWORD> STRING_MAP;

public:
	DWORD	Alloc(const char* pstr)
	{
		STRING_MAP::iterator it = m_kStrMap.find(pstr);
		if ( it == m_kStrMap.end() )
		{
			DWORD dwCookie = m_kIDGen.Alloc();
			m_kStrMap[pstr] = dwCookie;
			return dwCookie;
		}
		else
		{
			return m_kStrMap[pstr];
		}
	}

	void	Free(DWORD dwCookie)	
	{
		STRING_MAP::iterator it = m_kStrMap.begin();
		for ( ; it != m_kStrMap.end(); ++it )
		{
			if ( it->second == dwCookie )
			{
				m_kStrMap.erase(it);
				m_kIDGen.Free(dwCookie);
				return;
			}
		}	
	}

	const char*	Value(DWORD dwCookie)
	{
		STRING_MAP::iterator it = m_kStrMap.begin();
		for ( ; it != m_kStrMap.end(); ++it )
		{
			if ( it->second == dwCookie )
			{
				return it->first.c_str();
			}
		}
		return NULL;
	}

private:
	CIDGen		m_kIDGen;
	STRING_MAP	m_kStrMap;
};
#endif