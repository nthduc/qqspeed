/*
 *	GameImageLib.cpp
 *	game image library functions.
 *
 *	Jim, 2003.12.12.
 */
#include "Engine.h"
#include "Display.h"
#include "ImageLib.h"
#include "GameImageLib.h"
#include <assert.h>

//#define IMAGE_LOAD_SYNC
const unsigned int IMAGE_COLLOECT_TICK = 10*1000;  //超过这么长时间不用的image将被收集
const unsigned int GC_INTERVAL = 2000;   //每隔多长时间gc一次


//todo: ImageLib析构释放正在load的资源
class CMutexLock
{
public:
	CMutexLock( HANDLE hmutex ) : m_hMutex(hmutex)
	{
		WaitForSingleObject( m_hMutex, INFINITE );
	}

	~CMutexLock()
	{
		ReleaseMutex( m_hMutex );
	}

private:
	HANDLE m_hMutex;
};

static DWORD WINAPI LoadMainFunc( void *pdata )
{
	CGameImageLib *plib = (CGameImageLib *)pdata;

	for(;;)
	{
		WaitForSingleObject( plib->m_hHasReqEvent, INFINITE );
		if( plib->m_bQuit ) break;

		//得到要load的ID
		WaitForSingleObject( plib->m_hMutex, INFINITE );

		DWORD id = plib->m_ImLoadImgID;
		if( id==0 && !plib->m_RequestImg.empty() ) id = plib->m_RequestImg.front();

		if( id == 0 ) //no image to load
		{
			ResetEvent( plib->m_hHasReqEvent );
			ReleaseMutex( plib->m_hMutex );
			continue;
		}
		ReleaseMutex( plib->m_hMutex );

		//load
		CIDImage *image = new CIDImage;
		image->m_dwImageID = id;
		ULONG ref = image->AddRef();
		if( ref == 0 ) //load 不成功
		{
			delete image;
			image = NULL;
		}


		//load 完成， 改变plib数据
		WaitForSingleObject( plib->m_hMutex, INFINITE );

		if( id == plib->m_ImLoadImgID )
		{
			assert( plib->m_RequestImg.empty() || id!=plib->m_RequestImg.front() );

			if( image ) image->m_HotTick = ::timeGetTime();
			plib->m_IDImageSet[id] = image;
			plib->m_ImLoadImgID = 0;

			SetEvent( plib->m_hIMLoadOK );
		}
		else if( !plib->m_RequestImg.empty() && id==plib->m_RequestImg.front() )
		{
			assert( id != plib->m_ImLoadImgID );
			
			if( image ) image->m_HotTick = ::timeGetTime();
			plib->m_IDImageSet[id] = image;

			plib->m_RequestImg.pop_front();
		}
		else //要求载入的image已经被删除
		{
			delete image;
		}

		ReleaseMutex( plib->m_hMutex );
	}

	return 0;
}
//////////////////////////////////////////////////////////////////////////
// class CGameImageLib

CGameImageLib::CGameImageLib() : m_bQuit(false), m_LastGCTick(0), m_ImLoadImgID(0)
{
	m_LastGCTick = ::timeGetTime();

	DWORD threadid = 0;
	m_hMutex = CreateMutex( NULL, FALSE, NULL );
	m_hHasReqEvent = CreateEvent( NULL, TRUE, FALSE, NULL );
	m_hIMLoadOK = CreateEvent( NULL, FALSE, FALSE, NULL ); //通过以后，自动消掉信号
	m_hThread = CreateThread( NULL, 0, LoadMainFunc, this, 0, &threadid );

	//SetThreadPriority( m_hThread, THREAD_PRIORITY_IDLE );
	//SetThreadPriority( m_hThread, THREAD_PRIORITY_LOWEST );
	SetThreadPriority( m_hThread, THREAD_PRIORITY_BELOW_NORMAL );
}

