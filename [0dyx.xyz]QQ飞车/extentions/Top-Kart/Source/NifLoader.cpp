#include "Top-KartPCH.h"
#include "NifLoader.h"
#include "CallbackStream.h"
#include "screendraw.h"

CNifLoader::CNifLoader():
m_dwPoolSize(DEFAULT_NIFLOADER_POOLSIZE)
{
}

CNifLoader::~CNifLoader()
{
	Clear();
}

void CNifLoader::SetPoolSize(DWORD dwPoolSize)
{
	USE_AUTOCS;
	if ( dwPoolSize )
	{
		m_dwPoolSize = dwPoolSize;
	}
	else
	{
		m_dwPoolSize = DEFAULT_NIFLOADER_POOLSIZE;
	}
	SweepGarbage();
}

NiNode* CNifLoader::Load(LPCSTR pstrNifFileName, INifLoaderEvent* pEvent, 
						 DWORD* pdwCookie, DWORD dwExtraData, bool bShared, bool* pbWouldBlock)
{
	DWORD dwCookie = m_kStrHash.Alloc(pstrNifFileName);
	if ( pdwCookie )
		*pdwCookie = dwCookie;
	return Load(dwCookie, pEvent, dwExtraData, bShared, pbWouldBlock);
}

NiNode* CNifLoader::Load(DWORD dwCookie, INifLoaderEvent* pEvent, 
						 DWORD dwExtraData, bool bShared, bool* pbWouldBlock)
{
	USE_AUTOCS;

	SweepGarbage();

	if ( pbWouldBlock ) //默认为非阻塞方式加载
	{
		*pbWouldBlock = true;
	}

	NIF_LOAD_DATA_MAP::iterator it = m_kLoadMap.find(dwCookie);
	if ( it != m_kLoadMap.end() )
	{
		NIF_LOAD_DATA* pLoadData = it->second;
		while( pLoadData )
		{
			if ( pLoadData->bShared || pLoadData->pkNode->GetRefCount() == 1 )
			{	//如果资源已经存在，而且可以被重用(要么是可以共享的，要么是没有人用的)
				pLoadData->dwRef++;
				pLoadData->dwLastRefTime = GetTickCount();
				if ( pbWouldBlock )
				{
					*pbWouldBlock = false;
				}
				XTrace("====NifLoader: %s already exist, good!", m_kStrHash.Value(dwCookie));
				return pLoadData->pkNode;
			}
			pLoadData = pLoadData->pNext;
		}
	}

	const char* pstrNifFileName = m_kStrHash.Value(dwCookie);
	assert(pstrNifFileName);
	
	NiNode* pkNode = NiNew NiNode;

	WIN32_FIND_DATAA kFindData;
	HANDLE hFind = FindFirstFile(pstrNifFileName, &kFindData);
	if ( hFind == INVALID_HANDLE_VALUE )
	{
		return pkNode;	//如果文件不存在，返回默认Node
	}
	FindClose(hFind);

	XTrace("====NifLoader: now load %s", m_kStrHash.Value(dwCookie));
	CallbackStream* pStream = NiNew CallbackStream(this, dwCookie);
	pStream->BackgroundLoadBegin(pstrNifFileName);

	pkNode->IncRefCount();
	NIF_LOAD_DATA* pkLoadData = new NIF_LOAD_DATA(pStream, pkNode, dwExtraData, pEvent, bShared, 1, kFindData.nFileSizeLow, GetTickCount());
	it = m_kLoadMap.find(dwCookie);
	if ( it != m_kLoadMap.end() )
	{
		NIF_LOAD_DATA* pLoadData = it->second;
		while( pLoadData->pNext )
		{
			pLoadData = pLoadData->pNext;
		}
		pLoadData->pNext = pkLoadData;
	}
	else
	{
		m_kLoadMap[dwCookie] = pkLoadData;
	}
	return pkNode;
}

