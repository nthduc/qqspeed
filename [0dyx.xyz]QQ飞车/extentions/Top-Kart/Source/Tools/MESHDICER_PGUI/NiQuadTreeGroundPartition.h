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

#ifndef NIQUADTREEGROUNDPARTITION_H
#define NIQUADTREEGROUNDPARTITION_H

#include "NiGroundPartition.h"
class NiPick;


class NiQuadTreeGroundPartition : public NiGroundPartition
{
public:

	NiQuadTreeGroundPartition(unsigned int uiMaxTriangles,
        bool bShareTriangles);
	virtual ~NiQuadTreeGroundPartition();

    // Given a scene graph build a quad tree ground partition. 
	virtual void BuildGround(NiNode* pkGround);

    // Given a world position, pick down the Z axis colliding with the ground 
    // and returning the height of the collision. 
    virtual bool GetGroundElevation(NiPoint3& kWorldPosition, float& fHeight);

protected:

    class NiQuadTreeNode : public NiMemObject
    {
    public:
        NiQuadTreeNode(NiPoint3 kCenter, float fXWidth, float fYWidth);
        ~NiQuadTreeNode();

        // Using the master triangle list build lists for each child quad. 
        // If a triangle is in multiple quads then it is added to the parent 
        // list. If the triangle is unique to a quad then it is added to the 
        // quad’s list.
        void BuildQuadTreeNodeNoTriangleSharing(
            NiTPrimitiveArray<const NiPoint3*>& pkTriangles, 
            NiTPrimitiveArray<const NiPoint3*>& pkNormals,
            unsigned int uiMaxTriangles, unsigned int iDepth);

        // Using the master triangle list build lists for each child quad. If a
        // triangle is in multiple quads then it is added to each quad list. 
        void BuildQuadTreeNodeTriangleSharing(
            NiTPrimitiveArray<const NiPoint3*>& pkTriangles, 
            NiTPrimitiveArray<const NiPoint3*>& pkNormals,
            unsigned int uiMaxTriangles, unsigned int iDepth);

        // Given a world position, pick down the Z axis colliding with the  
        // ground and returning the height of the collision. 
        bool GetGroundElevation(NiPick* pkPicker, NiPoint3& kWorldPosition, 
            float& fHeight);

        // Build a Gamebryo Scene graph which represents the Quad Tree.
        NiAVObject* RecursiveBuildHeirarchy();

        // Recursively find the total number of quadrents. 
        void GetNumQuadrents(int& iTotal, int& iLeaf);

    protected:

        NiQuadTreeNode* m_pkChildQuads[4];
        NiAVObjectPtr m_spSharedObject;
        NiPoint3 m_kCenter;
        float m_fXWidth;
        float m_fYWidth;
    };


	NiPoint3 m_kGroundCenter;
    unsigned int m_uiMaxTriangles;
	NiPick* m_pkGroundPicker;
    NiQuadTreeNode* m_pkTreeRoot;
    bool m_bShareTriangles;


    // Define the nodes as friends
    friend NiQuadTreeNode;

};

#endif