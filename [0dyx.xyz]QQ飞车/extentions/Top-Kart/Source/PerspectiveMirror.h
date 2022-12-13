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

#ifndef PERSPECTIVEMIRROR_H
#define PERSPECTIVEMIRROR_H

#include <NiPoint3.h>
#include <NiSmartPointer.h>
#include <NiCullingProcess.h>
#include <NiNode.h>

NiSmartPointer(NiCamera);
NiSmartPointer(NiRenderer);
NiSmartPointer(NiRenderedTexture);
NiSmartPointer(NiRenderTargetGroup);


class PerspectiveMirror : public NiMemObject
{
public:
    PerspectiveMirror(NiRenderer* pkRenderer, NiNode* pkScene, NiNode * pkMirror,
		const NiFixedString & kMirrorNodeName, const NiFixedString & kMirrorGeoName);
    ~PerspectiveMirror();

    NiTexture* GetImage();
    void Update(NiCamera* pkCam);
    void SetPlane(const NiPoint3& kCenter, const NiPoint3& kDir, 
        const NiPoint3& kUp); 
	//void SetProjectMatrix(const NiMatrix3& kMatrix);
    void SetVisible(bool bShow)
	{
		m_bShow = bShow;
	}
	void SetName(const NiFixedString & kName)
	{
		m_kName = kName;
	}
	const NiFixedString & GetName()
	{
		return m_kName;
	}

protected:
    NiCameraPtr m_spCamera;

    NiRendererPtr m_spRenderer;
    NiRenderedTexturePtr m_spRenderedTexture;
    NiRenderTargetGroupPtr m_spRenderTargetGroup;

    //The mirror effect needs to have a camera manager since it is camera
    //relative.

    NiPoint3 m_kCenter;
    NiPoint3 m_kDir; 
    NiPoint3 m_kUp;
    NiPoint3 m_kRight;

	NiMatrix3 m_kProjectMatrix;

    NiNodePtr m_spScene;
	NiNodePtr m_pkMirror;
    NiVisibleArray m_kVisible;
    NiCullingProcess m_kCuller;

	bool			   m_bShow;	
	NiTextureEffectPtr m_spMirrorEnvMap;
	NiFixedString      m_kName;
};

#endif