NiNode* CNifLoader::LoadTexture(LPCSTR pstrTexFileName, DWORD* pdwCookie, bool* pbWouldBlock)
{
	DWORD dwCookie = m_kStrHash.Alloc(pstrTexFileName);
	if ( pdwCookie )
		*pdwCookie = dwCookie;
	return LoadTexture(dwCookie, pbWouldBlock);
}

NiNode* CNifLoader::LoadTexture(DWORD dwCookie, bool* pbWouldBlock)
{
	SweepGarbage();

	if ( pbWouldBlock ) //总是同步加载
	{
		*pbWouldBlock = false;
	}

	NIF_LOAD_DATA_MAP::iterator it = m_kLoadMap.find(dwCookie);
	if ( it != m_kLoadMap.end() )
	{
		NIF_LOAD_DATA* pLoadData = it->second;
		while( pLoadData )
		{
			if ( pLoadData->bShared || pLoadData->pkNode->GetRefCount() == 1 )
			{	//如果资源已经存在，而且可以被重用(要么是可以共享的，要么是没有人用的)
				pLoadData->dwRef++;
				pLoadData->dwLastRefTime = GetTickCount();
				XTrace("====NifLoader: %s already exist, good!", m_kStrHash.Value(dwCookie));
				return pLoadData->pkNode;
			}
			pLoadData = pLoadData->pNext;
		}
	}

	const char* pstrTexFileName = m_kStrHash.Value(dwCookie);
	assert(pstrTexFileName);

	XTrace("====NifLoader: now load %s", m_kStrHash.Value(dwCookie));
	NiNode* pkNode = CScreenDraw::CreateTextureObject(NiApplication::ConvertMediaFilename(pstrTexFileName));

	WIN32_FIND_DATAA kFindData;
	HANDLE hFind = FindFirstFile(pstrTexFileName, &kFindData);
	if ( hFind == INVALID_HANDLE_VALUE )
	{
		return pkNode;	//如果文件不存在，返回默认Node
	}
	FindClose(hFind);

	pkNode->IncRefCount();
	NIF_LOAD_DATA* pkLoadData = new NIF_LOAD_DATA(NULL, pkNode, 0, NULL, TRUE, 1, kFindData.nFileSizeLow, GetTickCount());
	it = m_kLoadMap.find(dwCookie);
	if ( it != m_kLoadMap.end() )
	{
		NIF_LOAD_DATA* pLoadData = it->second;
		while( pLoadData->pNext )
		{
			pLoadData = pLoadData->pNext;
		}
		pLoadData->pNext = pkLoadData;
	}
	else
	{
		m_kLoadMap[dwCookie] = pkLoadData;
	}
	return pkNode;
}

void CNifLoader::Update(float fTime)
{
	LOAD_CACHE_DATA_LIST::iterator itCache = m_kLoadCache.begin();
	for ( ; itCache != m_kLoadCache.end(); ++itCache )
	{
		LOAD_CACHE_DATA* pCacheData = &*itCache;
		if ( !pCacheData )
			continue;
		DWORD dwCookie = pCacheData->dwCookie;
		DWORD dwExtraData = pCacheData->dwExtraData;
		DWORD dwThreadState = pCacheData->dwThreadState;

		NIF_LOAD_DATA_MAP::iterator it = m_kLoadMap.find(dwCookie);
		if ( it != m_kLoadMap.end() )
		{
			NIF_LOAD_DATA* pLoadData = it->second;
			while( pLoadData )
			{
				CallbackStream* pStream = pLoadData->pStream;
				if ( (DWORD)pStream == dwExtraData )
				{
					XTrace("====NifLoader: %s load completed.", m_kStrHash.Value(dwCookie));
					for ( unsigned int i = 0; i < pStream->GetObjectCount(); i++ )
					{
						NiObject* pkObject = pStream->GetObjectAt(i);
						if (NiIsKindOf(NiNode, pkObject))
						{
							pLoadData->pkNode->AttachChild((NiNode*)pkObject);
						}
					}
					//不用GetAccumTime还真不行！
					float fAccuTime = GetMainApp()->GetAccumTime();
					NiTimeController::StartAnimations(pLoadData->pkNode, fAccuTime);
					NiNode* pkNode = pLoadData->pkNode;
					//if ( pkNode->GetParent() )
					//{
					//	pkNode = pkNode->GetParent();
					//}
					pkNode->UpdateNodeBound();
					pkNode->UpdateEffects();
					pkNode->UpdateProperties();
					pkNode->Update(fAccuTime);
					if ( pLoadData->pEvent )
					{
						pLoadData->pEvent->OnLoadExit(dwCookie, dwThreadState, pLoadData->dwExtraData);
					}
					break;
				}
				pLoadData = pLoadData->pNext;
			}
		}
		else
		{
			XWaring("NifLoader: no cookie=%d", dwCookie);
		}
	}
	m_kLoadCache.clear();
}

