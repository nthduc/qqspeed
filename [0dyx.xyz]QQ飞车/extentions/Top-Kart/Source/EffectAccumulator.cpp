
#include "Top-KartPCH.h"

#include "EffectAccumulator.h"
//#include "LevelManager.h"
#include "NxSceneMgr.h"
#include "SceneLevel.h"

#ifdef _XENON
#include "NiXenonRenderer.h"
#include "NiXenonDirect3DTextureData.h"
#endif

//---------------------------------------------------------------------------
EffectAccumulator::EffectAccumulator()
{

}
//---------------------------------------------------------------------------
EffectAccumulator::~EffectAccumulator()
{

}
//---------------------------------------------------------------------------
void EffectAccumulator::RegisterObjectArray(NiVisibleArray& kArray)
{
    NiRenderer* pkRenderer = NiRenderer::GetRenderer();
//    NiShader*   pkIceShader = LevelManager::GetIceCubeShader();
//    NiShader*   pkHeatShader = LevelManager::GetHeatShimmerShader();

    const unsigned int uiQuantity = kArray.GetCount();
    for (unsigned int i = 0; i < uiQuantity; i++)
    {
        NiGeometry& kObject = kArray.GetAt(i);
        NiShader*   pkShader = kObject.GetShaderFromMaterial();

        if(  kObject.GetSortObject() )
        {
            m_kItems.AddTail(&kObject);
        }
        //else if(pkShader == pkIceShader ||
        //    pkShader == pkHeatShader )
        //{
        //    m_kItems.AddTail(&kObject);
        //}
        else
        {
            kObject.RenderImmediate(pkRenderer);
        }
    }
}
//---------------------------------------------------------------------------
void EffectAccumulator::FinishAccumulating(float fCopyThreshold)
{
    NiRenderer* pkRenderer = NiRenderer::GetRenderer();
    assert(pkRenderer);
    if (!pkRenderer)
        return;
    
    Ni2DBuffer* pkBackBuffer;
//    NiShader*   pkIceShader = LevelManager::GetIceCubeShader();
 //   NiShader*   pkHeatShader = LevelManager::GetHeatShimmerShader();
 //   NiRenderedTexturePtr spFrameTexture = LevelManager::Get()->
  //      GetFrameTexture();

#ifdef _XENON
    NiTexturePtr spTexture;
#endif

   //if (LevelManager::GetGameStatusManager()->GetBulletTime() ||
     //       LevelManager::GetGameStatusManager()->GetFadeOutBulletTime())
       ; //pkBackBuffer = LevelManager::Get()->GetBlurBuffer();
   // else
        pkBackBuffer = pkRenderer->GetDefaultBackBuffer();

    Sort();
    m_iCurrItem = m_iNumItems;

    bool bFirstCopy = false;
    const NiPoint3 kCameraLoc = NxSceneMgr::Get()->GetActiveSceneLevel()->GetCamera()->GetWorldLocation();
    float fLastCopyDist;

    NiGeometry* pkItem =  m_iCurrItem ? m_ppkItems[--m_iCurrItem] : 0;

    while (pkItem)
    {
        NiShader* pkShader = pkItem->GetShaderFromMaterial();
//        if(pkShader == pkIceShader ||
//            pkShader == pkHeatShader)
//        {
//            // Check to see if we need to perform a copy of the back buffer to
//            // our frame texture used for effects. If the current object is 
//            // close enough to last object that performed a copy then we can
//            // get away without needed to copy the back buffer again.
//
//            bool bNeedToCopy = false;
//            NiPoint3 kVect = pkItem->GetWorldTranslate() - kCameraLoc;
//            float fDist = kVect.Length();
//
//            if (!bFirstCopy)
//            {
//                bNeedToCopy = true;
//                bFirstCopy = true;
//                fLastCopyDist = fDist;
//            }
//            else
//            {
//                if (fDist > 2000.0f &&
//                    fLastCopyDist - fDist > fCopyThreshold)
//                {
//                    bNeedToCopy = true;
//                    fLastCopyDist = fDist;
//
//                }
//            }
//
//            if(bNeedToCopy)
//            {
//#ifdef _XENON
//            const NiRenderTargetGroup* pkRenderTargetGroup = 
//                pkRenderer->GetCurrentRenderTargetGroup();
//
//            // Force a resolve of the current render target group
//            pkRenderer->EndUsingRenderTargetGroup();
//
//
//            // Get the resolve destination from the current render target group
//            // and create a texture out of it so it can be used as an input for
//            // the desired effect.
//            NiXenon2DBufferData* pkBuffData = NiDynamicCast(
//                NiXenon2DBufferData, (NiXenon2DBufferData*)
//                pkRenderTargetGroup->GetBufferRendererData(0));
//
//            D3DBaseTexturePtr pkD3DBaseTexture = NULL;
//            unsigned int uiFace = 0;
//            pkBuffData->GetResolveDestination(pkD3DBaseTexture, uiFace);
//
//            spTexture = ((NiXenonRenderer*)pkRenderer)->
//                CreateNiTextureFromD3DTexture(pkD3DBaseTexture);
//
//            // Update the texturing property of the screenQuad to use the newly
//            // created texture. This is used to blit the contents of the back
//            // into EDRAM as it may have been clobbered in the forced resolve.
//            NiTexturingProperty* pkTexProp =
//                (NiTexturingProperty*)m_pkScreenQuad->
//                GetProperty(NiProperty::TEXTURING);
//            
//            pkTexProp->SetBaseTexture(spTexture);
//            m_pkScreenQuad->UpdateProperties();
//
//            // Now that we have the contents of the render target group in a 
//            // texture we render the screenquad with the current render target
//            // group.
//            pkRenderer->BeginUsingRenderTargetGroup(
//                (NiRenderTargetGroup*)pkRenderTargetGroup, 
//                NiRenderer::CLEAR_ALL);
//
//            pkRenderer->SetScreenSpaceCameraData();
//            m_pkScreenQuad->Draw(pkRenderer);
//     
//            NiCamera* pkCamera = LevelManager::GetCamera();
//
//
//            // Restore camera data to its state before we set it to use 
//            // screen space camera data. 
//            pkRenderer->SetCameraData(pkCamera->GetWorldTranslate(),
//                pkCamera->GetWorldDirection(), pkCamera->GetWorldUpVector(),
//                pkCamera->GetWorldRightVector(), pkCamera->GetViewFrustum(),
//                pkCamera->GetViewPort());
//
//
//            // Restore the depth buffer to the state it was at before we 
//            // rendered the effect.
//            NiResolvableDepthStencilBuffer* pkRDS = 
//                (NiResolvableDepthStencilBuffer*)pkRenderTargetGroup->
//                GetDepthStencilBuffer();
//            assert(pkRDS);
//            ((NiXenonRenderer*)pkRenderer)->RestoreDepthStencilBuffer(
//                pkRDS, 1.0f);
//#endif
//
//#ifdef WIN32
//                // Copy the current contents of the back buffer 
//                // to the frame buffer
//                // texture to be used by the heat and ice shaders
//                pkRenderer->FastCopy(pkBackBuffer,
//                    spFrameTexture->GetBuffer());            
//#endif
//            }
//
//#ifdef _XENON            
//            // Update texturing property of the item being rendered with the 
//            // effect so that it references the texture containing a copy of
//            // the back buffer.
//            NiTexturingProperty* pkTexProp =
//                (NiTexturingProperty*)pkItem->
//                GetProperty(NiProperty::TEXTURING);
//
//            NiTexturingProperty::ShaderMap* pkShaderMap = 
//                pkTexProp->GetShaderMap(0);
//
//            pkShaderMap->SetTexture(spTexture);
//            pkItem->UpdateProperties();
//#endif
//        }

        pkItem->RenderImmediate(pkRenderer);
        pkItem = m_iCurrItem ? m_ppkItems[--m_iCurrItem] : 0;
    }

    // DO NOT delete the NiGeometries!
    while (m_kItems.GetSize())
        m_kItems.RemoveHead();

    NiAccumulator::FinishAccumulating();
}
//---------------------------------------------------------------------------
