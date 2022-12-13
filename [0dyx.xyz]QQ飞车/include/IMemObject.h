// GameObject.h: interface for the CGameObject class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GAMEOBJECT_H__2E8217A3_0FF9_483B_AE16_FBA21D35A586__INCLUDED_)
#define AFX_GAMEOBJECT_H__2E8217A3_0FF9_483B_AE16_FBA21D35A586__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#ifdef new
#undef new
#endif

#ifdef delete
#undef delete
#endif

#ifndef MEM_MGR

#ifdef _DEBUG

#pragma comment(lib, "MemMgrD.lib")

#else

#pragma comment(lib, "MemMgr.lib")

#endif
	
#endif



#include "MemMgr.h"

class IMemObject
{
public:   	
	inline void* operator new(size_t _size)
	{
		IMemObject* pGameOBJ = (IMemObject*)CMemMgr::GetInstance().New(_size);
		
		return (void*)pGameOBJ;
	}
	
	inline void * operator new[](size_t _size)
	{
		IMemObject * pGameOBJ = (IMemObject*) CMemMgr::GetInstance().New(_size);
		return (void*)pGameOBJ;
	}
	inline void operator delete(void * pGameOBJ)
	{
		CMemMgr::GetInstance().Delete(pGameOBJ);
	}
	
	inline void operator delete[](void* pGameOBJ)
	{
		CMemMgr::GetInstance().Delete(pGameOBJ);
	}
};

#endif // !defined(AFX_GAMEOBJECT_H__2E8217A3_0FF9_483B_AE16_FBA21D35A586__INCLUDED_)
