////////////////////////////////////////////////
//     FileName:    gpeffect.cpp
//     Author:      Fish
//     Date:        2007-1-26 10:18:50
//     Description: []
////////////////////////////////////////////////

#include "Top-KartPCH.h"
#include "gpeffect.h"
#include "eventviewer.h"
#include "RMOperator.h"
#include "common.h"
#include "gpcontrol.h"
#include "nifloader.h"
#include "nifop.h"

CGPEffectMgr::CGPEffectMgr()/*:
m_arrUnusePool(0)*/
{
//	InitializeCriticalSection(&critSec);
	LoadEffects();
}

CGPEffectMgr::~CGPEffectMgr()
{
	/*
	UINT nCnt=0,nNum=m_arrUnusePool.size();
	for (;nCnt<nNum;++nCnt)
	{
		std::list<NiNodePtr>::iterator it=m_arrUnusePool[nCnt].begin();
		for (;it!=m_arrUnusePool[nCnt].end();++it)
		{
			*it=NULL;
		}
	}
	DeleteCriticalSection(&critSec);
	*/
}

//extern void FindAndAdd(NiObjectNET * pkRoot);
/*
LoadEffects:
从脚本配置中读取项目，加载特效模型资源
*/
BOOL CGPEffectMgr::LoadEffects()
{
	CScriptState ls;
	if ( 0 == ls.DoFile("config/effects.lua") )
	{
		CVarArray varRoot("effects");
		varRoot.GetFromScript(&ls);
		UINT nNum=varRoot.GetNum();
		//初始化模型池
		//m_arrUnusePool.resize(nNum);
		//UINT nCntIns=0;
		for ( int i = 0; i < nNum; ++i )
		{
			CVarTable* pVar = (CVarTable*)varRoot.Element(i);
			{
				/*
				NiNodePtr spEffect = _LoadEffect(pVar->Element("id")->String());
				m_Effects[i + 1] = spEffect;
				for (nCntIns=0;nCntIns<conEffectPoolInitNum;++nCntIns)
				{
					m_arrUnusePool[i].push_back(NiDynamicCast(NiNode, spEffect->Clone()));
				}*/
				m_Effects[i + 1] = pVar->Element("id")->String();
			}
		}
	}
	
	return TRUE;
}

void CGPEffectMgr::Update(DWORD)
{
}

void CGPEffectMgr::Render(NiCamera* pCamera)
{
}

/*
NewEffect:
从已加载资源中Clone出一个（多处使用）
*/
NiNode* CGPEffectMgr::NewEffect(DWORD nEffect, BOOL bClone)
{
	if ( CNifLoader::Get() )
	{
		EFFECT_MAP::iterator it = m_Effects.find(nEffect);
		if ( it != m_Effects.end() )
		{
			return CNifLoader::Get()->Load(NiApplication::ConvertMediaFilename(it->second.c_str()));
		}
	}
	return NULL;
	/*
	_ASSERTE(nEffect>0);
	NiNodePtr spNode=NULL;
	EnterCriticalSection( &critSec );
	if(m_Effects.find(nEffect)!=m_Effects.end())
	{
		if ( bClone )
		{//克隆模式，先尝试在模型池中调用
			if (!m_arrUnusePool[nEffect-1].empty())
			{//有可用的
				std::list<NiNodePtr>::iterator it=m_arrUnusePool[nEffect-1].begin();
				for (;it!=m_arrUnusePool[nEffect-1].end();++it)
				{
					if(NULL==((NiNode*)*it)->GetParent())
					{//没有parent，可用
						spNode=*it;
						break;
					}
				}
				//spNode=m_arrUnusePool[nEffect-1].front();
				//m_arrUnusePool[nEffect-1].pop_front();
				//m_arrUsingPool[nEffect-1].push_back(spNode);	//在已使用链中登记
			}
			//
			if (!spNode)
			{//没有可用的,分配新的克隆实例
				spNode = NiDynamicCast(NiNode, m_Effects[nEffect]->Clone());
				m_arrUnusePool[nEffect-1].push_back(spNode);
				//m_arrUsingPool[nEffect-1].push_back(spNode);	//在已使用链中登记
			}
		}
		else
		{
			spNode=m_Effects[nEffect];
		}
	}
	LeaveCriticalSection( &critSec );
	char chName[10] = "";
	sprintf(chName, "%d", nEffect);
	spNode->SetName(chName);
	return spNode;
	*/
}
/*
bool CGPEffectMgr::ReleaseEffect(DWORD nEffect,NiNodePtr pkNode)
{
	_ASSERTE(nEffect>0);
	EnterCriticalSection( &critSec );
	//确保在链中可以找到
	bool bRes=false;
	if (std::find(m_arrUsingPool[nEffect-1].begin(),
		m_arrUsingPool[nEffect-1].end(),pkNode)!=m_arrUsingPool[nEffect-1].end())
	{
		m_arrUsingPool[nEffect-1].remove(pkNode);
		m_arrUnusePool[nEffect-1].push_back(pkNode);	//重新入可用池
		bRes=true;
	}
	else
	{
		_ASSERTE(0);
	}
	LeaveCriticalSection( &critSec );
	return bRes;
}
*/

