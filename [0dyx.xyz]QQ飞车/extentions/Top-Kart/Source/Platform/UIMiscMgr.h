////////////////////////////////////////////////
//     FileName:    UIMiscMgr.h
//     Author:      Fish
//     Date:        2006-10-11 15:39:19
//     Description: []
////////////////////////////////////////////////
#pragma once

#include "vector"
#include "map"
#include "display.h"
#include "imagelib.h"
#include "gameui.h"
#include "fileop.h"
#include "txtctrlr.h"
#include "../UI/ImageEx.h"
using namespace std;


#define MAX_SHOTCUTKEY	20
typedef map<string, CImageEx*>	StrKeyToImageMap;
typedef map<string, int>		StrKeyToActionMap;

class CUIMiscCallback
{
public:
	virtual void PlayerDoAction(UINT Uin, int ActionSeq) = 0;
	virtual LPCSTR GetPlayerName() = 0;
	virtual void SetCtrlTextColor(INT index, DWORD c) = 0;
};

class CUIMiscMgr
{
public:
	CUIMiscMgr(CUIMiscCallback* pCB = NULL);
	~CUIMiscMgr();

	void Init(IDisplay*);
	void UnInit();

	void ParseKeyChar(stKeyChar*);
	void DrawChatFace(int Index, int frame, int left, int top);
	BOOL DoChatAction(UINT uin, LPCSTR str);
	BOOL ParseField(LPCSTR name, LPSTR outstr, INT* outlen);

private:
	void LoadChatFaces();
	void LoadCtrlColor();
	void LoadChatActions();

public:
	IDisplay*			m_pDisplay;
	BOOL				m_bLoaded;
	StrKeyToImageMap	m_ChatFaceMap;
	StrKeyToActionMap	m_ChatActionMap;	//”¢Œƒ
	StrKeyToActionMap	m_ChatActionMapLocal;//±æµÿ”Ô—‘

	CUIMiscCallback*	m_pCallback;
};

