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

#include "NiQuadTreeGroundPartition.h"
#include <NiNode.h>
#include <NiPick.h>
#include <NiMaterialProperty.h>
#include <NiVertexColorProperty.h>
#include <NiSystem.h>

//---------------------------------------------------------------------------
NiQuadTreeGroundPartition::NiQuadTreeNode::NiQuadTreeNode(NiPoint3 kCenter, 
    float fXWidth, float fYWidth)
{
    m_kCenter = kCenter;
    m_fXWidth = fXWidth;
    m_fYWidth = fYWidth;

	m_pkChildQuads[0] = NULL;
	m_pkChildQuads[1] = NULL;
	m_pkChildQuads[2] = NULL;
	m_pkChildQuads[3] = NULL;
}
//---------------------------------------------------------------------------
NiQuadTreeGroundPartition::NiQuadTreeNode::~NiQuadTreeNode()
{
    delete m_pkChildQuads[0];
    delete m_pkChildQuads[1];
    delete m_pkChildQuads[2];
    delete m_pkChildQuads[3];
}
//---------------------------------------------------------------------------
void NiQuadTreeGroundPartition::NiQuadTreeNode::
BuildQuadTreeNodeNoTriangleSharing(
    NiTPrimitiveArray<const NiPoint3*>& pkTriangles, 
    NiTPrimitiveArray<const NiPoint3*>& pkNormals,
    unsigned int uiMaxTriangles, unsigned int uiDepth)
{
    // Using the master triangle list build lists for each child quad. If a 
    // triangle is in multiple quads then it is added to the parent list. If 
    // the triangle is unique to a quad then it is added to the quad’s list.


    // Check to see if we dont need to sub divide
    if (((pkTriangles.GetSize() / 3) <= uiMaxTriangles) || (uiDepth == 0))
    {
        // Build the Geometry for this partition
        m_spSharedObject = NiGroundPartition::BuildGroundGeometry(
            pkTriangles, pkNormals);
        return;
    }

    // Setup the Quad Data
    float fQuadXWidth = m_fXWidth / 2.0f;
    float fQuadYWidth = m_fYWidth / 2.0f;
    NiPoint3 kQuadCenter[4];
    kQuadCenter[0] = m_kCenter + NiPoint3(fQuadXWidth, fQuadYWidth, 0.0f);
    kQuadCenter[1] = m_kCenter + NiPoint3(fQuadXWidth, -fQuadYWidth, 0.0f);
    kQuadCenter[2] = m_kCenter + NiPoint3(-fQuadXWidth, fQuadYWidth, 0.0f);
    kQuadCenter[3] = m_kCenter + NiPoint3(-fQuadXWidth, -fQuadYWidth, 0.0f);

    int iInQuad;
    bool bInMultiple;
    const NiPoint3* pP1;
    const NiPoint3* pP2;
    const NiPoint3* pP3;


    // PreCompute the sizes
    unsigned int uiLocalSize = 0;
    unsigned int auiSize[4] = {0, 0, 0, 0};

    unsigned int uiCurrent;
    // Sort all of the triangles into the quads
    for (uiCurrent = 0; uiCurrent < pkTriangles.GetSize(); uiCurrent += 3)
    {
        pP1 = pkTriangles.GetAt(uiCurrent);
        pP2 = pkTriangles.GetAt(uiCurrent+1);
        pP3 = pkTriangles.GetAt(uiCurrent+2);

        iInQuad = -1;
        bInMultiple = false;

        // Check against each of the quadrents
        int iLoop;
        for (iLoop = 0; (iLoop < 4) && !bInMultiple; iLoop++)
        {
            if (TriangleInPartition(pP1, pP2, pP3, 
                kQuadCenter[iLoop], fQuadXWidth, fQuadYWidth))
            {
#ifdef NO_SHARED
                if (iInQuad >= 0)
                    bInMultiple = true;
#endif

                iInQuad = iLoop;
            }
        }


        if (bInMultiple)
        {
            // Add the Triangle and Normals to the local list
            uiLocalSize += 3;
        }
        else
        {
            if (iInQuad == -1)
            {
                int i = 0;
//                assert(false);
            }
            else
            {
                auiSize[iInQuad] += 3;
            }
        }
    }



    NiTPrimitiveArray<const NiPoint3*> pkLocalTriangles(uiLocalSize, 32767);
    NiTPrimitiveArray<const NiPoint3*> pkLocalNormals(uiLocalSize, 32767);

    NiTPrimitiveArray<const NiPoint3*> pkQuadTriangles[4];
    NiTPrimitiveArray<const NiPoint3*> pkQuadNormals[4];

    pkQuadTriangles[0].SetSize(auiSize[0]);
    pkQuadTriangles[0].SetGrowBy(32767);
    pkQuadNormals[0].SetSize(auiSize[0]);
    pkQuadNormals[0].SetGrowBy(32767);

    pkQuadTriangles[1].SetSize(auiSize[1]);
    pkQuadTriangles[1].SetGrowBy(32767);
    pkQuadNormals[1].SetSize(auiSize[1]);
    pkQuadNormals[1].SetGrowBy(32767);

    pkQuadTriangles[2].SetSize(auiSize[2]);
    pkQuadTriangles[2].SetGrowBy(32767);
    pkQuadNormals[2].SetSize(auiSize[2]);
    pkQuadNormals[2].SetGrowBy(32767);

    pkQuadTriangles[3].SetSize(auiSize[3]);
    pkQuadTriangles[3].SetGrowBy(32767);
    pkQuadNormals[3].SetSize(auiSize[3]);
    pkQuadNormals[3].SetGrowBy(32767);


    // Sort all of the triangles into the quads
    for (uiCurrent = 0; uiCurrent < pkTriangles.GetSize(); uiCurrent += 3)
    {
        pP1 = pkTriangles.GetAt(uiCurrent);
        pP2 = pkTriangles.GetAt(uiCurrent+1);
        pP3 = pkTriangles.GetAt(uiCurrent+2);

        iInQuad = -1;
        bInMultiple = false;

        // Check against each of the quadrants
        int iLoop;
        for (iLoop = 0; (iLoop < 4) && !bInMultiple; iLoop++)
        {
            if (TriangleInPartition(pP1, pP2, pP3, 
                kQuadCenter[iLoop], fQuadXWidth, fQuadYWidth))
            {
#ifdef NO_SHARED
                if (iInQuad >= 0)
                    bInMultiple = true;
#endif

                iInQuad = iLoop;
            }
        }


        if (bInMultiple)
        {
            // Add the Triangle and Normals to the local list
            pkLocalTriangles.Add(pP1);
            pkLocalTriangles.Add(pP2);
            pkLocalTriangles.Add(pP3);

            pkLocalNormals.Add(pkNormals.GetAt(uiCurrent));
            pkLocalNormals.Add(pkNormals.GetAt(uiCurrent+1));
            pkLocalNormals.Add(pkNormals.GetAt(uiCurrent+2));
        }
        else
        {
            if (iInQuad == -1)
            {
                  int i = 0;
//                assert(false);
            }
            else
            {
                // Add the Triangle and Normals to the local list
                pkQuadTriangles[iInQuad].Add(pP1);
                pkQuadTriangles[iInQuad].Add(pP2);
                pkQuadTriangles[iInQuad].Add(pP3);

                pkQuadNormals[iInQuad].Add(pkNormals.GetAt(uiCurrent));
                pkQuadNormals[iInQuad].Add(pkNormals.GetAt(uiCurrent+1));
                pkQuadNormals[iInQuad].Add(pkNormals.GetAt(uiCurrent+2));
            }
        }
    }

    // Build the parent AVObject
    m_spSharedObject = NiGroundPartition::BuildGroundGeometry(
        pkLocalTriangles, pkLocalNormals);

    // Build the Quad Tree Nodes with Unique triangle lists
    int iLoop;
    for (iLoop = 0; iLoop < 4; iLoop++)
    {
        if (pkQuadTriangles[iLoop].GetSize() > 0)
        {
            // Create the New Child Quad
            m_pkChildQuads[iLoop] = NiNew NiQuadTreeNode( kQuadCenter[iLoop], 
                fQuadXWidth, fQuadYWidth);

            // Build that new Quad Child reducing the current depth
            m_pkChildQuads[iLoop]->BuildQuadTreeNodeNoTriangleSharing( 
                pkQuadTriangles[iLoop], pkQuadNormals[iLoop], uiMaxTriangles, 
                uiDepth - 1);
        }
    }

}
//---------------------------------------------------------------------------
void NiQuadTreeGroundPartition::NiQuadTreeNode::
BuildQuadTreeNodeTriangleSharing( 
    NiTPrimitiveArray<const NiPoint3*>& pkTriangles, 
    NiTPrimitiveArray<const NiPoint3*>& pkNormals, 
    unsigned int uiMaxTriangles, unsigned int uiDepth)
{
    // Using the master triangle list build lists for each child quad. If a 
    // triangle is in multiple quads then it is added to each quad list. 

    // Check to see if we dont need to sub divide
    if (((pkTriangles.GetSize() / 3) <= uiMaxTriangles) || (uiDepth == 0))
    {
        // Build the Geometry for this partition
        m_spSharedObject = NiGroundPartition::BuildGroundGeometry(
            pkTriangles, pkNormals);

        return;
    }

    // Setup the Quad Data
    float fQuadXWidth = m_fXWidth / 2.0f;
    float fQuadYWidth = m_fYWidth / 2.0f;
    NiPoint3 kQuadCenter1 =
        m_kCenter + NiPoint3(fQuadXWidth, fQuadYWidth, 0.0f);;
    NiPoint3 kQuadCenter2 =
        m_kCenter + NiPoint3(fQuadXWidth, -fQuadYWidth, 0.0f);;
    NiPoint3 kQuadCenter3 =
        m_kCenter + NiPoint3(-fQuadXWidth, fQuadYWidth, 0.0f);;
    NiPoint3 kQuadCenter4 =
        m_kCenter + NiPoint3(-fQuadXWidth, -fQuadYWidth, 0.0f);;


    const NiPoint3* pP1;
    const NiPoint3* pP2;
    const NiPoint3* pP3;

    const NiPoint3* pN1;
    const NiPoint3* pN2;
    const NiPoint3* pN3;

    // Compute the Array Sizes
    unsigned int auiSize[4] = {0,0,0,0};


    unsigned int uiCurrent;
    for (uiCurrent = 0; uiCurrent < pkTriangles.GetSize(); uiCurrent += 3)
    {
        pP1 = pkTriangles.GetAt(uiCurrent);
        pP2 = pkTriangles.GetAt(uiCurrent+1);
        pP3 = pkTriangles.GetAt(uiCurrent+2);

        pN1 = pkNormals.GetAt(uiCurrent);
        pN2 = pkNormals.GetAt(uiCurrent+1);
        pN3 = pkNormals.GetAt(uiCurrent+2);


        // Check against each of the quadrents
        if (TriangleInPartition(pP1, pP2, pP3, 
            kQuadCenter1, fQuadXWidth, fQuadYWidth))
        {
            auiSize[0] += 3;
        }

        if (TriangleInPartition(pP1, pP2, pP3, 
            kQuadCenter2, fQuadXWidth, fQuadYWidth))
        {
            auiSize[1] += 3;
        }

        if (TriangleInPartition(pP1, pP2, pP3, 
            kQuadCenter3, fQuadXWidth, fQuadYWidth))
        {
            auiSize[2] += 3;
        }

        if (TriangleInPartition(pP1, pP2, pP3, 
            kQuadCenter4, fQuadXWidth, fQuadYWidth))
        {
            auiSize[3] += 3;
        }
    }

    NiTPrimitiveArray<const NiPoint3*> pkQuadTriangles1(auiSize[0], 10000);
    NiTPrimitiveArray<const NiPoint3*> pkQuadTriangles2(auiSize[1], 10000);
    NiTPrimitiveArray<const NiPoint3*> pkQuadTriangles3(auiSize[2], 10000);
    NiTPrimitiveArray<const NiPoint3*> pkQuadTriangles4(auiSize[3], 10000);

    NiTPrimitiveArray<const NiPoint3*> pkQuadNormals1(auiSize[0], 10000);
    NiTPrimitiveArray<const NiPoint3*> pkQuadNormals2(auiSize[1], 10000);
    NiTPrimitiveArray<const NiPoint3*> pkQuadNormals3(auiSize[2], 10000);
    NiTPrimitiveArray<const NiPoint3*> pkQuadNormals4(auiSize[3], 10000);



    for (uiCurrent = 0; uiCurrent < pkTriangles.GetSize(); uiCurrent += 3)
    {
        pP1 = pkTriangles.GetAt(uiCurrent);
        pP2 = pkTriangles.GetAt(uiCurrent+1);
        pP3 = pkTriangles.GetAt(uiCurrent+2);

        pN1 = pkNormals.GetAt(uiCurrent);
        pN2 = pkNormals.GetAt(uiCurrent+1);
        pN3 = pkNormals.GetAt(uiCurrent+2);


        // Check against each of the quadrents
        if (TriangleInPartition(pP1, pP2, pP3, 
            kQuadCenter1, fQuadXWidth, fQuadYWidth))
        {
            // Add the Triangle and Normals to the local list
            pkQuadTriangles1.Add(pP1);
            pkQuadTriangles1.Add(pP2);
            pkQuadTriangles1.Add(pP3);

            pkQuadNormals1.Add(pN1);
            pkQuadNormals1.Add(pN2);
            pkQuadNormals1.Add(pN3);
        }

        if (TriangleInPartition(pP1, pP2, pP3, 
            kQuadCenter2, fQuadXWidth, fQuadYWidth))
        {
            // Add the Triangle and Normals to the local list
            pkQuadTriangles2.Add(pP1);
            pkQuadTriangles2.Add(pP2);
            pkQuadTriangles2.Add(pP3);

            pkQuadNormals2.Add(pN1);
            pkQuadNormals2.Add(pN2);
            pkQuadNormals2.Add(pN3);
        }

        if (TriangleInPartition(pP1, pP2, pP3, 
            kQuadCenter3, fQuadXWidth, fQuadYWidth))
        {
            // Add the Triangle and Normals to the local list
            pkQuadTriangles3.Add(pP1);
            pkQuadTriangles3.Add(pP2);
            pkQuadTriangles3.Add(pP3);

            pkQuadNormals3.Add(pN1);
            pkQuadNormals3.Add(pN2);
            pkQuadNormals3.Add(pN3);
        }

        if (TriangleInPartition(pP1, pP2, pP3, 
            kQuadCenter4, fQuadXWidth, fQuadYWidth))
        {
            // Add the Triangle and Normals to the local list
            pkQuadTriangles4.Add(pP1);
            pkQuadTriangles4.Add(pP2);
            pkQuadTriangles4.Add(pP3);

            pkQuadNormals4.Add(pN1);
            pkQuadNormals4.Add(pN2);
            pkQuadNormals4.Add(pN3);
        }

    }


    // Build the Quad Tree Nodes with Unique triangle lists
    if (pkQuadTriangles1.GetSize() > 0)
    {
        // Create the New Child Quad
        m_pkChildQuads[0] = NiNew NiQuadTreeNode( kQuadCenter1, 
            fQuadXWidth, fQuadYWidth);

        // Build that new Quad Child reducing the current depth
        m_pkChildQuads[0]->BuildQuadTreeNodeTriangleSharing( 
            pkQuadTriangles1, pkQuadNormals1, uiMaxTriangles, 
            uiDepth - 1);
    }

    if (pkQuadTriangles2.GetSize() > 0)
    {
        // Create the New Child Quad
        m_pkChildQuads[1] = NiNew NiQuadTreeNode( kQuadCenter2, 
            fQuadXWidth, fQuadYWidth);

        // Build that new Quad Child reducing the current depth
        m_pkChildQuads[1]->BuildQuadTreeNodeTriangleSharing( 
            pkQuadTriangles2, pkQuadNormals2, uiMaxTriangles, 
            uiDepth - 1);
    }

    if (pkQuadTriangles3.GetSize() > 0)
    {
        // Create the New Child Quad
        m_pkChildQuads[2] = NiNew NiQuadTreeNode( kQuadCenter3, 
            fQuadXWidth, fQuadYWidth);

        // Build that new Quad Child reducing the current depth
        m_pkChildQuads[2]->BuildQuadTreeNodeTriangleSharing( 
            pkQuadTriangles3, pkQuadNormals3, uiMaxTriangles, 
            uiDepth - 1);
    }

    if (pkQuadTriangles4.GetSize() > 0)
    {
        // Create the New Child Quad
        m_pkChildQuads[3] = NiNew NiQuadTreeNode( kQuadCenter4, 
            fQuadXWidth, fQuadYWidth);

        // Build that new Quad Child reducing the current depth
        m_pkChildQuads[3]->BuildQuadTreeNodeTriangleSharing( 
            pkQuadTriangles4, pkQuadNormals4, uiMaxTriangles, 
            uiDepth - 1);
    }
}
//---------------------------------------------------------------------------
NiAVObject* 
NiQuadTreeGroundPartition::NiQuadTreeNode::RecursiveBuildHeirarchy()
{
    // Build a Gamebryo Scene graph which represents the Quad Tree.

    if ((m_pkChildQuads[0] == NULL) &&
        (m_pkChildQuads[1] == NULL) &&
        (m_pkChildQuads[2] == NULL) &&
        (m_pkChildQuads[3] == NULL))
    {
        return m_spSharedObject;
    }

    NiNode* pkNode = NiNew NiNode;

    if (m_spSharedObject != NULL)
        pkNode->AttachChild(m_spSharedObject);

    int iLoop;
    for (iLoop = 0; iLoop < 4; iLoop++)
    {
        if (m_pkChildQuads[iLoop] != NULL)
        {
            pkNode->AttachChild(
                m_pkChildQuads[iLoop]->RecursiveBuildHeirarchy());
        }
    }

    return pkNode;
}
//---------------------------------------------------------------------------
bool NiQuadTreeGroundPartition::NiQuadTreeNode::GetGroundElevation(
    NiPick* pkPicker, NiPoint3& kWorldPosition, float& fHeight)
{
    // Given a world position, pick down the Z axis colliding with the ground 
    // and returning the height of the collision. 


    // Check the point is in our partition
    if ((kWorldPosition.x < m_kCenter.x - m_fXWidth) ||
        (kWorldPosition.x > m_kCenter.x + m_fXWidth) ||
        (kWorldPosition.y < m_kCenter.y - m_fYWidth) ||
        (kWorldPosition.y > m_kCenter.y + m_fYWidth))
        return false;



    // Start but checking  the local triangles
	if (m_spSharedObject)
	{
		pkPicker->SetTarget(m_spSharedObject);
		NiPoint3 worldDirection = -NiPoint3::UNIT_Z;
		
		if (pkPicker->PickObjects(kWorldPosition, worldDirection))
		{
			// get pick results
			NiPick::Results& pickResults = pkPicker->GetResults();
			
			if (pickResults.GetAt(0) != NULL)
			{
				fHeight =
				    kWorldPosition.z - pickResults.GetAt(0)->GetDistance();
				return true;
			}
		}
	}


    // Check each of the children
    int iLoop;
    for (iLoop = 0; iLoop < 4; iLoop++)
    {
        if ((m_pkChildQuads[iLoop] != NULL) &&
            m_pkChildQuads[iLoop]->GetGroundElevation(pkPicker, 
                kWorldPosition, fHeight))
        {
            return true;
        }
    }

    return false;
}
//---------------------------------------------------------------------------
void NiQuadTreeGroundPartition::NiQuadTreeNode::GetNumQuadrents(int& iTotal, 
	int& iLeaf)
{
    // Recursively find the total number of quadrents. 

	iTotal++;

	// check for a leaf
    if ((m_pkChildQuads[0] == NULL) &&
        (m_pkChildQuads[1] == NULL) &&
        (m_pkChildQuads[2] == NULL) &&
        (m_pkChildQuads[3] == NULL))
    {
        iLeaf++;
    }


	// check all children
    int iLoop;
    for (iLoop = 0; iLoop < 4; iLoop++)
    {
        if (m_pkChildQuads[iLoop] != NULL)
        {
            m_pkChildQuads[iLoop]->GetNumQuadrents(iTotal, iLeaf);
        }
    }

}
//---------------------------------------------------------------------------




