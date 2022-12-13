////////////////////////////////////////////////
//     FileName:    CDTimerShow_Atom.h
//     Author:      rayhuang
//     Date:        2007-5-28
//     Description: ��ʾ����ʱʱ��
////////////////////////////////////////////////

#pragma once 
#include "AtomBase.h"
#include "../RenderBase.h"
#include "ScreenTexMgr.h"

class CCDTimerShow_Atom : public CAtomBase, public CRenderBase2D
{
public:
	CCDTimerShow_Atom();
	~CCDTimerShow_Atom();
	void Start();
	void Update(float fTime);
	void Draw(NiRendererPtr spRenderer);
	void SetParam(DWORD dwParam);
protected:
	void _Stop();
private:
	CScreenTex* m_pTimerBase;
	NiString2DPtr m_spTime;
	DWORD m_dwEndTime;

};