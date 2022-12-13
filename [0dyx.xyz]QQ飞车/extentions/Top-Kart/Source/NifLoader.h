/*
	Nif文件加载策略：
	要点：
	1、每个文件名对应一个Cookie值
	2、采用Background load的方式，结合事件回调通知加载完成结果
	3、资源缓存和最久最不常用的淘汰策略（设定资源缓存上限，达到上限值而继续加载的时候就启动淘汰策略直到条件满足为止）
	4、资源加载完自动PreCache
	5、对于同一份资源被存取多次的情况，维护一个列表，优先从列表中找空闲的资源（有计数为1的资源或者可共享的），否则就启动一个新的加载任务
*/
#ifndef __NIFLOADER_H__
#define __NIFLOADER_H__

#include "strhash.h"
#include "autocs.h"
#include "singlet.h"
#include "callbackstream.h"

#define DEFAULT_NIFLOADER_POOLSIZE	20 * 1024 * 1024	//100M

interface INifLoaderEvent;
typedef struct tagNIF_LOAD_DATA
{
	CallbackStream*	pStream;
	NiNode*			pkNode;
	DWORD			dwExtraData;
	INifLoaderEvent* pEvent;
	bool	bShared;
	DWORD	dwRef;
	DWORD	dwFileSize;
	DWORD	dwLastRefTime;
	tagNIF_LOAD_DATA* pNext;

	tagNIF_LOAD_DATA()
	{
		ZeroMemory(this, sizeof(tagNIF_LOAD_DATA));
	}

	tagNIF_LOAD_DATA(
					CallbackStream*	Stream,
					NiNode*			Node,
					DWORD			ExtraData,
					INifLoaderEvent* Event,
					bool	Shared,
					DWORD	Ref,
					DWORD	FileSize,
					DWORD	LastRefTime
					)
	{
		pStream = Stream;
		pkNode = Node;
		dwExtraData = ExtraData;
		pEvent = Event;
		bShared = Shared;
		dwRef = Ref;
		dwFileSize = FileSize;
		dwLastRefTime = LastRefTime;
		pNext = NULL;
	}

	~tagNIF_LOAD_DATA()
	{
		if ( pStream )
		{
			pStream->BackgroundLoadFinish();
			NiDelete pStream;
			pStream = NULL;
		}
		if ( pkNode )
		{
			NiDelete pkNode;
			pkNode = NULL;
		}
		pEvent = NULL;
		if ( pNext )
		{
			delete pNext;
			pNext = NULL;
		}
	}
}NIF_LOAD_DATA;

typedef map<DWORD, NIF_LOAD_DATA*>	NIF_LOAD_DATA_MAP;

typedef struct tagLOAD_CACHE_DATA
{
	DWORD	dwCookie;
	DWORD	dwThreadState;
	DWORD	dwExtraData;
}LOAD_CACHE_DATA;

typedef list<LOAD_CACHE_DATA>	LOAD_CACHE_DATA_LIST;

interface INifLoaderEvent
{
	virtual bool OnLoadExit(DWORD dwCookie, DWORD dwThreadState, DWORD dwExtraData) = 0;
};

class CNifLoader : public INifLoaderEvent, public CSingleT<CNifLoader>
{
public:
	CNifLoader();
	~CNifLoader();

	NiNode*	Load(
		LPCSTR pstrNifFileName, 
		INifLoaderEvent* pEvent = NULL, 
		DWORD*	pdwCookie = NULL, 
		DWORD	dwExtraData = 0,
		bool	bShared = false,
		bool*	pbWouldBlock = NULL);

	NiNode*	Load(
		DWORD	dwCookie, 
		INifLoaderEvent* pEvent = NULL, 
		DWORD	dwExtraData = 0,
		bool	bShared = false,
		bool*	pbWouldBlock = NULL);

	NiNode* LoadTexture(
		LPCSTR pstrTexFileName,
		DWORD* pdwCookie = NULL,
		bool* pbWouldBlock = NULL);

	NiNode* LoadTexture(
		DWORD dwCookie,
		bool* pbWouldBlock = NULL);

	bool	CancelLoad(INifLoaderEvent*);

	void	SetPoolSize(DWORD dwPoolSize = DEFAULT_NIFLOADER_POOLSIZE);

	CStrHash* GetHashTool() { return &m_kStrHash; }

	void Update(float fTime);

	//INifLoaderEvent
	virtual bool OnLoadExit(DWORD dwCookie, DWORD dwThreadState, DWORD dwExtraData);

protected:
	void	Clear();
	void	SweepGarbage();
	DWORD	GetFreePoolSize();
	DWORD	PickIll();

private:
	NIF_LOAD_DATA_MAP	m_kLoadMap;
	LOAD_CACHE_DATA_LIST	m_kLoadCache;
	CStrHash			m_kStrHash;
	DWORD				m_dwPoolSize;

public:
	DECL_AUTOCS
};

#endif