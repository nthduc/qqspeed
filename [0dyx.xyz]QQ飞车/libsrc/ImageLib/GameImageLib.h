/*
 *	GameImageLib.h
 *	game image library & manager functions header.
 *
 *	Jim, 2003.12.12.
 *	
 *
 *	影像ID组成 
 *  影像ID为32位无符号整数, 构成如下(bit):
 *	+--------------+------------+-------------------------+
 *	| 31.....24    | 23......16 | 15 ...................0 |
 *	+--------------+------------+-------------------------+
 *	| 保留         | 物件类型   | 物件索引                | 
 *	+--------------+------------+-------------------------+
 *	
 *	说明: 1. 目前保留位始终为0.
 *	      2. 物件类型从1~255, 0保留.
 *	      3. 物件索引从1~65535, 0保留.
 *	
 *	
*/
#ifndef _GAMEIMAGELIB_H_
#define _GAMEIMAGELIB_H_

								//in the debug information
#include "ImageLib.h"
#include "IDImage.h"

#pragma warning(disable:4786)	//identifier was truncated to '255' characters 
#include <map>
#include <deque>
using namespace std;


//////////////////////////////////////////////////////////////////////////
// class CGameImageLib

class CGameImageLib : public IImageLib
{
public:
	CGameImageLib();
	~CGameImageLib();

public:
	ULONG   Require(DWORD dwID);
	ULONG	AddRef(DWORD dwID);
    ULONG   GetRef(DWORD dwID);
	ULONG	Release(DWORD dwID);
	int		GetTotalDirs(DWORD dwID);
	int		GetTotalFrames(DWORD dwID);
	int		GetDirFrames(DWORD dwID, int nDir);
	TBoundInfo * GetBoundInfo(DWORD dwID);
	TFrameInfo * GetFrameInfo(DWORD dwID, int nDir, int nFrame);

    //Jim, add [2/10/2004]
    //{{{
    TDirInfo * GetDirInfo(DWORD dwID, int nDir);
    //}}}

	//Delta, add [8/12/2004]
	//{{{
	virtual bool IsImageExist( DWORD dwID );
	//}}

	virtual void GarbageCollect();

public:
	HANDLE m_hMutex;       //互斥对m_IDImageSet, m_RequestImg, m_ImLoadImgID的访问
	HANDLE m_hHasReqEvent; //是否有图片需要load
	HANDLE m_hIMLoadOK;    //立即要的图片(通过AddRef)是否load完成
	HANDLE m_hThread;

	bool   m_bQuit;
	unsigned int m_LastGCTick;

	std::deque<DWORD>m_RequestImg; //需要图片的请求队列
	DWORD m_ImLoadImgID;  //立即需要load的图片

	std::map<DWORD, CIDImage *> m_IDImageSet;

	//可能找到了，但是不能load, 返回NULL, bfound=true
	CIDImage *FindLoadedImage( DWORD dwID, bool &bfound ); //with lock
	CIDImage *FindLoadedImageRaw( DWORD dwID, bool &bfound ); //without lock

	ULONG  AddRef_Sync( DWORD dwID );

};


#endif//_GAMEIMAGELIB_H_