CGameImageLib::~CGameImageLib()
{
	m_bQuit = true;
	SetEvent( m_hHasReqEvent );
	if( WaitForSingleObject(m_hThread, 2000) == WAIT_TIMEOUT)
	{
		TerminateThread( m_hThread, 1 );
	}
	else CloseHandle( m_hThread );

	CloseHandle( m_hIMLoadOK );
	CloseHandle( m_hHasReqEvent );
	CloseHandle( m_hMutex );

	std::map<DWORD, CIDImage *>::iterator iter = m_IDImageSet.begin();
	for(; iter!=m_IDImageSet.end(); iter++ )
	{
		delete iter->second;
	}
}

ULONG CGameImageLib::Require(DWORD dwID)
{
#ifdef IMAGE_LOAD_SYNC
	return AddRef_Sync(dwID);
#else
	CMutexLock lock(m_hMutex);

	bool bfound;
	CIDImage *pimage = FindLoadedImageRaw( dwID, bfound );
	if( bfound )
	{
		if( pimage == NULL ) return 0;
		else
		{
			pimage->m_HotTick = ::timeGetTime();
			return pimage->AddRef();
		}
	}

	assert( m_ImLoadImgID == 0 );
	std::deque<DWORD>::iterator iter = m_RequestImg.begin();
	for(; iter!=m_RequestImg.end(); iter++ )
	{
		if( (*iter) == dwID ) return 0; //已经加入request队列了
	}
	m_RequestImg.push_back( dwID );

    SetEvent( m_hHasReqEvent );

	return 0;
#endif
}

bool CGameImageLib::IsImageExist( DWORD dwID )
{
	return CIDImage::IsImageExist(dwID);
}

ULONG CGameImageLib::AddRef(DWORD dwID)
{
#ifdef IMAGE_LOAD_SYNC
	return AddRef_Sync(dwID);
#else
	WaitForSingleObject( m_hMutex, INFINITE );

	bool bfound;
	CIDImage *pimage = FindLoadedImageRaw( dwID, bfound );
	if( bfound )
	{
		ReleaseMutex( m_hMutex );
		if( pimage == NULL ) return 0;
		else
		{
			pimage->m_HotTick = ::timeGetTime();
			return pimage->AddRef();
		}
	}
	else //在m_RequestImg里面寻找
	{
		std::deque<DWORD>::iterator iter = m_RequestImg.begin();
		for(; iter!=m_RequestImg.end(); iter++ )
		{
			if( *iter == dwID )
			{
				m_RequestImg.erase( iter );
				break;
			}
		}
		assert( m_ImLoadImgID == 0 );
		m_ImLoadImgID = dwID;
		assert( m_ImLoadImgID != 0 );
	
		SetEvent( m_hHasReqEvent );
	}
	ReleaseMutex( m_hMutex );

	WaitForSingleObject( m_hIMLoadOK, INFINITE );

	pimage = FindLoadedImage( dwID, bfound );
	if( pimage )
	{
		//assert( pimage->m_dwRef == 0 );
		pimage->m_HotTick = ::timeGetTime();
		return pimage->AddRef();
	}
	else return 0;
#endif
}


ULONG CGameImageLib::GetRef(DWORD dwID)
{
    ULONG ref = 0;
    std::map<DWORD, CIDImage *>::iterator iter = m_IDImageSet.find( dwID );
	if( iter != m_IDImageSet.end() )
	{
		CIDImage *pimage = iter->second;
        if ( pimage == 0 )
            ref = 0;
        else 
            ref = pimage->GetRef();
	}

    return ref;
}
ULONG  CGameImageLib::AddRef_Sync( DWORD dwID )
{
	std::map<DWORD, CIDImage *>::iterator iter = m_IDImageSet.find( dwID );
	if( iter == m_IDImageSet.end() )
	{
		CIDImage *image = new CIDImage;
		image->m_dwImageID = dwID;
		ULONG ref = image->AddRef();

		image->m_HotTick = ::timeGetTime();
		m_IDImageSet[dwID] = image;
		return ref;
	}
	else
	{
		CIDImage *pimage = iter->second;
		assert( pimage );
		pimage->m_HotTick = ::timeGetTime();
		return pimage->AddRef();
	}
}

CIDImage *CGameImageLib::FindLoadedImage( DWORD dwID, bool &bfound )
{
	CMutexLock lock(m_hMutex);
	return FindLoadedImageRaw( dwID, bfound );
}

