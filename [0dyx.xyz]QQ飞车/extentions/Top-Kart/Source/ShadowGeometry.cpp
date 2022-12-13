

#include "Top-KartPCH.h"

#include "ShadowGeometry.h"

//#include "LevelManager.h"

#include <NiAlphaProperty.h>
#include <NiCamera.h>
#include <NiMaterialProperty.h>
#include <NiRenderedTexture.h>
#include <NiShadeProperty.h>
#include <NiSourceTexture.h>
#include <NiTriShape.h>
#include <NiTriShapeDynamicData.h>
#include <NiTriStrips.h>
#include <NiVertexColorProperty.h>
#include <NiZBufferProperty.h>
#include <NiD3DShaderFactory.h>
#include <NiD3DDefaultShader.h>
#include <NiDX9Renderer.h>

//---------------------------------------------------------------------------
ShadowGeometry::ShadowGeometry(unsigned int uiMaxTriangleCount) :
    m_kCastingObjects(4), m_bForceRender(false),
    m_kVisible(8, 8), m_kCuller(&m_kVisible)
{
    // the direction of the infinite light casting the shadow.  
	m_kLightDir = NiPoint3(-0.0f, -0.0f, -1.0f);
	m_kLightDir.Unitize();

    // the camera used to project the shadow caster to create the shadow mask
    // this camera is a distant perspective camera with a narrow FOV, to 
    // approximate a parallel camera.
    m_spCamera = NiNew NiCamera;
    m_spCamera->SetTranslate(0.0f, 0.0f, 1000.0f);

    // Need to generate an orthonormal frame, with the X axis mapping to the
    // light direction.  The other dimensions do not matter, so long as they
    // define a right-handed orthonormal frame
    NiPoint3 kYVec = -m_kLightDir.Perpendicular();
    NiPoint3 kZVec = m_kLightDir.UnitCross(kYVec);

    // Rotate the camera based on the orthonormal frame
    NiMatrix3 kRotation(m_kLightDir, kYVec, kZVec);
    m_spCamera->SetRotate(kRotation);
    m_spCamera->Update(0.0f);

    NiFrustum kFrust = m_spCamera->GetViewFrustum();
    kFrust.m_fFar  = 1000.0f;
    kFrust.m_fNear =   1.0f;
    m_spCamera->SetViewFrustum(kFrust);

    // The following (master) properties are used to batch render the shadow
    // casting objects.  These properties ensure that the shadow casters 
    // appear opaque and flat, the color of the desired shadow.  If these 
    // were not used, the shadow could contain surface colors of the casters'
    // textures, which would look incorrect.
    m_spPropertyState = NiNew NiPropertyState;
    NiVertexColorProperty* pkVC = NiNew NiVertexColorProperty;
    NiMaterialProperty* pkMat = NiNew NiMaterialProperty;
    NiTexturingProperty* pkTex = NiNew NiTexturingProperty;
    NiShadeProperty* pkShade = NiNew NiShadeProperty;

    pkVC->SetSourceMode(NiVertexColorProperty::SOURCE_IGNORE);
    pkVC->SetLightingMode(NiVertexColorProperty::LIGHTING_E);
    m_spPropertyState->SetProperty(pkVC);

    pkMat->SetEmittance(NiColor(0.5f, 0.55f, 0.9f));
    pkMat->SetAlpha(0.3f);
    m_spPropertyState->SetProperty(pkMat);

    m_spPropertyState->SetProperty(pkTex);

    pkShade->SetSmooth(false);
    m_spPropertyState->SetProperty(pkShade);

    if (uiMaxTriangleCount == 0)
        m_uiMaxTriangleCount = TRIANGLE_COUNT;
    else
        m_uiMaxTriangleCount = uiMaxTriangleCount;
    m_uiMaxVertexCount = 3 * m_uiMaxTriangleCount;

    // Create the shadow geometry - this is a dynamic trishape that will be
    // refilled each frame to include ground (recipient) triangles that fall
    // within the shadow frustum.  It must be marked as dynamic, as both the
    // geometry and the vertex/triangle counts will change each frame
    NiPoint3* pkVertex = NiNew NiPoint3[m_uiMaxVertexCount];
    NiPoint2* pkTexC = NiNew NiPoint2[m_uiMaxVertexCount];
    assert(pkVertex);
    assert(pkTexC);
    unsigned short* pusConnect = (unsigned short*)NiMalloc(
        sizeof(unsigned short) * (m_uiMaxTriangleCount * 3));
    assert(pusConnect);
    NiTriShapeDynamicData* pkTriData = NiNew NiTriShapeDynamicData(
        m_uiMaxVertexCount, pkVertex, 0, 0, pkTexC, 1, 
        NiGeometryData::NBT_METHOD_NONE, m_uiMaxTriangleCount, pusConnect);
    m_spShadowGeometry = NiNew NiTriShape(pkTriData);

	m_spShadowGeometry->CreateColors(true);
      
    assert(pkTriData);
    assert(m_spShadowGeometry);
    m_spShadowGeometry->SetName("ShadowGeometry");
    m_spShadowGeometry->SetActiveVertexCount(0);
    m_spShadowGeometry->SetActiveTriangleCount(0);

    // The shadow is alpha blended, so it must not write the ZBuffer.
    NiAlphaProperty* pAlpha = NiNew NiAlphaProperty;
    NiZBufferProperty* pkZ = NiNew NiZBufferProperty;
    assert(pkZ);
    pkZ->SetZBufferTest(true);
    pkZ->SetZBufferWrite(false);
    m_spShadowGeometry->AttachProperty(pkZ);

    // The shadow is a darkmap, so is multiplies the framebuffer color
    pAlpha->SetAlphaBlending(true);
	pAlpha->SetSrcBlendMode(NiAlphaProperty::ALPHA_ZERO);
	pAlpha->SetDestBlendMode(NiAlphaProperty::ALPHA_INVSRCALPHA);//ALPHA_SRCCOLOR);
    m_spShadowGeometry->AttachProperty(pAlpha);

	m_spDefaultShader = NiNew NiD3DDefaultShader;
	m_spDefaultShader->Initialize();
}
//----------------------------------------------------------------------------
ShadowGeometry* ShadowGeometry::Create(unsigned int uiDetail, 
    unsigned int uiMaxTriangleCount, const char* pcFileName)
{
    ShadowGeometry* pkThis = NiNew ShadowGeometry(uiMaxTriangleCount);
	unsigned int uiTexWidth;
	NiRenderer* pkRenderer = 0;

    if (!pcFileName)
    {
        // No texture file specified, so we must create a rendered texture
        // that will be update each frame
		pkRenderer = NiRenderer::GetRenderer();
		assert(pkRenderer);

        // We match the texture to the depth of the backbuffer for optimal
        // performance
        NiTexture::FormatPrefs kPrefs;
        const NiRenderTargetGroup* pkRTGroup = 
            NiRenderer::GetRenderer()->GetDefaultRenderTargetGroup();
        assert(pkRTGroup);
        const NiPixelFormat* pkPixelFormat = pkRTGroup->GetPixelFormat(0);
        if (pkPixelFormat->GetBitsPerPixel() == 16)
            kPrefs.m_ePixelLayout = NiTexture::FormatPrefs::HIGH_COLOR_16;
        else
            kPrefs.m_ePixelLayout = NiTexture::FormatPrefs::TRUE_COLOR_32;

        uiTexWidth = 1 << uiDetail;
        NiRenderedTexture* pkRenderedTexture = NiRenderedTexture::Create( 
            uiTexWidth, uiTexWidth, pkRenderer, kPrefs);
        pkThis->m_spTexture = pkRenderedTexture;
        pkThis->m_spRenderTargetGroup = NiRenderTargetGroup::Create(
            pkRenderedTexture->GetBuffer(), pkRenderer, true);
        pkThis->m_bRendered = true;
    }
    else
    {
        // A shadow texture filename was specified - just load the shadow
        // texture and use it each frame.  Note that since there is no way of
        // specifying a specific orientation of the shadow texture as it
        // moves to match the caster, the shadow texture should be 
        // rotationally symmetric.
        NiTexture::FormatPrefs kPrefs;
        kPrefs.m_eMipMapped = NiTexture::FormatPrefs::NO;
        pkThis->m_spTexture = NiSourceTexture::Create(pcFileName, kPrefs);
        assert(pkThis->m_spTexture);
        pkThis->m_spRenderTargetGroup = NULL;
        pkThis->m_bRendered = false;
    }

    if (!pkThis->m_spTexture)
    {
        NiOutputDebugString("Cannot create shadow texture\n");
        NiDelete pkThis;
        return NULL;
    }

    // Since we may generate texture coordinates outside of the range [0,1],
    // we must use clamping to avoid strange wrapping artifacts.
    NiTexturingProperty::Map* pkMap = NiNew NiTexturingProperty::Map(
		pkThis->m_spTexture, 0, NiTexturingProperty::CLAMP_S_CLAMP_T,
        NiTexturingProperty::FILTER_BILERP);
    NiTexturingProperty* pkTex = NiNew NiTexturingProperty();
    assert(pkMap);
    assert(pkTex);
    pkTex->SetBaseMap(pkMap);
    pkTex->SetApplyMode(NiTexturingProperty::APPLY_REPLACE);

/*
    NiShaderPtr spCopyShader = NiShaderFactory::GetInstance()->
        RetrieveShader("ShadowBlur", 
        NiShader::DEFAULT_IMPLEMENTATION, true);

    NiSingleShaderMaterial* pkCopyMat = 
        NiSingleShaderMaterial::Create(spCopyShader);

    pkThis->m_spShadowGeometry->ApplyAndSetActiveMaterial(pkCopyMat);

    unsigned int ui;
    unsigned int uiTunedWidth = 512;
    unsigned int uiTunedHeight = 512;

    // Compute blur texel kernels.
    float* pfBlurTexelKernelX = NiAlloc(float, BLUR_KERNEL_SIZE * 2);
    float* pfBlurTexelKernelY = NiAlloc(float, BLUR_KERNEL_SIZE * 2);
    for (ui = 0; ui < BLUR_KERNEL_SIZE; ui++)
    {
        pfBlurTexelKernelX[ui * 2] = g_afBlurPixelKernel[ui] / uiTunedWidth;
        pfBlurTexelKernelX[ui * 2 + 1] = 0;

        pfBlurTexelKernelY[ui * 2] = 0;
        pfBlurTexelKernelY[ui * 2 + 1] = 
            g_afBlurPixelKernel[ui] / uiTunedHeight;
    }
    NiD3DShaderFactory::UpdateGlobalShaderConstant(
        "g_vBlurTexelKernelX", BLUR_KERNEL_SIZE * sizeof(float) * 2, 
        pfBlurTexelKernelX);
    NiD3DShaderFactory::UpdateGlobalShaderConstant(
        "g_vBlurTexelKernelY", BLUR_KERNEL_SIZE * sizeof(float) * 2, 
        pfBlurTexelKernelY);
    NiFree(pfBlurTexelKernelX);
    NiFree(pfBlurTexelKernelY);
*/

    pkThis->m_spShadowGeometry->AttachProperty(pkTex);
    pkThis->m_spShadowGeometry->SetSortObject(false);
    pkThis->m_spShadowGeometry->UpdateProperties();

    // Clear shadow texture
    if (pkThis->m_bRendered)
    {
        pkRenderer->BeginOffScreenFrame();
        pkRenderer->BeginUsingRenderTargetGroup(pkThis->m_spRenderTargetGroup,
            NiRenderer::CLEAR_ALL);
        pkRenderer->EndUsingRenderTargetGroup();
        pkRenderer->EndOffScreenFrame();

	    // Set camera port so that outer texels are never drawn to.
	    float fFraction = 2.0f / ((float)uiTexWidth);
		NiRect<float> kPort(fFraction, 1.0f - fFraction, 
			1.0f - fFraction, fFraction);

		pkThis->m_spCamera->SetViewPort(kPort);
    }

    return pkThis;
}
//----------------------------------------------------------------------------
ShadowGeometry::~ShadowGeometry()
{
	m_spCaster = 0;
	m_spCamera = 0;
	m_spTexture = 0;
	m_spRenderTargetGroup = 0;
	m_spGround = 0;
	m_spShadowGeometry = 0;
	m_spPropertyState = 0;
	m_spDefaultShader = 0;
} 
//---------------------------------------------------------------------------
void ShadowGeometry::SetCaster(NiAVObject* pkCaster, bool clearList)
{
    m_spCaster = pkCaster;

    // Traverse the new caster to find all of the leaf geometry objects.
    // Note that this will not clear out the list of caster geometry, and
    // will thus _add_ caster geometry to the shadow system.  However, only
    // the most recently-set caster will affect the centerpoint and bounding
    // sphere of the shadow
	if (m_bRendered)
    {
        if (clearList)
            m_kCastingObjects.RemoveAll();
		RecursiveStoreCasterObjects(pkCaster);
    }
}
//---------------------------------------------------------------------------
void ShadowGeometry::RecursiveStoreCasterObjects(NiAVObject* pkObject)
{
	//	增加人:	wincoqin
	//  时间:	2007/03/07
	//  目的:	被裁减掉的就不要参与影子的绘制了	
	if (pkObject->GetAppCulled() || pkObject->GetName().Equals("center")) 
	{
		return;
	}
    // Find all leaf geometry objects and add them to the caster geometry 
    // list, to be rendered as batch
    if (NiIsKindOf(NiGeometry, pkObject))
    {
        NiGeometry* pkGeometry = (NiGeometry*)pkObject;
        if (pkGeometry->GetSkinInstance() == NULL)
            m_kCastingObjects.Add(pkObject);
        else
            m_kSkinnedCastingObjects.Add(pkObject);
    }
    else if (NiIsKindOf(NiNode, pkObject))
    {
        NiNode* pkNode = (NiNode*)pkObject;
        for (unsigned int i = 0; i < pkNode->GetArrayCount(); i++)
        {
            NiAVObject* pkChild = pkNode->GetAt(i);
            if (pkChild)
                RecursiveStoreCasterObjects(pkChild);
        }
    }
}
//----------------------------------------------------------------------------
void ShadowGeometry::Click(float fTime, NiCamera* pkSceneCamera, 
    bool bVisible, bool bUpdateImage)
{
    assert(m_spCaster && m_spTexture);


    // If the visible flag is false, clear out the shadow geometry and skip
    // rendering the shadow, as it won't matter to the final, rendered image.
    if (!bVisible)
    {
        m_spShadowGeometry->SetActiveVertexCount(0);
        m_spShadowGeometry->SetActiveTriangleCount(0);
        return;
    }

    // Cull shadow geometry against the scene to avoid updating
    if (m_bForceRender)
    {
        m_bForceRender = false;
    }
    else if (pkSceneCamera && m_spShadowGeometry->GetActiveVertexCount())
    {
        m_kFrPlanes.Set(*pkSceneCamera);

        for (unsigned int i = NiFrustumPlanes::LEFT_PLANE; 
            i < NiFrustumPlanes::MAX_PLANES; i++)
        {
            const NiPlane& kPlane = m_kFrPlanes.GetPlane(i);
            int iSide = m_spShadowGeometry->GetWorldBound().WhichSide(kPlane);

            if (iSide == NiPlane::NEGATIVE_SIDE) 
            {
                // Update bounds regardless to ensure proper culling
                UpdateShadowGeometryBound();

                return;
            }
        }
    }

    // move the shadow camera (and thus the apparent light)
    UpdateShadowCamera(fTime);


    if (m_bRendered && bUpdateImage)
    {
        NiRenderer* pkRenderer = NiRenderer::GetRenderer();
        assert(pkRenderer);

        // update the background color to the renderer -- note it clears alpha
        // channel to completely transparent value to get an alpha rendered
        // texture
        pkRenderer->SetBackgroundColor(NiColor::WHITE);

        pkRenderer->BeginUsingRenderTargetGroup(m_spRenderTargetGroup,
            NiRenderer::CLEAR_ALL);

        // renderer camera port settings
        pkRenderer->SetCameraData(m_spCamera);

        NiAccumulatorPtr spSorter = pkRenderer->GetSorter();
        pkRenderer->SetSorter(NULL);

        // Render all leaf shadow caster geometry in a batch, replacing their
        // built-in renderstates with the single shadow state.  This will 
        // cause all of the shadow casters to be rendered as dark gray, making
        // them look like shadows.
        pkRenderer->BeginBatch(m_spPropertyState, NULL);

        for (unsigned int i = 0; i < m_kCastingObjects.GetSize(); i++)
        {
            NiGeometry* pkGeom = NiSmartPointerCast(NiGeometry,
                m_kCastingObjects.GetAt(i));
			NiShaderPtr spOldShader = pkGeom->GetShader();// Save old shader
			pkGeom->SetShader(m_spDefaultShader);

            if (NiIsKindOf(NiTriStrips, pkGeom))
            {
                pkRenderer->BatchRenderStrips((NiTriStrips*)pkGeom);
            }
            else if (NiIsKindOf(NiTriShape, pkGeom))
            {
                pkRenderer->BatchRenderShape((NiTriShape*)pkGeom);
            }
			pkGeom->SetShader(spOldShader);// Recover Shader
        }

        pkRenderer->EndBatch();

        pkRenderer->BeginBatch(m_spPropertyState, NULL);

        for (unsigned int i = 0; i < m_kSkinnedCastingObjects.GetSize(); i++)
        {
            NiGeometry* pkGeom = NiSmartPointerCast(NiGeometry,
                m_kSkinnedCastingObjects.GetAt(i));

			NiShaderPtr spOldShader = pkGeom->GetShader();// Save old shader
			pkGeom->SetShader(m_spDefaultShader);

            if (NiIsKindOf(NiTriStrips, pkGeom))
            {
                pkRenderer->BatchRenderStrips((NiTriStrips*)pkGeom);
            }
            else if (NiIsKindOf(NiTriShape, pkGeom))
            {
                pkRenderer->BatchRenderShape((NiTriShape*)pkGeom);
            }

			pkGeom->SetShader(spOldShader);// Recover Shader
        }

        pkRenderer->EndBatch();

        pkRenderer->SetSorter(spSorter);

        pkRenderer->EndUsingRenderTargetGroup();

    }


    // Determine which triangles fall inside of the shadow frustum.
    UpdateShadowGeometry();
}
//---------------------------------------------------------------------------
void ShadowGeometry::UpdateShadowCamera(float fTime)
{
    // this function moves the shadow camera so that it appears to view the
    // target (caster) from infinity, facing a fixed direction.  This is done
    // by  moving the camera so the the desired fixed direction vector is
    // always coincident with the line through the caster's bounding volume
    // center and the camera location
    if (!m_spCamera)
        return;

    // get the "look at" point
    NiPoint3 kTarget = m_spCaster->GetWorldBound().GetCenter();

//	LogText("pos : %f, %f, %f\n", kTarget.x, kTarget.y, kTarget.z);

    // translate the camera to a distant point such that the camera is looking
    // directly at the target point
    NiPoint3 kTranslate = kTarget - (m_kLightDir * 1000.0f);
    m_spCamera->SetTranslate(kTranslate);// temp comment
	NiPoint3 kYVec = -m_kLightDir.Perpendicular();
	NiPoint3 kZVec = m_kLightDir.UnitCross(kYVec);

	// Rotate the camera based on the orthonormal frame
	NiMatrix3 kRotation(m_kLightDir, kYVec, kZVec);
	m_spCamera->SetRotate(kRotation);

    // set the field of view of the camera to enclose the bounding sphere of 
    // the caster object.
    float fRadius = m_spCaster->GetWorldBound().GetRadius();

    float fFOV =  fRadius / 
        (kTarget - kTranslate).Length() * 0.95f;

    NiFrustum kFrust = m_spCamera->GetViewFrustum();
    kFrust.m_fLeft = -fFOV;
    kFrust.m_fRight = fFOV;
    kFrust.m_fTop = fFOV;
    kFrust.m_fBottom = -fFOV;
    m_spCamera->SetViewFrustum(kFrust);

    m_spCamera->Update(fTime);
}
//---------------------------------------------------------------------------
void ShadowGeometry::UpdateShadowGeometry()
{
    m_spShadowGeometry->SetActiveVertexCount(0);
    m_spShadowGeometry->SetActiveTriangleCount(0);

    m_kCenterP = m_spCamera->GetWorldLocation();
   
	/*float fHeight;
    if (!LevelManager::GetTerrainManager()->
        GetHeight(m_kCenterP.x, m_kCenterP.y, fHeight))
    {
        // No terrain was found by pick operation.
        assert("Error: no pick intersection occurred");
    }*/


    float fRadius = m_spCaster->GetWorldBound().GetRadius();
    m_fOOWidth = 1.0f / (fRadius * 2.0f);  


    TraverseGroundGeometry(m_spGround);

    m_spShadowGeometry->GetModelData()->MarkAsChanged(
       NiGeometryData::VERTEX_MASK | NiGeometryData::TEXTURE_MASK | 
       NiTriBasedGeomData::TRIANGLE_INDEX_MASK |
       NiTriBasedGeomData::TRIANGLE_COUNT_MASK);

    UpdateShadowGeometryBound();
}
//---------------------------------------------------------------------------
void ShadowGeometry::UpdateShadowGeometryBound()
{
    // Manually set the bounding sphere of the shadow geometry to ensure
    // it is not culled by the engine
    float fRadius = m_spCaster->GetWorldBound().GetRadius();

    NiBound kSphere;
    kSphere.SetCenterAndRadius(m_spCaster->GetWorldBound().GetCenter(),
        fRadius * 1.05f);
    m_spShadowGeometry->SetModelBound(kSphere);

    m_spShadowGeometry->Update(0.0f);
}
//---------------------------------------------------------------------------
void ShadowGeometry::TraverseGroundGeometry(NiAVObject* pkObject)
{
    m_kVisible.RemoveAll();
    m_kCuller.Process(m_spCamera, pkObject, 0);
        
    const unsigned int uiQuantity = m_kVisible.GetCount();
    for (unsigned int i = 0; i < uiQuantity; i++)
    {
        NiGeometry& kGeom = m_kVisible.GetAt(i);
        AddToShadowGeometry(NiDynamicCast(NiTriBasedGeom, &kGeom));
    }
}
//---------------------------------------------------------------------------
void ShadowGeometry::AddToShadowGeometry(NiTriBasedGeom* pkTriGeom)
{
    assert(pkTriGeom);
   
    // This code assumes that model space vertices ARE in world space
    NiPoint3* pkWorldVerts = pkTriGeom->GetVertices();
    assert(pkWorldVerts);
    NiTriBasedGeomData* pkTriData = 
        (NiTriBasedGeomData*)pkTriGeom->GetModelData();
    assert(pkTriData);

    unsigned short s0;
    unsigned short s1;
    unsigned short s2;
    NiPoint3* pkP0;
    NiPoint3* pkP1;
    NiPoint3* pkP2;

    unsigned short usTris = pkTriData->GetTriangleCount();
    for (unsigned short i = 0; i < usTris; i++)
    {
        // Clip each world triangle to the camera's frustum
        pkTriData->GetTriangleIndices(i, s0, s1, s2);
        pkP0 = &pkWorldVerts[s0];
        pkP1 = &pkWorldVerts[s1];
        pkP2 = &pkWorldVerts[s2];

		CullTriAgainstCameraFrustum(*pkP0, *pkP1, *pkP2);
    }
}
//---------------------------------------------------------------------------
bool ShadowGeometry::GenerateCameraRay(unsigned int uiIndex, NiPoint3& kPt, 
    NiPoint3& kDir)
{
    const NiFrustum& kFrust = m_spCamera->GetViewFrustum();
    float fVx = (uiIndex & 0x1) ? kFrust.m_fRight :  kFrust.m_fLeft;
    float fVy = (uiIndex & 0x2) ? kFrust.m_fTop : kFrust.m_fBottom;

    // convert world view plane coordinates to ray with kDir and kOrigin
    // kDir: camera world location to view plane coordinate
    // kOrigin: camera world location
    m_spCamera->ViewPointToRay(fVx, fVy, kPt, kDir);
    return true;
}
//---------------------------------------------------------------------------
void ShadowGeometry::CullTriAgainstCameraFrustum(NiPoint3& kV0, 
    NiPoint3& kV1, NiPoint3& kV2)
{

    // This method does no clipping, but "extends" the reliance on 
    // CLAMP_S_CLAMP_T texture wrapping modes
    
    // Cull triangle vertices against camera planes.  If all three vertices
    // fall on the outside of any single plane, cull.  Skip the near and far
    // planes (indices i = 0,1).
    unsigned int i = NiFrustumPlanes::LEFT_PLANE;
    for (; i < NiFrustumPlanes::MAX_PLANES; i++)
    {
        const NiPlane& kPlane = m_kCuller.GetFrustumPlanes().GetPlane(i);

        // vert 0 in?
        if (kPlane.Distance(kV0) >= 0.0f) 
            continue;

        // vert 1 in?
        if (kPlane.Distance(kV1) >= 0.0f) 
            continue;

        // vert 2 in?
        if (kPlane.Distance(kV2) >= 0.0f) 
            continue;

        // all verts outside of a single plane - culled
        return;
    }

    NiPlane kTriPlane(kV0, kV1, kV2);
    const NiPoint3& kNorm = kTriPlane.m_kNormal;

    // we know that the ray center is the camera location
    NiPoint3 kCamRayPt = m_spCamera->GetWorldLocation();
    NiPoint3 akCamRayDirs[4];
    NiPoint3 akCamRayIntersect[4];

    // No intersection if the point is behind the triangle
    const float fDist = kTriPlane.Distance(kCamRayPt);
    if (fDist < 0.0f)
        return;

    NiPoint3 kE;

    // Next, compute the intersection points of the frustum corners with the
    // plane of the triangle.  There will be four of them if the frustum
    // goes through the triangle.  If not, there may be far fewer.  However,
    // for now, if any view frustum edge doesn't intersect the plane of the
    // triangle, we give up and add the triangle.
    for (i = 0; i < 4; i++)
    {
        NiPoint3 kTemp;
        if (!GenerateCameraRay(i, kTemp, akCamRayDirs[i]))
            goto CannotCull;
        
        // compute the intersection of the plane of the triangle with the ray
        const float fNormDot = kNorm.Dot(akCamRayDirs[i]);
        
        // if the rays are in the same direction, no intersection (backfacing)
        if (fNormDot >= -1.0e-5f)
            goto CannotCull;
        
        akCamRayIntersect[i] 
            = kCamRayPt - akCamRayDirs[i] * (fDist / fNormDot);
    }
    
    // now, test each of the frustum intersection points against each of the
    // triangle edges.  If all 4 points are on the same side of a triangle
    // edge, cull the triangle

    // We will compute the cross product of the triangle edge and the vector
    // from one of the edge vertices to the frustum point, and then compute
    // the dot of that vector with the triangle normal.  The sign of this dot
    // is the CCW/CW value
    kE = kV1 - kV0;
    for (i = 0; i < 4; i++)
    {
        // if the dot triple product ((UxV)*W) is positive, then the frustum 
        // point is on the inside of the given triangle edge, and we cannot
        // cull based on this edge.
        if (kE.Cross(akCamRayIntersect[i] - kV1).Dot(kNorm) >= 1.0e-5f)
            break;
    }
    // all points on outside of single edge - culled
    if (i == 4)
        return;

    kE = kV2 - kV1;
    for (i = 0; i < 4; i++)
    {
        // if the dot triple product ((UxV)*W) is positive, then the frustum 
        // point is on the inside of the given triangle edge, and we cannot
        // cull based on this edge.
        if (kE.Cross(akCamRayIntersect[i] - kV2).Dot(kNorm) >= 1.0e-5f)
            break;
    }
    // all points on outside of edge - culled
    if (i == 4)
        return;

    kE = kV0 - kV2;
    for (i = 0; i < 4; i++)
    {
        // if the dot triple product ((UxV)*W) is positive, then the frustum 
        // point is on the inside of the given triangle edge, and we cannot
        // cull based on this edge.
        if (kE.Cross(akCamRayIntersect[i] - kV0).Dot(kNorm) >= 1.0e-5f)
            break;
    }
    // all points on outside of edge - culled
    if (i == 4)
        return;

CannotCull:

    NiPoint3 akTri[3];
    akTri[0] = kV0;
    akTri[1] = kV1;
    akTri[2] = kV2;

    AddShadowTriangle(akTri);
}    

