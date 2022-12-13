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

#ifndef EFFECTACCUMULATOR_H
#define EFFECTACCUMULATOR_H

#include <NiAlphaAccumulator.h>
#include <NiCamera.h>
#include <NiGeometry.h>

class NiScreenElements;

class NIMAIN_ENTRY EffectAccumulator : public NiAlphaAccumulator
{
public:
    // creation and destruction
    EffectAccumulator();
    virtual ~EffectAccumulator();

    // *** begin Emergent internal use only ***

    virtual void RegisterObjectArray (NiVisibleArray& kArray);

    // *** end Emergent internal use only ***

    virtual void FinishAccumulating(float fCopyThreshold);

#ifdef _XENON

private:
    NiScreenElements* m_pkScreenQuad;

#endif
};

typedef NiPointer<EffectAccumulator> EffectAccumulatorPtr;

#endif // EFFECTACCUMULATOR_H

