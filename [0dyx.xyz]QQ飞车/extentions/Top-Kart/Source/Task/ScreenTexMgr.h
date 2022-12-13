////////////////////////////////////////////////
//     FileName:    ScreenTexMgr.h
//     Author:      rayhuang
//     Date:        2007-5-18
//     Description: 屏幕纹理管理类
////////////////////////////////////////////////


#pragma once 
#include "TaskDefine.h"
#include "singlet.h"
#include "../RenderBase.h"
#include <list>
#include <string>
using namespace std;

typedef struct tagScreenTexConfig
{
	DWORD dwID;
	float fX;
	float fY;
	float fWidth;
	float fHeight;
	string strTexPath;
}SCREENTEXCFG, *PSCREENTEXCFG;

typedef list<SCREENTEXCFG> SCRTEXCFG_LIST;
typedef list<SCREENTEXCFG>::const_iterator SCRTEXCFG_LIST_IT;


class CScreenTex : public CRenderBase2D
{
public:
	CScreenTex();
	~CScreenTex();
	void SetTex(NiSourceTexturePtr spTex);
	void SetRectangle(float fLeft, float fTop, float fWidth, float fHeight);
	virtual void Draw(NiRendererPtr spRenderer);
	virtual void Update(float fTime);

private:
	NiScreenElementsPtr m_spScreenElet;
};


class CTScreenTexMgr : public CSingleT<CTScreenTexMgr>
{
public:
	CTScreenTexMgr();
	~CTScreenTexMgr();
	bool Init();
	CScreenTex* CreateScreenTex(DWORD dwID);

private:
	CScriptState m_lsTex;
	SCRTEXCFG_LIST m_listTexCfg;

};