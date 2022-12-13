/********************************************************************
	作者:	　wincoqin
	时间:	  2007/04/18
    文件名:   JointCloth.h
	说明:     []
*********************************************************************/
#pragma once

// libfile
#include "NxJoint.h"
#include "NxJointDesc.h"

// userfile


// predefine

typedef enum enmClothType
{
	ECT_BALLOON = 0,
	ECT_FLAG	= 1,
}ENM_CLOTHTYPE;

class CJointCloth
{
	struct PatchLink
	{
		NxSphericalJoint* xJoint;
		NxSphericalJoint* zJoint;
	};
public:
	CJointCloth();
	virtual ~CJointCloth();

	static CJointCloth * CreateJointCloth();

	void Update();
	void AttachTo(NiAVObject * pObj);
	void BindObject(NiAVObject * pObj);
	void UnBind();
	NiAVObject * GetAVObject()
	{
		return m_spGeometry;
	}

protected:
	void UpdateClothPose(const NiTransform& kTransform);
	void SynchronizeModel();

	NxActor*			CreatePatch(const NxVec3& pos, const NxVec3& patchDim, const NxReal density, NxScene * pScene);
	void				CreateMattress(const NxVec3& pos, const int width, const int height, const NxVec3& patchDim, NxScene * pScene);
	NxSphericalJoint*	CreateClothSphericalJoint(NxActor* a0, NxActor* a1, const NxVec3& globalAnchor, const NxVec3& globalAxis, NxScene * pScene);

	NiTriShape *        CreateStickGeometry(const NxVec3 & pos, unsigned int unKnotNum, float fStickLenth, float fHalfWidth, const char * pPicturePath);
	void				CreateStick();

	void				UpdateAttachedNodeParam();

	void				UpdateKnotVertex(NxActor * pActor, NiPoint3 * pVertex);
		
private:
	unsigned int m_unWidth;
	unsigned int m_unHeight;
	float		 m_fRadius;

	NxActor * m_pAttachedNode;
	NxActor * m_pLastActor;
	NxMat34   m_kAttachedNodePose;
	NxMat34	  m_kAttachedNodeInvesePose;

	NxActor** m_ppClothPatches;

	NiAVObjectPtr m_spAttachedObject;
	NiAVObjectPtr m_spBindingObject;
	NiGeometryPtr m_spGeometry;
};

