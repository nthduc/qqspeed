#include "Top-KartPCH.h"

#include "MirrorManager.h"
#include "variant.h"
#include "script.h"
#include "common.h"

MirrorManager::MirrorManager()
: m_bVisible(true)
{

}

MirrorManager::~MirrorManager()
{

}

void MirrorManager::SetVisible(bool bVisible)
{
	m_bVisible = bVisible;
}



//-----------------------------------------------------------------------


bool MirrorManager::CreateMirrors(NiRenderer * pkRenderer, NiNode * pkScene, NiNode * pkMirrorObject, CVarTable * pMapdesc)
{

	if (!pkScene || !pkMirrorObject || !pMapdesc) 
	{
		assert(0);
		LogText("MirrorManager::CreateMirrors, Wrong parameter!");
		return false;
	}
	/*
	CVarTable mapdesc("map_desc");
	CVar::LoadVarsFromFile(&mapdesc, pcMirrofConfig);*/

	bool bHasMirror = pMapdesc->Element("map_has_mirror") 
		? pMapdesc->Element("map_has_mirror")->Integer() == 1 : false;

	if (!bHasMirror) 
	{
		//LogText("CSceneLevel::CreateMirror, We don't need any mirror characteristics! So return");
		return true;
	}

	CVarArray * pVarMirrors = (CVarArray*)pMapdesc->Element("mirrors");
	
	if (!pVarMirrors || pVarMirrors->GetType() != VAR_ARRAY) 
	{
		LogText("MirrorManager::CreateMirror, pMapdesc doesn't contain a 'mirror' option");
		assert(0);
		return false;
	}

	int nMirrorNum = pVarMirrors->GetNum();
	
	for (int i = 0; i < nMirrorNum; ++i)
	{
		CVarTable * pVarMirror = (CVarTable *)pVarMirrors->Element(i);

		if (!pVarMirror || pVarMirror->GetType() != VAR_TABLE) 
		{
			LogText("MirrorManager::CreateMirror, file pcMirrofConfig doesn't contain a 'mirror' option");
			assert(0);
			return false;
		}

		NiFixedString kMirrorNodeName		= pVarMirror->Element("node_name") ? pVarMirror->Element("node_name")->String() : "";
		NiFixedString kMirrorTriShapeName	= pVarMirror->Element("trishape_name") ? pVarMirror->Element("trishape_name")->String() : "";

		if (!kMirrorTriShapeName.Exists() || !kMirrorNodeName.Exists()) 
		{
			LogText("MirrorManager::CreateMirror.Node not found!!");
			assert(0);
			DestroyAllMirrors();
			return false;
		}

		PerspectiveMirror * pkMirror = NiNew PerspectiveMirror(pkRenderer, pkScene, pkMirrorObject, kMirrorNodeName, kMirrorTriShapeName);
		if (!pkMirror)
		{
			LogText("MirrorManager::CreateMirror.CreateMirrorFailed!!");
			DestroyAllMirrors();
			return false;
		}

		NiPoint3 kCenterPoint, kRightDir, kUpDir;
		pVarMirror->Element("centpoint")->FloatArray((float*)&kCenterPoint, 3);
		pVarMirror->Element("right_dir")->FloatArray((float*)&kRightDir, 3);
		pVarMirror->Element("up_dir")->FloatArray((float*)&kUpDir, 3);

		pkMirror->SetPlane(kCenterPoint, kRightDir, kUpDir);

		NiFixedString kName = pVarMirror->Element("mirror_name")->String();
		pkMirror->SetName(kName);

		m_arrMirrors.Add(pkMirror);
	}

	return true;
}


void MirrorManager::UpdateMirrors(NiCamera* pkCamera)
{
	if (!m_bVisible) 
	{
		return;
	}

	unsigned int uiMirrorNum = m_arrMirrors.GetEffectiveSize();
	PerspectiveMirror * pkMirror = NULL;

	for (unsigned int i = 0; i < uiMirrorNum; ++i)
	{
		pkMirror = m_arrMirrors.GetAt(i);
		pkMirror->Update(pkCamera);
	}
}


void MirrorManager::DestroyAllMirrors()
{
	unsigned int uiMirrorNum = m_arrMirrors.GetEffectiveSize();
	PerspectiveMirror * pkMirror = NULL;

	for (unsigned int i = 0; i < uiMirrorNum; ++i)
	{
		pkMirror = m_arrMirrors.GetAt(i);
		SAFE_NIDELETE(pkMirror);
	}

	m_arrMirrors.RemoveAll();
}