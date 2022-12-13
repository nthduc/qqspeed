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

#include "NiGroundPartition.h"
#include <NiNode.h>
#include <NiOptTrishape.h>
#include <NiCollision.h>

//---------------------------------------------------------------------------
NiGroundPartition::NiGroundPartition()
{
	m_spGround = NULL;
}
//---------------------------------------------------------------------------
NiGroundPartition::~NiGroundPartition()
{
	m_spGround = NULL;
}
//---------------------------------------------------------------------------
NiAVObjectPtr NiGroundPartition::BuildGroundPartition(NiAVObject* pkGround, 
	float fMinX, float fMinY, float fMaxX, float fMaxY)
{
    // Given an area and scene graph build ground geometry.
    NiTPrimitiveArray<const NiPoint3*> pkTriangles(100000, 10000);
    NiTPrimitiveArray<const NiPoint3*> pkNormals(100000, 10000);

    // Build the triangle and normal list for given area
    RecursivelyBuildGroundPartition(pkGround, pkTriangles, pkNormals,
        fMinX, fMinY, fMaxX, fMaxY);

    // Create the Ground Trishape or NiNode
    NiAVObjectPtr spGeometry = BuildGroundGeometry(pkTriangles, pkNormals);

    // Create a Partition Name
    if (spGeometry != NULL)
    {
        char acBuffer[256];
        NiSprintf(acBuffer, 256, "Partition: MinX=%f MaxX=%f MinY=%f MaxY=%f", 
            fMinX, fMaxX, fMinY, fMaxY);

        spGeometry->SetName(acBuffer);
    }

    return spGeometry;
}
//---------------------------------------------------------------------------
NiAVObjectPtr NiGroundPartition::BuildGroundPartition(
    NiTPrimitiveArray<const NiPoint3*>& pkTriangles, 
    NiTPrimitiveArray<const NiPoint3*>& pkNormals,
    float fMinX, float fMinY, float fMaxX, float fMaxY)
{
    // Create the Ground Trishape or NiNode
    NiAVObjectPtr spGeometry = BuildGroundGeometry(pkTriangles, pkNormals);

    // Create a Partition Name
    if (spGeometry != NULL)
    {
        char acBuffer[256];
        NiSprintf(acBuffer, 256, "Partition: MinX=%f MaxX=%f MinY=%f MaxY=%f", 
            fMinX, fMaxX, fMinY, fMaxY);

        spGeometry->SetName(acBuffer);
    }

    return spGeometry;
}
//---------------------------------------------------------------------------
NiAVObjectPtr NiGroundPartition::BuildGroundGeometry(
    NiTPrimitiveArray<const NiPoint3*>& pkTriangles, 
    NiTPrimitiveArray<const NiPoint3*>& pkNormals)
{
    // Given a list of triangles and normals build one or more NiTriShapes. 
    // Each NiTriShape will have a random color for debugging purposes. 

    // Check for an empty triangle list
    if (pkTriangles.GetSize() == 0)
    {
        return NULL;
    }

    unsigned int iMaxTriangles = 10000;
    bool bSingleTriShape = (pkTriangles.GetSize() <= (iMaxTriangles * 3));

    NiNode* pkTriShapeRoot = NULL;
    if (!bSingleTriShape)
    {
        pkTriShapeRoot = NiNew NiNode;
    }

    // For Debugging Randomly pick a color
    float fRed = NiUnitRandom();
    float fGreen = NiUnitRandom();
    float fBlue = NiUnitRandom();

    unsigned int uiCurrent = 0;

    // Create as many trishapes as we need
    while(pkTriangles.GetSize() > 0)
    {

        // Build a new Trishape

        // The consturctor for NiOptVertex is very slow and making thousands
        // of function calls is also very slow. Hear we allocate in a single
        // call THEN clear Loop through "Constructing" the vertices
        unsigned int uiNumVertices = (unsigned int)NiMin(
            (int)(iMaxTriangles*3), (int)pkTriangles.GetSize());

        NiOptVertex* pkVertices = NiNew NiOptVertex[uiNumVertices];

        unsigned int* puiTriList = NiAlloc(unsigned int, uiNumVertices);

        NiOptVertex* pkTempVertex = pkVertices;
        int iLoop;
        for (iLoop = 0; iLoop < (int)uiNumVertices; iLoop++, pkTempVertex++,
            uiCurrent++)
        {
            pkTempVertex->m_uiNumUVs = 0;
            pkTempVertex->m_uiNumVertexFloats = 0;
            pkTempVertex->m_uiNumBones = 0;
            pkTempVertex->m_puiBoneIndex = 0;
            pkTempVertex->m_pfWeight = 0;

            pkTempVertex->m_kPosition = *pkTriangles.GetAt(uiCurrent);
            pkTempVertex->m_kNormal = *pkNormals.GetAt(uiCurrent);

            pkTempVertex->m_kColor.r = fRed;
            pkTempVertex->m_kColor.g = fGreen;
            pkTempVertex->m_kColor.b = fBlue;
            pkTempVertex->m_kColor.a = 1.0f;

            puiTriList[iLoop] = iLoop;
        }

        NiTransform kRootToSkin;
        kRootToSkin.MakeIdentity();

        NiOptTriShape* pkNiOptTriShape = NULL;
        pkNiOptTriShape = NiNew NiOptTriShape(
            uiNumVertices, 
            pkVertices, 
            /* uiNumBones = */ 0,
            /* ppvBones = */ NULL,
            /* pvRootBone = */ NULL,
            /* pInitXForms = */ NULL,
            /* kRootToSkin = */ kRootToSkin,
            uiNumVertices,
            puiTriList,
            /* bAllowLargeTriLists = */ true,
            false);

        // CREATE THE OPTIMIZED TRISHAPE
        NiTriShape* pkConvertedTri;
        pkNiOptTriShape->ConvertTo(pkConvertedTri);

        // Delete the OptTriShape
        NiDelete pkNiOptTriShape;

        pkConvertedTri->UpdateNodeBound();

        if (!bSingleTriShape)
            pkTriShapeRoot->AttachChild(pkConvertedTri);
        else
            return pkConvertedTri;
    }

    return pkTriShapeRoot;
}
//---------------------------------------------------------------------------
unsigned int NiGroundPartition::RecursivelyBuildGroundPartitionSize(
    NiAVObject* pkGround, float fMinX, float fMinY, float fMaxX, float fMaxY)
{
    // Walk the scene graph. If a NiTriShape’s bound intersect with the region 
    // then total the NiTriShape’s triangles that are in the region. 
    unsigned int uiTotalVertices = 0;

    if (NiIsKindOf(NiTriBasedGeom, pkGround))
    {
        NiTriBasedGeom* pkTriBasedGeom = (NiTriBasedGeom*)pkGround;

        // Check for bounds intersecting the partitions
        if (BoundingSphereInPartition(pkTriBasedGeom->GetWorldBound(), fMinX,
            fMinY, fMaxX, fMaxY))
        {
            NiPoint3* pkP1;
            NiPoint3* pkP2;
            NiPoint3* pkP3;

            // Get the Normal List
            NiCollisionData* pkCD = NiDynamicCast(NiCollisionData, 
                pkTriBasedGeom->GetCollisionObject());

            assert(pkCD);
            const NiPoint3* pkWorldNormals = pkCD->GetWorldNormals();

            unsigned short usLoop;
            for (usLoop = 0; usLoop < pkTriBasedGeom->GetTriangleCount(); 
                usLoop++)
            {
                pkCD->GetWorldTriangle(usLoop, pkP1, pkP2, pkP3);
                unsigned short usI0, usI1, usI2;
                pkTriBasedGeom->GetTriangleIndices(usLoop, usI0, usI1, usI2);

                // Check for degenerate triangles
                // Check to see if the triangle is in the bounds
                if ((usI0 != usI1) && (usI1 != usI2) && (usI0 != usI2) &&
                    TriangleInPartition(pkP1, pkP2, pkP3, fMinX, fMinY, fMaxX, 
                    fMaxY))
                {
                    uiTotalVertices += 3;
                }
            }
        }
        return uiTotalVertices;
    }

    // Loop through any children if there are any
    if (NiIsKindOf(NiNode, pkGround))
    {
        NiNode* pkNode = (NiNode*)pkGround;

        unsigned int uiLoop;
        for (uiLoop = 0;uiLoop < pkNode->GetChildCount(); uiLoop++)
        {
            if (pkNode->GetAt(uiLoop) != NULL)
            {
                uiTotalVertices += RecursivelyBuildGroundPartitionSize(
                    pkNode->GetAt(uiLoop), fMinX, fMinY, fMaxX, fMaxY);
            }
        }
    }

    return uiTotalVertices;
}
//---------------------------------------------------------------------------
void NiGroundPartition::RecursivelyBuildGroundPartition(NiAVObject* pkGround,
    NiTPrimitiveArray<const NiPoint3*>& pkTriangles, 
    NiTPrimitiveArray<const NiPoint3*>& pkNormals, float fMinX, float fMinY, 
    float fMaxX, float fMaxY)
{
    // Walk the scene graph. If a NiTriShape’s bound intersect with the region 
    // then add the NiTriShape’s vertices and normals to the lists. 

    if (NiIsKindOf(NiTriBasedGeom, pkGround))
    {
        NiTriBasedGeom* pkTriBasedGeom = (NiTriBasedGeom*)pkGround;

        // Check for bounds intersecting the partitions
        if (BoundingSphereInPartition(pkTriBasedGeom->GetWorldBound(), fMinX,
            fMinY, fMaxX, fMaxY))
        {
            NiPoint3* pkP1;
            NiPoint3* pkP2;
            NiPoint3* pkP3;

            // Get the Normal List
            NiCollisionData* pkCD = NiDynamicCast(NiCollisionData, 
                pkTriBasedGeom->GetCollisionObject());

            assert(pkCD);
            const NiPoint3* pkWorldNormals = pkCD->GetWorldNormals();

            unsigned short usLoop;
            for (usLoop = 0;
                usLoop < pkTriBasedGeom->GetTriangleCount(); usLoop++)
            {
                pkCD->GetWorldTriangle(usLoop, pkP1, pkP2, pkP3);
                unsigned short usI0, usI1, usI2;
                pkTriBasedGeom->GetTriangleIndices(usLoop, usI0, usI1, usI2);

                // Check for degenerate triangles
                // Check to see if the triangle is in the bounds
                if ((usI0 != usI1) && (usI1 != usI2) && (usI0 != usI2) &&
                    TriangleInPartition(pkP1, pkP2, pkP3, fMinX, fMinY, fMaxX, 
                    fMaxY))
                {
                    pkTriangles.Add(pkP1);
                    pkTriangles.Add(pkP2);
                    pkTriangles.Add(pkP3);

                    pkNormals.Add(&pkWorldNormals[usI0]);
                    pkNormals.Add(&pkWorldNormals[usI1]);
                    pkNormals.Add(&pkWorldNormals[usI2]);
                }
            }
        }
        return;
    }

    // Loop through any children if there are any
    if (NiIsKindOf(NiNode, pkGround))
    {
        NiNode* pkNode = (NiNode*)pkGround;

        unsigned int uiLoop;
        for (uiLoop = 0;uiLoop < pkNode->GetChildCount(); uiLoop++)
        {
            if (pkNode->GetAt(uiLoop) != NULL)
            {
                RecursivelyBuildGroundPartition(pkNode->GetAt(uiLoop), 
                    pkTriangles, pkNormals, fMinX, fMinY, 
                    fMaxX, fMaxY);
            }
        }
    }
}
//---------------------------------------------------------------------------
void NiGroundPartition::BuildGroundPartition(
    NiTPrimitiveArray<const NiPoint3*>&  pkTriangles, 
    NiTPrimitiveArray<const NiPoint3*>&  pkNormals,
    float fMinX, float fMinY, float fMaxX, float fMaxY,
    NiTPrimitiveArray<const NiPoint3*>& pkNewTriangles, 
    NiTPrimitiveArray<const NiPoint3*>& pkNewNormals)
{
    // Given a list of triangles and normals, build a new list of triangles and
    // normals that are in an XY region. 

    // Points of a triangle
    const NiPoint3* pkP1;
    const NiPoint3* pkP2;
    const NiPoint3* pkP3;

    unsigned int uiLoop;
    for (uiLoop = 0; uiLoop < pkTriangles.GetSize(); uiLoop += 3)
    {
        pkP1 = pkTriangles.GetAt(uiLoop);
        pkP2 = pkTriangles.GetAt(uiLoop+1);
        pkP3 = pkTriangles.GetAt(uiLoop+2);

        // Check to see if the triangle is in the bounds
        if (TriangleInPartition(pkP1, pkP2, pkP3, fMinX, fMinY, fMaxX, fMaxY))
        {
            pkNewTriangles.Add(pkP1);
            pkNewTriangles.Add(pkP2);
            pkNewTriangles.Add(pkP3);

            pkNewNormals.Add(pkNormals.GetAt(uiLoop));
            pkNewNormals.Add(pkNormals.GetAt(uiLoop+1));
            pkNewNormals.Add(pkNormals.GetAt(uiLoop+2));
        }
    }
}
//---------------------------------------------------------------------------
int NiGroundPartition::FindTotalTriangles(NiAVObject* pkObject)
{
    // Walk the scene graph and get the total number of triangles. This count 
    // will include degenerate triangles. 

	int iTotal = 0;

	if (NiIsKindOf(NiTriBasedGeom, pkObject))
	{
		iTotal += ((NiTriBasedGeom*)pkObject)->GetTriangleCount();
	}

	// Loop through any children if there are any
	if (NiIsKindOf(NiNode, pkObject))
	{
		NiNode* pkNode = (NiNode*)pkObject;
		
        unsigned int uiLoop;
		for (uiLoop = 0; uiLoop < pkNode->GetChildCount(); uiLoop++)
		{
			if (pkNode->GetAt(uiLoop) != NULL)
			{
				iTotal += FindTotalTriangles(pkNode->GetAt(uiLoop));
			}
		}
	}
	
	return iTotal;
}
//---------------------------------------------------------------------------