bool CNifLoader::OnLoadExit(DWORD dwCookie, DWORD dwThreadState, DWORD dwExtraData)
{
	USE_AUTOCS;
	LOAD_CACHE_DATA kCacheData = { dwCookie, dwThreadState, dwExtraData };
	m_kLoadCache.push_back(kCacheData);
	return true;
}

bool	CNifLoader::CancelLoad(INifLoaderEvent* pEvent)
{
	bool bFind = false;
	NIF_LOAD_DATA_MAP::iterator it = m_kLoadMap.begin();
	for ( ; it != m_kLoadMap.end(); ++it )
	{
		NIF_LOAD_DATA* pLoadData = it->second;
		while( pLoadData )
		{
			if ( pLoadData->pEvent == pEvent )
			{
				pLoadData->pEvent = NULL;
				bFind = true;
			}
			pLoadData = pLoadData->pNext;
		}
	}
	return bFind;
}

void CNifLoader::Clear()
{
	USE_AUTOCS;
	NIF_LOAD_DATA_MAP::iterator it = m_kLoadMap.begin();
	for ( ; it != m_kLoadMap.end(); ++it )
	{
		NIF_LOAD_DATA* pLoadData = it->second;
		delete pLoadData;
	}
	m_kLoadMap.clear();
}

DWORD CNifLoader::GetFreePoolSize()
{
	DWORD dwFreePoolSize = 0;
	NIF_LOAD_DATA_MAP::iterator it = m_kLoadMap.begin();
	for ( ; it != m_kLoadMap.end(); ++it )
	{
		NIF_LOAD_DATA* pLoadData = it->second;
		while( pLoadData )
		{
			if ( pLoadData->pkNode->GetRefCount() == 1 )
			{
				dwFreePoolSize += pLoadData->dwFileSize;
			}
			pLoadData = pLoadData->pNext;
		}
	}
	return dwFreePoolSize;
}

DWORD CNifLoader::PickIll()
{
	DWORD dwLastTime = GetTickCount();
	DWORD dwCookie = 0;
	NIF_LOAD_DATA_MAP::iterator it = m_kLoadMap.begin();
	for ( ; it != m_kLoadMap.end(); ++it )
	{
		NIF_LOAD_DATA* pLoadData = it->second;
		while( pLoadData )
		{
			if ( pLoadData->pkNode->GetRefCount() == 1
				&& pLoadData->dwLastRefTime < dwLastTime )
			{
				dwCookie = it->first;
				dwLastTime = pLoadData->dwLastRefTime;
			}
			pLoadData = pLoadData->pNext;
		}
	}
	return dwCookie;
}

void CNifLoader::SweepGarbage()
{
	DWORD dwFreePoolSize = GetFreePoolSize();
	if ( dwFreePoolSize > m_dwPoolSize )
	{
		DWORD dwCookie = PickIll();
		if ( dwCookie )
		{
			NIF_LOAD_DATA_MAP::iterator it = m_kLoadMap.find(dwCookie);
			if ( it != m_kLoadMap.end() )
			{
				NIF_LOAD_DATA* pLoadData = it->second;
				delete pLoadData;
				m_kLoadMap.erase(it);

				SweepGarbage();
			}
		}
	}
}