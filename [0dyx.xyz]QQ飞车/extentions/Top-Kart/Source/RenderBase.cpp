#include "Top-KartPCH.h"
#include "RenderBase.h"

//CRenderMgr* CRenderMgr::ms_pRenderMgr = 0;
//--------------------------------------------------------------------------------------
//2D元素
//--------------------------------------------------------------------------------------
CRenderBase2D::CRenderBase2D()
{
	m_bActive = false;
	if(CSingleT<CRenderMgr>::Get())
	{
		CSingleT<CRenderMgr>::Get()->ActivateObject2D(this);
	}	
}

CRenderBase2D::~CRenderBase2D()
{
	if(CSingleT<CRenderMgr>::Get())
	{
		CSingleT<CRenderMgr>::Get()->TerminateObject2D(this);
	}	
}

void CRenderBase2D::SetActivity(bool bActive)
{
	m_bActive = bActive;
}

bool CRenderBase2D::IsActive()
{
	return m_bActive;
}

//--------------------------------------------------------------------------------------
//3D元素
//--------------------------------------------------------------------------------------
CRenderBase3D::CRenderBase3D()
{
	m_bActive = false;
	if(CSingleT<CRenderMgr>::Get())
	{
		CSingleT<CRenderMgr>::Get()->ActivateObject3D(this);
	}	
}

CRenderBase3D::~CRenderBase3D()
{
	if(CSingleT<CRenderMgr>::Get())
	{
		CSingleT<CRenderMgr>::Get()->TerminateObject3D(this);
	}	
}

void CRenderBase3D::SetActivity(bool bActive)
{
	m_bActive = bActive;
}

bool CRenderBase3D::IsActive()
{
	return m_bActive;
}
//--------------------------------------------------------------------------------------




//--------------------------------------------------------------------------------------
//CRenderMgr渲染控制类
//所有的渲染对象加入此处，统一更新，渲染
//--------------------------------------------------------------------------------------

CRenderMgr::CRenderMgr()
{

}

CRenderMgr::~CRenderMgr()
{
	m_listRenderObject2D.clear();
	m_listRenderObject3D.clear();
}
/*
bool CRenderMgr::Create()
{
	ms_pRenderMgr = new CRenderMgr;	

	if(ms_pRenderMgr)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void CRenderMgr::Destroy()
{
	SAFE_DELETE(ms_pRenderMgr);
}

CRenderMgr* CRenderMgr::Get()
{
	return ms_pRenderMgr;
}
*/
void CRenderMgr::ActivateObject2D(IRenderBase* pObject)
{
	m_renderListIt2D = m_listRenderObject2D.begin();
	while(m_renderListIt2D != m_listRenderObject2D.end())
	{
		if(*m_renderListIt2D == pObject)
		{
			return;
		}
		m_renderListIt2D++;
	}
	m_listRenderObject2D.push_back(pObject);
}

void CRenderMgr::ActivateObject3D(IRenderBase* pObject)
{
	m_renderListIt3D = m_listRenderObject3D.begin();
	while(m_renderListIt3D != m_listRenderObject3D.end())
	{
		if(*m_renderListIt3D == pObject)
		{
			return;
		}
		m_renderListIt3D++;
	}
	m_listRenderObject3D.push_back(pObject);
}

void CRenderMgr::TerminateObject2D(IRenderBase* pObject)
{
	m_renderListIt2D = m_listRenderObject2D.begin();
	while(m_renderListIt2D != m_listRenderObject2D.end())
	{
		if(*m_renderListIt2D == pObject)
		{
			m_listRenderObject2D.erase(m_renderListIt2D);
			return;
		}
		m_renderListIt2D++;
	}
}

void CRenderMgr::TerminateObject3D(IRenderBase* pObject)
{
	m_renderListIt3D = m_listRenderObject3D.begin();
	while(m_renderListIt3D != m_listRenderObject3D.end())
	{
		if(*m_renderListIt3D == pObject)
		{
			m_listRenderObject3D.erase(m_renderListIt3D);
			return;
		}
		m_renderListIt3D++;
	}
}


void CRenderMgr::Update(float fTime)
{
	m_renderListIt2D = m_listRenderObject2D.begin();
	for(; m_renderListIt2D != m_listRenderObject2D.end(); m_renderListIt2D++)
	{
		if((*m_renderListIt2D)->IsActive())
		{
			(*m_renderListIt2D)->Update(fTime);
		}
	}

	m_renderListIt3D = m_listRenderObject3D.begin();
	for(; m_renderListIt3D != m_listRenderObject3D.end(); m_renderListIt3D++)
	{
		if((*m_renderListIt3D)->IsActive())
		{
			(*m_renderListIt3D)->Update(fTime);
		}
		
	}
}


void CRenderMgr::Render()
{
	NiRenderer* pkRender = NiRenderer::GetRenderer();
	m_renderListIt3D = m_listRenderObject3D.begin();
	for(; m_renderListIt3D != m_listRenderObject3D.end(); m_renderListIt3D++)
	{
		if((*m_renderListIt3D)->IsActive())
		{
			(*m_renderListIt3D)->Draw(pkRender);
		}
	}
}

void CRenderMgr::ScreenRender()
{
	m_renderListIt2D = m_listRenderObject2D.begin();
	NiRenderer* pkRender = NiRenderer::GetRenderer();
	for(; m_renderListIt2D != m_listRenderObject2D.end(); m_renderListIt2D++)
	{
		if((*m_renderListIt2D)->IsActive())
		{
			(*m_renderListIt2D)->Draw(pkRender);
		}

	}
}