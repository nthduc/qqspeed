/*
	Nif�ļ����ز��ԣ�
	Ҫ�㣺
	1��ÿ���ļ�����Ӧһ��Cookieֵ
	2������Background load�ķ�ʽ������¼��ص�֪ͨ������ɽ��
	3����Դ������������õ���̭���ԣ��趨��Դ�������ޣ��ﵽ����ֵ���������ص�ʱ���������̭����ֱ����������Ϊֹ��
	4����Դ�������Զ�PreCache
	5������ͬһ����Դ����ȡ��ε������ά��һ���б����ȴ��б����ҿ��е���Դ���м���Ϊ1����Դ���߿ɹ���ģ������������һ���µļ�������
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