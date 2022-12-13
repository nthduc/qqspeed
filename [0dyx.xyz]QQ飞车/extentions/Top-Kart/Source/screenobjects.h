////////////////////////////////////////////////
//     FileName:    screenobjects.h
//     Author:      Fish
//     Date:        2007-3-26 15:26:03
//     Description: 提供把3D对象绘制在UI上的功能
////////////////////////////////////////////////

#ifndef __SCREENOBJECTS_H__
#define __SCREENOBJECTS_H__

#include "NiAVObject.h"
#include "NiCamera.h"
#include "NiRenderTargetGroup.h"
#include "singlet.h"
#include "idgen.h"
#include "map"
using namespace std;

class CScreenObject
{
public:
	enum AlignMode
	{
		AlignMode_Center,
		AlignMode_TopLeft,
	};

	CScreenObject(NiAVObject*, NiCamera*, bool bVisible = true);
	~CScreenObject();

	bool GetVisible();
	bool SetVisible(bool);

	void CacheRender();
	void Draw(float x, float y, float w, float h, float fScale, AlignMode Mode = AlignMode_Center);

public:
	DWORD			m_dwHandle;

private:
	NiAVObjectPtr	m_spRootObject;
	NiCameraPtr		m_spCamera;
	bool			m_bVisible;
	NiRenderedTexturePtr	m_spRenderedTexture;
	NiRenderTargetGroupPtr	m_spRenderTargetGroup;
};

class CScreenObjectsMgr : public CSingleT<CScreenObjectsMgr>
{
public:
	CScreenObjectsMgr();
	~CScreenObjectsMgr();

	void	CacheRender();

	DWORD	AddScreenObject(NiAVObject*, NiCamera*, bool bVisible = true);
	bool	RemoveScreenObject(DWORD);
	bool	DrawScreenObject(DWORD, float x, float y, float w, float h, float fScale, CScreenObject::AlignMode Mode = CScreenObject::AlignMode_Center);
	bool	SetVisible(DWORD, bool bVisible);

private:
	map<DWORD, CScreenObject*>	m_kObjects;
	CIDGen	m_kIDGen;
};

#endif