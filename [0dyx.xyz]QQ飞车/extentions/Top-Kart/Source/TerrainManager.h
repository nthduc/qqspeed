// EMERGENT GAME TECHNOLOGIES PROPRIETARY INFORMATION
//
// This software is supplied under the terms of a license agreement or
// nondisclosure agreement with Emergent Game Technologies and may not 
// be copied or disclosed except in accordance with the terms of that 
// agreement.
//
//      Copyright (c) 1996-2006 Emergent Game Technologies.
//      All Rights Reserved.
//
// Emergent Game Technologies, Chapel Hill, North Carolina 27517
// http://www.emergent.net

#ifndef TERRAIN_MANAGER_H
#define TERRAIN_MANAGER_H

#include <NiAVObject.h>
#include <NiPick.h>

class NxActor;

class TerrainManager : public NiMemObject
{
public:
	virtual ~TerrainManager();

	virtual bool GetHeight(float fX, float fY, float &fHeight) const = 0;
	virtual bool GetHeightAndNormal(float fX, float fY, 
		float &fHeight, NiPoint3& kNormal) const = 0;
	virtual bool GetHeightAndColor(float fX, float fY, 
		float &fHeight, NiColorA& kColor) const = 0;

	virtual NiAVObject* GetGroundSceneGraph() const;

	virtual float GetMaxX() const = 0;
	virtual float GetMaxY() const = 0;
	virtual float GetMinX() const = 0;
	virtual float GetMinY() const = 0;

	virtual unsigned int GetNumGeysers() const = 0;
	virtual NiPoint3& GetBlockReleasePt(const unsigned int uiN) = 0;
	NiAVObject *  GetTerrainGraph(){return m_spTerrainGraph;}

protected:
	NiAVObjectPtr m_spTerrainGraph;
};

class LookupTerrainManager : public TerrainManager
{
public:
	LookupTerrainManager(NiAVObject * pkNodeTerrain, 
		const char* pcShadowFilename,NxScene* pScene);
	virtual ~LookupTerrainManager();

	virtual bool GetHeight(float fX, float fY, float &fHeight) const;
	virtual bool GetHeightAndNormal(float fX, float fY, 
		float &fHeight, NiPoint3& kNormal) const;
	virtual bool GetHeightAndColor(float fX, float fY, 
		float &fHeight, NiColorA& kColor) const;

	virtual float GetMaxX() const;
	virtual float GetMaxY() const;
	virtual float GetMinX() const;
	virtual float GetMinY() const;

	virtual unsigned int GetNumGeysers() const;
	virtual NiPoint3& GetBlockReleasePt(const unsigned int uiN);

protected:
	static void TransformModelToWorldInPlace(NiAVObject* pkObj, bool bNormals);
	static void TransformPointsBasicInPlace(unsigned short uiVerts,
		float* pModel, const NiTransform* pXform);
	static void TransformVectorsBasicInPlace(unsigned short uiVerts,
		float* pModel, const NiTransform* pXform);

	//load terrain
	bool AppendMesh(NxActorDesc* pNxActorDesc, const NiTriBasedGeom* pkTriBaseGeom);
	//

	class VertVals : public NiMemObject
	{
	public:
		float m_fZ;
		NiPoint3 m_kNormal;
		NiColorA m_kColor;
	};

	unsigned short m_usVertsX;
	unsigned short m_usVertsY;
	float m_fMinX;
	float m_fMaxX;
	float m_fMinY;
	float m_fMaxY;
	float m_fBlockDimX;
	float m_fBlockDimY;
	float m_fOOBlockDimX;
	float m_fOOBlockDimY;
	VertVals* m_pkLookupTable;

	unsigned int m_uiNumGeysers;
	NiPoint3* m_pkBlockReleasePts;
	NxActor* m_pkTerrainActor;
};

#include "TerrainManager.inl"

#endif // #ifdef TERRAIN_MANAGER_H