//void PrepackNode(NiObjectNET* pkRoot);
/*
_LoadEffect:
从指定路径加载资源
*/
/*
NiNodePtr CGPEffectMgr::_LoadEffect(const char* pszEffect)
{
	NiNodePtr spNode = RMOperator::OpenNifAndPrepack(pszEffect);
	if ( spNode )
	{
		FindAndAdd(spNode);
		PrepackNode(spNode);
	}
	return spNode;
}

NiCamera*	CGPEffectMgr::FindCamera(NiAVObject* pRoot)
{
	assert(pRoot);
	if ( NiIsKindOf(NiCamera, pRoot) )
	{
		return (NiCamera*)pRoot;
	}
	if ( NiIsKindOf(NiNode, pRoot) )
	{
		NiNode* pNode = (NiNode*)pRoot;
		for ( unsigned int i = 0; i < pNode->GetArrayCount(); ++i )
		{
			NiAVObject* pObj = pNode->GetAt(i);
			if ( pObj )
			{
				NiCamera* pCam = FindCamera(pObj);
				if ( pCam )
				{
					return pCam;
				}
			}
		}
	}
	return NULL;
}
*/

CGPAffect_Effect::CGPAffect_Effect():
m_spNode(NULL),
m_pCamera(NULL)
{
}

CGPAffect_Effect::~CGPAffect_Effect()
{
}

void CGPAffect_Effect::Start()
{
	CGPAffectBase::Start();
	IGPTarget* pTarget = m_pDestTarget;
	if ( m_dwOption & EAO_AffectMeInstead )
	{
		pTarget = m_pSrcTarget;
	}
	if ( pTarget )
	{
		m_spNode = CGPEffectMgr::Get()->NewEffect((DWORD)m_fParam);
		m_pCamera = CNifOp::FindCamera(m_spNode);
		IGPTargetIterator* pIter = CGPControl::Get()->GetTargetIterator();
		IGPTarget* pHost = pIter ? pIter->GetHost() : NULL;
		if ( m_pCamera && pHost == m_pSrcTarget )
		{
			EventViewerMgr::StartItemEvent(m_pCamera);
		}

		if ( m_dwOption & EAO_AttachToWorld )
		{
			NiPoint3 kPos = pTarget->GetPos();
			m_spNode->SetTranslate(kPos);
			CLevelManager::Get()->AttachObject(m_spNode);
		}
		else
		{
			pTarget->Attach(m_spNode);
		}
	}
}

void CGPAffect_Effect::Stop()
{
	CGPAffectBase::Stop();
	if ( m_pCamera )
	{
		EventViewerMgr::CloseItemEvent();
	}
	IGPTarget* pTarget = m_pDestTarget;
	if ( m_dwOption & EAO_AffectMeInstead )
	{
		pTarget = m_pSrcTarget;
	}
	if ( pTarget )
	{
		if ( m_dwOption & EAO_AttachToWorld )
		{
			CLevelManager::Get()->DetachObject(m_spNode);
		}
		else
		{
			pTarget->Detach(m_spNode);
		}
	}
}

#include "nifop.h"
void CGPAffect_Effect::Update(DWORD)
{
	if ( !m_spNode )
	{
		return;
	}
	if ( m_dwOption & EAO_FaceToPeer )
	{
		IGPTarget* pSrcTarget = m_pSrcTarget;
		IGPTarget* pDestTarget = m_pDestTarget;
		if ( m_dwOption & EAO_AffectMeInstead )
		{
			pSrcTarget = m_pDestTarget;
			pDestTarget = m_pSrcTarget;
		}
		NiPoint3 kSrcPos = pSrcTarget->GetPos();
		NiPoint3 kDestPos = pDestTarget->GetPos();
		NiPoint3 kDir = kSrcPos - kDestPos;
		NiPoint3 kUp(0, 0, 1);
		kDir.Unitize();
		NiPoint3 kRight = kDir.Cross(kUp);
		kRight.Unitize();
		NiMatrix3 kRot;
		kRot.SetCol(0, kDir);
		kRot.SetCol(1, kUp);
		kRot.SetCol(2, kRight);
		kRot = CNifOp::AdjustRotate(kRot);

		NiTransform kTrans = pDestTarget->GetTransform();
		NiTransform KInvertTrans;
		kTrans.Invert(KInvertTrans);
		kRot = kRot * KInvertTrans.m_Rotate;
		m_spNode->SetRotate(kRot);
		//m_spNode->SetWorldRotate(kRot);
	}
}