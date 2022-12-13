////////////////////////////////////////////////
//     FileName:    screendraw.h
//     Author:      Fish
//     Date:        2007-04-10
//     Description: 用于把3D对象在屏幕上绘制
////////////////////////////////////////////////


#ifndef __SCREENDRAW_H__
#define __SCREENDRAW_H__

#include <NiZBufferProperty.h>
#include <NiRenderTargetGroup.h>
#include <NiRect.h>
#include <NiCamera.h>
#include <NiAVObject.h>
#include <NiRenderer.h>

class CScreenDraw
{
public:
	enum SCREENDRAW_ALIGN_TYPE
	{
		SCREENDRAW_ALIGN_AUTO,
		SCREENDRAW_ALIGN_WIDTH,
		SCREENDRAW_ALIGN_HEIGHT,
	};

	static void Draw(NiAVObject* pkObject, NiCamera* pkCamera, int x, int y, int w, int h, SCREENDRAW_ALIGN_TYPE aligntype = SCREENDRAW_ALIGN_AUTO)
	{
		if( !pkCamera || !pkObject )
		{
			return;
		}

		if ( w == 0 || h == 0 )
		{
			LogText("zero screendraw width or height, ignore");
			return;
		}
		
		NiRenderer* pkRenderer = NiRenderer::GetRenderer();
		pkRenderer->ClearBuffer(NULL, NiRenderer::CLEAR_ZBUFFER);
		const NiRenderTargetGroup* pkRTGroup = pkRenderer->GetDefaultRenderTargetGroup();
		UINT nWidth = pkRTGroup->GetWidth(0);
		UINT nHeight = pkRTGroup->GetHeight(0);

		NiRect<float> kViewPort = pkCamera->GetViewPort();
		float fRatio = abs(kViewPort.m_right - kViewPort.m_left) / abs(kViewPort.m_top - kViewPort.m_bottom);
		float fBkBufferRatio = nWidth / (float)nHeight;

		NiRect<float> kViewPortNew;
		kViewPortNew.m_left = x / (float)nWidth;
		kViewPortNew.m_right = (x + w) / (float)nWidth;
		kViewPortNew.m_top = (nHeight - y) / (float)nHeight;
		kViewPortNew.m_bottom = (int)(nHeight - y - h) / (float)nHeight;
		NormalizeViewPort(kViewPortNew);
		pkCamera->SetViewPort(kViewPortNew);
		
		NiDrawScene(pkCamera, pkObject, GetMainApp()->GetDefaultCuller());
		
		pkCamera->SetViewPort(kViewPort);
		
	}
	static void NormalizeViewPort(NiRect<float>& kViewPort)
	{
		if ( kViewPort.m_left < 0 )
			kViewPort.m_left = 0;
		if ( kViewPort.m_left > 1 )
			kViewPort.m_left = 1;

		if ( kViewPort.m_right < 0 )
			kViewPort.m_right = 0;
		if ( kViewPort.m_right > 1 )
			kViewPort.m_right = 1;

		if ( kViewPort.m_top < 0 )
			kViewPort.m_top = 0;
		if ( kViewPort.m_top > 1 )
			kViewPort.m_top = 1;

		if ( kViewPort.m_bottom < 0 )
			kViewPort.m_bottom = 0;
		if ( kViewPort.m_bottom > 1 )
			kViewPort.m_left = 1;
	}
	
	static NiNode* CreateTextureObject(const char* pcTextureName, DWORD* pdwWidth = NULL, DWORD* pdwHeight = NULL)
	{
		assert(pcTextureName);

		NiNode* spNode = NiNew NiNode;

		NiPoint3 * pkVertex = NiNew NiPoint3[4];
		pkVertex[0] =  NiPoint3(-0.5, 0.5, 0);
		pkVertex[1] =  NiPoint3(-0.5, -0.5, 0);
		pkVertex[2] =  NiPoint3(0.5, -0.5, 0);
		pkVertex[3] =  NiPoint3(0.5, 0.5, 0);

		NiPoint2 * pkUV = NiNew NiPoint2[4];
		pkUV[0] = NiPoint2(0,0);
		pkUV[1] = NiPoint2(0,1);
		pkUV[2] = NiPoint2(1,1);
		pkUV[3] = NiPoint2(1,0);

		unsigned short* pusConnect = (unsigned short*)NiMalloc(
			sizeof(unsigned short) * (6));
		pusConnect[0] = 0;
		pusConnect[1] = 1;
		pusConnect[2] = 2;
		pusConnect[3] = 0;
		pusConnect[4] = 2;
		pusConnect[5] = 3;

		NiTriShapeDynamicDataPtr spkTriData = NiNew NiTriShapeDynamicData
			(4, pkVertex, 0, NULL, pkUV,
			1,
			NiGeometryData::NBT_METHOD_NONE,
			2,
			pusConnect,
			4, 2);

		NiTriShapePtr spTriShape = NiNew NiTriShape(NiDynamicCast(NiTriShapeData,spkTriData));
		spNode->AttachChild(NiDynamicCast(NiAVObject, spTriShape));

		NiTexturingPropertyPtr spTexProp = NiNew NiTexturingProperty();
		spTexProp->SetApplyMode(NiTexturingProperty::APPLY_MODULATE);
		spTriShape->AttachProperty(spTexProp);
		NiSourceTexturePtr	spTex = NiSourceTexture::Create(pcTextureName);
		spTexProp->SetBaseTexture(spTex);
		if ( pdwWidth )
			*pdwWidth = spTex->GetWidth();
		if ( pdwHeight )
			*pdwHeight = spTex->GetHeight();
		
		NiAlphaPropertyPtr spAlphaPro = NiNew NiAlphaProperty;
		spAlphaPro->SetAlphaBlending(true);
		spAlphaPro->SetSrcBlendMode(NiAlphaProperty::ALPHA_SRCALPHA);
		spAlphaPro->SetDestBlendMode(NiAlphaProperty::ALPHA_INVSRCALPHA);
		spTriShape->AttachProperty(spAlphaPro);

		NiZBufferPropertyPtr spZbufferPro = NiNew NiZBufferProperty;
		spZbufferPro->SetZBufferTest(true);
		spZbufferPro->SetZBufferWrite(true);
		spTriShape->AttachProperty(spZbufferPro);

		NiAmbientLightPtr spLight = NiNew NiAmbientLight;
		spLight->SetDimmer(1.0);
		spLight->SetAmbientColor(NiColor(1.0, 1.0, 1.0));
		spLight->AttachAffectedNode(spNode);
		spNode->AttachChild(spLight);

		NiCameraPtr spCamera = NiNew NiCamera;
		NiPoint3 kEyePos(0, 0, 1.0);
		spCamera->SetWorldTranslate(kEyePos);
		spCamera->SetTranslate(kEyePos);
		spCamera->LookAtWorldPoint(NiPoint3(0, 0, 0), NiPoint3(0, 1, 0));
		spNode->AttachChild(spCamera);

		spNode->UpdateProperties();
		spNode->UpdateEffects();
		spNode->Update(0.0);
		return spNode;
	}
	
};

#endif