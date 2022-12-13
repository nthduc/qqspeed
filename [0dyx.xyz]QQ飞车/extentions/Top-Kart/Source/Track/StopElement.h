#pragma once
#include "NiObject.h"
#include "NiMemObject.h"
#include "NiRenderer.h"

class CScreenAnimManager;
class CScreenAnim
	: public NiObject
{
protected:
	CScreenAnim();
	virtual ~CScreenAnim();

	bool Create(const char * path);

public:
	friend class CScreenAnimManager;
	virtual void Update(float fDelta);

	void SetVisible(bool bVisible);

protected:
	bool      m_bvisible;
	NiNodePtr m_spAnimNode;
};

NiSmartPointer(CScreenAnim);

class CScreenAnimManager
	: public NiMemObject
{
public:
	unsigned int AddAnim(const char * path, DWORD dwType);

	void Update(float fDelta);
	void Render(NiRenderer * pkRender);

protected:
	NiTObjectArray<CScreenAnimPtr> m_arrScreenAnims;
};