CIDImage *CGameImageLib::FindLoadedImageRaw( DWORD dwID, bool &bfound )
{
	std::map<DWORD, CIDImage *>::iterator iter = m_IDImageSet.find( dwID );
	if( iter == m_IDImageSet.end() )
	{
		bfound = false;
		return NULL;
	}
	else
	{
		bfound = true;
		return iter->second;
	}
}

ULONG CGameImageLib::Release(DWORD dwID)
{
	CMutexLock lock(m_hMutex);

	std::map<DWORD, CIDImage *>::iterator iter = m_IDImageSet.find( dwID );
	if( iter == m_IDImageSet.end() )
	{
		//如果是IMLoad或者RequestImg, 删除
		if( m_ImLoadImgID == dwID ) m_ImLoadImgID = 0;
		std::deque<DWORD>::iterator it = m_RequestImg.begin();
		for(; it!=m_RequestImg.end(); it++ )
		{
			if( (*it) == dwID )
			{
				m_RequestImg.erase(it);
				break;
			}
		}
		return 0;
	}
	else
	{
		CIDImage *pimage = iter->second;
		assert( pimage );
		ULONG ref = 0;
		if( pimage )
		{
			ref = pimage->Release();
			//assert( ref > 0 );
		}
		/*if( ref == 0 )
		{
			delete pimage;
			m_IDImageSet.erase( iter );
		}*/
		return ref;
	}
}

void CGameImageLib::GarbageCollect()
{
	unsigned int curtick = ::timeGetTime();
	if( curtick < m_LastGCTick+GC_INTERVAL ) return;  //1秒钟收集一次
	m_LastGCTick = curtick;

	CMutexLock lock(m_hMutex);

	std::map<DWORD, CIDImage *>::iterator iter = m_IDImageSet.begin();
	while( iter != m_IDImageSet.end() )
	{
		CIDImage *pimage = iter->second;

#ifdef _DEBUG
		if( pimage && pimage->m_dwRef>1 )
		{
			int aaa = 0;
		}
#endif

		if( pimage!=NULL && pimage->m_dwRef<=1 && curtick>pimage->m_HotTick+IMAGE_COLLOECT_TICK )
		{
			assert( pimage->m_dwRef == 1 );
			delete pimage;
			iter = m_IDImageSet.erase(iter);
		}
		else iter++;
	}
}

int CGameImageLib::GetTotalDirs(DWORD dwID)
{
	bool bfound;
	CIDImage *pimage = FindLoadedImage(dwID, bfound);
	assert( pimage );
	if( pimage ) return pimage->GetTotalDirs();
	else return 0;
}

int	CGameImageLib::GetTotalFrames(DWORD dwID)
{
	bool bfound;
	CIDImage *pimage = FindLoadedImage(dwID, bfound);
	assert( pimage );
	if( pimage ) return pimage->GetTotalFrames();
	else return 0;
}


int	CGameImageLib::GetDirFrames(DWORD dwID, int nDir)
{
	bool bfound;
	CIDImage *pimage = FindLoadedImage(dwID, bfound);
	assert( pimage );
	if( pimage ) return pimage->GetDirFrames( nDir );
	else return 0;
}


TBoundInfo * CGameImageLib::GetBoundInfo(DWORD dwID)
{
	bool bfound;
	CIDImage *pimage = FindLoadedImage(dwID, bfound);
    if ( 0 == pimage )
        return NULL;
	assert( pimage );
	if( pimage ) return pimage->GetBoundInfo();
	else return NULL;
}


TFrameInfo * CGameImageLib::GetFrameInfo(DWORD dwID, int nDir, int nFrame)
{
	bool bfound;
	CIDImage *pimage = FindLoadedImage(dwID, bfound);
	assert( pimage );
	if( pimage ) return pimage->GetFrameInfo( nDir, nFrame );
	else return NULL;
}


//Jim, add [2/10/2004]
//{{{
TDirInfo * CGameImageLib::GetDirInfo(DWORD dwID, int nDir)
{
	bool bfound;
	CIDImage *pimage = FindLoadedImage(dwID, bfound);
	assert( pimage );
	if( pimage ) return pimage->GetDirInfo( nDir );
	else return 0;
}
//}}}

