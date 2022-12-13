
#include "Top-KartPCH.h"

#include "StopElement.h"


CScreenAnim::CScreenAnim()
: m_spAnimNode(NULL)
, m_bvisible(true)
{

}

CScreenAnim::~CScreenAnim()
{
	m_spAnimNode = NULL;
}

bool CScreenAnim::Create(const char * path)
{
	NiStream kStream;
	if (!kStream.Load(NiApplication::ConvertMediaFilename(path))) 
	{
		return false;
	}
	m_spAnimNode = NiDynamicCast(NiNode, kStream.GetObjectAt(0));

	return (m_spAnimNode != NULL);
}

void CScreenAnim::Update(float fDelta)
{
	m_spAnimNode->Update(fDelta);
}



unsigned int CScreenAnimManager::AddAnim(const char * path, DWORD dwType)
{
	CScreenAnimPtr spScreenAnim = NiNew CScreenAnim;
	if (!spScreenAnim || !spScreenAnim->Create(path))
		return 0;
	return m_arrScreenAnims.Add(spScreenAnim);
}


void CScreenAnimManager::Update(float fDelta)
{
	unsigned int uiNum = m_arrScreenAnims.GetEffectiveSize();

	for (unsigned int i = 0; i < uiNum; ++i)
	{
		m_arrScreenAnims.GetAt(i)->Update(fDelta);
	}
}

void CScreenAnimManager::Render(NiRenderer * pkRender)
{
	unsigned int uiNum = m_arrScreenAnims.GetEffectiveSize();

	for (unsigned int i = 0; i < uiNum; ++i)
	{
		//m_arrScreenAnims.GetAt(i)->Update(fDelta);
	}
}