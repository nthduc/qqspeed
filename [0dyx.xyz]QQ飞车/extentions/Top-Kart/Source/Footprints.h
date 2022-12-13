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

#ifndef FOOTPRINTS_H
#define FOOTPRINTS_H

#include <NiNode.h>
#include <NiTObjectList.h>

class DynamicQuads;

class Footprints : public NiMemObject
{
public:

    struct FootprintInfo
    {
        NiPoint3 m_akVerts[4];
        float m_fTime;
    };

    Footprints(NiAVObject* pkFootprint, DynamicQuads* pkDynaQuad);
    ~Footprints();
    void Update(float fTime);

    void SetFadeTime(float fTime);

    void AddFootprint(float fTime, const NiPoint3& kTranslate, 
        const NiMatrix3& kRotate);

    static void Shutdown();
    void Restart();

protected:

    float m_fFadeTimeFactor;

    NiAVObjectPtr m_spFootprint;

    DynamicQuads* m_pkDynaQuad;
    NiTObjectList<FootprintInfo> m_kFootprintInfoList;

};

#endif
