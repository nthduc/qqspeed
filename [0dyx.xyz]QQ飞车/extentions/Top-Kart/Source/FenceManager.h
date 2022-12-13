#pragma once

class CFenceManager
{
public:
	CFenceManager(NiAVObject * pkFenceNode, NxScene* pScene,NiPhysXScene* pNiScene);
	~CFenceManager(void);
	inline NxActor* GetActor(){
		return m_pkFenceActor;
	}

protected:
	static void TransformModelToWorldInPlace(NiAVObject* pkObj, bool bNormals);
	static void TransformPointsBasicInPlace(unsigned short uiVerts,
		float* pModel, const NiTransform* pXform);
	static void TransformVectorsBasicInPlace(unsigned short uiVerts,
		float* pModel, const NiTransform* pXform);

	void CreateBlocks(const NiAVObject* pNode, NxScene* pScene);

	NxActor* m_pkFenceActor;
};
