////////////////////////////////////////////////
//     FileName:    physxtask.h
//     Author:      Fish
//     Date:        2007-2-9 10:57:50
//     Description: 本文件实现了物理模型的添加、删除、属性修改
//	因为物理模拟在后台线程，如果在主线程中做这些操作，不可避免的
//	产生共享冲突问题，所以封装了一个类似操作队列的东西，主线程需要
//	这些操作的时候先把命令push进队列，最后在fectch和simulate之间来
//	完成这些操作，避免了多线程冲突
////////////////////////////////////////////////

#ifndef _PHYSXTASK_H_
#define _PHYSXTASK_H_

#include "common.h"
#include "singlet.h"
#include "idgen.h"
#include "list"
#include "map"
//#include "PhysXManager.h"
using namespace std;

class XUserData;

typedef DWORD	HACTOR;
#define INVALID_HACTOR	0

class CPhysXTask : public CSingleT<CPhysXTask>
{
public:
	typedef struct tagADDACTORTASK 
	{
		NxScene*	pScene;
		NxActorDesc kActorDesc;
		DWORD		hActor;
	}ADDACTORTASK, *PADDACTORTASK, *LPADDACTORTASK;

	typedef struct tagREMOVEACTORTASK
	{
		NxScene*	pScene;
		DWORD		hActor;
	}REMOVEACTORTASK, *PREMOVEACTORTASK, *LPREMOVEACTORTASK;

	typedef struct tagMODIFYFLAGTASK
	{
		DWORD		hActor;
		NxActorFlag Flag;
	}MODIFYFLAGTASK, *PMODIFYFLAGTASK, *LPMODIFYFLAGTASK;

	typedef list<ADDACTORTASK>		ADDACTORTASK_LIST;
	typedef list<REMOVEACTORTASK>	REMOVEACTORTASK_LIST;
	typedef list<MODIFYFLAGTASK>	MODIFYFLAGTASK_LIST;
	typedef map<DWORD, NxActor*>	ACTORMAP;

	/*
	createActor:
	创建物理对象
	comment:
	kActorDesc中的shapes不能在堆上分配，在这里不会作处理
	*/
	HACTOR createActor(NxScene* pScene, NxActorDesc& kActorDesc)
	{
		assert(pScene);
		HACTOR	hActor = m_IDGen.Alloc();
		ADDACTORTASK kTask = { pScene, kActorDesc, hActor };
		m_listAddActorTask.push_back(kTask);
		return hActor;
	}

	/*
	releaseActor:
	删除物理对象
	*/
	HACTOR releaseActor(NxScene* pScene, HACTOR hActor)
	{
		assert(pScene);
		REMOVEACTORTASK kTask = { pScene, hActor };
		m_listRemoveActorTask.push_back(kTask);
		return hActor;
	}

	/*
	raiseActorFlag
	*/
	void raiseActorFlag(HACTOR hActor, NxActorFlag Flag)
	{
		MODIFYFLAGTASK kTask = { hActor, Flag };
		m_listRaiseActorFlagTask.push_back(kTask);
	}

	/*
	clearActorFlag
	*/
	void clearActorFlag(HACTOR hActor, NxActorFlag Flag)
	{
		MODIFYFLAGTASK kTask = { hActor, Flag };
		m_listClearActorFlagTask.push_back(kTask);
	}

	void RemoveAll()
	{
		m_listAddActorTask.clear();
		m_listRemoveActorTask.clear();
		m_listRaiseActorFlagTask.clear();
		m_listClearActorFlagTask.clear();
	}

	/*
	Process:
	处理一帧
	*/
	void Process()
	{
		{
			ADDACTORTASK_LIST::iterator it = m_listAddActorTask.begin();
			for ( ; it != m_listAddActorTask.end(); ++it )
			{
				LPADDACTORTASK pTask = &*it;
				NxActor* pActor = pTask->pScene->createActor(pTask->kActorDesc);
				NxArray<NxShapeDesc*>::iterator it = pTask->kActorDesc.shapes.begin();
				for ( ; it != pTask->kActorDesc.shapes.end(); ++it )
				{
					NxShapeDesc* pShapeDesc = *it;
					delete pShapeDesc;
				}
				assert(pActor);
				m_ActorMap[pTask->hActor] = pActor;
			}
		}
		{
			MODIFYFLAGTASK_LIST::iterator it = m_listRaiseActorFlagTask.begin();
			for ( ; it != m_listRaiseActorFlagTask.end(); ++it )
			{
				LPMODIFYFLAGTASK pTask = &*it;
				ACTORMAP::iterator itFind = m_ActorMap.find(pTask->hActor);
				if ( itFind != m_ActorMap.end() )
				{
					NxActor* pActor = itFind->second;
					if ( pActor )
					{
						pActor->raiseActorFlag(pTask->Flag);
					}
				}
				else
				{
					XWaring("raiseActorFlag:handle not found");
				}
			}
		}
		{
			MODIFYFLAGTASK_LIST::iterator it = m_listClearActorFlagTask.begin();
			for ( ; it != m_listClearActorFlagTask.end(); ++it )
			{
				LPMODIFYFLAGTASK pTask = &*it;
				ACTORMAP::iterator itFind = m_ActorMap.find(pTask->hActor);
				if ( itFind != m_ActorMap.end() )
				{
					NxActor* pActor = itFind->second;
					if ( pActor )
					{
						pActor->clearActorFlag(pTask->Flag);
					}
				}
				else
				{
					XWaring("clearActorFlag:handle not found");
				}
			}
		}
		{
			REMOVEACTORTASK_LIST::iterator it = m_listRemoveActorTask.begin();
			for ( ; it != m_listRemoveActorTask.end(); ++it )
			{
				LPREMOVEACTORTASK pTask = &*it;
				ACTORMAP::iterator itFind = m_ActorMap.find(pTask->hActor);
				if ( itFind != m_ActorMap.end() )
				{
					XUserData* pUd = (XUserData*)(itFind->second)->userData;
					SAFE_DELETE(pUd);

					pTask->pScene->releaseActor(*itFind->second);
					m_IDGen.Free(itFind->first);
					m_ActorMap.erase(itFind);
				}	
				else
				{
					XWaring("releaseActor:handle not found");
				}
			}
		}
		RemoveAll();
	}

private:
	ADDACTORTASK_LIST		m_listAddActorTask;
	REMOVEACTORTASK_LIST	m_listRemoveActorTask;
	MODIFYFLAGTASK_LIST		m_listRaiseActorFlagTask;
	MODIFYFLAGTASK_LIST		m_listClearActorFlagTask;
	CIDGen					m_IDGen;
	ACTORMAP				m_ActorMap;
};

#endif