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

#ifndef NIGROUNDPARTITION_H
#define NIGROUNDPARTITION_H

#include <NiAVObject.h>
#include <NiTList.h>


class NiGroundPartition : public NiMemObject
{
public:

	NiGroundPartition();
	virtual ~NiGroundPartition();

    // Given a scene graph build a ground partition. This function is abstract 
    //and must be implemented in the subclass.	
    virtual void BuildGround(NiNode* pkGround) = 0;

    // Return the scene graph which represents the partitioned ground.
	virtual NiAVObjectPtr GetGround();

    // Given a world position, pick down the Z axis colliding with the ground 
    // and returning the height of the collision. This function is abstract 
    // and must be implemented in the subclass.
	virtual bool GetGroundElevation(NiPoint3& kWorldPosition, float& fHeight)=0;

    // Check if the bounding sphere is contained in the XY region. 
	static bool BoundingSphereInPartition(const NiBound& kBound, float fMinX, 
        float fMinY, float fMaxX, float fMaxY);

    // Check if the triangle is contained in this XY region.  This code only 
    // checks the vertices it does not check for a triangle that contains the 
    // region or the region clips one of the triangle edges.
    static bool TriangleInPartition(const NiPoint3* pkP0, 
        const NiPoint3* pkP1, const NiPoint3* pkP2, 
        float fMinX, float fMinY, float fMaxX, 
        float fMaxY);

    // Check if the triangle is contained in this XY region.  This code only 
    // checks the vertices it does not check for a triangle that contains the 
    // region or the region clips one of the triangle edges.
	static bool TriangleInPartition(const NiPoint3* pkP1, const NiPoint3* pkP2, 
        const NiPoint3* pkP3, NiPoint3& kCenter, float fXWidth, 
        float fYWidth);

protected:

    static bool PickSide(float fRX, float fRY, const NiPoint3* pkP0, 
        const NiPoint3* pkP1);

    // Given an area and scene graph build ground geometry.
	static NiAVObjectPtr BuildGroundPartition(NiAVObject* pkGround, 
        float fMinX, float fMinY, float fMaxX, float fMaxY);

    static NiAVObjectPtr BuildGroundPartition(
        NiTPrimitiveArray<const NiPoint3*>& pkTriangles, 
        NiTPrimitiveArray<const NiPoint3*>& pkNormals,
        float fMinX, float fMinY, float fMaxX, float fMaxY);


    // Given a list of triangles and normals build one or more NiTriShapes. 
    // Each NiTriShape will have a random color for debugging purposes. 
    static NiAVObjectPtr BuildGroundGeometry(
        NiTPrimitiveArray<const NiPoint3*>& pkTriangles, 
        NiTPrimitiveArray<const NiPoint3*>& pkNormals);

    // Walk the scene graph. If a NiTriShape’s bound intersect with the region 
    // then total the NiTriShape’s triangles that are in the region. 
    static unsigned int RecursivelyBuildGroundPartitionSize(
        NiAVObject* pkGround, float fMinX, float fMinY, float fMaxX, 
        float fMaxY);

    // Walk the scene graph. If a NiTriShape’s bound intersects with the region
    // then add the NiTriShape’s vertices and normals to the lists. 
    static void RecursivelyBuildGroundPartition(NiAVObject* pkGround, 
        NiTPrimitiveArray<const NiPoint3*>& pkTriangles, 
        NiTPrimitiveArray<const NiPoint3*>& pkNormals, 
        float fMinX, float fMinY, 
        float fMaxX, float fMaxY);

    // Given a list of triangles and normals, build a new list of triangles and
    // normals that are in an XY region. 
    static void BuildGroundPartition(
        NiTPrimitiveArray<const NiPoint3*>& pkTriangles, 
        NiTPrimitiveArray<const NiPoint3*>& pkNormals,
        float fMinX, float fMinY, float fMaxX, float fMaxY,
        NiTPrimitiveArray<const NiPoint3*>& pkNewTriangles,
        NiTPrimitiveArray<const NiPoint3*>& pkNewNormals);

    // Walk the scene graph and get the total number of triangles. This count 
    // will include degenerate triangles. 
	static int FindTotalTriangles(NiAVObject* pkObject);

	NiAVObjectPtr m_spGround;

};

#include "NiGroundPartition.inl"

#endif