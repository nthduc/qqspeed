#pragma once

#include <NiMemObject.h>
#include <NiTArray.h>
#include <NiCamera.h>
#include <NiRenderer.h>

#include "PerspectiveMirror.h"

class MirrorManager
	: public NiMemObject
{
typedef NiTPrimitiveArray<PerspectiveMirror*> Mirrors;
public:
	MirrorManager();
	virtual ~MirrorManager();

public:
	// �������ӣ�pkScene��ʾ������Ķ���pkMirrorObject��ʾ����
	bool CreateMirrors(NiRenderer * pkRenderer, NiNode * pkScene, NiNode * pkMirrorObject, CVarTable * pVarTable);
	void UpdateMirrors(NiCamera* pkCamera);
	void DestroyAllMirrors();
	void SetVisible(bool bVisible);

protected:
	Mirrors m_arrMirrors;
	bool	m_bVisible;
};