const static NiColorA s_ShadowColor(0.2f, 0.2f, 0.2f, 0.8f);
//---------------------------------------------------------------------------
void ShadowGeometry::AddShadowTriangle(NiPoint3 akV[3])
{
    unsigned short usVertCount = m_spShadowGeometry->GetActiveVertexCount();
    unsigned short usTriCount = m_spShadowGeometry->GetActiveTriangleCount();

    if ((unsigned int)(usVertCount + 3) <= m_uiMaxVertexCount && 
        (unsigned int)(usTriCount + 1) <= m_uiMaxTriangleCount)
    {
		
		//float fTurnAngle = NxPlayMgr::GetHostPlay()->GetKart()->GetTurnAngle()+ NI_PI;
		NiMatrix3 rot = m_spCaster->GetWorldRotate();

		float x, y, z;
		rot.ToEulerAnglesXYZ(x, y, z);

		NiMatrix3 rotZ;
		rotZ.MakeZRotation(z - NI_PI*0.5);

//		LogText("turn angle %f\n", fTurnAngle);

        // For texture coordinates, generate s and t based on point Q's
        // distance along the RIGHT(R) and UP(U) relative to the 
        // projection of the camera's location(P) (assuming non-skewed
        // frustum) onto the ground geometry.
        //
        // s = (R * (Q - P)) / width - 0.5
        // t = (U * (Q - P)) / height - 0.5
        NiPoint2* pkTex = m_spShadowGeometry->GetTextures();
        assert(pkTex);
        for (unsigned int i = 0; i < 3; i++)
        {
            //NiPoint3 kDiff = akV[i] - m_kCenterP;
			
            //pkTex[usVertCount + i].x = (m_spCamera->GetWorldRightVector() *
            //    kDiff) * m_fOOWidth + 0.5f;
            //// Negated direction is due to rendered texture being inverted
            //pkTex[usVertCount + i].y = (-m_spCamera->GetWorldUpVector() *
            //    kDiff) * m_fOOWidth + 0.5f;

			
			NiPoint3 kDiff = akV[i] - m_kCenterP;
			NiPoint3 kRight = m_spCamera->GetWorldRightVector();
			kRight = rotZ * kRight;
			
			pkTex[usVertCount + i].x = ((kRight *
			    kDiff) * m_fOOWidth + 0.5f);// * 1.5f -  0.05f;
			// Negated direction is due to rendered texture being inverted

			NiPoint3 kUp = m_spCamera->GetWorldUpVector();
			kUp = rotZ* kUp;
			pkTex[usVertCount + i].y = ((-kUp*
			    kDiff) * m_fOOWidth + 0.5f);// * 1.5f;

        }

        // We know z-axis is up so we offset upward in z by a few inches
        const float fZOffset = 0.01f;
        akV[0].z += fZOffset;
        akV[1].z += fZOffset;
        akV[2].z += fZOffset;

        NiPoint3* pkVerts = m_spShadowGeometry->GetVertices();
        assert(pkVerts);
        pkVerts[usVertCount] = akV[0];        
        pkVerts[usVertCount + 1] = akV[1];        
        pkVerts[usVertCount + 2] = akV[2];        

        unsigned short usBase = usTriCount * 3;
        unsigned short* pusConnect = m_spShadowGeometry->GetTriList();
        assert(pusConnect);
        pusConnect[usBase] = usVertCount;
        pusConnect[usBase + 1] = usVertCount + 1;
        pusConnect[usBase + 2] = usVertCount + 2;

		NiColorA * pkColors = m_spShadowGeometry->GetColors();

		pkColors[usVertCount] = s_ShadowColor;
		pkColors[usVertCount + 1] = s_ShadowColor;
		pkColors[usVertCount + 2] = s_ShadowColor;

        m_spShadowGeometry->SetActiveVertexCount(usVertCount + 3);
        m_spShadowGeometry->SetActiveTriangleCount(usTriCount + 1);
    }
}
//---------------------------------------------------------------------------
//#endif