//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
NiQuadTreeGroundPartition::NiQuadTreeGroundPartition(
    unsigned int uiMaxTriangles, bool bShareTriangles)
{
	m_uiMaxTriangles = uiMaxTriangles;
    m_bShareTriangles = bShareTriangles;

		   // set up picker
	m_pkGroundPicker = NiNew NiPick();
    m_pkGroundPicker->SetPickType(NiPick::FIND_ALL);
    m_pkGroundPicker->SetSortType(NiPick::SORT);
    m_pkGroundPicker->SetIntersectType(NiPick::TRIANGLE_INTERSECT);
    m_pkGroundPicker->SetFrontOnly(true);
	m_pkGroundPicker->SetObserveAppCullFlag(true);
    m_pkGroundPicker->SetReturnTexture(false);
    m_pkGroundPicker->SetReturnNormal(true);
	m_pkGroundPicker->SetReturnSmoothNormal(false);
    m_pkGroundPicker->SetReturnColor(false);

}
//---------------------------------------------------------------------------
NiQuadTreeGroundPartition::~NiQuadTreeGroundPartition()
{
	m_spGround = NULL;

    delete m_pkTreeRoot;
    m_pkTreeRoot = NULL;

	delete m_pkGroundPicker;
	m_pkGroundPicker = NULL;

}
//---------------------------------------------------------------------------
bool NiQuadTreeGroundPartition::GetGroundElevation(NiPoint3& kWorldPosition, 
	float& fHeight)
{
    // Check to see if have build a tree
    if (m_pkTreeRoot != NULL)
    {
        return m_pkTreeRoot->GetGroundElevation(m_pkGroundPicker, 
            kWorldPosition, fHeight);
    }

	return false;
}
//---------------------------------------------------------------------------
void NiQuadTreeGroundPartition::BuildGround(NiNode* pkGround)
{
    // Update the ground
    pkGround->Update(0.0f);

    float fRadius = pkGround->GetWorldBound().GetRadius();
    m_kGroundCenter = pkGround->GetWorldBound().GetCenter();

    // Pre Determine the size of our arrays
    unsigned int uiVertices = RecursivelyBuildGroundPartitionSize( pkGround,
        m_kGroundCenter.x - fRadius, m_kGroundCenter.y - fRadius, 
        m_kGroundCenter.x + fRadius, m_kGroundCenter.y + fRadius);

    NiTPrimitiveArray<const NiPoint3*> pkTriangles(uiVertices);
    NiTPrimitiveArray<const NiPoint3*> pkNormals(uiVertices);

    // Build the triangle and normal list for given area
    RecursivelyBuildGroundPartition(pkGround, pkTriangles, pkNormals,
        m_kGroundCenter.x - fRadius, m_kGroundCenter.y - fRadius, 
        m_kGroundCenter.x + fRadius, m_kGroundCenter.y + fRadius);

    // Create the Ground Trishape or NiNode
    m_pkTreeRoot = NiNew NiQuadTreeNode(m_kGroundCenter, fRadius, 
        fRadius);

    // Build the Tree from the triangle Lists
    if (m_bShareTriangles)
    {
        m_pkTreeRoot->BuildQuadTreeNodeTriangleSharing(pkTriangles, 
            pkNormals, m_uiMaxTriangles, 20);
    }
    else
    {
        m_pkTreeRoot->BuildQuadTreeNodeNoTriangleSharing(pkTriangles, 
            pkNormals, m_uiMaxTriangles, 20);
    }

    m_spGround = m_pkTreeRoot->RecursiveBuildHeirarchy();

    NiMaterialProperty* pkMaterialProperty = NiNew NiMaterialProperty;
    pkMaterialProperty->SetDiffuseColor(NiColor(0.5, 0.5, 0.5));
    pkMaterialProperty->SetAmbientColor(NiColor(0.5, 0.5, 0.5));
    m_spGround->AttachProperty(pkMaterialProperty);

    NiVertexColorProperty* pkVertexColor = NiNew NiVertexColorProperty;
    pkVertexColor->SetSourceMode(NiVertexColorProperty::SOURCE_EMISSIVE);
    pkVertexColor->SetLightingMode(NiVertexColorProperty::LIGHTING_E_A_D);
    m_spGround->AttachProperty(pkVertexColor);


    m_spGround->UpdateProperties();
    m_spGround->UpdateEffects();


    //Find the total number of triangles. This will tell us how efficent
    //the division is
    char szBuffer[256];

    int iOriginalTriangles = pkTriangles.GetSize() / 3;
    int iTotalTriangles = FindTotalTriangles(m_spGround);
    float fPercentChange = 
        ((float)iTotalTriangles / (float)iOriginalTriangles) - 1.0f;

    NiSprintf(szBuffer, 256, 
        "Original Triangles = %d  "
        "NiNew Triangles = %d  Percent Change = %f\n",
        ///		pkTriangles.GetSize(), iTotalTriangles, fPercentChange);
        iOriginalTriangles, iTotalTriangles, fPercentChange);
    fprintf(stdout, szBuffer);

    int iTotal = 0;
    int iLeaf = 0;
    m_pkTreeRoot->GetNumQuadrents(iTotal, iLeaf);

    sprintf(szBuffer,
        "Total Quad Tree Nodes = %d   Total Leaf Quad Tree nodes = %d\n", 
        iTotal, iLeaf);
    fprintf(stdout, szBuffer);

}
//---------------------------------------------------------------------------
