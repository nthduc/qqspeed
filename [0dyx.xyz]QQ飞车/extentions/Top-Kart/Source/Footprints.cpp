

#include "LenguinsPCH.h"

#include "Footprints.h"
#include <NiZBufferProperty.h>
#include <NiMaterialProperty.h>
#include <NiVertexColorProperty.h>
#include <NiFloatExtraData.h>

#include "DynamicQuads.h"
#include <NiCamera.h>

NiAllocatorDeclareStatics(Footprints::FootprintInfo, 1024)


//---------------------------------------------------------------------------
Footprints::Footprints(NiAVObject* pkFootprint, DynamicQuads* pkDynaQuad)
    : m_pkDynaQuad(pkDynaQuad), m_fFadeTimeFactor(20.0f), 
    m_spFootprint(pkFootprint)
{
    assert (pkDynaQuad);
}
//---------------------------------------------------------------------------
void Footprints::Shutdown()
{
    NiAllocatorShutdown(FootprintInfo);
}
//---------------------------------------------------------------------------
void Footprints::AddFootprint(float fTime, const NiPoint3& kTranslate, 
    const NiMatrix3& kRotate)
{
    NiTransform kTransform;
    kTransform.m_fScale = 1.0f;
    kTransform.m_Rotate = kRotate;
    kTransform.m_Translate = kTranslate;

    FootprintInfo* pkInfo = m_kFootprintInfoList.AddNewTail();

    pkInfo->m_fTime = fTime;
    
    NiProcessorSpecificCode::TransformPoints(4, (const float*)
        NiSmartPointerCast(NiGeometry, m_spFootprint)->GetVertices(), 
        (float *)pkInfo->m_akVerts, &kTransform);
}
//---------------------------------------------------------------------------
Footprints::~Footprints()
{
    m_spFootprint = 0;
}
//---------------------------------------------------------------------------
void Footprints::SetFadeTime(float fTime)
{
    m_fFadeTimeFactor = 1.0f / fTime;
}
//---------------------------------------------------------------------------
void Footprints::Update(float fTime)
{
    // Fade out footprints, remove any that are completely faded
    NiTListIterator kPos = m_kFootprintInfoList.GetHeadPos();

    while (kPos)
    {
        FootprintInfo* pkInfo = m_kFootprintInfoList.GetNext(kPos);

        // Fade out 
        float fAlpha = 1.0f - ((fTime - pkInfo->m_fTime) *
            m_fFadeTimeFactor);

        if (fAlpha <= 0.0f)
        {
            assert(pkInfo == m_kFootprintInfoList.GetHead());

            m_kFootprintInfoList.RemoveHead();
        }
        else
        {
            DynamicQuads::QuadInfo kQuadInfo;
            kQuadInfo.m_fAlpha = fAlpha;
            kQuadInfo.m_pkVerts = pkInfo->m_akVerts;

            m_pkDynaQuad->AddQuad(kQuadInfo);
        }
    }

}
//---------------------------------------------------------------------------
void Footprints::Restart()
{
    // Clear out all the active foot prints
    m_kFootprintInfoList.RemoveAll();
    m_pkDynaQuad->ResetQuads();
}
//---------------------------------------------------------------------------