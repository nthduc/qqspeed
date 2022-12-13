#include "Top-KartPCH.h"
#include "NifAlloc.h"
#include "RMOperator.h"

CNifAlloc::CNifAlloc()
{
}

CNifAlloc::~CNifAlloc()
{
	Clear();
}

DWORD	CNifAlloc::Alloc(LPCSTR pstrNifFileName)
{
	assert(pstrNifFileName);
	NiNodePtr pkNode = RMOperator::OpenNifAndPrepack(pstrNifFileName);
	if ( pkNode )
	{
		DWORD dwHandle = m_kIDGen.Alloc();
		m_kMap[dwHandle] = pkNode;
		pkNode->IncRefCount();
		pkNode->Update(0);
		NiTimeController::StartAnimations(pkNode);
		return dwHandle;
	}
	return INVALIDID;
}

void	CNifAlloc::Free(DWORD dwHandle)
{
	NIFMAP::iterator it = m_kMap.find(dwHandle);
	if ( it != m_kMap.end() )
	{
		NiAVObject* pkObj = it->second;
		NiDelete pkObj;
		m_kMap.erase(it);
	}
}

void CNifAlloc::Clear()
{
	if ( m_kMap.size() == 0 )
		return;

	NIFMAP::iterator it = m_kMap.begin();
	for ( ; it != m_kMap.end(); ++it )
	{
		NiAVObject* pkObj = it->second;
		NiDelete pkObj;
	}
	m_kMap.clear();
}

NiNode*	CNifAlloc::GetObject(DWORD dwHandle)
{
	NIFMAP::iterator it = m_kMap.find(dwHandle);
	if ( it != m_kMap.end() ) 
	{
		return it->second;
	}
	return NULL;
}

void CNifAlloc::Update(float fAccuTime)
{
	NIFMAP::iterator it = m_kMap.begin();
	for ( ; it != m_kMap.end(); ++it )
	{
		NiAVObject* pkObj = it->second;
		pkObj->Update(fAccuTime);
	}
}