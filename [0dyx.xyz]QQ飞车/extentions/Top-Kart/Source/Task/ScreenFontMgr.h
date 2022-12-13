////////////////////////////////////////////////
//     FileName:    ScreenFontMgr.h
//     Author:      rayhuang
//     Date:        2007-6-13
//     Description: 屏幕文字管理类
////////////////////////////////////////////////

#pragma once 
#include "TaskDefine.h"
#include "singlet.h"
#include "../RenderBase.h"
#include <list>
#include <string>
using namespace std;

typedef struct tagScreenFontConfig
{
	DWORD dwID;
	float fX;
	float fY;
	float fFontSize;
	DWORD dwColor;
	string strDesc;
}SCREENFONTCFG, *PSCREENFONTCFG;

typedef list<SCREENFONTCFG> SCRFONTCFG_LIST;
typedef list<SCREENFONTCFG>::const_iterator SCRFONTCFG_LIST_IT;

class CScreenFont : public CRenderBase2D
{
public:
	CScreenFont();
	~CScreenFont();
	void Update(float fTime);
	void Draw(NiRendererPtr spRenderer);
	void SetConfig(const SCREENFONTCFG& kFontCfg);
private:
	float m_fX;
	float m_fY;
	float m_fFontSize;
	DWORD m_dwColor;
	string m_strDesc;
};

class CScreenFontMgr : public CSingleT<CScreenFontMgr>
{
public:
	CScreenFontMgr();
	~CScreenFontMgr();
	bool Init();
	CScreenFont* CreateScreenFont(DWORD dwID);

private:
	CScriptState m_lsFont;
	SCRFONTCFG_LIST m_listFontCfg;
};