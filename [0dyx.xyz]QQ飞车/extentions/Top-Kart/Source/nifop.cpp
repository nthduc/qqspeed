#include "Top-KartPCH.h"
#include "nifop.h"

NiNode* CNifOp::FindParent(NiAVObject* pkObj, const char* ParentName)
{
	assert(pkObj);
	assert(ParentName);
	NiAVObject* pkNode = pkObj;
	while(pkNode)
	{
		if ( pkNode->GetName() && stricmp(ParentName, pkNode->GetName()) == 0 )
		{
			return NiDynamicCast(NiNode, pkNode);
		}
		pkNode = pkNode->GetParent();
	}
	return NULL;
}


NiCamera* CNifOp::FindCamera(NiAVObject* pkRoot)
{
	if ( !pkRoot )
	{
		return NULL;
	}

	if ( NiIsKindOf(NiCamera, pkRoot) )
	{
		return (NiCamera*)pkRoot;
	}
	if ( NiIsKindOf(NiNode, pkRoot) )
	{
		NiNode* pkNode = (NiNode*)pkRoot;
		for ( unsigned int i = 0; i < pkNode->GetArrayCount(); ++i )
		{
			NiAVObject* pkObj = pkNode->GetAt(i);
			if ( pkObj )
			{
				NiCamera* pkCamera = FindCamera(pkObj);
				if ( pkCamera )
				{
					return pkCamera;
				}
			}
		}
	}
	return NULL;
}

NiGeometry* CNifOp::FindGeometry(NiAVObject* pkRoot)
{
	if ( !pkRoot )
	{
		return NULL;
	}

	if ( NiIsKindOf(NiGeometry, pkRoot) )
	{
		return (NiGeometry*)pkRoot;
	}
	if ( NiIsKindOf(NiNode, pkRoot) )
	{
		NiNode* pkNode = (NiNode*)pkRoot;
		for ( unsigned int i = 0; i < pkNode->GetArrayCount(); ++i )
		{
			NiAVObject* pkObj = pkNode->GetAt(i);
			if ( pkObj )
			{
				NiGeometry* pkGeometry = FindGeometry(pkObj);
				if ( pkGeometry )
				{
					return pkGeometry;
				}
			}
		}
	}
	return NULL;
}

NiCamera* CNifOp::FindCameraByName(NiNode* pkRoot, const char* pstrCameraName)
{
	if ( !pstrCameraName )
	{
		return FindCamera(pkRoot);
	}
	else
	{
		NiAVObject* pkObj = pkRoot->GetObjectByName(pstrCameraName);
		///无法查询镜头时返回NULL pighuang 20070528
		if (pkObj)
		{
			return FindCamera(pkObj);
		}
		else
		{
			return NULL;
		}
	}
}

NiMatrix3& CNifOp::AdjustRotate(NiMatrix3& kMat)
{
	NiMatrix3 kMatLocal;
	kMatLocal.SetCol(0, 0, -1, 0);
	kMatLocal.SetCol(1, 0, 0, 1);
	kMatLocal.SetCol(2, -1, 0, 0);
	NiMatrix3 kMatLocalInv;
	kMatLocal.Inverse(kMatLocalInv);
	kMat = kMat * kMatLocalInv;
	return kMat;
}

void CNifOp::AttachChild(NiNode* pkNode, NiAVObject* pkObj)
{
	assert(pkNode && pkObj);
	pkNode->AttachChild(pkObj);
	pkObj->UpdateNodeBound();
	pkObj->UpdateProperties();
	pkObj->UpdateEffects();
	pkObj->Update(0);
}