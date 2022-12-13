#ifndef __NIFALLOC_H__
#define __NIFALLOC_H__

#include "singlet.h"
#include "idgen.h"
#include "map"
using namespace std;

typedef map<DWORD, NiNode*>	NIFMAP;
class CNifAlloc : public CSingleT<CNifAlloc>
{
public:
	CNifAlloc();
	~CNifAlloc();

	DWORD	Alloc(LPCSTR pstrNifFileName);
	void	Free(DWORD dwHandle);
	void	Clear();
	NiNode*	GetObject(DWORD dwHandle);
	void	Update(float fAccuTime);

private:
	CIDGen	m_kIDGen;
	NIFMAP	m_kMap;
};

#endif