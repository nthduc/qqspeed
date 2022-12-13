/*
 *	GameImageLib.h
 *	game image library & manager functions header.
 *
 *	Jim, 2003.12.12.
 *	
 *
 *	Ӱ��ID��� 
 *  Ӱ��IDΪ32λ�޷�������, ��������(bit):
 *	+--------------+------------+-------------------------+
 *	| 31.....24    | 23......16 | 15 ...................0 |
 *	+--------------+------------+-------------------------+
 *	| ����         | �������   | �������                | 
 *	+--------------+------------+-------------------------+
 *	
 *	˵��: 1. Ŀǰ����λʼ��Ϊ0.
 *	      2. ������ʹ�1~255, 0����.
 *	      3. ���������1~65535, 0����.
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
	HANDLE m_hMutex;       //�����m_IDImageSet, m_RequestImg, m_ImLoadImgID�ķ���
	HANDLE m_hHasReqEvent; //�Ƿ���ͼƬ��Ҫload
	HANDLE m_hIMLoadOK;    //����Ҫ��ͼƬ(ͨ��AddRef)�Ƿ�load���
	HANDLE m_hThread;

	bool   m_bQuit;
	unsigned int m_LastGCTick;

	std::deque<DWORD>m_RequestImg; //��ҪͼƬ���������
	DWORD m_ImLoadImgID;  //������Ҫload��ͼƬ

	std::map<DWORD, CIDImage *> m_IDImageSet;

	//�����ҵ��ˣ����ǲ���load, ����NULL, bfound=true
	CIDImage *FindLoadedImage( DWORD dwID, bool &bfound ); //with lock
	CIDImage *FindLoadedImageRaw( DWORD dwID, bool &bfound ); //without lock

	ULONG  AddRef_Sync( DWORD dwID );

};


#endif//_GAMEIMAGELIB_H_
