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

//---------------------------------------------------------------------------
inline NiAVObjectPtr NiGroundPartition::GetGround()
{
	return m_spGround;
}
//---------------------------------------------------------------------------
inline bool NiGroundPartition::BoundingSphereInPartition(
    const NiBound& kBound, float fMinX, float fMinY, float fMaxX, float fMaxY)
{
    // Check if a the bounding sphere is contained in the XY region. 
    float fRadius = kBound.GetRadius();
    NiPoint3 fCenter = kBound.GetCenter();

    // Check for bounds intersecting the partitions
    return( (fCenter.x >= fMinX - fRadius) &&
        (fCenter.x <= fMaxX + fRadius) &&
        (fCenter.y >= fMinY - fRadius) &&
        (fCenter.y <= fMaxY + fRadius));
}
//---------------------------------------------------------------------------
inline bool NiGroundPartition::TriangleInPartition(const NiPoint3* pkP0, 
    const NiPoint3* pkP1, const NiPoint3* pkP2, float fMinX, float fMinY, 
    float fMaxX, float fMaxY)
{
    // Check if the triangle is contained in this XY region.  This code only 
    // checks the vertices it does not check for a triangle that contains the 
    // region or the region clips on of the triangle edges.
    if (((pkP0->x >= fMinX) && (pkP0->x <= fMaxX) && 
        (pkP0->y >= fMinY) && (pkP0->y <= fMaxY)) ||
        ((pkP1->x >= fMinX) && (pkP1->x <= fMaxX) && 
        (pkP1->y >= fMinY) && (pkP1->y <= fMaxY)) || 
        ((pkP2->x >= fMinX) && (pkP2->x <= fMaxX) && 
        (pkP2->y >= fMinY) && (pkP2->y <= fMaxY)))
        return true;

    // Verify all triangle vertices are outside the region edge
    if ((pkP0->x < fMinX) && (pkP1->x < fMinX) && (pkP2->x < fMinX))
        return false;

    if ((pkP0->x > fMaxX) && (pkP1->x > fMaxX) && (pkP2->x > fMaxX))
        return false;

    if ((pkP0->y < fMinY) && (pkP1->y < fMinY) && (pkP2->y < fMinY))
        return false;

    if ((pkP0->y > fMaxY) && (pkP1->y > fMaxY) && (pkP2->y > fMaxY))
        return false;

    // Check if any of the region vertices are within the triangle
    bool bSide0;
    bool bSide1;
    bool bSide2;

    bSide0 = PickSide(fMinX, fMinY, pkP0, pkP1);
    bSide1 = PickSide(fMinX, fMinY, pkP1, pkP2);
    bSide2 = PickSide(fMinX, fMinY, pkP2, pkP0);

    if ((bSide0 == bSide1) && (bSide1 == bSide2))
        return true;

    bSide0 = PickSide(fMinX, fMaxY, pkP0, pkP1); 
    bSide1 = PickSide(fMinX, fMaxY, pkP1, pkP2);
    bSide2 = PickSide(fMinX, fMaxY, pkP2, pkP0);

    if ((bSide0 == bSide1) && (bSide1 == bSide2))
        return true;

    bSide0 = PickSide(fMaxX, fMaxY, pkP0, pkP1);
    bSide1 = PickSide(fMaxX, fMaxY, pkP1, pkP2);
    bSide2 = PickSide(fMaxX, fMaxY, pkP2, pkP0);

    if ((bSide0 == bSide1) && (bSide1 == bSide2))
        return true;

    bSide0 = PickSide(fMaxX, fMinY, pkP0, pkP1);
    bSide1 = PickSide(fMaxX, fMinY, pkP1, pkP2);
    bSide2 = PickSide(fMaxX, fMinY, pkP2, pkP0);

    if ((bSide0 == bSide1) && (bSide1 == bSide2))
        return true;

    return false;
}
//---------------------------------------------------------------------------
inline bool NiGroundPartition::PickSide(float fRX, float fRY,
    const NiPoint3* pkP0, const NiPoint3* pkP1)
{
    NiPoint3 kEdge;
    NiPoint3 kRegion;
    NiPoint3 kCross;

    kEdge = *pkP1 - *pkP0;
    kEdge.z = 0.0f;

    // Create the region vertex vector
    kRegion.x = fRX - pkP0->x;
    kRegion.y = fRY - pkP0->y;
    kRegion.z = 0.0f;

    kCross = kEdge.Cross(kRegion);
    return kCross.z > 0.0f;
}
//---------------------------------------------------------------------------
inline bool NiGroundPartition::TriangleInPartition(const NiPoint3* pkP1, 
    const NiPoint3* pkP2, const NiPoint3* pkP3, NiPoint3& kCenter, 
    float fXWidth, float fYWidth)
{
    // Check if the triangle is contained in this XY region.  This code only 
    // checks the vertices it does not check for a triangle that contains the 
    // region or the region clips on of the triangle edges.

    float fMinX = kCenter.x - fXWidth;
    float fMaxX = kCenter.x + fXWidth;
    float fMinY = kCenter.y - fYWidth;
    float fMaxY = kCenter.y + fYWidth;

    return TriangleInPartition(pkP1, pkP2, pkP3, fMinX, fMinY, fMaxX, fMaxY);
}
//---------------------------------------------------------------------------
